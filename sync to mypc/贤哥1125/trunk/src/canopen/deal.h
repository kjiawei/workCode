#ifndef DEAL_H
#define DEAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "config_canopen.h"
//#include "packet_ui.h"

enum CanNodeEnum {
    CANNODE_MASTER,
    CANNODE_MASTER_CONTROL,
    CANNODE_MASTER_MONITOR,
    CANNODE_TEST,
    CANNODE_POWER,
    CANNODE_BLOODPUMP,
    CANNODE_BLOODPUMP2,
    CANNODE_HEPARINPUMP,
    CANNODE_HEMOMANO,
    CANNODE_HIETLAU,
    CANNODE_NUM
};

struct CanNode {
    char name[20];
    unsigned char nodeId;
    /* ... */
};

/* Heparin pump cmd */
int HeparinPump_CanState(void);
int HeparinPump_CanStart(void);
int HeparinPump_CanStop(void);
int HeparinPump_ActStart(void);
int HeparinPump_ActStop(void);
int HeparinPump_ActQuickSpeed(int speed);
int HeparinPump_ActDoseSpeed(float dose, float speed);
int HeparinPump_ActSyring(const float len, const float style);
int HeparinPump_ActReadQuickSpeed(void);
int HeparinPump_ActReadDoseSpeed(void);
int HeparinPump_ActReadSyring(void);

/* Power cmd */
int Power_CanState(void);
int Power_CanStart(void);
int Power_CanStop(void);
int Power_ActShut(void);

/* Blood pump cmd */
int BloodPump_CanState(void);
int BloodPump_CanStart(void);
int BloodPump_CanStop(void);
int BloodPump_ActStart(void);
int BloodPump_ActStop(void);
int BloodPump_ActTest(void);
int BloodPump_ActPause(void);
int BloodPump_ActDia(unsigned char dia);
int BloodPump_ActSpeed(unsigned short speed);

/* Master Board control cmd */
#include "packet_can.h"
int MasterCtrol_CanState(void);
int MasterCtrol_SetSetup(struct PacketSdoSetup *sdoSetup);

int MasterCtrol_CMD_Checkself(void);
int MasterCtrol_CMD_Pass_Checkself(void);
int MasterCtrol_CMD_Startpreflush(unsigned char mode);//启动预冲#/
int MasterCtrol_CMD_Stoppreflush(void);//停止预冲#/
int MasterCtrol_CMD_Startreturnblood(unsigned char mode);//启动回血#/
int MasterCtrol_CMD_Stopreturnblood(void);//停止回血#/
int MasterCtrol_CMD_Stop_water(void);
int MasterCtrol_CMD_Treat_start(int model);
int MasterCtrol_CMD_Treat_stop(void);
int MasterCtrol_CMD_Treat_end(void);
int MasterCtrol_CMD_WaterReboot(void);

int MasterCtrol_CMD_Dry_start(void);
int MasterCtrol_CMD_Dry_stop(void);
int MasterCtrol_CMD_Disinfect_start(int model);
int MasterCtrol_CMD_Disinfect_stop(void);
int MasterCtrol_CMD_BloodStart(void);
int BloodPump_CMD_BloodStart(void);//直接启动血泵#/
int BloodPump_CMD_BloodStop(void);//直接停止血泵#/
int MasterCtrol_CMD_Blood1EnableButton(void);//血泵1 启用物理开关#/
int MasterCtrol_CMD_Blood1DisableButton(void);//血泵1 禁止物理开关#/
int MasterCtrol_CMD_BloodStop(void);
int MasterCtrol_CMD_ResetBloodAmount(void);
int MasterCtrol_CMD_SubsStart(void);
int SubPump_CMD_SubsStart(void);//直接启动置换液泵#/
int SubPump_CMD_SubsStop(void);//直接停止置换液泵#/
int MasterCtrol_CMD_SubsDisableButton(void);//置换液泵 禁止物理开关#/
int MasterCtrol_CMD_SubsEnableButton(void);//置换液泵 启用物理开关#/
int MasterCtrol_CMD_SubsStop(void);
int MasterCtrol_CMD_SubsAdd(char num);//置换液泵--单次在线补液#/
int MasterCtrol_CMD_ResetSubsAmount(void);
int MasterCtrol_CMD_HeparinPumpStartHaveFirst(void);
int MasterCtrol_CMD_HeparinPumpStartNoFirst(void);
int MasterCtrol_CMD_HeparinPumpAddSingle(unsigned char vol);//启动肝素 带快推量的快推#/
int MasterCtrol_CMD_HeparinPumpStop(void);
int MasterCtrol_CMD_ResetHeparinPumpAmount(void);
int MasterCtrol_CMD_UFStart(void);
int MasterCtrol_CMD_UFStop(void);
int MasterCtrol_CMD_ResetUFAmount(void);

int MasterCtrol_CMD_ClipStart(void);//阻流夹#/
int MasterCtrol_CMD_ClipStop(void);

