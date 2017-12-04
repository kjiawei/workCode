#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H

#include <list>
#include <iostream>
#include "state.h"
using namespace std;


#define DE_5058_VERSION  "V2.00.0"   //全功能初始版本#/
//#define DE_5058_VERSION  "V2.01.0"   //添加可控水路图功能#/


#include "lang_switch.h"
extern list<LangSetText*> listText;

#include "xml_config.h"
extern list<XmlConfig*> listXmlConfig;
extern QString XMLALARM_STR;

#include <QtSql>
extern QSqlDatabase db;
/*
extern QSqlQuery *sqlPower;
extern QSqlQuery *sqlHeparin;
extern QSqlQuery *sqlBloodPump;
*/

extern QSqlDatabase disinfect_db;

extern QSqlDatabase runLog_db;


#include "func_modules.h"
extern QHash<QString, FuncModuleSeries*> hashFuncModule;

#include "input_panel_context.h"
extern InputPanelContext *ic;

#include <QWidget>
extern QWidget *window_parent; //所有窗口類的父類#/

#include "widget_data_curve.h"
extern Widget_Data_Curve *curveWidget;    //数据曲线控件#/

#include "Alarm_module.h"
extern AlarmModule *alarmmodule;    //报警处理模块#/

#include "audioserver.h"
extern audioserver *audio;

#include "lamp_task.h"
extern lamptask *lamp;

#include "data_manage.h"
extern DataManage *dataManage;

#include "page_info.h"
extern PageManage pageManage;

extern bool selfcheck_enable;

#include "data_timer.h"
extern data_timer* timer;

#include "widget_switch.h"
//extern MsgBox *msgBox;
extern QMessageBox *msgBox;

#include "engineering_mode.h"
extern Dialog *enginerDialog;

#include "data_set.h"
extern Engineer engineer;

extern struct Params usr_params; //用户相关参数集合#/
extern int G_SAVECUREPARAM_FLAG; //返回准备状态时，是否保存治疗参数 0：不保存 1保存#/


#include <packet_ui.h>
extern Params_Disinfect Disinfect;  //消毒参数全局变量#/
extern int Disinfect_secCount;           //消毒剩余时间#/
extern bool G_RebirthDisinfect_flag;  //消毒重生标志#/

extern int total_secCount; //总治疗时间#/

extern int heparinPreStop_sec;  //肝素泵预先停止时间 秒#/
extern bool heparinPreStop;     //肝素泵的运行时间到标志#/
extern bool HeparinPumpFirstflag; //肝素泵首剂量是否已运行标志
extern int G_HEPARIN_STYLE;   //肝书泵的注射器容量
extern bool G_HeparinEnable_flag;  //肝素泵使能标志#/

extern int residue_secCount;  //剩余治疗时间 秒#/


extern int G_UF_PRETARGET;   //预冲阶段超滤目标量#/
extern int G_UF_CURERESIDUE;  //治疗剩余的超滤量#/
extern int G_UF_CURETARGET;  //治疗阶段目标超滤量#/

extern int BloodPump_Volume;     //预冲或治疗时 实时血泵流量#/
extern int BloodPump_Pre_Amount;   //血泵预冲累计量#/
extern int BloodPump_Cure_Amount;  //血泵治疗累计量#/
extern unsigned short int static_BP_amount;//血泵流量计算基数#/

extern int G_BPRATE_GETBLOOD;   //工程设置--引血血泵速度#/
extern int G_BPRATE_TREAT;      //工程设置--治疗血泵速度#/
extern int G_BPRATE_RETURNBLOOD; //工程设置--回血血泵速度#/

extern unsigned short int G_SET_BPSPEED; //血泵速度设定值，防止血泵速度乱跳#/


extern int G_UF_PREFINISH;   //实时已预冲超滤量#/
extern int G_UF_CUREFINISH;  //治疗阶段已完成超滤量#/
extern int UFPump_Pre_Amount;   //超滤泵预冲累计量#/
extern int UFPump_Cure_Amount;   //超滤泵治疗累计量#/
extern int static_UF_amount;  //UF流量计算基数#/

extern int ISO_totalsec;     //单超总时间#/
extern int ISO_residuesec;   //单超剩余时间#/
extern int ISO_totalvolume;  //单超总目标量#/
extern int ISO_finishvolume; //单超已完成量#/
extern int UFPump_Now_volume; //开始ISO时，已超滤量#/

