#ifndef __DATASET_H__
#define __DATASET_H__

#include <QString>
#include <QMap>

#include "data_timer.h"
#include "Alarm_module.h"
#include "curve_datahandle.h"
#

/**
 * 配方 数据缓冲区
 * @brief The FormulaSet struct
 */
struct FormulaSet{
    int use_formula;

    struct FormulaTarget {
        short int scale_b;
        short int scale_water;
        short int target_b;
        short int target_mix;
    }outTarget;

    struct FormuladataBuffer {
        float scale_b;
        float scale_water;
        float naac;
        float stand_na;
        float cl;
        float k;
        float stand_hco3;
        float mg;
        float ace;
        float ca;
        float bnacl;
        float na;
        float hco3;
        float stand_b_workmode;
        float stand_water_workmode;
        float stand_na_workmode;
        float stand_hco3_workmode;
    }tempBuffer;
};


//20151029重生模块---恢复参数列表#/
struct STR_Rebirth
{
    bool selfcheck_enable;   //自检标志#/
    bool heparinPreStop;     //肝素泵的运行时间到标志#/
    bool HeparinPumpFirstflag; //肝素泵首剂量是否已运行标志
    bool BloodPump_link_flag;  //血泵1和置换泵联动标志 true：联动 false：不联动#/
    bool G_UF_FINISHVOL_FLAG;    //超滤泵已完成本次超滤目标的标志#/
    bool Blood_online_lead;   //在线引血标志#/
    bool Blood_online_return; //在线回血标志#/
    bool prestatusUFPump;  //上一次超滤泵状态#/
    bool prestatusHeparinPump;
    bool prestatusSideWay;
    bool G_NA_PROFILE_FLAG;     //NA曲线开始标志#/
    bool G_UF_PROFILE_FLAG;  //UF曲线开始标志#/
    bool iSPipeok;  //消毒时管路状态#/

    unsigned char dietimes;         //死机次数#/
    unsigned char rebirthtimes;     //恢复次数#/
    unsigned char Startpreflush_mode;  //启动预冲或引血的方式#/
    unsigned char Startreturnblood_mode; //启动回血的方式#/
    unsigned char G_BYPASS_BUT_FLAG;  //旁路按键标志#/
    char G_NA_PROFILE_NUM;      //NA曲线从第几段开始#/
    char G_NA_SELECTCURVE_NUM;    //当前选择运行的曲线号#/
    char G_UF_PROFILE_NUM;   //UF曲线从第几段开始#/
    char G_UF_SELECTCURVE_NUM;    //当前选择运行的曲线号#/

    unsigned int G_UF_STARTTIME;      //超滤开启时间 单位秒#/
    unsigned int G_UF_ENDTIME;        //超滤的结束时间#/
    unsigned int G_UF_SETRATE;        //UF的设置速度#/
    unsigned int G_NA_PROFILE_START;  //NA曲线开始时间#/
    unsigned int G_NA_PROFILE_END;    //NA曲线结束时间#/
    unsigned int G_CURE_STARTTIME;  //治疗开始时间#/
    unsigned int G_CURE_ENDTIME;    //治疗结束时间#/
    unsigned int G_UF_PROFILE_START;  //UF曲线开始时间#/
    unsigned int G_UF_PROFILE_END;   //UF曲线结束时间#/
    unsigned int G_UF_PROFILE_STARTVOL; //UF曲线时段初始超滤量#/
    unsigned int G_UF_PROFILE_PERTOTALVOL; //UF曲线每时段目标超滤量#/

