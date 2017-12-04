#include <stdio.h>
#include <unistd.h>
#include "deal.h"
#include "packet_canopen.h"
#include "canopen_shell.h"
#ifdef OS_LINUX
#include "ObjDict.h"
#endif


struct CanNode canNode[CANNODE_NUM] = {
    {"Human Interface", CANID_UI},
    {"Master Control", CANID_MASTERCTRL},
    {"Monitor", CANID_MONITOR},
    {"Test", CANID_TEST},
    {"Power", CANID_POWER},
    {"Blood Pump", CANID_BLOOD_PUMP},
    {"Blood Pump2", CANID_BLOOD_PUMP2},
    {"Heparin Pump", CANID_HEPARIN_PUMP},
    {"Hemomano Meter", CANID_HEMOMANO_METER},
    {"Hietlau Sensor", 21}
};

int treatmodel[11][2]={
    {0x01,0x00},
    {0x01,0x01},
    {0x01,0x02},
    {0x03,0x00},
    {0x03,0x00},
    {0x04,0x00},
    {0x04,0x00},
    {0x02,0x00},
    {0x02,0x01},
    {0x02,0x02},
    {0x06,0x00}
};

#define CanopenState_Judge(i,r) \
    r=CanOpen_GetState(i);      \
    if ((r == STATE_CAN_Pre_operational) || (r == STATE_CAN_Pre_operational)) { \
        CanNode_Start(r);       \
    } else if (r != STATE_CAN_Operational) {    \
        return -1;              \
    }


/******************************************************************************
  Heparin pump Function
 ******************************************************************************/
int HeparinPump_CanState(void)
{
    return CanOpen_GetState(canNode[CANNODE_TEST].nodeId);
}

int HeparinPump_CanStart(void)
{
    return CanNode_Start(canNode[CANNODE_TEST].nodeId);
}

int HeparinPump_CanStop(void)
{
    return CanNode_Stop(canNode[CANNODE_HEPARINPUMP].nodeId);
}

int HeparinPump_ActStart(void)
{
    if (CanNode_Start(canNode[CANNODE_HEPARINPUMP].nodeId) != 0) {
        return -1;
    }
    //return WriteDeviceSdoEntry(canNode[CANNODE_HEPARINPUMP].nodeId,
    //        CAN_HEPARIN_PUMP_SDO_INDEX, 0x04, 1, 0x01);
    return 0;
}

int HeparinPump_ActStop(void)
{
    //return WriteDeviceSdoEntry(canNode[CANNODE_TEST].nodeId, CAN_HEPARIN_PUMP_SDO_INDEX, 0x04, 1, 0x02);
    return 0;
}

int HeparinPump_ActDoseSpeed(float dose, float speed)
{
    if (CanNode_Start(canNode[CANNODE_HEPARINPUMP].nodeId) != 0) {
        return -1;
    }

    if ((speed < 0) || (speed > 9.9) || (dose < 0) || (dose > 9.9)) {
        return -1;
    }

    unsigned int value;
    value = ((unsigned int)(speed*10))<<16;
    value = value | ((unsigned short)(dose*10));
    //return WriteDeviceSdoEntry(canNode[CANNODE_TEST].nodeId, CAN_HEPARIN_PUMP_SDO_INDEX, 0x03, 4, value);
    return 0;
}

int HeparinPump_ActQuickSpeed(int speed)
{
    if (CanNode_Start(canNode[CANNODE_HEPARINPUMP].nodeId) != 0) {
        return -1;
    }

    if ((speed < 1) || (speed > 20000)) {
        return -1;
    }

    //return WriteDeviceSdoEntry(canNode[CANNODE_TEST].nodeId, CAN_HEPARIN_PUMP_SDO_INDEX, 0x01, 4, (unsigned int)(speed<<16));
    return 0;
}

int HeparinPump_ActSyring(const float len, const float style)
{
    if (CanNode_Start(canNode[CANNODE_HEPARINPUMP].nodeId) != 0) {
        return -1;
    }

    if ((len < 6) || (len > 99.9) || (style < 6) || (style > 99.9)) {
        return -1;
    }

    unsigned int value;
    value = ((unsigned int)(style*10))<<16;
    value = value | ((unsigned short)(len*10));
    //return WriteDeviceSdoEntry(canNode[CANNODE_TEST].nodeId, CAN_HEPARIN_PUMP_SDO_INDEX, 0x02, 4, value);
    return 0;
}

int HeparinPump_ActReadQuickSpeed(void)
{
    return ReadDeviceSdoEntry(canNode[CANNODE_TEST].nodeId, CAN_HEPARIN_PUMP_SDO_INDEX, 0x01);
}

int HeparinPump_ActReadDoseSpeed(void)
{
    return ReadDeviceSdoEntry(canNode[CANNODE_TEST].nodeId, CAN_HEPARIN_PUMP_SDO_INDEX, 0x03);
}

