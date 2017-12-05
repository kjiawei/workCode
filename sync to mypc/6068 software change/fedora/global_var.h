#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H

#include <list>
#include <iostream>
#include "state.h"

using namespace std;

//版本号
#define DE_5058_VERSION  "T2.01_20171128" //"V2.00.0" 全功能初始 "V2.01.0"添加可控水路图

#include "lang_switch.h"
extern list<LangSetText*> listText;

#include "xml_config.h"
extern list<XmlConfig*> listXmlConfig;
extern QString XMLALARM_STR;

//数据库操作
#include <QtSql>
extern QSqlDatabase db;
extern QSqlDatabase disinfect_db;
extern QSqlDatabase runLog_db;
/*
extern QSqlQuery *sqlPower;
extern QSqlQuery *sqlHeparin;
extern QSqlQuery *sqlBloodPump;
*/

#include "func_modules.h"
extern QHash<QString, FuncModuleSeries*> hashFuncModule;

#include "input_panel_context.h"
extern InputPanelContext *ic;

#include <QWidget>
extern QWidget *window_parent; //所有窗口類的父類#/

//数据曲线控件
#include "widget_data_curve.h"
extern Widget_Data_Curve *curveWidget;

//报警模块
#include "Alarm_module.h"
extern AlarmModule *alarmmodule;

#include "audioserver.h"
extern audioserver *audio;

#include "lamp_task.h"
extern lamptask *lamp;

//数据模块
#include "data_manage.h"
extern DataManage *dataManage;

#include "page_info.h"
extern PageManage pageManage;

extern bool isSelfCheckEnable;//selfcheck_enable

#include "data_timer.h"
extern data_timer* timer;

#include "widget_switch.h"
extern QMessageBox *msgBox;

#include "engineering_mode.h"
extern Dialog *enginerDialog;

#include "data_set.h"
extern Engineer engineer;
extern struct Params usr_params; //用户参数
extern int SaveCureParam_RetReady; //返回准备状态是否保存治疗参数 0：不保存 1保存 G_SAVECUREPARAM_FLAG

#include <packet_ui.h>
extern Params_Disinfect Disinfect;  //消毒参数全局变量#/
extern int disinfect_remainSec;    //消毒剩余时间 Disinfect_secCount
extern bool isRebirthDisinfect;  //消毒重生标志G_RebirthDisinfect_flag

//治疗时间(秒) 治疗初始设定
extern int total_secCount; //总
extern int remainCureSec;  //剩余  residue_secCount
extern unsigned int G_CURE_STARTTIME;  //治疗开始时间
extern unsigned int G_CURE_ENDTIME;    //治疗结束时间

extern DisinfectCond disinfectCond;//消毒电导
extern WorkState workstate;        //工作状态
extern PacketWorkMode G_WORKMODE_VAL;  //工作模式

//20170411调试模块#/
extern bool is_debug_formula_setted;    //水路调试中进行配方设置标记，true：设置中 G_SET_FORMULA
extern bool is_param_setted;   //参数调试模块进入标志，true：设置中 G_SET_PARAMFLAG
extern bool is_setup_setted;   //设置调试模块进入标志，true：设置中 G_SET_SETUPFLAG

//20151029重生模块#/
extern bool isRebirth;//G_rebirth_flag
extern struct STR_Rebirth *G_P_REBIRTH;
extern bool isCanSdoNormal;  //CANOPEN SDO是否正常状态 G_CANSDONORMAL_FLAG
extern bool isUFPumpPreStatus;  //上一次超滤泵状态 prestatusUFPump
extern bool isHeparinPumpPreStatus;//上一次肝素泵状态 prestatusHeparinPump
extern bool isSideWayPreStatus;//上一次旁路状态 prestatusSideWay
extern unsigned char dieTimes;         //死机次数 dietimes
extern unsigned char rebirthTimes;     //恢复次数 rebirthtimes
extern bool isEMSoundSilence;     //工程模式声音静音标志 G_sound_shielding
extern float G_UF_AutoChangeLimit;      //超滤速度自动改变时，限制的百分比范围 G_UF_autochangelimit
extern float G_SUB_AutoChangeLimit;   //置换泵速度自动改变时，限制的百分比范围 G_SUB_autochangelimit