    int G_SAVECUREPARAM_FLAG; //返回准备状态时，是否保存治疗参数 0：不保存 1保存#/
    int total_secCount; //总治疗时间#/
    int residue_secCount;  //剩余治疗时间 秒#/
    int heparinPreStop_sec;  //肝素泵预先停止时间 秒#/
    int BloodPump_Volume;     //预冲或治疗时 实时血泵流量#/
    int BloodPump_Pre_Amount;   //血泵预冲累计量#/
    int BloodPump_Cure_Amount;  //血泵治疗累计量#/
    int static_BP_amount;//血泵流量计算基数#/
    int G_BPRATE_TREAT;      //治疗血泵速度#/
    int G_SET_BPSPEED; //血泵速度设定值，防止血泵速度乱跳#/
    int SubsPump_totalvolume;   //置换泵总目标量#/
    int SubsAddnum;  //单次置换的速度选择#/
    int G_UF_PRETARGET;   //预冲阶段超滤目标量#/
    int G_UF_CURERESIDUE;  //治疗剩余的超滤量#/
    int G_UF_CURETARGET;  //治疗阶段目标超滤量#/
    int G_UF_PREFINISH;   //实时已预冲超滤量#/
    int G_UF_CUREFINISH;  //治疗阶段已完成超滤量#/
    int UFPump_Pre_Amount;   //超滤泵预冲累计量#/
    int UFPump_Cure_Amount;   //超滤泵治疗累计量#/
    int static_UF_amount;  //UF流量计算基数#/
    int ISO_totalsec;     //单超总时间#/
    int ISO_residuesec;   //单超剩余时间#/
    int ISO_totalvolume;  //单超总目标量#/
    int ISO_finishvolume; //单超已完成量#/
    int UFPump_Now_volume; //开始ISO时，已超滤量#/
    int language;
    int Disinfect_secCount;           //消毒剩余时间#/

    float G_PRE_NA_VALUE;    //初始化的NA离子值#/
    UF_Profile_Array A_UF_PROFILE[10];
    NA_Profile_Array A_NA_PROFILE[10];  //NA曲线数据标志数组#/
    PacketPDOHeparinPump heparinPDO;  //肝素泵参数#/
    PacketSdoBlood sdoBlood;//血泵1参数#/
    PacketSubBloodPump SubBPump;  //置换泵参数#/
    PacketUF packetUF;  //超滤泵参数#/
    FormulaSet formulaSet;//配方参数表#/
    PacketFluid Fluid;  //透析液流速和温度#/
    PacketPreflush Preflush;         //预冲参数#/
    WorkState workstate;   //工作状态设置#/
    PacketWorkMode G_WORKMODE_VAL;  //工作模式#/
    Params_Disinfect Disinfect;  //消毒参数全局变量#/
    DisinfectCond disinfectCond;   //消毒电导#、
    PacketAP packetAP; // AP  动脉压
    PacketVP packetVP; // VP
    PacketTMP packetTMP; // TMP
    PacketCond packetCond;
    PacketBPMParam packetBPMparam;  //血压计参数#/
};//20151029重生模块---恢复参数列表#/