int HeparinPump_ActReadSyring(void)
{
    return ReadDeviceSdoEntry(canNode[CANNODE_TEST].nodeId, CAN_HEPARIN_PUMP_SDO_INDEX, 0x02);
}

/******************************************************************************
  Power Function
 ******************************************************************************/
int Power_CanState(void)
{
    return CanOpen_GetState(canNode[CANNODE_POWER].nodeId);
}

int Power_CanStart(void)
{
    return CanNode_Start(canNode[CANNODE_POWER].nodeId);
}

int Power_CanStop(void)
{
    return CanNode_Stop(canNode[CANNODE_POWER].nodeId);
}

int Power_ActShut(void)
{
    if (CanNode_Start(canNode[CANNODE_POWER].nodeId) != 0) {
        return -1;
    }
    //return WriteDeviceSdoEntry(canNode[CANNODE_POWER].nodeId,
    //        CAN_POWER_SDO_INDEX,
    //        CAN_POWER_SDO_INDEXSUB_SHUTDOWN, 1, 1);
    return 0;
}

/******************************************************************************
  Blood pump Function
 ******************************************************************************/
int BloodPump_CanState(void)
{
    return CanOpen_GetState(canNode[CANNODE_BLOODPUMP].nodeId);
}

int BloodPump_CanStart(void)
{
    return CanNode_Start(canNode[CANNODE_BLOODPUMP].nodeId);
}

int BloodPump_CanStop(void)
{
    return CanNode_Stop(canNode[CANNODE_BLOODPUMP].nodeId);
}

int BloodPump_ActStart(void)
{
#if 0
    if (CanNode_Start(canNode[CANNODE_BLOODPUMP].nodeId) != 0) {
        return -1;
    }
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_BLOOD_PUMP_SDO_INDEX,
            CAN_BLOOD_PUMP_SDO_INDEXSUB_MODE, 4, 0x11);
#endif
    return 0;
}

int BloodPump_ActStop(void)
{
#if 0
    if (CanNode_Start(canNode[CANNODE_BLOODPUMP].nodeId) != 0) {
        return -1;
    }
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_BLOOD_PUMP_SDO_INDEX,
            CAN_BLOOD_PUMP_SDO_INDEXSUB_MODE, 4, 0x12);
#endif
    return 0;
}

int BloodPump_ActTest(void)
{
    if (CanNode_Start(canNode[CANNODE_BLOODPUMP].nodeId) != 0) {
        return -1;
    }
    //return WriteDeviceSdoEntry(canNode[CANNODE_BLOODPUMP].nodeId,
    //        CAN_BLOOD_PUMP_SDO_INDEX,
    //        CAN_BLOOD_PUMP_SDO_INDEXSUB_MODE, 1, 4);
    return 0;

}

int BloodPump_ActPause(void)
{
    if (CanNode_Start(canNode[CANNODE_BLOODPUMP].nodeId) != 0) {
        return -1;
    }
    //return WriteDeviceSdoEntry(canNode[CANNODE_BLOODPUMP].nodeId,
    //        CAN_BLOOD_PUMP_SDO_INDEX,
    //        CAN_BLOOD_PUMP_SDO_INDEXSUB_MODE, 1, 3);
    return 0;
}

int BloodPump_ActDia(unsigned char dia)
{
    if (CanNode_Start(canNode[CANNODE_BLOODPUMP].nodeId) != 0) {
        return -1;
    }
    dia = dia;
    //return WriteDeviceSdoEntry(canNode[CANNODE_BLOODPUMP].nodeId,
    //        CAN_BLOOD_PUMP_SDO_INDEX,
    //        CAN_BLOOD_PUMP_SDO_INDEXSUB_DIA, 1, dia);
    return 0;
}

int BloodPump_ActSpeed(unsigned short speed)
{
    if (CanNode_Start(canNode[CANNODE_BLOODPUMP].nodeId) != 0) {
        return -1;
    }
    speed = speed;
    //return WriteDeviceSdoEntry(canNode[CANNODE_BLOODPUMP].nodeId,
    //        CAN_BLOOD_PUMP_SDO_INDEX,
    //        CAN_BLOOD_PUMP_SDO_INDEXSUB_SPEED, 2, speed);
    return 0;
}

/******************************************************************************
  Master control Function
 ******************************************************************************/
int MasterCtrol_CanState(void)
{
    return CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId);
}

int MasterCtrol_SetSetup(struct PacketSdoSetup *sdoSetup)
{
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_SETUP, 4, sdoSetup->N3_set);
}

/*!
 * \brief MasterCtrol_CMD_Checkself 发送准备状态
 * \return
 */
