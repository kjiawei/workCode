/*****************************************************
** 作者： Huangjunxian
** 创始时间：2014-1-8
** 修改人：Huangjunxian
** 修改时间：2014-1-8
** 描述：
**    5058报警模块 《源文件》
*****************************************************/
#include "global_var.h"
#include "data_set.h"
#include "Alarm_module.h"
#include "common_tr_string.h"
#include "data_sqllite.h"
#include "deal.h"
#include "common_func.h"

//测试开关#/
#define DE_TEST_SWITCH 0

volatile bool Alarm_Handle_mutexflag=false;   //报警处理互斥标志，防止报警处理函数多次调用#/

bool Alarm_Reset_Finish=true;   //报警复位操作是否完成标志 true 完成

bool Alarm_Pass_Flag=false; //报警时，旁路处理标志#/

bool Alarm_BloodPump_Flag=false;  //发生停血泵报警标志#/
bool Alarm_UFPump_Flag=false;//发生停超滤泵报警标志#/
bool Alarm_SubsPump_Flag=false;//发生停置换泵报警标志#/
bool Alarm_HeparinPump_Flag=false;//发生停肝素泵报警标志#/

/*----------------------全局变量区域---------------------------*/

//告警编码表#/
StrAlarm alarm_table[25];

QMap<int,StrAlarm> curAlarmMap;//当前报警信息列表#/
QMap<int,StrAlarm> preAlarmMap;//上一次报警信息列表#/

QMap<int,AlarmInfor> curtimeAlarmMap;  //包含发生时间的当前报警保存区#/
QMap<int,AlarmInfor> historyAlarmMap;  //历史报警信息保存区,它的KEY值和上面的不同，只是一个顺序ID号#/

int historyAlarmID=0;     //记录历史告警记录条数，并且作为其索引#/

QList<int> onceAlarmList;  //一次性报警标示区#/

int alarmWidgetShow=0;   //指示历史报警页面是否显示 0：不显示  1：当前报警  2：历史报警#/

//通知右上角的图标按钮#/
unsigned int Alarmcolorflag = 0;

/*----------------------函数接口区域---------------------------*/

int cmp( const void *a ,const void *b)//从小到大排列#/
{
    return (*(AlarmInfor *)a).pri > (*(AlarmInfor *)b).pri ? 1 : -1;
}