/*
//20151029重生模块---恢复参数列表#/
struct STR_Rebirth
{
    bool selfcheck_enable;   //自检标志#/
    int G_SAVECUREPARAM_FLAG; //返回准备状态时，是否保存治疗参数 0：不保存 1保存#/
    int total_secCount; //总治疗时间#/
    int residue_secCount;  //剩余治疗时间 秒#/

    int heparinPreStop_sec;  //肝素泵预先停止时间 秒#/
    bool heparinPreStop;     //肝素泵的运行时间到标志#/
    bool HeparinPumpFirstflag; //肝素泵首剂量是否已运行标志
    PacketPDOHeparinPump heparinPDO;  //肝素泵参数#/

    int BloodPump_Volume;     //预冲或治疗时 实时血泵流量#/
    int BloodPump_Pre_Amount;   //血泵预冲累计量#/
    int BloodPump_Cure_Amount;  //血泵治疗累计量#/ 
    int static_BP_amount;//血泵流量计算基数#/
    int G_BPRATE_TREAT;      //治疗血泵速度#/
    bool BloodPump_link_flag;  //血泵1和置换泵联动标志 true：联动 false：不联动#/
    int G_SET_BPSPEED; //血泵速度设定值，防止血泵速度乱跳#/
    PacketSdoBlood sdoBlood;//血泵1参数#/

    int SubsPump_totalvolume;   //置换泵总目标量#/
    int SubsAddnum;  //单次置换的速度选择#/
    PacketSubBloodPump SubBPump;  //置换泵参数#/

    int G_UF_PRETARGET;   //预冲阶段超滤目标量#/
    int G_UF_CURERESIDUE;  //治疗剩余的超滤量#/
    int G_UF_CURETARGET;  //治疗阶段目标超滤量#/
    unsigned int G_UF_STARTTIME;      //超滤开启时间 单位秒#/
    unsigned int G_UF_ENDTIME;        //超滤的结束时间#/
    bool G_UF_FINISHVOL_FLAG;    //超滤泵已完成本次超滤目标的标志#/
    int G_UF_PREFINISH;   //实时已预冲超滤量#/
    int G_UF_CUREFINISH;  //治疗阶段已完成超滤量#/
    int UFPump_Pre_Amount;   //超滤泵预冲累计量#/
    int UFPump_Cure_Amount;   //超滤泵治疗累计量#/
    int static_UF_amount;  //UF流量计算基数#/
    unsigned int G_UF_SETRATE;        //UF的设置速度#/
    PacketUF packetUF;  //超滤泵参数#/

    FormulaSet formulaSet;//配方参数表#/
    PacketFluid Fluid;  //透析液流速和温度#/

    bool Blood_online_lead;   //在线引血标志#/
    bool Blood_online_return; //在线回血标志#/
    unsigned char Startpreflush_mode;  //启动预冲或引血的方式#/
    unsigned char Startreturnblood_mode; //启动回血的方式#/
    PacketPreflush Preflush;         //预冲参数#/

    unsigned char G_BYPASS_BUT_FLAG;  //旁路按键标志#/
    unsigned int G_CURE_STARTTIME;  //治疗开始时间#/
    unsigned int G_CURE_ENDTIME;    //治疗结束时间#/

    bool prestatusUFPump;  //上一次超滤泵状态#/
    bool prestatusHeparinPump;
    bool prestatusSideWay;

    int ISO_totalsec;     //单超总时间#/
    int ISO_residuesec;   //单超剩余时间#/
    int ISO_totalvolume;  //单超总目标量#/
    int ISO_finishvolume; //单超已完成量#/
    int UFPump_Now_volume; //开始ISO时，已超滤量#/

    //NA曲线治疗相关#/
    bool G_NA_PROFILE_FLAG;     //NA曲线开始标志#/
    char G_NA_PROFILE_NUM;      //NA曲线从第几段开始#/
    unsigned int G_NA_PROFILE_START;  //NA曲线开始时间#/
    unsigned int G_NA_PROFILE_END;    //NA曲线结束时间#/
    NA_Profile_Array A_NA_PROFILE[10];  //NA曲线数据标志数组#/
    char G_NA_SELECTCURVE_NUM;    //当前选择运行的曲线号#/
    float G_PRE_NA_VALUE;    //初始化的NA离子值#/

    //UF曲线治疗相关#/
    bool G_UF_PROFILE_FLAG;  //UF曲线开始标志#/
    char G_UF_PROFILE_NUM;   //UF曲线从第几段开始#/
    unsigned int G_UF_PROFILE_START;  //UF曲线开始时间#/
    unsigned int G_UF_PROFILE_END;   //UF曲线结束时间#/
    UF_Profile_Array A_UF_PROFILE[10];
    unsigned int G_UF_PROFILE_STARTVOL; //UF曲线时段初始超滤量#/
    unsigned int G_UF_PROFILE_PERTOTALVOL; //UF曲线每时段目标超滤量#/
    char G_UF_SELECTCURVE_NUM;    //当前选择运行的曲线号#/

    int language;

    WorkState workstate;   //工作状态设置#/
    PacketWorkMode G_WORKMODE_VAL;  //工作模式#/

    int Disinfect_secCount;           //消毒剩余时间#/
    Params_Disinfect Disinfect;  //消毒参数全局变量#/
    DisinfectCond disinfectCond;   //消毒电导#、
    bool iSPipeok;  //消毒时管路状态#/

    PacketAP packetAP; // AP  动脉压
    PacketVP packetVP; // VP
    PacketTMP packetTMP; // TMP
    PacketCond packetCond;

    PacketBPMParam packetBPMparam;  //血压计参数#/
};//20151029重生模块---恢复参数列表#/
*/


//Timer Packet
struct PacketTimer {
    int timeCount;                //总时间长度，以MS为单位
    int timeOut;                  //定时时间间隔
}__attribute__ ((__packed__));


struct Params_Attribute {
    float default_value;
    float max_value;
    float min_value;
    float step_value;
};

struct Display_Attribute {
    bool AutoOffTime;
    bool RemainTime;
    bool LastPrograms;
    bool FitterChange;
};

//机器所有参数设置，工程模式下使用
struct Params {
    struct UF{    //超虑参数

