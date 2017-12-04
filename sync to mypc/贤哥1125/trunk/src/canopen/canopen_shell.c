#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "canopen_shell.h"
#include "middle.h"
#ifdef OS_LINUX
#include "ObjDict.h"
#include "canfestival.h"
#include "interactive.h"
#endif




//本文件是在canopen 的源码/examples/canopenshell目录下的例程文件CANOpenShell修改而成#/
#ifdef HW_ARM
static pthread_mutex_t can_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

char BoardBusName[4]; //总线名称#/
char BoardBaudRate[8]; //设置的总线波特率#/

char CAN_MUTEX_STATE=0;   //CAN SDO 锁状态#/

struct sdo_sendstate{
  unsigned char state;   //对应设备节点的SDO发送状态#/
  unsigned char date[8]; //对应接收到设备节点发送回来的数据#/
}GA_SDOSTATE_ID[8];   //各节点SDO命令的应答状态，判断SDO是否接收成功#/

#ifdef OS_LINUX
s_BOARD Board = {BoardBusName, BoardBaudRate};
CO_Data* CANOpenShellOD_Data; //canopen的设备结构体#/
#endif

//整体读写SDO的状态标志#/
unsigned int ReadSdoAbortCode;//读SDO时的异常编号#/
unsigned int ReadSdoData=0; //已发送读请求的SDO索引号#/
unsigned int ReadSdoSize=64;//已读成功的SDO数据长度#/

unsigned int WriteSdoAbortCode;
unsigned int WriteSdoData=0;
unsigned int WriteSdoSize=64;

enum EventType {
    /*EVENT_NULL*/
    EVENT_PDO,
    EVENT_SDO,
    EVENT_SDO_Pre,
    EVENT_SDO_READ,
    EVENT_SDO_WRITE,
    EVENT_SDO_RETURN,

    EVENT_NODE_RECEIVE,

    /* ... */
    EVENT_ERROR
};
enum EventType EventSdoState=EVENT_SDO_Pre;

//切换指定ID号的设备节点到operational状态，以进行正常的通信操作#/
int CanNode_Start(unsigned char nodeid)
{
        return 0;

#ifdef HW_ARM //开发板才编译#/
    int i=5;

   // return 0;

    unsigned char ret;
    while (i-- > 0) {
        ret = CanOpen_GetState(nodeid);
        if (ret == Operational) {
            return 0;
        }

        if ((ret == Pre_operational) || (ret == Stopped)) {
            masterSendNMTstateChange(CANOpenShellOD_Data, nodeid, NMT_Start_Node);
            usleep(400000);
        } else {
            return -1;
        }
    }
    return -1;
#else
    nodeid = nodeid;
#endif
    return 0;
}

//切换指定ID号的设备节点到Stopped状态，这时节点不会对总线干扰#/
int CanNode_Stop(unsigned char nodeid)
{
        return 0;

#ifdef HW_ARM
    unsigned char ret = CanOpen_GetState(nodeid);
    if ((ret == Pre_operational) || (ret == Operational)) {
        masterSendNMTstateChange(CANOpenShellOD_Data, nodeid, NMT_Stop_Node);
    }
#else
    nodeid = nodeid;
#endif
    return 0;
}

//切换指定ID号的设备节点到initialisation 状态，这时节点进行初始化操作#/
void CanNode_Reset(unsigned char nodeid)
{
        return;

#ifdef HW_ARM
    masterSendNMTstateChange(CANOpenShellOD_Data, nodeid, NMT_Reset_Node);
#else
    nodeid = nodeid;
#endif
}

/* Reset all nodes on the network and print message when boot-up*/
void CanNode_Discover()
{
        return;

    printf("Wait for Slave nodes bootup...\n\n");
#ifdef HW_ARM
    CanNode_Reset(0x00);
#endif
}

int get_info_step = 0;

