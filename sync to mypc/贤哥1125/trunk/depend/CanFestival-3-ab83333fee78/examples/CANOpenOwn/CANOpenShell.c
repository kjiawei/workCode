#include "canfestival.h"
#include "CANOpenShell.h"
#include "ObjDict.h"
#include <signal.h>
#include "config.h"
#include "interactive.h"
#include "states.h"

char BoardBusName[4];
char BoardBaudRate[8];
s_BOARD Board = {BoardBusName, BoardBaudRate};
CO_Data* CANOpenShellOD_Data;

UNS32 ReadSdoAbortCode;
UNS32 ReadSdoData=0;
UNS32 ReadSdoSize=64;

UNS32 WriteSdoAbortCode;
UNS32 WriteSdoData=0;
UNS32 WriteSdoSize=64;

int CanNode_Start(UNS8 nodeid)
{
    UNS8 ret = CanOpen_GetState(nodeid);
    if (ret == Operational) {
        return 0;
    }

    if ((ret != Pre_operational) && (ret != Stopped)) {
        return -1;
    }

    masterSendNMTstateChange(CANOpenShellOD_Data, nodeid, NMT_Start_Node);
    return 0;
}

int CanNode_Stop(UNS8 nodeid)
{
    if ((CanOpen_GetState(nodeid) != Pre_operational) ||
            (CanOpen_GetState(nodeid) != Operational)) {
        return -1;
    }

    masterSendNMTstateChange(CANOpenShellOD_Data, nodeid, NMT_Stop_Node);
}

void CanNode_Reset(UNS8 nodeid)
{
    masterSendNMTstateChange(CANOpenShellOD_Data, nodeid, NMT_Reset_Node);
}

/* Reset all nodes on the network and print message when boot-up*/
void CanNode_Discover()
{
    printf("Wait for Slave nodes bootup...\n\n");
    CanNode_Reset(0x00);
}