        struct UF_Goal {
            struct Params_Attribute attribute;
        }UF_Goal;

        struct UF_Rate {
            struct Params_Attribute attribute;
        }UF_Rate;

        struct UF_Time {
            struct Params_Attribute attribute;
        }UF_Time;

        struct Earlier_Time {
            struct Params_Attribute attribute;
        }Earlier_Time;

        struct AutoChange_Limit {
            struct Params_Attribute attribute;
        }AutoChange_Limit;

    }Params_UF;

    struct Params_Dialyser {   //透析夜参数
        struct Temp {   //温度
            struct Params_Attribute attribute;
        }Temp;

        struct Cond {
            struct Params_Attribute attribute;
        }Cond;

        struct Rate {
            struct Params_Attribute attribute;
        }Rate;

        struct OnlineRate {
            struct Params_Attribute attribute;
        }OnlineRate;

    }Params_Dialyser;

    struct Params_BP {

        struct BP_Width {
            struct Params_Attribute attribute;
        }BP_Width;

        struct Rate_CheckSelf {
            struct Params_Attribute attribute;
        }Rate_CheckSelf;

        struct Rate_ThreatMent {
            struct Params_Attribute attribute;
        }Rate_ThreatMent;

        struct Rate_ReturnBlood {
            struct Params_Attribute attribute;
        }Rate_ReturnBlood;

        struct Rate_Preflush {
            struct Params_Attribute attribute;
        }Rate_Preflush;

        struct Rate_GetBlood {
            struct Params_Attribute attribute;
        }Rate_GetBlood;

    }Params_BP;

    struct Params_Heparin {
        struct First_Volume {
            struct Params_Attribute attribute;
        }First_Volume;

        struct Rate {
            struct Params_Attribute attribute;
        }Rate;

        struct Pre_StopTime {
            struct Params_Attribute attribute;
        }Pre_StopTime;

        struct add_Volume {
            struct Params_Attribute attribute;
        }add_Volume;

       /* struct Fast_Rate {
            struct Params_Attribute attribute;
        }Fast_Rate;

        struct Injector_Length {
            struct Params_Attribute attribute;
        }Injector_Length;

        struct Injector_Volume {
            struct Params_Attribute attribute;
        }Injector_Volume;*/


    }Params_Heparin;

    struct Params_SubP {
        struct Flux {
            struct Params_Attribute attribute;
        }Flux;

        struct Goal {
            struct Params_Attribute attribute;
        }Goal;

        struct Rapid_Rate {
            struct Params_Attribute attribute;
        }Rapid_Rate;

        struct AutoChange_Limit {
            struct Params_Attribute attribute;
        }AutoChange_Limit;

    }Params_SubP;


    struct Params_TreatTime {
        struct Hour {
            struct Params_Attribute attribute;
        }Hour;

        struct min {
            struct Params_Attribute attribute;
        }min;

    }Params_TreatTime;

    struct Params_PreFlush {
        struct BpRate_First_FiveMin {
            struct Params_Attribute attribute;
        }BpRate_First_FiveMin;

        struct BpRate {
            struct Params_Attribute attribute;
        }BpRate;//预冲时血泵速度
        struct Dialyser_Rate {
            struct Params_Attribute attribute;
        }Dialyser_Rate;

        struct Time {
            struct Params_Attribute attribute;
        }Time;

        struct PF_UF_Rate {
            struct Params_Attribute attribute;
        }PF_UF_Rate;

        struct PF_UF_Volume {
            struct Params_Attribute attribute;
        }PF_UF_Volume;

//        struct Blood_Return_Volume {
//            struct Params_Attribute attribute;
//        }Blood_Return_Volume;

        struct PF_Volume {
            struct Params_Attribute attribute;
        }PF_Volume;

        struct BpRate_Conect {
            struct Params_Attribute attribute;
        }BpRate_Conect;

        struct BpRate_phase1 {
            struct Params_Attribute attribute;
        }BpRate_phase1;

        struct BpRate_phase2 {
            struct Params_Attribute attribute;
        }BpRate_phase2;

        struct BpRate_phase3 {
            struct Params_Attribute attribute;
        }BpRate_phase3;

        struct TMPUp_PF { //跨膜压上限
            struct Params_Attribute attribute;
        }TMPUp_PF;

    }Params_PreFlush;