//超滤与时间同步
extern unsigned int G_UF_STARTTIME;      //超滤开启时间 单位秒#/
extern unsigned int G_UF_ENDTIME;        //超滤的结束时间#/
extern unsigned int G_SYS_CURTIME;       //系统当前时间#/
extern int G_UF_EARLIER_TIME;            //UF量提前完成的时间#/
extern unsigned int G_UF_SETRATE;        //UF的设置速度#/
extern bool IS_UF_FINISH_CURRENT_TARGET;    //超滤泵完成本次超滤目标G_UF_FINISHVOL_FLAG

//治疗模式:单超
extern int ISO_TotalSec;     //总时间ISO_totalsec
extern int ISO_RemainSec;   //单超剩余时间ISO_residuesec
extern int ISO_TotalVolume;  //单超总目标量ISO_totalvolume
extern int ISO_FinishedVolume; //单超已完成量ISO_finishvolume
extern int UFPump_Now_volume; //开始ISO时的已超滤量

//超滤量
extern int UF_PRIME_TARGET_AMOUNT;   //预冲阶段超滤目标量 G_UF_PRETARGET
extern int UF_CURE_REMAIN_AMOUNT;  //治疗剩余的超滤量 G_UF_CURERESIDUE
extern int UF_CURE_TARGET_AMOUNT;  //治疗阶段目标超滤量 G_UF_CURETARGET
//超滤泵
extern int RealTime_UF_PreFinished_Amount;   //实时已预冲超滤量G_UF_PREFINISH
extern int UF_CureFinished_Amount;  //治疗阶段已完成超滤量G_UF_CUREFINISH
extern int UFPump_Pre_Amount;   //超滤泵预冲累计量
extern int UFPump_Cure_Amount;   //超滤泵治疗累计量
extern int Static_UF_Amount_Base;  //UF流量计算基数static_UF_amount

//血泵流量
extern int RT_POrC_BP_Volume;     //预冲或治疗时 实时血泵流量 BloodPump_Volume
extern int BloodPump_Pre_Amount;   //血泵预冲累计量
extern int BloodPump_Cure_Amount;  //血泵治疗累计量
extern unsigned short int Static_BP_Amount_Base;//血泵流量计算基数static_BP_amount
//血泵速度
extern int EM_GetBlood_BPRate;   //工程设置--引血血泵速度G_BPRATE_GETBLOOD
extern int EM_Cure_BPRate;      //工程设置--治疗血泵速度G_BPRATE_TREAT
extern int EM_ReturnBlood_BPRate; //工程设置--回血血泵速度G_BPRATE_RETURNBLOOD
extern unsigned short int G_SET_BPSPEED; //血泵速度设定值,防止乱跳

//肝素泵
extern int heparinPreStop_sec;  //预先停止时间 秒#/
extern bool isHeparinPreStop;     //肝素泵运行时间到
extern bool isHeparinPumpFirstRun; //肝素泵首剂量是否已运行HeparinPumpFirstflag
extern int Heparin_Inject_Volume;   //肝书泵的注射器容量G_HEPARIN_STYLE
extern bool isHeparinEnable;  //肝素泵使能标志G_HeparinEnable_flag

//置换泵
extern bool IS_SUB_SINGLE_ADD_FLOW;     //一次性补液标志 true:补液中 G_SUB_RAPID_FLAG
extern int G_SUB_SETSPEED;          //置换泵设置速度，防止置换泵速度乱跳#/
extern int SubsPump_TotalVolume;   //置换泵总目标量SubsPump_totalvolume
extern int SubsPump_FinishedVolume;  //置换泵已完成量SubsPump_finishvolume

