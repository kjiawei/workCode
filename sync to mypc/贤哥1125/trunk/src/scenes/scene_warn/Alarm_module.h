/*****************************************************
** 作者： Huangjunxian
** 创始时间：2014-1-8
** 修改人：Huangjunxian
** 修改时间：2014-1-8
** 描述：
**    5058报警模块 《头文件》
*****************************************************/

#ifndef ALARM_MODULE_H
#define ALARM_MODULE_H

#include <QtGui>
#include "data_custom.h"

/*-------------------------报警内容条目宏定义----------------------------*/
/*
//监控板报警
//血路报警条目
#define ALARM_APUP  1000  //动脉压上限#/
#define ALARM_VPUP  1001  //静脉压上限#/
#define ALARM_TMPUP 1002  //跨膜压上限#/
#define ALARM_APDOWN 1003 //动脉压下限#/
#define ALARM_VPDOWN 1004
#define ALARM_TMPDOWN 1005

#define ALARM_FLUID 1006        //液位报警#/
#define ALARM_ARTERIES_AIR 1007 //动脉气泡#/
#define ALARM_NO_BLOOD 1008     //无血#/

#define ALARM_N9BLOODOUT 1009  //血泵1失控#/
#define ALARM_N10BLOODOUT 1010 //血泵2失控#/
#define ALARM_N9CAPOPEN 1011   //血泵1盖门打开#/
#define ALARM_N10CAPOPEN 1012  //血泵2盖门打开#/

#define ALARM_BLOODLEAK 1013   //漏血#/

#define ALARM_HEPARINBLOCK 1014 //肝素空液、阻塞#/

#define ALARM_BLOODMETER 1015 //血压计报警#/

#define ALARM_BDPUP 1016   //透析器前压力上限#/
#define ALARM_BDPDOWN 1017 //透析器前压力下限#/

#define ALARM_TEMPERUP 1018  //透析夜温上限#/
#define ALARM_TEMPERDOWN 1019  //透析夜下限#/

//水路报警条目#/
#define ALARM_OUTSUMCONDUP 2000  //出水口总电导度上限#/
#define ALARM_OUTSUMCONDDOWN 2001  //出水口总电导度下限#/
#define ALARM_OUTSUMCOND 2002  //出水口总电导度范围报警#/
#define ALARM_MIXSUMCONDUP 2003 //混合总电导度上限#/
#define ALARM_MIXSUMCONDDOWN 2004 //混合总电导度下限#/
#define ALARM_MIXSUMCOND 2005 //混合总电导度范围报警#/

#define ALARM_FTMPHIGHLIMIT 2006 //透析夜温度太高 极限#/
#define ALARM_FTMPLOWLIMIT 2007  //温度太低 极限#/
#define ALARM_FTMPUP 2008  //温度上限#/
#define ALARM_FTMPDOWN 2009 //温度下限#/
#define ALARM_FTMPRANGE 2010 //温度范围报警#/

#define ALARM_ACONDERROR 2011  //A液电导率错误#/
#define ALARM_BCONDERROR 2012  //B液电导率错误#/

#define ALARM_ANOFLUID 2013 //A无液#/
#define ALARM_BNOFLUID 2014 //B无液#/

#define ALARM_NOWATER 2015 //无水报警#/

#define ALARM_DISCONDUP 2016 //消毒液电导上限错误#/
#define ALARM_DISCONDDOWN 2017 //消毒液电导下限错误#/

#define ALARM_FLUIDOUTAIR 2018 //补液除气错误#/

#define ALARM_BALANCEBREAK 2019 //平衡腔破膜报警#/

//提示性报警条目#/
#define PROMPT_ATOBARREL 3000 //将A液吸管放到A液桶#/
#define PROMPT_ATODEVICE 3001 //将A液吸管插回机器#/
#define PROMPT_BTOBARREL 3002 //将B液管放到B液桶#/
#define PROMPT_BTODEVICE 3003 //将B液管插回机器#/
#define PROMPT_FEEDTODIA 3004 //将供液管接到透析器#/
#define PROMPT_FEEDTODEVICE 3005 //将供液管接回机器#/
#define PROMPT_BACKTODIA 3006 //将回液管接到透析器#/
#define PROMPT_BACKTODEVICE 3007 //将回液管接回机器#/
#define PROMPT_DRYFRAMEOPEN 3008 //将干粉架打开#/
#define PROMPT_DRYFRAMECLOSE 3009 //将干粉架关闭#/

//自检状态下的报警条目#/
#define ALARM_SELFCHECK_CHOKECLIP1 4000  //阻流夹1故障#/
#define ALARM_SELFCHECK_CHOKECLIP2 4001  //阻流夹2故障#/
#define ALARM_SELFCHECK_VPBUBBLE 4002  //静脉气泡故障#/
#define ALARM_SELFCHECK_APBUBBLE 4003  //动脉气泡故障#/
#define ALARM_SELFCHECK_LEVEL 4004  //液位故障#/
#define ALARM_SELFCHECK_BLOODLEAK 4005  //漏血监测器故障#/
#define ALARM_SELFCHECK_BLOODOXYGEN 4006 //血氧模块故障#/
#define ALARM_SELFCHECK_TONOMETER 4007  //血压模块故障#/

#define ALARM_SELFCHECK_N9BLOODOUT 4008  //血泵1失控#/
#define ALARM_SELFCHECK_N9NOFEEDBACK 4009 //血泵1无反馈信号#/
#define ALARM_SELFCHECK_N9SELFCHECKFAIL 4010 //血泵1自检失败#/
#define ALARM_SELFCHECK_N15SELFCHECKFAIL 4011 //肝素泵自检失败#/
#define ALARM_SELFCHECK_BLOODPARTSELFCHECKFAIL 4012 //血路自检失败#/
#define ALARM_SELFCHECK_WATERPARTSELFCHECKFAIL 4013 //水路自检失败#/
#define ALARM_SELFCHECK_N7ERROR 4014   //N7自检失败#/
#define ALARM_SELFCHECK_N9ERROR 4015   //N9自检失败
#define ALARM_SELFCHECK_N10ERROR 4016  //N10自检失败
#define ALARM_SELFCHECK_N15ERROR 4017  //N15自检失败
#define ALARM_SELFCHECK_N5ERROR 4018  //N5自检失败

#define ALARM_SELFCHECK_PA_ERROR 4019  //动脉压故障#/
#define ALARM_SELFCHECK_PV_ERROR 4020  //静脉压故障#/
#define ALARM_SELFCHECK_TMP_ERROR 4021  //跨膜压故障#/

*/