    struct Params_ColdFlush {//
        //struct PreFlush_Time {
            //struct Params_Attribute attribute;
        //}preFlush_Time;

        struct Loop_Time {
            struct Params_Attribute attribute;
        }loop_Time;

        /*struct Hold_Time {
            struct Params_Attribute attribute;
        }hold_Time;

        struct BackFlush_Time {
            struct Params_Attribute attribute;
        }backFlush_Time;*/

        struct Flush_flux {
            struct Params_Attribute attribute;
        }flush_flux;

       /* struct Loop_flux {
            struct Params_Attribute attribute;
        }loop_flux;

        struct Suck_Flux {
            struct Params_Attribute attribute;
        }suck_flux;*/

    }params_ColdFlush;

    struct Params_HotFlush {//
        //struct PreFlush_Time {
            //struct Params_Attribute attribute;
        //}preFlush_Time;

        struct Loop_Time {
            struct Params_Attribute attribute;
        }loop_Time;

       /* struct Hold_Time {
            struct Params_Attribute attribute;
        }hold_Time;

        struct BackFlush_Time {
            struct Params_Attribute attribute;
        }backFlush_Time;*/

        struct Flush_flux {
            struct Params_Attribute attribute;
        }flush_flux;

        //struct Loop_flux {
            //struct Params_Attribute attribute;
        //}loop_flux;

        struct TargetTemp {
            struct Params_Attribute attribute;
        }targetTemp;

        struct RealTmep {
            struct Params_Attribute attribute;
        }realTmep;

        //struct Suck_Flux {
            //struct Params_Attribute attribute;
       // }suck_flux;

    }params_HotFlush;


    struct Params_Chymistry_Citric {
        struct PreFlush_Time {
            struct Params_Attribute attribute;
        }preFlush_Time;

        struct Loop_Time {
            struct Params_Attribute attribute;
        }loop_Time;

        struct Hold_Time {
            struct Params_Attribute attribute;
        }hold_Time;

        struct BackFlush_Time {
            struct Params_Attribute attribute;
        }backFlush_Time;

        struct Flush_flux {
            struct Params_Attribute attribute;
        }flush_flux;

        struct Loop_flux {
            struct Params_Attribute attribute;
        }loop_flux;

        struct TargetTemp {
            struct Params_Attribute attribute;
        }targetTemp;

        struct RealTmep {
            struct Params_Attribute attribute;
        }realTmep;

        struct Conduction_MAX {
            struct Params_Attribute attribute;
        }conduction_MAX;

        struct Conduction_MIN {
            struct Params_Attribute attribute;
        }conduction_MIN;

        struct Suck_Flux {
            struct Params_Attribute attribute;
        }suck_flux;
    }params_Chymistry_Citric;//柠檬酸

    struct Params_Chymistry_Peracetic {//
        struct PreFlush_Time {
            struct Params_Attribute attribute;
        }preFlush_Time;

        struct Loop_Time {
            struct Params_Attribute attribute;
        }loop_Time;

        struct Hold_Time {
            struct Params_Attribute attribute;
        }hold_Time;

        struct BackFlush_Time {
            struct Params_Attribute attribute;
        }backFlush_Time;

        struct Flush_flux {
            struct Params_Attribute attribute;
        }flush_flux;

        struct Loop_flux {
            struct Params_Attribute attribute;
        }loop_flux;

        struct TargetTemp {
            struct Params_Attribute attribute;
        }targetTemp;

        struct RealTmep {
            struct Params_Attribute attribute;
        }realTmep;

        struct Conduction_MAX {
            struct Params_Attribute attribute;
        }conduction_MAX;

        struct Conduction_MIN {
            struct Params_Attribute attribute;
        }conduction_MIN;

        struct Suck_Flux {
            struct Params_Attribute attribute;
        }suck_flux;
    }params_Chymistry_Peracetic;//过氧乙酸

    struct Params_Chymistry_Reserve3 {
        struct PreFlush_Time {
            struct Params_Attribute attribute;
        }preFlush_Time;

        struct Loop_Time {
            struct Params_Attribute attribute;
        }loop_Time;

        struct Hold_Time {
            struct Params_Attribute attribute;
        }hold_Time;

        struct BackFlush_Time {
            struct Params_Attribute attribute;
        }backFlush_Time;

