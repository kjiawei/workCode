#ifndef PACKET_H
#define PACKET_H

#include "packet_ui.h"
#include "packet_can.h"

/*
 * Master Ctrl 主控板相关
 */
#define PACKET_STATUS_NOTE          10 //"Master Ctrl Status Note"

#define PACKET_MASTERCTRL_VALVE     11 //"Master Ctrl Valve"
#define PACKET_MASTERCTRL_OUTLET    12 //"Master Ctrl Outlet"
#define PACKET_MASTERCTRL_MIX       13 //"Master Ctrl Mix"
#define PACKET_MASTERCTRL_FLUIDB    14 //"Master Ctrl FluidB"
#define PACKET_MASTERCTRL_FLUIDAFTER    15 //"Master Ctrl FluidAfter"
#define PACKET_MASTERCTRL_UFVALUE     16 //"Master Ctrl UfValue"
#define PACKET_MASTERCTRL_UFPUMP    17 //"Master Ctrl UfPump"
#define PACKET_MASTERCTRL_HEAT      18 //"Master Ctrl Heat"
#define PACKET_MASTERCTRL_BPUMP     19 //"Master Ctrl BPump"
#define PACKET_MASTERCTRL_MIXPUMP   20 //"Master Ctrl MixPump"
#define PACKET_MASTERCTRL_DISPLAY   21 //"Master Ctrl Display"
#define PACKET_MASTERCTRL_DISPLAY_TABLE   22 //"Master Ctrl Display table"
#define PACKET_MASTERCTRL_SDO       23 //"Master Ctrl SDO"
#define PACKET_MASTERCTRL_PDOERR    24 //"Master PDO ERR"

//血泵管径和速度 （canopen操作）
#define PACKET_SDO_BLOOD            25 //"SDO Blood"
#define PACKET_SDO_HEPARIN          26 //"SDO Heparin"
#define PACKET_SDO_RANGETEMP        27 //"SDO RangeTemp"

/*
 * Monitor 监控板相关
 */
#define PACKET_MONITOR_WATER        28 //"Monitor WaterOut"
#define PACKET_MONITOR_PRESS        29 //"Monitor Press"
#define PACKET_MONITOR_BLOODST      30 //"Monitor BloodState"
#define PACKET_MONITOR_BPMST        31 //"Monitor BPMState"
#define PACKET_MONITOR_BPMMISC      32 //"Monitor BPMMisc"
#define PACKET_STATUS_MASTALARM     33 //"Master Ctrl Status Alarm"  //主控板报警PDO1
#define PACKET_STATUS_ALARM         34 //"Monitor Ctrl Status Alarm"//监控板板报警PDO1
#define PACKET_RESUME_ALARM         35 //"Resume Alarm"    //复位报警标记 可remove
#define PACKET_BPM_PARAM            36 //"Monitor Ctrl Status Alarm"

/*
 * Miscellaneous 其余相关
 */
#define PACKET_NAME_BLOODPUMP       37 //"Blood Pump"
#define PACKET_NAME_BLOODPUMPTUBE   38 //"Blood Pump Tube"

#define PACKET_NAME_SUBBLOODPUMP    39 //"SUB Pump"
#define PACKET_NAME_SUBBLOODPUMPTUBE 40 //"SUB Pump Tube"

//血泵2控制（canopen操作）
#define PACKET_PDO_SUBBLOODPUMP    41 //"SubBlood Pump"

#define PACKET_NAME_HEPARINPUMP     42 //"Heparin Pump"
#define PACKET_NAME_HEPARINDATA     43 //"Heparin Pump data"

#define PACKET_NAME_SELFTEST        44 //"Self-test"

//全部工作模式和状态（canopen操作）
#define PACKET_NAME_WORKMODE        45 //"Work Mode"
#define PACKET_NAME_SCENESWITCH     46 //"SceneSwitch"
#define PACKET_NAME_WORKSTATE       47 //"Workstate"

//动脉压上下限和血温（canopen操作）
#define PACKET_NAME_AP              48 //"PressureAP"
#define PACKET_NAME_SCALE_AP        49   //   "ScaleAP"
#define PACKET_NAME_UPDATE_AP       50    //   "UpdateAP"
#define PACKET_NAME_VP              51 //"PressureVP"
#define PACKET_NAME_SCALE_VP        52   //   "ScaleVP"
#define PACKET_NAME_UPDATE_VP       53  //     "UpdateVP"
//跨膜压上下限（canopen操作）
#define PACKET_NAME_TMP             54 //"PressureTMP"
#define PACKET_NAME_SCALE_TMP       55  //    "ScaleTMP"
#define PACKET_NAME_UPDATE_TMP      56  //     "UpdateTMP"
//电导率上下限（canopen操作）
#define PACKET_NAME_COND            57 //"COND"
#define PACKET_NAME_UF              58 //"UF"
#define PACKET_NAME_NOW_UF_VOLUME   59 //"Now UF volume"