//血路报警条目
#define ALARM_APUP          502020040  //动脉压上限全码是0502020040 由于前面写0会出错，故不写  #/
//--前两位是设备ID号05，02：PDO第一个字节值 02：该PDO内第几个字节 004：该字节值十进制 0：报警类型-0普通 1自检 3提示#/
#define ALARM_VPUP          502020010  //静脉压上限#/
#define ALARM_TMPUP         502020160  //跨膜压上限#/
#define ALARM_APDOWN        502020080 //动脉压下限#/
#define ALARM_VPDOWN        502020020
#define ALARM_TMPDOWN       502020320

#define ALARM_FLUID         502010040        //液位报警#/
#define ALARM_ARTERIES_AIR  502010020 //动脉气泡#/
#define ALARM_NO_BLOOD      502010010    //无血报警#/
#define ALARM_VEIN_AIR      502011280    //静脉气泡#/

#define ALARM_N9BLOODOUT    901070320  //血泵1失控#/
#define ALARM_N10BLOODOUT   1001070320 //血泵2失控#/
#define ALARM_N9CAPOPEN     901070010   //血泵1盖门打开#/
#define ALARM_N10CAPOPEN    1001070010  //血泵2盖门打开#/

#define ALARM_BLOODLEAK     502030320   //漏血#/

#define ALARM_HEPARINBLOCK  1501060020 //肝素空液、阻塞#/

#define ALARM_BLOODMETER    502071110 //血压计报警  特别设置，没使用#/

#define ALARM_BDPUP         1016   //透析器前压力上限#/
#define ALARM_BDPDOWN       1017 //透析器前压力下限#/

#define ALARM_TEMPERUP      1018  //透析夜温上限#/
#define ALARM_TEMPERDOWN    1019  //透析夜下限#/