        struct Flush_flux {
            struct Params_Attribute attribute;
        }flush_flux;

        struct Loop_flux {
            struct Params_Attribute attribute;
        }loop_flux;

        struct TargetTemp {
            struct Params_Attribute attribute;
        }targetTemp;

        struct RealTmep {
            struct Params_Attribute attribute;
        }realTmep;

        struct Conduction_MAX {
            struct Params_Attribute attribute;
        }conduction_MAX;

        struct Conduction_MIN {
            struct Params_Attribute attribute;
        }conduction_MIN;
        struct Suck_Flux {
            struct Params_Attribute attribute;
        }suck_flux;
    }params_Chymistry_Reserve3;//化消3

    struct Params_Hot_Citric {
        struct PreFlush_Time {
            struct Params_Attribute attribute;
        }preFlush_Time;

        struct Loop_Time {
            struct Params_Attribute attribute;
        }loop_Time;

        struct Hold_Time {
            struct Params_Attribute attribute;
        }hold_Time;

        struct BackFlush_Time {
            struct Params_Attribute attribute;
        }backFlush_Time;

        struct Flush_flux {
            struct Params_Attribute attribute;
        }flush_flux;

        struct Loop_flux {
            struct Params_Attribute attribute;
        }loop_flux;

        struct TargetTemp {
            struct Params_Attribute attribute;
        }targetTemp;

        struct RealTmep {
            struct Params_Attribute attribute;
        }realTmep;

        struct Conduction_MAX {
            struct Params_Attribute attribute;
        }conduction_MAX;

        struct Conduction_MIN {
            struct Params_Attribute attribute;
        }conduction_MIN;
        struct Suck_Flux {
            struct Params_Attribute attribute;
        }suck_flux;
    }params_Hot_Citric;//热（柠檬酸）

    struct Params_Hot {
        struct PreFlush_Time {
            struct Params_Attribute attribute;
        }preFlush_Time;

        struct Loop_Time {
            struct Params_Attribute attribute;
        }loop_Time;

        struct BackFlush_Time {
            struct Params_Attribute attribute;
        }backFlush_Time;

        struct Flush_flux {
            struct Params_Attribute attribute;
        }flush_flux;

        struct Loop_flux {
            struct Params_Attribute attribute;
        }loop_flux;

        struct TargetTemp {
            struct Params_Attribute attribute;
        }targetTemp;

        struct RealTmep {
            struct Params_Attribute attribute;
        }realTmep;

    }params_Hot;//热消

    struct Params_Center_Chymistry_Input {
        struct PreFlush_Time {
            struct Params_Attribute attribute;
        }preFlush_Time;

        struct Hold_Time {
            struct Params_Attribute attribute;
        }hold_Time;

        struct Loop_Time {
            struct Params_Attribute attribute;
        }loop_Time;

        struct BackFlush_Time {
            struct Params_Attribute attribute;
        }backFlush_Time;

        struct Flush_flux {
            struct Params_Attribute attribute;
        }flush_flux;

        struct Loop_flux {
            struct Params_Attribute attribute;
        }loop_flux;

        struct Suck_Flux {
            struct Params_Attribute attribute;
        }suck_flux;

    }params_Center_Chymistry_Input;

    struct Params_Center_Chymistry_Flush {
        struct PreFlush_Time {
            struct Params_Attribute attribute;
        }preFlush_Time;

        struct Hold_Time {
            struct Params_Attribute attribute;
        }hold_Time;

        struct Loop_Time {
            struct Params_Attribute attribute;
        }loop_Time;

        struct BackFlush_Time {
            struct Params_Attribute attribute;
        }backFlush_Time;

        struct Flush_flux {
            struct Params_Attribute attribute;
        }flush_flux;

        struct Loop_flux {
            struct Params_Attribute attribute;
        }loop_flux;

        struct Suck_Flux {
            struct Params_Attribute attribute;
        }suck_flux;

    }params_Center_Chymistry_Flush;

    struct Params_Center_Hot {
        struct PreFlush_Time {
            struct Params_Attribute attribute;
        }preFlush_Time;

        struct Hold_Time {
            struct Params_Attribute attribute;
        }hold_Time;

        struct Loop_Time {
            struct Params_Attribute attribute;
        }loop_Time;

        struct BackFlush_Time {
            struct Params_Attribute attribute;
        }backFlush_Time;

