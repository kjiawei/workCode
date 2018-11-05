
//原 Alarm_module.h
/*****************************************************
** 作者： Huangjunxian
** 创始时间：2014-1-8
** 修改人：Huangjunxian
** 修改时间：2014-1-8
** 描述：
** 5058报警模块 《头文件》
*****************************************************/

#ifndef ALARM_MODULE_H
#define ALARM_MODULE_H

#include <QtGui>
#include "data_custom.h"

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

//水路报警条目
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

//提示性报警条目
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

//自定义提示性报警，复位键可以消除
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

//自检状态下的报警条目
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

//主工作状态
#define DE_NOALARM 0 //不需要报警状态#/
#define DE_NORMAL 1 //正常状态#/
#define DE_CLEAN 2  //消毒#/
#define DE_DRAIN 3 //排水#/

//工作子状态--血路
#define DE_NOBLOODHAVEFLUID 1  //预冲无血有液前，即自检之后#/
#define DE_OTHERSTATUS1 2      //上面状态之外的状态#/
#define DE_BEFOREBLOOD  3      //预冲有血前，开始预冲后#/
#define DE_OTHERSTATUS2 4      //上面状态之外的状态#/
#define DE_RETURNNOBLOOD 5     //回血，无血后#/

//模式
#define  DE_ONLINE 1          //HDFonline HFonline时#/
#define  DE_ISOUFHP 2         //ISO UF  PE/HP#/
#define  DE_NOTONLINE 3
#define  DE_ONLYHEATWATER 4 //纯热水消毒#/

//水路
#define DE_WATEROPEN 1        //水路启动时#/
#define DE_CLEARDRAIN 2        //清洁后排空#/
#define DE_WATEROFF  3        //水路不启动时#/
#define DE_LOOPDRAIN 4       //消毒循环#/

//报警来源
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

//报警处理类型
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

//指示当前的报警工作状态
#define DE_NORMAL_FLAG  0  //正常#/
#define DE_RESET_FLAG 1   //重启#/
#define DE_MUTE_FLAG 2  //静音#/
#define DE_HIDDED_FLAG 3  //屏蔽报警提示#/
#define DE_HISTORYALARM_MAXNUM 2000//历史报警记录最大的条数

/*
//监控板报警 血路报警条目
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

struct StrAlarm //报警字符串
{
    int code;    //报警处理编码
    char pri;    //报警优先级
    char who;    //报警来源
};

struct AlarmInfor//报警信息
{
    int alarmid;   //报警条目编码
    int code;      //报警处理编码
    char pri;
    char who;
    unsigned int time;      //时间戳
    char dispose;  //是否确认  0：没有  1：确认
};

//全局变量区域 告警编码表
extern StrAlarm alarm_table[25];
extern QMap<int,StrAlarm> curAlarmMap;//当前报警信息列表
extern QMap<int,StrAlarm> preAlarmMap;//上一次报警信息列表
extern QMap<int,AlarmInfor> curtimeAlarmMap;  //包含发生时间的当前报警保存区#/
extern QMap<int,AlarmInfor> historyAlarmMap;  //历史报警信息保存区,它的KEY值和上面的不同，只是一个顺序ID号#/
extern int historyAlarmID;     //记录历史告警记录条数，并且作为其索引#/
extern QList<int> onceAlarmList;  //一次性报警标示区#/
extern int alarmWidgetShow;   //指示历史报警页面是否显示 0：不显示  1：当前报警  2：历史报警#/

//通知右上角的图标按钮
extern unsigned int Alarmcolorflag;
extern bool Alarm_Reset_Finish;   //报警复位操作是否完成标志 true 完成 //20151022 修改#/
extern bool Alarm_BloodPump_Flag;  //发生停血泵报警标志#/
extern bool Alarm_UFPump_Flag;//发生停超滤泵报警标志#/
extern bool Alarm_SubsPump_Flag;//发生停置换泵报警标志#/
extern bool Alarm_HeparinPump_Flag;//发生停肝素泵报警标志#/

void Search_alarminfo(int alarmid,QString &title,QString &info);//根据报警条目ID查找报警内容
QString Search_sourceinfo(int who);//根据报警来源ID返回报警来源信息
void Confirm_to_histroy(AlarmInfor alarm);//确认并且保存
void Set_AlarmWidgetShow(int status);//设置报警页面是否显示

//报警处理类区域
class AlarmModule : public QObject,public ActionCustom
{
    Q_OBJECT

public:
    AlarmModule(QObject *parent=0);
    void Alarm_Handle();   //报警处理
    void Get_AlarmCode();  //根据各节点设备状态计算生成报警编码
    void sound(char para);  //报警声音处理
    void light(char para);
    void action(char para);
    void Set_WorkState(int state); //设置报警模块的工作状态
    void Reset_AlarmHandle();    //复位报警操作
    QMap<int,AlarmInfor> Get_curtimeAlarmMap();
    QMap<int,AlarmInfor> Get_historyALarmMap();
    void Afresh_Alarm();
    int Get_CurrentAlarmNum();
    bool Is_CurrentAlarm(int id);  //判断指定报警ID是否当前报警
    void Custom_Prompt(int pid);   //自定义的提示信息功能,按复位键可以消除   
    void Custom_Prompt_Clear(int pid); //清除指定自定义的提示信息

#if HJX_COMMUNICATION_SWITCH
signals:
    void signalSDO(int id);
#endif
public slots:
    void timerHandle();

private:
    void ActionHandle(const void *dataPtr=NULL, int name=0); //相应数据包变量变化时的通知接口
    int Check_N1_status();   //获取当前主状态报警判断条件
    int Check_N1_sub_status(); //获取子状态报警判断条件
    int Check_N1_blood_status();
    int Check_N1_water_status();
    int Check_alarm_type(StrAlarm type);  //判断报警处理类型
    int Check_Promptbox_type(int code);   //判断提示框处理类型
    int Save_to_histroy(AlarmInfor alarm);

private:
    PacketBloodPump alarm_N9;   //血泵1
    PacketBloodPump alarm_N10; //血泵2
    PacketStatusNote note_N3;  //提示性报警，主控板
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






//原packet_can.h文件
#ifndef PACKET_CAN_H
#define PACKET_CAN_H

//本文件是具体和底层设备canopen通信的数据结构内容

//所有结构变量均一一对应协议文件中底层设备中的变量
//详情参考 /pulic/protocols/中的协议文件
//N1：表示发送该PDO的设备号为1
//01H：对应第一个字节的值，即协议文件中的BYTE0的值
//PDO数据结构为 8个字节

#ifdef __cplusplus
extern "C" {
#endif

/*  Function Codes  */
//canopen把canbus数据包的前11位标示符称通讯对象ID（COB-ID）
//COB-ID=4位功能码+7位节点ID
//下面就是各通讯对象的功能码
#define EVENT_NMT           0x0
#define EVENT_SYNC          0x1
#define EVENT_TIME_STAMP    0x2
#define EVENT_PDO1tx        0x3
#define EVENT_PDO1rx        0x4
#define EVENT_PDO2tx        0x5
#define EVENT_PDO2rx        0x6
#define EVENT_PDO3tx        0x7
#define EVENT_PDO3rx        0x8
#define EVENT_PDO4tx        0x9
#define EVENT_PDO4rx        0xA
#define EVENT_SDOtx         0xB    //人机板对其他节点发送SDO命令后，节点应答的SDO，如主控板的0X583#/
#define EVENT_SDOrx         0xC    //人机板接收到其他节点的SDO命令，如主控板的0x603#/
#define EVENT_NODE_GUARD    0xE
#define EVENT_LSS           0xF
#define EVENT_NULL          0x7F

#define CompareAssignFlage(x, y, f)     do{if(x!=y){x=y;f=0;}}while(0)

/*
 * Master Ctrl struct
 */

//
struct PacketStatusNote{
    unsigned char dataType;

    struct PacketStatus{
        unsigned char header_status; //总状态
        unsigned char blood_status; //血路状态
        unsigned char water_status; //水路状态
    }__attribute__ ((__packed__))status;

    struct PacketNote{   //对应主控板的PDO0的4个提示信息字节
        unsigned char note1;
        unsigned char note2;
        unsigned char note3;
        unsigned char note4;
    }__attribute__ ((__packed__))note;
}__attribute__ ((__packed__));//这个添加的字段是告诉编译器，取消编译过程中的优化对齐，按照
//实际占用字节数进行对齐。

struct PacketOwnNote{//本机的提示信息
    unsigned char dataType;
    struct OwnNote{
        unsigned char note1;   //1:护士呼#/
        unsigned char note2;
        unsigned char note3;
        unsigned char note4;    //HJX程序提示信息  bit0:1 单次定时报警 bit1：1 循环定时报警 #/
        unsigned char note5;    //HJX程序提示信息  bit0:1 收缩压范围报警 bit1：1舒张压范围报警 bit2：1脉搏范围报警#/
        unsigned char note6;    //HJX程序提示信息  bit0:N3通讯失败 bit1：N5通讯失败 2：N9 3：N10 4：N15 5：N7 #/
        unsigned char note7;    //HJX程序提示信息  bit0:1 水路工作异常,bit1:1主控板手动预冲状态错误，bit2：自动预冲 bit3：在线预冲 bit4：引血 bit5：在线引血 bit6：回血 bit7：在线回血#/
    }__attribute__ ((__packed__))note;
}__attribute__ ((__packed__));

struct PacketStatusAlarm{
        unsigned char dataType;
        struct PacketAlarm{   //对应主控板和监控板的PDO1的几个字节
            unsigned char errorstate1;
            unsigned char errorstate2;
            unsigned char errorstate3;
            unsigned char errorstate4;
            unsigned char errorstate5;
            unsigned char Iostatus;
            unsigned char errorstate7;
          }__attribute__ ((__packed__))alarm;
}__attribute__ ((__packed__));

struct PacketValve{
    unsigned char dataType;
    unsigned int valve32;
    unsigned char u502a;
    unsigned char u502b;
    unsigned char u503b;
}__attribute__ ((__packed__));

struct PacketPdoErr{
    unsigned char dataType;
    unsigned char byte1;
    unsigned char byte2;
    unsigned char byte3;
    unsigned char byte4;
    unsigned char byte5;
    unsigned char byte6;
    unsigned char byte7;
}__attribute__ ((__packed__));

struct PacketOutlet{
    unsigned char dataType;
    unsigned char N3_P2_Power;
    unsigned short int N3_out_temp;
    unsigned short int N3_out_cond;
    unsigned short int N3_WP;//水路压
}__attribute__ ((__packed__));
struct PacketFluidB{
    unsigned char dataType;
    unsigned char N3_P3_Power;
    unsigned short int N3_B_temp;
    unsigned short int N3_B_cond;
    unsigned short int N3_B_Target;
}__attribute__ ((__packed__));
struct PacketMix{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N3_Mix_temp;
    unsigned short int N3_Mix_cond;
    unsigned short int N3_mix_Target;
}__attribute__ ((__packed__));

struct PacketFluidAfter{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N3_after_temp;
    unsigned short int N3_after_cond;
    unsigned short int noUse1;
}__attribute__ ((__packed__));

struct PacketUfValue{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N3_UF_amount;  //已超虑量
    unsigned short int N3_UF_target;  //超虑目标
    unsigned short int N3_UF_rate;//超滤速度
}__attribute__ ((__packed__));

struct PacketUfPump{
    unsigned char dataType;
    unsigned char N3_P1_Power;
    unsigned short int N3_Degass_P;
    unsigned short int N3_UFP_Rate;
    unsigned short int N3_UFP_Feedback;
}__attribute__ ((__packed__));
struct PacketHeat{
    unsigned char dataType;
    unsigned char N3_heat_Power;
    unsigned short int N3_heat_temp;
    unsigned short int N3_heat_Target;
    unsigned short int noUse;
}__attribute__ ((__packed__));

struct PacketBPump{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N3_BP_Stand;
    unsigned short int N3_BP_Now;
    unsigned short int N3_MixP_Stand;
}__attribute__ ((__packed__));

//struct PacketSubstitutionPump{
//    unsigned char dataType;
//    unsigned char noUse;
//    unsigned short int N3_BP_Stand;
//    unsigned short int N3_BP_Now;
//    unsigned short int N3_MixP_Stand;
//}__attribute__ ((__packed__));

struct PacketMixPump{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N3_MixP_Now;
    unsigned short int N3_balance_T;
    unsigned short int noUse1;
}__attribute__ ((__packed__));
struct PacketDisplay{
    unsigned char dataType;
    unsigned char N3_pos;
    unsigned char N3_display_letter[4];
    unsigned short int N3_display_data;
}__attribute__ ((__packed__));
struct PacketAntrum{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N3_A100;
    unsigned short int N3_B100;
    unsigned short int N3_C1000;
}__attribute__ ((__packed__));
struct PacketNowater{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N3_Nowater_T;
    unsigned short int noUse1;
    unsigned short int noUse2;
}__attribute__ ((__packed__));
struct PacketMasterCtrlSdo{
    unsigned char noUse;
    unsigned short int index;
    unsigned char indexSub;
    unsigned char dataSdo[4];
}__attribute__ ((__packed__));

struct PacketSdoSetup {
    unsigned char N3_set[4];
};
struct PacketSdoCmd {
    unsigned char cmd;
};

struct PacketSdoScale {
    unsigned short int N3_set_Bscale;
    unsigned short int N3_set_Wscale;
};
struct PacketSdoAim {
    unsigned short int N3_set_B;
    unsigned short int N3_set_mix;
};

struct PacketSdoBlood {
    unsigned short int N3_set_BPspeed;
    unsigned char N3_set_BPtube;
    unsigned char noUsed;
};
struct PacketSdoHeparin {
    unsigned short int N3_set_heparin_first;
    unsigned short int N3_set_heparin_speed;
};
struct PacketSdoRangeTemp {
    unsigned short int N3_set_temp_Up;
    unsigned short int N3_set_temp_Lower;
};

struct PacketPDOHeparinPump {
    unsigned short dose;
    unsigned short speed;
    unsigned short preStopTime;
};

/*
 * Monitor struct
 */
struct PacketWaterOut {
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N5_out_temp;//
    unsigned short int N5_out_cond;
    unsigned short int N5_leak;
};
struct PacketPress {
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N5_AP;
    unsigned short int N5_VP;
    unsigned short int N5_TMP;
};
struct PacketBloodState {
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N5_WP;
    unsigned short int N5_Blood_temp;
    unsigned short int N5_Oxygen;
};
struct PacketBPMState {
    unsigned char dataType;
    unsigned char N5_BPM_status;
    unsigned short int N5_BPM_SYS;
    unsigned short int N5_BPM_DIA;
    unsigned short int N5_BPM_MAP;
};
struct PacketBPMMisc {
    unsigned char dataType;
    unsigned char N5_BPM_unit;
    unsigned short int N5_BPM_pulse;
    unsigned short int N5_BPM_inflation;
    unsigned short int N5_Oxi_pulse;
};


/*
 * Power struct
 */

//struct PacketPower {
//    unsigned int canSt;
//    short adc0;
//    short adc1;
//    short adc2;
//    short adc3;
//    char state;
//    char batH;
//    char batL;
//    char charge;
//};

struct PacketBattry{
    unsigned char dataType;
    unsigned char notuse1;
    unsigned char BAT_state1;
    unsigned char BAT_state2;
    unsigned short int adc0;
    unsigned short int adc1;
  }__attribute__ ((__packed__));

struct PacketPower{
    unsigned char dataType;
    unsigned char notuse1;
    unsigned char powerstate1; //电量情况，是否充足
    unsigned char powerstate2; //充电情况，是否完成
    unsigned short int adc2; //正极电压采样值
    unsigned short int adc3; //负极电压采样值
  }__attribute__ ((__packed__));

struct PacketPowerAlarm{
    unsigned char dataType;
    unsigned char notuse1;
    unsigned char error;
    unsigned char notuse2;
    unsigned char notuse3;
    unsigned char notuse4;
    unsigned char notuse5;
    unsigned char notuse6;
  }__attribute__ ((__packed__));

struct PacketPowerMornitor{
    unsigned char dataType;
    unsigned char notuse1;
    unsigned char notuse2;
    unsigned char notuse3;
    unsigned char notuse4;
    unsigned char notuse5;
    unsigned char notuse6;
    unsigned char notuse7;
  }__attribute__ ((__packed__));