extern bool G_SUB_RAPID_FLAG;     //置换泵一次性补液标志 true:补液中#/
extern int G_SUB_SETSPEED;          //置换泵设置速度，防止置换泵速度乱跳#/
extern int SubsPump_totalvolume;   //置换泵总目标量#/
extern int SubsPump_finishvolume;  //置换泵已完成量#/


//治疗时间#/
extern unsigned int G_CURE_STARTTIME;  //治疗开始时间#/
extern unsigned int G_CURE_ENDTIME;    //治疗结束时间#/

//超滤与时间同步
extern unsigned int G_UF_STARTTIME;      //超滤开启时间 单位秒#/
extern unsigned int G_UF_ENDTIME;        //超滤的结束时间#/
extern unsigned int G_SYS_CURTIME;       //系统当前时间#/

extern int G_UF_EARLIER_TIME;            //UF量提前完成的时间#/
extern unsigned int G_UF_SETRATE;        //UF的设置速度#/
extern bool G_UF_FINISHVOL_FLAG;    //超滤泵已完成本次超滤目标的标志#/

#include "curve_datahandle.h"
//NA曲线治疗相关#/
extern bool G_NA_PROFILE_FLAG;     //NA曲线开始标志#/
extern char G_NA_PROFILE_NUM;      //NA曲线从第几段开始#/
extern unsigned int G_NA_PROFILE_START;  //NA曲线开始时间#/
extern unsigned int G_NA_PROFILE_END;    //NA曲线结束时间#/
extern NA_Profile_Array A_NA_PROFILE[];  //NA曲线数据标志数组#/
extern char G_NA_SELECTCURVE_NUM;    //当前选择运行的曲线号#/
extern float G_PRE_NA_VALUE;    //初始化的NA离子值#/



//UF曲线治疗相关#/
extern bool G_UF_PROFILE_FLAG;  //UF曲线开始标志#/
extern char G_UF_PROFILE_NUM;   //UF曲线从第几段开始#/
extern unsigned int G_UF_PROFILE_START;  //UF曲线开始时间#/
extern unsigned int G_UF_PROFILE_END;   //UF曲线结束时间#/
extern UF_Profile_Array A_UF_PROFILE[];
extern unsigned int G_UF_PROFILE_STARTVOL; //UF曲线时段初始超滤量#/
extern unsigned int G_UF_PROFILE_PERTOTALVOL; //UF曲线每时段目标超滤量#/
extern char G_UF_SELECTCURVE_NUM;    //当前选择运行的曲线号#/


//透析液曲线治疗相关#/
extern bool G_DIA_PROFILE_FLAG;     //曲线开始标志#/
extern char G_DIA_PROFILE_NUM;      //曲线当前运行第几段#/
extern unsigned int G_DIA_PROFILE_START;  //曲线开始时间#/
extern unsigned int G_DIA_PROFILE_END;    //曲线结束时间#/
extern unsigned int G_DIA_PROFILE_RELVULE;    //曲线运行前的设置值#/
extern Cure_Profile_Array A_DIA_PROFILE[];  //曲线数据标志数组#/

//肝素曲线治疗相关#/
extern bool G_HEP_PROFILE_FLAG;     //曲线开始标志#/
extern char G_HEP_PROFILE_NUM;      //曲线从第几段开始#/
extern unsigned int G_HEP_PROFILE_START;  //曲线开始时间#/
extern unsigned int G_HEP_PROFILE_END;    //曲线结束时间#/
extern unsigned int G_HEP_PROFILE_RELVULE;    //曲线运行前的设置值#/
extern Cure_Profile_Array A_HEP_PROFILE[];  //曲线数据标志数组#/


//电导曲线治疗相关#/
extern bool G_COND_PROFILE_FLAG;     //曲线开始标志#/
extern char G_COND_PROFILE_NUM;      //曲线从第几段开始#/
extern unsigned int G_COND_PROFILE_START;  //曲线开始时间#/
extern unsigned int G_COND_PROFILE_END;    //曲线结束时间#/
extern unsigned int G_COND_PROFILE_RELVULE;    //曲线运行前的设置值#/
extern Cure_Profile_Array A_COND_PROFILE[];  //曲线数据标志数组#/


//温度曲线治疗相关#/
extern bool G_TEMP_PROFILE_FLAG;     //曲线开始标志#/
extern char G_TEMP_PROFILE_NUM;      //曲线从第几段开始#/
extern unsigned int G_TEMP_PROFILE_START;  //曲线开始时间#/
extern unsigned int G_TEMP_PROFILE_END;    //曲线结束时间#/
extern unsigned int G_TEMP_PROFILE_RELVULE;    //曲线运行前的设置值#/
extern Cure_Profile_Array A_TEMP_PROFILE[];  //曲线数据标志数组#/