int get_info_step = 0;
/* Callback function that check the read SDO demand */
void CheckReadInfoSDO(CO_Data* d, UNS8 nodeid)
{
    UNS32 abortCode;
    UNS32 data=0;
    UNS32 size=64;

    if(getReadResultNetworkDict(CANOpenShellOD_Data, nodeid, &data, &size, &abortCode) != SDO_FINISHED)
        printf("Master : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeid, abortCode);
    else
    {
        /* Display data received */
        switch(get_info_step)
        {
            case 1:
                printf("Device type     : %x\n", data);
                break;
            case 2:
                printf("Vendor ID       : %x\n", data);
                break;
            case 3:
                printf("Product Code    : %x\n", data);
                break;
            case 4:
                printf("Revision Number : %x\n", data);
                break;
        }
    }
    /* Finalize last SDO transfer with this node */
    closeSDOtransfer(CANOpenShellOD_Data, nodeid, SDO_CLIENT);

    GetSlaveNodeInfo(nodeid);
}

/* Retrieve node informations located at index 0x1000 (Device Type) and 0x1018 (Identity) */
void GetSlaveNodeInfo(UNS8 nodeid)
{
    switch(++get_info_step)
    {
        case 1: /* Get device type */
            printf("##################################\n");
            printf("#### Informations for node %x ####\n", nodeid);
            printf("##################################\n");
            readNetworkDictCallback(CANOpenShellOD_Data, nodeid, 0x1000, 0x00, 0, CheckReadInfoSDO, 0);
            break;

        case 2: /* Get Vendor ID */
            readNetworkDictCallback(CANOpenShellOD_Data, nodeid, 0x1018, 0x01, 0, CheckReadInfoSDO, 0);
            break;

        case 3: /* Get Product Code */
            readNetworkDictCallback(CANOpenShellOD_Data, nodeid, 0x1018, 0x02, 0, CheckReadInfoSDO, 0);
            break;

        case 4: /* Get Revision Number */
            readNetworkDictCallback(CANOpenShellOD_Data, nodeid, 0x1018, 0x03, 0, CheckReadInfoSDO, 0);
            break;

        case 5: /* Print node info */
            get_info_step = 0;
    }
}

/* Callback function that check the read SDO demand */
void CheckReadSDO(CO_Data* d, UNS8 nodeid)
{
    if(getReadResultNetworkDict(CANOpenShellOD_Data, nodeid,
                &ReadSdoData, &ReadSdoSize, &ReadSdoAbortCode) != SDO_FINISHED)
        printf("\nResult : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeid, ReadSdoAbortCode);
    else
        printf("\nResult : %x\n", ReadSdoData);

    /* Finalize last SDO transfer with this node */
    closeSDOtransfer(CANOpenShellOD_Data, nodeid, SDO_CLIENT);
}

/* Read a slave node object dictionary entry */
int ReadDeviceSdoEntry(int nodeid, int index, int subindex)
{
	EnterMutex();
    readNetworkDictCallback(CANOpenShellOD_Data,
            (UNS8)nodeid,
            (UNS16)index,
            (UNS8)subindex,
            (UNS8)0,
            CheckReadSDO, 0);
	LeaveMutex();
}

// read ok: 0, Otherwise:-1
int ReadDeviceSdoIsOk()
{

    return -1;
}

unsigned int ReadDeviceSdoData()
{
    return ReadSdoData;
}

void ReadHeartBeat(int nodeid)
{
    ReadDeviceSdoEntry(nodeid, 0x1017, 0);
}

/* Callback function that check the write SDO demand */
void CheckWriteSDO(CO_Data* d, UNS8 nodeid)
{
    if(getWriteResultNetworkDict(CANOpenShellOD_Data, nodeid, &WriteSdoAbortCode) != SDO_FINISHED)
        printf("\nResult : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n",
                nodeid, WriteSdoAbortCode);
    else
        printf("\nSend data OK\n");

    /* Finalize last SDO transfer with this node */
    closeSDOtransfer(CANOpenShellOD_Data, nodeid, SDO_CLIENT);
}

/* Write a slave node object dictionnary entry */
int WriteDeviceSdoEntry(int nodeid, int index, int subindex, int size, unsigned int data)
{
	EnterMutex();
    writeNetworkDictCallBack(CANOpenShellOD_Data, nodeid, index, subindex, size, 0, &data, CheckWriteSDO, 0);
	LeaveMutex();
}

// write ok: 0, Otherwise:-1
int WriteDeviceSdoIsOk()
{
    return -1;
}


void CANOpenShellOD_post_SlaveBootup(CO_Data* d, UNS8 nodeid)
{
    printf("Slave %x boot up\n", nodeid);
}

/***************************  CALLBACK FUNCTIONS  *****************************************/
void CANOpenShellOD_initialisation(CO_Data* d)
{
    printf("Node_initialisation\n");
}

void CANOpenShellOD_preOperational(CO_Data* d)
{
    printf("Node_preOperational\n");
}

void CANOpenShellOD_operational(CO_Data* d)
{
    printf("Node_operational\n");
}

void CANOpenShellOD_stopped(CO_Data* d)
{
    printf("Node_stopped\n");
}

void CANOpenShellOD_post_sync(CO_Data* d)
{
    //printf("Master_post_sync\n");
}

void CANOpenShellOD_post_TPDO(CO_Data* d)
{
    //printf("Master_post_TPDO\n");
}

/***************************  INITIALISATION  **********************************/
void Init(CO_Data* d, UNS32 id)
{
    if(Board.baudrate)
    {
        /* Init node state*/
        //setState(CANOpenShellOD_Data, Initialisation);
        setState(CANOpenShellOD_Data, Operational);
    }
}

/***************************  CLEANUP  *****************************************/
void Exit(CO_Data* d, UNS32 nodeid)
{
    if(strcmp(Board.baudrate, "none"))
    {
        /* Reset all nodes on the network */
        masterSendNMTstateChange(CANOpenShellOD_Data, 0 , NMT_Reset_Node);

        /* Stop master */
        setState(CANOpenShellOD_Data, Stopped);
    }
}


/*
 *  remake the new function
 *
 */

int NodeMaster_Init(void)
{
    strcpy(Board.busname, CANOPEN_DEV_NAME);
    strcpy(Board.baudrate, CANOPEN_DEV_BAUD_RATE);

    //CANOpenShellOD_Data = &CANOpenShellMasterOD_Data;
    CANOpenShellOD_Data = &Master_Data;

    /* Load can library */
    LoadCanDriver(CANOPEN_LIBRARY_PATH);

    /* Define callback functions */
    CANOpenShellOD_Data->initialisation = CANOpenShellOD_initialisation;
    CANOpenShellOD_Data->preOperational = CANOpenShellOD_preOperational;
    CANOpenShellOD_Data->operational = CANOpenShellOD_operational;
    CANOpenShellOD_Data->stopped = CANOpenShellOD_stopped;
    CANOpenShellOD_Data->post_sync = CANOpenShellOD_post_sync;
    CANOpenShellOD_Data->post_TPDO = CANOpenShellOD_post_TPDO;
    CANOpenShellOD_Data->post_SlaveBootup=CANOpenShellOD_post_SlaveBootup;

    /* Open the Peak CANOpen device */
    if(!canOpen(&Board,CANOpenShellOD_Data))
        return -1;

    /* Defining the node Id */
    setNodeId(CANOpenShellOD_Data, CANOPEN_MASTER_NODE_ID);
    /* Start Timer thread */
    StartTimerLoop(&Init);
    return 0;
}

int CanOpen_Master_Init(void)
{
    InterData_Init();
    TimerInit();
    if (NodeMaster_Init() != 0) {
        return -1;
    }

    return 0;
}

int CanOpen_Master_Quit(void)
{
    InterData_Quit();

    // Stop timer thread
    StopTimerLoop(&Exit);

    /* Close CAN board */
    canClose(CANOpenShellOD_Data);

    TimerCleanup();

    return 0;
}


e_nodeState CanOpen_GetState(UNS8 nodeId)
{
    return getNodeState(CANOpenShellOD_Data, nodeId);
}
