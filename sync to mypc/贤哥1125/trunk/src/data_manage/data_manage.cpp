#include "data_manage.h"
#include "global_var.h"


#if HJX_COMMUNICATION_SWITCH

DataManage::DataManage(QObject *parent) :
    QObject(parent),
    dataCan(parent)
{
    //清空QHash列表 hashParameter#/
    dataHashInit();
    //建立数据库链接#/
    DataSql_Open();
    //初始化所有设备节点的数据结构和数据管理类#/
    dataCan.Init();

    /* tmp to put here */
    //下面是创建具体和底层设备canopen通信的数据结构对应的管理类DataCustom#/
    //并且并没有为其分配存储数据空间#/
    //目的是为了，在对每个管理类添加对应的操作处理后#/
    //可以根据关键字名称调用与底层的具体通信操作#/
    dataNodeAppend(PACKET_NAME_WORKMODE, NULL, sizeof(PacketWorkMode));
    dataNodeAppend(PACKET_NAME_SCENESWITCH, NULL, sizeof(PacketSceneSw));
    dataNodeAppend(PACKET_NAME_AP, NULL, sizeof(PacketAP));
    dataNodeAppend(PACKET_NAME_SCALE_AP, NULL, sizeof(int));
    dataNodeAppend(PACKET_NAME_UPDATE_AP, NULL, sizeof(int));
    dataNodeAppend(PACKET_NAME_VP, NULL, sizeof(PacketVP));
    dataNodeAppend(PACKET_NAME_SCALE_VP, NULL, sizeof(int));
    dataNodeAppend(PACKET_NAME_UPDATE_VP, NULL, sizeof(int));
    dataNodeAppend(PACKET_NAME_TMP, NULL, sizeof(PacketTMP));
    dataNodeAppend(PACKET_NAME_SCALE_TMP, NULL, sizeof(int));
    dataNodeAppend(PACKET_NAME_UPDATE_TMP, NULL, sizeof(int));
    dataNodeAppend(PACKET_NAME_COND, NULL, sizeof(PacketCond));
    dataNodeAppend(PACKET_NAME_UF, NULL, sizeof(PacketUF));
    dataNodeAppend(PACKET_NAME_SELFTEST, NULL, sizeof(PacketCheckItem));

    dataNodeAppend(PACKET_NAME_DIALYSATE, NULL, sizeof(PacketFluid));
    dataNodeAppend(PACKET_SDO_BLOOD, NULL, sizeof(PacketSdoBlood));
    dataNodeAppend(PACKET_SDO_HEPARIN, NULL, sizeof(PacketSdoHeparin));
    dataNodeAppend(PACKET_SDO_RANGETEMP, NULL, sizeof(PacketSdoRangeTemp));
    dataNodeAppend(Disinfect_Work_Model, NULL, sizeof(DisinfectWorkModel));
    dataNodeAppend(PACKET_NAME_HEPARINPUMP,NULL,sizeof(PacketHeparinPump));
    dataNodeAppend(PACKET_PDO_HEPARINPUMP, NULL, sizeof(PacketPDOHeparinPump));

    //na curve value#/
    dataNodeAppend(PACKET_NA_CURVE_VALUE, NULL, sizeof(PacketNaCurveValue));

    //curve uf 11to30#/
    dataNodeAppend(PACKET_CURVE_UF_11TO30, NULL, sizeof(PacketCurveUF11to30));

    //data_set#/
    dataNodeAppend(PACKET_FORMULA_TARGET,NULL,sizeof(struct FormulaSet));

    //alarm btn#/
    dataNodeAppend(PACKET_NAME_BTNALARM, NULL, sizeof(unsigned int));
    dataNodeAppend(PACKET_NAME_MESSAGES,NULL,sizeof(int));

    dataNodeAppend(ALARMPRO, NULL, sizeof(int));

    dataNodeAppend(PACKET_BP_WIDTH, NULL, sizeof(unsigned char));

    //maintain return#/
    dataNodeAppend(PACKET_MAINTAIN_RETURN, NULL, sizeof(bool));

    //maintain params
    dataNodeAppend(PACKET_MAINTAIN_PARAMS, NULL, sizeof(struct Params));
    dataNodeAppend(PACKET_PARAMS_UF, NULL, sizeof(int));
    dataNodeAppend(PACKET_NAME_NOW_UF_VOLUME, NULL, sizeof(int));
    dataNodeAppend(PACKET_ENGINEER, NULL, sizeof(struct Engineer));

    dataNodeAppend(Preflush_Model, NULL, sizeof(PreflushModel));

    dataNodeAppend(PACKET_PREFLUSH, NULL, sizeof(PacketPreflush));

//    dataNodeAppend(PACKET_NAME_BLOODPUMP, NULL, sizeof(PacketBloodPump));

//    dataNodeAppend(PACKET_NAME_BLOODPUMPTUBE, NULL, sizeof(PacketBloodPumpTube));

    dataNodeAppend(PACKET_PDO_SUBBLOODPUMP, NULL, sizeof(PacketSubBloodPump));

    dataNodeAppend(SelfCheck_Timer, NULL, sizeof(bool));

    dataNodeAppend(TREAT_TIMER, NULL, sizeof(int));

    dataNodeAppend(DE_STR_SAVECUREPARAM,NULL,sizeof(int));

    dataNodeAppend(PACKET_NAME_WORKSTATE, NULL, sizeof(WorkState));

    dataNodeAppend(PACKET_NAME_ADJPRESSLIMIT, NULL, sizeof(int));

    dataNodeAppend(PACKET_NAME_N1STATE, NULL, sizeof(PacketN1State));

    dataNodeAppend(PACKET_RESUME_ALARM, NULL, sizeof(int));

    dataNodeAppend(PACKET_NAME_BLOODVOLUME, NULL, sizeof(int));

    dataNodeAppend(OWNNOTE, NULL, sizeof(PacketOwnNote));

 //   dataNodeAppend(PACKET_NAME_HEPARINDATA, NULL, sizeof(PackN15PDO));

    dataNodeAppend(PACKET_NAME_BLOODTEMP, NULL, sizeof(int));

 //   dataNodeAppend(PACKET_NAME_POWER, NULL,sizeof(PacketPower) );

    dataNodeAppend(PACKET_BMPRESULT, NULL,sizeof(bpmResult));

    dataNodeAppend(PACKET_RESETPARAMETERS,NULL,sizeof(int));

    dataNodeAppend(ALARM_SILENCE_SET,NULL,sizeof(int));

    dataNodeAppend(BLOOD_CHANGE_PREFLUSH,NULL,sizeof(PacketSdoBlood));

    dataNodeAppend(PACKET_REBIRTH_HANDLE,NULL,sizeof(int)); //重生操作处理#/
    dataNodeAppend(PACKET_REBIRTH_START,NULL,sizeof(int));  //重生启动开始#/

    //主要是为了update 语言的xml通信用途，，非can用途#/
    dataNodeAppend(UPLANGUAGEXML, NULL, sizeof(int));

    //开启canopen的工作#/
    CanopenOpen();

    //具体对上面的数据包管理类添加相应的操作处理#/
#if HJX_COMMUNICATION_SWITCH
    dataCanSend->init();
#else
    dataCanSend.init();
#endif

}

