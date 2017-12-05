#include <list>
#include <iostream>
using namespace std;

#include "lang_switch.h"
list<LangSetText*> listText;

#include "xml_config.h"
list<XmlConfig*> listXmlConfig;

#include <QtSql>
QSqlDatabase db;
/*
QSqlQuery *sqlPower = NULL;
QSqlQuery *sqlHeparin = NULL;
QSqlQuery *sqlBloodPump = NULL;
*/

QSqlDatabase disinfect_db;

QSqlDatabase runLog_db;


#include "func_modules.h"
QHash<QString, FuncModuleSeries*> hashFuncModule;

#include <QWidget>
QWidget *window_parent = NULL;

#include "audioserver.h"
audioserver *audio;

#include "lamp_task.h"
lamptask *lamp;

#include "widget_switch.h"
//MsgBox *msgBox;
QMessageBox *msgBox=NULL;

#include "input_panel_context.h"
InputPanelContext *ic;

#include "data_manage.h"
DataManage *dataManage = NULL;

#include "page_info.h"
PageManage pageManage;

#include "data_timer.h"
data_timer* timer = NULL;

#include "engineering_mode.h"
Dialog *enginerDialog;

Engineer engineer;

#include "widget_data_curve.h"
Widget_Data_Curve *curveWidget=NULL;    //数据曲线控件#/

AlarmModule *alarmmodule=NULL;    //报警处理模块#/

bool isSelfCheckEnable = false;

struct Params usr_params;



int SaveCureParam_RetReady=0; //返回准备状态时，是否保存治疗参数#/

Params_Disinfect Disinfect;
int disinfect_remainSec=0;           //消毒剩余时间#/
bool isRebirthDisinfect=false;  //消毒重生标志#/

int total_secCount=0;           //总治疗时间#/

int UF_CURE_TARGET_AMOUNT=0;             //总超滤量#/

int heparinPreStop_sec=0;//肝素泵提前停止时间(秒)#/
bool isHeparinPumpFirstRun=false; //肝素泵首剂量是否已运行标志
int Heparin_Inject_Volume=0;   //肝书泵的注射器容量
bool isHeparinEnable=false;  //肝素泵使能标志#/

int remainCureSec=0;   //治疗剩余时间#/

int G_UF_EARLIER_TIME=0;

int UF_CURE_REMAIN_AMOUNT=0;      //剩下需要超滤量#/

int RealTime_UF_PreFinished_Amount=0;   //实时预冲超滤量#/
int UF_PRIME_TARGET_AMOUNT=0;   //预冲阶段超滤目标量#/

int UF_CureFinished_Amount=0;          //已完成超滤量#/

//超滤与时间同步
unsigned int G_UF_STARTTIME=0;      //超滤开启时间 单位秒#/
unsigned int G_UF_ENDTIME=0;        //超滤的总时间#/
unsigned int G_SYS_CURTIME=0;       //系统当前时间#/
unsigned int G_UF_SETRATE=0;        //UF的设置速度#/
bool IS_UF_FINISH_CURRENT_TARGET=false;    //超滤泵已完成本次超滤目标的标志#/

unsigned int G_CURE_STARTTIME=0;  //治疗开始时间#/
unsigned int G_CURE_ENDTIME=0;    //治疗结束时间#/

#include "curve_datahandle.h"
//NA曲线治疗相关#/
bool ISNA_PROFILE_START=false;     //NA曲线开始标志#/
char NA_PROFILE_START_NUM=0;      //NA曲线从第几段开始#/
unsigned int G_NA_PROFILE_START=0;  //NA曲线开始时间#/
unsigned int G_NA_PROFILE_END=0;    //NA曲线结束时间#/
NA_Profile_Array A_NA_PROFILE[10];  //NA曲线数据标志数组#/
char G_NA_SELECTCURVE_NUM=-1;    //当前选择运行的曲线号#/
float G_PRE_NA_VALUE=0;    //初始化的NA离子值#/

//UF曲线治疗相关#/
bool IS_UF_PROFILE_START=false;  //UF曲线开始标志#/
char UF_PROFILE_START_NUM=0;   //UF曲线从第几段开始#/
unsigned int G_UF_PROFILE_START=0;  //UF曲线开始时间#/
unsigned int G_UF_PROFILE_END=0;   //UF曲线结束时间#/
//UF_Profile_Array A_UF_PROFILE[10]={0,0,0};
UF_Profile_Array A_UF_PROFILE[10];
unsigned int G_UF_PROFILE_STARTVOL=0; //UF曲线时段初始超滤量#/
unsigned int G_UF_PROFILE_PERTOTALVOL=0; //UF曲线时段目标超滤量#/
char G_UF_SELECTCURVE_NUM=-1;    //当前选择运行的曲线号#/