//水路报警条目#/
#define ALARM_OUTSUMCONDUP      502030010 //出水口总电导度上限#/
#define ALARM_OUTSUMCONDDOWN    502030020  //出水口总电导度下限#/
#define ALARM_OUTSUMCOND        302010640  //出水口总电导度范围报警#/
//#define ALARM_MIXSUMCONDUP      50203001  //混合总电导度上限#/
//#define ALARM_MIXSUMCONDDOWN    50203002  //混合总电导度下限#/
#define ALARM_MIXSUMCONDUP      502031110  //混合总电导度上限 特别设置，没使用#/
#define ALARM_MIXSUMCONDDOWN    502031120  //混合总电导度下限 特别设置，没使用#/
#define ALARM_MIXSUMCOND        302010320  //混合总电导度范围报警#/

#define ALARM_FTMPHIGHLIMIT     502030640 //透析夜温度太高 极限#/
#define ALARM_FTMPLOWLIMIT      502031280  //温度太低 极限#/
#define ALARM_FTMPUP            502020640  //温度上限#/
#define ALARM_FTMPDOWN          502021280 //温度下限#/
#define ALARM_FTMPRANGE         302011280 //温度范围报警#/

#define ALARM_ACONDERROR        302010020  //A液电导率错误#/
#define ALARM_BCONDERROR        302010040  //B液电导率错误#/

#define ALARM_ANOFLUID          302020080    //A无液#/
#define ALARM_BNOFLUID          302020160    //B无液#/

#define ALARM_NOWATER           302010010 //无水报警#/

#define ALARM_DISCONDUP         302010080 //消毒液电导上限错误#/
#define ALARM_DISCONDDOWN       302010160 //消毒液电导下限错误#/

#define ALARM_FLUIDOUTAIR       302020640 //补液除气错误#/



#define ALARM_BALANCEBREAK      2019 //平衡腔破膜报警#/

#define ALARM_N3CANOPENFAIL     101060010  //N3 canopen通信失败#/
#define ALARM_N5CANOPENFAIL     101060020  //N5 canopen通信失败#/
#define ALARM_N9CANOPENFAIL     101060040  //N9 canopen通信失败#/
#define ALARM_N10CANOPENFAIL    101060080  //N10 canopen通信失败#/
#define ALARM_N15CANOPENFAIL    101060160  //N15 canopen通信失败#/
#define ALARM_N7CANOPENFAIL     101060320  //N7 canopen通信失败#/

#define ALARM_NUSECALL               101060640      //护士呼提示#/
#define ALARM_ONETIME                 101040013      //单次定时报警提示#/
#define ALARM_CYCLETIME              101040023      //循环定时报警提示#/

#define ALARM_ELECTRICITY_STOP  701020030  //N7 市电停止报警#/

#define ALARM_WATERWORKFAIL     101070001  //水路工作异常#/

//提示性报警条目#/
#define PROMPT_ATOBARREL        301040012 //将A液吸管放到A液桶#/
#define PROMPT_ATODEVICE        301040022 //将A液吸管插回机器#/
#define PROMPT_BTOBARREL        301040042 //将B液管放到B液桶#/
#define PROMPT_BTODEVICE        301040082 //将B液管插回机器#/
#define PROMPT_FEEDTODIA        301040162 //将供液管接到透析器#/
#define PROMPT_FEEDTODEVICE     301040322 //将供液管接回机器#/
#define PROMPT_BACKTODIA        301040642 //将回液管接到透析器#/
#define PROMPT_BACKTODEVICE     301041282 //将回液管接回机器#/
#define PROMPT_DRYFRAMEOPEN     301050012 //将干粉架打开#/
#define PROMPT_DRYFRAMECLOSE    301050022 //将干粉架关闭#/

#define PROMPT_SETUPCORRESPONDING    301050042 //将相应管路接到置换液接口#/ Set up corresponding line displacement fluid interface
#define PROMPT_REMOVETHEINTERFACE    301050082 //将置换液接口管路移除#/ Remove the displacement fluid interface line
#define PROMPT_SETUPFLUSHINTERFACE   301050162 //将相应管路接到置换液冲洗接口#/ Set up corresponding line displacement fluid flush interface
#define PROMPT_REMOVEFLUSHINTERFACE  301050322 //将置换液冲洗接口管路移除#/ Remove the replacement wash liquid interface line