        struct Flush_flux {
            struct Params_Attribute attribute;
        }flush_flux;

        struct Loop_flux {
            struct Params_Attribute attribute;
        }loop_flux;

        struct TargetTemp {
            struct Params_Attribute attribute;
        }targetTemp;

        struct Suck_Flux {
            struct Params_Attribute attribute;
        }suck_flux;

    }params_Center_Hot;

    struct Params_Center_Flush_Entry {
        struct PreFlush_Time {
            struct Params_Attribute attribute;
        }preFlush_Time;

        struct Hold_Time {
            struct Params_Attribute attribute;
        }hold_Time;

        struct Loop_Time {
            struct Params_Attribute attribute;
        }loop_Time;

        struct BackFlush_Time {
            struct Params_Attribute attribute;
        }backFlush_Time;

        struct Suck_Flux {
            struct Params_Attribute attribute;
        }suck_flux;

    }params_Center_Flush_Entry;

    struct Clean_cond {
        struct B_cond {
            struct Params_Attribute attribute;
        }b_cond;
        struct Mix_cond {
            struct Params_Attribute attribute;
        }mix_cond;

        struct Out_cond {
            struct Params_Attribute attribute;
        }out_cond;
       }clean_cond;

    struct Auto_scale {
        struct AP_SAFE_SCALE_UP {
            struct Params_Attribute attribute;
        }ap_ready_safe_scale_up;//准备状态和治疗状态下的安全卡标上限

        struct AP_SAFE_SCALE_LOW {
            struct Params_Attribute attribute;
        }ap_ready_safe_scale_low;//准备状态和治疗状态下的安全卡标下限

        struct AP_SAFE_SCALE_STOP_UP {
            struct Params_Attribute attribute;
        }ap_run_safe_scale_up;//治疗停止状态下的安全卡标上限

        struct AP_SAFE_SCALE_STOP_LOW {
            struct Params_Attribute attribute;
        }ap_run_safe_scale_low;//治疗停止状态下的安全卡标下限

        struct AP_AUTO_SCALE_STOP_UP {
            struct Params_Attribute attribute;
        }ap_stop_safe_scale_up;

        struct AP_AUTO_SCALE_STOP_LOW {
            struct Params_Attribute attribute;
        }ap_stop_safe_scale_low;

        struct AP_AUTO_SCALE_RUN_UP {
            struct Params_Attribute attribute;
        }ap_run_auto_scale_up;//治疗状态下的自动卡标上限计算值

        struct AP_AUTO_SCALE_RUN_LOW {
            struct Params_Attribute attribute;
        }ap_run_auto_scale_low;//治疗状态下的自动卡标上限计算值



        struct VP_SAFE_SCALE_UP {
            struct Params_Attribute attribute;
        }vp_ready_safe_scale_up;//准备状态和治疗状态下的安全卡标上限

        struct VP_SAFE_SCALE_LOW {
            struct Params_Attribute attribute;
        }vp_ready_safe_scale_low;//准备状态和治疗状态下的安全卡标下限

        struct VP_SAFE_SCALE_STOP_UP {
            struct Params_Attribute attribute;
        }vp_run_safe_scale_up;//治疗停止状态下的安全卡标上限

        struct VP_SAFE_SCALE_STOP_LOW {
            struct Params_Attribute attribute;
        }vp_run_safe_scale_low;//治疗停止状态下的安全卡标下限

        struct VP_AUTO_SCALE_STOP_UP {
            struct Params_Attribute attribute;
        }vp_stop_safe_scale_up;

        struct VP_AUTO_SCALE_STOP_LOW {
            struct Params_Attribute attribute;
        }vp_stop_safe_scale_low;

        struct VP_AUTO_SCALE_RUN_UP {
            struct Params_Attribute attribute;
        }vp_run_auto_scale_up;//治疗状态下的自动卡标上限计算值

        struct VP_AUTO_SCALE_RUN_LOW {
            struct Params_Attribute attribute;
        }vp_run_auto_scale_low;//治疗状态下的自动卡标上限计算值


        struct TMP_SAFE_SCALE_UP {
            struct Params_Attribute attribute;
        }tmp_ready_safe_scale_up;//准备状态和治疗状态下的安全卡标上限