int MasterCtrol_CMD_Checkself(void)
{
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }

    //unsigned int cmd=0x1;
    unsigned char cmd[4]={0x1,0x04};
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//启动预冲#/
int MasterCtrol_CMD_Startpreflush(unsigned char mode)
{
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }

    //unsigned int cmd=0x1;
    unsigned char cmd[4]={0x07,0x00};
    cmd[1]=mode;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//停止预冲#/
int MasterCtrol_CMD_Stoppreflush(void)
{
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }

    //unsigned int cmd=0x1;
    unsigned char cmd[4]={0x08,0x00};
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//启动回血#/
int MasterCtrol_CMD_Startreturnblood(unsigned char mode)
{
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }

    //unsigned int cmd=0x1;
    unsigned char cmd[4]={0x09,0x00};
    cmd[1]=mode;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//停止回血#/
int MasterCtrol_CMD_Stopreturnblood(void)
{
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }

    //unsigned int cmd=0x1;
    unsigned char cmd[4]={0x0A,0x00};
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

int MasterCtrol_CMD_Pass_Checkself(void)
{
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }

    unsigned char cmd[4]={0x1,0x01};
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

int MasterCtrol_CMD_Stop_water(void)//停止水路#/
{
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }

    unsigned int cmd=0x2;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

/*!
 * \brief MasterCtrol_CMD_Treat 发送治疗状态
 * \return
 */