void Search_alarminfo(int alarmid,QString &title,QString &info)
{
    switch(alarmid)
    {
    case ALARM_APUP://动脉压上限#/
        title += DE_TR_TITLE_APUP;
        info=DE_TR_ALARM_APUP;
        break;
    case ALARM_VPUP://静脉压上限#/
        title+=DE_TR_TITLE_VPUP;
        info=DE_TR_ALARM_VPUP;
        break;
    case ALARM_TMPUP://跨膜压上限#/
        title+=DE_TR_TITLE_TMPUP;
        info=DE_TR_ALARM_TMPUP;
        break;
    case ALARM_APDOWN://动脉压下限#/
        title+=DE_TR_TITLE_APDOWN;
        info=DE_TR_ALARM_APDOWN;
        break;
    case ALARM_VPDOWN://静脉压下限#/
        title+=DE_TR_TITLE_VPDOWN;
        info=DE_TR_ALARM_VPDOWN;
        break;
    case ALARM_TMPDOWN://跨膜压下限#/
        title+=DE_TR_TITLE_TMPDOWN;
        info=DE_TR_ALARM_TMPDOWN;
        break;
    case ALARM_TEMPERUP://血温上限#/
        title+=DE_TR_TITLE_TEMPERUP;
        info=DE_TR_ALARM_TEMPERUP;
        break;
    case ALARM_TEMPERDOWN://血温下限#/
        title+=DE_TR_TITLE_TEMPERDOWN;
        info=DE_TR_ALARM_TEMPERDOWN;
        break;
    case ALARM_FLUID: //液位报警#/
        title+=DE_TR_TITLE_FLUID;
        info=DE_TR_ALARM_FLUID;
        break;
    case ALARM_ARTERIES_AIR://动脉气泡#/
        title+=DE_TR_TITLE_ARTERIES_AIR;
        info=DE_TR_ALARM_ARTERIES_AIR;
        break;
    case ALARM_VEIN_AIR://静脉气泡#/
        title+=DE_TR_TITLE_VEIN_AIR;
        info=DE_TR_ALARM_VEIN_AIR;
        break;
    case ALARM_NO_BLOOD://无血#/
        title+=DE_TR_TITLE_NO_BLOOD;
        info=DE_TR_ALARM_NO_BLOOD;
        break;
    case ALARM_N9BLOODOUT://血泵1失控#/
        title+=DE_TR_TITLE_N9BLOODOUT;
        info=DE_TR_ALARM_N9BLOODOUT;
        break;
    case ALARM_N10BLOODOUT://血泵2失控#/
        title+=DE_TR_TITLE_N10BLOODOUT;
        info=DE_TR_ALARM_N10BLOODOUT;
        break;
    case ALARM_N9CAPOPEN://血泵1盖门打开#/
        title+=DE_TR_TITLE_N9CAPOPEN;
        info=DE_TR_ALARM_N9CAPOPEN;
        break;
    case ALARM_N10CAPOPEN://血泵2盖门打开#/
        title+=DE_TR_TITLE_N10CAPOPEN;
        info=DE_TR_ALARM_N10CAPOPEN;
        break;
    case ALARM_BLOODLEAK://漏血#/
        title+=DE_TR_TITLE_BLOODLEAK;
        info=DE_TR_ALARM_BLOODLEAK;
        break;
    case ALARM_HEPARINBLOCK://肝素空液、阻塞#/
        title+=DE_TR_TITLE_HEPARINBLOCK;
        info=DE_TR_ALARM_HEPARINBLOCK;
        break;
    case ALARM_BLOODMETER://血压计报警#/
        title+=DE_TR_TITLE_BLOODMETER;
        info=DE_TR_ALARM_BLOODMETER;
        break;
    case ALARM_BDPUP://透析器前压力上限#/
        title+=DE_TR_TITLE_BDPUP;
        info=DE_TR_ALARM_BDPUP;
        break;
    case ALARM_BDPDOWN://透析器前压力下限#/
        title+=DE_TR_TITLE_BDPDOWN;
        info=DE_TR_ALARM_BDPDOWN;
        break;
    case ALARM_OUTSUMCONDUP://出水口总电导度上限#/
        title+=DE_TR_TITLE_OUTSUMCONDUP;
        info=DE_TR_ALARM_OUTSUMCONDUP;
        break;
    case ALARM_OUTSUMCONDDOWN://出水口总电导度下限#/
        title+=DE_TR_TITLE_OUTSUMCONDDOWN;
        info=DE_TR_ALARM_OUTSUMCONDDOWN;
        break;
    case ALARM_OUTSUMCOND://出水口总电导度范围报警#/
        title+=DE_TR_TITLE_OUTSUMCOND;
        info=DE_TR_ALARM_OUTSUMCOND;
        break;
    case ALARM_MIXSUMCONDUP://混合总电导度上限#/
        title+=DE_TR_TITLE_MIXSUMCONDUP;
        info=DE_TR_ALARM_MIXSUMCONDUP;
        break;
    case ALARM_MIXSUMCONDDOWN://混合总电导度下限#/
        title+=DE_TR_TITLE_MIXSUMCONDDOWN;
        info=DE_TR_ALARM_MIXSUMCONDDOWN;
        break;
    case ALARM_MIXSUMCOND://混合总电导度范围报警#/
        title+=DE_TR_TITLE_MIXSUMCOND;
        info=DE_TR_ALARM_MIXSUMCOND;
        break;
    case ALARM_FTMPHIGHLIMIT://透析夜温度太高 极限#/
        title+=DE_TR_TITLE_FTMPHIGHLIMIT;
        info=DE_TR_ALARM_FTMPHIGHLIMIT;
        break;
    case ALARM_FTMPLOWLIMIT: //温度太低 极限#/
        title+=DE_TR_TITLE_FTMPLOWLIMIT;
        info=DE_TR_ALARM_FTMPLOWLIMIT;
        break;
    case ALARM_FTMPUP://温度上限#/
        title+=DE_TR_TITLE_FTMPUP;
        info=DE_TR_ALARM_FTMPUP;
        break;
    case ALARM_FTMPDOWN://温度下限#/
        title+=DE_TR_TITLE_FTMPDOWN;
        info=DE_TR_ALARM_FTMPDOWN;
        break;
    case ALARM_FTMPRANGE://温度范围报警#/
        title+=DE_TR_TITLE_FTMPRANGE;
        info=DE_TR_ALARM_FTMPRANGE;
        break;
    case ALARM_ACONDERROR:  //A液电导率错误#/
        title+=DE_TR_TITLE_ACONDERROR;
        info=DE_TR_ALARM_ACONDERROR;
        break;
    case ALARM_BCONDERROR://B液电导率错误#/
        title+=DE_TR_TITLE_BCONDERROR;
        info=DE_TR_ALARM_BCONDERROR;
        break;
    case ALARM_ANOFLUID: //A无液#/
        title+=DE_TR_TITLE_ANOFLUID;
        info=DE_TR_ALARM_ANOFLUID;
        break;
    case ALARM_BNOFLUID://B无液#/
        title+=DE_TR_TITLE_BNOFLUID;
        info=DE_TR_ALARM_BNOFLUID;
        break;
    case ALARM_NOWATER://无水报警#/
        title+=DE_TR_TITLE_NOWATER;
        info=DE_TR_ALARM_NOWATER;
        break;
    case ALARM_DISCONDUP://消毒液电导上限错误#/
        title+=DE_TR_TITLE_DISCONDUP;
        info=DE_TR_ALARM_DISCONDUP;
        break;
    case ALARM_DISCONDDOWN://消毒液电导下限错误#/
        title+=DE_TR_TITLE_DISCONDDOWN;
        info=DE_TR_ALARM_DISCONDDOWN;
        break;
    case ALARM_FLUIDOUTAIR://补液除气错误#/
        title+=DE_TR_TITLE_FLUIDOUTAIR;
        info=DE_TR_ALARM_FLUIDOUTAIR;
        break;
    case ALARM_BALANCEBREAK://平衡腔破膜报警#/
        title+=DE_TR_TITLE_BALANCEBREAK;
        info=DE_TR_ALARM_BALANCEBREAK;
        break;
    case ALARM_N3CANOPENFAIL://N3 canopen通信失败#/
        title+=DE_TR_TITLE_N3CANOPEN_FAIL;
        info+=DE_TR_PROMPT_N3CANOPEN_FAIL;
        break;
    case ALARM_N5CANOPENFAIL://N5 canopen通信失败#/
        title+=DE_TR_TITLE_N5CANOPEN_FAIL;
        info+=DE_TR_PROMPT_N5CANOPEN_FAIL;
        break;
    case ALARM_N9CANOPENFAIL: //N9 canopen通信失败#/
        title+=DE_TR_TITLE_N9CANOPEN_FAIL;
        info+=DE_TR_PROMPT_N9CANOPEN_FAIL;
        break;
    case ALARM_N10CANOPENFAIL://N10 canopen通信失败#/
        title+=DE_TR_TITLE_N10CANOPEN_FAIL;
        info+=DE_TR_PROMPT_N10CANOPEN_FAIL;
        break;
    case ALARM_N15CANOPENFAIL://N15 canopen通信失败#/
        title+=DE_TR_TITLE_N15CANOPEN_FAIL;
        info+=DE_TR_PROMPT_N15CANOPEN_FAIL;
        break;
    case ALARM_N7CANOPENFAIL://N7 canopen通信失败#/
        title+=DE_TR_TITLE_N7CANOPEN_FAIL;
        info+=DE_TR_PROMPT_N7CANOPEN_FAIL;
        break;
    case ALARM_NUSECALL://护士呼#/
        title+=DE_TR_TITLE_NURSECALL;
        info+=DE_TR_PROMPT_NURSECALL;
        break;
    case ALARM_ONETIME://单次定时报警提示#/
        title+=DE_TR_TITLE_ALARMONETIME;
        info+=DE_TR_PROMPT_ALARMONETIME;
        break;
    case ALARM_CYCLETIME://循环定时报警提示#/
        title+=DE_TR_TITLE_ALARMCYCLETIME;
        info+=DE_TR_PROMPT_ALARMCYCLETIME;
        break;
    case ALARM_WATERWORKFAIL://水路工作异常#/
        title+=DE_TR_TITLE_WATER_WORKFAIL;
        info+=DE_TR_PROMPT_WATER_WORKFAIL;
        break;        
    case PROMPT_MANUAL_PREFLUSH://主控板手动预冲状态错误#/
        title+=DE_TR_TITLE_MANUAL_PREFLUSH_ERROR;
        info+=DE_TR_PROMPT_MANUAL_PREFLUSH_ERROR;
        break;
    case PROMPT_AUTO_PREFLUSH://bit2：自动预冲#/
        title+=DE_TR_TITLE_AUTO_PREFLUSH_ERROR;
        info+=DE_TR_PROMPT_AUTO_PREFLUSH_ERROR;
        break;
    case PROMPT_ONLINE_PREFLUSH://bit3：在线预冲#/
        title+=DE_TR_TITLE_ONLINE_PREFLUSH_ERROR;
        info+=DE_TR_PROMPT_ONLINE_PREFLUSH_ERROR;
        break;
    case PROMPT_GETBLOOD://bit4：引血#/
        title+=DE_TR_TITLE_GETBLOOD_ERROR;
        info+=DE_TR_PROMPT_GETBLOOD_ERROR;
        break;
    case PROMPT_ONLINE_GETBLOOD://bit5：在线引血#/
        title+=DE_TR_TITLE_ONLINE_GETBLOOD_ERROR;
        info+=DE_TR_PROMPT_ONLINE_GETBLOOD_ERROR;
        break;
    case PROMPT_RETURNBLOOD://bit6：回血#/
        title+=DE_TR_TITLE_RETURNBLOOD_ERROR;
        info+=DE_TR_PROMPT_RETURNBLOOD_ERROR;
        break;
    case PROMPT_ONLINE_RETURNBLOOD://bit7：在线回血#/
        title+=DE_TR_TITLE_ONLINE_RETURNBLOOD_ERROR;
        info+=DE_TR_PROMPT_ONLINE_RETURNBLOOD_ERROR;
        break;

    case PROMPT_SYSTOLIC_UP:     //收缩压上限报警#/
        title+=DE_TR_TITLE_SYSTOLIC_UP_ALARM;
        info+=DE_TR_PROMPT_SYSTOLIC_UP_ALARM;
        break;

    case PROMPT_SYSTOLIC_DOWN: //收缩压下限报警#/
        title+=DE_TR_TITLE_SYSTOLIC_DOWN_ALARM;
        info+=DE_TR_PROMPT_SYSTOLIC_DOWN_ALARM;
        break;

    case PROMPT_DIASTOLIC_UP:  //舒张压UP报警#/
        title+=DE_TR_TITLE_DIASTOLIC_UP_ALARM;
        info+= DE_TR_PROMPT_DIASTOLIC_UP_ALARM;
        break;

    case PROMPT_DIASTOLIC_DOWN: //舒张压DOWN报警#/
        title+=DE_TR_TITLE_DIASTOLIC_DOWN_ALARM;
        info+= DE_TR_PROMPT_DIASTOLIC_DOWN_ALARM;
        break;

    case PROMPT_PULSE_UP:  //脉搏UP报警#/
        title+=DE_TR_TITLE_PULSE_UP_ALARM;
        info+=DE_TR_PROMPT_PULSE_UP_ALARM;
        break;

    case PROMPT_PULSE_DOWN:  //脉搏DOWN报警#/
        title+=DE_TR_TITLE_PULSE_DOWN_ALARM;
        info+=DE_TR_PROMPT_PULSE_DOWN_ALARM;
        break;

    case PROMPT_HEPARIN_TIMESTOP: //肝素泵时间到停止#/
        title+=DE_TR_TITLE_HEPARIN_TIMESTOP_ALARM;
        info+=DE_TR_PROMPT_HEPARIN_TIMESTOP_ALARM;
        break;

    case PROMPT_HEPARIN_NOOPEN: //肝素泵没有打开#/
        title+=DE_TR_TITLE_HEPARIN_PLEASEOPEN_PROMPT;
        info+=DE_TR_PROMPT_HEPARIN_PLEASEOPEN_PROMPT;
        break;

    case PROMPT_PLEASEOPENBLOOD: //报警处理完后，提示打开血泵#/
        title+=DE_TR_TITLE_PLEASEOPENBLOOD_PROMPT;
        info+=DE_TR_PROMPT_PLEASEOPENBLOOD_PROMPT;
        break;

    case PROMPT_XMLFILEERROR: //XML文件读取异常#/
        title+=DE_TR_TITLE_XMLFILEERROR_PROMPT;
        info=DE_TR_PROMPT_XMLFILEERROR_PROMPT;
        info+=XMLALARM_STR;
   //     XMLALARM_STR.clear();
        break;

   case ALARM_ELECTRICITY_STOP://N7 市电停止报警#/
        title+=DE_TR_TITLE_ELECTRICSUPPLYSTOP;
        info=DE_TR_PROMPT_ELECTRICSUPPLYSTOP;
        break;
    //提示性报警条目#/
    case PROMPT_ATOBARREL://将A液吸管放到A液桶#/
        title+=DE_TR_TITLE_ATOBARREL;
        info=DE_TR_PROMPT_ATOBARREL;
        break;
    case PROMPT_ATODEVICE://将A液吸管插回机器#/
        title+=DE_TR_TITLE_ATODEVICE;
        info=DE_TR_PROMPT_ATODEVICE;
        break;
    case PROMPT_BTOBARREL://将B液管放到B液桶#/
        title+=DE_TR_TITLE_BTOBARREL;
        info=DE_TR_PROMPT_BTOBARREL;
        break;
    case PROMPT_BTODEVICE://将B液管插回机器#/
        title+=DE_TR_TITLE_BTODEVICE;
        info=DE_TR_PROMPT_BTODEVICE;
        break;
    case PROMPT_FEEDTODIA://将供液管接到透析器#/
        title+=DE_TR_TITLE_FEEDTODIA;
        info=DE_TR_PROMPT_FEEDTODIA;
        break;
    case PROMPT_FEEDTODEVICE://将供液管接回机器#/
        title+=DE_TR_TITLE_FEEDTODEVICE;
        info=DE_TR_PROMPT_FEEDTODEVICE;
        break;
    case PROMPT_BACKTODIA://将回液管接到透析器#/
        title+=DE_TR_TITLE_BACKTODIA;
        info=DE_TR_PROMPT_BACKTODIA;
        break;
    case PROMPT_BACKTODEVICE://将回液管接回机器#/
        title+=DE_TR_TITLE_BACKTODEVICE;
        info=DE_TR_PROMPT_BACKTODEVICE;
        break;
    case PROMPT_DRYFRAMEOPEN://将干粉架打开#/
        title+=DE_TR_TITLE_DRYFRAMEOPEN;
        info=DE_TR_PROMPT_DRYFRAMEOPEN;
        break;
    case PROMPT_DRYFRAMECLOSE://将干粉架关闭#/
        title+=DE_TR_TITLE_DRYFRAMECLOSE;
        info=DE_TR_PROMPT_DRYFRAMECLOSE;
        break;
    case PROMPT_SETUPCORRESPONDING://将相应管路接到置换液接口#/
        title+=DE_TR_TITLE_SETUPCORRESPONDING;
        info=DE_TR_PROMPT_SETUPCORRESPONDING;
        break;
    case PROMPT_REMOVETHEINTERFACE: //将置换液接口管路移除#/
        title+=DE_TR_TITLE_REMOVETHEINTERFACE;
        info=DE_TR_PROMPT_REMOVETHEINTERFACE;
        break;
    case PROMPT_SETUPFLUSHINTERFACE: //将相应管路接到置换液冲洗接口#/
        title+=DE_TR_TITLE_SETUPFLUSHINTERFACE;
        info=DE_TR_PROMPT_SETUPFLUSHINTERFACE;
        break;
    case PROMPT_REMOVEFLUSHINTERFACE://将置换液冲洗接口管路移除#/
        title+=DE_TR_TITLE_REMOVEFLUSHINTERFACE;
        info=DE_TR_PROMPT_REMOVEFLUSHINTERFACE;
        break;
    case ALARM_SELFCHECK_CHOKECLIP1:
        title+=DE_TR_TITLE_SELFCHECK_CHOKECLIP1;
        info=DE_TR_PROMPT_SELFCHECK_CHOKECLIP1;
        break;
    case ALARM_SELFCHECK_CHOKECLIP2:
        title+=DE_TR_TITLE_SELFCHECK_CHOKECLIP2;
        info=DE_TR_PROMPT_SELFCHECK_CHOKECLIP2;
        break;
    case ALARM_SELFCHECK_VPBUBBLE:
        title+=DE_TR_TITLE_SELFCHECK_VPBUBBLE;
        info=DE_TR_PROMPT_SELFCHECK_VPBUBBLE;
        break;
    case ALARM_SELFCHECK_APBUBBLE:
        title+=DE_TR_TITLE_SELFCHECK_APBUBBLE;
        info=DE_TR_PROMPT_SELFCHECK_APBUBBLE;
        break;
    case ALARM_SELFCHECK_LEVEL:
        title+=DE_TR_TITLE_SELFCHECK_LEVEL;
        info=DE_TR_PROMPT_SELFCHECK_LEVEL;
        break;
    case ALARM_SELFCHECK_PA_ERROR:  //动脉压故障#/
        title+=DE_TR_TITLE_SELFCHECK_PA_ERROR;
        info=DE_TR_PROMPT_SELFCHECK_PA_ERROR;
        break;
    case ALARM_SELFCHECK_PV_ERROR: //静脉压故障#/
        title+=DE_TR_TITLE_SELFCHECK_PV_ERROR;
        info=DE_TR_PROMPT_SELFCHECK_PV_ERROR;
        break;
    case ALARM_SELFCHECK_TMP_ERROR:  //跨膜压故障#/
        title+=DE_TR_TITLE_SELFCHECK_TMP_ERROR;
        info=DE_TR_PROMPT_SELFCHECK_TMP_ERROR;
        break;
    case ALARM_SELFCHECK_BLOODLEAK:
        title+=DE_TR_TITLE_SELFCHECK_BLOODLEAK;
        info=DE_TR_PROMPT_SELFCHECK_BLOODLEAK;
        break;
    case ALARM_SELFCHECK_BLOODOXYGEN:
        title+=DE_TR_TITLE_SELFCHECK_BLOODOXYGEN;
        info=DE_TR_PROMPT_SELFCHECK_BLOODOXYGEN;
        break;
    case ALARM_SELFCHECK_TONOMETER:
        title+=DE_TR_TITLE_SELFCHECK_TONOMETER;
        info=DE_TR_PROMPT_SELFCHECK_TONOMETER;
        break;
    case ALARM_SELFCHECK_N9BLOODOUT:
        title+=DE_TR_TITLE_SELFCHECK_N9BLOODOUT;
        info=DE_TR_PROMPT_SELFCHECK_N9ISOUT;
        break;
    case ALARM_SELFCHECK_N9NOFEEDBACK:
        title+=DE_TR_TITLE_SELFCHECK_N9NOFEEDBACK;
        info=DE_TR_PROMPT_SELFCHECK_N9NOFEEDBACK;
        break;
    case ALARM_SELFCHECK_N9SELFCHECKFAIL:
        title+=DE_TR_TITLE_SELFCHECK_N9SELFCHECKFAIL;
        info=DE_TR_PROMPT_SELFCHECK_NO1BLOODPUMPSELFCHECKFAIL;
        break;      
    case ALARM_SELFCHECK_N10BLOODOUT://血泵2失控  特别设置，没使用#/
        title+=DE_TR_TITLE_SELFCHECK_N10BLOODOUT;
        info=DE_TR_PROMPT_SELFCHECK_N10ISOUT;
        break;
    case ALARM_SELFCHECK_N10NOFEEDBACK: //血泵2无反馈信号#/
        title+=DE_TR_TITLE_SELFCHECK_N10NOFEEDBACK;
        info=DE_TR_PROMPT_SELFCHECK_N10NOFEEDBACK;
        break;
    case ALARM_SELFCHECK_N10SELFCHECKFAIL: //血泵2自检失败#/
        title+=DE_TR_TITLE_SELFCHECK_N10SELFCHECKFAIL;
        info=DE_TR_PROMPT_SELFCHECK_NO2BLOODPUMPSELFCHECKFAIL;
        break;
    case ALARM_SELFCHECK_N15SELFCHECKFAIL:
        title+=DE_TR_TITLE_SELFCHECK_N15SELFCHECKFAIL;
        info=DE_TR_PROMPT_SELFCHECK_HEPARINSELFCHECKFAIL;
        break;
    case ALARM_SELFCHECK_BLOODPARTSELFCHECKFAIL:
        title+=DE_TR_TITLE_SELFCHECK_BLOODPARTSELFCHECKFAIL;
        info=DE_TR_PROMPT_SELFCHECK_BLOODPARTSELFCHECKFAIL;
        break;
    case ALARM_SELFCHECK_WATERPARTSELFCHECKFAIL:
        title+=DE_TR_TITLE_SELFCHECK_WATERPARTSELFCHECKFAIL;
        info=DE_TR_PROMPT_SELFCHECK_WATERPARTSELFCHECKFAIL;
        break;
    case ALARM_SELFCHECK_N7ERROR:
        //N7自检失败
        title+=DE_TR_TITLE_SELFCHECK_N7ERROR;
        info=DE_TR_PROMPT_SELFCHECK_N7SELFCHECKFAIL;
        break;
    case ALARM_SELFCHECK_N9ERROR:
        //N9自检失败
        title+=DE_TR_TITLE_SELFCHECK_N9ERROR;
        info=DE_TR_PROMPT_SELFCHECK_N9SELFCHECKFAIL;
        break;
    case ALARM_SELFCHECK_N10ERROR:
        //N10自检失败
        title+=DE_TR_TITLE_SELFCHECK_N10ERROR;
        info=DE_TR_PROMPT_SELFCHECK_N10SELFCHECKFAIL;
        break;
     case ALARM_SELFCHECK_N15ERROR:
        //N15自检失败
        title+=DE_TR_TITLE_SELFCHECK_N15ERROR;
        info=DE_TR_PROMPT_SELFCHECK_N15SELFCHECKFAIL;
        break;
     case ALARM_SELFCHECK_N5ERROR:
        //N5自检失败
        title+=DE_TR_TITLE_SELFCHECK_N5ERROR;
        info=DE_TR_PROMPT_SELFCHECK_N5SELFCHECKFAIL;
        break;
    default:
        if((alarmid/100000000)==3)
        {
            title+=DE_TR_TITLE_SOME_ERROR;
            info=DE_TR_PROMPT_ALARM_CODE;
        }
        else if((alarmid/100000000)==5)
        {
            title+=DE_TR_TITLE_SOME_ERROR;
            info=DE_TR_PROMPT_ALARM_CODE;
        }
        break;
    }
}

//根据报警来源ID，返回报警来源信息#/
QString Search_sourceinfo(int who)
{
    QString tmpstr;
/*
    switch(who)
    {
    case NOTE://提示性报警#/
        tmpstr=DE_TR_NOTE;
        break;
    case ALARM_PDO1://来源N3或者N5的PDO1
    case ALARM_PDO2:
        tmpstr=DE_TR_ALARM_PDO1;
        break;
    case ALARM_OWN://程序提示#/
        tmpstr=DE_TR_ALARM_OWN;
        break;
    case ALARM_N7://电源#/
        tmpstr=DE_TR_ALARM_N7;
        break;
    case ALARM_N9://血泵1#/
        tmpstr=DE_TR_ALARM_N9;
        break;
    case ALARM_N10://血泵2#/
        tmpstr=DE_TR_ALARM_N10;
        break;
    case ALARM_N15://肝素泵#/
        tmpstr=DE_TR_ALARM_N15;
        break;
    case ALARM_SELFCHECK:
        tmpstr=DE_TR_SELFCHECK;
        break;
    default:
        tmpstr=DE_TR_UNKNOWN;
        break;
    }
    return tmpstr;
    */

    switch(who)
    {
    case NOTE://提示性报警#/
        tmpstr=DE_TR_NOTE;
        break;
    case ALARM_N3://来源N3#/
        tmpstr="N3";
        break;
    case ALARM_N5://
        tmpstr="N5";
        break;
    case ALARM_N7://电源#/
        tmpstr="N7";
        break;
    case ALARM_N9://血泵1#/
        tmpstr="N9";
        break;
    case ALARM_N10://血泵2#/
        tmpstr="N10";
        break;
    case ALARM_N15://肝素泵#/
        tmpstr="N15";
        break;
    case ALARM_OWN://人机板#/
        tmpstr="N1";
        break;
    case ALARM_SELFCHECK:
        tmpstr=DE_TR_SELFCHECK;
        break;
    default:
        tmpstr=DE_TR_UNKNOWN;
        break;
    }
    return tmpstr;
}