//struct PacketPower{

//        struct Packet1{
//            unsigned char dataType;
//            unsigned char notuse1;
//            unsigned char N7_BAT_state1;
//            unsigned char N7_BAT_state2;
//            unsigned short int N7_adc0;
//            unsigned short int N7_adc1;
//          }__attribute__ ((__packed__))packet1;

//        struct Packet2{
//            unsigned char dataType;
//            unsigned char notuse1;
//            unsigned char N7_state1;
//            unsigned char N7_state2;
//            unsigned short int N7_adc2;
//            unsigned short int N7_adc3;
//          }__attribute__ ((__packed__))packet2;

//        struct Packet3{
//            unsigned char dataType;
//            unsigned char notuse1;
//            unsigned char N7_error;
//            unsigned char notuse2;
//            unsigned char notuse3;
//            unsigned char notuse4;
//            unsigned char notuse5;
//            unsigned char notuse6;
//          }__attribute__ ((__packed__))packet3;

//        struct Packet4{
//            unsigned char dataType;
//            unsigned char notuse1;
//            unsigned char notuse2;
//            unsigned char notuse3;
//            unsigned char notuse4;
//            unsigned char notuse5;
//            unsigned char notuse6;
//            unsigned char notuse7;
//          }__attribute__ ((__packed__))packet4;
//}__attribute__ ((__packed__));

struct PacketHeparinPump {//肝素泵参数
    unsigned int canSt;
    unsigned short int inputAllValue;
    unsigned short int doseSpeed;
    unsigned char workMode;
    unsigned char workModeSub;
    unsigned char senserState;

    int quickSpeed;
    int syringeLen;
    int syringeStyle;
    int doseFirst;
    char selstatus;    //是否选中，1 yes 0 no
    char runstatus;    // 1 run 0 stop
};

struct PacketBloodPump {   //血泵参数 //为了累计量超过65535的限制定义的类型
    unsigned char dataType;
//    unsigned char reserve1;
    unsigned short int N9_speed;
//    unsigned short int N9_amount;//血泵累计量
    unsigned int N9_amount;//血泵累计量
    unsigned char N9_mode;
    unsigned char N9_status;//其他各个状态
};

struct PacketN9N10Pump {  //对应N9N10血泵发上来PDO的类型#/
    unsigned char dataType;
    unsigned char reserve1;
    unsigned short int N9_speed;
    unsigned short int N9_amount;//血泵累计量
    unsigned char N9_mode;
    unsigned char N9_status;//其他各个状态
};

struct PacketBloodPumpTube {
    unsigned char dataType;
    unsigned char N9_set_tube;
    unsigned short int N9_set_speed;
    unsigned char NoUse4;
    unsigned char NoUse5;
    unsigned char NoUse6;
    unsigned char N9_Key;
};

struct PacketSubBloodPump {
    unsigned short int N10_set_speed;
    unsigned short int N10_goal;
    unsigned short int N10_amount;
    unsigned char N10_mode;
    unsigned char N10_status;
};

struct PacketN1State {
unsigned char N1_status;
unsigned char N1_blood_status;
unsigned char N1_sub_status;
unsigned char N1_water_status;
unsigned char N1_sub2_status;
unsigned char N1_subbit1_status;
unsigned char N1_subbit2_status;
};

//人机板发命令给其它板
struct PackN1PDO01H
{
    unsigned char dataType;
    struct N1SetByte1
    {
        unsigned char SN_B :1;          //0,双针;1,单针
        unsigned char B_powder_B :1;    //0,非干粉;1,干粉
        unsigned char U_filter_B :1;    //0,非超净;1,超净
        unsigned char online_B :1;      //0,非online;1,online
        unsigned char KTV_B :1;         //0,非ktv;1,带ktv
        unsigned char Back_in_B :1;     //0,前置换;1,后置换
        unsigned char Acetate_B :1;     //0,非     1,醋酸盐
        unsigned char A_center_B :1;    //0,非    1,中央供A液
    }byte1;

    struct N1SetByte2
    {
        unsigned char pe :1;  //0,   1,B液中含Nacl
        unsigned char notuse1 :1;   //0,    1,热冲
        unsigned char notuse2 :1;  //0,  1,化(过氧乙酸)
        unsigned char notuse3 :1;    //0,  1,化(次氯酸钠)
        unsigned char notuse4:1;     //0,  1,化(柠檬酸)
        unsigned char notuse5 :1;         //0,  1,热(纯水)
        unsigned char notuse6 :1;    //0,  1,热(柠檬酸)
        unsigned char isouf :1;   //0,  1,ISO单超
    }byte2;

    struct N1SetByte3
    {
        unsigned char status_screen_B :1;   //0,非 1,状态界面
        unsigned char engeer_screen_B :1;   //0,非 1,工程界面
        unsigned char no_water_B :1;        //0,有水 1,无水
        unsigned char save_AB_B :1;         //0,非  1,省液
        unsigned char auto_limit_B :1;      //0,手动 1,自动卡标
        unsigned char SN_type_B :1;         //0,单针单泵 1,单针双泵
        unsigned char skip_test_B :1;       //0,自检 1,跳过自检
        unsigned char b7 :1;
    }byte3;

    struct N1SetByte4
    {
        unsigned char Function_Bpowder_B :1;   //0,1,带干粉功能
        unsigned char b1 :1;
        unsigned char b2 :1;
        unsigned char b3 :1;
        unsigned char b4 :1;
        unsigned char b5 :1;
        unsigned char b6 :1;
        unsigned char b7 :1;
    }byte4;

    unsigned char noUse[3];
};

struct PackN1PDO02H
{
    unsigned char dataType;
    unsigned char N1_set_BPtube;
    unsigned short int N1_set_temp;
    unsigned short int N1_set_Wspeed;
    unsigned short int N1_set_BPspeed;
}__attribute__ ((__packed__));

struct PackN1PDO03H
{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N1_set_Bscale;
    unsigned short int N1_set_Wscale;
    unsigned short int N1_set_B;
}__attribute__ ((__packed__));

struct PackN1PDO04H
{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N1_set_mix;
    unsigned short int N1_set_UFspeed;
    unsigned short int N1_set_UFtarget;
}__attribute__ ((__packed__));

struct PackN1PDO05H
{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N1_set_heparin_first;
    unsigned short int N1_set_heparin_speed;
    unsigned short int N1_set_temp_UP;
}__attribute__ ((__packed__));

struct PackN1PDO06H
{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N1_set_temp_DOWN;
    unsigned short int N1_set_AP_UP;
    unsigned short int N1_set_AP_DOWN;
}__attribute__ ((__packed__));

struct PackN1PDO07H
{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N1_set_VP_UP;
    unsigned short int N1_set_VP_DOWN;
    unsigned short int N1_set_TMP_UP;
}__attribute__ ((__packed__));

struct PackN1PDO08H
{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N1_set_TMP_DOWN;
    unsigned short int N1_set_Cond_UP;
    unsigned short int N1_set_Cond_DOWN;
}__attribute__ ((__packed__));

struct PackN1PDO09H
{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N1_set_QJspeed;
    unsigned short int N1_set_QJcond_UP;
    unsigned short int N1_set_QJcond_DOWN;
}__attribute__ ((__packed__));

struct PackN1PDO0AH
{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N1_set_QJtemp;//两字节
    unsigned short int N1_set_QJsuck;
    unsigned short int noUse1;
}__attribute__ ((__packed__));

struct PackN1PDO0BH
{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N1_set_SubBPspeed;//两字节
    unsigned short int noUse1;
    unsigned short int noUse2;
}__attribute__ ((__packed__));

struct PackN1PDO0CH   //人机板PDO2对应的字节
{
    unsigned char dataType;
//    unsigned char N1_status;
//    unsigned char N1_blood_status;
//    unsigned char N1_sub_status;
//    unsigned char N1_water_status;
//    unsigned char N1_sub2_status;
//    unsigned char N1_subbit1_status;
//    unsigned char N1_subbit2_status;
  struct PacketN1State state;
}__attribute__ ((__packed__));//对齐防优化


struct PackN15PDO
{
    //unsigned char dataType;
    unsigned short int inputAllValue;  //已注入总量
    unsigned short int doseSpeed;  //正常速度
    unsigned char workMode; //工作状态  01H:启动   02H:停止  03H:暂停  04H:自检  06
    unsigned char workModeSub; //子工作状态 具体看协议
    unsigned char senserState;  //限位开关状态
    unsigned char notuse;   //没定义

}__attribute__ ((__packed__));

struct bpmResult{
    unsigned short int systolic;   //心脏收缩压#/
    unsigned short int diastolic;  //心脏舒张压#/
    unsigned short int pulse;   //心跳#/
};

#ifdef __cplusplus
};
#endif

#endif







//原packet.h文件
#ifndef PACKET_H
#define PACKET_H

#include "packet_ui.h"
#include "packet_can.h"

//Master Ctrl 主控板相关
#define PACKET_STATUS_NOTE 10 //"Master Ctrl Status Note" 本机状态赋值canopen的通信结构
#define PACKET_MASTERCTRL_VALVE 11         //"Master Ctrl Valve"
#define PACKET_MASTERCTRL_OUTLET 12        //"Master Ctrl Outlet"
#define PACKET_MASTERCTRL_MIX 13           //"Master Ctrl Mix"
#define PACKET_MASTERCTRL_FLUIDB 14        //"Master Ctrl FluidB"
#define PACKET_MASTERCTRL_FLUIDAFTER 15    //"Master Ctrl FluidAfter"
#define PACKET_MASTERCTRL_UFVALUE 16       //"Master Ctrl UfValue"
#define PACKET_MASTERCTRL_UFPUMP 17        //"Master Ctrl UfPump"
#define PACKET_MASTERCTRL_HEAT 18          //"Master Ctrl Heat"
#define PACKET_MASTERCTRL_BPUMP 19         //"Master Ctrl BPump"
#define PACKET_MASTERCTRL_MIXPUMP 20       //"Master Ctrl MixPump"
#define PACKET_MASTERCTRL_DISPLAY 21       //"Master Ctrl Display"
#define PACKET_MASTERCTRL_DISPLAY_TABLE 22 //"Master Ctrl Display table"
#define PACKET_MASTERCTRL_SDO 23           //"Master Ctrl SDO"
#define PACKET_MASTERCTRL_PDOERR 24        //"Master PDO ERR"

//血泵管径和速度 （canopen操作）
#define PACKET_SDO_BLOOD 25     //"SDO Blood"
#define PACKET_SDO_HEPARIN 26   //"SDO Heparin"
#define PACKET_SDO_RANGETEMP 27 //"SDO RangeTemp"

//Monitor 监控板相关
#define PACKET_MONITOR_WATER 28    //"Monitor WaterOut"
#define PACKET_MONITOR_PRESS 29    //"Monitor Press"
#define PACKET_MONITOR_BLOODST 30  //"Monitor BloodState"
#define PACKET_MONITOR_BPMST 31    //"Monitor BPMState"
#define PACKET_MONITOR_BPMMISC 32  //"Monitor BPMMisc"
#define PACKET_STATUS_MASTALARM 33 //"Master Ctrl Status Alarm"  //主控板报警PDO1
#define PACKET_STATUS_ALARM 34     //"Monitor Ctrl Status Alarm"//监控板板报警PDO1
#define PACKET_RESUME_ALARM 35     //"Resume Alarm"    //复位报警标记 可remove
#define PACKET_BPM_PARAM 36        //"Monitor Ctrl Status Alarm"

//Miscellaneous 其余相关
#define PACKET_NAME_BLOODPUMP 37     //"Blood Pump"
#define PACKET_NAME_BLOODPUMPTUBE 38 //"Blood Pump Tube"
#define PACKET_NAME_SUBBLOODPUMP 39     //"SUB Pump"
#define PACKET_NAME_SUBBLOODPUMPTUBE 40 //"SUB Pump Tube"

//血泵2控制（canopen操作）
#define PACKET_PDO_SUBBLOODPUMP 41 //"SubBlood Pump"
#define PACKET_NAME_HEPARINPUMP 42 //"Heparin Pump"
#define PACKET_NAME_HEPARINDATA 43 //"Heparin Pump data"
#define PACKET_NAME_SELFTEST 44 //"Self-test"

//全部工作模式和状态（canopen操作）
#define PACKET_NAME_WORKMODE 45    //"Work Mode" 工作模式
#define PACKET_NAME_SCENESWITCH 46 //"SceneSwitch"
#define PACKET_NAME_WORKSTATE 47   //"Workstate" 各项工作状态

//动脉压上下限和血温（canopen操作）
#define PACKET_NAME_AP 48        //"PressureAP"
#define PACKET_NAME_SCALE_AP 49  //   "ScaleAP"
#define PACKET_NAME_UPDATE_AP 50 //   "UpdateAP"
#define PACKET_NAME_VP 51        //"PressureVP"
#define PACKET_NAME_SCALE_VP 52  //   "ScaleVP"
#define PACKET_NAME_UPDATE_VP 53 //     "UpdateVP"

//跨膜压上下限（canopen操作）
#define PACKET_NAME_TMP 54        //"PressureTMP"
#define PACKET_NAME_SCALE_TMP 55  //    "ScaleTMP"
#define PACKET_NAME_UPDATE_TMP 56 //     "UpdateTMP"

//电导率上下限（canopen操作）
#define PACKET_NAME_COND 57          //"COND"
#define PACKET_NAME_UF 58            //"UF"
#define PACKET_NAME_NOW_UF_VOLUME 59 //"Now UF volume"
#define PACKET_NAME_POWER 60          //"Power"   //对应电源板的PDO
#define PACKET1_NAME_BATTERY 61       //"battery"
#define PACKET2_NAME_POWERALARM 62    //"Poweralram"
#define PACKET3_NAME_POWERMORNITOR 63 //"Powermornitor"
#define PACKET_NAEME_POWER_WAKEUP 64 //"Power auto wakeup"
#define PACKET_NAME_ADJPRESSLIMIT 65 //"Adjpresslimit"

//透析夜、血液、肝素的温度和流速（canopen操作）
#define PACKET_NAME_DIALYSATE 66   //"Dialysate"
#define PACKET_PREFLUSH 67         //"Preflush"
#define PACKET_NAME_N1STATE 68     //"n1 state"
#define PACKET_NAME_BLOODVOLUME 69 // "Blood volume"
#define PACKET_NAME_BLOODTEMP 70 //"Blood temp"
#define PACKET_BMPRESULT 71 //"bmp result"
#define BLOOD_CHANGE_PREFLUSH 72 //"blood_change_preflush"

//清洁相关控制（canopen操作）
#define Disinfect_Work_Model 73 //"DisinfectWorkModel"
//肝素控制（canopen操作）
#define PACKET_PDO_HEPARINPUMP 74 //"SEND_PDO_HEPARINPUMP"

//A B液混合透析液的控制参数（canopen操作）
#define PACKET_FORMULA_TARGET 75 //"formula_out_target"

//Dialog packet
#define PACKET_NAME_MESSAGES 76 //"Msg_History"

//报警闪动按钮
#define PACKET_NAME_BTNALARM 77 //"btn_alarm"

//"maintain_return"
#define PACKET_MAINTAIN_RETURN 78 

//工程模式数据变化
#define PACKET_MAINTAIN_PARAMS 79 //"maintain_params"
#define PACKET_PARAMS_UF 80       //"params_uf"
#define PACKET_ENGINEER 81        //"engineer"
#define PACKET_NA_CURVE_VALUE 82  //"na_curve_value"
#define PACKET_CURVE_UF_11TO30 83 //"curve_uf_11to30"
#define PACKET_BP_WIDTH 84        //"BP_Width_params"

//是否血路ok
#define PACKET_BLOODROAD_OK 85 //"blood_road_ok"

//是否水路ok
#define PACKET_WATERROAD_OK 86 //"water_road_ok"
#define VEINS_HAVE_BLOOD 87 //"veins_have_blood"

//液位
#define BLOOD_LEVEL_OK 88 //"blood_level_ok"

//预冲方式
#define Preflush_Model 89 //"PreflushModel"
#define SelfCheck_Timer 90 //"SelfCheckTimer" 自检定时器
#define TREAT_TIMER 91 //"treattimer"