//停止canopen工作#/
DataManage::~DataManage()
{
    /* tmp to put here */
    CanopenClose();
    dataHashQuit();
    DataSql_Close();
}

//开启canopen工作#/
void DataManage::CanopenOpen()
{
    dataCan.Open();
}

//关闭canopen的工作#/
void DataManage::CanopenClose()
{
    dataCan.Quit();
}

#else
DataManage::DataManage(QObject *parent) :
    QObject(parent),
    dataCan(parent),
    dataCanSend(parent)
{
    //清空QHash列表 hashParameter#/
    dataHashInit();
    //建立数据库链接#/
    DataSql_Open();
    //初始化所有设备节点的数据结构和数据管理类#/
    dataCan.Init();

    /* tmp to put here */
    //下面是创建具体和底层设备canopen通信的数据结构对应的管理类DataCustom#/
    //并且并没有为其分配存储数据空间#/
    //目的是为了，在对每个管理类添加对应的操作处理后#/
    //可以根据关键字名称调用与底层的具体通信操作#/
    dataNodeAppend(PACKET_NAME_WORKMODE, NULL, sizeof(PacketWorkMode));
    dataNodeAppend(PACKET_NAME_SCENESWITCH, NULL, sizeof(PacketSceneSw));
    dataNodeAppend(PACKET_NAME_AP, NULL, sizeof(PacketAP));
    dataNodeAppend(PACKET_NAME_SCALE_AP, NULL, sizeof(int));
    dataNodeAppend(PACKET_NAME_UPDATE_AP, NULL, sizeof(int));
    dataNodeAppend(PACKET_NAME_VP, NULL, sizeof(PacketVP));
    dataNodeAppend(PACKET_NAME_SCALE_VP, NULL, sizeof(int));
    dataNodeAppend(PACKET_NAME_UPDATE_VP, NULL, sizeof(int));
    dataNodeAppend(PACKET_NAME_TMP, NULL, sizeof(PacketTMP));
    dataNodeAppend(PACKET_NAME_SCALE_TMP, NULL, sizeof(int));
    dataNodeAppend(PACKET_NAME_UPDATE_TMP, NULL, sizeof(int));
    dataNodeAppend(PACKET_NAME_COND, NULL, sizeof(PacketCond));
    dataNodeAppend(PACKET_NAME_UF, NULL, sizeof(PacketUF));
    dataNodeAppend(PACKET_NAME_SELFTEST, NULL, sizeof(PacketCheckItem));

    dataNodeAppend(PACKET_NAME_DIALYSATE, NULL, sizeof(PacketFluid));
    dataNodeAppend(PACKET_SDO_BLOOD, NULL, sizeof(PacketSdoBlood));
    dataNodeAppend(PACKET_SDO_HEPARIN, NULL, sizeof(PacketSdoHeparin));
    dataNodeAppend(PACKET_SDO_RANGETEMP, NULL, sizeof(PacketSdoRangeTemp));
    dataNodeAppend(Disinfect_Work_Model, NULL, sizeof(DisinfectWorkModel));
    dataNodeAppend(PACKET_NAME_HEPARINPUMP,NULL,sizeof(PacketHeparinPump));
    dataNodeAppend(PACKET_PDO_HEPARINPUMP, NULL, sizeof(PacketPDOHeparinPump));

    //na curve value#/
    dataNodeAppend(PACKET_NA_CURVE_VALUE, NULL, sizeof(PacketNaCurveValue));

    //curve uf 11to30#/
    dataNodeAppend(PACKET_CURVE_UF_11TO30, NULL, sizeof(PacketCurveUF11to30));

    //data_set#/
    dataNodeAppend(PACKET_FORMULA_TARGET,NULL,sizeof(struct FormulaSet));

    //alarm btn#/
    dataNodeAppend(PACKET_NAME_BTNALARM, NULL, sizeof(unsigned int));
    dataNodeAppend(PACKET_NAME_MESSAGES,NULL,sizeof(int));

    dataNodeAppend(ALARMPRO, NULL, sizeof(int));

    dataNodeAppend(PACKET_BP_WIDTH, NULL, sizeof(unsigned char));

    //maintain return#/
    dataNodeAppend(PACKET_MAINTAIN_RETURN, NULL, sizeof(bool));

    //maintain params
    dataNodeAppend(PACKET_MAINTAIN_PARAMS, NULL, sizeof(struct Params));
    dataNodeAppend(PACKET_PARAMS_UF, NULL, sizeof(int));
    dataNodeAppend(PACKET_NAME_NOW_UF_VOLUME, NULL, sizeof(int));
    dataNodeAppend(PACKET_ENGINEER, NULL, sizeof(struct Engineer));

    dataNodeAppend(Preflush_Model, NULL, sizeof(PreflushModel));

    dataNodeAppend(PACKET_PREFLUSH, NULL, sizeof(PacketPreflush));

    dataNodeAppend(PACKET_NAME_BLOODPUMP, NULL, sizeof(PacketBloodPump));

    dataNodeAppend(PACKET_NAME_BLOODPUMPTUBE, NULL, sizeof(PacketBloodPumpTube));

    dataNodeAppend(PACKET_PDO_SUBBLOODPUMP, NULL, sizeof(PacketSubBloodPump));

    dataNodeAppend(SelfCheck_Timer, NULL, sizeof(bool));

    dataNodeAppend(TREAT_TIMER, NULL, sizeof(int));

    dataNodeAppend(PACKET_NAME_WORKSTATE, NULL, sizeof(WorkState));

    dataNodeAppend(PACKET_NAME_ADJPRESSLIMIT, NULL, sizeof(int));

    dataNodeAppend(PACKET_NAME_N1STATE, NULL, sizeof(PacketN1State));

    dataNodeAppend(PACKET_RESUME_ALARM, NULL, sizeof(int));

    dataNodeAppend(PACKET_NAME_BLOODVOLUME, NULL, sizeof(int));

    dataNodeAppend(OWNNOTE, NULL, sizeof(PacketOwnNote));

    dataNodeAppend(PACKET_NAME_HEPARINDATA, NULL, sizeof(PackN15PDO));

    dataNodeAppend(PACKET_NAME_BLOODTEMP, NULL, sizeof(int));

    dataNodeAppend(PACKET_NAME_POWER, NULL,sizeof(PacketPower) );

    dataNodeAppend(PACKET_BMPRESULT, NULL,sizeof(bpmResult));

    dataNodeAppend(PACKET_RESETPARAMETERS,NULL,sizeof(int));

    dataNodeAppend(ALARM_SILENCE_SET,NULL,sizeof(int));

    //主要是为了update 语言的xml通信用途，，非can用途#/
    dataNodeAppend(UPLANGUAGEXML, NULL, sizeof(int));

    //开启canopen的工作#/
    CanopenOpen();

    //具体对上面的数据包管理类添加相应的操作处理#/
    dataCanSend.init();

}

//停止canopen工作#/
DataManage::~DataManage()
{
    /* tmp to put here */
    CanopenClose();
    dataHashQuit();
    DataSql_Close();
}

//开启canopen工作#/
void DataManage::CanopenOpen()
{
    dataCan.Open();
}

//关闭canopen的工作#/
void DataManage::CanopenClose()
{
    dataCan.Quit();
}
#endif