//确认并且保存#/
void Confirm_to_histroy(AlarmInfor alarm)
{
    int key;
    char byte=(alarm.code>>16)&0xf0;
    unsigned int maxtime=0;
    key=alarm.alarmid;
    alarm.dispose=1;    //已确认#/

//    QMap<int,AlarmInfor>::iterator it;
    QMap<int,AlarmInfor>::iterator it;
    QMap<int,AlarmInfor>::iterator it2;

    //确认后，修改变量#/
    for(it=curtimeAlarmMap.begin();it!=curtimeAlarmMap.end();it++)
    {
        if((it.value().alarmid==alarm.alarmid)&&(it.value().time==alarm.time))
        {
            it.value().dispose=1;
        }
    }

    //一次性报警确认后，从当前报警消除#/
    if(byte==0x30&&(onceAlarmList.contains(key)))
    {
        curtimeAlarmMap.remove(key);
        historyAlarmMap.insert(historyAlarmID,alarm);
        historyAlarmID++;
        /*
        if(historyAlarmID>DE_HISTORYALARM_MAXNUM)
        {
            historyAlarmMap.remove(historyAlarmMap.begin().key());
        }
        */
    }

    //查找是否已经保存过了#/
    for(it2 = historyAlarmMap.begin(); it2 != historyAlarmMap.end(); it2++)
    {
        if(it2.value().alarmid==alarm.alarmid)
        {
            if(maxtime<it2.value().time)
                maxtime=it2.value().time;    //获取这类报警的最后一次时间#/
        }
    }

    //查询该条报警记录是否已保存进历史报警表#/
    if(DataSql_Check(alarm.time,alarm.alarmid))
    {
        //修改特定报警条目信息#/
        DataSql_UpdatewarnTable(alarm.time,alarm.alarmid,alarm.dispose);

        for(it2=historyAlarmMap.begin();it2!=historyAlarmMap.end();it2++)
        {
            if((it2.value().alarmid==alarm.alarmid)&&(it2.value().time==alarm.time))
            {
                it2.value().dispose=1;
            }
        }
    }
    else
    {
         //为了避免频繁的报警保存，因此暂定间隔在2分钟内的，不进行保存#/
        if((alarm.time-maxtime)>120)
        {
            //往报警表插入一条记录#/
            DataList_InsertwarnTable(alarm.time,alarm.who,alarm.code,alarm.pri,alarm.alarmid,alarm.dispose);
            if(historyAlarmID>DE_HISTORYALARM_MAXNUM)   //超过保存范围，则要进行自动删除最早的记录
            {
                DataSql_DeletewarnTable(historyAlarmMap.begin().value().time,historyAlarmMap.begin().value().alarmid);
                historyAlarmMap.remove(historyAlarmMap.begin().key());
            }
        }
    }
}

void Set_AlarmWidgetShow(int status)
{
    alarmWidgetShow=status;
}

//报警初始化操作#/
AlarmModule::AlarmModule(QObject *parent) : QObject(parent)
{
    parent=parent;
    curAlarmMap.clear();
    preAlarmMap.clear();
    curtimeAlarmMap.clear();
    historyAlarmMap.clear();

    memset(&alarm_N9,0,sizeof(PacketBloodPump));
    memset(&alarm_N10,0,sizeof(PacketBloodPump));
    memset(&note_N3,0,sizeof(PacketStatusNote));
    memset(&alarm_N3,0,sizeof(PacketStatusAlarm));
    memset(&alarm_N5,0,sizeof(PacketStatusAlarm));
    memset(&alarm_own,0,sizeof(PacketOwnNote));
    memset(&alarm_N15,0,sizeof(PackN15PDO));
    memset(&alarm_N7,0,sizeof(PacketPower));
    memset(&Prompt_own,0,sizeof(PacketOwnNote));

    this->stateflag=DE_NORMAL_FLAG;
/*
    alarm_table[0].code=0x00000000;
    alarm_table[0].pri=0;
    alarm_table[0].who=NOTE;
    alarm_table[1].code=0x00242202;//水路提示#/
    alarm_table[1].pri=2;
    alarm_table[1].who=NOTE;

    alarm_table[2].code=0x00202200;//血路#/
    alarm_table[2].pri=1;
    alarm_table[2].who=ALARM_PDO1;
    alarm_table[3].code=0x0f111101;//血路#/
    alarm_table[3].pri=3;
    alarm_table[3].who=ALARM_PDO1;
    alarm_table[4].code=0x0f242202;//血路#/
    alarm_table[4].pri=3;
    alarm_table[4].who=ALARM_PDO1;
    alarm_table[5].code=0x0f241101;
    alarm_table[5].pri=3;
    alarm_table[5].who=ALARM_PDO1;

    alarm_table[6].code=0x18241101;//液路#/
    alarm_table[6].pri=3;
    alarm_table[6].who=ALARM_PDO1;
    alarm_table[7].code=0x00310004;//液路#/
    alarm_table[7].pri=1;
    alarm_table[7].who=ALARM_PDO1;
    alarm_table[8].code=0x00340106;//液路#/
    alarm_table[8].pri=2;
    alarm_table[8].who=ALARM_PDO1;

    alarm_table[9].code=0x00112202;//程序提示#/
    alarm_table[9].pri=3;
    alarm_table[9].who=ALARM_OWN;
    alarm_table[10].code=0x00111101;//肝素泵#/
    alarm_table[10].pri=3;
    alarm_table[10].who=ALARM_N15;
    alarm_table[11].code=0x0f242202;//血泵1#/
    alarm_table[11].pri=3;
    alarm_table[11].who=ALARM_N9;
    alarm_table[12].code=0x0f242202;//血泵2#/
    alarm_table[12].pri=3;
    alarm_table[12].who=ALARM_N10;
    alarm_table[13].code=0x18241101;//液路#/
    alarm_table[13].pri=3;
    alarm_table[13].who=ALARM_PDO2;
    alarm_table[14].code=0x00111101;//电源#/
    alarm_table[14].pri=3;
    alarm_table[14].who=ALARM_N7;
    alarm_table[15].code=0x00111101;//程序提示#/
    alarm_table[15].pri=3;
    alarm_table[15].who=ALARM_OWN;
    alarm_table[16].code=0x00241101;
    alarm_table[16].pri=3;
    alarm_table[16].who=ALARM_SELFCHECK;
*/
    alarm_table[0].code=0x00000000;
    alarm_table[0].pri=0;
    alarm_table[0].who=NOTE;
    alarm_table[1].code=0x00241202;//水路提示#/
    alarm_table[1].pri=2;
    alarm_table[1].who=ALARM_N3;

    alarm_table[2].code=0x00202200;//血路#/
    alarm_table[2].pri=1;
    alarm_table[2].who=ALARM_N5;
    alarm_table[3].code=0x0f111101;//血路#/
    alarm_table[3].pri=3;
    alarm_table[3].who=ALARM_N5;

    alarm_table[4].code=0x0c242202;//BLOOD1#/
    alarm_table[4].pri=2;
    alarm_table[4].who=ALARM_N9;
    alarm_table[5].code=0x0c241101;//BLOOD1#/
    alarm_table[5].pri=3;
    alarm_table[5].who=ALARM_N9;

    alarm_table[6].code=0x18241101;//液路#/
    alarm_table[6].pri=3;
    alarm_table[6].who=ALARM_N5;

    alarm_table[7].code=0x00310004;//液路#/
    alarm_table[7].pri=1;
    alarm_table[7].who=ALARM_N3;
    alarm_table[8].code=0x00340106;//液路#/
    alarm_table[8].pri=2;
    alarm_table[8].who=ALARM_N3;

    alarm_table[9].code=0x00112202;//程序提示#/
    alarm_table[9].pri=3;
    alarm_table[9].who=ALARM_OWN;

    alarm_table[10].code=0x00111101;//肝素泵#/
    alarm_table[10].pri=3;
    alarm_table[10].who=ALARM_N15;

    alarm_table[11].code=0x0f242202;//血泵1#/
    alarm_table[11].pri=3;
    alarm_table[11].who=ALARM_N9;
    alarm_table[12].code=0x0f242202;//血泵2#/
    alarm_table[12].pri=3;
    alarm_table[12].who=ALARM_N10;
    alarm_table[13].code=0x18241101;//液路#/
    alarm_table[13].pri=3;
    alarm_table[13].who=ALARM_PDO2;
    alarm_table[14].code=0x00111101;//电源#/
    alarm_table[14].pri=3;
    alarm_table[14].who=ALARM_N7;
    alarm_table[15].code=0x00111101;//程序提示#/
    alarm_table[15].pri=3;
    alarm_table[15].who=ALARM_N5;
    alarm_table[16].code=0x00241101;
    alarm_table[16].pri=3;
    alarm_table[16].who=ALARM_SELFCHECK;

    alarm_table[17].code=0x0c242202;//BLOOD2#/
    alarm_table[17].pri=2;
    alarm_table[17].who=ALARM_N10;

    alarm_table[18].code=0x0c241101;//BLOOD2#/
    alarm_table[18].pri=3;
    alarm_table[18].who=ALARM_N10;

    alarm_table[19].code=0x18241101;//液路#/
    alarm_table[19].pri=3;
    alarm_table[19].who=ALARM_N3;

    alarm_table[20].code=0x00241101;//程序提示#/
    alarm_table[20].pri=3;
    alarm_table[20].who=NOTE;

    alarm_table[21].code=0x00241101;//N5扩展报警#/
    alarm_table[21].pri=3;
    alarm_table[21].who=ALARM_N5;

    alarm_table[22].code=0x00241101;//N3扩展报警#/
    alarm_table[22].pri=3;
    alarm_table[22].who=ALARM_N3;

    alarm_table[23].code=0x18241101;//液路 在线预冲、在线引血、在线回血#/
    alarm_table[23].pri=3;
    alarm_table[23].who=ALARM_N3;

    dataNodeAddObject(PACKET_NAME_BLOODPUMP,this); //血泵#/
    dataNodeAddObject(PACKET_NAME_SUBBLOODPUMP,this);//血泵2#/
    dataNodeAddObject(PACKET_STATUS_NOTE,this);  //提示报警 供回液管状态 对应主控板PDO0的4字节#/
    dataNodeAddObject(PACKET_STATUS_MASTALARM,this); //主控板告警状态#/
    dataNodeAddObject(PACKET_STATUS_ALARM,this); //监控板告警状态#/
    dataNodeAddObject(OWNNOTE,this);  //本机的提示报警信息#/
    dataNodeAddObject(PACKET_NAME_HEPARINDATA,this); //肝素泵的信息#/
    dataNodeAddObject(PACKET_NAME_POWER,this); //电源板信息处理#/

    alarmWidgetShow=DE_ALARMWIDGETHIDDEN;

    historyAlarmID=0;     //记录历史告警记录条数，并且作为其索引#/

    //获取报警的行数#/
    int rownum=0;
    rownum=DataSql_GetrownumwarnTable();
    /*
    if(rownum>=0&&rownum<=100)
        historyAlarmMap=DataSql_GetinforwarnTable(0,100,historyAlarmID);
    else
        historyAlarmMap=DataSql_GetinforwarnTable(rownum-100,100,historyAlarmID);
   */
    historyAlarmMap=DataSql_GetinforwarnTable(0,rownum,historyAlarmID);

    connect(&alarmtimer,SIGNAL(timeout()),this,SLOT(timerHandle()));
#if HJX_COMMUNICATION_SWITCH
    connect(this,SIGNAL(signalSDO(int)),dataCanSend,SLOT(sendSDOData(int)),DE_CONNECTION_TYPE);
#endif
#if DE_TEST_SWITCH
    alarmtimer.start(4000);  //测试专用#/
#endif

}