        struct TMP_SAFE_SCALE_LOW {
            struct Params_Attribute attribute;
        }tmp_ready_safe_scale_low;//准备状态和治疗状态下的安全卡标下限

        struct TMP_SAFE_SCALE_STOP_UP {
            struct Params_Attribute attribute;
        }tmp_run_safe_scale_up;//治疗停止状态下的安全卡标上限

        struct TMP_SAFE_SCALE_STOP_LOW {
            struct Params_Attribute attribute;
        }tmp_run_safe_scale_low;//治疗停止状态下的安全卡标下限

        struct TMP_AUTO_SCALE_STOP_UP {
            struct Params_Attribute attribute;
        }tmp_stop_safe_scale_up;

        struct TMP_AUTO_SCALE_STOP_LOW {
            struct Params_Attribute attribute;
        }tmp_stop_safe_scale_low;

        struct TMP_AUTO_SCALE_RUN_UP {
            struct Params_Attribute attribute;
        }tmp_run_auto_scale_up;//治疗状态下的自动卡标上限计算值

        struct TMP_AUTO_SCALE_RUN_LOW {
            struct Params_Attribute attribute;
        }tmp_run_auto_scale_low;//治疗状态下的自动卡标上限计算值



        struct AP_SPAN {
            struct Params_Attribute attribute;
        }ap_span;


        struct VP_SPAN {
            struct Params_Attribute attribute;
        }vp_span;

        struct TEMP_SPAN {
            struct Params_Attribute attribute;
        }temp_span;

        struct Hold_Time {
            struct Params_Attribute attribute;
        }hold_time;

        struct Alarm_Time {
            struct Params_Attribute attribute;
        }alarm_time;
    }auto_scale;

};

struct Engineer{
    struct Factory{
        unsigned short Volume;
        struct Display {
            struct Display_Attribute Attribute;
        }display;
        char ChemicalDisinfectModel;
        char CentralDisinfectModel;
        char PrimeModel;
        unsigned char CureModeSelect;
        unsigned char Fluidmode;   //供液模式#/
        bool HeparinSelect;
        bool onlinegetblood;
        bool onlinereturnblood;
        bool naprofileflag;
        bool ufprofileflag;
        bool diaprofileflag;
        bool condprofileflag;
        bool tempprofileflag;
        bool hepprofileflag;
        bool hco3profileflag;
    }factoryset;
};

struct EngineeNAHO3
{
    QString na;
    QString hco3;
};

//function
void updateFaisonFormulaCond(struct FormulaSet* formulaSet);
float getThreeCurveParam(float treatTime, float water_value, float initValue, float maxValue, float maxTime, int min);
float getLineCurveParam(float treatTime, float maxTreatValue, float initValue, int min);
float getPaoWuLineCurveParam(float treatTime, float maxTreatValue, float maxValue, float maxValueTime, int min);
float getZhiShuCurveParam(float treatTime, float maxTreatValue, float initValue, int min);

//确认性消息框操作
void showMessagesDialog();
void closeMessagesDialog();
void clearMessagesDialogInfo();
void removeOneMessages(int key);
void updateMessageDialog(QMap<int, AlarmInfor> mapValue);
bool isMessageDialogShow();
bool isMessageInfoNull();

//报警消息框操作
void showAlarmDialog();
void closeAlarmDialog();
void clearAlarmDialogInfo();
void removeOneAlarm(int key);
void updateAlarmDialog(QMap<int, AlarmInfor> mapValue);
bool isAlarmDialogShow();
bool isAlarmInfoNull();

//警告消息框 会延时消失
void showWarnDialog();
void closeWarnDialog();
void clearWarnDialogInfo();
void removeOneWarn(int key);
void updateWarnDialog(QMap<int, AlarmInfor> mapValue);
bool isWarnInfoNull();

void closeAllDialog();

void showAllDialog();

//专用于具体状态的消息提示框
void showNoteDialog(const QString title);
void closeNoteDialog();
bool IsNoteDialogShow();

void showYesNoDialog(QString title,QString info);
void closeYesNoDialog();
bool IsYesNoDialogShow();

void showYesDialog(QString title,QString info);
void showYesDialogNoYes(QString title,QString info);
void closeYesDialog();
bool IsYesDialogShow();

void setAlarmDialogOnTop();

void showFilterDialog(QString title,QString info);

#endif