//治疗曲线
#include "curve_datahandle.h"
//NA
extern bool ISNA_PROFILE_START;     //NA曲线开始G_NA_PROFILE_FLAG
extern char NA_PROFILE_START_NUM;      //NA曲线开始段G_NA_PROFILE_NUM
extern unsigned int G_NA_PROFILE_START;  //NA曲线开始时间
extern unsigned int G_NA_PROFILE_END;    //NA曲线结束时间
extern NA_Profile_Array A_NA_PROFILE[];  //NA曲线数据标志
extern char G_NA_SELECTCURVE_NUM;    //当前选择运行的曲线号
extern float G_PRE_NA_VALUE;    //初始化的NA离子值

//UF
extern bool IS_UF_PROFILE_START;  //UF曲线开始G_UF_PROFILE_FLAG
extern char UF_PROFILE_START_NUM;   //UF曲线开始段G_UF_PROFILE_NUM
extern unsigned int G_UF_PROFILE_START;  //UF曲线开始时间#/
extern unsigned int G_UF_PROFILE_END;   //UF曲线结束时间#/
extern UF_Profile_Array A_UF_PROFILE[];
extern unsigned int G_UF_PROFILE_STARTVOL; //UF曲线时段初始超滤量#/
extern unsigned int G_UF_PROFILE_PERTOTALVOL; //UF曲线每时段目标超滤量#/
extern char G_UF_SELECTCURVE_NUM;    //当前选择运行的曲线号#/

//透析液
extern bool IS_DIA_PROFILE_START;     //曲线开始G_DIA_PROFILE_FLAG
extern char DIA_PROFILE_RUNNING_NUM;      //曲线当前运行段G_DIA_PROFILE_NUM
extern unsigned int G_DIA_PROFILE_START;  //曲线开始时间
extern unsigned int G_DIA_PROFILE_END;    //曲线结束时间
extern unsigned int G_DIA_PROFILE_RELVULE;    //曲线运行前的设置值
extern Cure_Profile_Array A_DIA_PROFILE[];  //曲线数据标志

//肝素
extern bool IS_HEP_PROFILE_START;     //肝素曲线开始G_HEP_PROFILE_FLAG
extern char HEP_PROFILE_START_NUM;      //曲线开始段G_HEP_PROFILE_NUM
extern unsigned int G_HEP_PROFILE_START;  //曲线开始时间#/
extern unsigned int G_HEP_PROFILE_END;    //曲线结束时间#/
extern unsigned int G_HEP_PROFILE_RELVULE;    //曲线运行前的设置值#/
extern Cure_Profile_Array A_HEP_PROFILE[];  //曲线数据标志数组#/

//电导
extern bool IS_COND_PROFILE_START;     //电导曲线开始G_COND_PROFILE_FLAG
extern char COND_PROFILE_START_NUM;      //曲线开始段G_COND_PROFILE_NUM
extern unsigned int G_COND_PROFILE_START;  //曲线开始时间
extern unsigned int G_COND_PROFILE_END;    //曲线结束时间
extern unsigned int G_COND_PROFILE_RELVULE;    //曲线运行前的设置值#/
extern Cure_Profile_Array A_COND_PROFILE[];  //曲线数据标志

//温度
extern bool IS_TEMP_PROFILE_START;     //温度曲线开始G_TEMP_PROFILE_FLAG
extern char TEMP_PROFILE_START_NUM;      //曲线开始段G_TEMP_PROFILE_NUM
extern unsigned int G_TEMP_PROFILE_START;  //曲线开始
extern unsigned int G_TEMP_PROFILE_END;    //曲线结束
extern unsigned int G_TEMP_PROFILE_RELVULE;    //曲线运行前的设置值#/
extern Cure_Profile_Array A_TEMP_PROFILE[];  //曲线数据标志