//进行报警处理#/
void AlarmModule::Alarm_Handle()
{
    int key,typecode=0;
    StrAlarm stralarm;
    AlarmInfor hisalarm;

    QMap<int,StrAlarm>::iterator it;

#if SYSTEM_DEBUG_FLAG
    Debug_insert("Alarm_Handle-in0");
#endif
    if(Alarm_Handle_mutexflag==true)
        return;
    Alarm_Handle_mutexflag=true;

    //对新发生的报警，记录时间，并存入curtimeAlarmMap中#/
    if(curAlarmMap.size()>0)
    {
        for(it = curAlarmMap.begin(); it != curAlarmMap.end(); it++)
        {
            key = it.key();   //报警条目编码#/
            stralarm = it.value();
            //已处理的一次性报警，不再处理#/
            if((Check_alarm_type(stralarm)==DE_ONCE)&&(onceAlarmList.contains(key)))
                continue;
            else
            {
                if(!curtimeAlarmMap.contains(key))
                {
                    hisalarm.alarmid = key;
                    hisalarm.code = stralarm.code;
                    hisalarm.pri = stralarm.pri;
                    hisalarm.who = stralarm.who;
                    hisalarm.time =QDateTime().currentDateTimeUtc().toTime_t(); //时间戳#/
                    hisalarm.dispose = 0;
                    curtimeAlarmMap.insert(key,hisalarm);  //保存到了当前保存区#/
                }
            }
        }
    }

    //根据上一次报警队列判断，是否有相应报警消除了#/
    //有消除的则保存到历史记录中#/
    if(preAlarmMap.size()>0)
    {
        for(it = preAlarmMap.begin(); it != preAlarmMap.end(); it++)
        {
            key= it.key();
            stralarm = it.value();

            if(!curAlarmMap.contains(key))  //本次该类报警已消除#/
            {
                if(Check_alarm_type(stralarm)==DE_KEEP) //保持类的报警，不自动消除#/
                {
                    curAlarmMap.insert(key,stralarm);
                    Save_to_histroy(curtimeAlarmMap.value(key));//历史记录保存操作#/
                }
                else if(Check_alarm_type(stralarm)==DE_ONCE)//一次性报警，只处理一次#/
                {
                    if(curtimeAlarmMap.contains(key))
                    {
                        Save_to_histroy(curtimeAlarmMap.value(key));//历史记录保存操作#/
                        if(!onceAlarmList.contains(key))//此一次性报警未处理过#/
                            onceAlarmList.append(key);
                        hisalarm=curtimeAlarmMap.value(key);
                        curtimeAlarmMap.remove(key);
                    }
                } // else if(Check_alarm_type(stralarm)==DE_ONCE)//一次性#/
                else if(Check_alarm_type(stralarm)==DE_RESTORE)
                {
                    Save_to_histroy(curtimeAlarmMap.value(key));//历史记录保存操作#/
                    hisalarm=curtimeAlarmMap.value(key);
                    curtimeAlarmMap.remove(key);
                }
                //从提示框中消失#/
                if(Check_alarm_type(stralarm)!=DE_KEEP)
                {
                    typecode=Check_Promptbox_type(stralarm.code);
                    switch(typecode)
                    {
                        case DE_DELAYDISAPPEAR://跳出提示，不确认，延时消失#/
#if SYSTEM_DEBUG_FLAG
    Debug_insert("Alarm_Handle-remove1");
#endif
                            removeOneWarn(key);
                            break;

                        case DE_LINKAGE:  //提示与报警联动#/
#if SYSTEM_DEBUG_FLAG
    Debug_insert("Alarm_Handle-remove2");
#endif
                            removeOneMessages(key);
                            break;
                    }
                }// if(Check_alarm_type(stralarm)!=DE_KEEP)
            }//if(!curAlarmMap.contains(key))  //本次该类报警已消除#/
            else  //如果没有消除，则判断是否需要升级#/
            {
                if(curAlarmMap.value(key).code != stralarm.code)
                {
                    curtimeAlarmMap.remove(key);
                    hisalarm.alarmid = key;
                    hisalarm.code = curAlarmMap.value(key).code;
                    hisalarm.pri = curAlarmMap.value(key).pri;
                    hisalarm.who = curAlarmMap.value(key).who;
                    hisalarm.time =QDateTime().currentDateTimeUtc().toTime_t(); //时间戳#/
                    hisalarm.dispose = 0;
                    curtimeAlarmMap.insert(key,hisalarm);  //保存到了当前保存区#/
                }
            }
        }//for
    } //    if(preAlarmMap.size()>0)
#if SYSTEM_DEBUG_FLAG
    Debug_insert("Alarm_Handle-in1");
#endif
/*-----------------------报警的相应处理------------------------------*/
    int i=0,code=0;
    char byte=0;

    char lightbyte=0;
    char soundbyte=0;

    char wflag=0,bflag=0,passflag=0,ufflag=0,hepflag=0;

//    Alarmcolorflag = 0;
    QMap<int,AlarmInfor>::iterator it2;
    AlarmInfor tmpalarm;

    QMap<int,AlarmInfor> messagemap;
    QMap<int,AlarmInfor> alarmmap;
    QMap<int,AlarmInfor> warnmap;

    //处理报警消除后，显示不消除的问题#/
    if(curtimeAlarmMap.size()>0)
    {
        for(it2 = curtimeAlarmMap.begin(); it2 != curtimeAlarmMap.end(); it2++)
        {
            key=it2.key();
            hisalarm=it2.value();
            stralarm.code=hisalarm.code;
            stralarm.pri=hisalarm.pri;
            stralarm.who=hisalarm.who;
            if((!curAlarmMap.contains(key))&&(!preAlarmMap.contains(key)))  //该报警已消失#/
            {
                if(Check_alarm_type(stralarm)==DE_ONCE)//一次性报警，只处理一次#/
                {
                    if(curtimeAlarmMap.contains(key))
                    {
                        Save_to_histroy(curtimeAlarmMap.value(key));//历史记录保存操作#/
                        if(!onceAlarmList.contains(key))//此一次性报警未处理过#/
                            onceAlarmList.append(key);
                        hisalarm=curtimeAlarmMap.value(key);
                        curtimeAlarmMap.remove(key);
                    }
                } // else if(Check_alarm_type(stralarm)==DE_ONCE)//一次性#/
                else if(Check_alarm_type(stralarm)==DE_RESTORE)
                {
                    Save_to_histroy(curtimeAlarmMap.value(key));//历史记录保存操作#/
                    hisalarm=curtimeAlarmMap.value(key);
                    curtimeAlarmMap.remove(key);
                }
                //从提示框中消失#/
                if(Check_alarm_type(stralarm)!=DE_KEEP)
                {
                    typecode=Check_Promptbox_type(stralarm.code);
                    switch(typecode)
                    {
                        case DE_DELAYDISAPPEAR://跳出提示，不确认，延时消失#/
                            removeOneWarn(key);
                            break;

                        case DE_LINKAGE:  //提示与报警联动#/
                            removeOneMessages(key);
                            break;
                    }
                }// if(Check_alarm_type(stralarm)!=DE_KEEP)
            }
        }
    } //   if(curtimeAlarmMap.size()>0)


    //当前有告警要处理#/
    struct PacketBloodPump blood1; //血泵1#/
    struct PacketBloodPump blood2; //血泵2#/
    struct PacketStatusNote MasterStatus;//主控板状态#/
    struct PackN15PDO N15PDO;//肝素泵#/
    dataRead(PACKET_NAME_BLOODPUMP,&blood1);
    dataRead(PACKET_NAME_SUBBLOODPUMP,&blood2);
    dataRead(PACKET_STATUS_NOTE,&MasterStatus);
    dataRead(PACKET_NAME_HEPARINDATA,&N15PDO);

    if(curtimeAlarmMap.size()>0)
    {
        i=curtimeAlarmMap.size();
        for(it2 = curtimeAlarmMap.begin(); it2 != curtimeAlarmMap.end(); it2++)
        {
            if(i<=0)
                break;

            wflag=0;//水路报警标志处理#/
            bflag=0;//血路报警标志#/
            ufflag=0;//超滤泵停止标志#/
            hepflag=0;//停肝素泵#/

            key=it2.key();
            tmpalarm=it2.value();
            code=tmpalarm.code;//报警处理编码#/

            byte=(code>>16)&0xff;
            if((byte&0x80)&&(onceAlarmList.contains(key)))//一次性报警，只处理一次#/
                continue;
            else
                onceAlarmList.append(key);

            byte=code&0x0f;
            if((byte>0)&&(byte<soundbyte||soundbyte==0))
            {
                soundbyte=byte;
            }

            byte=(code>>8)&0xff;
            byte=byte&0x0f;
            if((byte>0)&&(byte<lightbyte||lightbyte==0))
            {
                lightbyte=byte;

                byte=(code>>8)&0xff;
                byte=byte&0xf0;
                //通知右上角图标按钮颜色#/
                switch(byte) {
                case 0x10: //红#/
                    Alarmcolorflag=3;
                    break;
                case 0x20://黄#/
                    Alarmcolorflag=2;
                    break;
                case 0x30://绿#/
                    Alarmcolorflag=1;
                    break;
                }
            }
            //提示框处理#/
            //此前已经处理过了的，不再重复处理#/
            if(!preAlarmMap.contains(key)||(preAlarmMap.contains(key)&&(preAlarmMap.value(key).code!=code)))
            {
#if SYSTEM_DEBUG_FLAG
    Debug_insert("Alarm_Handle-in2");
#endif
                typecode=Check_Promptbox_type(code);
                switch(typecode)
                {
                case DE_NONE:  //无提示框#/
                    break;

                case DE_MUSTCONFIRM:  //跳出提示，需确认#/
                    alarmmap.insert(key,tmpalarm);
                    break;

                case DE_DELAYDISAPPEAR:  //跳出提示，不确认，延时消失#/
                    warnmap.insert(key,tmpalarm);
                    break;

                case DE_NOPOP:  //不跳出提示#/
                    break;

                case DE_LINKAGE:  //提示与报警联动#/
                    messagemap.insert(key,tmpalarm);
                    break;
                }
                byte=(code>>24)&0xff;
                action(byte);   //动作处理#/
            } // if(!preAlarmMap.contains(key))
            else
            {

                byte=(code>>24)&0xff;
                if(byte&0x08)   //仍然有停置换泵的动作#/
                    wflag=1;
                if(byte&0x01) //停血泵#/
                    bflag=1;
                if(byte&0x20) //停超滤泵#/
                    ufflag=1;
                if(byte&0x04) //停肝素泵#/
                    hepflag=1;

                //仍有停相应泵的动作时，此时开启泵，则弹出报警#/
          //      if(((bflag==1)&&(blood1.N9_mode&0x01))||((ufflag==1)&&(MasterStatus.note.note2&0x80))||((hepflag==1)&&(N15PDO.workMode&0x01)))
                if(((bflag==1)&&(alarm_N9.N9_mode==1))||((ufflag==1)&&(note_N3.note.note2&0x80))||((hepflag==1)&&(alarm_N15.workMode==1)))
                {
                    typecode=Check_Promptbox_type(code);
                    switch(typecode)
                    {
                    case DE_NONE:  //无提示框#/
                        break;

                    case DE_MUSTCONFIRM:  //跳出提示，需确认#/
                        alarmmap.insert(key,tmpalarm);
                        break;

                    case DE_DELAYDISAPPEAR:  //跳出提示，不确认，延时消失#/
                        warnmap.insert(key,tmpalarm);
                        break;

                    case DE_NOPOP:  //不跳出提示#/
                        break;

                    case DE_LINKAGE:  //提示与报警联动#/
                        messagemap.insert(key,tmpalarm);
                        break;
                    }
                }
            }   //else
            i--;
        }  // for(it2 = curtimeAlarmMap.begin();

#if SYSTEM_DEBUG_FLAG
    Debug_insert("Alarm_Handle-in4");
#endif
        passflag=0;//旁路报警标志#/
        wflag=0;//水路报警标志处理#/
        bflag=0;//血路报警标志#/
        ufflag=0;//超滤泵停止标志#/
        hepflag=0;//停肝素泵#/

        for(it2 = curtimeAlarmMap.begin(); it2 != curtimeAlarmMap.end(); it2++)
        {
            tmpalarm=it2.value();
            code=tmpalarm.code;//报警处理编码#/
            byte=(code>>24)&0xff;
            if(byte&0x10)   //仍然有旁路的动作#/
                passflag=1;
            if(byte&0x08)   //仍然有停置换泵的动作#/
                wflag=1;
            if(byte&0x01) //停血泵#/
                bflag=1;
            if(byte&0x20) //停超滤泵#/
                ufflag=1;
            if(byte&0x04) //停肝素泵#/
                hepflag=1;
        }

        //旁路动作取消#/
        if(Alarm_Pass_Flag==true&&passflag==0)
        {
            if(G_BYPASS_BUT_FLAG!=DE_BYPASS_CHECKED)
            {
#if HJX_COMMUNICATION_SWITCH
            emit signalSDO(DE_CMD_Nobypass);
#else
            us_sleep(10000);
            MasterCtrol_CMD_Nobypass();//停旁路#/
            us_sleep(10000);
#endif
            }
            Alarm_Pass_Flag=false;
        }

        //置换泵处理
        if(Waterway_alarm_flag==true&&wflag==0)
        {
            Waterway_alarm_flag=false;
            Alarm_SubsPump_Flag=false;
        }
        else if((wflag==1)&&(blood2.N9_mode&0x01))
        {
#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_SubsStop);
#else
         MasterCtrol_CMD_SubsStop();    //停止置换液泵#/
         us_sleep(10000);
#endif
            Waterway_alarm_flag=true;
            Alarm_SubsPump_Flag=true;
        }

        //血泵处理
        if(Alarm_BloodPump_Flag==true&&bflag==0)
        {
            Alarm_BloodPump_Flag=false;
        }
        else if((bflag==1)&&(blood1.N9_mode&0x01))
        {
    #if HJX_COMMUNICATION_SWITCH
            emit signalSDO(DE_CMD_BloodStop);
    #else
            MasterCtrol_CMD_BloodStop();   //停血泵#/
            us_sleep(10000);
    #endif

    #if HJX_COMMUNICATION_SWITCH
            emit signalSDO(DE_CMD_ClipStop);
            emit signalSDO(DE_CMD_Clip2Stop);
    #else
            MasterCtrol_CMD_ClipStop();    //阻流夹#/
            us_sleep(10000);
            MasterCtrol_CMD_Clip2Stop();   //阻流夹#/
            us_sleep(10000);
    #endif
            Alarm_BloodPump_Flag=true;
        }
        /*
        else if(bflag==1&&Alarm_BloodPump_Flag==false)
        {
#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_BloodStop);
#else
        MasterCtrol_CMD_BloodStop();   //停血泵#/
        us_sleep(10000);
#endif
        Alarm_BloodPump_Flag=true;

#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_ClipStop);
        emit signalSDO(DE_CMD_Clip2Stop);
#else
        MasterCtrol_CMD_ClipStop();    //阻流夹#/
        us_sleep(10000);
        MasterCtrol_CMD_Clip2Stop();   //阻流夹#/
        us_sleep(10000);
#endif
        }
        */

        //超滤泵处理
        if(Alarm_UFPump_Flag==true&&ufflag==0)
        {
            Alarm_UFPump_Flag=false;
        }
        else if((ufflag==1)&&(MasterStatus.note.note2&0x80))
        {
    #if HJX_COMMUNICATION_SWITCH
            emit signalSDO(DE_CMD_UFStop);
    #else
            MasterCtrol_CMD_UFStop();     //停止超虑#/
            us_sleep(10000);
    #endif
            Alarm_UFPump_Flag=true;
        }
        /*
        else if(ufflag==1&&Alarm_UFPump_Flag==false)
        {
#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_UFStop);
#else
        MasterCtrol_CMD_UFStop();     //停止超虑#/
        us_sleep(10000);
#endif
            Alarm_UFPump_Flag=true;
        }
        */

        //肝素泵处理#/
        if(Alarm_HeparinPump_Flag==true&&hepflag==0)
        {
            Alarm_HeparinPump_Flag=false;
        }
        else if((hepflag==1)&&(N15PDO.workMode&0x01))
        {
    #if HJX_COMMUNICATION_SWITCH
            emit signalSDO(DE_CMD_HeparinPumpStop);
    #else
            MasterCtrol_CMD_HeparinPumpStop();            //停肝素泵#/
            us_sleep(10000);
    #endif
            Alarm_HeparinPump_Flag=true;
        }

        /*
        else if(hepflag==1&&Alarm_HeparinPump_Flag==false)
        {
#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_HeparinPumpStop);
#else
        MasterCtrol_CMD_HeparinPumpStop();            //停肝素泵#/
        us_sleep(10000);
#endif
        Alarm_HeparinPump_Flag=true;
        }
        */

        //报警提示窗口处理#/
        if((messagemap.size()>0)&&(alarmWidgetShow==DE_ALARMWIDGETHIDDEN))
        {
            updateMessageDialog(messagemap);
        }
        //警告 跳出提示，不确认，延时消失#/
        if((warnmap.size()>0)&&(alarmWidgetShow==DE_ALARMWIDGETHIDDEN))
        {
            updateWarnDialog(warnmap);
        }
        //报警 提示与报警联动#/
        if((alarmmap.size()>0)&&(alarmWidgetShow==DE_ALARMWIDGETHIDDEN))
        {
            updateAlarmDialog(alarmmap);
        }

        //灯光处理#/
        if(lightbyte>0)
        {
            if(Alarm_light_shielding==false)
                light(lightbyte);    //灯光处理#/
        }
        else
        {
            if(Alarm_light_shielding==false)
            {
                light(0);  //平时绿灯
            }
        }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("Alarm_Handle-in5");
#endif
        if(soundbyte>0)
        {
            if(stateflag != DE_MUTE_FLAG)
            {
                if(Alarm_sound_shielding==false)
                    sound(soundbyte);
            }
        }
        else
        {
          if(Alarm_sound_shielding==false)
            sound(0);
        }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("Alarm_Handle-in6");
#endif
        if(Alarmcolorflag>0)
        {
            dataNodeUpdate(PACKET_NAME_BTNALARM, &Alarmcolorflag);
        }

        //在显示当前报警页面时#/
        if(alarmWidgetShow==DE_CURRENTALARMSHOW)
        {
            dataNodeUpdate(PACKET_NAME_MESSAGES, &alarmWidgetShow);
        }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("Alarm_Handle-in7");
#endif
    }    //if(curtimeAlarmMap.size()>0)    //当前有告警要处理#/
    else
    {
        if(Alarmcolorflag!=0)
        {
            dataNodeUpdate(PACKET_NAME_MESSAGES, &alarmWidgetShow);
            Alarmcolorflag=0;
            Alarm_BloodPump_Flag=false;  //发生停血泵报警标志#/
            Alarm_UFPump_Flag=false;//发生停超滤泵报警标志#/
            Alarm_SubsPump_Flag=false;//发生停置换泵报警标志#/
            Alarm_HeparinPump_Flag=false;//发生停肝素泵报警标志#/
        }

        if(Alarm_Pass_Flag==true)
        {
            if(G_BYPASS_BUT_FLAG!= DE_BYPASS_CHECKED)
            {
#if HJX_COMMUNICATION_SWITCH
                emit signalSDO(DE_CMD_Nobypass);
#else
            us_sleep(10000);
            MasterCtrol_CMD_Nobypass();//停旁路#/
            us_sleep(10000);
#endif
            }
            Alarm_Pass_Flag=false;
        }

        if(Alarm_light_shielding==false)
            light(0);
        if(Alarm_sound_shielding==false)
            sound(0);
        dataNodeUpdate(PACKET_NAME_BTNALARM, &Alarmcolorflag);
    }

    messagemap.clear();
    warnmap.clear();
    alarmmap.clear();
    preAlarmMap.clear();    //重新赋值上一次报警列表#/
    preAlarmMap=curAlarmMap;
//    delete [] alarmArray;
#if SYSTEM_DEBUG_FLAG
    Debug_insert("Alarm_Handle-out");
#endif
    Alarm_Handle_mutexflag=false;
}