//透析液曲线治疗相关#/
bool IS_DIA_PROFILE_START=false;     //曲线开始标志#/
char DIA_PROFILE_RUNNING_NUM=0;      //曲线从第几段开始#/
unsigned int G_DIA_PROFILE_START=0;  //曲线开始时间#/
unsigned int G_DIA_PROFILE_END=0;    //曲线结束时间#/
unsigned int G_DIA_PROFILE_RELVULE=0;    //曲线运行前的设置值#/
Cure_Profile_Array A_DIA_PROFILE[10];  //曲线数据标志数组#/


//肝素曲线治疗相关#/
bool IS_HEP_PROFILE_START=false;     //曲线开始标志#/
char HEP_PROFILE_START_NUM=0;      //曲线从第几段开始#/
unsigned int G_HEP_PROFILE_START=0;  //曲线开始时间#/
unsigned int G_HEP_PROFILE_END=0;    //曲线结束时间#/
unsigned int G_HEP_PROFILE_RELVULE=0;    //曲线运行前的设置值#/
Cure_Profile_Array A_HEP_PROFILE[10];  //曲线数据标志数组#/


//电导曲线治疗相关#/
bool IS_COND_PROFILE_START=false;     //曲线开始标志#/
char COND_PROFILE_START_NUM=0;      //曲线从第几段开始#/
unsigned int G_COND_PROFILE_START=0;  //曲线开始时间#/
unsigned int G_COND_PROFILE_END=0;    //曲线结束时间#/
unsigned int G_COND_PROFILE_RELVULE=0;    //曲线运行前的设置值#/
Cure_Profile_Array A_COND_PROFILE[10];  //曲线数据标志数组#/


//温度曲线治疗相关#/
bool IS_TEMP_PROFILE_START=false;     //曲线开始标志#/
char TEMP_PROFILE_START_NUM=0;      //曲线从第几段开始#/
unsigned int G_TEMP_PROFILE_START=0;  //曲线开始时间#/
unsigned int G_TEMP_PROFILE_END=0;    //曲线结束时间#/
unsigned int G_TEMP_PROFILE_RELVULE=0;    //曲线运行前的设置值#/
Cure_Profile_Array A_TEMP_PROFILE[10];  //曲线数据标志数组#/

//HCO3曲线治疗相关#/
bool IS_HCO3_PROFILE_START=false;     //曲线开始标志#/
char HCO3_PROFILE_START_NUM=0;      //曲线从第几段开始#/
unsigned int G_HCO3_PROFILE_START=0;  //曲线开始时间#/
unsigned int G_HCO3_PROFILE_END=0;    //曲线结束时间#/
unsigned int G_HCO3_PROFILE_RELVULE=0;    //曲线运行前的设置值#/
Cure_Profile_Array A_HCO3_PROFILE[10];  //曲线数据标志数组#/


bool isDisinfectPipeOk=true;//AB管、供回液管状态#/

int language;



#include "scene_note.h"
Window_Note *NoteDialog = NULL;
Window_Note *CureFinishDlg = NULL;
Window_Confirm *ConfirmDlgAP = NULL;
Window_Confirm *ConfirmDlgVP = NULL;
Window_Confirm *ConfirmDlgTMP = NULL;
Window_Note *NoteCureMode=NULL;
Window_Note *CureLeaveDlg = NULL;
Window_Note *NoteReturnReady=NULL;
Window_Passwd *PasswdDialog=NULL;
Window_YESNO *YesNoDialog=NULL;
Window_YES   *YesDialog=NULL;

Mini_Keyboard *NumberKeyboard=NULL;
Window_ChangeFilter *ChangeFilterDlg=NULL;

PackN1PDO0CH PDOstate;

#include "buttosirq.h"
buttonsirq buttonsirqApp;

QMutex global_mutex;   //can数据收处理互斥#/
QMutex Map_mutex;      //报警模块相关互斥#/
QMutex G_CanSend_Mutex;   //CAN 数据发送互斥锁#/
//QMutex G_log_mutex;     //操作日志互斥#/
QSemaphore startsend_sem(0);  //CAN 数据发送信号量#/
QSemaphore sem(3);

DisinfectCond disinfectCond;

WorkState workstate;
PacketWorkMode G_WORKMODE_VAL;  //工作模式全局变量#/

bool isHeparinPreStop=false;

bool isShieldAlarmLight=false; //报警灯光操作屏蔽#/