//返回准备状态下治疗参数的保存
#define DE_STR_SAVECUREPARAM 92 //"savecureparam"
#define UPLANGUAGEXML 93 //"UpdataLanguageXml"
#define ALARMPRO 94 //"Alarmpro"
#define PACKET_BICARBONATE 95 //"BicarbonateHCO3"
#define OWNNOTE 96 //"ownnote"   //有本机程序生成的提示
#define PACKET_RESETPARAMETERS 97 //"ResetParameters"  //重设参数标志
#define ALARM_SILENCE_SET 98 //"alarmsilence"  //报警静音
#define PACKET_REBIRTH_HANDLE 99 //"rebirth" 重生启动操作处理
#define PACKET_REBIRTH_START 100 //"rebirthstart"  //重生启动开始
#define PACKET_MASTERCTRL_ANTRUM 101 //主控板-腔数量#/
#define PACKET_MASTERCTRL_NOWATER 102 //主控板-无液时间#/

#endif


//原common_tr_string.h文件
/*****************************************************
** 作者： Huangjunxian
** 创始时间：2014-11-26
** 修改人：Huangjunxian
** 修改时间：2014-11-26
** 描述：
** 专门的tr函数宏定义，为了统一字符串翻译 《头文件》
*****************************************************/
#ifndef COMMON_TR_STRING_H
#define COMMON_TR_STRING_H

#include<QLabel>

/*---------------------scene_setup_curve_na----------------------------*/
/*---------------------scene_setup_curve_uf----------------------------*/
/*---------------------scene_setup_nauf--------------------------------*/
/*---------------------scene_setup_sequential--------------------------*/
#define DE_TR_SUMUF  QLabel::tr("Sum UF")
#define DE_TR_UFMODE QLabel::tr("UF mode")
#define DE_TR_CURETIME QLabel::tr("Treatment time")
#define DE_TR_SELECTING QLabel::tr("Selecting")
#define DE_TR_PREDEFINED QLabel::tr("Predefined curve can not modify!")
#define DE_TR_CONDUCTANCE QLabel::tr("Conductivity(Na) Curve")
#define DE_TR_SEQUENTIALUF QLabel::tr("Sequential UF Curve")
#define DE_TR_THESELECT QLabel::tr("The seleceed\nperiod value\n[mmol/L]")
#define DE_TR_THESELECT2 QLabel::tr("The seleceed\nperiod value\n[%]")
#define DE_TR_SETALL QLabel::tr("Set all\nperiod\nvalue")
#define DE_TR_CURRENTCURVE QLabel::tr("Current\ncurve num")
#define DE_TR_NACURVE QLabel::tr("Na+ Profile")
#define DE_TR_UFCURVE QLabel::tr("UF Profile")
#define DE_TR_BROWSECURVE QLabel::tr("Browse\ncurve")
#define DE_TR_TREATMENT QLabel::tr("Treatment time")
#define DE_TR_EACHPERIOD QLabel::tr("Each period")
#define DE_TR_NOTSELECT QLabel::tr("Not selected")
#define DE_TR_MORETHANUF QLabel::tr("More than the UF seted value!")
#define DE_TR_LESSTHANUF QLabel::tr("Less than the UF seted value!")
#define DE_TR_PREVIOUS QLabel::tr("Previous")
#define DE_TR_NEXT QLabel::tr("Next")
#define DE_TR_EDIT QLabel::tr("Edit")
#define DE_TR_SAVE QLabel::tr("Confirm")
#define DE_TR_APPLY QLabel::tr("Apply")
#define DE_TR_RETURN QLabel::tr("Return")
#define DE_TR_DEFINE QLabel::tr("Define")
#define DE_TR_CUSTOM QLabel::tr("Custom")
#define DE_TR_CURVERUNSUCCESS QLabel::tr("This curve data run success!")
#define DE_TR_PROFILECANCEL QLabel::tr("This profile already cancel!")  //本曲线已经取消应用！
#define DE_TR_CURVESAVESUCCESS QLabel::tr("This curve data save success!")
#define DE_TR_CURVERESET QLabel::tr("This curve already reset !")
#define DE_TR_CURVSAVEFAIL QLabel::tr("An error occurred,save fail!")
#define DE_TR_CURVEERROR QLabel::tr("An error occurred,run fail!")
#define DE_TR_PNGFILEERROR QLabel::tr("An error occurred on PNG file!")
#define DE_TR_PROMPT QLabel::tr("Prompt")
#define DE_TR_CURVESAVERUN QLabel::tr("This curve data save and run!")
#define DE_TR_PLEASESELECTCURVE QLabel::tr("Please select a columnar!")
#define DE_TR_PLEASESELECT QLabel::tr("Please select a curve!")
#define DE_TR_AREYOUSURE QLabel::tr("Are you sure to apply?")
#define DE_TR_AREYOUSURECANCEL QLabel::tr("Are you sure to cancel?")   //是否确定取消?
#define DE_TR_AREYOUSAVE QLabel::tr("Are you sure to save?")
#define DE_TR_CANNOTLESS QLabel::tr("Treatment time can not less")
#define DE_TR_THISCURVEISRUNING QLabel::tr("This profile is runing!")
#define DE_TR_THISCURVEISUNDEFINED QLabel::tr("This profile is undefined!")
#define DE_TR_MINTUES QLabel::tr("mintues")
#define DE_TR_YES QLabel::tr("Yes")
#define DE_TR_NO QLabel::tr("No")

#define DE_TR_CONTROL QLabel::tr("Control")  //控制#/
#define DE_TR_VOLTAGE QLabel::tr("Voltage")  //电压#/
#define DE_TR_SETVALUE  QLabel::tr("Set value")  //设置值#/
#define DE_TR_FOREWARD QLabel::tr("Foreward")   //正转#/
#define DE_TR_ROLLBACK QLabel::tr("Rollback") //反转#/
#define DE_TR_CANCEL QLabel::tr("Cancel") //取消#/

//window_boot.cpp
#define DE_TR_PLEASECONFIRM QLabel::tr("Please Confirm")
#define DE_TR_PASSCHECK QLabel::tr("Warning:Do not skip self test to do treatment.")
#define DE_TR_WHETHERACCESSDEBUG QLabel::tr("Prompt:Whether access debug module?")
#define DE_TR_SUPERCLEANTIMESLESS3 QLabel::tr("Super clean filter useable times less 3.")
#define DE_TR_SUPERCLEANTIMESIS0 QLabel::tr("Super clean filter useable times is 0.")

#define DE_TR_DISINFECTSTOP QLabel::tr("Do you want to Stop Cleanning ?")
#define DE_TR_SHOTOFF QLabel::tr("Do you want to ShotDown ?")
#define DE_TR_SELECTDISINFECTTYP QLabel::tr("Please selct disinfect type!")
#define DE_TR_DISINFECTNOWATER QLabel::tr("NO water!")

//scene_main.cpp
#define DE_TR_SELFCHECKTIMEOUT QLabel::tr("Selfcheck timed out failure, please restart!")
#define DE_TR_SELFCHECKERROR QLabel::tr("Selfcheck error, please restart!")
#define DE_TR_BLOODSELFCHECKFAILS QLabel::tr("The blood Selfcheck fails!")
#define DE_TR_WATERSELFCHECKFAILS QLabel::tr("The water Selfcheck fails!")

//widget_switch.cpp
#define DE_TR_CANBETREATED QLabel::tr("Can be treated!")
#define DE_TR_NOOPENHEPARINPUMP QLabel::tr("Didn't open the heparin pump, please open!")
#define DE_TR_WHETHERTORETURNBLOOD QLabel::tr("No blood,whether to continue return blood? 'Yes'-continue,'No'-end!")
#define DE_TR_WHETHERHASENDEDRETURNBLOOD QLabel::tr("Whether has ended return blood? 'Yes'-start drain,'No'-return!")
#define DE_TR_WHETHERTODRAIN QLabel::tr("Whether to continue drain? 'Yes'-continue,'No'-end!")
#define DE_TR_STOPBLOODPUMPALARMISPROCESSED QLabel::tr("Stop blood pump alarm is processed,please open blood pump!")
#define DE_TR_AREYOUSUREPREFLUSH QLabel::tr("Are you sure preflush finish?")
#define DE_TR_TREATMENTFINISH QLabel::tr("Treatment finish,go to blood return!")

#define DE_TR_ONLINEPRIME QLabel::tr("online prime")
#define DE_TR_ONLINECONNECTPT QLabel::tr("online P.t")
#define DE_TR_ONLINERETURNBLOOD QLabel::tr("online RB")

//scene_setup_heparin.c
#define DE_TR_HEPARINISRUNNING QLabel::tr("Heparin is running, please first suspended!")

//widget_notice.cpp
#define DE_TR_START_INIT  QLabel::tr("Initialize")
#define DE_TR_PREPARESTATE QLabel::tr("Preparation")
#define DE_TR_TREATMENTSTATE QLabel::tr("Treatment")
#define DE_TR_FINISHTREATMENTSTATE QLabel::tr("Disconnection")
#define DE_TR_CLEANINGSTATE QLabel::tr("Cleaning")
#define DE_TR_DEBUGSTATE QLabel::tr("Debug")
#define DE_TR_DRAINSTATE QLabel::tr("Drain")

#define DE_TR_SUB_STATE_NULL QLabel::tr("Free state")
#define DE_TR_SELFTEST QLabel::tr("Self checking")
#define DE_TR_SELFTEST_COMPLETE_FLUID QLabel::tr("Self check ok have fluid")
#define DE_TR_SELFTEST_COMPLETE_NO_FLUID QLabel::tr("Self check ok no fluid")
#define DE_TR_PREFLUSH_RUN_FLUID QLabel::tr("Preflush runing have fluid")
#define DE_TR_MANUALPREFLUSH_RUN_FLUID QLabel::tr("Manual preflush have fluid")
#define DE_TR_AUTOPREFLUSH_RUN_FLUID   QLabel::tr("Auto preflush have fluid")
#define DE_TR_ONLINEPREFLUSH_RUN_FLUID QLabel::tr("Online preflush have fluid")

#define DE_TR_PREFLUSH_RUN_NO_FLUID QLabel::tr("Preflush runing no fluid")
#define DE_TR_MANUALPREFLUSH_RUN_NO_FLUID QLabel::tr("Manual preflush no fluid")
#define DE_TR_AUTOPREFLUSH_RUN_NO_FLUID   QLabel::tr("Auto preflush no fluid")
#define DE_TR_ONLINEPREFLUSH_RUN_NO_FLUID QLabel::tr("Online preflush no fluid")

#define DE_TR_PREFLUSH_STOP_FLUID QLabel::tr("Preflush stop have fluid")
#define DE_TR_PREFLUSH_STOP_NO_FLUID QLabel::tr("Preflush stop no fluid")
#define DE_TR_PREFLUSH_COMPLETE_FLUID QLabel::tr("Preflush ok have fluid")
#define DE_TR_PREFLUSH_COMPLETE_NO_FLUID QLabel::tr("Preflush ok no fluid")
#define DE_TR_TREAT_ENABLE_FLUID QLabel::tr("have blood and fluid can treatment")
#define DE_TR_TREAT_ENABLE_NO_FLUID QLabel::tr("have blood no fluid can treatment")
#define DE_TR_GETBLOOD_RUN_NO_FLUID QLabel::tr("connectP.t no fluid")
#define DE_TR_ONLINEGETBLOOD_RUN_NO_FLUID QLabel::tr("Online connectP.t no fluid")
#define DE_TR_GETBLOOD_RUN_FLUID    QLabel::tr("connectP.t have fluid")
#define DE_TR_ONLINEGETBLOOD_RUN_FLUID    QLabel::tr("Online connectP.t have fluid")
#define DE_TR_GETBLOOD_STOP_NO_FLUID   QLabel::tr("connectP.t stop no fluid")
#define DE_TR_GETBLOOD_STOP_FLUID     QLabel::tr("connectP.t stop have fluid")

#define DE_TR_CLEANING_RUN QLabel::tr("Clean runing")
#define DE_TR_CLEANING_STOP QLabel::tr("Clean stop")
#define DE_TR_CLEANING_FINISHED QLabel::tr("Clean finished")

#define DE_TR_CURE_BLOODRETURN_RUN QLabel::tr("Bloodreturn have blood")
#define DE_TR_CURE_ONLINEBLOODRETURN_RUN QLabel::tr("Online Bloodreturn have blood")
#define DE_TR_CURE_BLOODRETURN_RUN_NO_BLOOD QLabel::tr("Bloodreturn runing no blood")
#define DE_TR_CURE_ONLINEBLOODRETURN_RUN_NO_BLOOD QLabel::tr("Online Bloodreturn no blood")
#define DE_TR_CURE_BLOODRETURN_STOP QLabel::tr("Bloodreturn stop have blood")
#define DE_TR_CURE_BLOODRETURN_STOP_NO_BLOOD QLabel::tr("Bloodreturn stop no blood")
#define DE_TR_DRAIN_RUN QLabel::tr("Drain runing")
#define DE_TR_DRAIN_STOP QLabel::tr("Drain stop")

#define DE_TR_WATERTOPREPARE QLabel::tr("Water to prepare")
#define DE_TR_DRYPREFLUSH QLabel::tr("Dry barrels preflush")
#define DE_TR_DMTEST QLabel::tr("DM test")
#define DE_TR_DMTESTOK QLabel::tr("DM test ok")
#define DE_TR_PROVINCIALFLUID QLabel::tr("Provincial fluid")
#define DE_TR_WATERSUSPENSION QLabel::tr("Water suspension")
#define DE_TR_PROVINCIALFLUIDBYPASS QLabel::tr("Provincial fluid and bypass")
#define DE_TR_STOPFLUID QLabel::tr("Stop fluid")
#define DE_TR_BYPASS QLabel::tr("Bypass")

#define DE_TR_DISINFECTIONPREPARE QLabel::tr("Disinfection preparation")
#define DE_TR_DISINFECTIONPREFLUSH QLabel::tr("Disinfection preflush")
#define DE_TR_SUCKFLUSH QLabel::tr("SUCK flush")
#define DE_TR_HOLD QLabel::tr("Hold")
#define DE_TR_LOOP QLabel::tr("Loop")
#define DE_TR_AFTERRINSE QLabel::tr("After rinse")
#define DE_TR_COLDRINSE QLabel::tr("Cold rinse")
#define DE_TR_HOTRINSE1 QLabel::tr("Hot rinse 1")
#define DE_TR_HOTRINSE2 QLabel::tr("Hot rinse 2")
#define DE_TR_HOTRINSE3 QLabel::tr("Hot rinse 3")
#define DE_TR_EMPTYING QLabel::tr("Emptying")
#define DE_TR_PIPERINSE QLabel::tr("Pipe rinse")
#define DE_TR_CCDSUCK QLabel::tr("The CCD suck")
#define DE_TR_CCDRINSE QLabel::tr("The CCD rinse")
#define DE_TR_CHD QLabel::tr("The CHD")

#define DE_TR_NAANDUFPROFILE QLabel::tr("NA UF Profile")
#define DE_TR_NAPROFILE QLabel::tr("NA Profile")
#define DE_TR_UFPROFILE QLabel::tr("UF Profile")

#define DE_TR_COOL_RINSE QLabel::tr("Cool Rinse") //冷冲洗#/
#define DE_TR_HOT_RINSE QLabel::tr("Hot Rinse") //热冲洗#/
#define DE_TR_CHEMICAL_CITRICACID QLabel::tr("Chemical Citricacid") //化消柠檬酸#/
#define DE_TR_CHEMICAL_PERACETIC QLabel::tr("Chemical Preacetic") //化消过氧乙酸#/
#define DE_TR_CHEMICAL3 QLabel::tr("Chemical-3") //化学消毒3#/
#define DE_TR_HOT_CHEMICAL QLabel::tr("Hot Chemical") //热化学消毒#/
#define DE_TR_HOT QLabel::tr("Hot Clean") //纯热消毒#/