//HCO3曲线治疗相关#/
extern bool G_HCO3_PROFILE_FLAG;     //曲线开始标志#/
extern char G_HCO3_PROFILE_NUM;      //曲线从第几段开始#/
extern unsigned int G_HCO3_PROFILE_START;  //曲线开始时间#/
extern unsigned int G_HCO3_PROFILE_END;    //曲线结束时间#/
extern unsigned int G_HCO3_PROFILE_RELVULE;    //曲线运行前的设置值#/
extern Cure_Profile_Array A_HCO3_PROFILE[];  //曲线数据标志数组#/



extern bool iSPipeok;  //消毒时管路状态#/

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

#include "buttosirq.h"
#include "buttons.h"
extern buttonsirq buttonsirqApp;
extern void buttons(int key);

extern QMutex global_mutex;
extern QMutex Map_mutex;
extern QSemaphore sem;
extern QSemaphore startsend_sem;  //CAN 数据发送信号量#/
extern QMutex G_CanSend_Mutex;   //CAN 数据发送互斥锁#/
//extern QMutex G_log_mutex;     //操作日志互斥#/

extern DisinfectCond disinfectCond;

extern WorkState workstate;        //工作状态全局变量#/

extern PacketWorkMode G_WORKMODE_VAL;  //工作模式全局变量#/

extern bool Alarm_light_shielding; //报警灯光操作屏蔽#/

extern bool Alarm_sound_shielding; //报警声音操作屏蔽#/

extern bool Disinfection_No_completed; //上次消毒是否完成#/

extern bool BloodPump_link_flag;  //血泵1和置换泵联动标志 true：联动 false：不联动#/

extern bool Blood_online_lead;   //在线引血标志#/
extern bool Blood_online_return; //在线回血标志#/

//卡标标志
extern bool AlarmLock_AP;
extern bool AlarmLock_VP;
extern bool AlarmLock_TMP;

extern bool Waterway_alarm_flag;   //水路报警标志，用于置换泵联动#/

extern unsigned char G_BYPASS_BUT_FLAG;  //旁路按键标志#/

#if HJX_COMMUNICATION_SWITCH
extern DataCanSend *dataCanSend;//canopen发送类#/
#endif

extern int SubsAddnum;  //单次置换的速度选择#/

extern unsigned char Startpreflush_mode;  //启动预冲或引血的方式#/
extern unsigned char Startreturnblood_mode; //启动回血的方式#/

//判断各个板子canopen通信失败标志
extern char N3_CANOPEN_FLAG;
extern char N5_CANOPEN_FLAG;
extern char N9_CANOPEN_FLAG;
extern char N10_CANOPEN_FLAG;
extern char N15_CANOPEN_FLAG;
extern char N7_CANOPEN_FLAG;

extern PacketBPMParam packetBPMparam;  //血压计参数#/

//20170411调试模块#/
extern bool G_SET_FORMULA;    //水路调试中进行配方设置标记，true：设置中#/
extern bool G_SET_PARAMFLAG;   //参数调试模块进入标志，true：设置中#/
extern bool G_SET_SETUPFLAG;   //设置调试模块进入标志，true：设置中#/

//20151029重生模块#/
extern bool G_rebirth_flag;
extern struct STR_Rebirth *G_P_REBIRTH;

extern bool G_CANSDONORMAL_FLAG;  //CANOPEN SDO是否正常状态#/

extern bool prestatusUFPump;  //上一次超滤泵状态#/
extern bool prestatusHeparinPump;//上一次肝素泵状态#/
extern bool prestatusSideWay;//上一次旁路状态#/

extern unsigned char dietimes;         //死机次数#/
extern unsigned char rebirthtimes;     //恢复次数#/
extern bool G_sound_shielding;     //工程模式声音静音标志#/

extern float G_UF_autochangelimit;      //超滤速度自动改变时，限制的百分比范围#/
extern float G_SUB_autochangelimit;   //置换泵速度自动改变时，限制的百分比范围#/

//-----------------------病人信息系统部分----------------------------------
extern int G_PATIENT_IDNUM_NOW;  //当前病人ID号
extern int G_PATIENT_CUREID_NOW;    //当前病人治疗次号
extern int G_PATIENT_CURESUM_NOW;  //当前病人治疗总次数

extern QMap<int,stpatientinfor> G_MAP_PATIENT;   //病人信息数据
extern QMap<int,stcuredatainfor> G_MAP_CUREDATA; //病人治疗记录数据

#include "scene_log_run.h"
extern LogRunModel *LogRun;

#endif