#ifdef OS_LINUX
/* Callback function that check the read SDO demand */
//发送SDO读取请求后，检查是否读取成功#/
void CheckReadInfoSDO(CO_Data* d, unsigned char nodeid)
{
    unsigned int abortCode;
    unsigned int data=0;
    unsigned int size=64;

    d = d;

        return;

    //先通过readNetworkDict等函数发送一个SDO读取请求#/
    //然后通过getReadResultNetworkDict检测这个SDO是否读取成功#/
    //nodeid：要读取的设备ID data：返回之前读请求的SDO索引号#/
    //size：返回读该SDO成功的数据长度 abortCode：异常编号#/
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
#endif

/* Retrieve node informations located at index 0x1000 (Device Type) and 0x1018 (Identity) */
//读取相应设备的信息#/
void GetSlaveNodeInfo(unsigned char nodeid)
{
        return;

    switch(++get_info_step)
    {
        case 1: /* Get device type */
            printf("##################################\n");
            printf("#### Informations for node %x ####\n", nodeid);
            printf("##################################\n");
#ifdef OS_LINUX
            readNetworkDictCallback(CANOpenShellOD_Data, nodeid, 0x1000, 0x00, 0, CheckReadInfoSDO, 0);
#endif
            break;

        case 2: /* Get Vendor ID */
#ifdef OS_LINUX
            readNetworkDictCallback(CANOpenShellOD_Data, nodeid, 0x1018, 0x01, 0, CheckReadInfoSDO, 0);
#endif
            break;

        case 3: /* Get Product Code */
#ifdef OS_LINUX
            readNetworkDictCallback(CANOpenShellOD_Data, nodeid, 0x1018, 0x02, 0, CheckReadInfoSDO, 0);
#endif
            break;

        case 4: /* Get Revision Number */
#ifdef OS_LINUX
            readNetworkDictCallback(CANOpenShellOD_Data, nodeid, 0x1018, 0x03, 0, CheckReadInfoSDO, 0);
#endif
            break;

        case 5: /* Print node info */
            get_info_step = 0;
    }
}

#ifdef OS_LINUX
/* Callback function that check the read SDO demand */
//每次发送SDO读请求后，可使用该函数进行检查处理#/
void CheckReadSDO(CO_Data* d, unsigned char nodeid)
{

    d = d;

        return;

    if(getReadResultNetworkDict(CANOpenShellOD_Data, nodeid,
                &ReadSdoData, &ReadSdoSize, &ReadSdoAbortCode) != SDO_FINISHED)
        printf("\nResult : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeid, ReadSdoAbortCode);

    /* Finalize last SDO transfer with this node */
    closeSDOtransfer(CANOpenShellOD_Data, nodeid, SDO_CLIENT);
    EventSdoState = EVENT_SDO_RETURN;
}
#endif

/* Read a slave node object dictionary entry */
//读取具体设备中的具体SDO#/
int ReadDeviceSdoEntry(int nodeid, int index, int subindex)
{

//    UNS8 i;
    time_t timep;
    time(&timep);

    printf("ReadSDO id =%d,index =%X,sub=%d  ", nodeid,index,subindex);

    printf("  time:%s \n",asctime(localtime(&timep)));

#ifdef HW_ARM
    UNS8 crc;
    UNS16 indextmp;
    Message pdomsg;

    indextmp=(UNS16)index&0xffff;
    pdomsg.data[0]=0x40;
    pdomsg.data[1]=(UNS8)(indextmp&0xff);
    pdomsg.data[2]=(UNS8)(indextmp>>8&0xff);
    pdomsg.data[3]=subindex;
    pdomsg.data[4]=0;
    pdomsg.data[5]=0;
    pdomsg.data[6]=0;
    pdomsg.data[7]=0;
    pdomsg.len=8;
    pdomsg.cob_id=nodeid+0x600;
    pdomsg.rtr=NOT_A_REQUEST;    

    pdomsg.data[1]=0;
    crc=cal_crc(pdomsg.data, 8);
    pdomsg.data[1]=crc;

    //发送SDO时的状态标志设置#/
    SetSendSDO_State(nodeid,DE_SDO_SENDING);

    //-------跳过了canfestival#/
//    return canSend(CANOpenShellOD_Data->canHandle,&pdomsg);
    //----HJX实现直接操作#/
    return CanSend_Handle(CAN_FD_FLAG, &pdomsg);

    //20151116修改#/
/*---------------------HJX屏蔽，为了解决有时读不成功#/
    if ((EventSdoState != EVENT_SDO_RETURN) && (EventSdoState != EVENT_SDO_Pre)) {
        return -1;
    }
*/
    //EnterMutex();

//    pthread_mutex_lock(&can_mutex);

    EventSdoState = EVENT_SDO_READ;
    readNetworkDictCallback(CANOpenShellOD_Data,
            (unsigned char)nodeid,
            (unsigned short)index,
            (unsigned char)subindex,
            (unsigned char)0,
             CheckReadSDO, 0);
    //LeaveMutex();

//    pthread_mutex_unlock(&can_mutex);

#else
    nodeid = nodeid;
    index = index;
    subindex = subindex;
#endif
    return 0;
}

// read ok: 0, Otherwise:-1
int ReadDeviceSdoIsOk()
{
    if (EventSdoState != EVENT_SDO_RETURN) {
        return -1;
    }
    return 0;
}

unsigned int ReadDeviceSdoData()
{
    EventSdoState = EVENT_SDO_Pre;
    return ReadSdoData;
}

void ReadHeartBeat(int nodeid)
{
    ReadDeviceSdoEntry(nodeid, 0x1017, 0);
}

#ifdef OS_LINUX
/* Callback function that check the write SDO demand */
void CheckWriteSDO(CO_Data* d, unsigned char nodeid)
{
    d = d;

        return;
    if(getWriteResultNetworkDict(CANOpenShellOD_Data, nodeid, &WriteSdoAbortCode) != SDO_FINISHED)
    {
        printf("\nResult : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n",
                nodeid, WriteSdoAbortCode);
        //closeSDOtransfer(CANOpenShellOD_Data, nodeid, SDO_CLIENT);
        //return;
     }
    /* Finalize last SDO transfer with this node */
    //closeSDOtransfer(CANOpenShellOD_Data, nodeid, SDO_CLIENT);
    //EventSdoState = EVENT_SDO_RETURN;
}
#endif

/* Write a slave node object dictionnary entry */
//对具体设备写其特定的SDO#/
int WriteDeviceSdoEntry(int nodeid, int index, int subindex, int size, void *data)
{
    UNS8 i;
    time_t timep;
    time(&timep);

    printf("SDO id =%d,index =%X,sub=%d  ", nodeid,index,subindex);

    unsigned char * chp=(unsigned char *)data;

    for (i=0; i < 4; i++)
    {
        printf("%02X ",chp[i]);
    }

    printf("  time:%s \n",asctime(localtime(&timep)));

#ifdef HW_ARM
    UNS16 indextmp;
    Message pdomsg;
    UNS8 crc=0;

    indextmp=(UNS16)index&0xffff;
    pdomsg.data[0]=0x23;
    pdomsg.data[1]=(UNS8)(indextmp&0xff);
    pdomsg.data[2]=(UNS8)(indextmp>>8&0xff);
    pdomsg.data[3]=subindex;
    pdomsg.data[4]=chp[0];
    pdomsg.data[5]=chp[1];
    pdomsg.data[6]=chp[2];
    pdomsg.data[7]=chp[3];
    pdomsg.len=8;
    pdomsg.cob_id=nodeid+0x600;
    pdomsg.rtr=NOT_A_REQUEST;

    pdomsg.data[1]=0;
    crc=cal_crc(pdomsg.data, 8);
    pdomsg.data[1]=crc;

    //发送SDO时的状态标志设置#/
    SetSendSDO_State(nodeid,DE_SDO_SENDING);

    //-------跳过了canfestival#/
//    return canSend(CANOpenShellOD_Data->canHandle,&pdomsg);
    //----HJX实现直接操作#/
    return CanSend_Handle(CAN_FD_FLAG, &pdomsg);

    int trc;
    if ((EventSdoState != EVENT_SDO_RETURN) && (EventSdoState != EVENT_SDO_Pre)) {
        return -1;
    }

    //EnterMutex();

    pthread_mutex_lock(&can_mutex);
    CAN_MUTEX_STATE=1;

    EventSdoState = EVENT_SDO_WRITE;
    writeNetworkDictCallBack(CANOpenShellOD_Data, nodeid, index, subindex, size, 0, data, CheckWriteSDO, 0);
    trc=0;
//    while(writeNetworkDictCallBack(CANOpenShellOD_Data, nodeid, index, subindex, size, 0, data, CheckWriteSDO, 0)
//           !=0 )//&& trc<5
//    {
//     //usleep(200000);
//     sleep(1);
//     //trc++;
//    }
    closeSDOtransfer(CANOpenShellOD_Data, nodeid, SDO_CLIENT);
    EventSdoState = EVENT_SDO_RETURN;

    if(EventSdoState!=EVENT_SDO_RETURN){
    EventSdoState = EVENT_SDO_RETURN;
    pthread_mutex_unlock(&can_mutex);
    CAN_MUTEX_STATE=0;
    return -1;
    }

    pthread_mutex_unlock(&can_mutex);
    CAN_MUTEX_STATE=0;
    //LeaveMutex();

#else
    nodeid = nodeid;
    index = index;
    subindex = subindex;
    size = size;
    data = data;
#endif
    return 0;
}

// write ok: 0, Otherwise:-1
int WriteDeviceSdoIsOk()
{
    if (EventSdoState != EVENT_SDO_RETURN) {
        return -1;
    }

    return 0;
}

#ifdef OS_LINUX
void CANOpenShellOD_post_SlaveBootup(CO_Data* d, unsigned char nodeid)
{
    d = d;
    printf("Slave %x boot up\n", nodeid);
}

/***************************  CALLBACK FUNCTIONS  *****************************************/
void CANOpenShellOD_initialisation(CO_Data* d)
{
    d = d;
    printf("Node_initialisation\n");
}

void CANOpenShellOD_preOperational(CO_Data* d)
{
    d = d;
    printf("Node_preOperational\n");
}

void CANOpenShellOD_operational(CO_Data* d)
{
    d = d;
    printf("Node_operational\n");
}

void CANOpenShellOD_stopped(CO_Data* d)
{
    d = d;
    printf("Node_stopped\n");
}

void CANOpenShellOD_post_sync(CO_Data* d)
{
    d = d;
    //printf("Master_post_sync\n");
}

void CANOpenShellOD_post_TPDO(CO_Data* d)
{
    d = d;
    //printf("Master_post_TPDO\n");
}

/***************************  INITIALISATION  **********************************/
void Init(CO_Data* d, unsigned int id)
{
    d = d;
    id = id;

        return;

    if(Board.baudrate)
    {
        /* Init node state*/
        //setState(CANOpenShellOD_Data, Initialisation);
        //设置本设备进入Operational状态，可以正常通信#/
        setState(CANOpenShellOD_Data, Operational);
    }
}

/***************************  CLEANUP  *****************************************/
void Exit(CO_Data* d, unsigned int nodeid)
{
    d = d;
    nodeid = nodeid;

        return;

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
//本节点（主设备）的canopen协议栈的初始化操作#/
/*--------------------------初始化操作流程如下
 * 1.先初始化配置canopen设备结构体 CO_Data
 * 2.然后根据波特率和设备结构体，通过canOpen函数进行协议栈初始化
 * 3.通过setNodeId设置设备的节点ID号
 * 4.调用 StartTimerLoop 开始canopen的工作

*/
int NodeMaster_Init(void)
{
    CO_Data null;

    return 0;

    strcpy(Board.busname, CANOPEN_DEV_NAME);
    strcpy(Board.baudrate, CANOPEN_DEV_BAUD_RATE);

    //CANOpenShellOD_Data = &CANOpenShellMasterOD_Data;
    //CO_Data 是CANopen中的设备结构，设备的状态和操作均在这个结构有表现#/
    CANOpenShellOD_Data = &Master_Data;

    /* Load can library */
    //加载can设备的canopen接口库#/
    LoadCanDriver(CANOPEN_LIBRARY_PATH);

    /* Define callback functions */
    //设置canopen在切换到如下各种状态时，进行的操作#/
    CANOpenShellOD_Data->initialisation = CANOpenShellOD_initialisation;
    CANOpenShellOD_Data->preOperational = CANOpenShellOD_preOperational;
    CANOpenShellOD_Data->operational = CANOpenShellOD_operational;
    CANOpenShellOD_Data->stopped = CANOpenShellOD_stopped;
    CANOpenShellOD_Data->post_sync = CANOpenShellOD_post_sync;
    CANOpenShellOD_Data->post_TPDO = CANOpenShellOD_post_TPDO;
    CANOpenShellOD_Data->post_SlaveBootup=CANOpenShellOD_post_SlaveBootup;

    /* Open the Peak CANOpen device */
    //初始化CO_Data结构，并根据相应参数打开CAN设备#/
    if(!canOpen(&Board,CANOpenShellOD_Data))
        return -1;

    /* Defining the node Id */
    setNodeId(CANOpenShellOD_Data, CANOPEN_MASTER_NODE_ID);
    /* Start Timer thread */
    //开始CANopen的处理线程，canopen的大部分处理均于时间息息相关#/
    //当出现故障、重启等状态时，会调用回调函数Init#/
//    StartTimerLoop(&Init); //李喜进修改#/
    Init(&null,0);
    return 0;
}
#endif

//-------跳过了canfestival#/
/*--
int CanOpen_Master_Init(void)
{
#ifdef _MODULE_CAN
//绑定回调函数，通过该函数实现CANOpen每收发到一个CAN的数据包#/
//则通过该函数发送给data_manage_can.c的处理函数，进行canopen接收处理#/
    InterData_Init(&MessageSignalMiddle);
    TimerInit();
    if (NodeMaster_Init() != 0) {
        return -1;
    }
#endif
    return 0;
}
*/
//----HJX实现直接操作#/
int CanOpen_Master_Init(void)
{
   unsigned char i;
   for(i=0;i<8;i++)
   {
       GA_SDOSTATE_ID[i].state=0;
   }

#ifdef _MODULE_CAN
    //打开can设备#/
    CAN_FD_FLAG=CanOpen_Handle(CANOPEN_DEV_BAUD_RATE);
    if(CAN_FD_FLAG>0)
         printf("CANOPEN SUCCESS!\n");
    return CAN_FD_FLAG;
#endif
    return 0;
}

//终止本设备的canopen协议栈工作#/
int CanOpen_Master_Quit(void)
{
    //-------跳过了canfestival#/
/*
#ifdef _MODULE_CAN
    // Stop timer thread
    StopTimerLoop(&Exit);

    // Close CAN board#/
    canClose(CANOpenShellOD_Data);

    TimerCleanup();
#endif
*/
    //----HJX实现直接操作#/
#ifdef _MODULE_CAN
    CanClose_Handle(CAN_FD_FLAG);
    CAN_FD_FLAG=0;
#endif

    return 0;
}

//获取特定设备的canopen状态#/
unsigned char CanOpen_GetState(unsigned char nodeId)
{
        return 0;
#ifdef _MODULE_CAN
    return getNodeState(CANOpenShellOD_Data, nodeId);
#else
    nodeId = nodeId;
    return Pre_operational;
#endif
}


/******************************************************************************
 PDO Trinsmit Function
 ******************************************************************************/
//发送本设备所有T-PDO的数据#/
unsigned char TrinsmitPDO(void)
{

        return 0;
#ifdef _MODULE_CAN
    return sendPDOevent (CANOpenShellOD_Data);
#else
    return 0;
#endif
}

//把要发送的PDO内容放到索引为2013的对象中，而T-PDO1则是映射到2013的#/
unsigned char setPDOContent(unsigned char *pData)
{

        return 0;
#ifdef _MODULE_CAN

    const indextable *ptrTable;
    unsigned int errorCode;
    ODCallback_t *Callback;
    unsigned char i;

    if (pData == NULL)
        return -1;

    //获取索引为2013的内容，并且修改为要发送的#/
    ptrTable = (*CANOpenShellOD_Data->scanIndexOD)(0x2013, &errorCode, &Callback);
    for(i = 1;i < 9; i++,pData++) { //8个字节
        *((unsigned char *)(ptrTable->pSubindex[i].pObject)) = *pData;
    }

    return 0;

#else

    pData = pData;
    return 0;

#endif
}

//----HJX实现直接操作#/
//发送本设备所有T-PDO的数据#/
unsigned char HJX_TrinsmitPDO(unsigned char *pData)
{
#ifdef HW_ARM

//    printf("Send PDO data!\n");

    Message pdomsg;

    pdomsg.data[0]=pData[0];
    pdomsg.data[1]=pData[1];
    pdomsg.data[2]=pData[2];
    pdomsg.data[3]=pData[3];
    pdomsg.data[4]=pData[4];
    pdomsg.data[5]=pData[5];
    pdomsg.data[6]=pData[6];
    pdomsg.data[7]=pData[7];
    pdomsg.len=8;
    pdomsg.cob_id=0x181;
    pdomsg.rtr=NOT_A_REQUEST;

    //----HJX实现直接操作#/
    return CanSend_Handle(CAN_FD_FLAG, &pdomsg);

    #endif

    pData=pData;
    return 0;
}

//发送SDO时的状态标志设置#/
void SetSendSDO_State(int id,unsigned char state)
{
    switch(id)
    {
    case CANID_UI:
        GA_SDOSTATE_ID[0].state=state;
        memset(GA_SDOSTATE_ID[0].date,0,8);
        break;
    case CANID_MASTERCTRL:
        GA_SDOSTATE_ID[1].state=state;
        memset(GA_SDOSTATE_ID[1].date,0,8);
        break;
    case CANID_MONITOR:
        GA_SDOSTATE_ID[2].state=state;
        memset(GA_SDOSTATE_ID[2].date,0,8);
        break;
    case CANID_POWER:
        GA_SDOSTATE_ID[3].state=state;
        memset(GA_SDOSTATE_ID[3].date,0,8);
        break;
    case CANID_BLOOD_PUMP:
        GA_SDOSTATE_ID[4].state=state;
        memset(GA_SDOSTATE_ID[4].date,0,8);
        break;
    case CANID_BLOOD_PUMP2:
        GA_SDOSTATE_ID[5].state=state;
        memset(GA_SDOSTATE_ID[5].date,0,8);
        break;
    case CANID_HEPARIN_PUMP:
        GA_SDOSTATE_ID[6].state=state;
        memset(GA_SDOSTATE_ID[6].date,0,8);
        break;
    }
}

//接收SDO时的状态标志设置，并且保存接收数据#/
void SetRecSDO_State(int id,unsigned char state,void *ptr)
{
    switch(id)
    {
    case CANID_UI:
        GA_SDOSTATE_ID[0].state=state;
        memcpy(GA_SDOSTATE_ID[0].date,ptr,8);
        break;
    case CANID_MASTERCTRL:
        GA_SDOSTATE_ID[1].state=state;
        memcpy(GA_SDOSTATE_ID[1].date,ptr,8);
        break;
    case CANID_MONITOR:
        GA_SDOSTATE_ID[2].state=state;
        memcpy(GA_SDOSTATE_ID[2].date,ptr,8);
        break;
    case CANID_POWER:
        GA_SDOSTATE_ID[3].state=state;
        memcpy(GA_SDOSTATE_ID[3].date,ptr,8);
        break;
    case CANID_BLOOD_PUMP:
        GA_SDOSTATE_ID[4].state=state;
        memcpy(GA_SDOSTATE_ID[4].date,ptr,8);
        break;
    case CANID_BLOOD_PUMP2:
        GA_SDOSTATE_ID[5].state=state;
        memcpy(GA_SDOSTATE_ID[5].date,ptr,8);
        break;
    case CANID_HEPARIN_PUMP:
        GA_SDOSTATE_ID[6].state=state;
        memcpy(GA_SDOSTATE_ID[6].date,ptr,8);
        break;
    }
}

//获取发送SDO的状态标志设置，并读取接收数据#/
unsigned char GetSDO_State(int id,void *gptr)
{
    switch(id)
    {
    case CANID_UI:
        memcpy(gptr,GA_SDOSTATE_ID[0].date,8);
        return GA_SDOSTATE_ID[0].state;
    case CANID_MASTERCTRL:
        memcpy(gptr,GA_SDOSTATE_ID[1].date,8);
        return GA_SDOSTATE_ID[1].state;
    case CANID_MONITOR:
        memcpy(gptr,GA_SDOSTATE_ID[2].date,8);
        return GA_SDOSTATE_ID[2].state;
    case CANID_POWER:
        memcpy(gptr,GA_SDOSTATE_ID[3].date,8);
        return GA_SDOSTATE_ID[3].state;
    case CANID_BLOOD_PUMP:
        memcpy(gptr,GA_SDOSTATE_ID[4].date,8);
        return GA_SDOSTATE_ID[4].state;
    case CANID_BLOOD_PUMP2:
        memcpy(gptr,GA_SDOSTATE_ID[5].date,8);
        return GA_SDOSTATE_ID[5].state;
    case CANID_HEPARIN_PUMP:
        memcpy(gptr,GA_SDOSTATE_ID[6].date,8);
        return GA_SDOSTATE_ID[6].state;
    }

    return 0;
}


//-----------------------------跳过CANfestival，实现直接操作CAN驱动层-----------------------------------------//

#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>

/* CAN ioctl magic number */
#define CAN_IOC_MAGIC 'C'
#define CAN_IOCTRESET			_IO(CAN_IOC_MAGIC, 0)			/* reset device */
#define CAN_IOCTWAKEUP			_IO(CAN_IOC_MAGIC, 1)			/* wakup device */
#define CAN_IOCTNORMALMODE		_IO(CAN_IOC_MAGIC, 2)			/* go normal mode */
#define CAN_IOCTLOOPBACKMODE	_IO(CAN_IOC_MAGIC, 3)			/* go loopback mode */
#define CAN_IOCTLISTENONLYMODE	_IO(CAN_IOC_MAGIC, 4)			/* go listen only mode */
#define CAN_IOCTSLEEPMODE		_IO(CAN_IOC_MAGIC, 5)			/* go sleep mode */
#define CAN_IOCSRATE			_IOW(CAN_IOC_MAGIC, 6, int)		/* set baud rate */
#define CAN_IOCSFILTER			_IOW(CAN_IOC_MAGIC, 7, long)	/* set receive filter */
#define CAN_IOCGRATE			_IOR(CAN_IOC_MAGIC, 8, int)		/* get baud rate */
#define CAN_IOCGFILTER			_IOR(CAN_IOC_MAGIC, 9, long)	/* get receive filter */

#define CAN_NORMALMODE          1   /* go normal mode */
#define CAN_LOOPBACKMODE        2   /* go loopback mode */
#define CAN_LISTENONLYMODE      3   /* go listen only mode */
#define CAN_SLEEPMODE           4   /* go sleep mode */

/* Definitions to use for canmsg_tx and canfilt_t flags */
#define MSG_RTR   (1<<0)
#define MSG_OVR   (1<<1)
#define MSG_EXT   (1<<2)
#define MSG_LOCAL (1<<3)

struct canmsg_header {
    unsigned id:11;
    unsigned srr:1;
    unsigned ide:1;
    unsigned eid:18;
    unsigned rtr:1;
    unsigned rb1:1;
    unsigned rb0:1;
    unsigned length:4;
};

struct canmsg_tx{
    struct canmsg_header header;
    unsigned char data[8];
};

int CAN_FD_FLAG=0; //CAN设备打开文件描述符#/

/*半字节查表法 X8+X2+X1+1   key = 0x07*/
const unsigned char CRC8Table[16] = {
    0x00,0x07,0x0e,0x09,
    0x1c,0x1b,0x12,0x15,
    0x38,0x3f,0x36,0x31,
    0x24,0x23,0x2a,0x2d
};

unsigned char cal_crc(unsigned char *ptr, unsigned char len)
{
    //半字节查表
    unsigned char crc = 0;
    unsigned char da;
    while(len-- != 0)
    {
        da = crc / 16;							/* 暂存CRC 的高四位 */
        crc <<= 4;								/* CRC 右移4 位，相当于取CRC 的低12 位）*/
        crc ^= CRC8Table[da ^ (*ptr / 16)];		/* CRC 的高4 位和本字节的前半字节相加后查表计算CRC，然后加上上一次CRC的余数 */
        da = crc / 16;							/* 暂存CRC 的高4 位 */
        crc <<= 4;								/* CRC 右移4 位， 相当于CRC 的低12 位） */
        crc ^= CRC8Table[da ^ (*ptr & 0x0f)];	/* CRC 的高4位和本字节的后半字节相加后查表计算CRC，然后再加上上一次CRC的余数 */
        ptr++;
    }
    return(crc);
}


//设置波特率#/
int TranslateBaudeRate(char* optarg)
{
    if(!strcmp( optarg, "1M")) return 1000000;
    if(!strcmp( optarg, "500K")) return 500000;
    if(!strcmp( optarg, "250K")) return 250000;
    if(!strcmp( optarg, "125K")) return 125000;
    if(!strcmp( optarg, "100K")) return 100000;
    if(!strcmp( optarg, "50K")) return 50000;
    if(!strcmp( optarg, "20K")) return 20000;
    if(!strcmp( optarg, "10K")) return 10000;
    if(!strcmp( optarg, "none")) return 0;
    return 0x0000;
}


UNS8 canModeset_driver(int fd0, int mode)
{
    switch (mode) {
        case CAN_NORMALMODE:
            ioctl((int)fd0, CAN_IOCTNORMALMODE, 0);     /* go normal mode */
            break;
        case CAN_LOOPBACKMODE:
            ioctl((int)fd0, CAN_IOCTLOOPBACKMODE, 0);   /* go loopback mode */
            break;
        case CAN_LISTENONLYMODE:
            ioctl((int)fd0, CAN_IOCTLISTENONLYMODE, 0); /* go listen only mode */
            break;
        case CAN_SLEEPMODE:
            ioctl((int)fd0, CAN_IOCTSLEEPMODE, 0);      /* go sleep mode */
            break;
    }
    return 1;
}

//打开can设备#/
int CanOpen_Handle(char* baud)
{
    int o_flags = 0;
    int baudrate;
    int ret = -1;
    int fd=-1;

    /*o_flags = O_NONBLOCK;*/

    fd = open("/dev/can0", O_RDWR|o_flags);
    if(fd < 0) {
        printf("Can Open fail!\n");
        return 0;
    }

    // 设置波特比率#/
    baudrate = TranslateBaudeRate(baud);

    if(baudrate == 0)
        return 0;

    ioctl(fd, CAN_IOCSRATE, baudrate);
    ioctl(fd, CAN_IOCGRATE, &ret);
    if(ret != baudrate) {
        printf("canOpen_driver (lincan): IOCTL set speed failed\n");
        return 0;
    }

    //设置工作模式#/
    canModeset_driver(fd,CAN_NORMALMODE);

    return fd;
}

//关闭can设备#/
int CanClose_Handle(int fd0)
{
    if(!fd0)
        return 0;
    close(fd0);
    return 0;
}

UNS8 CanReset_Handle(int fd0)
{
    return ioctl(fd0, CAN_IOCTRESET, 0);
}

//CAN设备发送数据#/
UNS8 CanSend_Handle(int fd0, Message const *m)
{
    int res;
    struct canmsg_tx canmsg;

    if(fd0<=0)
        return 1;

    memset(&canmsg, 0, sizeof(struct canmsg_tx));

    canmsg.header.id = m->cob_id;
    canmsg.header.length = m->len;
    if(m->rtr){
        canmsg.header.rtr = MSG_RTR;
    } else {
        memcpy(canmsg.data, m->data, m->len);
    }

    res = write(fd0, &canmsg, sizeof(struct canmsg_tx));
    if(res!=sizeof(struct canmsg_tx))
        return 1;

    return 0;
}

//CAN设备读取接收数据#/
UNS8 CanReceive_Handle(int fd0, Message *m)
{
    int res;
    struct canmsg_tx canmsg;

    memset(&canmsg, 0, sizeof(struct canmsg_tx));

//    res = read(fd0, &canmsg, sizeof(canmsg_tx));

    do{
        res = read(fd0, &canmsg, sizeof(struct canmsg_tx));
        if((res<0)&&(errno == -EAGAIN)) res = 0;
    }while(res==0);

    if(res != sizeof(struct canmsg_tx)) // No new message
        return 1;

    m->cob_id = canmsg.header.id;
    m->len = canmsg.header.length;
    if(canmsg.header.rtr&MSG_RTR){
        m->rtr = 1;
    }else{
        m->rtr = 0;
        memcpy(m->data,canmsg.data,8);
    }

    return 0;
}