#define DE_TR_HDDOUBLENEEDLE QLabel::tr("HD DoubleNeedle")
#define DE_TR_ISODOUBLENEEDLE QLabel::tr("ISO DoubleNeedle")
#define DE_TR_HDFONLINE QLabel::tr("HDFonline")
#define DE_TR_HFONLINE QLabel::tr("HFonline")
#define DE_TR_PEPH QLabel::tr("PE/PH")
#define DE_TR_HDSNSP QLabel::tr("HD SNSP")
#define DE_TR_HDSNDP QLabel::tr("HD SNDP")
#define DE_TR_ISOSNSP QLabel::tr("ISO SNSP")
#define DE_TR_ISOSNDP QLabel::tr("ISO SNDP")

#define DE_TR_PREDILUTE QLabel::tr("Pre Dilute")
#define DE_TR_POSTDILUTE QLabel::tr("Post Dilute")
#define DE_TR_TRNONE QLabel::tr("None")

#define DE_TR_DIAFLOWPROFILE QLabel::tr("DiaFlow Profile")  //透析液曲线#/
#define DE_TR_HEPARINPROFILEE QLabel::tr("Heparin Profile") //肝素曲线#/
#define DE_TR_CONDPROFILE QLabel::tr("Cond Profile") //电导曲线#/
#define DE_TR_TEMPPROFILE QLabel::tr("Temp Profile") //温度曲线#/
#define DE_TR_HCO3PROFILE QLabel::tr("HCO3 Profile") //碳酸氢根曲线#/

#define DE_TR_YOUCANSET QLabel::tr("You can set a maximum of 6 time period curves !") //最多可以设置6个时间段曲线#/
#define DE_TR_ADDITION QLabel::tr("Addition") //添加#/
#define DE_TR_RESET QLabel::tr("Reset") //重设#/
#define DE_TR_STARTTIME QLabel::tr("Start Time:") //开始时间：#/
#define DE_TR_ENDTIME QLabel::tr("End Time:") //结束时间：#/

#define DE_TR_ICCARDREADING QLabel::tr("IC card reading...") //IC 卡正在读取中...#/
#define DE_TR_ICCARDWRITING QLabel::tr("IC card writing...") //IC 卡正在写入中...#/
#define DE_TR_MALE QLabel::tr("male")  //男#/
#define DE_TR_FEMALE QLabel::tr("female") //女#/
#define DE_TR_RENALMEDICINE QLabel::tr("renal medicine")   //肾内科#/
#define DE_TR_ENDOCRINE QLabel::tr("endocrine")   //内分泌#/
#define DE_TR_RENALFAILURE QLabel::tr("renal failure") //肾衰竭#/
#define DE_TR_UREMIA QLabel::tr("uremia")  //尿毒症#/

#define DE_TR_PULSEAVG QLabel::tr("Pulse AVG")  //平均脉搏
#define DE_TR_SYSAVG QLabel::tr("SYS AVG")  //平均收缩压
#define DE_TR_DIAAVG QLabel::tr("DIA AVG")  //平均舒张压
#define DE_TR_APAVG QLabel::tr("AP AVG")  //平均动脉压
#define DE_TR_VPAVG QLabel::tr("VP AVG")  //平均静脉压
#define DE_TR_TMPAVG QLabel::tr("TMP AVG")  //平均跨膜压

#define DE_TR_AP QLabel::tr("AP")  //动脉压
#define DE_TR_VP QLabel::tr("VP")  //静脉压
#define DE_TR_TMP QLabel::tr("TMP")  //跨膜压

#define DE_TR_TIMEH QLabel::tr("Time(H)")  //时间(H)
#define DE_TR_TEMP QLabel::tr("Temp")  //温度
#define DE_TR_NA QLabel::tr("Na+")   //钠离子
#define DE_TR_HCO3 QLabel::tr("HCO3")  //碳酸氢根
#define DE_TR_UFGOAL QLabel::tr("UF goal")  //超滤目标
#define DE_TR_BLOODFLOW QLabel::tr("Blood Flow") //血泵流速
#define DE_TR_SUBVOLUME QLabel::tr("SUB Volume") //置换量
#define DE_TR_HEPARINVOLUME QLabel::tr("Heparin Volume") //肝素量
#define DE_TR_HEPARINRATE QLabel::tr("Heparin rate") //肝素速度
#define DE_TR_TREATMENTMODE QLabel::tr("Treatment mode") //治疗方式
#define DE_TR_PROFILEMODE QLabel::tr("Profile Mode") //曲线模式
#define DE_TR_WRITE QLabel::tr("Write") //写入
#define DE_TR_READ QLabel::tr("Read") //读取
#define DE_TR_CHOOSE QLabel::tr("Choose") //选择
#define DE_TR_DATATABLE QLabel::tr("Data Table") //数据表格

#define DE_TR_PATIENTMESSAGE QLabel::tr("patientMessage") //病人信息

#define DE_TR_CURETIMESSELECT QLabel::tr("Cure Times select") //治疗次数选择
#define DE_TR_CURETIMES QLabel::tr("Cure Times") //治疗次数
#define DE_TR_PRETIMES QLabel::tr("Pre Times") //上一次
#define DE_TR_NXTTIMES QLabel::tr("Nxt Times") //下一次

#define DE_TR_CONDPROFILE QLabel::tr("Cond Profile")  //电导曲线
#define DE_TR_CONDDATA QLabel::tr("Cond Data") //电导数据

#define DE_TR_TEMPPROFILE QLabel::tr("Temp Profile")  //温度曲线
#define DE_TR_TEMPDATA QLabel::tr("Temp Data") //温度数据

#define DE_TR_PRESSPROFILE QLabel::tr("Press profile") //压力曲线
#define DE_TR_PRESSDATA QLabel::tr("Press data")    //压力数据

#define DE_TR_BLOODPRESSPROFILE QLabel::tr("BloodPress profile")//血压曲线
#define DE_TR_BLOODPRESSDATA QLabel::tr("BloodPress data")//血压数据

#define DE_TR_IDNUM QLabel::tr("ID")   //编号
#define DE_TR_FIND QLabel::tr("Find")   //查找


#define DE_TR_ADDQUICKRATE QLabel::tr("Add Quick Rate")//补液速度

#define DE_TR_THISIDNUMBERFORCURRENTLYTREATMENT QLabel::tr("This id number for currently treatment") //当前进行治疗的ID号

#define DE_TR_TIMES QLabel::tr("times")   //次数
#define DE_TR_DATE QLabel::tr("date")  //日期
#define DE_TR_TEMP QLabel::tr("Temp")  //温度
#define DE_TR_COND QLabel::tr("Cond")  //电导
#define DE_TR_UFFLOW QLabel::tr("UFFlow") //超滤速度
#define DE_TR_BPFLOW QLabel::tr("BPFlow") //血泵速度
#define DE_TR_PULSE QLabel::tr("Pulse")  //脉搏
#define DE_TR_SYSDIA QLabel::tr("SYS/DIA")  //收缩/舒张
#define DE_TR_FIRSTPAGE QLabel::tr("First Page")  //第一页
#define DE_TR_PREPAGE QLabel::tr("Pre Page")  //上一页
#define DE_TR_NXTPAGE QLabel::tr("Nxt Page")  //下一页
#define DE_TR_LASTPAGE QLabel::tr("Last Page")  //最后页

#define DE_TR_SYS QLabel::tr("SYS")  //收缩压
#define DE_TR_DIA QLabel::tr("DIA")  //舒张压

#define DE_TR_MAXVALUE QLabel::tr("Max Value")
#define DE_TR_MINVALUE QLabel::tr("Min Value")
#define DE_TR_AVGVALUE QLabel::tr("Avg Value")

#define DE_TR_DATAANALYSIS QLabel::tr("Data Analysis") //数据分析

#define DE_TR_STANDBYMODE01 QLabel::tr("standby mode 01") //第一待机模式#/
#define DE_TR_STANDBYMODE02 QLabel::tr("standby mode 02") //第二待机模式#/
#define DE_TR_NOSTANDBYMODE QLabel::tr("No standby mode runing") //无待机模式运行#/
//scence_setup_workmode.cpp
#define DE_TR_TREATMENTMODELNOTCHANGED QLabel::tr("Treatment model has not changed!")

//scence_setup_isouf.cpp
#define DE_TR_UFVOLUMECANNOTEXCEED QLabel::tr("UF volume can not exceed the remaining!")
#define DE_TR_UFVOLUMECANNOTLESS QLabel::tr("UF volume can not less than finish volume!")
#define DE_TR_THETIMECANNOTEXCEED QLabel::tr("The time can not exceed the remaining time!")
#define DE_TR_THETIMECANNOTLESS QLabel::tr("The time can not less than residue time!")
#define DE_TR_THEPARAMETERSAFRESH QLabel::tr("The parameters afresh,and take effect!")
#define DE_TR_WHETHERTOSTARTISO QLabel::tr("Whether to start ISO?")
//scence_setup_subspump.cpp
#define DE_TR_WHETHERTOSTARTTHIS QLabel::tr("Whether to start this model?")
#define DE_TR_THESUBSPUMPGOAL QLabel::tr("The subspump goal reach,stop!")

//widget_aim_config.cpp
#define DE_TR_ISOTREATMENTEND QLabel::tr("ISO treatment ends, have switch to ")
#define DE_TR_CANNOTSWITCHTOPE QLabel::tr("Can not switch to PE in treatment runing!")

//scene_disinfect.cpp
#define DE_TR_CHANGEFILTERCONFIRM QLabel::tr("Please Confirm")
#define DE_TR_FILTERASK  QLabel::tr("Warning:Do You want reset all Filter data ?")
#define DE_TR_FILTERINFO QLabel::tr(" Reset  Filter data  or dry filter ")
#define DE_TR_CHANGEFILTER1  QLabel::tr("Change Filter1")
#define DE_TR_CHANGEFILTER2  QLabel::tr("Change Filter2")

//state_disinfect.cpp
#define DE_TR_THESTATUSOFMASTERABNORMAL QLabel::tr("The status of the mastercontrol is abnormal,please try again!")

/*------------------------报警模块的报警条目内容------------------------------*/
//血路报警条目
//动脉压上限
#define DE_TR_TITLE_APUP QLabel::tr("Upper Arterial pressure alarm")
#define DE_TR_ALARM_APUP QLabel::tr("Upper Arterial pressure alarm !")
//静脉压上限
#define DE_TR_TITLE_VPUP QLabel::tr("Upper venous pressure alarm")
#define DE_TR_ALARM_VPUP QLabel::tr("Upper venous pressure alarm !")
//跨膜压上限
#define DE_TR_TITLE_TMPUP QLabel::tr("Upper TMP alarm")
#define DE_TR_ALARM_TMPUP QLabel::tr("Upper TMP alarm !")
//血温上限
#define DE_TR_TITLE_TEMPERUP QLabel::tr("Upper temperature alarm")
#define DE_TR_ALARM_TEMPERUP QLabel::tr("Upper temperature alarm !")
//动脉压下限
#define DE_TR_TITLE_APDOWN QLabel::tr("Lower Arterial pressure alarm")
#define DE_TR_ALARM_APDOWN QLabel::tr("Lower Arterial pressure alarm !")
//静脉压下限
#define DE_TR_TITLE_VPDOWN QLabel::tr("Lower venous pressure alarm")
#define DE_TR_ALARM_VPDOWN QLabel::tr("Lower venous pressure alarm !")
//跨膜压下限
#define DE_TR_TITLE_TMPDOWN QLabel::tr("Lower TMP alarm")
#define DE_TR_ALARM_TMPDOWN QLabel::tr("Lower TMP alarm !")
//血温下限
#define DE_TR_TITLE_TEMPERDOWN QLabel::tr("Lower temperature alarm")
#define DE_TR_ALARM_TEMPERDOWN QLabel::tr("Lower temperature alarm !")
//液位报警
#define DE_TR_TITLE_FLUID QLabel::tr("Blood level alarm")
#define DE_TR_ALARM_FLUID QLabel::tr("Blood level alarm !")
//动脉气泡
#define DE_TR_TITLE_ARTERIES_AIR QLabel::tr("Arteries Bubble alarm")
#define DE_TR_ALARM_ARTERIES_AIR QLabel::tr("Arteries Bubble alarm !")
//无血报警
#define DE_TR_TITLE_NO_BLOOD QLabel::tr("No blood alarm")
#define DE_TR_ALARM_NO_BLOOD QLabel::tr("No blood alarm !")
//静脉气泡
#define DE_TR_TITLE_VEIN_AIR QLabel::tr("Vein Bubble alarm")
#define DE_TR_ALARM_VEIN_AIR QLabel::tr("Vein Bubble alarm !")
//血泵1失控
#define DE_TR_TITLE_N9BLOODOUT QLabel::tr("No1 blood pump out of control")
#define DE_TR_ALARM_N9BLOODOUT QLabel::tr("No1 blood pump out of control !")
//血泵2失控
#define DE_TR_TITLE_N10BLOODOUT QLabel::tr("No2 blood pump out of control")
#define DE_TR_ALARM_N10BLOODOUT QLabel::tr("No2 blood pump out of control !")
//血泵1盖门打开
#define DE_TR_TITLE_N9CAPOPEN QLabel::tr("Please close blood pump door")
#define DE_TR_ALARM_N9CAPOPEN QLabel::tr("Please close blood pump door!")
//血泵2盖门打开
#define DE_TR_TITLE_N10CAPOPEN QLabel::tr("Please close spare pump door")
#define DE_TR_ALARM_N10CAPOPEN QLabel::tr("Please close spare pump door!")
//漏血
#define DE_TR_TITLE_BLOODLEAK QLabel::tr("Blood Leak")
#define DE_TR_ALARM_BLOODLEAK QLabel::tr("Blood Leak !")
//肝素空液、阻塞
#define DE_TR_TITLE_HEPARINBLOCK QLabel::tr("Heparin empty Or  block")
#define DE_TR_ALARM_HEPARINBLOCK QLabel::tr("Heparin empty Or  block !")
//血压计报警
#define DE_TR_TITLE_BLOODMETER QLabel::tr("Blood pressure monitor alarm")
#define DE_TR_ALARM_BLOODMETER QLabel::tr("Blood pressure monitor alarm !")
//透析器前压力上限
#define DE_TR_TITLE_BDPUP QLabel::tr("The pressure before the dialyzer")
#define DE_TR_ALARM_BDPUP QLabel::tr("The pressure before the dialyzer upper limit !")
//透析器前压力下限
#define DE_TR_TITLE_BDPDOWN  QLabel::tr("The pressure before the dialyzer")
#define DE_TR_ALARM_BDPDOWN  QLabel::tr("The pressure before the dialyzer lower limit !")