#define PROMPT_MANUAL_PREFLUSH  101070002  //主控板手动预冲状态错误#/
#define PROMPT_AUTO_PREFLUSH    101070004  //bit2：自动预冲#/
#define PROMPT_ONLINE_PREFLUSH  101070008  //bit3：在线预冲#/
#define PROMPT_GETBLOOD         101070016  //bit4：引血#/
#define PROMPT_ONLINE_GETBLOOD  101070032  //bit5：在线引血#/
#define PROMPT_RETURNBLOOD      101070064  //bit6：回血#/
#define PROMPT_ONLINE_RETURNBLOOD  101070128  //bit7：在线回血#/

#define PROMPT_SYSTOLIC_RANGE   101050011  //收缩压范围报警#/
#define PROMPT_DIASTOLIC_RANGE  101050021  //舒张压范围报警#/
#define PROMPT_PULSE_RANGE      101050041  //脉搏范围报警#/

//自定义提示性报警，复位键可以消除#/
#define PROMPT_SYSTOLIC_UP      101010010  //收缩压上限报警#/
#define PROMPT_SYSTOLIC_DOWN    101010020  //收缩压下限报警#/
#define PROMPT_DIASTOLIC_UP     101010030  //舒张压UP报警#/
#define PROMPT_DIASTOLIC_DOWN   101010040  //舒张压DOWN报警#/
#define PROMPT_PULSE_UP         101010160  //脉搏UP报警#/
#define PROMPT_PULSE_DOWN       101010320  //脉搏DOWN报警#/

#define PROMPT_HEPARIN_TIMESTOP 101010640  //肝素泵时间到停止#/
#define PROMPT_HEPARIN_NOOPEN   101011280  //肝素泵没有打开#/
#define PROMPT_PLEASEOPENBLOOD  101020010  //报警处理完后，提示打开血泵#/
#define PROMPT_XMLFILEERROR     101020020  //XML文件读取异常#/



//自检状态下的报警条目#/
#define ALARM_SELFCHECK_CHOKECLIP1              502070011  //阻流夹1故障#/
#define ALARM_SELFCHECK_CHOKECLIP2              502070021  //阻流夹2故障#/
#define ALARM_SELFCHECK_VPBUBBLE                502070031  //静脉气泡故障#/
#define ALARM_SELFCHECK_APBUBBLE                502070041  //动脉气泡故障#/
#define ALARM_SELFCHECK_LEVEL                   502070051  //液位故障#/
#define ALARM_SELFCHECK_PA_ERROR                502070061  //动脉压故障#/
#define ALARM_SELFCHECK_PV_ERROR                502070071  //静脉压故障#/
#define ALARM_SELFCHECK_TMP_ERROR               502070081  //跨膜压故障#/
#define ALARM_SELFCHECK_BLOODLEAK               502070091  //漏血监测器故障#/
#define ALARM_SELFCHECK_BLOODOXYGEN             502070101  //血氧模块故障#/
#define ALARM_SELFCHECK_TONOMETER               502070111  //血压模块故障#/

#define ALARM_SELFCHECK_N9BLOODOUT              901070321 //血泵1失控  特别设置，没使用#/
#define ALARM_SELFCHECK_N9NOFEEDBACK            901070641 //血泵1无反馈信号#/
#define ALARM_SELFCHECK_N9SELFCHECKFAIL         901071281 //血泵1自检失败#/

#define ALARM_SELFCHECK_N10BLOODOUT             1001070321 //血泵2失控  特别设置，没使用#/
#define ALARM_SELFCHECK_N10NOFEEDBACK           1001070641 //血泵2无反馈信号#/
#define ALARM_SELFCHECK_N10SELFCHECKFAIL        1001071281 //血泵2自检失败#/