//根据各节点设备状态计算生成报警编码#/
void AlarmModule::Get_AlarmCode()
{

    StrAlarm tmpAlarm;
    PacketWorkMode workModeData;    //获取供液模式和工作模式#/

 //   int tipState=PDOstate.state.N1_status;  //总状态#/
    int subState=PDOstate.state.N1_sub_status;//子状态#/
//    int waterState=PDOstate.state.N1_water_status;  //水路状态#/
    int bloodState=PDOstate.state.N1_blood_status;  //血路状态#/

    int n1status=Check_N1_status();  //获取当前主状态，报警判断条件#/
  //  int n1substatus=Check_N1_sub_status(); //获取子状态，报警判断条件#/
    int n1bloodstatus=Check_N1_blood_status();
    int n1waterstatus=Check_N1_water_status();

    curAlarmMap.clear();

    dataRead(PACKET_NAME_WORKMODE,&workModeData);

    if(n1status==DE_NORMAL)  //正常工作状态#/
    {
         //预冲有液前#/
        if((subState>0x12)&&(subState==0x14||subState==0x16||subState==0x18||subState==0x1a||subState==0x1d||subState==0x1f))
        {
            //动脉 、静脉 、TMP 压#/
            if(alarm_N5.alarm.errorstate2 & 0x01)
                curAlarmMap.insert(ALARM_VPUP,alarm_table[2]);
            if(alarm_N5.alarm.errorstate2 & 0x02)
                curAlarmMap.insert(ALARM_VPDOWN,alarm_table[2]);
            if(alarm_N5.alarm.errorstate2 & 0x04)
                curAlarmMap.insert(ALARM_APUP,alarm_table[2]);
            if(alarm_N5.alarm.errorstate2 & 0x08)
                curAlarmMap.insert(ALARM_APDOWN,alarm_table[2]);
            if(alarm_N5.alarm.errorstate2 & 0x10)
                curAlarmMap.insert(ALARM_TMPUP,alarm_table[2]);
            if(alarm_N5.alarm.errorstate2 & 0x20)
                curAlarmMap.insert(ALARM_TMPDOWN,alarm_table[2]);
        }
        else if(subState>0x12)
        {
            //动脉 、静脉 、TMP 压#/
            if(alarm_N5.alarm.errorstate2 & 0x01)
                curAlarmMap.insert(ALARM_VPUP,alarm_table[3]);
            if(alarm_N5.alarm.errorstate2 & 0x02)
                curAlarmMap.insert(ALARM_VPDOWN,alarm_table[3]);
            if(alarm_N5.alarm.errorstate2 & 0x04)
                curAlarmMap.insert(ALARM_APUP,alarm_table[3]);
            if(alarm_N5.alarm.errorstate2 & 0x08)
                curAlarmMap.insert(ALARM_APDOWN,alarm_table[3]);
            if(alarm_N5.alarm.errorstate2 & 0x10)
                curAlarmMap.insert(ALARM_TMPUP,alarm_table[3]);
            if(alarm_N5.alarm.errorstate2 & 0x20)
                curAlarmMap.insert(ALARM_TMPDOWN,alarm_table[3]);
        }

        //预冲有血前#/
        if((subState>=0x13&&subState<=0x1a)||(subState>=0x1d&&subState<=0x20))
        {
            //液位报警#/
            if(alarm_N5.alarm.errorstate1 & 0x04)
                curAlarmMap.insert(ALARM_FLUID,alarm_table[2]);
            //空气报警#/
            if(alarm_N5.alarm.errorstate1 & 0x01)  //无血#/
                curAlarmMap.insert(ALARM_NO_BLOOD ,alarm_table[2]);
            if(alarm_N5.alarm.errorstate1 & 0x80)   //静脉气泡#/
                curAlarmMap.insert(ALARM_VEIN_AIR ,alarm_table[2]);
            if(alarm_N5.alarm.errorstate1 & 0x02) //动脉气泡#/
                curAlarmMap.insert(ALARM_ARTERIES_AIR ,alarm_table[2]);
        }
        else if(subState>=CURE_BLOODRETURN_RUN && subState<=CURE_BLOODRETURN_STOP_DRAIN_NO_BLOOD) //回血时#/
        {
            tmpAlarm=alarm_table[2];
            tmpAlarm.code=0x00212200;//血路
            //液位报警#/
            if(alarm_N5.alarm.errorstate1 & 0x04)
                curAlarmMap.insert(ALARM_FLUID,alarm_table[2]);
            //空气报警#/
            if(alarm_N5.alarm.errorstate1 & 0x01)  //无血#/
                curAlarmMap.insert(ALARM_NO_BLOOD ,tmpAlarm);
            if(alarm_N5.alarm.errorstate1 & 0x80)   //静脉气泡#/
                curAlarmMap.insert(ALARM_VEIN_AIR ,alarm_table[3]);
            if(alarm_N5.alarm.errorstate1 & 0x02) //动脉气泡#/
                curAlarmMap.insert(ALARM_ARTERIES_AIR ,alarm_table[2]);
        }
        else if(subState>0x12)  //有血时
        {            
            //液位报警#/
            if(alarm_N5.alarm.errorstate1 & 0x04)
                curAlarmMap.insert(ALARM_FLUID,alarm_table[3]);
            //空气报警#/
            if(alarm_N5.alarm.errorstate1 & 0x01)  //无血#/
                curAlarmMap.insert(ALARM_NO_BLOOD ,alarm_table[3]);
            if(alarm_N5.alarm.errorstate1 & 0x80)   //静脉气泡#/
                curAlarmMap.insert(ALARM_VEIN_AIR ,alarm_table[3]);
            if(alarm_N5.alarm.errorstate1 & 0x02) //动脉气泡#/
                curAlarmMap.insert(ALARM_ARTERIES_AIR ,alarm_table[3]);
        }

        if(n1bloodstatus==DE_ONLINE)
        {
            //预冲有血前和回血无血hou#/
          //  if((subState>0x12)&&(subState<0x1b||subState==0x53||subState==0x55||subState==0x59||subState==0x5b))
            if(subState<0x1b||subState==0x53||subState==0x55||subState==0x59||subState==0x5b)
            {
                //血泵2 盖打开#/
                if(alarm_N10.N9_status & 0x01)
                    curAlarmMap.insert(ALARM_N10CAPOPEN ,alarm_table[17]);
            }
            else if(subState>0x12)
            {
                //血泵2 盖打开#/
                if(alarm_N10.N9_status & 0x01)
                    curAlarmMap.insert(ALARM_N10CAPOPEN ,alarm_table[18]);
            }

            tmpAlarm=alarm_table[4];
            tmpAlarm.code=0x2c241101;

            //血泵1 盖打开#/
            if(alarm_N9.N9_status & 0x01)
                curAlarmMap.insert(ALARM_N9CAPOPEN ,tmpAlarm);

        }  //    if(n1bloodstatus==DE_ONLINE)#/
        else
        {
            /*
            if((subState>=0x1b)&&(subState!=0x53&&subState!=0x55&&subState!=0x59&&subState!=0x5b))
            {
                //血泵1 盖打开#/
                if(alarm_N9.N9_status & 0x01)
                    curAlarmMap.insert(ALARM_N9CAPOPEN ,alarm_table[5]);
            }
            */

            tmpAlarm=alarm_table[4];
            tmpAlarm.code=0x2c241101;

            //血泵1 盖打开#/
            if(alarm_N9.N9_status & 0x01)
                curAlarmMap.insert(ALARM_N9CAPOPEN ,tmpAlarm);
        }

        //漏血报警#/
        if(alarm_N5.alarm.errorstate3 & 0x20)
            curAlarmMap.insert(ALARM_BLOODLEAK,alarm_table[3]);

        //肝素泵报警#/
        if(alarm_N15.senserState & 0x02)
            curAlarmMap.insert(ALARM_HEPARINBLOCK,alarm_table[10]);

        //水路工作异常#/
        if(alarm_own.note.note7 & 0x01)
            curAlarmMap.insert(ALARM_WATERWORKFAIL,alarm_table[20]);


        if(subState==PREFLUSH_RUN_FLUID||subState==PREFLUSH_RUN_NO_FLUID||\
           subState==GETBLOOD_RUN_FLUID||subState==GETBLOOD_RUN_NO_FLUID||\
           subState==CURE_BLOODRETURN_RUN||subState==CURE_BLOODRETURN_RUN_NO_BLOOD)
        {
            tmpAlarm.code=0x00111101;
            tmpAlarm.pri=2;
            tmpAlarm.who=ALARM_OWN;

            //主控板手动预冲状态错误#/
            if(alarm_own.note.note7 & 0x02)
                curAlarmMap.insert(PROMPT_MANUAL_PREFLUSH,tmpAlarm);
            else if(alarm_own.note.note7 & 0x04)//bit2：自动预冲#/
                curAlarmMap.insert(PROMPT_AUTO_PREFLUSH,tmpAlarm);
            else if(alarm_own.note.note7 & 0x08) //bit3：在线预冲#/
                curAlarmMap.insert(PROMPT_ONLINE_PREFLUSH,tmpAlarm);
            else if(alarm_own.note.note7 & 0x10) //bit4：引血#/
                curAlarmMap.insert(PROMPT_GETBLOOD,tmpAlarm);
            else if(alarm_own.note.note7 & 0x20) //bit5：在线引血#/
                curAlarmMap.insert(PROMPT_ONLINE_GETBLOOD,tmpAlarm);
            else if(alarm_own.note.note7 & 0x40) //bit6：回血#/
                curAlarmMap.insert(PROMPT_RETURNBLOOD,tmpAlarm);
            else if(alarm_own.note.note7 & 0x80)//bit7：在线回血#/
                curAlarmMap.insert(PROMPT_ONLINE_RETURNBLOOD,tmpAlarm);
        }

        if(n1waterstatus==DE_WATEROPEN&&(subState>=0x13))  //水路启动,并且水路稳定后#/
        {
            //只有混合电导错误报警时，将电导上下限报警作为混合电导上下限报警#/
            //有N5的时候，只报N5的报警内容，否则报N3的#/
            /*
            if((alarm_N3.alarm.errorstate1 & 0x40)&&(alarm_N5.alarm.errorstate3 & 0x03))
            {
                if(alarm_N5.alarm.errorstate3 & 0x01)
                    curAlarmMap.insert(ALARM_MIXSUMCONDUP,alarm_table[6]);
                if(alarm_N5.alarm.errorstate3 & 0x02)
                    curAlarmMap.insert(ALARM_MIXSUMCONDDOWN,alarm_table[6]);
            }
            else if(alarm_N5.alarm.errorstate3 & 0x03)//其余情况，作为出水口电导上下限报警#/
            {
                if(alarm_N5.alarm.errorstate3 & 0x01)
                    curAlarmMap.insert(ALARM_OUTSUMCONDUP,alarm_table[6]);
                if(alarm_N5.alarm.errorstate3 & 0x02)
                    curAlarmMap.insert(ALARM_OUTSUMCONDDOWN,alarm_table[6]);
            }
            else
            {   //出水口总电导度范围报警 N3#/
                if(alarm_N3.alarm.errorstate1 & 0x40)
                    curAlarmMap.insert(ALARM_OUTSUMCOND,alarm_table[19]);
                //混合总电导度范围报警 N3#/
                if(alarm_N3.alarm.errorstate1 & 0x20)
                    curAlarmMap.insert(ALARM_MIXSUMCOND,alarm_table[19]);
            }
            */

            if(bloodState!=0x12&&bloodState!=0x19&&bloodState!=0x1a)  //单超的时候，不报透析夜相关报警#/
            {
            //    PacketStatusNote L_data;
            //    dataRead(PACKET_STATUS_NOTE,&L_data);

                //在线自动预冲 在线引血 在线回血 停血泵#/
            //    if(L_data.status.blood_status==0x16||L_data.status.blood_status==0x18||L_data.status.blood_status==0x1a)
                //在线自动预冲  停血泵#/
                if((subState==PREFLUSH_RUN_FLUID||subState==PREFLUSH_RUN_NO_FLUID)&&(Startpreflush_mode==ONLINE_PREFLUSH_MODE))
                {
                    tmpAlarm.code=0x19241101;//液路
                    tmpAlarm.pri=3;
                    tmpAlarm.who=ALARM_N3;
                    if(alarm_N5.alarm.errorstate3 & 0x03)//其余情况，作为出水口电导上下限报警#/
                    {
                        tmpAlarm.who=ALARM_N5;
                        if(alarm_N5.alarm.errorstate3 & 0x01)
                            curAlarmMap.insert(ALARM_OUTSUMCONDUP,tmpAlarm);
                        if(alarm_N5.alarm.errorstate3 & 0x02)
                            curAlarmMap.insert(ALARM_OUTSUMCONDDOWN,tmpAlarm);
                    }
                    else
                    {
                         tmpAlarm.who=ALARM_N3;
                        //出水口总电导度范围报警 N3#/
                        if(alarm_N3.alarm.errorstate1 & 0x40)
                            curAlarmMap.insert(ALARM_OUTSUMCOND,tmpAlarm);
                        /*
                        //混合总电导度范围报警 N3#/
                        if(alarm_N3.alarm.errorstate1 & 0x20)
                            curAlarmMap.insert(ALARM_MIXSUMCOND,tmpAlarm);
                            */
                    }

                    //有N5的时候，只报N5的报警内容，否则报N3的#/
                    if((alarm_N5.alarm.errorstate2 & 0xC0)||(alarm_N5.alarm.errorstate3 & 0xC0))
                    {
                        tmpAlarm.who=ALARM_N5;
                        //温度高于极限 41C#/
                        if(alarm_N5.alarm.errorstate3 & 0x40)
                            curAlarmMap.insert(ALARM_FTMPHIGHLIMIT,tmpAlarm);
                        //温度低于极限 30#/
                        if(alarm_N5.alarm.errorstate3 & 0x80)
                            curAlarmMap.insert(ALARM_FTMPLOWLIMIT,tmpAlarm);
                        //温度上限#/
                        if(alarm_N5.alarm.errorstate2 & 0x40)
                            curAlarmMap.insert(ALARM_FTMPUP,tmpAlarm);
                        //温度下限#/
                        if(alarm_N5.alarm.errorstate2 & 0x80)
                            curAlarmMap.insert(ALARM_FTMPDOWN,tmpAlarm);
                    }
                    else if(alarm_N3.alarm.errorstate1 & 0x80)
                    {
                        tmpAlarm.who=ALARM_N3;
                        //温度范围报警 N3#/
                        curAlarmMap.insert(ALARM_FTMPRANGE,tmpAlarm);
                    }

                    tmpAlarm.who=ALARM_N3;
                    //A液电导率错误#/
                    if(alarm_N3.alarm.errorstate1 & 0x02)
                        curAlarmMap.insert(ALARM_ACONDERROR,tmpAlarm);
                    //B液电导率错误#/
                    if(alarm_N3.alarm.errorstate1 & 0x04)
                        curAlarmMap.insert(ALARM_BCONDERROR,tmpAlarm);

                    //A无液#/
                    if(alarm_N3.alarm.errorstate2 & 0x08)
                        curAlarmMap.insert(ALARM_ANOFLUID,tmpAlarm);
                    //B无液#/
                    if(alarm_N3.alarm.errorstate2 & 0x10)
                        curAlarmMap.insert(ALARM_BNOFLUID,tmpAlarm);
                }
                else
                {
                    if(alarm_N5.alarm.errorstate3 & 0x03)//其余情况，作为出水口电导上下限报警#/
                    {
                        if(alarm_N5.alarm.errorstate3 & 0x01)
                            curAlarmMap.insert(ALARM_OUTSUMCONDUP,alarm_table[6]);
                        if(alarm_N5.alarm.errorstate3 & 0x02)
                            curAlarmMap.insert(ALARM_OUTSUMCONDDOWN,alarm_table[6]);
                    }
                    else
                    {   //出水口总电导度范围报警 N3#/
                        if(alarm_N3.alarm.errorstate1 & 0x40)
                            curAlarmMap.insert(ALARM_OUTSUMCOND,alarm_table[19]);
                        /*
                        //混合总电导度范围报警 N3#/
                        if(alarm_N3.alarm.errorstate1 & 0x20)
                            curAlarmMap.insert(ALARM_MIXSUMCOND,alarm_table[19]);
                            */
                    }

                    //有N5的时候，只报N5的报警内容，否则报N3的#/
                    if((alarm_N5.alarm.errorstate2 & 0xC0)||(alarm_N5.alarm.errorstate3 & 0xC0))
                    {
                        //温度高于极限 41C#/
                        if(alarm_N5.alarm.errorstate3 & 0x40)
                            curAlarmMap.insert(ALARM_FTMPHIGHLIMIT,alarm_table[6]);
                        //温度低于极限 30#/
                        if(alarm_N5.alarm.errorstate3 & 0x80)
                            curAlarmMap.insert(ALARM_FTMPLOWLIMIT,alarm_table[6]);
                        //温度上限#/
                        if(alarm_N5.alarm.errorstate2 & 0x40)
                            curAlarmMap.insert(ALARM_FTMPUP,alarm_table[6]);
                        //温度下限#/
                        if(alarm_N5.alarm.errorstate2 & 0x80)
                            curAlarmMap.insert(ALARM_FTMPDOWN,alarm_table[6]);
                    }
                    else if(alarm_N3.alarm.errorstate1 & 0x80)
                    {
                        //温度范围报警 N3#/
                        curAlarmMap.insert(ALARM_FTMPRANGE,alarm_table[19]);
                    }
                    //A液电导率错误#/
                    if(alarm_N3.alarm.errorstate1 & 0x02)
                        curAlarmMap.insert(ALARM_ACONDERROR,alarm_table[19]);
                    //B液电导率错误#/
                    if(alarm_N3.alarm.errorstate1 & 0x04)
                        curAlarmMap.insert(ALARM_BCONDERROR,alarm_table[19]);

                    //A无液#/
                    if(alarm_N3.alarm.errorstate2 & 0x08)
                        curAlarmMap.insert(ALARM_ANOFLUID,alarm_table[19]);
                    //B无液#/
                    if(alarm_N3.alarm.errorstate2 & 0x10)
                        curAlarmMap.insert(ALARM_BNOFLUID,alarm_table[19]);
                }

            }  //if(bloodState!=0x12&&bloodState!=0x19&&bloodState!=0x1a)  //单超的时候，不报透析夜相关报警#/

        } //    if(n1waterstatus==DE_WATEROPEN) //水路启动时#/

        if(alarm_N9.N9_status & 0x20)
            curAlarmMap.insert(ALARM_N9BLOODOUT ,alarm_table[5]);

        if(alarm_N10.N9_status & 0x20)
            curAlarmMap.insert(ALARM_N10BLOODOUT ,alarm_table[18]);

        tmpAlarm.code=0x00241101;
        tmpAlarm.pri=2;
        tmpAlarm.who=ALARM_OWN;

        //自定义提示性报警#/
        if(Prompt_own.note.note1&0x01)//收缩压上限报警#/
             curAlarmMap.insert(PROMPT_SYSTOLIC_UP,tmpAlarm);
        else if(Prompt_own.note.note1&0x02)//收缩压下限报警#/
             curAlarmMap.insert(PROMPT_SYSTOLIC_DOWN,tmpAlarm);

        if(Prompt_own.note.note1&0x04)//舒张压UP报警#/
             curAlarmMap.insert(PROMPT_DIASTOLIC_UP,tmpAlarm);
        else if(Prompt_own.note.note1&0x08)//舒张压DOWN报警#/
             curAlarmMap.insert(PROMPT_DIASTOLIC_DOWN,tmpAlarm);

        if(Prompt_own.note.note1&0x10) //脉搏UP报警#/
             curAlarmMap.insert(PROMPT_PULSE_UP,tmpAlarm);
        else if(Prompt_own.note.note1&0x20)//脉搏DOWN报警#/
             curAlarmMap.insert(PROMPT_PULSE_DOWN,tmpAlarm);

        tmpAlarm.code=0x00242303;
        tmpAlarm.pri=2;
        tmpAlarm.who=ALARM_OWN;

        if(Prompt_own.note.note1&0x40) //肝素泵时间到停止#/
             curAlarmMap.insert(PROMPT_HEPARIN_TIMESTOP,tmpAlarm);

        if(Prompt_own.note.note1&0x80)//肝素泵没有打开#/
             curAlarmMap.insert(PROMPT_HEPARIN_NOOPEN,tmpAlarm);

        if(Prompt_own.note.note2 &0x01)//报警处理完后，提示打开血泵#/
             curAlarmMap.insert(PROMPT_PLEASEOPENBLOOD,tmpAlarm);

        if(Prompt_own.note.note2 &0x02)  //XML文件读取异常#/
             curAlarmMap.insert(PROMPT_XMLFILEERROR,tmpAlarm);

    }  //if(n1status==DE_NORMAL)#/

    //排空时不进行无水报警#/
    if(PDOstate.state.N1_status != 0x0A && PDOstate.state.N1_water_status != 0x7a)
    {
        if(PDOstate.state.N1_blood_status==0x12||PDOstate.state.N1_blood_status==0x19||
           PDOstate.state.N1_blood_status==0x1a||PDOstate.state.N1_blood_status==0x16)
        {   //无水报警#/
            if(alarm_N3.alarm.errorstate1 & 0x01)
                curAlarmMap.insert(ALARM_NOWATER,alarm_table[7]);
        }
        else
        {
            if(alarm_N3.alarm.errorstate1 & 0x01)
                curAlarmMap.insert(ALARM_NOWATER,alarm_table[6]);
        }
    }

    //消毒液电导上下限报警，非消毒循环 和 热冲洗 纯热水消毒时不用报#/
    if(PDOstate.state.N1_blood_status != HOT  && PDOstate.state.N1_status==CLEANING &&PDOstate.state.N1_blood_status != COOL_RINSE&&PDOstate.state.N1_blood_status != HOT_RINSE)
    {
        if(alarm_N3.alarm.errorstate1 & 0x08)
            curAlarmMap.insert(ALARM_DISCONDUP,alarm_table[8]);//消毒液电导上限错误#/
        if(alarm_N3.alarm.errorstate1 & 0x10)
            curAlarmMap.insert( ALARM_DISCONDDOWN,alarm_table[8]);//消毒液电导下限错误#/
    }

    /*
    if(alarm_N5.alarm.errorstate7==8)
        curAlarmMap.insert(ALARM_BLOODMETER ,alarm_table[15]);
        */

    //将A液吸管放到A液桶#/
    if(note_N3.note.note1 & 0x01)
        curAlarmMap.insert(PROMPT_ATOBARREL,alarm_table[1]);
    //将A液吸管插回机器#/
    if(note_N3.note.note1 & 0x02)
        curAlarmMap.insert(PROMPT_ATODEVICE,alarm_table[1]);
    //将B液管放到B液桶#/
    if(note_N3.note.note1 & 0x04)
        curAlarmMap.insert(PROMPT_BTOBARREL,alarm_table[1]);
    //将B液吸管插回机器#/
    if(note_N3.note.note1 & 0x08)
        curAlarmMap.insert(PROMPT_BTODEVICE,alarm_table[1]);
    //将供液管接到透析器#/
    if(note_N3.note.note1 & 0x10)
        curAlarmMap.insert(PROMPT_FEEDTODIA,alarm_table[1]);
    //将供液管接回机器#/
    if(note_N3.note.note1 & 0x20)
        curAlarmMap.insert(PROMPT_FEEDTODEVICE,alarm_table[1]);
    //将回液管接到透析器#/
    if(note_N3.note.note1 & 0x40)
        curAlarmMap.insert(PROMPT_BACKTODIA,alarm_table[1]);
    //将回液管接回机器#/
    if(note_N3.note.note1 & 0x80)
        curAlarmMap.insert(PROMPT_BACKTODEVICE,alarm_table[1]);

    if(workModeData.fluidmode.current==A_and_BPower)
    {
        //将干粉架打开#/
        if(note_N3.note.note2 & 0x01)
            curAlarmMap.insert(PROMPT_DRYFRAMEOPEN,alarm_table[1]);
        // 将干粉架关闭#/
        if(note_N3.note.note2 & 0x02)
            curAlarmMap.insert(PROMPT_DRYFRAMECLOSE,alarm_table[1]);
    }

    //将相应管路接到置换液接口#/
    if(note_N3.note.note2 & 0x04)
        curAlarmMap.insert(PROMPT_SETUPCORRESPONDING,alarm_table[1]);

    //将置换液接口管路移除#/
    if(note_N3.note.note2 & 0x08)
        curAlarmMap.insert(PROMPT_REMOVETHEINTERFACE,alarm_table[1]);

    //将相应管路接到置换液冲洗接口#/
    if(note_N3.note.note2 & 0x10)
        curAlarmMap.insert(PROMPT_SETUPFLUSHINTERFACE,alarm_table[1]);

    //将置换液冲洗接口管路移除#/
    if(note_N3.note.note2 & 0x20)
        curAlarmMap.insert(PROMPT_REMOVEFLUSHINTERFACE,alarm_table[1]);

    //N3 canopen通信失败#/
    if(alarm_own.note.note6 & 0x01)
        curAlarmMap.insert(ALARM_N3CANOPENFAIL,alarm_table[20]);
    //N5 canopen通信失败#/
    if(alarm_own.note.note6 & 0x02)
        curAlarmMap.insert(ALARM_N5CANOPENFAIL,alarm_table[20]);
    //N9 canopen通信失败#/
    if(alarm_own.note.note6 & 0x04)
        curAlarmMap.insert(ALARM_N9CANOPENFAIL,alarm_table[20]);
    //N10 canopen通信失败#/
    if(alarm_own.note.note6 & 0x08)
        curAlarmMap.insert(ALARM_N10CANOPENFAIL,alarm_table[20]);
     //N15 canopen通信失败#/
    if(alarm_own.note.note6 & 0x10)
        curAlarmMap.insert(ALARM_N15CANOPENFAIL,alarm_table[20]);
    //N7 canopen通信失败#/
    if(alarm_own.note.note6 & 0x20)
        curAlarmMap.insert(ALARM_N7CANOPENFAIL,alarm_table[20]);

    //护士呼提示#/
    if(alarm_own.note.note1 & 0x02)
        curAlarmMap.insert(ALARM_NUSECALL,alarm_table[20]);

    //单次定时报警提示#/
    if(alarm_own.note.note4&0X01)
        curAlarmMap.insert(ALARM_ONETIME,alarm_table[20]);

    //循环定时报警提示#/
    if(alarm_own.note.note4&0X02)
        curAlarmMap.insert(ALARM_CYCLETIME,alarm_table[20]);

    if(alarm_N7.powerstate1==3)
        curAlarmMap.insert(ALARM_ELECTRICITY_STOP,alarm_table[14]);

    //自检报警判断#/
    if(PDOstate.state.N1_status==0x01&&(PDOstate.state.N1_sub_status==0x11||PDOstate.state.N1_sub_status==0x12))
    {
        switch(alarm_N5.alarm.errorstate7)
        {
        case 1:
            curAlarmMap.insert(ALARM_SELFCHECK_CHOKECLIP1,alarm_table[16]);
            break;
        case 2:
            curAlarmMap.insert(ALARM_SELFCHECK_CHOKECLIP2,alarm_table[16]);
            break;
        case 3:
            curAlarmMap.insert(ALARM_SELFCHECK_VPBUBBLE,alarm_table[16]);
            break;
        case 4:
            curAlarmMap.insert(ALARM_SELFCHECK_APBUBBLE,alarm_table[16]);
            break;
        case 5:
            curAlarmMap.insert(ALARM_SELFCHECK_LEVEL,alarm_table[16]);
            break;
        case 6://动脉压故障#/
            curAlarmMap.insert(ALARM_SELFCHECK_PA_ERROR,alarm_table[16]);
            break;
        case 7: //静脉压故障#/
            curAlarmMap.insert(ALARM_SELFCHECK_PV_ERROR,alarm_table[16]);
            break;
        case 8://跨膜压故障#/
            curAlarmMap.insert(ALARM_SELFCHECK_TMP_ERROR,alarm_table[16]);
            break;
        case 9:
            curAlarmMap.insert(ALARM_SELFCHECK_BLOODLEAK,alarm_table[16]);
            break;
        case 10:
            curAlarmMap.insert(ALARM_SELFCHECK_BLOODOXYGEN,alarm_table[16]);
            break;
        case 11:
            curAlarmMap.insert(ALARM_SELFCHECK_TONOMETER,alarm_table[16]);
            break;

        default:
            if(alarm_N5.alarm.errorstate7>0)
            {
                unsigned int key=502070001+alarm_N5.alarm.errorstate7*10;
                curAlarmMap.insert(key,alarm_table[21]);//N5扩展自检失败#/
            }
            break;

        }

        switch(alarm_N3.alarm.errorstate7)
        {
        case 1:
            curAlarmMap.insert(ALARM_SELFCHECK_N7ERROR,alarm_table[16]);//N7自检失败#/
            break;
        case 2:
            curAlarmMap.insert(ALARM_SELFCHECK_N9ERROR,alarm_table[16]);//N9自检失败#/
            break;
        case 3:
            curAlarmMap.insert(ALARM_SELFCHECK_N10ERROR,alarm_table[16]);//N10自检失败#/
            break;
        case 4:
            curAlarmMap.insert(ALARM_SELFCHECK_N15ERROR,alarm_table[16]); //N15自检失败#/
            break;
        case 5:
            curAlarmMap.insert(ALARM_SELFCHECK_N5ERROR,alarm_table[16]);//N5自检失败#/
            break;

        default:
            if(alarm_N3.alarm.errorstate7>0)
            {
                unsigned int key=302070001+alarm_N3.alarm.errorstate7*10;
                curAlarmMap.insert(key,alarm_table[22]);//N3扩展自检失败#/
            }
            break;
        }

        if(alarm_N9.N9_status & 0x20)
            curAlarmMap.insert(ALARM_SELFCHECK_N9BLOODOUT,alarm_table[16]);
        if(alarm_N9.N9_status & 0x40)
            curAlarmMap.insert(ALARM_SELFCHECK_N9NOFEEDBACK,alarm_table[16]);     
        if((alarm_N9.N9_status & 0x80)&&(alarm_N9.N9_status&0x01)==0)   //血泵门打开引发的自检不过#/
            curAlarmMap.insert(ALARM_SELFCHECK_N9SELFCHECKFAIL,alarm_table[16]);

        if(alarm_N10.N9_status & 0x20)
            curAlarmMap.insert(ALARM_SELFCHECK_N10BLOODOUT,alarm_table[16]);
        if(alarm_N10.N9_status & 0x40)
            curAlarmMap.insert(ALARM_SELFCHECK_N10NOFEEDBACK,alarm_table[16]);
        if((alarm_N10.N9_status & 0x80)&&(alarm_N10.N9_status&0x01)==0)   //血泵门打开引发的自检不过#/
            curAlarmMap.insert(ALARM_SELFCHECK_N10SELFCHECKFAIL,alarm_table[16]);

        if(alarm_N15.workMode==0x04&&alarm_N15.workModeSub==0x99)
            curAlarmMap.insert(ALARM_SELFCHECK_N15SELFCHECKFAIL,alarm_table[16]);

        if(note_N3.note.note4 & 0x02)
            curAlarmMap.insert(ALARM_SELFCHECK_BLOODPARTSELFCHECKFAIL,alarm_table[16]);
        else if(note_N3.note.note4 & 0x80)
            curAlarmMap.insert(ALARM_SELFCHECK_WATERPARTSELFCHECKFAIL,alarm_table[16]);
    } //自检报警判断#/
    else  //非自检时#/
    {
        /*
        if(alarm_N9.N9_status & 0x20)
            curAlarmMap.insert(ALARM_N9BLOODOUT ,alarm_table[5]);

        if(alarm_N10.N9_status & 0x20)
            curAlarmMap.insert(ALARM_N10BLOODOUT ,alarm_table[5]);
        */
    }
}