//水路报警条目
//出水口总电导度上限
#define DE_TR_TITLE_OUTSUMCONDUP  QLabel::tr("The water outlet Conductivity")
#define DE_TR_ALARM_OUTSUMCONDUP  QLabel::tr("The water outlet of the upper limit of Conductivity !")
//出水口总电导度下限
#define DE_TR_TITLE_OUTSUMCONDDOWN  QLabel::tr("The water outlet Conductivity")
#define DE_TR_ALARM_OUTSUMCONDDOWN  QLabel::tr("The water outlet of the lower limit of the Conductivity !")
//出水口总电导度范围报警
#define DE_TR_TITLE_OUTSUMCOND  QLabel::tr("Alarming of water outlet total Conductivity")
#define DE_TR_ALARM_OUTSUMCOND  QLabel::tr("Alarming of water outlet total Conductivity !")
//混合总电导度上限
#define DE_TR_TITLE_MIXSUMCONDUP  QLabel::tr("Alarming of the total Conductivity of mixed")
#define DE_TR_ALARM_MIXSUMCONDUP  QLabel::tr("The upper limit of the total Conductivity of mixed !")
//混合总电导度下限
#define DE_TR_TITLE_MIXSUMCONDDOWN  QLabel::tr("Alarming of the total Conductivity of mixed")
#define DE_TR_ALARM_MIXSUMCONDDOWN  QLabel::tr("The lower limit of the total Conductivity of mixed !")
//混合总电导度范围报警
#define DE_TR_TITLE_MIXSUMCOND  QLabel::tr("Hybrid total Conductivity alarm")
#define DE_TR_ALARM_MIXSUMCOND  QLabel::tr("Hybrid total Conductivity range alarm !")
//透析夜温度太高 极限
#define DE_TR_TITLE_FTMPHIGHLIMIT  QLabel::tr("Dialysis fluid temperature is higher than the limit")
#define DE_TR_ALARM_FTMPHIGHLIMIT  QLabel::tr("Dialysis fluid temperature is higher than the limit !")
//温度太低 极限
#define DE_TR_TITLE_FTMPLOWLIMIT  QLabel::tr("Dialysis fluid temperature is below the limit")
#define DE_TR_ALARM_FTMPLOWLIMIT  QLabel::tr("Dialysis fluid temperature is below the limit !")
//温度上限
#define DE_TR_TITLE_FTMPUP  QLabel::tr("The upper temperature limit")
#define DE_TR_ALARM_FTMPUP  QLabel::tr("The upper temperature limit !")
//温度下限
#define DE_TR_TITLE_FTMPDOWN  QLabel::tr("The lower limit of temperature")
#define DE_TR_ALARM_FTMPDOWN  QLabel::tr("The lower limit of temperature !")
//温度范围报警
#define DE_TR_TITLE_FTMPRANGE  QLabel::tr("The range of temperature alarm")
#define DE_TR_ALARM_FTMPRANGE  QLabel::tr("The range of temperature alarm !")
//A液电导率错误
#define DE_TR_TITLE_ACONDERROR  QLabel::tr("A liquid conductivity error")
#define DE_TR_ALARM_ACONDERROR  QLabel::tr("A liquid conductivity error !")
//B液电导率错误
#define DE_TR_TITLE_BCONDERROR  QLabel::tr("B liquid conductivity error")
#define DE_TR_ALARM_BCONDERROR  QLabel::tr("B liquid conductivity error !")
//A无液
#define DE_TR_TITLE_ANOFLUID  QLabel::tr("No A dialysate")
#define DE_TR_ALARM_ANOFLUID  QLabel::tr("No A dialysate !")
//B无液
#define DE_TR_TITLE_BNOFLUID  QLabel::tr("No B dialysate")
#define DE_TR_ALARM_BNOFLUID  QLabel::tr("No B dialysate !")
//无水报警
#define DE_TR_TITLE_NOWATER  QLabel::tr("The alarm of no water")
#define DE_TR_ALARM_NOWATER  QLabel::tr("The alarm of no water !")
//消毒液电导上限错误
#define DE_TR_TITLE_DISCONDUP  QLabel::tr("Disinfection liquid conductivity toplimit error")
#define DE_TR_ALARM_DISCONDUP  QLabel::tr("Disinfection liquid conductivity toplimit error !")
//消毒液电导下限错误
#define DE_TR_TITLE_DISCONDDOWN QLabel::tr("Disinfection liquid conductivity lower limit error")
#define DE_TR_ALARM_DISCONDDOWN QLabel::tr("Disinfection liquid conductivity lower limit error !")
//补液除气错误
#define DE_TR_TITLE_FLUIDOUTAIR  QLabel::tr("Fluid degassing error")
#define DE_TR_ALARM_FLUIDOUTAIR  QLabel::tr("Fluid degassing error !")
//平衡腔破膜报警
#define DE_TR_TITLE_BALANCEBREAK  QLabel::tr("Balance cavity film breaking alarm")
#define DE_TR_ALARM_BALANCEBREAK  QLabel::tr("Balance cavity film breaking alarm !")

//提示性报警条目
//将A液吸管放到A液桶
#define DE_TR_TITLE_ATOBARREL  QLabel::tr("Connect tube A to pot A")
#define DE_TR_PROMPT_ATOBARREL QLabel::tr("Connect tube A to pot A !")
//将A液吸管插回机器
#define DE_TR_TITLE_ATODEVICE  QLabel::tr("Connect tube A to machine")
#define DE_TR_PROMPT_ATODEVICE QLabel::tr("Connect tube A to machine !")
//将B液管放到B液桶
#define DE_TR_TITLE_BTOBARREL  QLabel::tr("Connect tube B to pot B")
#define DE_TR_PROMPT_BTOBARREL QLabel::tr("Connect tube B to pot B !")
//将B液管插回机器
#define DE_TR_TITLE_BTODEVICE  QLabel::tr("Connect tube B to machine")
#define DE_TR_PROMPT_BTODEVICE  QLabel::tr("Connect tube B to machine !")
//将供液管接到透析器
#define DE_TR_TITLE_FEEDTODIA  QLabel::tr("Connect supply tube to dialyzer")
#define DE_TR_PROMPT_FEEDTODIA  QLabel::tr("Connect supply tube to dialyzer !")
//将供液管接回机器
#define DE_TR_TITLE_FEEDTODEVICE  QLabel::tr("Connect supply tube to machine")
#define DE_TR_PROMPT_FEEDTODEVICE  QLabel::tr("Connect supply tube to machine !")
//将回液管接到透析器
#define DE_TR_TITLE_BACKTODIA  QLabel::tr("Connect return tubes of dialyzer to the  port of dialyzer")
#define DE_TR_PROMPT_BACKTODIA  QLabel::tr("Connect return tubes of dialyzer to the  port of dialyzer !")
//将回液管接回机器
#define DE_TR_TITLE_BACKTODEVICE  QLabel::tr("Connect return tubes of dialyzer to the  port of machine")
#define DE_TR_PROMPT_BACKTODEVICE  QLabel::tr("Connect return tubes of dialyzer to the  port of machine!")
//将干粉架打开
#define DE_TR_TITLE_DRYFRAMEOPEN  QLabel::tr("Dry frame open")
#define DE_TR_PROMPT_DRYFRAMEOPEN QLabel::tr("Dry frame open !")
//将干粉架关闭
#define DE_TR_TITLE_DRYFRAMECLOSE  QLabel::tr("Dry frame close")
#define DE_TR_PROMPT_DRYFRAMECLOSE QLabel::tr("Dry frame close !")

//将相应管路接到置换液接口#/
#define DE_TR_TITLE_SETUPCORRESPONDING  QLabel::tr("Set up fluid interface")
#define DE_TR_PROMPT_SETUPCORRESPONDING QLabel::tr("Set up corresponding line displacement fluid interface!")

//将置换液接口管路移除#/
#define DE_TR_TITLE_REMOVETHEINTERFACE  QLabel::tr("Remove fluid interface")
#define DE_TR_PROMPT_REMOVETHEINTERFACE QLabel::tr("Remove the displacement fluid interface line!")

//将相应管路接到置换液冲洗接口#/
#define DE_TR_TITLE_SETUPFLUSHINTERFACE QLabel::tr("Set up fluid flush interface")
#define DE_TR_PROMPT_SETUPFLUSHINTERFACE QLabel::tr("Set up corresponding line displacement fluid flush interface!")

//将置换液冲洗接口管路移除#/
#define DE_TR_TITLE_REMOVEFLUSHINTERFACE QLabel::tr("Remove fluid flush interface")
#define DE_TR_PROMPT_REMOVEFLUSHINTERFACE QLabel::tr("Remove the replacement wash liquid interface line!")

//自检状态下的报警条目
//阻流夹1故障
#define DE_TR_TITLE_SELFCHECK_CHOKECLIP1 QLabel::tr("CLPV error")
#define DE_TR_PROMPT_SELFCHECK_CHOKECLIP1 QLabel::tr("CLPV drive or flow resistance feedback!")
//阻流夹2故障
#define DE_TR_TITLE_SELFCHECK_CHOKECLIP2 QLabel::tr("CLPA error")
#define DE_TR_PROMPT_SELFCHECK_CHOKECLIP2 QLabel::tr("CLPA drive or flow resistance feedback!")
//静脉气泡故障
#define DE_TR_TITLE_SELFCHECK_VPBUBBLE QLabel::tr("ADPV error")
#define DE_TR_PROMPT_SELFCHECK_VPBUBBLE QLabel::tr("Remove the line!")
//动脉气泡故障
#define DE_TR_TITLE_SELFCHECK_APBUBBLE QLabel::tr("ADPA error")
#define DE_TR_PROMPT_SELFCHECK_APBUBBLE QLabel::tr("Remove the line!")
//液位故障
#define DE_TR_TITLE_SELFCHECK_LEVEL QLabel::tr("LDS error")
#define DE_TR_PROMPT_SELFCHECK_LEVEL QLabel::tr("Remove the line!")
//动脉压故障#/
#define DE_TR_TITLE_SELFCHECK_PA_ERROR QLabel::tr("PA error")
#define DE_TR_PROMPT_SELFCHECK_PA_ERROR QLabel::tr("The zero offset is too large!")
//静脉压故障#/
#define DE_TR_TITLE_SELFCHECK_PV_ERROR QLabel::tr("PV error")
#define DE_TR_PROMPT_SELFCHECK_PV_ERROR QLabel::tr("The zero offset is too large!")
//跨膜压故障#/
#define DE_TR_TITLE_SELFCHECK_TMP_ERROR QLabel::tr("TMP error")
#define DE_TR_PROMPT_SELFCHECK_TMP_ERROR QLabel::tr("The zero offset is too large!")
//漏血监测器故障
#define DE_TR_TITLE_SELFCHECK_BLOODLEAK QLabel::tr("BLM error")
#define DE_TR_PROMPT_SELFCHECK_BLOODLEAK QLabel::tr("Blood leak monitor fault!")
//血氧模块故障
#define DE_TR_TITLE_SELFCHECK_BLOODOXYGEN QLabel::tr("BOM error")
#define DE_TR_PROMPT_SELFCHECK_BLOODOXYGEN QLabel::tr("No blood oxygen module or communication failures!")
//血压模块故障
#define DE_TR_TITLE_SELFCHECK_TONOMETER QLabel::tr("BPM error")
#define DE_TR_PROMPT_SELFCHECK_TONOMETER QLabel::tr("No Blood pressure module or communication failures!")
//血泵1失控
#define DE_TR_TITLE_SELFCHECK_N9BLOODOUT QLabel::tr("BLOOD1 error")
#define DE_TR_PROMPT_SELFCHECK_N9ISOUT QLabel::tr("NO1 blood pump is out of control!")
//血泵1无反馈信号
#define DE_TR_TITLE_SELFCHECK_N9NOFEEDBACK QLabel::tr("BLOOD1 error")
#define DE_TR_PROMPT_SELFCHECK_N9NOFEEDBACK QLabel::tr("NO1 blood pump no feedback!")
//血泵1自检失败
#define DE_TR_TITLE_SELFCHECK_N9SELFCHECKFAIL QLabel::tr("BLOOD1 error")
#define DE_TR_PROMPT_SELFCHECK_NO1BLOODPUMPSELFCHECKFAIL QLabel::tr("NO1 blood pump selfcheck fail!")
//血泵2失控
#define DE_TR_TITLE_SELFCHECK_N10BLOODOUT QLabel::tr("BLOOD2 error")
#define DE_TR_PROMPT_SELFCHECK_N10ISOUT QLabel::tr("NO2 blood pump is out of control!")
//血泵2无反馈信号
#define DE_TR_TITLE_SELFCHECK_N10NOFEEDBACK QLabel::tr("BLOOD2 error")
#define DE_TR_PROMPT_SELFCHECK_N10NOFEEDBACK QLabel::tr("NO2 blood pump no feedback!")
//血泵2自检失败
#define DE_TR_TITLE_SELFCHECK_N10SELFCHECKFAIL QLabel::tr("BLOOD2 error")
#define DE_TR_PROMPT_SELFCHECK_NO2BLOODPUMPSELFCHECKFAIL QLabel::tr("NO2 blood pump selfcheck fail!")
//肝素泵自检失败
#define DE_TR_TITLE_SELFCHECK_N15SELFCHECKFAIL QLabel::tr("Heparin error")
#define DE_TR_PROMPT_SELFCHECK_HEPARINSELFCHECKFAIL QLabel::tr("Heparin pump selfcheck fail!")

//主控板的自检项目
//N7自检失败
#define DE_TR_TITLE_SELFCHECK_N7ERROR QLabel::tr("N7 error")
#define DE_TR_PROMPT_SELFCHECK_N7SELFCHECKFAIL QLabel::tr("N7 selfcheck fail!")
//N9自检失败
#define DE_TR_TITLE_SELFCHECK_N9ERROR QLabel::tr("N9 error")
#define DE_TR_PROMPT_SELFCHECK_N9SELFCHECKFAIL QLabel::tr("N9 selfcheck fail!")
//N10自检失败
#define DE_TR_TITLE_SELFCHECK_N10ERROR QLabel::tr("N10 error")
#define DE_TR_PROMPT_SELFCHECK_N10SELFCHECKFAIL QLabel::tr("N10 selfcheck fail!")
//N15自检失败
#define DE_TR_TITLE_SELFCHECK_N15ERROR QLabel::tr("N15 error")
#define DE_TR_PROMPT_SELFCHECK_N15SELFCHECKFAIL QLabel::tr("N15 selfcheck fail!")
//N5自检失败
#define DE_TR_TITLE_SELFCHECK_N5ERROR QLabel::tr("N5 error")
#define DE_TR_PROMPT_SELFCHECK_N5SELFCHECKFAIL QLabel::tr("N5 selfcheck fail!")

//血路自检失败
#define DE_TR_TITLE_SELFCHECK_BLOODPARTSELFCHECKFAIL QLabel::tr("Selfcheck:Part of the blood selfcheck fail")
#define DE_TR_PROMPT_SELFCHECK_BLOODPARTSELFCHECKFAIL QLabel::tr("Part of the blood selfcheck fail!")
//水路自检失败
#define DE_TR_TITLE_SELFCHECK_WATERPARTSELFCHECKFAIL QLabel::tr("Selfcheck:Part of the water selfcheck fail")
#define DE_TR_PROMPT_SELFCHECK_WATERPARTSELFCHECKFAIL QLabel::tr("Part of the water selfcheck fail!")

//扩展自检失败
#define DE_TR_TITLE_SOME_ERROR QLabel::tr("some error")
#define DE_TR_PROMPT_ALARM_CODE QLabel::tr("Please check alarm code!")

//报警来源
 //提示性报警
#define DE_TR_NOTE  QLabel::tr("prompt")
//来源N3或者N5的PDO1
#define DE_TR_ALARM_PDO1 QLabel::tr("Blood or water")
//程序提示
#define DE_TR_ALARM_OWN QLabel::tr("Program prompts")
//电源
#define DE_TR_ALARM_N7 QLabel::tr("The power")
//血泵1
#define DE_TR_ALARM_N9 QLabel::tr("Blood pump")
//血泵2
#define DE_TR_ALARM_N10 QLabel::tr("Backup pump")
//肝素泵
#define DE_TR_ALARM_N15 QLabel::tr("Heparin pump")
//自检
#define DE_TR_SELFCHECK QLabel::tr("Self check")
//未知来源
#define DE_TR_UNKNOWN QLabel::tr("The unknown")

//canopen通信失败
//N3 canopen通信失败#/
#define DE_TR_TITLE_N3CANOPEN_FAIL QLabel::tr("N3 communication failure")
#define DE_TR_PROMPT_N3CANOPEN_FAIL QLabel::tr("N3 canopen error!")
//N5 canopen通信失败#/
#define DE_TR_TITLE_N5CANOPEN_FAIL QLabel::tr("N5 communication failure")
#define DE_TR_PROMPT_N5CANOPEN_FAIL QLabel::tr("N5 canopen error!")
//N9 canopen通信失败#/
#define DE_TR_TITLE_N9CANOPEN_FAIL QLabel::tr("N9 communication failure")
#define DE_TR_PROMPT_N9CANOPEN_FAIL QLabel::tr("N9 canopen error!")
//N10 canopen通信失败#/
#define DE_TR_TITLE_N10CANOPEN_FAIL QLabel::tr("N10 communication failure")
#define DE_TR_PROMPT_N10CANOPEN_FAIL QLabel::tr("N10 canopen error!")
//N15 canopen通信失败#/
#define DE_TR_TITLE_N15CANOPEN_FAIL QLabel::tr("N15 communication failure")
#define DE_TR_PROMPT_N15CANOPEN_FAIL QLabel::tr("N15 canopen error!")
//N7 canopen通信失败#/
#define DE_TR_TITLE_N7CANOPEN_FAIL QLabel::tr("N7 communication failure")
#define DE_TR_PROMPT_N7CANOPEN_FAIL QLabel::tr("N7 canopen error!")
//护士呼#/
#define DE_TR_TITLE_NURSECALL QLabel::tr("The patient calls")   //病人呼叫#/
#define DE_TR_PROMPT_NURSECALL QLabel::tr("Patients with emergency call !") //病人紧急呼叫#/
//水路工作异常#/
#define DE_TR_TITLE_WATER_WORKFAIL  QLabel::tr("water working abnormal")
#define DE_TR_PROMPT_WATER_WORKFAIL QLabel::tr("Master plate water problem!")
//单次定时报警提示#/
#define DE_TR_TITLE_ALARMONETIME QLabel::tr("Single time alarm")   //单次定时报警#/
#define DE_TR_PROMPT_ALARMONETIME QLabel::tr("Single timeout !") //单次定时到#/
//循环定时报警提示#/
#define DE_TR_TITLE_ALARMCYCLETIME QLabel::tr("Cycle time alarm")   //循环定时报警#/
#define DE_TR_PROMPT_ALARMCYCLETIME QLabel::tr("Cycle timeout !") //循环定时到#/