#define ALARM_SELFCHECK_N15SELFCHECKFAIL        1501051531 //肝素泵自检失败#/
#define ALARM_SELFCHECK_BLOODPARTSELFCHECKFAIL  301070021 //血路自检失败#/
#define ALARM_SELFCHECK_WATERPARTSELFCHECKFAIL  301071281 //水路自检失败#/
#define ALARM_SELFCHECK_N7ERROR                 302070011 //N7自检失败#/
#define ALARM_SELFCHECK_N9ERROR                 302070021 //N9自检失败
#define ALARM_SELFCHECK_N10ERROR                302070031 //N10自检失败
#define ALARM_SELFCHECK_N15ERROR                302070041 //N15自检失败
#define ALARM_SELFCHECK_N5ERROR                 302070051 //N5自检失败


/*------------------------相应的状态宏定义--------------------------*/
//主工作状态#/
#define DE_NOALARM 0 //不需要报警状态#/
#define DE_NORMAL 1 //正常状态#/
#define DE_CLEAN 2  //消毒#/
#define DE_DRAIN 3 //排水#/

//工作子状态--血路#/
#define DE_NOBLOODHAVEFLUID 1  //预冲无血有液前，即自检之后#/
#define DE_OTHERSTATUS1 2      //上面状态之外的状态#/
#define DE_BEFOREBLOOD  3      //预冲有血前，开始预冲后#/
#define DE_OTHERSTATUS2 4      //上面状态之外的状态#/
#define DE_RETURNNOBLOOD 5     //回血，无血后#/
//模式#/
#define  DE_ONLINE 1          //HDFonline HFonline时#/
#define  DE_ISOUFHP 2         //ISO UF  PE/HP#/
#define  DE_NOTONLINE 3
#define  DE_ONLYHEATWATER 4 //纯热水消毒#/
//水路#/
#define DE_WATEROPEN 1        //水路启动时#/
#define DE_CLEARDRAIN 2        //清洁后排空#/
#define DE_WATEROFF  3        //水路不启动时#/
#define DE_LOOPDRAIN 4       //消毒循环#/
//报警来源#/
#define ALARM_RESET   0   //报警复位#/
#define NOTE          1   //提示性报警#/
#define ALARM_PDO1    2   //来源N3或者N5的PDO1#/
#define ALARM_N3      2   //来源N3#/
#define ALARM_N5      3
#define ALARM_PDO2    3
#define ALARM_OWN     4   //程序提示#/
#define ALARM_N7      7   //电源#/
#define ALARM_N9      9   //血泵1#/
#define ALARM_N10     10  //血泵2#/
#define ALARM_N15     15  //肝素泵#/
#define ALARM_SELFCHECK 16   //自检#/


//报警处理类型#/
#define DE_KEEP  1      //触发保持#/
#define DE_RESTORE 2    //自恢复#/
#define DE_ONCE  3      //一次性，确认后无#/

//提示框处理类型#/
#define DE_NONE  0              //无#/
#define DE_MUSTCONFIRM 1        //跳出提示，需确认#/
#define DE_DELAYDISAPPEAR 2    //会延时消失#/
#define DE_NOPOP 3              //不跳出提示#/
#define DE_LINKAGE 4            //提示联动报警提示#/

//指示历史报警页面是否显示#/
#define DE_ALARMWIDGETHIDDEN  0
#define DE_CURRENTALARMSHOW   1
#define DE_HISTORYALARMSHOW   2
#define DE_CURRENTALARMHIDDEN 3
#define DE_HISTORYALARMHIDDEN 4


//指示当前的报警工作状态#/
#define DE_NORMAL_FLAG  0  //正常#/
#define DE_RESET_FLAG 1   //重启#/
#define DE_MUTE_FLAG 2  //静音#/
#define DE_HIDDED_FLAG 3  //屏蔽报警提示#/

//历史报警记录最大的条数
#define DE_HISTORYALARM_MAXNUM 2000


//
struct StrAlarm
{
    int code;    //报警处理编码#/
    char pri;    //优先级#/
    char who;    //来自哪里#/
};

//报警信息结构#/
struct AlarmInfor
{
    int alarmid;   //报警条目编码#/
    int code;      //报警处理编码#/
    char pri;
    char who;
    unsigned int time;      //时间戳#/
    char dispose;  //是否确认  0：没有  1：确认#/
};

/*----------------------全局变量区域---------------------------*/