#define PACKET_NAME_POWER           60 //"Power"   //对应电源板的PDO
#define PACKET1_NAME_BATTERY        61 //"battery"
#define PACKET2_NAME_POWERALARM     62  //"Poweralram"
#define PACKET3_NAME_POWERMORNITOR  63 //"Powermornitor"


#define PACKET_NAEME_POWER_WAKEUP   64 //"Power auto wakeup"
#define PACKET_NAME_ADJPRESSLIMIT   65 //"Adjpresslimit"
//透析夜、血液、肝素的温度和流速（canopen操作）
#define PACKET_NAME_DIALYSATE       66 //"Dialysate"
#define PACKET_PREFLUSH             67 //"Preflush"
#define PACKET_NAME_N1STATE         68 //"n1 state"
#define PACKET_NAME_BLOODVOLUME     69 // "Blood volume"

#define PACKET_NAME_BLOODTEMP  70 //"Blood temp"

#define PACKET_BMPRESULT  71 //"bmp result"

#define BLOOD_CHANGE_PREFLUSH  72 //"blood_change_preflush"

/*
 *
 */
//清洁相关控制（canopen操作）
#define Disinfect_Work_Model        73 //"DisinfectWorkModel"
//肝素控制（canopen操作）
#define PACKET_PDO_HEPARINPUMP      74 //"SEND_PDO_HEPARINPUMP"

/**
 *data set
 */
//A B液混合透析液的控制参数（canopen操作）
#define PACKET_FORMULA_TARGET       75 //"formula_out_target"

/**
 * Dialog packet
 */
#define PACKET_NAME_MESSAGES        76 //"Msg_History"

/**
 * 报警闪动按钮
 */
#define PACKET_NAME_BTNALARM        77 //"btn_alarm"

/**
 *
 */
#define PACKET_MAINTAIN_RETURN      78 //"maintain_return"

/**
 * 工程模式数据变化
 */
#define PACKET_MAINTAIN_PARAMS      79 //"maintain_params"
#define PACKET_PARAMS_UF            80 //"params_uf"
#define PACKET_ENGINEER             81 //"engineer"

#define PACKET_NA_CURVE_VALUE       82 //"na_curve_value"
#define PACKET_CURVE_UF_11TO30      83 //"curve_uf_11to30"
#define PACKET_BP_WIDTH             84 //"BP_Width_params"
/*
 * 是否血路ok？
 */
#define PACKET_BLOODROAD_OK         85 //"blood_road_ok"

/*
 *是否水路ok?
 */
#define PACKET_WATERROAD_OK         86 //"water_road_ok"

#define VEINS_HAVE_BLOOD            87 //"veins_have_blood"
//液位
#define BLOOD_LEVEL_OK              88 //"blood_level_ok"

//预冲方式
#define Preflush_Model              89 //"PreflushModel"

#define SelfCheck_Timer             90 //"SelfCheckTimer"

#define TREAT_TIMER                 91 //"treattimer"

//返回准备状态保存治疗参数#/
#define DE_STR_SAVECUREPARAM        92 //"savecureparam"

#define UPLANGUAGEXML               93 //"UpdataLanguageXml"

#define ALARMPRO                    94 //"Alarmpro"

#define  PACKET_BICARBONATE         95 //"BicarbonateHCO3"

#define  OWNNOTE         96 //"ownnote"   //有本机程序生成的提示

#define PACKET_RESETPARAMETERS      97 //"ResetParameters"  //重设参数标志

#define ALARM_SILENCE_SET   98 //"alarmsilence"  //报警静音

#define PACKET_REBIRTH_HANDLE   99 //"rebirth"       //重生启动操作处理#/
#define PACKET_REBIRTH_START    100 //"rebirthstart"  //重生启动开始#/

#define PACKET_MASTERCTRL_ANTRUM 101  //主控板-腔数量#/

#define PACKET_MASTERCTRL_NOWATER 102  //主控板-无液时间#/

#endif