//主控板手动预冲状态错误#/
#define DE_TR_TITLE_MANUAL_PREFLUSH_ERROR  QLabel::tr("Manual preflush state error")
#define DE_TR_PROMPT_MANUAL_PREFLUSH_ERROR QLabel::tr("Master plate manual preflush state error!")
//bit2：自动预冲#/
#define DE_TR_TITLE_AUTO_PREFLUSH_ERROR  QLabel::tr("Auto preflush state error")
#define DE_TR_PROMPT_AUTO_PREFLUSH_ERROR QLabel::tr("Master plate auto preflush state error!")
//bit3：在线预冲#/
#define DE_TR_TITLE_ONLINE_PREFLUSH_ERROR  QLabel::tr("Online preflush state error")
#define DE_TR_PROMPT_ONLINE_PREFLUSH_ERROR QLabel::tr("Master plate online preflush state error!")
//bit4：引血#/
#define DE_TR_TITLE_GETBLOOD_ERROR  QLabel::tr("Connect P.t state error")
#define DE_TR_PROMPT_GETBLOOD_ERROR QLabel::tr("Master plate Connect P.t state error!")
//bit5：在线引血#/
#define DE_TR_TITLE_ONLINE_GETBLOOD_ERROR  QLabel::tr("Online connect P.t state error")
#define DE_TR_PROMPT_ONLINE_GETBLOOD_ERROR QLabel::tr("Master plate Online connect P.t state error!")
//bit6：回血#/
#define DE_TR_TITLE_RETURNBLOOD_ERROR  QLabel::tr("Return blood state error")
#define DE_TR_PROMPT_RETURNBLOOD_ERROR QLabel::tr("Master plate Return blood state error!")
//bit7：在线回血#/
#define DE_TR_TITLE_ONLINE_RETURNBLOOD_ERROR  QLabel::tr("Online return blood state error")
#define DE_TR_PROMPT_ONLINE_RETURNBLOOD_ERROR QLabel::tr("Master plate Online return blood state error!")

//收缩压UP报警#/
#define DE_TR_TITLE_SYSTOLIC_UP_ALARM  QLabel::tr("Systolic upper limit alarm")
#define DE_TR_PROMPT_SYSTOLIC_UP_ALARM QLabel::tr("Systolic beyond the upper limit!")
//收缩压DOWN报警#/
#define DE_TR_TITLE_SYSTOLIC_DOWN_ALARM  QLabel::tr("Systolic lower limit alarm")
#define DE_TR_PROMPT_SYSTOLIC_DOWN_ALARM QLabel::tr("Systolic beyond the lower limit!")

//舒张压UP报警#/
#define DE_TR_TITLE_DIASTOLIC_UP_ALARM  QLabel::tr("Diastolic upper limit alarm")
#define DE_TR_PROMPT_DIASTOLIC_UP_ALARM QLabel::tr("Diastolic beyond the upper limit!")
//舒张压DOWN报警#/
#define DE_TR_TITLE_DIASTOLIC_DOWN_ALARM  QLabel::tr("Diastolic lower limit alarm")
#define DE_TR_PROMPT_DIASTOLIC_DOWN_ALARM QLabel::tr("Diastolic beyond the lower limit!")

//脉搏UP报警#/
#define DE_TR_TITLE_PULSE_UP_ALARM  QLabel::tr("Pulse upper limit alarm")
#define DE_TR_PROMPT_PULSE_UP_ALARM QLabel::tr("Pulse beyond the upper limit!")
//脉搏DOWN报警#/
#define DE_TR_TITLE_PULSE_DOWN_ALARM  QLabel::tr("Pulse lower limit alarm")
#define DE_TR_PROMPT_PULSE_DOWN_ALARM QLabel::tr("Pulse beyond the lower limit!")

//肝素泵时间到停止#/
#define DE_TR_TITLE_HEPARIN_TIMESTOP_ALARM  QLabel::tr("Time to stop heparin pump")
#define DE_TR_PROMPT_HEPARIN_TIMESTOP_ALARM QLabel::tr("Has reached the stop time!")

//肝素泵没有打开#/
#define DE_TR_TITLE_HEPARIN_PLEASEOPEN_PROMPT  QLabel::tr("Please open the heparin pump")
#define DE_TR_PROMPT_HEPARIN_PLEASEOPEN_PROMPT QLabel::tr("Didn't open the heparin pump!")

//报警处理完后，提示打开血泵#/
#define DE_TR_TITLE_PLEASEOPENBLOOD_PROMPT  QLabel::tr("Please open the blood pump")
#define DE_TR_PROMPT_PLEASEOPENBLOOD_PROMPT QLabel::tr("The alarm already processed,please open the blood pump!")

//XML文件读取异常#/
#define DE_TR_TITLE_XMLFILEERROR_PROMPT  QLabel::tr("XML file error")
#define DE_TR_PROMPT_XMLFILEERROR_PROMPT QLabel::tr("xml file read error,the param is:")

//市电停止
#define DE_TR_TITLE_ELECTRICSUPPLYSTOP QLabel::tr("Electric supply stop")
#define DE_TR_PROMPT_ELECTRICSUPPLYSTOP QLabel::tr("Electric supply stop!")

//---报警模块---提示性文字
#define DE_TR_PLEASESELECTAROW QLabel::tr("Please select a row !")
#define DE_TR_AREYOUSUREDELETE QLabel::tr("Are you sure to delete ?")
#define DE_TR_BPMTESTING QLabel::tr("BPM runing...")

//scene_setup_cure_time.cpp
#define DE_TR_RESIDUELESSHEPARINPRESTOP QLabel::tr("Residue time less than heparinPrestop time,heparin will not run!")
#define DE_TR_CURECHANGEDPROFILECANCEL QLabel::tr("Cure time has changed,profile cancel!")
#define DE_TR_WORKMODECHANGEDPROFILECANCEL QLabel::tr("Workmode has changed,profile cancel!")
#define DE_TR_UFAIMVOLCHANGEDPROFILECANCEL QLabel::tr("UF Aimvol has changed,uf profile cancel!")
#define DE_TR_REMAININGTIMETOOLESSPROFILECANCEL QLabel::tr("The remaining time too little, uf volume can not reach,uf profile cancel!")

//---记录曲线页面
#define DE_TR_CONDRECORDCURVE QLabel::tr("Cond Record Profile")
#define DE_TR_TEMPRECORDCURVE QLabel::tr("Temp Record Profile")
#define DE_TR_CONDRECORDDATA QLabel::tr("Cond Record Data")
#define DE_TR_TEMPRECORDDATA QLabel::tr("Temp Record Data")

//scene_summay.cpp
#define DE_TR_HASSELECTPROFILE QLabel::tr("Has select profile cure,profile num:")
#define DE_TR_PROFILERUNING QLabel::tr("Profile cure runing,profile num:")
#define DE_TR_NOTSELECTPROFILECURE QLabel::tr("Not select profile cure")

//工程模式页面#/
#define DE_TR_NACUREPREDEFINEDMODIFY QLabel::tr("Na cure predefined modify")
#define DE_TR_UFCUREPREDEFINEDMODIFY QLabel::tr("UF cure predefined modify")
#define DE_TR_NAPRECURE QLabel::tr("Na precure")
#define DE_TR_UFPRECURE QLabel::tr("Uf precure")
#define DE_TR_SAVESUCCESS QLabel::tr("Save success!")
#define DE_TR_WHETHERTORESTOREXMLFILE QLabel::tr("Whether to Restore xml file?")

//工程模式页面----XML#/
//#define DE_TR_UFGOAL  QLabel::tr("UF_Goal")
#define DE_TR_UFRATE  QLabel::tr("UF_Rate")
#define DE_TR_UFTIME  QLabel::tr("UF_Time")
#define DE_TR_UFEARLIERTIME  QLabel::tr("Earlier_Time")
#define DE_TR_AUTOCHANGELIMIT  QLabel::tr("AutoChange_Limit")

#define DE_TR_DIATEMP  QLabel::tr("Dia_Temp")
#define DE_TR_DIACOND  QLabel::tr("Dia_Cond")
#define DE_TR_DIARATE  QLabel::tr("Dia_Rate")
#define DE_TR_DIAONLINERATE  QLabel::tr("Dia_OnlineRate")

#define DE_TR_BPWIDTH  QLabel::tr("BP_Width")
#define DE_TR_BPRATECHECKSELF  QLabel::tr("BP_RateCheckSelf")
#define DE_TR_BPRATETHREATMENT  QLabel::tr("BP_RateThreatMent")
#define DE_TR_BPRATERETURNBLOOD  QLabel::tr("BP_RateReturnBlood")
#define DE_TR_BPRATEPREFLUSH  QLabel::tr("BP_RatePreflush")
#define DE_TR_BPRATEGETBLOOD  QLabel::tr("BP_RateGetBlood")

#define DE_TR_HEPARINFIRSTVOLUME QLabel::tr("First_Volume")
//#define DE_TR_HEPARINRATE QLabel::tr("Heparin_Rate")
#define DE_TR_HEPARINPRESTOPTIME QLabel::tr("Pre_StopTime")
#define DE_TR_HEPARINADDVOLUME QLabel::tr("Add_Volume")

#define DE_TR_SUBPUMPFLUX QLabel::tr("SubPump_Flux")
#define DE_TR_SUBPUMPGOAL QLabel::tr("SubPump_Goal")
#define DE_TR_SUBPUMPRAPIDRATE QLabel::tr("SubPump_RapidRate")

#define DE_TR_TREATTIMEHOUR QLabel::tr("Hour")
#define DE_TR_TREATTIMEMIN QLabel::tr("Min")

#define DE_TR_PREFLUSHBPRATEFIRST QLabel::tr("BpRate_First_FiveMin")
#define DE_TR_PREFLUSHBPRATE QLabel::tr("BpRate")
#define DE_TR_PREFLUSHDIALYSERRATE QLabel::tr("Dialyser_Rate")
#define DE_TR_PREFLUSHTIME QLabel::tr("Preflush_Time")
#define DE_TR_PREFLUSHPFUFRATE QLabel::tr("PF_UF_Rate")
#define DE_TR_PREFLUSHPFUFVOLUME QLabel::tr("PF_UF_Volume")
#define DE_TR_PREFLUSHPFVOLUME QLabel::tr("PF_Volume")
#define DE_TR_PREFLUSHBPRATECONECT QLabel::tr("BpRate_Conect")
#define DE_TR_PREFLUSHBPRATEPHASE1 QLabel::tr("BpRate_phase1")
#define DE_TR_PREFLUSHBPRATEPHASE2 QLabel::tr("BpRate_phase2")
#define DE_TR_PREFLUSHBPRATEPHASE3 QLabel::tr("BpRate_phase3")
#define DE_TR_PREFLUSHTMPUPPF QLabel::tr("TMPUp_PF")

#define DE_TR_COLDFLUSHLOOPTIME QLabel::tr("Loop_Time")

// #define DE_TR_HOTFLUSHLOOPTIME QLabel::tr("Loop_Time")
#define DE_TR_HOTFLUSHTARGETTEMP QLabel::tr("TargetTemp")
#define DE_TR_HOTFLUSHREALTEMP QLabel::tr("RealTemp")

#define DE_TR_CHYMISTRYCITRICPREFLUSHTIME QLabel::tr("Preflush_Time")
#define DE_TR_CHYMISTRYCITRICLOOPTIME QLabel::tr("Loop_Time")
#define DE_TR_CHYMISTRYCITRICHOLDTIME QLabel::tr("Hold_Time")
#define DE_TR_CHYMISTRYCITRICBACKFLUSHTIME QLabel::tr("BackFlush_Time")
#define DE_TR_CHYMISTRYCITRICFLUSHFLUX QLabel::tr("Flush_Flux")
#define DE_TR_CHYMISTRYCITRICLOOPFLUX QLabel::tr("Loop_Flux")
#define DE_TR_CHYMISTRYCITRICTARGETTEMP QLabel::tr("Target_Temp")
#define DE_TR_CHYMISTRYCITRICREALTEMP QLabel::tr("Real_Temp")

//调试模块专用#/
#define DE_TR_BLOODPUMP QLabel::tr("Blood Pump")
#define DE_TR_SUBPUMP QLabel::tr("Sub Pump")
#define DE_TR_FUNCTIONPUMP QLabel::tr("Function Pump")  //功能泵#/
#define DE_TR_HEPARINPUMP QLabel::tr("Heparin Pump")
#define DE_TR_PRESSURE QLabel::tr("PRESSURE")
#define DE_TR_AIRBUBBLIES QLabel::tr("Air bubblies")
#define DE_TR_LIQUID QLabel::tr("Liquid")
#define DE_TR_BLOODMONITOR QLabel::tr("BLood monitor")
#define DE_TR_CHOKECLIP QLabel::tr("Choke clip")
#define DE_TR_ALARMSOUND QLabel::tr("Sound light")  //声光#/
#define DE_TR_LIGHT QLabel::tr("Light")
#define DE_TR_BATANDPOWER QLabel::tr("Bat and power")
#define DE_TR_COMMON QLabel::tr("Common")   //公共#/
#define DE_TR_NOWATERTIME QLabel::tr("No water time") //无液时间#/
#define DE_TR_SETFLOW QLabel::tr("Set flow") //设置流量#/

#define DE_TR_NOTREVISEDVALUE   QLabel::tr("Not revised value")    //未修正值#/
#define DE_TR_SENDVALUE    QLabel::tr("Send value")    //发送值#/
#define DE_TR_SHOWVALUE   QLabel::tr("Show value")    //显示值#/

#define DE_TR_BLIQUIDTEMP   QLabel::tr("B Temp")    //B液温度#/
#define DE_TR_MIXTEMP       QLabel::tr("Mix Temp")  //混合温度#/
#define DE_TR_OUTLETTEMP    QLabel::tr("Outlet Temp")    //出水口温度#/
#define DE_TR_AFTERTEMP     QLabel::tr("After Temp")    //透后温度#/
#define DE_TR_HEATERTEMP    QLabel::tr("Heater Temp")  //加热腔温度#/
#define DE_TR_BLIQUIDCOND   QLabel::tr("B Cond")   //B液电导#/
#define DE_TR_MIXCOND       QLabel::tr("Mix Cond")   //混合电导#/
#define DE_TR_OUTLETCOND    QLabel::tr("Outlet Cond")    //出水口电导#/
#define DE_TR_AFTERCOND     QLabel::tr("After Cond")    //透后电导#/
#define DE_TR_DISINFECTCOND     QLabel::tr("Disinfect Cond")    //消毒液电导#/
#define DE_TR_BLOODLEAKCHECK   QLabel::tr("BloodLeak")    //漏血检测#/
#define DE_TR_UFPUMP        QLabel::tr("UF Pump")    //超滤泵#/
#define DE_TR_DIALYSTATEFLOW  QLabel::tr("Dialystate Flow")  //透析液流速#/
#define DE_TR_CIRCULATEPUMP   QLabel::tr("Circulate Pump")    //循环泵#/
#define DE_TR_BEFOREPUMP      QLabel::tr("Before Pump")    //透前泵#/
#define DE_TR_AFTERPUMP       QLabel::tr("After Pump")   //透后泵#/
#define DE_TR_ALIQUIDPUMP     QLabel::tr("A Pump")    //A液泵#/
#define DE_TR_BLIQUIDPUMP     QLabel::tr("B Pump")    //B液泵#/
#define DE_TR_VALVECHECK      QLabel::tr("Valve Check")    //阀门检测#/