//启动治疗。。。。#/
//这个函数得改写。。。#/
//得写一个参数，说是现在要启动那种治疗。。。。。具体得看协议#/
int MasterCtrol_CMD_Treat_start(int modle)
{
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }

    unsigned char cmd[4]={0x03,treatmodel[modle][0],treatmodel[modle][1],0x0};//
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//水路重启#/
int MasterCtrol_CMD_WaterReboot(void)
{
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=0x66;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//启动停止治疗（回血）(血)#/
int MasterCtrol_CMD_Treat_stop(void)
{
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=0x9;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//停止结束治疗(回血) (血,水)#/
int MasterCtrol_CMD_Treat_end(void)
{
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=0xA;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

int MasterCtrol_CMD_Dry_start(void)
{
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }

    unsigned int cmd=0xB;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

int MasterCtrol_CMD_Dry_stop(void)
{
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }

    unsigned int cmd=0xC;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

int MasterCtrol_CMD_Disinfect_start(int model)
{
    unsigned char cmd[4]={0xD,0x11,0,0};

//    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
//        return -1;
//    }

    cmd[1]=model;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//
int MasterCtrol_CMD_Disinfect_stop(void)
{
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }

    unsigned int cmd=0xE;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//启动血泵#/
int MasterCtrol_CMD_BloodStart(void)
{
    //CMD_mutex.lock();
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=0x11;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);

//    trc=0;
//    while( WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
//            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd) == -1 && trc<5)
//    {
//     trc++;
//     usleep(100000);
//     }
//    if(trc>=5)
//        return -1;

    //return 0;
    //CMD_mutex.unlock();
}

//直接启动血泵#/
int BloodPump_CMD_BloodStart(void)
{
    //CMD_mutex.lock();
    if (CanNode_Start(canNode[CANNODE_BLOODPUMP].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=0x01000000;
    return WriteDeviceSdoEntry(canNode[CANNODE_BLOODPUMP].nodeId,
            CAN_SDO_INDEX, 0x01, 4, &cmd);
}


//直接停止血泵#/
int BloodPump_CMD_BloodStop(void)
{
    //CMD_mutex.lock();#/
    if (CanNode_Start(canNode[CANNODE_BLOODPUMP].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=0x02000000;
    return WriteDeviceSdoEntry(canNode[CANNODE_BLOODPUMP].nodeId,
            CAN_SDO_INDEX, 0x01, 4, &cmd);
}

//血泵1 禁止物理开关#/
int MasterCtrol_CMD_Blood1DisableButton(void)
{
    //CMD_mutex.lock();
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=0x0111;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//血泵1 启用物理开关#/
int MasterCtrol_CMD_Blood1EnableButton(void)
{
    //CMD_mutex.lock();
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=0x0211;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//停止血泵#/
int MasterCtrol_CMD_BloodStop(void)
{
    //CMD_mutex.lock();#/
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=0x12;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
//    trc=0;
//    while( WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
//            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd) == -1 && trc<5)
//    {
//     trc++;
//     usleep(100000);
//     }
//    if(trc>=5)
//        return -1;

    //return 0;
    //CMD_mutex.unlock();
}

//清空血泵的累计量#/
int MasterCtrol_CMD_ResetBloodAmount(void)
{

    if (CanNode_Start(canNode[CANNODE_BLOODPUMP].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=0x04000000;
    return WriteDeviceSdoEntry(canNode[CANNODE_BLOODPUMP].nodeId,
            CAN_SDO_INDEX,  CAN_SDO_INDEXSUB_SETUP, 4, &cmd);
}

//启动置换液泵#/
int MasterCtrol_CMD_SubsStart(void)
{
//    SubSAsmutex.lock();#/
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=0x13;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
//    SubSAsmutex.unlock();#/
}


//直接启动置换液泵#/
int SubPump_CMD_SubsStart(void)
{
//    SubSAsmutex.lock();#/
    if (CanNode_Start(canNode[CANNODE_BLOODPUMP2].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=0x01000000;
    return WriteDeviceSdoEntry(canNode[CANNODE_BLOODPUMP2].nodeId,
            CAN_SDO_INDEX, 0x01, 4, &cmd);
//    SubSAsmutex.unlock();#/
}

//直接停止置换液泵#/
int SubPump_CMD_SubsStop(void)
{
 //   SubSTsmutex.lock();
    if (CanNode_Start(canNode[CANNODE_BLOODPUMP2].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=0x02000000;
    return WriteDeviceSdoEntry(canNode[CANNODE_BLOODPUMP2].nodeId,
            CAN_SDO_INDEX, 0x01, 4, &cmd);
 //   SubSTsmutex.unlock();
}

//置换液泵 禁止物理开关#/
int MasterCtrol_CMD_SubsDisableButton(void)
{
//    SubSAsmutex.lock();#/
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=0x0113;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
//    SubSAsmutex.unlock();#/
}

//置换液泵 启用物理开关#/
int MasterCtrol_CMD_SubsEnableButton(void)
{
//    SubSAsmutex.lock();#/
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=0x0213;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
//    SubSAsmutex.unlock();#/
}

//停止置换液泵#/
int MasterCtrol_CMD_SubsStop(void)
{
 //   SubSTsmutex.lock();
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=0x14;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
 //   SubSTsmutex.unlock();
}

//置换液泵--单次在线补液#/
int MasterCtrol_CMD_SubsAdd(char num)
{
 //   SubSTsmutex.lock();
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=((num<<8)|0x2a);
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
 //   SubSTsmutex.unlock();
}

//清空置换液泵的累计量#/
int MasterCtrol_CMD_ResetSubsAmount(void)
{

    if (CanNode_Start(canNode[CANNODE_BLOODPUMP2].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=0x04000000;
    return WriteDeviceSdoEntry(canNode[CANNODE_BLOODPUMP2].nodeId,
            CAN_SDO_INDEX,  CAN_SDO_INDEXSUB_SETUP, 4, &cmd);
}

//启动肝素 有首剂量#/
int MasterCtrol_CMD_HeparinPumpStartHaveFirst(void)
{
    //CMD_mutex.lock();
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=0x0015;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
    //CMD_mutex.unlock();
}

//启动肝素 无首剂量#/
int MasterCtrol_CMD_HeparinPumpStartNoFirst(void)
{
    //CMD_mutex.lock();
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=0x0115;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
    //CMD_mutex.unlock();
}

//启动肝素 带快推量的快推#/
int MasterCtrol_CMD_HeparinPumpAddSingle(unsigned char vol)
{
    unsigned char cmd[4]={0x15,0x02,0x00,0x00};
    //CMD_mutex.lock();
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    cmd[2]=vol;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, cmd);
    //CMD_mutex.unlock();
}

//停止肝素#/
int MasterCtrol_CMD_HeparinPumpStop(void)
{
    //CMD_mutex.lock();
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=0x0116;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
    //CMD_mutex.unlock();
}

//开启肝素泵，有首计量#/
int Heparin_CMD_HeparinPumpStartHaveFirst(void)
{

    if (CanNode_Start(canNode[CANNODE_HEPARINPUMP].nodeId) != 0) {
        return -1;
    }

    //主控板，sdo命令#/
    unsigned int cmd=0x01;
    return WriteDeviceSdoEntry(canNode[CANNODE_HEPARINPUMP].nodeId,
            CAN_SDO_INDEX,  CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//开启肝素泵，无首计量#/
int Heparin_CMD_HeparinPumpStartNoFirst(void)
{

    if (CanNode_Start(canNode[CANNODE_HEPARINPUMP].nodeId) != 0) {
        return -1;
    }

    //主控板，sdo命令#/
    unsigned int cmd=0x05;
    return WriteDeviceSdoEntry(canNode[CANNODE_HEPARINPUMP].nodeId,
            CAN_SDO_INDEX,  CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//单次追加首剂量，速度为快推#/
int Heparin_CMD_HeparinPumpQuickAdd(void)
{

    if (CanNode_Start(canNode[CANNODE_HEPARINPUMP].nodeId) != 0) {
        return -1;
    }

    //主控板，sdo命令#/
    unsigned int cmd=0x06;
    return WriteDeviceSdoEntry(canNode[CANNODE_HEPARINPUMP].nodeId,
            CAN_SDO_INDEX,  CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//停止肝素泵，不清总量，能下拉#/
int Heparin_CMD_HeparinPumpStopCanDown(void)
{

    if (CanNode_Start(canNode[CANNODE_HEPARINPUMP].nodeId) != 0) {
        return -1;
    }

    unsigned int cmd=0x03;
    return WriteDeviceSdoEntry(canNode[CANNODE_HEPARINPUMP].nodeId,
            CAN_SDO_INDEX,  CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//停止肝素泵，清总量，能下拉#/
int Heparin_CMD_HeparinPumpStopResetCanDown(void)
{
    if (CanNode_Start(canNode[CANNODE_HEPARINPUMP].nodeId) != 0) {
        return -1;
    }

    unsigned int cmd=0x15;
    return WriteDeviceSdoEntry(canNode[CANNODE_HEPARINPUMP].nodeId,
            CAN_SDO_INDEX,  CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}


//设置单次追加首剂量和速度#/
int Heparin_CMD_SetQuickAddVol(int vol,int seep)
{
    unsigned char cmd[4]={0x00,0x00,0x00,0x00};

    if (CanNode_Start(canNode[CANNODE_HEPARINPUMP].nodeId) != 0) {
        return -1;
    }

    cmd[0]=vol;
    cmd[2]=seep;
    return WriteDeviceSdoEntry(canNode[CANNODE_HEPARINPUMP].nodeId,
            CAN_SDO_INDEX,  CAN_HEPARIN_PUMP_SDO_INDEXSUB_QUICKSPEED, 4, cmd);
}

//清空肝素泵的累计量#/
int MasterCtrol_CMD_ResetHeparinPumpAmount(void)
{

    if (CanNode_Start(canNode[CANNODE_HEPARINPUMP].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=0x15;
    return WriteDeviceSdoEntry(canNode[CANNODE_HEPARINPUMP].nodeId,
            CAN_SDO_INDEX,  CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//启动超滤#/
int MasterCtrol_CMD_UFStart(void)
{
    //CMD_mutex.lock();
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=0x17;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
    //CMD_mutex.unlock();
}

//停止超滤#/
int MasterCtrol_CMD_UFStop(void)
{
    //CMD_mutex.lock();
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=0x18;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
    //CMD_mutex.unlock();
}

//清空超滤泵的累计量#/
int MasterCtrol_CMD_ResetUFAmount(void)
{

    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
    //主控板，sdo命令#/
    unsigned int cmd=0x00000118;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX,  CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

int MasterCtrol_CMD_Bypass(void)
{
 //   BypassSAmutex.lock();
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }

    unsigned int cmd=0x19;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
 //   BypassSAmutex.unlock();
}

int MasterCtrol_CMD_Nobypass(void)
{
//    BypassSTmutex.lock();
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }

    unsigned int cmd=0x1A;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
//    BypassSTmutex.unlock();
}

int MasterCtrol_CMD_ClipStart(void)
{
   //CMD_mutex.lock();
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }

    unsigned int cmd=0x20;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
    //CMD_mutex.unlock();
}

int MasterCtrol_CMD_ClipStop(void)
{
    //CMD_mutex.lock();
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }

    unsigned int cmd=0x21;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
    //CMD_mutex.unlock();
}
int MasterCtrol_CMD_Clip2Start(void)
{
    //CMD_mutex.lock();
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }

    unsigned int cmd=0x2B;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
    //CMD_mutex.unlock();
}

int MasterCtrol_CMD_Clip2Stop(void)
{
    //CMD_mutex.lock();
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }

    unsigned int cmd=0x2C;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
    //CMD_mutex.unlock();
}

int MasterCtrol_SetFormulaB(struct PacketSdoScale *scale)
{
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_SCALE, 4, scale);
}

int MasterCtrol_SetFormulaTarget(struct PacketSdoAim* aim)
{
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_AIM, 4, aim);
}

int MasterCtrol_SetHeparin(struct PacketSdoHeparin* heparin)
{
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_HEPARIN, 4, heparin);
}

int MasterCtrol_SetRangeTemp(struct PacketSdoRangeTemp* rangeTemp)
{
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_RANGETEMP, 4, rangeTemp);
}

//发送pdo#/
void MasterCtrol_SendPDO(void *pack)
{

    //----HJX实现直接操作#/
    //发送本设备所有T-PDO的数据#/
    HJX_TrinsmitPDO((unsigned char *)pack);

//    setPDOContent((unsigned char *)pack);
    /*
    UNS8 i,index;

    time_t timep;
    time(&timep);
    Message pdomsg;

    unsigned char * chp=(unsigned char *)pack;
    for(i = 0;i < 8; i++) {
           pdomsg.data[i]=chp[i];
      }

      pdomsg.len=8;
      pdomsg.cob_id=0x181;
      pdomsg.rtr=NOT_A_REQUEST;

      printf("N1 send pdo:");
      for (index=0; index < 8; index++)
      {
          printf("%02X ", pdomsg.data[index]);
      }

    printf("  time:%s \n",asctime(localtime(&timep)));
    */

//    TrinsmitPDO();
}

//取反发送pdo#/
void MasterCtrol_SendPDO_negation(void *pack)
{
    unsigned char fdata[8];

    memcpy(fdata,pack,8);

    fdata[0]=~fdata[0];
    fdata[1]=~fdata[1];
    fdata[2]=~fdata[2];
    fdata[3]=~fdata[3];
    fdata[4]=~fdata[4];
    fdata[5]=~fdata[5];
    fdata[6]=~fdata[6];
    fdata[7]=~fdata[7];
    //----HJX实现直接操作#/
    //发送本设备所有T-PDO的数据#/
    HJX_TrinsmitPDO(fdata);
}

int MasterCtrol_CMD_Shutdown(void)
{
    if (CanNode_Start(canNode[CANNODE_POWER].nodeId) != 0) {
        return -1;
    }

    unsigned int cmd=0x01;
    return WriteDeviceSdoEntry(canNode[CANNODE_POWER].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_SHUTDOWN, 4, &cmd);
}

int MasterCtrol_CMD_Reboot(void)
{
    if (CanNode_Start(canNode[CANNODE_POWER].nodeId) != 0) {
        return -1;
    }

    unsigned char cmd[4]={0x00,0x00,0x00,0xAA};
    return WriteDeviceSdoEntry(canNode[CANNODE_POWER].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_SHUTDOWN, 4, &cmd);
}

int MasterCtrol_CMD_EEporm_read(int addr)
{
    int index;

    if(addr<500)
    {
        index=CANNODE_MASTER_CONTROL;
    }
    else if(addr>=500&&addr<600)
    {
        index=CANNODE_MASTER_MONITOR;
        addr=addr-500;
    }
    else if(addr>=900&&addr<1000)
    {

        index=CANNODE_BLOODPUMP;
        addr=addr-900;
    }
    else if(addr>=1000&&addr<1100)
    {
        index=CANNODE_BLOODPUMP2;
        addr=addr-1000;
    }
    else if(addr>=1500&&addr<1600)
    {
        index=CANNODE_HEPARINPUMP;
        addr=addr-1500;
    }
    else return -1;

    //20151116修改#/
/*---------------------HJX屏蔽，为了解决有时读不成功#/
    if (CanNode_Start(canNode[index].nodeId) != 0) {
        return -1;
    }
    */

    unsigned int cmd=addr;
    return WriteDeviceSdoEntry(canNode[index].nodeId,
            CAN_SDO_INDEX, 3, 4, &cmd);
}

int MasterCtrol_CMD_EEporm_write(unsigned char *addr_data)
{
    int index,address;
    address=addr_data[0]+addr_data[1]*256;
    if(address<500)index=CANNODE_MASTER_CONTROL;
    else if(address>=500&&address<600)
    {
        index=CANNODE_MASTER_MONITOR;
        address=address-500;
    }
    else if(address>=900&&address<1000)
    {
        index=CANNODE_BLOODPUMP;
        address=address-900;
    }
    else if(address>=1000&&address<1100)
    {
        index=CANNODE_BLOODPUMP2;
        address=address-1000;
    }
    else if(address>=1500&&address<1600)
    {
        index=CANNODE_HEPARINPUMP;
        address=address-1500;
    }
    else return -1;

    if (CanNode_Start(canNode[index].nodeId) != 0) {
        return -1;
    }

    addr_data[0]=(unsigned char)address;
    addr_data[1]=address>>8;
    return WriteDeviceSdoEntry(canNode[index].nodeId,
            CAN_SDO_INDEX, 4, 4, addr_data);
}

int ActReadEEprom(int subIndex,int addr)
{
    unsigned int tmp = 0;
    int index;
    if(addr<500)index=CANNODE_MASTER_CONTROL;
    else if(addr>=500&&addr<600)index=CANNODE_MASTER_MONITOR;
    else if(addr>=900&&addr<1000)index=CANNODE_BLOODPUMP;
    else if(addr>=1000&&addr<1100)index=CANNODE_BLOODPUMP2;
    else if(addr>=1500&&addr<1600)index=CANNODE_HEPARINPUMP;
    else return -1;

    if(ReadDeviceSdoEntry(canNode[index].nodeId, CAN_SDO_INDEX, subIndex)!=-1)
    {
        return tmp;
     }
    return 0;
}

int MasterCtrol_CMD_BMP_Start(void)
{
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }
     unsigned int cmd=0x22;
     int tmp=WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
                                 CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
     return tmp;

}

int MasterCtrol_CMD_BMP_Stop(void)
{
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }

    unsigned int cmd=0x23;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//监控板开启血压计  0：成人模式  1：婴儿#/
int MONITOR_CMD_BMP_Start(char mode)
{
    if (CanNode_Start(canNode[CANNODE_MASTER_MONITOR].nodeId) != 0) {
        return -1;
    }
     unsigned int cmd=0x05;
     cmd = (cmd|(mode<<8));
     int tmp=WriteDeviceSdoEntry(canNode[CANNODE_MASTER_MONITOR].nodeId,
                                 CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
     return tmp;

}

//监控板停止血压计#/
int MONITOR_CMD_BMP_Stop(void)
{
    if (CanNode_Start(canNode[CANNODE_MASTER_MONITOR].nodeId) != 0) {
        return -1;
    }

    unsigned int cmd=0x06;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_MONITOR].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}


int MasterCtrol_CMD_ver_read(int id,int flag)
{
    int index;

     index =id;

     if(ReadDeviceSdoEntry(canNode[index].nodeId, 0x1018, flag)!=-1)
     {
         return 0;
     }

     return -1;

}


int MasterCtrol_CMD_DryFilter(void)
{
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }

    unsigned int cmd=0x2D;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

int MasterCtrol_CMD_StopDryFilter(void)
{
    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }

    unsigned int cmd=0x2E;
    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//打开相应的电磁阀  1:open #/
int MasterCtrol_CMD_OpenValve(unsigned int valvestate,char valveid)
{
    unsigned int tmp=0;
    unsigned int cmd=0;
    tmp=valvestate|(1<<valveid);

    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }

    if(valveid<=15)
    {
        cmd=0x011d;
        cmd=cmd|(tmp<<16);
    }
    else
    {
        cmd=0x021d;
        tmp=tmp&(0xffff0000);
        cmd=cmd|tmp;
    }

    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);

}

//关闭相应的电磁阀  0:close #/
int MasterCtrol_CMD_CloseValve(unsigned int valvestate,char valveid)
{
    unsigned int tmp=0;
    unsigned int cmd=0;
    cmd=(1<<valveid);
    cmd=~cmd;
    tmp=valvestate&cmd;

    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }

    if(valveid<=15)
    {
        cmd=0x011d;
        cmd=cmd|(tmp<<16);
    }
    else
    {
        cmd=0x021d;
        tmp=tmp&(0xffff0000);
        cmd=cmd|tmp;
    }

    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);

}

//P1泵控制 mode：1 正转  2反转  3停止#/
int MasterCtrol_CMD_P1Pump(unsigned int mode,unsigned int vol)
{
    unsigned int cmd=0;

    cmd=0x031d;

    if((mode>=1)&&(mode<=3))
    {
        cmd=cmd|(mode<<16);
    }
    else
        return -1;

    cmd=cmd|(vol<<24);

    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }


    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//P2泵控制 mode：1 正转  2反转  3停止#/
int MasterCtrol_CMD_P2Pump(unsigned int mode,unsigned int vol)
{
    unsigned int cmd=0;

    cmd=0x041d;

    if((mode>=1)&&(mode<=3))
    {
        cmd=cmd|(mode<<16);
    }
    else
        return -1;

    cmd=cmd|(vol<<24);

    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }


    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//P3泵控制 mode：1 正转  2反转  3停止#/
int MasterCtrol_CMD_P3Pump(unsigned int mode,unsigned int vol)
{
    unsigned int cmd=0;

    cmd=0x051d;

    if((mode>=1)&&(mode<=3))
    {
        cmd=cmd|(mode<<16);
    }
    else
        return -1;

    cmd=cmd|(vol<<24);

    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }


    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//P4泵控制 mode：1 正转  2反转  3停止#/
int MasterCtrol_CMD_P4Pump(unsigned int mode,unsigned int vol)
{
    unsigned int cmd=0;

    cmd=0x061d;

    if((mode>=1)&&(mode<=3))
    {
        cmd=cmd|(mode<<16);
    }
    else
        return -1;

    cmd=cmd|(vol<<24);

    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }


    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//JP1泵控制 mode：1 正转  2反转  3停止#/
int MasterCtrol_CMD_JP1Pump(unsigned int mode,unsigned int vol)
{
    unsigned int cmd=0;

    cmd=0x071d;

    if((mode>=1)&&(mode<=3))
    {
        cmd=cmd|(mode<<16);
    }
    else
        return -1;

    cmd=cmd|(vol<<24);

    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }


    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//JP2泵控制 mode：1 正转  2反转  3停止#/
int MasterCtrol_CMD_JP2Pump(unsigned int mode,unsigned int vol)
{
    unsigned int cmd=0;

    cmd=0x081d;

    if((mode>=1)&&(mode<=3))
    {
        cmd=cmd|(mode<<16);
    }
    else
        return -1;

    cmd=cmd|(vol<<24);

    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }


    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//JP3泵控制 mode：1 正转  2反转  3停止#/
int MasterCtrol_CMD_JP3Pump(unsigned int mode,unsigned int vol)
{
    unsigned int cmd=0;

    cmd=0x091d;

    if((mode>=1)&&(mode<=3))
    {
        cmd=cmd|(mode<<16);
    }
    else
        return -1;

    cmd=cmd|(vol<<24);

    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }


    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//平衡腔控制 mode：1 启动  2停止#/
int MasterCtrol_CMD_Balance(unsigned int mode,unsigned int vol)
{
    unsigned int cmd=0;

    cmd=0x0a1d;

    if((mode>=1)&&(mode<=2))
    {
        cmd=cmd|(mode<<16);
    }
    else
        return -1;

    cmd=cmd|(vol<<24);

    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }


    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//加热腔控制 mode：1 启动  2停止#/
int MasterCtrol_CMD_Heating(unsigned int mode,unsigned int vol)
{
    unsigned int cmd=0;

    cmd=0x0b1d;

    if((mode>=1)&&(mode<=2))
    {
        cmd=cmd|(mode<<16);
    }
    else
        return -1;

    cmd=cmd|(vol<<24);

    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }


    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//补液腔除气控制 mode：1 启动  2停止#/
int MasterCtrol_CMD_DegassPump(unsigned int mode,unsigned int vol)
{
    unsigned int cmd=0;

    cmd=0x0c1d;

    if((mode>=1)&&(mode<=2))
    {
        cmd=cmd|(mode<<16);
    }
    else
        return -1;

    cmd=cmd|(vol<<24);

    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }


    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//配方测试 mode：1 启动  2停止#/
int MasterCtrol_CMD_FormulaTest(unsigned int mode)
{
    unsigned int cmd=0;

    cmd=0x0d1d;

    if((mode>=1)&&(mode<=2))
    {
        cmd=cmd|(mode<<16);
    }
    else
        return -1;


    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }


    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//透析夜测试 mode：1 启动  2停止#/
int MasterCtrol_CMD_DialysateTest(unsigned int mode)
{
    unsigned int cmd=0;

    cmd=0x0e1d;

    if((mode>=1)&&(mode<=2))
    {
        cmd=cmd|(mode<<16);
    }
    else
        return -1;


    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }


    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//A液泵测试 mode：1 启动  2停止#/
int MasterCtrol_CMD_APumpTest(unsigned int mode,unsigned int vol)
{
    unsigned int cmd=0;

    cmd=0x0f1d;

    if((mode>=1)&&(mode<=2))
    {
        cmd=cmd|(mode<<16);
    }
    else
        return -1;

    if(mode==1)
    {
        cmd=cmd|(vol<<24);
    }


    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }


    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//B液泵测试 mode：1 启动  2停止#/
int MasterCtrol_CMD_BPumpTest(unsigned int mode,unsigned int vol)
{
    unsigned int cmd=0;

    cmd=0x101d;

    if((mode>=1)&&(mode<=2))
    {
        cmd=cmd|(mode<<16);
    }
    else
        return -1;

    if(mode==1)
    {
        cmd=cmd|(vol<<24);
    }


    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }


    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//温度测试 mode：1 启动  2停止#/
int MasterCtrol_CMD_TempTest(unsigned int mode)
{
    unsigned int cmd=0;

    cmd=0x111d;

    if((mode>=1)&&(mode<=2))
    {
        cmd=cmd|(mode<<16);
    }
    else
        return -1;


    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }


    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//电导测试 mode：1 启动  2停止#/
int MasterCtrol_CMD_CondTest(unsigned int mode)
{
    unsigned int cmd=0;

    cmd=0x121d;

    if((mode>=1)&&(mode<=2))
    {
        cmd=cmd|(mode<<16);
    }
    else
        return -1;


    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }


    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}

//超滤泵测试 mode：1 启动  2停止#/
int MasterCtrol_CMD_UFTest(unsigned int mode)
{
    unsigned int cmd=0;

    cmd=0x131d;

    if((mode>=1)&&(mode<=2))
    {
        cmd=cmd|(mode<<16);
    }
    else
        return -1;


    if (CanNode_Start(canNode[CANNODE_MASTER_CONTROL].nodeId) != 0) {
        return -1;
    }


    return WriteDeviceSdoEntry(canNode[CANNODE_MASTER_CONTROL].nodeId,
            CAN_SDO_INDEX, CAN_SDO_INDEXSUB_CMD, 4, &cmd);
}