//HCO3
extern bool IS_HCO3_PROFILE_START;     //曲线开始 G_HCO3_PROFILE_FLAG
extern char HCO3_PROFILE_START_NUM;      //曲线开始段 G_HCO3_PROFILE_NUM
extern unsigned int G_HCO3_PROFILE_START;  //曲线开始 G_HCO3_PROFILE_START
extern unsigned int G_HCO3_PROFILE_END;    //曲线结束 G_HCO3_PROFILE_END
extern unsigned int G_HCO3_PROFILE_RELVULE;    //曲线运行前的设置值
extern Cure_Profile_Array A_HCO3_PROFILE[];  //曲线数据标志
extern bool isDisinfectPipeOk;  //消毒时管路状态 iSPipeok
extern int language;

#include "scene_note.h"
extern Window_Note *NoteDialog;
extern Window_Note *CureFinishDlg;
extern Window_Confirm *ConfirmDlgAP;
extern Window_Confirm *ConfirmDlgVP;
extern Window_Confirm *ConfirmDlgTMP;
extern Window_Note *NoteCureMode;
extern Window_Note *CureLeaveDlg;
extern Window_Note *NoteReturnReady;
extern Window_YESNO *YesNoDialog;
extern Window_YES   *YesDialog;
extern PackN1PDO0CH PDOstate;
extern Window_Passwd *PasswdDialog;
extern Mini_Keyboard *NumberKeyboard;
extern Window_ChangeFilter *ChangeFilterDlg;

//按键中断
#include "buttosirq.h"
#include "buttons.h"
extern buttonsirq buttonsirqApp;
extern void buttons(int key);

#if HJX_COMMUNICATION_SWITCH
extern DataCanSend *dataCanSend;//canopen发送类#/
#endif

//通信 锁
extern QMutex global_mutex;
extern QMutex Map_mutex;
extern QSemaphore sem;//信号量
extern QSemaphore startsend_sem;  //CAN发送数据信号量
extern QMutex G_CanSend_Mutex;   //CAN发送数据互斥锁
//extern QMutex G_log_mutex;     //操作日志互斥

//判断各个板子canopen通信失败标志
extern char N3_CANOPEN_FLAG;
extern char N5_CANOPEN_FLAG;
extern char N9_CANOPEN_FLAG;
extern char N10_CANOPEN_FLAG;
extern char N15_CANOPEN_FLAG;
extern char N7_CANOPEN_FLAG;

//报警相关
extern bool isShieldAlarmLight; //屏蔽灯光Alarm_light_shielding
extern bool isShieldAlarmSound; //屏蔽声音Alarm_sound_shielding
extern bool isPreDisinfectNoCompleted; //上次消毒Disinfection_No_completed
extern bool isBPumpLinkSubPump;  //血泵1联动置换泵 true联动BloodPump_link_flag
extern bool isGetBloodOnline;   //在线引血Blood_online_lead
extern bool isReturnBloodOnline; //在线回血Blood_online_return

//卡标标志
extern bool isAPAlarmLock;//AlarmLock_AP
extern bool isVPAlarmLock;//AlarmLock_VP
extern bool isTMPAlarmLock;//AlarmLock_TMP
extern bool isWaterWayAlarmLinkSub;   //水路报警,联动置换泵 Waterway_alarm_flag
extern unsigned char SideWayButton;  //旁路按键 G_BYPASS_BUT_FLAG

extern int singleSubsSpeedAdd;  //单次置换的速度选择 SubsAddnum
extern unsigned char startPreFlushMode;  //启动预冲或引血的方式 Startpreflush_mode
extern unsigned char startReturnBloodMode; //启动回血的方式 Startreturnblood_mode

//附加功能
//血压计参数
extern PacketBPMParam packetBPMparam;

//-----------------------病人信息系统部分----------------------------------
extern int G_PATIENT_IDNUM_NOW;  //当前病人ID号
extern int G_PATIENT_CUREID_NOW;    //当前病人治疗次号
extern int G_PATIENT_CURESUM_NOW;  //当前病人治疗总次数
extern QMap<int,stpatientinfor> G_MAP_PATIENT;   //病人信息数据
extern QMap<int,stcuredatainfor> G_MAP_CUREDATA; //病人治疗记录数据

#include "scene_log_run.h"
extern LogRunModel *LogRun;

#endif