#define DE_TR_SETFORMULA     QLabel::tr("Set Formula")    //配方设定#/
#define DE_TR_BEFOREAFTERPUMP QLabel::tr("Front-back Pump")  //透前透后泵#/
#define DE_TR_DEGASSINGPUMP QLabel::tr("Degassing Pump")  //除气泵#/

#define DE_TR_ABPUMP        QLabel::tr("AB Pump")   //AB泵#/
#define DE_TR_TEMPADJUST     QLabel::tr("Temp Adjust")  //温度调试#/
#define DE_TR_CONDADJUST    QLabel::tr("Cond Adjust")    //电导调试#/

#define DE_TR_BLOODDEBUG QLabel::tr("Blood Debug")
#define DE_TR_WATERDEBUG QLabel::tr("Water Debug")
#define DE_TR_USERPARAM QLabel::tr("User Param")
#define DE_TR_PROJECTPARAM QLabel::tr("Project Param")
#define DE_TR_SETUPPARAM QLabel::tr("Setup Param")
#define DE_TR_DRAINOPTION QLabel::tr("Drain Option")
#define DE_TR_TSDEBUG QLabel::tr("Ts Debug")
#define DE_TR_WATERTEST QLabel::tr("Water Test")
#define DE_TR_USETIME QLabel::tr("Use Time")

#define DE_TR_RESETDEFAUL QLabel::tr("Reset Defaults")  //恢复出厂设置#/
#define DE_TR_CONTROLWATERGRAPH QLabel::tr("Control Water Graph")  //可控水路图#/
#define DE_TR_TURNOFF QLabel::tr("Turn Off")     //关机#/

#define DE_TR_BUTTONPRE QLabel::tr("pre")
#define DE_TR_BUTTONNEXT QLabel::tr("next")

//血泵调试部分#/
#define DE_TR_READPARAM QLabel::tr("Read Param")   //读取参数#/
#define DE_TR_BLOODDEBUG QLabel::tr("Blood Debug")  //血路调试#/
#define DE_TR_BLOODPUMPDEBUG QLabel::tr("BloodPump Debug")  //血泵调试#/
#define DE_TR_SUBPUMPDEBUG QLabel::tr("SubPump Debug")  //置换泵调试#/
#define DE_TR_HEPARINPUMPDEBUG QLabel::tr("HeparinPump Debug")  //肝素泵调试#/
#define DE_TR_PRESSUREDEBUG QLabel::tr("Pressure Debug")  //压力调试#/
#define DE_TR_CHECKBUBBLE QLabel::tr("Check Bubble")  //气泡检测#/
#define DE_TR_CHECKCHOKECLIP QLabel::tr("Check Choke Clip")  //检测阻流夹#/
#define DE_TR_SOUNDANDLIGHT QLabel::tr("Sound and light detecting")  //声光检测#/
#define DE_TR_BATTERYSUPPLY QLabel::tr("Battery supply")  //电池电源#/

#define DE_TR_LIQUIDLEVEL QLabel::tr("Liquid Status")  //液位状态#/
#define DE_TR_LIQUIDALARMSTATUS QLabel::tr("Liquid Alarm Status:")  //液位报警状态#/
#define DE_TR_LIQUIDRAISED QLabel::tr("Liquid Raised Key:")  //液位上调键#/
#define DE_TR_LIQUIDREDUCE QLabel::tr("Liquid Reduce Key:")  //液位下调键#/

#define DE_TR_BLOODDETECTION QLabel::tr("Blood Detection")   //血液检测#/
#define DE_TR_ARTERYSTATUS QLabel::tr("Artery Status:")  //动脉检测状态#/
#define DE_TR_VEINSTATUS QLabel::tr("Vein Status:")  //静脉检测状态#/
#define DE_TR_HAVEBLOOD QLabel::tr("Have Blood")  //有血#/
#define DE_TR_NOBLOOD QLabel::tr("No Blood")  //无血#/
#define DE_TR_HAVEBLOODALARM QLabel::tr("If there a bloodleak alarm:")  //是否有漏血报警#/
#define DE_TR_BLOODLEAKLIMIT QLabel::tr("Blood Leak Limit") //漏血报警限值#/
#define DE_TR_SETBLOODLEAKLIMIT QLabel::tr("Set BLood Leak Limit") //设置漏血报警限值#/

#define DE_TR_ARTERYCHOKECLIPSTATUS QLabel::tr("Artery Clip Status:") //动脉阻流夹状态#/
#define DE_TR_VEINCHOKECLIPSTATUS QLabel::tr("Vein Clip Status:") //静脉阻流夹状态#/
#define DE_TR_ONOFFKEYSTATUS QLabel::tr("ON/OFF Key:") //开关按键#/
#define DE_TR_MONITORINGSENSORVALUE QLabel::tr("Monitoring sensor value:") //监测传感器值#/

#define DE_TR_STATEOFSOUND QLabel::tr("State of sound:") //声音状态#/
#define DE_TR_VOLUMESIZE QLabel::tr("Volume:") //音量大小#/
#define DE_TR_MACRON QLabel::tr("Macron") //长音#/
#define DE_TR_MEDIANT QLabel::tr("Mediant") //中音#/
#define DE_TR_TONEPIPS QLabel::tr("Tone pips") //短音#/
#define DE_TR_STATEOFALARMLIGHT QLabel::tr("State of alarmlight:") //报警灯状态#/

#define DE_TR_RED QLabel::tr("Red")  //红#/
#define DE_TR_YELLOW QLabel::tr("Yellow")  //黄#/
#define DE_TR_GREEN QLabel::tr("Green")  //绿#/

#define DE_TR_BATTERYSTATUS QLabel::tr("Battery status:")  //电池状态#/
#define DE_TR_POWERSTATUS QLabel::tr("Power status:")  //电源状态#/
#define DE_TR_BATTERYDISCHARGETEST QLabel::tr("Bat discharge test")  //电池放电测试#/
#define DE_TR_STOPDISCHARGETEST QLabel::tr("Stop discharge test")  //停止放电测试#/
#define DE_TR_THELASTSTOPOFBLOOD QLabel::tr("The last stop of blood pump:")  //上次停血泵时间#/
#define DE_TR_THELASTTIMEOFBATTERY QLabel::tr("The last time of battery discharge:")  //上次电池放电时间#/
#define DE_TR_BLOODPUMPSPEED QLabel::tr("Blood pump speed:")  //血泵速度#/
#define DE_TR_HEPARINPUMPSPEED QLabel::tr("Heparin pump speed:")  //肝素速度#/

#define DE_TR_CHARGING QLabel::tr("Charging")  //充电中#/
#define DE_TR_CHARGECOMPLETE QLabel::tr("Charge complete")  //充电完成#/
#define DE_TR_PREBOOT QLabel::tr("Pre boot")  //预开机#/
#define DE_TR_NORMAL QLabel::tr("Normal")  //正常#/
#define DE_TR_RESERVE QLabel::tr("Reserve")  //后备#/
#define DE_TR_DISCHARGETESTING QLabel::tr("Discharge testing")  //放电测试中#/
#define DE_TR_STOPDISCHARGETESTING QLabel::tr("Stop discharge test")  //停止放电测试#/

#define DE_TR_DOORSTATUS QLabel::tr("Door Status:")  //门盖状态#/
#define DE_TR_STARTKEYSTATUS QLabel::tr("Start Key:")  //启动键#/
#define DE_TR_ADDKEYSTATUS QLabel::tr("+Key:")   //+键
#define DE_TR_SUBKEYSTATUS QLabel::tr("-Key:")
#define DE_TR_UPKEYSTATUS QLabel::tr("Up Key:")   //向上键#/
#define DE_TR_DOWNKEYSTATUS QLabel::tr("Down Key:")   //向下键#/
#define DE_TR_UPALARMSTATUS QLabel::tr("Up Alarm:")   //上限报警#/
#define DE_TR_BLOCKINGUPSENSOR QLabel::tr("Blocking/Up Sensor:")   //阻塞/上限传感器#/
#define DE_TR_DOWNSENSOR QLabel::tr("Down Sensor:")   //下限传感器#/
#define DE_TR_DOWNALARMSTATUS QLabel::tr("Down Alarm:")   //下限报警#/
#define DE_TR_THELENGTHOFSYRINGE QLabel::tr("Length of syringe")   //注射器长度#/
#define DE_TR_CAPACITYOFSYRINGE QLabel::tr("Syringe capacity")   //注射器容量#/
#define DE_TR_OPENSTATUST QLabel::tr("Open")   //打开
#define DE_TR_CLOSESTATUST QLabel::tr("Close")  //关闭
#define DE_TR_CLICKSTATUST QLabel::tr("Press")  //点击
#define DE_TR_RELEASESTATUST QLabel::tr("Release")  //释放
#define DE_TR_CLICKTEXT QLabel::tr("Click")  //点击
#define DE_TR_PUMP QLabel::tr("Pump")  //泵#/
#define DE_TR_SHOULDBESET QLabel::tr("Should be set") //应该设置#/

#define DE_TR_HIGHSPEED  QLabel::tr("HSpeed")  //高速
#define DE_TR_LOWSPEED  QLabel::tr("LSpeed")   //低速
#define DE_TR_BASICPULSE QLabel::tr("Basic Pulse:")  //理论脉冲#/
#define DE_TR_REALPULSE QLabel::tr("Real Pulse:")  //实际脉冲#/
#define DE_TR_STARTTEST QLabel::tr("Start Test")  //开始测试#/
#define DE_TR_STOPTEST QLabel::tr("Stop Test")  //停止测试#/
#define DE_TR_STARTADJUST QLabel::tr("Start Adjust")  //开始自校正#/
#define DE_TR_STOPADJUST QLabel::tr("Stop Adjust")
#define DE_TR_PUMPSPEED QLabel::tr("Speed")  //速度
#define DE_TR_PUMPPIPE QLabel::tr("Pipe")  //管径
#define DE_TR_DEFAULT QLabel::tr("Default")  //默认#/
#define DE_TR_TIME QLabel::tr("Time")  //时间
#define DE_TR_KEYSTART QLabel::tr("Start")  //开始
#define DE_TR_KEYRUN QLabel::tr("Run")
#define DE_TR_KEYSTOP QLabel::tr("Stop")
#define DE_TR_CAVITYVALUE QLabel::tr("Cavity Value")  //腔数#/
#define DE_TR_CAVITYCAPACITY QLabel::tr("Cavity Capacity")  //腔量#/

#define DE_TR_SPEEDSELECT QLabel::tr("Speed Select")  //速度选择#/
#define DE_TR_LOW QLabel::tr("Low")  //低#/
#define DE_TR_MIDDLE QLabel::tr("Mid")  //中#/
#define DE_TR_HIGH QLabel::tr("High")  //高#/
#define DE_TR_OTHERUSE QLabel::tr("Other Use")  //其他操作#/

#define DE_TR_BASICVALUE QLabel::tr("Basic Value")  //理论量
#define DE_TR_REALVALUE QLabel::tr("Real Value")  //实际量
#define DE_TR_KEYADJUST QLabel::tr("Adjust")   //校正
#define DE_TR_KEYSEND QLabel::tr("Send")   //发送#/
#define DE_TR_SENDOK QLabel::tr("Send Ok") //发送完毕#/

#define DE_TR_RAPIDUP QLabel::tr("Rapid Up")   //快速上推#/
#define DE_TR_RAPIDDOWN QLabel::tr("Rapid Down")   //快速下推#/

#define DE_TR_RUNINGSTATUS QLabel::tr("Runing status:")  //运行状态
#define DE_TR_FIRSTDOSE QLabel::tr("First Dose") //首剂量#/
#define DE_TR_APCURRENTVALUE QLabel::tr("AP Current Value:")  //动脉压当前值#/
#define DE_TR_VPCURRENTVALUE QLabel::tr("VP Current Value:")  //静脉压当前值#/
#define DE_TR_TMPCURRENTVALUE QLabel::tr("TMP Current Value:")  //跨膜压当前值#/
#define DE_TR_WPCURRENTVALUE QLabel::tr("WP Current Value:")  //水路压当前值#/
#define DE_TR_DEGASSINGPRESSURE QLabel::tr("Degassing Pressure:")  //排气压当前值#/
#define DE_TR_PSNPRESSURE QLabel::tr("PSN Pressure:")  //PSN压当前值#/
#define DE_TR_DEGASSPOWERVALUE QLabel::tr("Degassing Value:")  //除气泵值#/
#define DE_TR_IFTHEREISABUBBLEALARM QLabel::tr("If there is a bubble alarm:")   //是否有气泡报警#/
#define DE_TR_TINYBUBBLESACCUMULATION QLabel::tr("Tiny bubbles accumulation limit:") //微小气泡累积极限#/
#define DE_TR_SETACCUMULATIONLIMIT QLabel::tr("Set accumulation limit:") //设置累积极限#/
#define DE_TR_BUBBLEPULSEWIDTHALARM QLabel::tr("Bubble pulse width alarm threshold:") //气泡脉宽报警限值#/
#define DE_TR_SETBUBBLEPULSEWIDTHALARM QLabel::tr("Set pulse width alarm threshold:") //设置脉宽报警限值#/

#define DE_TR_PARAMREADING QLabel::tr("Param reading,please wait!")    //参数正在读取，请稍后#/
#define DE_TR_READPARAMFAIL QLabel::tr("Read param fail!")    //参数读取失败#/
#define DE_TR_READPARAMSUCESS QLabel::tr("Read param sucess!")    //参数读取成功#/
#define DE_TR_HASNOTBEENREAD QLabel::tr("Has not been read parameters, please click to Read Param!")    //尚未读取参数#/
#define DE_TR_SENDPARAMFINISH QLabel::tr("Send param finish!")    //参数发送完毕#/
#define DE_TR_SENDPARAMFAIL QLabel::tr("Send param fail !")    //参数发送失败#/
#define DE_TR_THEWRITEIDNUMBER QLabel::tr("The write id number cannot be less than 10 !")  //写入ID号不能小于10#/
#define DE_TR_IDCANNOTLESS10 QLabel::tr("Id number cannot select be less than 10 !")  //不能选择小于10的ID号#/
#define DE_TR_PLEASEWRITEPATIENTFIRST QLabel::tr("Please write patient information first !")  //请先写入病人信息#/

#define DE_TR_READINFORMATIONSUCESS QLabel::tr("Read information sucess!")    //读取信息成功#/
#define DE_TR_WRITEINFORMATIONSUCESS QLabel::tr("Write information sucess!")    //写入信息成功#/
#define DE_TR_NOTTHISIDNUM QLabel::tr("Not this patient idnum! ")    //不存在这个病人ID号#/

#define DE_TR_PUMPRUNINGSTATUS QLabel::tr("Pump Runing")  //泵运行中#/
#define DE_TR_PUMPSTOPSTATUS QLabel::tr("Pump Stop")  //泵停止#/
#define DE_TR_PUMPADJUSTSTATUS QLabel::tr("Pump Adjust")  //泵校正中#/
#define DE_TR_TESTRUNING QLabel::tr("Test runing")  //测试进行中#/
#define DE_TR_TESTSTOP QLabel::tr("Test stop")  //测试终止#/
#define DE_TR_TESTFINISH QLabel::tr("Test finish")  //测试完成#/

#define DE_TR_APUMPTESTINGSTOP QLabel::tr("A Pump testing,please stop !")    //A液泵测试中，请先停止#/
#define DE_TR_BPUMPTESTINGSTOP QLabel::tr("B Pump testing,please stop !")    //B液泵测试中，请先停止#/

#define DE_TR_RUNNINGSTATEOFTHEWATER QLabel::tr("Running state of the water")  //水路运行状态#/
#define DE_TR_THESTATEOFBLOODLEADCHECK QLabel::tr("The state of Bloodleadcheck")  //漏血检测状态#/

#define DE_TR_BTEMPFEEDBACK QLabel::tr("B Temp Feedback")  //B液温度反馈值#/
#define DE_TR_SETBTEMPFEEDBACK QLabel::tr("Set B Temp Feedback") //B液反馈设置值#/