int MasterCtrol_CMD_Clip2Start(void);//阻流夹2#/
int MasterCtrol_CMD_Clip2Stop(void);

int MasterCtrol_CMD_Bypass(void);//旁路#/
int MasterCtrol_CMD_Nobypass(void);

int MasterCtrol_SetFormulaB(struct PacketSdoScale *scale);
int MasterCtrol_SetFormulaTarget(struct PacketSdoAim* aim);
int MasterCtrol_SetHeparin(struct PacketSdoHeparin* heparin);
int MasterCtrol_SetRangeTemp(struct PacketSdoRangeTemp* rangeTemp);

void MasterCtrol_SendPDO(void *pack);
//取反发送pdo#/
void MasterCtrol_SendPDO_negation(void *pack);

int MasterCtrol_CMD_Shutdown(void);//关机#/
int MasterCtrol_CMD_Reboot(void);//
int MasterCtrol_CMD_EEporm_read(int addr);
int MasterCtrol_CMD_EEporm_write(unsigned char *addr_data);
int ActReadEEprom(int subIndex,int addr);

//开启肝素泵，有首计量#/
int Heparin_CMD_HeparinPumpStartHaveFirst(void);
//开启肝素泵，无首计量#/
int Heparin_CMD_HeparinPumpStartNoFirst(void);
//单次追加首剂量，速度为快推#/
int Heparin_CMD_HeparinPumpQuickAdd(void);
//停止肝素泵，不清总量，能下拉#/
int Heparin_CMD_HeparinPumpStopCanDown(void);
//停止肝素泵，清总量，能下拉#/
int Heparin_CMD_HeparinPumpStopResetCanDown(void);
//设置单次追加首剂量和速度#/
int Heparin_CMD_SetQuickAddVol(int vol,int seep);

int MasterCtrol_CMD_BMP_Start(void);
int MasterCtrol_CMD_BMP_Stop(void);
int MONITOR_CMD_BMP_Start(char mode);
int MONITOR_CMD_BMP_Stop(void);

int MasterCtrol_CMD_ver_read(int id,int flag);


int MasterCtrol_CMD_DryFilter(void);

int MasterCtrol_CMD_StopDryFilter(void);

//打开相应的电磁阀#/
int MasterCtrol_CMD_OpenValve(unsigned int valvestate,char valveid);

//关闭相应的电磁阀#/
int MasterCtrol_CMD_CloseValve(unsigned int valvestate,char valveid);

//P1泵控制 mode：1 正转  2反转  3停止#/
int MasterCtrol_CMD_P1Pump(unsigned int mode,unsigned int vol);

//P2泵控制 mode：1 正转  2反转  3停止#/
int MasterCtrol_CMD_P2Pump(unsigned int mode,unsigned int vol);

//P3泵控制 mode：1 正转  2反转  3停止#/
int MasterCtrol_CMD_P3Pump(unsigned int mode,unsigned int vol);

//P4泵控制 mode：1 正转  2反转  3停止#/
int MasterCtrol_CMD_P4Pump(unsigned int mode,unsigned int vol);

//JP1泵控制 mode：1 正转  2反转  3停止#/
int MasterCtrol_CMD_JP1Pump(unsigned int mode,unsigned int vol);

//JP2泵控制 mode：1 正转  2反转  3停止#/
int MasterCtrol_CMD_JP2Pump(unsigned int mode,unsigned int vol);

//JP3泵控制 mode：1 正转  2反转  3停止#/
int MasterCtrol_CMD_JP3Pump(unsigned int mode,unsigned int vol);

//平衡腔控制 mode：1 启动  2停止#/
int MasterCtrol_CMD_Balance(unsigned int mode,unsigned int vol);

//加热腔控制 mode：1 启动  2停止#/
int MasterCtrol_CMD_Heating(unsigned int mode,unsigned int vol);

//补液腔除气控制 mode：1 启动  2停止#/
int MasterCtrol_CMD_DegassPump(unsigned int mode,unsigned int vol);

//配方测试 mode：1 启动  2停止#/
int MasterCtrol_CMD_FormulaTest(unsigned int mode);

//透析夜测试 mode：1 启动  2停止#/
int MasterCtrol_CMD_DialysateTest(unsigned int mode);

//A液泵测试 mode：1 启动  2停止#/
int MasterCtrol_CMD_APumpTest(unsigned int mode,unsigned int vol);

//B液泵测试 mode：1 启动  2停止#/
int MasterCtrol_CMD_BPumpTest(unsigned int mode,unsigned int vol);

//温度测试 mode：1 启动  2停止#/
int MasterCtrol_CMD_TempTest(unsigned int mode);

//电导测试 mode：1 启动  2停止#/
int MasterCtrol_CMD_CondTest(unsigned int mode);

//超滤泵测试 mode：1 启动  2停止#/
int MasterCtrol_CMD_UFTest(unsigned int mode);

#ifdef __cplusplus
}
#endif

#endif