//报警声音处理#/
void AlarmModule::sound(char para)
{
    switch(para) {

    case 0:
        audio->alarmStop();
        break;

    case 4://一声响#/
        audio->alarmStop();
        audio->alarmStart();
        audio->playNotice("./resource/sounds/notice.wav",3000);
        break;

    case 3://一声响#/
        audio->alarmStop();
        audio->alarmStart();
        audio->playNotice("./resource/sounds/notice.wav",3000);
        break;

    case 2://中#/
          audio->alarmStop();
          audio->alarmStart();
          audio->playAlarm("./resource/sounds/genmed.wav",3000);
        break;

    case 1: //高#/
          audio->alarmStop();
          audio->alarmStart();
          audio->playAlarm("./resource/sounds/genhi.wav",3000);
        break;

   case 5: //音乐#/
        break;

   case 6: //法规，中，一定时间#/
        break;

    default:
        break;
    }
}

void AlarmModule::light(char para)
{
    switch(para) {
    case 0:
        if(PDOstate.state.N1_sub_status<=SELFTEST_COMPLETE_FLUID)
            lamp->stop(2);  //自检时黄灯
        else if(PDOstate.state.N1_status==CLEANING)//消毒时黄灯#/
            lamp->stop(2);
        else
            lamp->stop(3);  //平时绿灯
        break;
    case 1: //快,红灯#/
        lamp->flash(1,500);
        break;
    case 2://中#/
        lamp->flash(2,2000);//黄灯#/
        /*
        if((para&0xf0)==0x10)
           lamp->flash(1,2000);//红灯#/
        if((para&0xf0)==0x20)
           lamp->flash(2,2000);//黄灯#/
           */
        break;
    case 3://低 绿灯#/
        lamp->flash(3,2000);
        break;

    default:
        break;
    }
}