#define DE_TR_MIXTEMPFEEDBACK QLabel::tr("Mix Temp Feedback")  //混合温度反馈值#/
#define DE_TR_SETMIXTEMPFEEDBACK QLabel::tr("Set Mix Temp Feedback") //混合温度反馈设置值#/

#define DE_TR_MIXCONDFEEDBACK QLabel::tr("Mix Cond Feedback")  //混合电导反馈值#/
#define DE_TR_SETMIXCONDFEEDBACK QLabel::tr("Set Mix Cond Feedback") //混合电导反馈设置值#/

#define DE_TR_OUTLETTEMPFEEDBACK QLabel::tr("Outlet Temp Feedback")  //出水口温度反馈值#/
#define DE_TR_SETOUTLETTEMPFEEDBACK QLabel::tr("Set Outlet Temp Feedback") //出水口温度反馈设置值#/

#define DE_TR_OUTLETCONDFEEDBACK QLabel::tr("Outlet Cond Feedback")  //出水口电导反馈值#/
#define DE_TR_SETOUTLETCONDFEEDBACK QLabel::tr("Set Outlet Cond Feedback") //出水口电导反馈设置值#/

#define DE_TR_AFTERTEMPFEEDBACK QLabel::tr("After Temp Feedback")  //透后温度反馈值#/
#define DE_TR_SETAFTERTEMPFEEDBACK QLabel::tr("Set After Temp Feedback") //透后温度反馈设置值#/

#define DE_TR_AFTERCONDFEEDBACK QLabel::tr("After Cond Feedback")  //透后电导反馈值#/
#define DE_TR_SETAFTERCONDFEEDBACK QLabel::tr("Set After Cond Feedback") //透后电导反馈设置值#/

#define DE_TR_DISINFECTCONDFEEDBACK QLabel::tr("Disinfect Cond Feedback")  //消毒液电导反馈值#/
#define DE_TR_SETDISINFECTCONDFEEDBACK QLabel::tr("Set Disinfect Cond Feedback") //消毒液电导反馈设置值#/

#define DE_TR_FEEDBACK QLabel::tr("Feedback")  //反馈值#/
#define DE_TR_CURRENTFEEDBACK QLabel::tr("Current feedback")  //当前反馈值#/
#define DE_TR_FEEDBACKALARMTHRESHOLD QLabel::tr("Feedback alarm threshold")  //反馈报警阀值#/
#define DE_TR_SETALARMTHRESHOLD QLabel::tr("Set alarm threshold")  //报警阀值设置#/
#define DE_TR_FEEDBACK100CAVITY QLabel::tr("Feedback 100 Cavity")  //反馈100腔体值#/
#define DE_TR_UPPERLIMIT QLabel::tr("Alarm upper limit")  //报警上限值#/
#define DE_TR_SETFEEDBACK QLabel::tr("Set  Feedback") //设置反馈值#/

#define DE_TR_HEATERTEMPFEEDBACK QLabel::tr("Heater Temp Feedback")  //加热腔温度反馈值#/
#define DE_TR_SETHEATERTEMPFEEDBACK QLabel::tr("Set Heater Temp Feedback") //加热腔温度反馈设置值#/

#define DE_TR_BCONDFEEDBACK QLabel::tr("B Cond Feedback")  //B液电导反馈值#/
#define DE_TR_SETBCONDFEEDBACK QLabel::tr("Set B Cond Feedback") //B液电导反馈设置值#/

//血泵信息#/
#define DE_TR_TEXTNOTEREAD QLabel::tr("Note: please click to read the parameters before debugging.")  //注意:调试前请先点击读取参数#/
#define DE_TR_TEXTSELECTHIGHORLOW01 QLabel::tr("1.Select high speed or low speed, \
click start test,read the pulse.")  //1.选择高速或者低速，点击开始测试，读取脉冲.#/
#define DE_TR_TEXTIFTHETHEORY02 QLabel::tr("2.If the theory of pulse and pulse actual gap is too big, \
then click start since the correction.")  //2.如果理论脉冲和实际脉冲差距过大，则点击开始自校正.#/
#define DE_TR_TEXTPREPAREAMEASURING03 QLabel::tr("3.Prepare a measuring cup, make sure good pipe \
diameter and time,click start.")  //3.准备好量杯，确定好管径和时间，点击开始.#/
#define DE_TR_TEXTTHEMEASURINGCUP04 QLabel::tr("4.The measuring cup measure when feedback input, \
and click the correction.")  //4.将量杯的量当反馈量输入，并点击校正.#/

//肝素泵信息#/
#define DE_TR_TEXTOBSERVETHEHEPARIN01 QLabel::tr("1.Observe the heparin pump button \
status and alarm information is correct.")  //1.观察肝素泵的按键状态和报警信息是否正确.#/
#define DE_TR_TEXTACCORDINGTO02 QLabel::tr("2.According to the actual situation, \
set up syringes length and capacity.")  //2.根据实际情况设置注射器长度和容量.#/
#define DE_TR_TEXTPREPAREAMAKESURESPEEDANDTIME03 QLabel::tr("3.Prepare a measuring cup, make sure \
good speed and time, click start, and input the actual measured values, click send.")  //3.准备好量杯，确定好速度和时间，点击启动，输入实际测量值，点击发送进行校正.#/
#define DE_TR_TEXTDETERMINETHEGOOD04 QLabel::tr("4.Determine the good starting dose, \
click start, and input the actual measured values, click send.")  //4.确定好首剂量，点击启动，输入实际测量值，点击发送进行校正.#/

//压力调试信息#/
#define DE_TR_THEPRESSUREVALUE01 QLabel::tr("1.The pressure value, based \
on the current equipment real-time pressure, dynamic update the display.") //1.各压力值，根据当前设备实时压力情况，动态更新显示。#/

#endif


//                                原packet_ui.h文件    UI变量 当需要和底层设备进行canopen通信时则需要将相应的结构变量内容读取到packet_can.h中对应的结构变量中
#ifndef PACKET_UI_H
#define PACKET_UI_H

enum ScaleSelectWork//选择卡标方式
{
    treatment_start_to_scale ,//自动卡标
    treatment_stop_to_scale ,//卡标范围设为最大
    treatment_ready_to_scale //准备状态卡标
};

enum DisinfectWorkModel
{
    noChooseDisinfectWorkModel = -1,

    cold_rinse_B = 0,  //冷冲洗
    hot_rinse_B = 1,   //热冲

    x_peracetic_B = 2,  //化(过氧乙酸)
    x_sodiumHypochlorite_B = 3,    //化(次氯酸钠)
    x_citricAcid_B = 4,     //化(柠檬酸)

    h_water_B = 5,         //热(纯水)
    h_citricAcid_B = 6    //热(柠檬酸)
};

enum FluidSupplyModel
{
    A_and_B = 0,  //A＋B
    A_and_BPower = 1, //A+B干粉
    Acetate = 2,  //醋酸盐
    Center = 3   //中央供A液
};

enum JudgeAlarmState //判断报警状态
{
    NoAlarm= 0,  //没有报警
    HappenAlarm = 1//产生报警
};

struct PacketSceneSw {
    int scene;
};

struct PacketLang {

};

struct PacketAP {
    int ap;         /* mmHg */
    int apUp;
    int apL;
};

struct PacketVP {
    int vp;         /* mmHg */
    int vpUp;
    int vpL;
};

struct PacketTMP {
    int tmp;        /* mmHg */
    int tmpUp;
    int tmpL;
};

struct PacketCond {
    float cond;     /* mS/cm. */
    float condUp;
    float condL;
};

struct PacketUF {
    int ufVolAim;   /* ml. 目标量 */
    int ufVolNow;   /* ml. */
    int ufT;        /* minute. */
    int ufRate;     /* ml/h  治疗时超滤泵的速度*/
    int preRate;    /*ml/h 预冲超滤泵的速度*/
    bool chooseRate; /*if false select preflush rate; if true select treat uf rate*/
};

struct PacketISO {
    int isoUfVolAim;   /* ml. */
    int isoUfVolNow;   /* ml. */
    int isoUfTime;        /* minute. */
};

struct PacketFluid {/*透析液*/
    unsigned short int tempTarget; /*目标温度*/
    unsigned short int tempNow; /*实际温度*/
    unsigned short int Wspeed; /*透析液流速*/
};

enum CheckItem/* Test-self */
{
    CHECK_HEMO_PUMP,
    CHECK_SUPPLY_PUMP,
    CHECK_HEPARIN_PUMP,
    CHECK_VENOUS_PRE,
    CHECK_ARTERIAL_PRE,
    CHECK_TRB_PRE,
    CHECK_LIQUID_LEVEL,
    CHECK_BUBBLES,
    CHECK_WATER_TEMP,
    CHECK_WATER_COND,

    /* ... */
    CHECK_NUM
};

enum CheckItemState
{
    CHECK_ITEM_CHECK_NO,
    CHECK_ITEM_CHECK_PRE,
    CHECK_ITEM_CHECK_DOING,
    CHECK_ITEM_CHECK_WELL,
    CHECK_ITEM_CHECK_FAILED,
    CHECK_ITEM_CHECK_NULL
};

struct PacketCheckItem {
    int checkIs;
    CheckItemState checkState[CHECK_NUM];
};

enum PreflushModel
{
    auto_model = 0,  //
    manual_model = 1, //
    online_model =2
};

struct PacketSet{
    int before;
    int current;
    int target;
};

enum WorkModeFlag
{
    init_state,
    seting_state,
    setok_state,
    runing_state
};

struct PacketWorkMode {//工作模式页面
    char modeNow[16];
    char modeDest[16];
    int dest;
    int src;
    int flag;            //标记：
    PacketSet treatmode; //治疗方式
    PacketSet submode; //子治疗方式
    PacketSet fluidmode;//供液方式
};

struct PacketNaCurveValue{
    float value1;
    float value2;
    float value3;
    float value4;
    float value5;
    float value6;
    float value7;
    float value8;
    float value9;
    float value10;
};

struct PacketCurveUF11to30{
    int curveNumberValue11[10];
    int curveNumberValue12[10];
    int curveNumberValue13[10];
    int curveNumberValue14[10];
    int curveNumberValue15[10];
    int curveNumberValue16[10];
    int curveNumberValue17[10];
    int curveNumberValue18[10];
    int curveNumberValue19[10];
    int curveNumberValue20[10];
    int curveNumberValue21[10];
    int curveNumberValue22[10];
    int curveNumberValue23[10];
    int curveNumberValue24[10];
    int curveNumberValue25[10];
    int curveNumberValue26[10];
    int curveNumberValue27[10];
    int curveNumberValue28[10];
    int curveNumberValue29[10];
    int curveNumberValue30[10];
};

enum Treatmodel
{
    HD_double = 0,   //
    HD_SNDP ,  //
    HD_SNSP ,  //
    HDF_pre_dilute ,  //前置换 透析滤过
    HDF_post_dilute ,  //后置换 透析滤过
    HF_pre_dilute ,  //
    HF_post_dilute,  //
    ISOUF_DNDP ,  //
    ISOUF_SNDP ,  //
    ISOUF_SNSP ,  //
    PE ,  //
    MODELNUM
};

struct FeedbackState {
    int dest;
    int src;
};

struct State{
    int current;
    int previous;
};

struct WorkState {
    State tipState;  //总状态
    State subState;//子状态
    State waterState;  //水路状态
    State bloodState;  //血路状态
    unsigned char workMode;//治疗模式
    bool   disinfectEnable;//true 可以进入消毒界面，false不能进入消毒界面
    int tip;//总状态
    int current;  //当前状态
    int previous; //上一个状态
    //FeedbackState waterState;
};

enum WorkStateTable
{
    selfcheck,  //自检
    preflush_run ,   //预冲
    AutoPreflush_run ,   //自动预冲
    preflush_stop,  //
    HD_double_needle_run,   //双针透析
    HD_double_needle_stop,   //
    HDF_double_needle_run,   //双针干粉
    HDF_double_needle_stop,   //
    ISOUF_double_needle_run,   //双针干超
    ISOUF_double_needle_stop,

    cool_rinse_run,   //冷冲洗
    cool_rinse_stop,
    hot_rinse_run,   //热冲洗
    hot_rinse_stop,   //

    chemical_Disinfect_run,   //化学消毒
    chemical_Disinfect_stop,   //

    hot_chemical_Disinfect_run,   //热化学消毒
    hot_chemical_Disinfect_stop,   //
    hot_Disinfect_run,  //纯热消毒
    hot_Disinfect_stop,

    Disinfect_run,//总状态
    Disinfect_stop,//总状态
    Disinfect_finish,//总状态
    Cure_run,//总状态
    Cure_pause,//总状态
    Cure_bloodreturn_run,//回血
    Cure_bloodreturn_stop,//回血
    Cure_stop//总状态
};

enum DisinfectStateTable//消毒步骤
{
    reverse = -1,  //
    preflush = 0x01,   //前冲洗
    suck =0x02,        //吸消毒液
    hold = 0x03,       //滞留
    loop = 0x04,       //循环消毒
    backflush = 0x05,  //后冲洗

    coldrinse = 0x11,   //独立冷冲洗
    hotrinse1 = 0x12,   //独立热冲洗（阶段1）
    hotrinse2 = 0x13,   //独立热冲洗（阶段2）
    hotrinse3 = 0x14,   //独立热冲洗（阶段3）
    dry = 0x15,         //清洁后排空
    flushpipe = 0x16    //冲洗进水管

};

struct PrimeBPRate {
    int ph1;
    int ph2;
    int ph3;
};

struct PacketPreflush
{
    enum PreflushModel PreflushModel;
    unsigned short int PreflushManualUF_Rate;/*手动预冲超滤率*/
    unsigned short int PreflushManualBloodRate; /*手动预冲血泵速率*/

    PrimeBPRate BPRate; /*预冲血泵速度分段*/
    unsigned short int PreflushBloodSpeed;/*预冲血泵速率,只有预冲启动后，设置保存，才下发*/
    unsigned short int PreflushTime; /*预冲时间*/
    unsigned short int PreflushUfRate; /*预冲超滤速度*/
    unsigned short int PreflushUfVolume; /*预冲超滤量*/
    unsigned short int TractBloodVolume; /*引血量*/
    unsigned short int PreflushBpVolume;/*已预冲量*/
    unsigned short int step;
//    bool               model;
};

enum TreatTimer_state
{
    TimeIdle = 0,
    Timestart = 1,  //
    Timepause = 2, //
    Timestop = 3 ,//
    Timechange=4
};

struct Params_Disinfect {//消毒参数
    int preFlush_Time;//前冲时间 （单位分钟）
    int hold_Time;//滞留时间
    int loop_Time;//循环消毒时间
    int backFlush_Time;//后冲时间
    int flush_flux;//冲洗时流量
    int loop_flux;//循环时流量
    int targetTemp;//消毒液目标温度
    int runTemp;//走时温度
    int cond_up;//电导上限
    int cond_down;//电导下限
    int suck_Volume;//消毒液吸入量
    int step;//当前步
};

enum language_table
{
    English = 0,
    Chinese = 1 //

};

struct DestStatus{
    unsigned char header_status; //总状态 四种状态：准备、 治疗、结束治疗、消毒
    unsigned char blood_status; //血路状态
    unsigned char water_status; //水路状态
    unsigned char work_model;  //工作模式 运用在治疗状态下的多种治疗模式
};

struct PacketBicarbonate{
    unsigned short int time;
    unsigned short int  value_hco3;
};

struct PacketBPMParam {//BPM?参数
    unsigned short int pulse;
    unsigned short int DIA_SYS;
    unsigned short int patient_position;
    unsigned short int measure_interval;
    unsigned short int systolic_upper;//收缩压上限#/
    unsigned short int systolic_lower;//收缩压下限#/
    unsigned short int diastolic_upper;//舒张压上限#/
    unsigned short int diastolic_lower;//舒张压下限#/
    unsigned short int Max_pulse;//脉搏上限#/
    unsigned short int Min_pulse;
    unsigned short int presel_pressure;
    unsigned short int time_hour;
    unsigned short int time_min;
};

struct DisinfectCond{
    unsigned short int Mix_cond;
    unsigned short int B_cond;
    unsigned short int out_cond;
};

#endif