bool isShieldAlarmSound=false; //报警声音操作屏蔽#/

bool isPreDisinfectNoCompleted=false; //上次消毒是否完成#/


int RT_POrC_BP_Volume=0;
int BloodPump_Pre_Amount=0;   //血泵预冲累计量#/
int BloodPump_Cure_Amount=0;  //血泵治疗累计量#/
int EM_GetBlood_BPRate=100;   //引血血泵速度#/
int EM_Cure_BPRate=100;      //治疗血泵速度#/
int EM_ReturnBlood_BPRate=100; //回血血泵速度#/
unsigned short int Static_BP_Amount_Base = 0;//血泵流量计算基数#/


unsigned short int G_SET_BPSPEED=0; //血泵速度设定值，防止血泵速度乱跳#/

int UFPump_Pre_Amount=0;   //超滤泵预冲累计量#/
int UFPump_Cure_Amount=0;   //超滤泵治疗累计量#/
int Static_UF_Amount_Base=0;  //UF流量计算基数#/

int ISO_TotalSec=0;     //单超总时间#/
int ISO_RemainSec=0;   //单超剩余时间#/
int ISO_TotalVolume=0;  //单超总目标量#/
int ISO_FinishedVolume=0; //单超已完成量#/
int UFPump_Now_volume=0; //

bool IS_SUB_SINGLE_ADD_FLOW=false;     //置换泵一次性补液标志 true:补液中#/
int G_SUB_SETSPEED=0;          //置换泵设置速度，防止置换泵速度乱跳#/
int SubsPump_TotalVolume=0;   //置换泵总目标量#/
int SubsPump_FinishedVolume=0;  //置换泵已完成量#/

bool isBPumpLinkSubPump=false;  //血泵1和置换泵联动标志 true：联动 false：不联动#/
bool isGetBloodOnline=false;   //在线引血标志#/
bool isReturnBloodOnline=false; //在线回血标志#/

//卡标标志#/
bool isAPAlarmLock=false;
bool isVPAlarmLock=false;
bool isTMPAlarmLock=false;

bool isWaterWayAlarmLinkSub=false;

#if HJX_COMMUNICATION_SWITCH
DataCanSend *dataCanSend=NULL;//canopen发送类#/
#endif

int singleSubsSpeedAdd=0;  //单次置换的速度选择#/

unsigned char startPreFlushMode=0;  //启动预冲的方式#/
unsigned char startReturnBloodMode=0; //启动回血的方式#/

unsigned char SideWayButton=0;  //旁路按键标志#/

//判断各个板子canopen通信失败标志
char N3_CANOPEN_FLAG=0;
char N5_CANOPEN_FLAG=0;
char N9_CANOPEN_FLAG=0;
char N10_CANOPEN_FLAG=0;
char N15_CANOPEN_FLAG=0;
char N7_CANOPEN_FLAG=0;

PacketBPMParam packetBPMparam;  //血压计参数#/


//20170411调试模块#/
bool is_debug_formula_setted=false;    //水路调试中进行配方设置标记，true：设置中#/
bool is_param_setted=false;   //参数调试模块进入标志，true：设置中#/
bool is_setup_setted=false;   //设置调试模块进入标志，true：设置中#/

//20151029重生模块#/
bool isRebirth=false;
//恢复参数列表#/
struct STR_Rebirth *G_P_REBIRTH;

bool isCanSdoNormal=0;  //CANOPEN SDO是否正常状态#/

bool isUFPumpPreStatus=false;  //上一次超滤泵状态#/
bool isHeparinPumpPreStatus=false;
bool isSideWayPreStatus=false;

unsigned char dieTimes=0;         //死机次数#/
unsigned char rebirthTimes=0;     //恢复次数#/
bool isEMSoundSilence=false;     //工程模式声音静音标志#/

float G_UF_AutoChangeLimit=0;   //超滤速度自动改变时，限制的百分比范围#/
float G_SUB_AutoChangeLimit=0;   //置换泵速度自动改变时，限制的百分比范围#/


//-----------------------病人信息系统部分----------------------------------
int G_PATIENT_IDNUM_NOW=1;  //当前病人ID号  默认是1号
int G_PATIENT_CUREID_NOW=0;    //当前病人治疗次号
int G_PATIENT_CURESUM_NOW=0;  //当前病人治疗总次数

QMap<int,stpatientinfor> G_MAP_PATIENT;   //病人信息数据
QMap<int,stcuredatainfor> G_MAP_CUREDATA; //病人治疗记录数据



#include "scene_log_run.h"
LogRunModel *LogRun=NULL;

QString XMLALARM_STR;