void AlarmModule::action(char para)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("Alarm_action-in");
#endif
//    para=para;
    if(para&0x01) //停血泵#/
    {
#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_BloodStop);
#else
        MasterCtrol_CMD_BloodStop();   //停血泵#/
        us_sleep(10000);
#endif
        Alarm_BloodPump_Flag=true;
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("Alarm_action-in01");
#endif
    if(para&0x02)
    {
#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_ClipStop);
        emit signalSDO(DE_CMD_Clip2Stop);
#else
        MasterCtrol_CMD_ClipStop();    //阻流夹#/
        us_sleep(10000);
        MasterCtrol_CMD_Clip2Stop();   //阻流夹#/
        us_sleep(10000);
#endif
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("Alarm_action-in02");
#endif
    if(para&0x04)  //停肝素泵#/
    {
#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_HeparinPumpStop);
#else
        MasterCtrol_CMD_HeparinPumpStop();            //停肝素泵#/
        us_sleep(10000);
#endif
        Alarm_HeparinPump_Flag=true;
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("Alarm_action-in03");
#endif
    if(para&0x08)
    {
#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_SubsStop);
#else
         MasterCtrol_CMD_SubsStop();    //停止置换液泵#/
         us_sleep(10000);
#endif
        Waterway_alarm_flag=true;
        Alarm_SubsPump_Flag=true;
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("Alarm_action-in04");
#endif
    if(para&0x10)
    {
         Alarm_Pass_Flag=true;
#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_Bypass);
#else
         MasterCtrol_CMD_Bypass();     //启动旁路#/
         us_sleep(10000);
#endif
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("Alarm_action-in05");
#endif
    if(para&0x20)
    {
#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_UFStop);
#else
        MasterCtrol_CMD_UFStop();     //停止超虑#/
        us_sleep(10000);
#endif
        Alarm_UFPump_Flag=true;
    }
    if(para&0x40)                       //停止供液#/
    {

    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("Alarm_action-out");
#endif
}

void AlarmModule::Set_WorkState(int state)
{
    if(state==DE_RESET_FLAG)
    {
#if DEBUG_OTHER_FLAG
    Debug_insert("AlarmModule::Set_WorkState_in");
#endif
        Alarm_Reset_Finish=false;   //报警复位操作是否完成标志 true 完成

        stateflag=DE_RESET_FLAG;   //20151022 修改#/

        //护士呼提示消除#/
        alarm_own.note.note1=0;
        //单次定时报警提示消除#/
        //循环定时报警提示消除#/
        alarm_own.note.note4=0;
        dataWrite(OWNNOTE, &alarm_own);


        Try_Map_mutex();
        this->Reset_AlarmHandle();
        Map_mutex.unlock();
        this->sound(0);
        this->light(0);
        Alarmcolorflag=0;
//        alarmtimer.start(500);
        Waterway_alarm_flag=false;
        Alarm_BloodPump_Flag=false;  //发生停血泵报警标志#/
        Alarm_UFPump_Flag=false;//发生停超滤泵报警标志#/
        Alarm_SubsPump_Flag=false;//发生停置换泵报警标志#/
        Alarm_HeparinPump_Flag=false;//发生停肝素泵报警标志#/
        if(Alarm_Pass_Flag==true)
        {
            if(G_BYPASS_BUT_FLAG!=DE_BYPASS_CHECKED)
            {
#if HJX_COMMUNICATION_SWITCH
            emit signalSDO(DE_CMD_Nobypass);
#else
            us_sleep(10000);
            MasterCtrol_CMD_Nobypass();//停旁路#/
            us_sleep(10000);
#endif
            }
            Alarm_Pass_Flag=false;
        }

        memset(&Prompt_own,0,sizeof(PacketOwnNote));//提示性报警全部消除#/

        stateflag=DE_NORMAL_FLAG;   //20151022 修改#/

        dataNodeUpdate(PACKET_NAME_BTNALARM, &Alarmcolorflag);

        Alarm_Reset_Finish=true;   //报警复位操作是否完成标志 true 完成
#if DEBUG_OTHER_FLAG
    Debug_insert("AlarmModule::Set_WorkState_out");
#endif
    }
    else if(state==DE_MUTE_FLAG)
    {
        stateflag=DE_MUTE_FLAG;
        this->sound(0);
        alarmtimer.start(1000);
    }
    else if(state==DE_HIDDED_FLAG)
    {
        stateflag=DE_HIDDED_FLAG;
        closeAllDialog();
    }
    else
        stateflag=state;
}

void AlarmModule::Reset_AlarmHandle()
{
    QMap<int,AlarmInfor>::iterator it;
    QMap<int,AlarmInfor>::iterator it2;
    clearMessagesDialogInfo();
    closeMessagesDialog();
    clearAlarmDialogInfo();
    closeAlarmDialog();
    clearWarnDialogInfo();
    closeWarnDialog();

    unsigned int maxtime=0;

    if(curtimeAlarmMap.size()>0)
    {
        for(it=curtimeAlarmMap.begin();it!=curtimeAlarmMap.end();it++)
        {

            if(DataSql_Check(it.value().time,it.value().alarmid))
            {
                continue;
            }
            else
            {
                //查找是否已经保存过了#/
                for(it2 = historyAlarmMap.begin(); it2 != historyAlarmMap.end(); it2++)
                {
                    if(it2.value().alarmid==it.value().alarmid)
                    {
                        if(maxtime<it2.value().time)
                            maxtime=it2.value().time;    //获取这类报警的最后一次时间#/
                    }
                }
                //为了避免频繁的报警保存，因此暂定间隔在2分钟内的，不进行保存#/
                if((it.value().time-maxtime)>120)
                {
                   // DataSql_InsertwarnTable(it.value().time,it.value().who,it.value().code,it.value().pri,it.value().alarmid,it.value().dispose);
                    DataList_InsertwarnTable(it.value().time,it.value().who,it.value().code,it.value().pri,it.value().alarmid,it.value().dispose);
                    historyAlarmID++;
                    if(historyAlarmID>DE_HISTORYALARM_MAXNUM)   //超过保存范围，则要进行自动删除最早的记录
                    {
                        DataSql_DeletewarnTable(historyAlarmMap.begin().value().time,historyAlarmMap.begin().value().alarmid);
                        historyAlarmMap.remove(historyAlarmMap.begin().key());
                    }
                }
                maxtime=0;
            }
        }
    }


    curtimeAlarmMap.clear();
    curAlarmMap.clear();
    preAlarmMap.clear();

    historyAlarmID=0;
    historyAlarmMap.clear();

    dataManage->dataCan.CleanData();

    int rownum=0;
    rownum=DataSql_GetrownumwarnTable();
    /*
    if(rownum>=0&&rownum<=100)
        historyAlarmMap=DataSql_GetinforwarnTable(0,100,historyAlarmID);
    else
        historyAlarmMap=DataSql_GetinforwarnTable(rownum-100,100,historyAlarmID);

    if(historyAlarmMap.size()<=0)
    {
        historyAlarmMap=DataSql_GetinforwarnTable(0,100,historyAlarmID);
    }
    */
    historyAlarmMap=DataSql_GetinforwarnTable(0,rownum,historyAlarmID);
}

QMap<int,AlarmInfor> AlarmModule::Get_curtimeAlarmMap()
{
    return curtimeAlarmMap;
}

QMap<int,AlarmInfor> AlarmModule::Get_historyALarmMap()
{
    return historyAlarmMap;
}

void AlarmModule::Afresh_Alarm()
{
    Try_Map_mutex();
    clearMessagesDialogInfo();
    closeMessagesDialog();
    clearAlarmDialogInfo();
    closeAlarmDialog();
    clearWarnDialogInfo();
    closeWarnDialog();
    curAlarmMap.clear();//当前报警信息列表#/
    preAlarmMap.clear();//上一次报警信息列表#/
    Alarm_BloodPump_Flag=false;  //发生停血泵报警标志#/
    Alarm_UFPump_Flag=false;//发生停超滤泵报警标志#/
    Alarm_SubsPump_Flag=false;//发生停置换泵报警标志#/
    Alarm_HeparinPump_Flag=false;//发生停肝素泵报警标志#/
    Map_mutex.unlock();
}

int AlarmModule::Get_CurrentAlarmNum()
{
    return curtimeAlarmMap.size();
}

bool AlarmModule::Is_CurrentAlarm(int id)
{
    if(curtimeAlarmMap.contains(id))
        return true;
    else
        return false;
}

void AlarmModule::Custom_Prompt(int pid)    //自定义的提示信息功能,按复位键可以消除#/
{
    switch(pid)
    {
    case PROMPT_SYSTOLIC_UP: //收缩压上限报警#/
        Prompt_own.note.note1 |=0x01;
        break;
    case PROMPT_SYSTOLIC_DOWN://收缩压下限报警#/
         Prompt_own.note.note1 |=0x02;
        break;
    case PROMPT_DIASTOLIC_UP:  //舒张压UP报警#/
         Prompt_own.note.note1 |=0x04;
        break;
    case PROMPT_DIASTOLIC_DOWN:  //舒张压DOWN报警#/
         Prompt_own.note.note1 |=0x08;
        break;
    case PROMPT_PULSE_UP:  //脉搏UP报警#/
         Prompt_own.note.note1 |=0x10;
        break;
    case PROMPT_PULSE_DOWN://脉搏DOWN报警#/
         Prompt_own.note.note1 |=0x20;
        break;
    case PROMPT_HEPARIN_TIMESTOP: //肝素泵时间到停止#/
         Prompt_own.note.note1 |=0x40;
        break;
    case PROMPT_HEPARIN_NOOPEN:  //肝素泵没有打开#/
         Prompt_own.note.note1 |=0x80;
        break;
    case PROMPT_PLEASEOPENBLOOD://报警处理完后，提示打开血泵#/
         Prompt_own.note.note2 |=0x01;
        break;
    case PROMPT_XMLFILEERROR:  //XML文件读取异常#/
         Prompt_own.note.note2 |=0x02;
       break;
    default:
        break;
    }
}

void AlarmModule::Custom_Prompt_Clear(int pid) //清除指定自定义的提示信息#/
{
    switch(pid)
    {
    case PROMPT_SYSTOLIC_UP: //收缩压上限报警#/
        Prompt_own.note.note1 &=0x01;
        break;
    case PROMPT_SYSTOLIC_DOWN://收缩压下限报警#/
         Prompt_own.note.note1 &=0x02;
        break;
    case PROMPT_DIASTOLIC_UP:  //舒张压UP报警#/
         Prompt_own.note.note1 &=0x04;
        break;
    case PROMPT_DIASTOLIC_DOWN:  //舒张压DOWN报警#/
         Prompt_own.note.note1 &=0x08;
        break;
    case PROMPT_PULSE_UP:  //脉搏UP报警#/
         Prompt_own.note.note1 &=0x10;
        break;
    case PROMPT_PULSE_DOWN://脉搏DOWN报警#/
         Prompt_own.note.note1 &=0x20;
        break;
    case PROMPT_HEPARIN_TIMESTOP: //肝素泵时间到停止#/
         Prompt_own.note.note1 &=0x40;
        break;
    case PROMPT_HEPARIN_NOOPEN:  //肝素泵没有打开#/
         Prompt_own.note.note1 &=0x80;
        break;
    case PROMPT_PLEASEOPENBLOOD://报警处理完后，提示打开血泵#/
         Prompt_own.note.note2 &=0x01;
        break;
    case PROMPT_XMLFILEERROR:  //XML文件读取异常#/
         Prompt_own.note.note2 &=0x02;
       break;
    default:
        break;
    }
}

void AlarmModule::timerHandle()
{
    static int selcount=0;

    //-----------------------测试专用----------------------------------
#if DE_TEST_SWITCH
    switch(selcount)
    {
    case 0:
        //动脉 上限#/
      //  alarm_N5.alarm.errorstate2=0x01;
        //血泵失控#/
        alarm_N9.N9_status = 0x20;

        break;

    case 1:
      //  alarm_N5.alarm.errorstate2=0x01;
        //血泵失控#/
        alarm_N9.N9_status = 0x20;
        break;

    case 2:
        //静脉下限#/
   //     alarm_N5.alarm.errorstate2=0x08;
        //血泵1 盖打开#/
//       alarm_N9.N9_status=0x01;
        //血泵失控#/
        alarm_N9.N9_status = 0x00;
        //肝素泵报警#/
        alarm_N15.senserState=0x02;
        break;

    case 3:
        //肝素泵报警#/
        alarm_N15.senserState=0x02;
        break;

    case 4:
       //出水口总电导度范围报警 N3#/
     //   alarm_N3.alarm.errorstate1=0x40;
        //肝素泵报警#/
        alarm_N15.senserState=0x02;
        break;

    case 5:
        //漏血报警#/
        alarm_N5.alarm.errorstate3=0x20;
        break;
    }
    selcount++;

    if(selcount>4)
        selcount=0;

    dataNodeUpdate(PACKET_NAME_BLOODPUMP, &alarm_N9);
      dataNodeUpdate(PACKET_STATUS_MASTALARM, &alarm_N3);
            dataNodeUpdate(PACKET_NAME_HEPARINDATA, &alarm_N15);
    /*
    //报警处理#/
    this->Get_AlarmCode();
    this->Alarm_Handle();
    */
#else
    if(stateflag==DE_RESET_FLAG)
    {
        selcount++;
        if(selcount>1)   //1s
        {
            selcount=0;
            alarmtimer.stop();
            stateflag=DE_NORMAL_FLAG;
        }
    }
    else if(stateflag==DE_MUTE_FLAG)
    {
        selcount++;
        if(selcount>120)   //120s
        {
            stateflag=DE_NORMAL_FLAG;
            selcount=0;
            alarmtimer.stop();
        }
    }
    else
    {
        selcount=0;
        alarmtimer.stop();
        stateflag=DE_NORMAL_FLAG;
    }

#endif
}

//相应数据包变量变化时的通知接口#/
void AlarmModule::ActionHandle(const void *dataPtr, int name)
{

//    static bool mutex_flag=false;

#if SYSTEM_DEBUG_FLAG
    Debug_insert("AlarmModule-ActionHandle-in");
#endif

    if(workstate.tipState.current==DEBUG)
        return;

    if (name==PACKET_NAME_BLOODPUMP){ //血泵#/
        alarm_N9 = *(struct PacketBloodPump*)dataPtr;
    }

    if (name==PACKET_NAME_SUBBLOODPUMP){ //血泵2#/
         alarm_N10 = *(struct PacketBloodPump*)dataPtr;
    }

    if(name==PACKET_STATUS_NOTE){     //提示报警 供回液管状态 对应主控板PDO0的4字节#/
        note_N3 = *(PacketStatusNote *)dataPtr;
        note_N3.note.note4=0;
//        note_N3.note.note2 &=0x7f;  // 屏蔽 超滤启动#/
    }

    //相同的报警项目下，在只有n3的情况下才报n3报警,否则只报n5.#/
    if (name==PACKET_STATUS_MASTALARM){ //主控板告警状态#/
        alarm_N3 = *(PacketStatusAlarm *) dataPtr;
    }

    if (name==PACKET_STATUS_ALARM){     //监控板告警状态#/
         alarm_N5 = *(PacketStatusAlarm *) dataPtr;

         if(alarm_N5.alarm.errorstate3 &0x40)//温度超过41C，屏蔽温度上限告警#/
         {
           alarm_N5.alarm.errorstate2 &=0xbf;
         }
         if(alarm_N5.alarm.errorstate3 &0x80)//温度低于30C，屏蔽温度下限告警#/
         {
           alarm_N5.alarm.errorstate2 &=0x7f;
         }
    }

    if (name==OWNNOTE){  //本机的提示报警信息#/
        alarm_own= *(PacketOwnNote *) dataPtr;
    }

    if (name==PACKET_NAME_HEPARINDATA){  //肝素泵的信息#/
        alarm_N15= *(PackN15PDO *) dataPtr;
    }

    if (name==PACKET_NAME_POWER){  //电源板信息处理#/
        alarm_N7= *(PacketPower*) dataPtr;
    }

    /*
    if(mutex_flag==true)
    {
        mutex_flag=false;
        return;
    }
    else
        mutex_flag=true;
        */

   if(G_rebirth_flag==true)  //重生操作时取消处理#/
        return;

  //报警处理#/
    if(stateflag != DE_RESET_FLAG&&stateflag != DE_HIDDED_FLAG)
    {
        Try_Map_mutex();
        this->Get_AlarmCode();
        this->Alarm_Handle();
        Map_mutex.unlock();
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("AlarmModule-ActionHandle-out");
#endif
}

//获取人机板当前主状态，报警判断条件#/
int AlarmModule::Check_N1_status()
{
    if(PDOstate.state.N1_status!=0x01&&
       PDOstate.state.N1_status!=0x03&&
       PDOstate.state.N1_status!=0x05)
    {
        return DE_NOALARM;
    }
    else if(PDOstate.state.N1_status==0x01||
            PDOstate.state.N1_status==0x03||
            PDOstate.state.N1_status==0x05)
    {
        return DE_NORMAL;
    }

    return DE_NOALARM;
}

//获取子状态，报警判断条件#/
int AlarmModule::Check_N1_sub_status()
{
    if((PDOstate.state.N1_sub_status>=0x11 && PDOstate.state.N1_sub_status<=0x13)||
        PDOstate.state.N1_sub_status==0x16 || PDOstate.state.N1_sub_status==0x18||PDOstate.state.N1_sub_status==0x1a)
    {
        return DE_NOBLOODHAVEFLUID;
    }
    else if((PDOstate.state.N1_sub_status<0x11 || PDOstate.state.N1_sub_status>0x15)&&
        PDOstate.state.N1_sub_status!=0x19 && PDOstate.state.N1_sub_status!=0x1d)
    {
        return DE_OTHERSTATUS1;
    }

    return DE_OTHERSTATUS1;
}

int AlarmModule::Check_N1_blood_status()
{
    if(PDOstate.state.N1_blood_status==0x13||PDOstate.state.N1_blood_status==0x14)
    {
        return DE_ONLINE;
    }
    else if(PDOstate.state.N1_blood_status==0x12||PDOstate.state.N1_blood_status==0x19||
            PDOstate.state.N1_blood_status==0x1a||PDOstate.state.N1_blood_status==0x16)
    {
        return DE_ISOUFHP;
    }
    else if(PDOstate.state.N1_blood_status==0x76) //纯热水消毒#/
    {
        return DE_ONLYHEATWATER;
    }
    else
        return DE_NOTONLINE;
}

int AlarmModule::Check_N1_water_status()
{
    if((PDOstate.state.N1_water_status >=0x21 && PDOstate.state.N1_water_status <=0x25 ))
        return DE_WATEROPEN;
    else if(PDOstate.state.N1_water_status == 0x7a)
        return DE_CLEARDRAIN;
    else if(PDOstate.state.N1_water_status == 0x74)
        return DE_LOOPDRAIN;   //消毒循环#/
    else
        return DE_WATEROFF;
}

int AlarmModule::Check_alarm_type(StrAlarm type)
{
    char byte=(type.code>>16)&0xf0;
    switch(byte)
    {
        case 0x30:
            return DE_ONCE;
        case 0x20:
            return DE_RESTORE;
        case 0x10:
            return DE_KEEP;
        default:
            return DE_RESTORE;
    }
}

int AlarmModule::Check_Promptbox_type(int code)
{
    char byte=(code>>16)&0x0f;

    switch(byte)
    {
        case 0:  //无提示框#/
            return DE_NONE;
            break;

        case 1:  //跳出提示，需确认#/
            return DE_MUSTCONFIRM;
            break;

        case 2:  //跳出提示，不确认，延时消失#/
            return DE_DELAYDISAPPEAR;
            break;

        case 3:  //不跳出提示#/
            return DE_NOPOP;
            break;

        case 4:  //提示与报警联动#/
            return DE_LINKAGE;
            break;

        default:
            return DE_NONE;
            break;
    }
}

int AlarmModule::Save_to_histroy(AlarmInfor alarm)
{
    char savestate=0;      //1:已保存#/
    unsigned int maxtime=0;
    bool ret;
    QMap<int,AlarmInfor>::iterator it;
    //查找是否已经保存过了#/
    for(it = historyAlarmMap.begin(); it != historyAlarmMap.end(); it++)
    {
        if(it.value().alarmid==alarm.alarmid)
        {
            if(maxtime<it.value().time)
                maxtime=it.value().time;    //获取这类报警的最后一次时间#/
        }
    }

    //为了避免频繁的报警保存，因此暂定间隔在5分钟内的，不进行保存#/
    if((alarm.time-maxtime)<300)
    {
        savestate=1;
    }

    if(savestate==0)
    {
        historyAlarmMap.insert(historyAlarmID,alarm);
        historyAlarmID++;  
    }

    //查询该条报警记录是否已保存进历史报警表#/
    if(savestate==1)
    {
        return true;
    }
    else//往报警表插入一条记录#/
    {
       DataList_InsertwarnTable(alarm.time,alarm.who,alarm.code,alarm.pri,alarm.alarmid,alarm.dispose);
       ret=true;
       if(historyAlarmID>DE_HISTORYALARM_MAXNUM)   //超过保存范围，则要进行自动删除最早的记录
       {
           DataSql_DeletewarnTable(historyAlarmMap.begin().value().time,historyAlarmMap.begin().value().alarmid);
           historyAlarmMap.remove(historyAlarmMap.begin().key());
       }
    }
#if DEBUG_DATA_SQL
    Debug_insert("DataSql_InsertwarnTable-out");
#endif
    return ret;
}