//告警编码表#/
extern StrAlarm alarm_table[25];

extern QMap<int,StrAlarm> curAlarmMap;//当前报警信息列表#/
extern QMap<int,StrAlarm> preAlarmMap;//上一次报警信息列表#/

extern QMap<int,AlarmInfor> curtimeAlarmMap;  //包含发生时间的当前报警保存区#/
extern QMap<int,AlarmInfor> historyAlarmMap;  //历史报警信息保存区,它的KEY值和上面的不同，只是一个顺序ID号#/

extern int historyAlarmID;     //记录历史告警记录条数，并且作为其索引#/

extern QList<int> onceAlarmList;  //一次性报警标示区#/

extern int alarmWidgetShow;   //指示历史报警页面是否显示 0：不显示  1：当前报警  2：历史报警#/

//通知右上角的图标按钮#/
extern unsigned int Alarmcolorflag;
extern bool Alarm_Reset_Finish;   //报警复位操作是否完成标志 true 完成 //20151022 修改#/
extern bool Alarm_BloodPump_Flag;  //发生停血泵报警标志#/
extern bool Alarm_UFPump_Flag;//发生停超滤泵报警标志#/
extern bool Alarm_SubsPump_Flag;//发生停置换泵报警标志#/
extern bool Alarm_HeparinPump_Flag;//发生停肝素泵报警标志#/


/*----------------------函数接口区域---------------------------*/

//根据报警条目ID，查找报警内容#/
void Search_alarminfo(int alarmid,QString &title,QString &info);
//根据报警来源ID，返回报警来源信息#/
QString Search_sourceinfo(int who);
//确认并且保存#/
void Confirm_to_histroy(AlarmInfor alarm);
//设置报警页面是否显示#/
void Set_AlarmWidgetShow(int status);

/*----------------------报警处理类区域---------------------------*/

class AlarmModule : public QObject,public ActionCustom
{
    Q_OBJECT

public:
    AlarmModule(QObject *parent=0);
    void Alarm_Handle();   //进行报警处理#/
    void Get_AlarmCode();  //根据各节点设备状态计算生成报警编码#/

    void sound(char para);  //报警声音处理#/
    void light(char para);
    void action(char para);
    void Set_WorkState(int state);    //设置报警模块的工作状态#/
    void Reset_AlarmHandle();    //复位报警操作#/

    QMap<int,AlarmInfor> Get_curtimeAlarmMap();
    QMap<int,AlarmInfor> Get_historyALarmMap();
    void Afresh_Alarm();
    int Get_CurrentAlarmNum();
    bool Is_CurrentAlarm(int id);  //判断指定报警ID是否当前报警#/
    void Custom_Prompt(int pid);   //自定义的提示信息功能,按复位键可以消除#/    
    void Custom_Prompt_Clear(int pid); //清除指定自定义的提示信息#/

#if HJX_COMMUNICATION_SWITCH
signals:
    void signalSDO(int id);
#endif
public slots:
    void timerHandle();

private:
    void ActionHandle(const void *dataPtr=NULL, int name=0); //相应数据包变量变化时的通知接口#/
    int Check_N1_status();   //获取当前主状态，报警判断条件#/
    int Check_N1_sub_status(); //获取子状态，报警判断条件#/
    int Check_N1_blood_status();
    int Check_N1_water_status();
    int Check_alarm_type(StrAlarm type);   //判断报警处理类型#/
    int Check_Promptbox_type(int code);   //判断提示框处理类型#/
    int Save_to_histroy(AlarmInfor alarm);

private:
    PacketBloodPump alarm_N9;   //血泵1#/
    PacketBloodPump alarm_N10; //血泵2#/
    PacketStatusNote note_N3;  //提示性报警，主控板#/
    PacketStatusAlarm alarm_N3; //主控板#/
    PacketStatusAlarm alarm_N5; //监控板#/
    PacketOwnNote alarm_own;   //本机的提示告警信息,由程序生成#/
    PacketOwnNote Prompt_own;  //通过报警模块实现的提示功能#/
    PackN15PDO alarm_N15;  //肝素泵#/
    PacketPower alarm_N7; //电源#/

    QTimer alarmtimer;
    int stateflag;
};

#endif
