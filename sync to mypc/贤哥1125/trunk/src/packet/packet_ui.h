#ifndef PACKET_UI_H
#define PACKET_UI_H

//这个文件是界面程序对外接口应用到的所有结构变量
//界面程序的设置保存是通过修改这个文件的变量内容

//当需要和底层设备进行canopen通信时，则需要将相应的结构变量内容
//读取到packet_can.h中对应的结构变量中

enum ScaleSelectWork
{//选择卡标方式
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

enum JudgeAlarmState //
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

/*透析液*/
struct PacketFluid {
    unsigned short int tempTarget; /*目标温度*/
    unsigned short int tempNow; /*实际温度*/
    unsigned short int Wspeed; /*透析液流速*/
};

/* Test-self */
enum CheckItem
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

//工作模式页面对应的结构体
struct PacketWorkMode {
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

struct Params_Disinfect {

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

struct PacketBPMParam {
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
