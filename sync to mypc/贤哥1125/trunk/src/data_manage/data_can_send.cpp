#include "data_can_send.h"
#include "common_func.h"
#include "global_var.h"
#include "../debug_mode/scene_controlwater_graph/scene_controlwater_graph.h"


#define DE_MUTEX_TRYLOCK_OPEN   0     //使用QMutex的方式 0：默认QMutex方式   1：try_CanSend_Mutex()方式  2：tryLock（）方式 #/

#if HJX_COMMUNICATION_SWITCH==2

//所有结构变量均一一对应协议文件中底层设备中的变量#/
//详情参考 /pulic/protocols/中的协议文件#/
//N1：表示发送该PDO的节点设备号为1的设备节点，#/
//01H：对应第一个字节的值，即协议文件中的BYTE0的值#/
//PDO数据结构为 8个字节#/
PackN1PDO01H uiPDO01; //将UI的运行状态和工作模式设置发送给主控板#/
PackN1PDO02H uiPDO02; //血泵速度和透析夜流速#/
PackN1PDO03H uiPDO03;
PackN1PDO04H uiPDO04;
PackN1PDO05H uiPDO05;
PackN1PDO06H uiPDO06;
PackN1PDO07H uiPDO07;
PackN1PDO08H uiPDO08;
PackN1PDO09H uiPDO09;
PackN1PDO0AH uiPDO0A;
PackN1PDO0BH uiPDO0B;
//    PackN1PDO0CH uiPDO0C;

volatile bool S_PDO_MUTEX=false;    //指示当前是否在发送PDO
bool S_UIPDO01_MUTEX=false;    //指示PDO1是否有改变
bool S_UIPDO02_MUTEX=false;
bool S_UIPDO03_MUTEX=false;
bool S_UIPDO04_MUTEX=false;
bool S_UIPDO05_MUTEX=false;
bool S_UIPDO06_MUTEX=false;
bool S_UIPDO07_MUTEX=false;
bool S_UIPDO08_MUTEX=false;
bool S_UIPDO09_MUTEX=false;
bool S_UIPDO0A_MUTEX=false;
bool S_UIPDO0B_MUTEX=false;



DataCanSend::DataCanSend(QObject *parent) :sendPDOTimer(parent)
{
    //进行各个PDO结构的初始化#/
    memset(&uiPDO01,0,sizeof(PackN1PDO01H));
    memset(&uiPDO02,0,sizeof(PackN1PDO02H));
    memset(&uiPDO03,0,sizeof(PackN1PDO03H));
    memset(&uiPDO04,0,sizeof(PackN1PDO04H));
    memset(&uiPDO05,0,sizeof(PackN1PDO05H));
    memset(&uiPDO06,0,sizeof(PackN1PDO06H));
    memset(&uiPDO07,0,sizeof(PackN1PDO07H));
    memset(&uiPDO08,0,sizeof(PackN1PDO08H));

    //dataType是对应各个设备节点的标示符#/
    //因为主机发送的PDO设置为各个节点设备均可接收到#/
    uiPDO01.dataType = 0x1;
    uiPDO02.dataType = 0x2;
    uiPDO03.dataType = 0x3;
    uiPDO04.dataType = 0x4;
    uiPDO05.dataType = 0x5;
    uiPDO06.dataType = 0x6;
    uiPDO07.dataType = 0x7;
    uiPDO08.dataType = 0x8;

    uiPDO09.dataType = 0x9;
    uiPDO09.N1_set_QJspeed = 500;
    uiPDO09.N1_set_QJcond_UP = 0;
    uiPDO09.N1_set_QJcond_DOWN = 0;

    uiPDO0A.dataType = 0xA;
    uiPDO0A.N1_set_QJtemp = 0;
    uiPDO0A.N1_set_QJsuck = 100;

    uiPDO0B.dataType = 0xB;
    uiPDO0B.N1_set_SubBPspeed = 0;

    //memset(&uiPDO0C,0,sizeof(uiPDO0C));
    //uiPDO0C.dataType = 0xC;
    memset(&PDOstate,0,sizeof(PDOstate));
    PDOstate.dataType = 0xC;
//    PDO_LIST.clear();
//    SDO_LIST.clear();

    memset(PDO_LIST,0,DE_PDO_MAXNUM);
    //每一秒，发送PDO参数一次#/
    connect(&sendPDOTimer,SIGNAL(timeout()),this,SLOT(timerHandle()),Qt::DirectConnection);

    sendPDOTimer.start(9000);
}

void DataCanSend::init()
{
    //为这些名称的数据管理包，添加操作处理#/
    dataNodeAddObject(PACKET_NAME_AP, this);
    dataNodeAddObject(PACKET_NAME_VP, this);
    dataNodeAddObject(PACKET_NAME_TMP, this);
    dataNodeAddObject(PACKET_NAME_COND, this);
    dataNodeAddObject(PACKET_PDO_HEPARINPUMP, this);
    dataNodeAddObject(PACKET_NAME_DIALYSATE, this);
    dataNodeAddObject(Disinfect_Work_Model, this);
    dataNodeAddObject(PACKET_SDO_BLOOD, this);
    dataNodeAddObject(PACKET_FORMULA_TARGET, this);
    dataNodeAddObject(PACKET_NAME_WORKMODE, this);
    dataNodeAddObject(PACKET_NAME_N1STATE, this);
    dataNodeAddObject(PACKET_NAME_UF, this);
    dataNodeAddObject(PACKET_PDO_SUBBLOODPUMP, this);

//    start(QThread::TimeCriticalPriority);
    this->start();
}

bool DataCanSend::test_mutex(bool vis)
{
    S_UIPDO01_MUTEX=vis;
    return vis;
}

void DataCanSend::AppendPDOData(int id)
{
    PDO_LIST[id]=id;
}

void DataCanSend::sendSDOData(int id)
{

#if DEBUG_MUTEXDIE_FLAG
    Debug_insert("DataCanSend::003");
#endif

#if DE_MUTEX_TRYLOCK_OPEN==0    
    G_CanSend_Mutex.lock();
#elif DE_MUTEX_TRYLOCK_OPEN==1
    try_CanSend_Mutex();
#endif
#if DE_MUTEX_TRYLOCK_OPEN==2
    G_CanSend_Mutex.tryLock(30);
#endif

    send_SDO_handle(id);
    us_sleep(5000);

    /*
    if(SDO_LIST.size()<20)
    {
        SDO_LIST.append(id);
    }
    */

    G_CanSend_Mutex.unlock();

//    startsend_sem.release();

#if DEBUG_MUTEXDIE_FLAG
    Debug_insert("DataCanSend::004");
#endif
}

void DataCanSend::sendSDOData2(int id,unsigned int vol1,unsigned int vol2)
{
#if DEBUG_MUTEXDIE_FLAG
    Debug_insert("DataCanSend::003");
#endif

#if DE_MUTEX_TRYLOCK_OPEN==0
    G_CanSend_Mutex.lock();
#elif DE_MUTEX_TRYLOCK_OPEN==1
    try_CanSend_Mutex();
#endif
#if DE_MUTEX_TRYLOCK_OPEN==2
    G_CanSend_Mutex.tryLock(30);
#endif

    send_SDO_handle(id,vol1,vol2);
    us_sleep(5000);

    /*
    if(SDO_LIST.size()<20)
    {
        SDO_LIST.append(id);
    }
    */

    G_CanSend_Mutex.unlock();

//    startsend_sem.release();

#if DEBUG_MUTEXDIE_FLAG
    Debug_insert("DataCanSend::004");
#endif
}

void DataCanSend::timerHandle()
{
    if(S_PDO_MUTEX==true)
        return;

#if DEBUG_MUTEXDIE_FLAG
    Debug_insert("DataCanSend::005");
#endif

#if DE_MUTEX_TRYLOCK_OPEN==0
    G_CanSend_Mutex.lock();
#elif DE_MUTEX_TRYLOCK_OPEN==1
    try_CanSend_Mutex();
#endif
#if DE_MUTEX_TRYLOCK_OPEN==2
    G_CanSend_Mutex.tryLock(30);
#endif

    AppendPDOData(DE_SENDPDO01_FLAG);
    AppendPDOData(DE_SENDPDO02_FLAG);
    AppendPDOData(DE_SENDPDO03_FLAG);
    AppendPDOData(DE_SENDPDO04_FLAG);
    AppendPDOData(DE_SENDPDO05_FLAG);
    AppendPDOData(DE_SENDPDO06_FLAG);
    AppendPDOData(DE_SENDPDO07_FLAG);
    AppendPDOData(DE_SENDPDO08_FLAG);
    AppendPDOData(DE_SENDPDO09_FLAG);
    AppendPDOData(DE_SENDPDO0A_FLAG);
    AppendPDOData(DE_SENDPDO0B_FLAG);
    AppendPDOData(DE_SENDPDO0C_FLAG);

    G_CanSend_Mutex.unlock();
    startsend_sem.release();

#if DEBUG_MUTEXDIE_FLAG
    Debug_insert("DataCanSend::006");
#endif
}

void DataCanSend::run()
{
    int i;

    while(1)
    {
        startsend_sem.acquire();  //等待信号量#/

#if DEBUG_MUTEXDIE_FLAG
    Debug_insert("DataCanSend::007");
#endif

#if DE_MUTEX_TRYLOCK_OPEN==0
    G_CanSend_Mutex.lock();
#elif DE_MUTEX_TRYLOCK_OPEN==1
    try_CanSend_Mutex();
#endif
#if DE_MUTEX_TRYLOCK_OPEN==2
    G_CanSend_Mutex.tryLock(30);
#endif

        S_PDO_MUTEX=true;

        for(i=0;i<DE_PDO_MAXNUM;i++)
        {
            if((PDO_LIST[i]==i) && i>0)  //第0个是无效的#/
            {
                send_PDO_handle(i);
                us_sleep(10000);
            }

            PDO_LIST[i]=0;
        }

        S_PDO_MUTEX=false;

     //   if(S_UIPDO01_MUTEX==false)  //线程死锁测试#/
            G_CanSend_Mutex.unlock();

#if DEBUG_MUTEXDIE_FLAG
    Debug_insert("DataCanSend::008");
#endif
    }
}

void DataCanSend::send_PDO_handle(int id)
{

    switch(id)
    {
        case DE_SENDPDO00_FLAG:
        break;
        case DE_SENDPDO01_FLAG:
        MasterCtrol_SendPDO(&uiPDO01);
        us_sleep(10000);
        //取反发送pdo#/
        MasterCtrol_SendPDO_negation(&uiPDO01);
        break;
        case DE_SENDPDO02_FLAG:
        uiPDO02.N1_set_BPspeed=G_SET_BPSPEED;   //防范血泵速度乱码#/
        MasterCtrol_SendPDO(&uiPDO02);
        us_sleep(10000);
        //取反发送pdo#/
        MasterCtrol_SendPDO_negation(&uiPDO02);
        break;
        case DE_SENDPDO03_FLAG:
        MasterCtrol_SendPDO(&uiPDO03);
        us_sleep(10000);
        //取反发送pdo#/
        MasterCtrol_SendPDO_negation(&uiPDO03);
        break;
        case DE_SENDPDO04_FLAG:
        MasterCtrol_SendPDO(&uiPDO04);
        us_sleep(10000);
        //取反发送pdo#/
        MasterCtrol_SendPDO_negation(&uiPDO04);
        break;
        case DE_SENDPDO05_FLAG:
        MasterCtrol_SendPDO(&uiPDO05);
        us_sleep(10000);
        //取反发送pdo#/
        MasterCtrol_SendPDO_negation(&uiPDO05);
        break;
        case DE_SENDPDO06_FLAG:
        MasterCtrol_SendPDO(&uiPDO06);
        us_sleep(10000);
        //取反发送pdo#/
        MasterCtrol_SendPDO_negation(&uiPDO06);
        break;
        case DE_SENDPDO07_FLAG:
        MasterCtrol_SendPDO(&uiPDO07);
        us_sleep(10000);
        //取反发送pdo#/
        MasterCtrol_SendPDO_negation(&uiPDO07);
        break;
        case DE_SENDPDO08_FLAG:
        MasterCtrol_SendPDO(&uiPDO08);
        us_sleep(10000);
        //取反发送pdo#/
        MasterCtrol_SendPDO_negation(&uiPDO08);
        break;
        case DE_SENDPDO09_FLAG:
        MasterCtrol_SendPDO(&uiPDO09);
        us_sleep(10000);
        //取反发送pdo#/
        MasterCtrol_SendPDO_negation(&uiPDO09);
        break;
        case DE_SENDPDO0A_FLAG:
        MasterCtrol_SendPDO(&uiPDO0A);
        us_sleep(10000);
        //取反发送pdo#/
        MasterCtrol_SendPDO_negation(&uiPDO0A);
        break;
        case DE_SENDPDO0B_FLAG:
        uiPDO0B.N1_set_SubBPspeed = G_SUB_SETSPEED;
        MasterCtrol_SendPDO(&uiPDO0B);
        us_sleep(10000);
        //取反发送pdo#/
        MasterCtrol_SendPDO_negation(&uiPDO0B);
        break;
        case DE_SENDPDO0C_FLAG:
        MasterCtrol_SendPDO(&PDOstate);
        us_sleep(10000);
        //取反发送pdo#/
        MasterCtrol_SendPDO_negation(&PDOstate);
        break;

        default:
        break;
    }
}


void DataCanSend::try_CanSend_Mutex()
{
    static char count=0;
    while(count<8)
    {
        if(G_CanSend_Mutex.tryLock()==true)
        {
            count=0;
            break;
        }
        else
        {
            count++;
            us_sleep(5000);
        }
    }
    if(count>=8)
    {
        G_CanSend_Mutex.unlock();
        count=0;
        G_CanSend_Mutex.tryLock();
    }
}

void DataCanSend::send_SDO_handle(int id,unsigned int vol1,unsigned int vol2)
{
    PacketBloodPump blood1;
    PacketBloodPump blood2;
    PackN15PDO N15PDO;
    PacketStatusNote MasterStatus;
    PacketStatusAlarm alarmState;

//    vol1=vol1;
    vol2=vol2;

    switch(id)
    {
    case 0:
        break;
    case DE_CMD_Checkself:
        MasterCtrol_CMD_Checkself();
        break;
    case DE_CMD_Pass_Checkself:
        MasterCtrol_CMD_Pass_Checkself();
        break;
    case DE_CMD_Stop_water:
        MasterCtrol_CMD_Stop_water();
        break;
    case DE_CMD_Treat_start:
        PacketWorkMode workModeTMP;
        dataRead(PACKET_NAME_WORKMODE,&workModeTMP);
        MasterCtrol_CMD_Treat_start(workModeTMP.treatmode.current);//
        break;
    case DE_CMD_WaterReboot:
        MasterCtrol_CMD_WaterReboot();
        break;
    case DE_CMD_Treat_stop:
        MasterCtrol_CMD_Treat_stop();
        break;
    case DE_CMD_Treat_end:
        MasterCtrol_CMD_Treat_end();
        break;
   case DE_CMD_Dry_start:
        MasterCtrol_CMD_Dry_start();
        break;
    case DE_CMD_Dry_stop:
        MasterCtrol_CMD_Dry_stop();
        break;
    case DE_CMD_Disinfect_start:
//        MasterCtrol_CMD_Disinfect_start(int model);
        break;
    case DE_CMD_Disinfect_stop:
        MasterCtrol_CMD_Disinfect_stop();
        break;
    case DE_CMD_BloodStart:        
        dataRead(PACKET_NAME_BLOODPUMP,&blood1);
        if(blood1.N9_mode!=0x01)
        {
        //    MasterCtrol_CMD_BloodStart();
            BloodPump_CMD_BloodStart();//直接启动血泵#/
        }
        break;
    case DE_CMD_BloodStop:
        dataRead(PACKET_NAME_BLOODPUMP,&blood1);
        if(blood1.N9_mode!=0x02)
        {
         //   MasterCtrol_CMD_BloodStop();
             BloodPump_CMD_BloodStop();//直接停止血泵#/
        }
        break;
    case DE_CMD_ResetBloodAmount:
        MasterCtrol_CMD_ResetBloodAmount();
        break;
    case DE_CMD_SubsStart:
        dataRead(PACKET_NAME_SUBBLOODPUMP,&blood2);
        if(blood2.N9_mode!=0x01)
        {
         //   MasterCtrol_CMD_SubsStart();
             SubPump_CMD_SubsStart();//直接启动置换液泵#/
        }
        break;
    case DE_CMD_SubsStop:
        dataRead(PACKET_NAME_SUBBLOODPUMP,&blood2);
        if(blood2.N9_mode!=0x02)
        {
         //   MasterCtrol_CMD_SubsStop();
            SubPump_CMD_SubsStop();//直接停止置换液泵#/
        }
        break;
    case DE_CMD_SubsAdd:
        MasterCtrol_CMD_SubsAdd(SubsAddnum);//
        break;
    case DE_CMD_ResetSubsAmount:
        MasterCtrol_CMD_ResetSubsAmount();
        break;
    case DE_CMD_HeparinPumpStartHaveFirst:
    //    MasterCtrol_CMD_HeparinPumpStartHaveFirst();
          Heparin_CMD_HeparinPumpStartHaveFirst();
        break;
    case DE_CMD_HeparinPumpStartNoFirst:
        dataRead(PACKET_NAME_HEPARINDATA,&N15PDO);
        if(N15PDO.workMode!=0x01)
        {
   //     MasterCtrol_CMD_HeparinPumpStartNoFirst();
          Heparin_CMD_HeparinPumpStartNoFirst();
        }
        break;
    case DE_CMD_HeparinPumpStop:
        dataRead(PACKET_NAME_HEPARINDATA,&N15PDO);
        if(N15PDO.workMode==0x01)
        {
    //        MasterCtrol_CMD_HeparinPumpStop();
            Heparin_CMD_HeparinPumpStopCanDown();
        }
        break;
    case DE_CMD_ResetHeparinPumpAmount:
        MasterCtrol_CMD_ResetHeparinPumpAmount();
        break;
    case DE_CMD_UFStart:
        dataRead(PACKET_STATUS_NOTE,&MasterStatus);
        if((MasterStatus.note.note2&0x80)!=0x80)
            MasterCtrol_CMD_UFStart();
        break;
    case DE_CMD_UFStop:
        dataRead(PACKET_STATUS_NOTE,&MasterStatus);
        if(MasterStatus.note.note2&0x80)
            MasterCtrol_CMD_UFStop();
        break;
    case DE_CMD_ResetUFAmount:
        MasterCtrol_CMD_ResetUFAmount();
        break;
    case DE_CMD_Bypass:
        MasterCtrol_CMD_Bypass();
        break;
    case DE_CMD_Nobypass:
        MasterCtrol_CMD_Nobypass();
        break;
    case DE_CMD_ClipStart:
        dataRead(PACKET_STATUS_ALARM,&alarmState);
        if(alarmState.alarm.Iostatus&0x08)  //动静脉阻流夹关闭#/
            MasterCtrol_CMD_ClipStart();
        break;
    case DE_CMD_ClipStop:
        dataRead(PACKET_STATUS_ALARM,&alarmState);
        if((alarmState.alarm.Iostatus&0x08)!=0x08)  //阻流夹打开时#/
            MasterCtrol_CMD_ClipStop();
        break;
    case DE_CMD_Clip2Start:
        dataRead(PACKET_STATUS_ALARM,&alarmState);
        if(alarmState.alarm.Iostatus&0x10)  //动静脉阻流夹关闭#/
            MasterCtrol_CMD_Clip2Start();
        break;
    case DE_CMD_Clip2Stop:
        dataRead(PACKET_STATUS_ALARM,&alarmState);
        if((alarmState.alarm.Iostatus&0x10)!=0x10)  //阻流夹打开时#/
            MasterCtrol_CMD_Clip2Stop();
        break;
    case DE_SetFormulaB:
//        MasterCtrol_SetFormulaB(struct PacketSdoScale *scale);//
        break;
    case DE_SetFormulaTarget:
//        MasterCtrol_SetFormulaTarget(struct PacketSdoAim* aim);//
        break;
    case DE_SetHeparin:
//        MasterCtrol_SetHeparin(struct PacketSdoHeparin* heparin);//
        break;
    case DE_SetRangeTemp:
//        MasterCtrol_SetRangeTemp(struct PacketSdoRangeTemp* rangeTemp);//
        break;
    case DE_CMD_Shutdown:
        MasterCtrol_CMD_Shutdown();
        break;
    case DE_CMD_Reboot:
        MasterCtrol_CMD_Reboot();
        break;
    case DE_CMD_EEporm_read:
//        MasterCtrol_CMD_EEporm_read(int addr);//
        break;
    case DE_CMD_EEporm_write:
//        MasterCtrol_CMD_EEporm_write(unsigned char *addr_data);//
        break;
    case DE_CMD_BMP_Start:
        MasterCtrol_CMD_BMP_Start();
        break;
    case DE_CMD_BMP_Stop:
        MasterCtrol_CMD_BMP_Stop();
        break;
    case DE_CMD_Startpreflush:
        MasterCtrol_CMD_Startpreflush(Startpreflush_mode);//启动预冲#/
        break;
    case DE_CMD_Stoppreflush:
        MasterCtrol_CMD_Stoppreflush();//停止预冲#/
        break;
    case DE_CMD_Startreturnblood:
        MasterCtrol_CMD_Startreturnblood(Startreturnblood_mode);//启动回血#/
        break;
    case DE_CMD_Stopreturnblood:
        MasterCtrol_CMD_Stopreturnblood();//停止回血#/
        break;
    case DE_CMD_Blood1EnableButton://血泵1 启用物理开关#/
        MasterCtrol_CMD_Blood1EnableButton();
        break;
    case DE_CMD_Blood1DisableButton://血泵1 禁止物理开关#/
        MasterCtrol_CMD_Blood1DisableButton();
        break;
    case DE_CMD_SubsDisableButton://置换液泵 禁止物理开关#/
        MasterCtrol_CMD_SubsDisableButton();
        break;
    case DE_CMD_SubsEnableButton://置换液泵 启用物理开关#/
        MasterCtrol_CMD_SubsEnableButton();
        break;
    case DE_CMD_OPENVALVE:  //开启相应的电磁阀#/
        MasterCtrol_CMD_OpenValve(G_EXTERNAL_VALVEFLAG,G_EXTERNAL_VALVEID);//打开相应的电磁阀#/
        break;
    case DE_CMD_CLOSEVALVE:  //关闭相应的电磁阀#/
        MasterCtrol_CMD_CloseValve(G_EXTERNAL_VALVEFLAG,G_EXTERNAL_VALVEID);
        break;
    case DE_CMD_P1PUMPACTION:
        MasterCtrol_CMD_P1Pump(G_EXTERNAL_PUMPMODE,G_EXTERNAL_PUMPVOLT);
        break;
    case DE_CMD_P2PUMPACTION:
        MasterCtrol_CMD_P2Pump(G_EXTERNAL_PUMPMODE,G_EXTERNAL_PUMPVOLT);
        break;
    case DE_CMD_P3PUMPACTION:
        MasterCtrol_CMD_P3Pump(G_EXTERNAL_PUMPMODE,G_EXTERNAL_PUMPVOLT);
        break;
    case DE_CMD_P4PUMPACTION:
        MasterCtrol_CMD_P4Pump(G_EXTERNAL_PUMPMODE,G_EXTERNAL_PUMPVOLT);
        break;
    case DE_CMD_JP1PUMPACTION:
        MasterCtrol_CMD_JP1Pump(G_EXTERNAL_PUMPMODE,G_EXTERNAL_PUMPVOLT);
        break;
    case DE_CMD_JP2PUMPACTION:
        MasterCtrol_CMD_JP2Pump(G_EXTERNAL_PUMPMODE,G_EXTERNAL_PUMPVOLT);
        break;
    case DE_CMD_JP3PUMPACTION:
        MasterCtrol_CMD_JP3Pump(G_EXTERNAL_PUMPMODE,G_EXTERNAL_PUMPVOLT);
        break;
    case DE_CMD_BALANCEACTION:
        MasterCtrol_CMD_Balance(G_EXTERNAL_PUMPMODE,G_EXTERNAL_PUMPVOLT);
        break;
    case DE_CMD_HEATINGACTION:
        MasterCtrol_CMD_Heating(G_EXTERNAL_PUMPMODE,G_EXTERNAL_PUMPVOLT);
        break;
    case DE_CMD_DEGASSINGPUMP:
        MasterCtrol_CMD_DegassPump(G_EXTERNAL_PUMPMODE,G_EXTERNAL_PUMPVOLT);
        break;
    case DE_SDO_FORMULA_STARTTEST: //配方测试启动
        MasterCtrol_CMD_FormulaTest(1);
        break;
    case DE_SDO_FORMULA_STOPTEST: //配方测试停止
        MasterCtrol_CMD_FormulaTest(2);
        break;
    case DE_SDO_DIALYSATE_STARTTEST: //透析夜测试启动
        MasterCtrol_CMD_DialysateTest(1);
        break;
    case DE_SDO_DIALYSATE_STOPTEST: //透析夜测试停止
        MasterCtrol_CMD_DialysateTest(2);
        break;
    case DE_SDO_APUMP_STARTTEST: //A液泵测试
         MasterCtrol_CMD_APumpTest(1,vol1);
         break;
    case DE_SDO_APUMP_STOPTEST: //
        MasterCtrol_CMD_APumpTest(2,vol1);
        break;
    case DE_SDO_BPUMP_STARTTEST: //B液泵测试
        MasterCtrol_CMD_BPumpTest(1,vol1);
        break;
    case DE_SDO_BPUMP_STOPTEST: //
        MasterCtrol_CMD_BPumpTest(2,vol1);
        break;
    case DE_SDO_TEMP_STARTTEST: //温度测试
        MasterCtrol_CMD_TempTest(1);
        break;
    case DE_SDO_TEMP_STOPTEST: //
        MasterCtrol_CMD_TempTest(2);
        break;
    case DE_SDO_COND_STARTTEST: //电导测试
        MasterCtrol_CMD_CondTest(1);
        break;
    case DE_SDO_COND_STOPTEST: //
        MasterCtrol_CMD_CondTest(2);
        break;
    case DE_SDO_UF_STARTTEST: //超滤泵测试
        MasterCtrol_CMD_UFTest(1);
        break;
    case DE_SDO_UF_STOPTEST://
        MasterCtrol_CMD_UFTest(2);
        break;
    }
}

void DataCanSend::ActionHandle(const void *dataPtr, int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("can_send-ActionHandle-in");
#endif

    if ((dataPtr == NULL) || (name == 0)) {
        return;
    }

    if (name==PACKET_SDO_BLOOD) {
            SdoBlood = *(PacketSdoBlood*)dataPtr;

            if((uiPDO02.N1_set_BPspeed != SdoBlood.N3_set_BPspeed)||(uiPDO02.N1_set_BPtube != SdoBlood.N3_set_BPtube)){

#if DE_MUTEX_TRYLOCK_OPEN==0
    G_CanSend_Mutex.lock();
#elif DE_MUTEX_TRYLOCK_OPEN==1
    try_CanSend_Mutex();
#endif
#if DE_MUTEX_TRYLOCK_OPEN==2
    G_CanSend_Mutex.tryLock(30);
#endif

                uiPDO02.N1_set_BPspeed = SdoBlood.N3_set_BPspeed;
                uiPDO02.N1_set_BPtube = SdoBlood.N3_set_BPtube;

//                send_PDO_handle(DE_SENDPDO02_FLAG);
                AppendPDOData(DE_SENDPDO02_FLAG);

                G_CanSend_Mutex.unlock();
                startsend_sem.release();
            }
    }
    else if (name==PACKET_NAME_DIALYSATE) {
        bool flage = false;
        packetFluid = *(PacketFluid *)dataPtr;

#if DE_MUTEX_TRYLOCK_OPEN==0
    G_CanSend_Mutex.lock();
#elif DE_MUTEX_TRYLOCK_OPEN==1
    try_CanSend_Mutex();
#endif
#if DE_MUTEX_TRYLOCK_OPEN==2
    G_CanSend_Mutex.tryLock(30);
#endif

        if (fluidTemp.tempTarget != packetFluid.tempTarget)
        {
            fluidTemp.tempTarget = packetFluid.tempTarget;
            uiPDO02.N1_set_temp = fluidTemp.tempTarget;
 //           uiPDO02.N1_set_temp *= 10;
            uiPDO05.N1_set_temp_UP= uiPDO02.N1_set_temp+20;
            uiPDO06.N1_set_temp_DOWN= uiPDO02.N1_set_temp-20;

            if(uiPDO05.N1_set_temp_UP>400)
                uiPDO05.N1_set_temp_UP=400;
            if(uiPDO06.N1_set_temp_DOWN<300)
                uiPDO06.N1_set_temp_DOWN=300;

            flage = true;
        }

        if (fluidTemp.Wspeed != packetFluid.Wspeed){
            fluidTemp.Wspeed = packetFluid.Wspeed;
            uiPDO02.N1_set_Wspeed = fluidTemp.Wspeed;
            flage = true;
        }

        if (flage) {
            uiPDO02.dataType = 0x2;

            AppendPDOData(DE_SENDPDO02_FLAG);
            AppendPDOData(DE_SENDPDO05_FLAG);
            AppendPDOData(DE_SENDPDO06_FLAG);

            G_CanSend_Mutex.unlock();
            startsend_sem.release();
        }
        else
        {
            G_CanSend_Mutex.unlock();
        }
    }

    if (name==PACKET_FORMULA_TARGET){

        bool flage = false;
        formulaSet = *(FormulaSet *)dataPtr;

#if DE_MUTEX_TRYLOCK_OPEN==0
    G_CanSend_Mutex.lock();
#elif DE_MUTEX_TRYLOCK_OPEN==1
    try_CanSend_Mutex();
#endif
#if DE_MUTEX_TRYLOCK_OPEN==2
    G_CanSend_Mutex.tryLock(30);
#endif

        if (formulaSetTemp.outTarget.scale_b != formulaSet.outTarget.scale_b)
        {
            formulaSetTemp.outTarget.scale_b = formulaSet.outTarget.scale_b;
            uiPDO03.N1_set_Bscale = formulaSetTemp.outTarget.scale_b;
            flage = true;
        }

        if (formulaSetTemp.outTarget.scale_water != formulaSet.outTarget.scale_water)
        {
            formulaSetTemp.outTarget.scale_water = formulaSet.outTarget.scale_water;
            uiPDO03.N1_set_Wscale = formulaSetTemp.outTarget.scale_water;
            flage = true;
        }

        if (formulaSetTemp.outTarget.target_b != formulaSet.outTarget.target_b)
        {
            formulaSetTemp.outTarget.target_b = formulaSet.outTarget.target_b;
            uiPDO03.N1_set_B = formulaSetTemp.outTarget.target_b;
            flage = true;
        }

        if (formulaSetTemp.outTarget.target_mix != formulaSet.outTarget.target_mix)
        {
            formulaSetTemp.outTarget.target_mix = formulaSet.outTarget.target_mix;
            uiPDO04.N1_set_mix = formulaSetTemp.outTarget.target_mix;
            flage = true;
        }

        if (formulaSet.tempBuffer.bnacl>0)
        {
            uiPDO01.byte2.pe = 1;
            flage = true;
        }
        else if(formulaSet.tempBuffer.bnacl==0)
        {
            uiPDO01.byte2.pe = 0;
            flage = true;
        }

        if (flage){

            uiPDO03.dataType = 0x3;
            uiPDO04.dataType = 0x4;

            AppendPDOData(DE_SENDPDO01_FLAG);
            AppendPDOData(DE_SENDPDO03_FLAG);
            AppendPDOData(DE_SENDPDO04_FLAG);


            G_CanSend_Mutex.unlock();
            startsend_sem.release();
        }
        else
        {
            G_CanSend_Mutex.unlock();
        }
    }

    if (name==PACKET_NAME_UF){
        bool flage = false;
        packetUF = *(PacketUF *)dataPtr;

#if DE_MUTEX_TRYLOCK_OPEN==0
    G_CanSend_Mutex.lock();
#elif DE_MUTEX_TRYLOCK_OPEN==1
    try_CanSend_Mutex();
#endif
#if DE_MUTEX_TRYLOCK_OPEN==2
    G_CanSend_Mutex.tryLock(30);
#endif

        if(packetUF.chooseRate){
            if(ufTmp.ufRate != packetUF.ufRate)
            {//选用的治疗超滤泵速度有改变则更新超滤泵的速度#/
                ufTmp.ufRate = packetUF.ufRate;
                uiPDO04.N1_set_UFspeed =(unsigned short int)ufTmp.ufRate;
                flage = true;               
            }
        }
        else if(ufTmp.preRate != packetUF.preRate)
        {//选用的预冲超滤泵速度有改变则更新超滤泵的速度#/
            ufTmp.preRate = packetUF.preRate;
            uiPDO04.N1_set_UFspeed = (unsigned short int)ufTmp.preRate;
            flage = true;
        }

        if (ufTmp.ufVolAim != packetUF.ufVolAim)
        {
            ufTmp.ufVolAim = packetUF.ufVolAim;
            uiPDO04.N1_set_UFtarget = ufTmp.ufVolAim;
            flage = true;
        }

        if (flage)
        {
            uiPDO04.dataType = 0x4;


            AppendPDOData(DE_SENDPDO04_FLAG);


            G_CanSend_Mutex.unlock();
            startsend_sem.release();
        }
        else
        {
            G_CanSend_Mutex.unlock();
        }
    }

    if (name==PACKET_PDO_HEPARINPUMP) {

        bool flage=false;
        packetPDOHeparinPump = *(PacketPDOHeparinPump *)dataPtr;

#if DE_MUTEX_TRYLOCK_OPEN==0
    G_CanSend_Mutex.lock();
#elif DE_MUTEX_TRYLOCK_OPEN==1
    try_CanSend_Mutex();
#endif
#if DE_MUTEX_TRYLOCK_OPEN==2
    G_CanSend_Mutex.tryLock(30);
#endif

        if (HeparinPump.dose != packetPDOHeparinPump.dose)
        {
            HeparinPump.dose = packetPDOHeparinPump.dose;
            uiPDO05.N1_set_heparin_first = packetPDOHeparinPump.dose;
            flage = true;
        }

        if (HeparinPump.speed != packetPDOHeparinPump.speed)
        {
            HeparinPump.speed = packetPDOHeparinPump.speed;
            uiPDO05.N1_set_heparin_speed = packetPDOHeparinPump.speed;
            flage = true;
        }

        if (flage)
        {
            uiPDO05.dataType = 0x5;

            AppendPDOData(DE_SENDPDO05_FLAG);

            G_CanSend_Mutex.unlock();
            startsend_sem.release();
        }
        else
        {
            G_CanSend_Mutex.unlock();
        }
    }

    if (name==PACKET_NAME_AP) {

        packetAP = *(PacketAP*)dataPtr;
        bool flage=false;

#if DE_MUTEX_TRYLOCK_OPEN==0
    G_CanSend_Mutex.lock();
#elif DE_MUTEX_TRYLOCK_OPEN==1
    try_CanSend_Mutex();
#endif
#if DE_MUTEX_TRYLOCK_OPEN==2
    G_CanSend_Mutex.tryLock(30);
#endif

        if (APTemp.apL != packetAP.apL) {
            APTemp.apL = packetAP.apL;
            uiPDO06.N1_set_AP_DOWN = (APTemp.apL) + 700;
            flage = true;
        }

        if (APTemp.apUp != packetAP.apUp) {
            APTemp.apUp = packetAP.apUp;
            uiPDO06.N1_set_AP_UP = (APTemp.apUp) + 700;
            flage = true;
        }

        if (flage) {
            uiPDO06.dataType = 0x6;

            AppendPDOData(DE_SENDPDO06_FLAG);

            G_CanSend_Mutex.unlock();
            startsend_sem.release();
        }
        else
        {
            G_CanSend_Mutex.unlock();
        }
    }

    if (name==PACKET_NAME_VP) {
        bool flage=false;
        packetVP = *(PacketVP*)dataPtr;

#if DE_MUTEX_TRYLOCK_OPEN==0
    G_CanSend_Mutex.lock();
#elif DE_MUTEX_TRYLOCK_OPEN==1
    try_CanSend_Mutex();
#endif
#if DE_MUTEX_TRYLOCK_OPEN==2
    G_CanSend_Mutex.tryLock(30);
#endif

        if (VPTemp.vpL != packetVP.vpL)
        {
            VPTemp.vpL = packetVP.vpL;
            uiPDO07.N1_set_VP_DOWN = (VPTemp.vpL) + 700;
            flage = true;
        }

        if (VPTemp.vpUp != packetVP.vpUp){
            VPTemp.vpUp = packetVP.vpUp;
            uiPDO07.N1_set_VP_UP = (VPTemp.vpUp) + 700;
            flage = true;
        }

        if (flage) {
            uiPDO07.dataType = 0x7;

            AppendPDOData(DE_SENDPDO07_FLAG);

            G_CanSend_Mutex.unlock();
            startsend_sem.release();
        }
        else
        {
            G_CanSend_Mutex.unlock();
        }
    }

    if (name==PACKET_NAME_TMP) {

        packetTMP = *(PacketTMP*)dataPtr;             

        if((TMPTemp.tmpL != packetTMP.tmpL)||(TMPTemp.tmpUp != packetTMP.tmpUp))
        {

#if DE_MUTEX_TRYLOCK_OPEN==0
    G_CanSend_Mutex.lock();
#elif DE_MUTEX_TRYLOCK_OPEN==1
    try_CanSend_Mutex();
#endif
#if DE_MUTEX_TRYLOCK_OPEN==2
    G_CanSend_Mutex.tryLock(30);
#endif

            TMPTemp.tmpL = packetTMP.tmpL;
            uiPDO08.N1_set_TMP_DOWN = (TMPTemp.tmpL) + 700;
            uiPDO08.dataType = 0x8;

            AppendPDOData(DE_SENDPDO08_FLAG);

            TMPTemp.tmpUp = packetTMP.tmpUp;
            uiPDO07.N1_set_TMP_UP = (TMPTemp.tmpUp) + 700;
            uiPDO07.dataType = 0x7;

            AppendPDOData(DE_SENDPDO07_FLAG);

            G_CanSend_Mutex.unlock();
            startsend_sem.release();

        }
    }

    if (name==PACKET_NAME_COND) {
#if DEBUG_OTHER_FLAG
    Debug_insert("can_sendNAME_COND-in01");
#endif
        packetCond = *(PacketCond*)dataPtr;
        if ((CONTemp.condL != packetCond.condL)||(CONTemp.condUp != packetCond.condUp)) {

#if DE_MUTEX_TRYLOCK_OPEN==0
    G_CanSend_Mutex.lock();
#elif DE_MUTEX_TRYLOCK_OPEN==1
    try_CanSend_Mutex();
#endif
#if DE_MUTEX_TRYLOCK_OPEN==2
    G_CanSend_Mutex.tryLock(30);
#endif

            CONTemp.condL = packetCond.condL;
            CONTemp.condUp = packetCond.condUp;
            uiPDO08.N1_set_Cond_DOWN = CONTemp.condL*100;
            uiPDO08.N1_set_Cond_UP = CONTemp.condUp*100;
            uiPDO08.dataType = 0x8;

            AppendPDOData(DE_SENDPDO08_FLAG);

            G_CanSend_Mutex.unlock();
            startsend_sem.release();
        }

#if DEBUG_OTHER_FLAG
    Debug_insert("can_sendNAME_COND-out");
#endif
    }

    if (name==PACKET_PDO_SUBBLOODPUMP) {
        packetSubBloodPump = *(PacketSubBloodPump*)dataPtr;
        if (subBPTmp.N10_set_speed !=packetSubBloodPump.N10_set_speed) {

#if DE_MUTEX_TRYLOCK_OPEN==0
    G_CanSend_Mutex.lock();
#elif DE_MUTEX_TRYLOCK_OPEN==1
    try_CanSend_Mutex();
#endif
#if DE_MUTEX_TRYLOCK_OPEN==2
    G_CanSend_Mutex.tryLock(30);
#endif

            subBPTmp.N10_set_speed = packetSubBloodPump.N10_set_speed;
            G_SUB_SETSPEED=packetSubBloodPump.N10_set_speed;
            uiPDO0B.dataType = 0x0B;
            uiPDO0B.N1_set_SubBPspeed = subBPTmp.N10_set_speed;

            AppendPDOData(DE_SENDPDO0B_FLAG);

            G_CanSend_Mutex.unlock();
            startsend_sem.release();
        }
    }

    if (name==PACKET_NAME_WORKMODE){
        packetWorkMode = *(PacketWorkMode *)dataPtr;

#if DE_MUTEX_TRYLOCK_OPEN==0
    G_CanSend_Mutex.lock();
#elif DE_MUTEX_TRYLOCK_OPEN==1
    try_CanSend_Mutex();
#endif
#if DE_MUTEX_TRYLOCK_OPEN==2
    G_CanSend_Mutex.tryLock(30);
#endif

        memset(&uiPDO01.byte2,0, sizeof(uiPDO01.byte2));
        switch (packetWorkMode.treatmode.current)
        {
            case HD_double:
                uiPDO01.byte1.SN_B= 0;//双针
                uiPDO01.byte1.online_B= 0;
                uiPDO01.byte1.Back_in_B = 0;
                uiPDO01.byte2.isouf = 0;
                uiPDO01.byte3.SN_type_B = 1;//双泵
                break;
            case HD_SNDP:
                uiPDO01.byte1.SN_B= 1;//单针
                uiPDO01.byte1.online_B= 0;
                uiPDO01.byte1.Back_in_B = 0;
                uiPDO01.byte2.isouf = 0;
                uiPDO01.byte3.SN_type_B = 1;//双泵
                break;
            case HD_SNSP:
                uiPDO01.byte1.SN_B= 1;
                uiPDO01.byte1.online_B= 0;
                uiPDO01.byte1.Back_in_B = 0;
                uiPDO01.byte2.isouf = 0;
                uiPDO01.byte3.SN_type_B = 0;//单泵
                break;

            case HDF_pre_dilute:
            case HF_pre_dilute:
                uiPDO01.byte1.SN_B= 0;
                uiPDO01.byte1.online_B= 1;
                uiPDO01.byte1.Back_in_B = 0;
                uiPDO01.byte2.isouf = 0;
                break;
            case HDF_post_dilute:
            case HF_post_dilute:
                uiPDO01.byte1.SN_B= 0;
                uiPDO01.byte1.online_B= 1;
                uiPDO01.byte1.Back_in_B = 1;
                uiPDO01.byte2.isouf = 0;
                break;

            case ISOUF_DNDP: //
                uiPDO01.byte1.SN_B= 0;//双针
                uiPDO01.byte1.online_B= 0;//在线
                uiPDO01.byte1.Back_in_B = 0;//0,前置换;1,后置换;
                uiPDO01.byte2.isouf = 1; //单超
                uiPDO01.byte3.SN_type_B = 1;//双泵
                break;
            case ISOUF_SNDP: //
                uiPDO01.byte1.SN_B= 1;//dan针
                uiPDO01.byte1.online_B= 0;//在线
                uiPDO01.byte1.Back_in_B = 0;//0,前置换;1,后置换;
                uiPDO01.byte2.isouf = 1; //单超
                uiPDO01.byte3.SN_type_B = 1;//双泵
                break;
            case ISOUF_SNSP: //
                uiPDO01.byte1.SN_B= 1;//dan针
                uiPDO01.byte1.online_B= 0;//在线
                uiPDO01.byte1.Back_in_B = 0;//0,前置换;1,后置换;
                uiPDO01.byte2.isouf = 1; //单超
                uiPDO01.byte3.SN_type_B = 0;//双泵
                break;
            case PE: //
                uiPDO01.byte1.SN_B= 0;//单针
                uiPDO01.byte1.online_B= 0;//在线
                uiPDO01.byte1.Back_in_B = 0;//0,前置换;1,后置换;
                //uiPDO01.byte2.pe = 1; //pe
                uiPDO01.byte2.isouf = 0; //非单超
                break;
            default:
                uiPDO01.byte1.SN_B= 0;
                uiPDO01.byte1.online_B= 0;
                uiPDO01.byte1.Back_in_B = 0;
                uiPDO01.byte2.isouf = 0;
                break;
        }

        switch (packetWorkMode.fluidmode.current)
        {
            case A_and_B:
                uiPDO01.byte1.B_powder_B= 0;
                uiPDO01.byte1.Acetate_B = 0;
                uiPDO01.byte1.A_center_B = 0;
                break;

            case A_and_BPower:
                uiPDO01.byte1.B_powder_B= 1;
                uiPDO01.byte1.Acetate_B = 0;
                uiPDO01.byte1.A_center_B = 0;
                break;

            case Acetate:
                uiPDO01.byte1.B_powder_B= 0;
                uiPDO01.byte1.Acetate_B = 1;
                uiPDO01.byte1.A_center_B = 0;
                break;
            case Center:
                uiPDO01.byte1.B_powder_B= 0; //0，1干粉
                uiPDO01.byte1.Acetate_B = 0;  //醋酸盐
                uiPDO01.byte1.A_center_B = 1;  //中央供液
                break;
            default:
                uiPDO01.byte1.B_powder_B= 0;
                uiPDO01.byte1.Acetate_B = 0;
                uiPDO01.byte1.A_center_B = 0;
                break;
        }

        uiPDO01.dataType = 0x1;
        uiPDO01.byte4.b1 = 0x1;

        AppendPDOData(DE_SENDPDO01_FLAG);

        G_CanSend_Mutex.unlock();
        startsend_sem.release();
    }

    if (name==PACKET_NAME_N1STATE) {
//        packetN1State = *(PacketN1State*)dataPtr;
//        if ( memcmp(&uiPDO0C.state,&packetN1State,sizeof(PacketN1State)) != 0 ) {
//            //uiPDO0C.state=packetN1State;
//            memcpy(&uiPDO0C.state,&packetN1State, sizeof(PacketN1State));
//            mutex.lock();
//            us_sleep(10000);
//            MasterCtrol_SendPDO(&uiPDO0C);
//            mutex.unlock();
//        }
    }

    if (name==Disinfect_Work_Model){

#if DE_MUTEX_TRYLOCK_OPEN==0
    G_CanSend_Mutex.lock();
#elif DE_MUTEX_TRYLOCK_OPEN==1
    try_CanSend_Mutex();
#endif
#if DE_MUTEX_TRYLOCK_OPEN==2
    G_CanSend_Mutex.tryLock(30);
#endif

        //switch ( * ( (DisinfectWorkModel *) dataPtr ) )
        if( Disinfect.step==loop)
          uiPDO09.N1_set_QJspeed= Disinfect.loop_flux;
        else
          uiPDO09.N1_set_QJspeed= Disinfect.flush_flux;

        uiPDO09.N1_set_QJcond_UP=Disinfect.cond_up;
        uiPDO09.N1_set_QJcond_DOWN=Disinfect.cond_down;

        if(Disinfect.step==preflush|| Disinfect.step==hold || Disinfect.step==backflush
                ||Disinfect.step==hotrinse3)
            uiPDO0A.N1_set_QJtemp=0;
        else
            uiPDO0A.N1_set_QJtemp=Disinfect.targetTemp;
        uiPDO0A.N1_set_QJsuck=Disinfect.suck_Volume;


        AppendPDOData(DE_SENDPDO09_FLAG);
        AppendPDOData(DE_SENDPDO0A_FLAG);


        G_CanSend_Mutex.unlock();
        startsend_sem.release();
    }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("can_send-ActionHandle-out");
#endif
}

#elif HJX_COMMUNICATION_SWITCH==1

QSemaphore startsend_sem(0);


DataCanSend::DataCanSend(QObject *parent) :sendPDOTimer(parent)
{
    //进行各个PDO结构的初始化#/
    memset(&uiPDO01,0,sizeof(PackN1PDO01H));
    memset(&uiPDO02,0,sizeof(PackN1PDO02H));
    memset(&uiPDO03,0,sizeof(PackN1PDO03H));
    memset(&uiPDO04,0,sizeof(PackN1PDO04H));
    memset(&uiPDO05,0,sizeof(PackN1PDO05H));
    memset(&uiPDO06,0,sizeof(PackN1PDO06H));
    memset(&uiPDO07,0,sizeof(PackN1PDO07H));
    memset(&uiPDO08,0,sizeof(PackN1PDO08H));

    //dataType是对应各个设备节点的标示符#/
    //因为主机发送的PDO设置为各个节点设备均可接收到#/
    uiPDO01.dataType = 0x1;
    uiPDO02.dataType = 0x2;
    uiPDO03.dataType = 0x3;
    uiPDO04.dataType = 0x4;
    uiPDO05.dataType = 0x5;
    uiPDO06.dataType = 0x6;
    uiPDO07.dataType = 0x7;
    uiPDO08.dataType = 0x8;

    uiPDO09.dataType = 0x9;
    uiPDO09.N1_set_QJspeed = 500;
    uiPDO09.N1_set_QJcond_UP = 0;
    uiPDO09.N1_set_QJcond_DOWN = 0;

    uiPDO0A.dataType = 0xA;
    uiPDO0A.N1_set_QJtemp = 0;
    uiPDO0A.N1_set_QJsuck = 100;

    uiPDO0B.dataType = 0xB;
    uiPDO0B.N1_set_SubBPspeed = 0;

    //memset(&uiPDO0C,0,sizeof(uiPDO0C));
    //uiPDO0C.dataType = 0xC;
    memset(&PDOstate,0,sizeof(PDOstate));
    PDOstate.dataType = 0xC;
    PDO_LIST.clear();
    SDO_LIST.clear();
    //每一秒，发送PDO参数一次#/
//    connect(&sendPDOTimer,SIGNAL(timeout()),this,SLOT(timerHandle()),Qt::DirectConnection);

//    sendPDOTimer.start(6000);
}

void DataCanSend::init()
{
    //为这些名称的数据管理包，添加操作处理#/
    dataNodeAddObject(PACKET_NAME_AP, this);
    dataNodeAddObject(PACKET_NAME_VP, this);
    dataNodeAddObject(PACKET_NAME_TMP, this);
    dataNodeAddObject(PACKET_NAME_COND, this);
    dataNodeAddObject(PACKET_PDO_HEPARINPUMP, this);
    dataNodeAddObject(PACKET_NAME_DIALYSATE, this);
    dataNodeAddObject(Disinfect_Work_Model, this);
    dataNodeAddObject(PACKET_SDO_BLOOD, this);
    dataNodeAddObject(PACKET_FORMULA_TARGET, this);
    dataNodeAddObject(PACKET_NAME_WORKMODE, this);
    dataNodeAddObject(PACKET_NAME_N1STATE, this);
    dataNodeAddObject(PACKET_NAME_UF, this);
    dataNodeAddObject(PACKET_PDO_SUBBLOODPUMP, this);

//    start(QThread::TimeCriticalPriority);
    this->start();
}

void DataCanSend::sendPDOData(int id)
{
    try_PDO_mutex();
    if(PDO_LIST.size()<20)
    {
        PDO_LIST.append(id);
    }
    G_CanSend_Mutex.unlock();
}

void DataCanSend::sendSDOData(int id)
{
    try_SDO_mutex();
    if(SDO_LIST.size()<20)
    {
        SDO_LIST.append(id);
    }
    G_CanSend_Mutex.unlock();
    startsend_sem.release();
}

void DataCanSend::timerHandle()
{    
    sendPDOData(DE_SENDPDO01_FLAG);
    sendPDOData(DE_SENDPDO02_FLAG);
    sendPDOData(DE_SENDPDO03_FLAG);
    sendPDOData(DE_SENDPDO04_FLAG);
    sendPDOData(DE_SENDPDO05_FLAG);
    sendPDOData(DE_SENDPDO06_FLAG);
    sendPDOData(DE_SENDPDO07_FLAG);
    sendPDOData(DE_SENDPDO08_FLAG);
    sendPDOData(DE_SENDPDO09_FLAG);
    sendPDOData(DE_SENDPDO0A_FLAG);
    sendPDOData(DE_SENDPDO0B_FLAG);
    sendPDOData(DE_SENDPDO0C_FLAG);
    startsend_sem.release();
}

void DataCanSend::run()
{
    int i;
    int id=0;
    while(1)
    {
        startsend_sem.acquire();
        if(PDO_LIST.size()<SDO_LIST.size())//如果要发送的PDO条数比要发送的SDO条数少，则先发PDO#/
        {
            if(PDO_LIST.count()>0)
            {
                try_PDO_mutex();
                for(i=0;i<PDO_LIST.size();i++)
                {
                    id=PDO_LIST.at(i);
                    send_PDO_handle(id);
                    us_sleep(10000);
                }
                PDO_LIST.clear();
                G_CanSend_Mutex.unlock();
            }

           try_SDO_mutex();
           for(i=0;i<SDO_LIST.size();i++)
           {
               id=SDO_LIST.at(i);
               send_SDO_handle(id);
               us_sleep(3000);
           }
           SDO_LIST.clear();
           G_CanSend_Mutex.unlock();
        }
        else
        {
            try_SDO_mutex();
            for(i=0;i<SDO_LIST.size();i++)
            {
                id=SDO_LIST.at(i);
                send_SDO_handle(id);
                us_sleep(3000);
            }
            SDO_LIST.clear();
            G_CanSend_Mutex.unlock();

            if(PDO_LIST.count()>0)
            {
                try_PDO_mutex();
                for(i=0;i<PDO_LIST.size();i++)
                {
                    id=PDO_LIST.at(i);
                    send_PDO_handle(id);
                    us_sleep(10000);
                }
                PDO_LIST.clear();
                G_CanSend_Mutex.unlock();
            }
        }
    }
}

void DataCanSend::send_PDO_handle(int id)
{
    switch(id)
    {
        case 0:
        break;
        case DE_SENDPDO00_FLAG:
        break;
        case DE_SENDPDO01_FLAG:
        MasterCtrol_SendPDO(&uiPDO01);
        break;
        case DE_SENDPDO02_FLAG:
        MasterCtrol_SendPDO(&uiPDO02);
        break;
        case DE_SENDPDO03_FLAG:
        MasterCtrol_SendPDO(&uiPDO03);
        break;
        case DE_SENDPDO04_FLAG:
        MasterCtrol_SendPDO(&uiPDO04);
        break;
        case DE_SENDPDO05_FLAG:
        MasterCtrol_SendPDO(&uiPDO05);
        break;
        case DE_SENDPDO06_FLAG:
        MasterCtrol_SendPDO(&uiPDO06);
        break;
        case DE_SENDPDO07_FLAG:
        MasterCtrol_SendPDO(&uiPDO07);
        break;
        case DE_SENDPDO08_FLAG:
        MasterCtrol_SendPDO(&uiPDO08);
        break;
        case DE_SENDPDO09_FLAG:
        MasterCtrol_SendPDO(&uiPDO09);
        break;
        case DE_SENDPDO0A_FLAG:
        MasterCtrol_SendPDO(&uiPDO0A);
        break;
        case DE_SENDPDO0B_FLAG:
        MasterCtrol_SendPDO(&uiPDO0B);
        break;
        case DE_SENDPDO0C_FLAG:
        MasterCtrol_SendPDO(&PDOstate);
        break;

        default:
        break;
    }
}

void DataCanSend::try_PDO_mutex()
{
    static char count=0;
    while(count<15)
    {
        if(G_CanSend_Mutex.tryLock()==true)
        {
            count=0;
            break;
        }
        else
        {
            count++;
            us_sleep(5000);
        }
    }
    if(count>=15)
    {
        G_CanSend_Mutex.unlock();
        count=0;
        G_CanSend_Mutex.tryLock();
    }
}

void DataCanSend::try_SDO_mutex()
{
    static char count=0;
    while(count<8)
    {
        if(G_CanSend_Mutex.tryLock()==true)
        {
            count=0;
            break;
        }
        else
        {
            count++;
            us_sleep(5000);
        }
    }
    if(count>=8)
    {
        G_CanSend_Mutex.unlock();
        count=0;
        G_CanSend_Mutex.tryLock();
    }
}

void DataCanSend::send_SDO_handle(int id)
{
    switch(id)
    {
    case 0:
        break;
    case DE_CMD_Checkself:
        MasterCtrol_CMD_Checkself();
        break;
    case DE_CMD_Pass_Checkself:
        MasterCtrol_CMD_Pass_Checkself();
        break;
    case DE_CMD_Stop_water:
        MasterCtrol_CMD_Stop_water();
        break;
    case DE_CMD_Treat_start:
        PacketWorkMode workModeTMP;
        dataRead(PACKET_NAME_WORKMODE,&workModeTMP);
        MasterCtrol_CMD_Treat_start(workModeTMP.treatmode.current);//
        break;
    case DE_CMD_WaterReboot:
        MasterCtrol_CMD_WaterReboot();
        break;
    case DE_CMD_Treat_stop:
        MasterCtrol_CMD_Treat_stop();
        break;
    case DE_CMD_Treat_end:
        MasterCtrol_CMD_Treat_end();
        break;
   case DE_CMD_Dry_start:
        MasterCtrol_CMD_Dry_start();
        break;
    case DE_CMD_Dry_stop:
        MasterCtrol_CMD_Dry_stop();
        break;
    case DE_CMD_Disinfect_start:
//        MasterCtrol_CMD_Disinfect_start(int model);
        break;
    case DE_CMD_Disinfect_stop:
        MasterCtrol_CMD_Disinfect_stop();
        break;
    case DE_CMD_BloodStart:
        MasterCtrol_CMD_BloodStart();
        break;
    case DE_CMD_BloodStop:
        MasterCtrol_CMD_BloodStop();
        break;
    case DE_CMD_ResetBloodAmount:
        MasterCtrol_CMD_ResetBloodAmount();
        break;
    case DE_CMD_SubsStart:
        MasterCtrol_CMD_SubsStart();
        break;
    case DE_CMD_SubsStop:
        MasterCtrol_CMD_SubsStop();
        break;
    case DE_CMD_SubsAdd:
        MasterCtrol_CMD_SubsAdd(SubsAddnum);//
        break;
    case DE_CMD_ResetSubsAmount:
        MasterCtrol_CMD_ResetSubsAmount();
        break;
    case DE_CMD_HeparinPumpStartHaveFirst:
        MasterCtrol_CMD_HeparinPumpStartHaveFirst();
        break;
    case DE_CMD_HeparinPumpStartNoFirst:
        MasterCtrol_CMD_HeparinPumpStartNoFirst();
        break;
    case DE_CMD_HeparinPumpStop:
        MasterCtrol_CMD_HeparinPumpStop();
        break;
    case DE_CMD_ResetHeparinPumpAmount:
        MasterCtrol_CMD_ResetHeparinPumpAmount();
        break;
    case DE_CMD_UFStart:
        MasterCtrol_CMD_UFStart();
        break;
    case DE_CMD_UFStop:
        MasterCtrol_CMD_UFStop();
        break;
    case DE_CMD_ResetUFAmount:
        MasterCtrol_CMD_ResetUFAmount();
        break;
    case DE_CMD_Bypass:
        MasterCtrol_CMD_Bypass();
        break;
    case DE_CMD_Nobypass:
        MasterCtrol_CMD_Nobypass();
        break;
    case DE_CMD_ClipStart:
        MasterCtrol_CMD_ClipStart();
        break;
    case DE_CMD_ClipStop:
        MasterCtrol_CMD_ClipStop();
        break;
    case DE_CMD_Clip2Start:
        MasterCtrol_CMD_Clip2Start();
        break;
    case DE_CMD_Clip2Stop:
        MasterCtrol_CMD_Clip2Stop();
        break;
    case DE_SetFormulaB:
//        MasterCtrol_SetFormulaB(struct PacketSdoScale *scale);//
        break;
    case DE_SetFormulaTarget:
//        MasterCtrol_SetFormulaTarget(struct PacketSdoAim* aim);//
        break;
    case DE_SetHeparin:
//        MasterCtrol_SetHeparin(struct PacketSdoHeparin* heparin);//
        break;
    case DE_SetRangeTemp:
//        MasterCtrol_SetRangeTemp(struct PacketSdoRangeTemp* rangeTemp);//
        break;
    case DE_CMD_Shutdown:
        MasterCtrol_CMD_Shutdown();
        break;
    case DE_CMD_Reboot:
        MasterCtrol_CMD_Reboot();
        break;
    case DE_CMD_EEporm_read:
//        MasterCtrol_CMD_EEporm_read(int addr);//
        break;
    case DE_CMD_EEporm_write:
//        MasterCtrol_CMD_EEporm_write(unsigned char *addr_data);//
        break;
    case DE_CMD_BMP_Start:
        MasterCtrol_CMD_BMP_Start();
        break;
    case DE_CMD_BMP_Stop:
        MasterCtrol_CMD_BMP_Stop();
        break;
    case DE_CMD_Startpreflush:
        MasterCtrol_CMD_Startpreflush(Startpreflush_mode);//启动预冲#/
        break;
    case DE_CMD_Stoppreflush:
        MasterCtrol_CMD_Stoppreflush();//停止预冲#/
        break;
    case DE_CMD_Startreturnblood:
        MasterCtrol_CMD_Startreturnblood(Startreturnblood_mode);//启动回血#/
        break;
    case DE_CMD_Stopreturnblood:
        MasterCtrol_CMD_Stopreturnblood();//停止回血#/
        break;
    case DE_CMD_Blood1EnableButton://血泵1 启用物理开关#/
        MasterCtrol_CMD_Blood1EnableButton();
        break;
    case DE_CMD_Blood1DisableButton://血泵1 禁止物理开关#/
        MasterCtrol_CMD_Blood1DisableButton();
        break;
    case DE_CMD_SubsDisableButton://置换液泵 禁止物理开关#/
        MasterCtrol_CMD_SubsDisableButton();
        break;
    case DE_CMD_SubsEnableButton://置换液泵 启用物理开关#/
        MasterCtrol_CMD_SubsEnableButton();
        break;
    }
}

void DataCanSend::ActionHandle(const void *dataPtr, const char *name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("can_send-ActionHandle-in");
#endif

    if ((dataPtr == NULL) || (name == NULL)) {
        return;
    }

    if (name==PACKET_SDO_BLOOD) {
            SdoBlood = *(PacketSdoBlood*)dataPtr;

            if((uiPDO02.N1_set_BPspeed != SdoBlood.N3_set_BPspeed)||(uiPDO02.N1_set_BPtube != SdoBlood.N3_set_BPtube)){
                uiPDO02.N1_set_BPspeed = SdoBlood.N3_set_BPspeed;
                uiPDO02.N1_set_BPtube = SdoBlood.N3_set_BPtube;
                sendPDOData(DE_SENDPDO02_FLAG);
                startsend_sem.release();
            }
    }

    if (strcmp(name,PACKET_NAME_DIALYSATE) == 0) {
        bool flage = false;
        packetFluid = *(PacketFluid *)dataPtr;

        if (fluidTemp.tempTarget != packetFluid.tempTarget)
        {
            fluidTemp.tempTarget = packetFluid.tempTarget;
            uiPDO02.N1_set_temp = fluidTemp.tempTarget;
 //           uiPDO02.N1_set_temp *= 10;
            uiPDO05.N1_set_temp_UP= uiPDO02.N1_set_temp+20;
            uiPDO06.N1_set_temp_DOWN= uiPDO02.N1_set_temp-20;
            flage = true;
        }

        if (fluidTemp.Wspeed != packetFluid.Wspeed){
            fluidTemp.Wspeed = packetFluid.Wspeed;
            uiPDO02.N1_set_Wspeed = fluidTemp.Wspeed;
            flage = true;
        }

        if (flage) {
            uiPDO02.dataType = 0x2;
            sendPDOData(DE_SENDPDO02_FLAG);
            sendPDOData(DE_SENDPDO05_FLAG);
            sendPDOData(DE_SENDPDO06_FLAG);
            startsend_sem.release();
        }
    }

    if (strcmp(name,PACKET_FORMULA_TARGET) == 0) {

        bool flage = false;
        formulaSet = *(FormulaSet *)dataPtr;

        if (formulaSetTemp.outTarget.scale_b != formulaSet.outTarget.scale_b)
        {
            formulaSetTemp.outTarget.scale_b = formulaSet.outTarget.scale_b;
            uiPDO03.N1_set_Bscale = formulaSetTemp.outTarget.scale_b;
            flage = true;
        }

        if (formulaSetTemp.outTarget.scale_water != formulaSet.outTarget.scale_water)
        {
            formulaSetTemp.outTarget.scale_water = formulaSet.outTarget.scale_water;
            uiPDO03.N1_set_Wscale = formulaSetTemp.outTarget.scale_water;
            flage = true;
        }

        if (formulaSetTemp.outTarget.target_b != formulaSet.outTarget.target_b)
        {
            formulaSetTemp.outTarget.target_b = formulaSet.outTarget.target_b;
            uiPDO03.N1_set_B = formulaSetTemp.outTarget.target_b;
            flage = true;
        }

        if (formulaSetTemp.outTarget.target_mix != formulaSet.outTarget.target_mix)
        {
            formulaSetTemp.outTarget.target_mix = formulaSet.outTarget.target_mix;
            uiPDO04.N1_set_mix = formulaSetTemp.outTarget.target_mix;
            flage = true;
        }

        if (formulaSet.tempBuffer.bnacl>0)
        {
            uiPDO01.byte2.pe = 1;
            flage = true;
        }
        else if(formulaSet.tempBuffer.bnacl==0)
        {
            uiPDO01.byte2.pe = 0;
            flage = true;
        }

        if (flage){

            uiPDO03.dataType = 0x3;
            uiPDO04.dataType = 0x4;
            sendPDOData(DE_SENDPDO01_FLAG);
            sendPDOData(DE_SENDPDO03_FLAG);
            sendPDOData(DE_SENDPDO04_FLAG);
            startsend_sem.release();
        }
    }

    if (strcmp(name,PACKET_NAME_UF) == 0){
        bool flage = false;
        packetUF = *(PacketUF *)dataPtr;

        if(packetUF.chooseRate){
            if(ufTmp.ufRate != packetUF.ufRate)
            {//选用的治疗超滤泵速度有改变则更新超滤泵的速度#/
                ufTmp.ufRate = packetUF.ufRate;
                uiPDO04.N1_set_UFspeed = ufTmp.ufRate;
                flage = true;
            }
        }
        else if(ufTmp.preRate != packetUF.preRate)
        {//选用的预冲超滤泵速度有改变则更新超滤泵的速度#/
            ufTmp.preRate = packetUF.preRate;
            uiPDO04.N1_set_UFspeed = ufTmp.preRate;
            flage = true;
        }

        if (ufTmp.ufVolAim != packetUF.ufVolAim)
        {
            ufTmp.ufVolAim = packetUF.ufVolAim;
            uiPDO04.N1_set_UFtarget = ufTmp.ufVolAim;
            flage = true;
        }

        if (flage)
        {
            uiPDO04.dataType = 0x4;
            sendPDOData(DE_SENDPDO04_FLAG);
            startsend_sem.release();
        }
    }

    if (name==PACKET_PDO_HEPARINPUMP) {

        bool flage=false;

        packetPDOHeparinPump = *(PacketPDOHeparinPump *)dataPtr;

        if (HeparinPump.dose != packetPDOHeparinPump.dose)
        {
            HeparinPump.dose = packetPDOHeparinPump.dose;
            uiPDO05.N1_set_heparin_first = packetPDOHeparinPump.dose;
            flage = true;
        }

        if (HeparinPump.speed != packetPDOHeparinPump.speed)
        {
            HeparinPump.speed = packetPDOHeparinPump.speed;
            uiPDO05.N1_set_heparin_speed = packetPDOHeparinPump.speed;
            flage = true;
        }

        if (flage)
        {
            uiPDO05.dataType = 0x5;
            sendPDOData(DE_SENDPDO05_FLAG);
            startsend_sem.release();
        }
    }

    if (name==PACKET_NAME_AP) {

        packetAP = *(PacketAP*)dataPtr;

        bool flage=false;

        if (APTemp.apL != packetAP.apL) {
            APTemp.apL = packetAP.apL;
            uiPDO06.N1_set_AP_DOWN = (APTemp.apL) + 700;
            flage = true;
        }

        if (APTemp.apUp != packetAP.apUp) {
            APTemp.apUp = packetAP.apUp;
            uiPDO06.N1_set_AP_UP = (APTemp.apUp) + 700;
            flage = true;
        }

        if (flage) {
            uiPDO06.dataType = 0x6;
            sendPDOData(DE_SENDPDO06_FLAG);
            startsend_sem.release();
        }
    }

    if (name==PACKET_NAME_VP) {
        bool flage=false;
        packetVP = *(PacketVP*)dataPtr;

        if (VPTemp.vpL != packetVP.vpL)
        {
            VPTemp.vpL = packetVP.vpL;
            uiPDO07.N1_set_VP_DOWN = (VPTemp.vpL) + 700;
            flage = true;
        }

        if (VPTemp.vpUp != packetVP.vpUp){
            VPTemp.vpUp = packetVP.vpUp;
            uiPDO07.N1_set_VP_UP = (VPTemp.vpUp) + 700;
            flage = true;
        }

        if (flage) {
            uiPDO07.dataType = 0x7;
            sendPDOData(DE_SENDPDO07_FLAG);
            startsend_sem.release();
        }
    }

    if (name==PACKET_NAME_TMP) {

        packetTMP = *(PacketTMP*)dataPtr;

        if (TMPTemp.tmpL != packetTMP.tmpL) {
            TMPTemp.tmpL = packetTMP.tmpL;
            uiPDO08.N1_set_TMP_DOWN = (TMPTemp.tmpL) + 700;
            uiPDO08.dataType = 0x8;
            sendPDOData(DE_SENDPDO08_FLAG);
            startsend_sem.release();
        }

        if (TMPTemp.tmpUp != packetTMP.tmpUp) {
            TMPTemp.tmpUp = packetTMP.tmpUp;
            uiPDO07.N1_set_TMP_UP = (TMPTemp.tmpUp) + 700;
            uiPDO07.dataType = 0x7;
            sendPDOData(DE_SENDPDO07_FLAG);
            startsend_sem.release();
        }
    }

    if (name==PACKET_NAME_COND) {
#if DEBUG_OTHER_FLAG
    Debug_insert("can_sendNAME_COND-in01");
#endif
        packetCond = *(PacketCond*)dataPtr;
        if (CONTemp.condL != packetCond.condL) {
            CONTemp.condL = packetCond.condL;
            uiPDO08.N1_set_Cond_DOWN = CONTemp.condL*100;
            uiPDO08.N1_set_Cond_UP = packetCond.condUp*100;
            uiPDO08.dataType = 0x8;
            sendPDOData(DE_SENDPDO08_FLAG);
            startsend_sem.release();
        }
#if DEBUG_OTHER_FLAG
    Debug_insert("can_sendNAME_COND-out");
#endif
    }

    if (name==PACKET_PDO_SUBBLOODPUMP) {
        packetSubBloodPump = *(PacketSubBloodPump*)dataPtr;
        if (subBPTmp.N10_set_speed !=packetSubBloodPump.N10_set_speed) {
            subBPTmp.N10_set_speed = packetSubBloodPump.N10_set_speed;
            uiPDO0B.dataType = 0x0B;
            uiPDO0B.N1_set_SubBPspeed = subBPTmp.N10_set_speed;
            sendPDOData(DE_SENDPDO0B_FLAG);
            startsend_sem.release();
        }
    }

    if (strcmp(name,PACKET_NAME_WORKMODE) == 0){
        packetWorkMode = *(PacketWorkMode *)dataPtr;
        memset(&uiPDO01.byte2,0, sizeof(uiPDO01.byte2));
        switch (packetWorkMode.treatmode.current)
        {
            case HD_double:
                uiPDO01.byte1.SN_B= 0;//双针
                uiPDO01.byte1.online_B= 0;
                uiPDO01.byte1.Back_in_B = 0;
                uiPDO01.byte2.isouf = 0;
                uiPDO01.byte3.SN_type_B = 1;//双泵
                break;
            case HD_SNDP:
                uiPDO01.byte1.SN_B= 1;//单针
                uiPDO01.byte1.online_B= 0;
                uiPDO01.byte1.Back_in_B = 0;
                uiPDO01.byte2.isouf = 0;
                uiPDO01.byte3.SN_type_B = 1;//双泵
                break;
            case HD_SNSP:
                uiPDO01.byte1.SN_B= 1;
                uiPDO01.byte1.online_B= 0;
                uiPDO01.byte1.Back_in_B = 0;
                uiPDO01.byte2.isouf = 0;
                uiPDO01.byte3.SN_type_B = 0;//单泵
                break;

            case HDF_pre_dilute:
            case HF_pre_dilute:
                uiPDO01.byte1.SN_B= 0;
                uiPDO01.byte1.online_B= 1;
                uiPDO01.byte1.Back_in_B = 0;
                uiPDO01.byte2.isouf = 0;
                break;
            case HDF_post_dilute:
            case HF_post_dilute:
                uiPDO01.byte1.SN_B= 0;
                uiPDO01.byte1.online_B= 1;
                uiPDO01.byte1.Back_in_B = 1;
                uiPDO01.byte2.isouf = 0;
                break;

            case ISOUF_DNDP: //
                uiPDO01.byte1.SN_B= 0;//双针
                uiPDO01.byte1.online_B= 0;//在线
                uiPDO01.byte1.Back_in_B = 0;//0,前置换;1,后置换;
                uiPDO01.byte2.isouf = 1; //单超
                uiPDO01.byte3.SN_type_B = 1;//双泵
                break;
            case ISOUF_SNDP: //
                uiPDO01.byte1.SN_B= 1;//dan针
                uiPDO01.byte1.online_B= 0;//在线
                uiPDO01.byte1.Back_in_B = 0;//0,前置换;1,后置换;
                uiPDO01.byte2.isouf = 1; //单超
                uiPDO01.byte3.SN_type_B = 1;//双泵
                break;
            case ISOUF_SNSP: //
                uiPDO01.byte1.SN_B= 1;//dan针
                uiPDO01.byte1.online_B= 0;//在线
                uiPDO01.byte1.Back_in_B = 0;//0,前置换;1,后置换;
                uiPDO01.byte2.isouf = 1; //单超
                uiPDO01.byte3.SN_type_B = 0;//双泵
                break;
            case PE: //
                uiPDO01.byte1.SN_B= 0;//单针
                uiPDO01.byte1.online_B= 0;//在线
                uiPDO01.byte1.Back_in_B = 0;//0,前置换;1,后置换;
                //uiPDO01.byte2.pe = 1; //pe
                uiPDO01.byte2.isouf = 0; //非单超
                break;
            default:
                uiPDO01.byte1.SN_B= 0;
                uiPDO01.byte1.online_B= 0;
                uiPDO01.byte1.Back_in_B = 0;
                uiPDO01.byte2.isouf = 0;
                break;
        }

        switch (packetWorkMode.fluidmode.current)
        {
            case A_and_B:
                uiPDO01.byte1.B_powder_B= 0;
                uiPDO01.byte1.Acetate_B = 0;
                uiPDO01.byte1.A_center_B = 0;
                break;

            case A_and_BPower:
                uiPDO01.byte1.B_powder_B= 1;
                uiPDO01.byte1.Acetate_B = 0;
                uiPDO01.byte1.A_center_B = 0;
                break;

            case Acetate:
                uiPDO01.byte1.B_powder_B= 0;
                uiPDO01.byte1.Acetate_B = 1;
                uiPDO01.byte1.A_center_B = 0;
                break;
            case Center:
                uiPDO01.byte1.B_powder_B= 0; //0，1干粉
                uiPDO01.byte1.Acetate_B = 0;  //醋酸盐
                uiPDO01.byte1.A_center_B = 1;  //中央供液
                break;
            default:
                uiPDO01.byte1.B_powder_B= 0;
                uiPDO01.byte1.Acetate_B = 0;
                uiPDO01.byte1.A_center_B = 0;
                break;
        }

        uiPDO01.dataType = 0x1;
        uiPDO01.byte4.b1 = 0x1;
        sendPDOData(DE_SENDPDO01_FLAG);
        startsend_sem.release();
    }

    if (strcmp(name, PACKET_NAME_N1STATE) == 0) {
//        packetN1State = *(PacketN1State*)dataPtr;
//        if ( memcmp(&uiPDO0C.state,&packetN1State,sizeof(PacketN1State)) != 0 ) {
//            //uiPDO0C.state=packetN1State;
//            memcpy(&uiPDO0C.state,&packetN1State, sizeof(PacketN1State));
//            mutex.lock();
//            us_sleep(10000);
//            MasterCtrol_SendPDO(&uiPDO0C);
//            mutex.unlock();
//        }
    }
    if (name==Disinfect_Work_Model){

        //switch ( * ( (DisinfectWorkModel *) dataPtr ) )
        if( Disinfect.step==loop)
          uiPDO09.N1_set_QJspeed= Disinfect.loop_flux;
        else
          uiPDO09.N1_set_QJspeed= Disinfect.flush_flux;

        uiPDO09.N1_set_QJcond_UP=Disinfect.cond_up;
        uiPDO09.N1_set_QJcond_DOWN=Disinfect.cond_down;

        if(Disinfect.step==preflush|| Disinfect.step==hold || Disinfect.step==backflush
                ||Disinfect.step==hotrinse3)
            uiPDO0A.N1_set_QJtemp=0;
        else
            uiPDO0A.N1_set_QJtemp=Disinfect.targetTemp;
        uiPDO0A.N1_set_QJsuck=Disinfect.suck_Volume;
        sendPDOData(DE_SENDPDO09_FLAG);
        sendPDOData(DE_SENDPDO0A_FLAG);
        startsend_sem.release();
    }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("can_send-ActionHandle-out");
#endif
}

#elif HJX_COMMUNICATION_SWITCH==0
DataCanSend::DataCanSend(QObject *parent) :
    QObject(parent),
    sendPDOTimer(parent)
{
    mutex.unlock();
    //进行各个PDO结构的初始化#/
    memset(&uiPDO01,0,sizeof(PackN1PDO01H));
    memset(&uiPDO02,0,sizeof(PackN1PDO02H));
    memset(&uiPDO03,0,sizeof(PackN1PDO03H));
    memset(&uiPDO04,0,sizeof(PackN1PDO04H));
    memset(&uiPDO05,0,sizeof(PackN1PDO05H));
    memset(&uiPDO06,0,sizeof(PackN1PDO06H));
    memset(&uiPDO07,0,sizeof(PackN1PDO07H));
    memset(&uiPDO08,0,sizeof(PackN1PDO08H));

    //dataType是对应各个设备节点的标示符#/
    //因为主机发送的PDO设置为各个节点设备均可接收到#/
    uiPDO01.dataType = 0x1;
    uiPDO02.dataType = 0x2;
    uiPDO03.dataType = 0x3;
    uiPDO04.dataType = 0x4;
    uiPDO05.dataType = 0x5;
    uiPDO06.dataType = 0x6;
    uiPDO07.dataType = 0x7;
    uiPDO08.dataType = 0x8;

    uiPDO09.dataType = 0x9;
    uiPDO09.N1_set_QJspeed = 500;
    uiPDO09.N1_set_QJcond_UP = 0;
    uiPDO09.N1_set_QJcond_DOWN = 0;

    uiPDO0A.dataType = 0xA;
    uiPDO0A.N1_set_QJtemp = 0;
    uiPDO0A.N1_set_QJsuck = 100;

    uiPDO0B.dataType = 0xB;
    uiPDO0B.N1_set_SubBPspeed = 0;

    //memset(&uiPDO0C,0,sizeof(uiPDO0C));
    //uiPDO0C.dataType = 0xC;
    memset(&PDOstate,0,sizeof(PDOstate));
    PDOstate.dataType = 0xC;

    //每一秒，发送PDO参数一次#/
    connect(&sendPDOTimer,SIGNAL(timeout()),this,SLOT(sendPDOData()),Qt::QueuedConnection);

    sendPDOTimer.start(1000);
}

void DataCanSend::init()
{
    //为这些名称的数据管理包，添加操作处理#/
    dataNodeAddObject(PACKET_NAME_AP, this);
    dataNodeAddObject(PACKET_NAME_VP, this);
    dataNodeAddObject(PACKET_NAME_TMP, this);
    dataNodeAddObject(PACKET_NAME_COND, this);
    dataNodeAddObject(PACKET_PDO_HEPARINPUMP, this);
    dataNodeAddObject(PACKET_NAME_DIALYSATE, this);
    dataNodeAddObject(Disinfect_Work_Model, this);
    dataNodeAddObject(PACKET_SDO_BLOOD, this);
    dataNodeAddObject(PACKET_FORMULA_TARGET, this);
    dataNodeAddObject(PACKET_NAME_WORKMODE, this);
    dataNodeAddObject(PACKET_NAME_N1STATE, this);
    /**/
    dataNodeAddObject(PACKET_NAME_UF, this);
    dataNodeAddObject(PACKET_PDO_SUBBLOODPUMP, this);
}

void DataCanSend::sendPDOData()
{
//#if 0

    mutex.lock();
    us_sleep(10000);
    MasterCtrol_SendPDO(&uiPDO01);
    mutex.unlock();

    mutex.lock();
    us_sleep(10000);
    MasterCtrol_SendPDO(&uiPDO02);
    mutex.unlock();

    mutex.lock();
    us_sleep(10000);
    MasterCtrol_SendPDO(&uiPDO03);
    mutex.unlock();

    mutex.lock();
    us_sleep(10000);

   /* PacketUF packetPDO;
    dataRead(PACKET_NAME_UF,&packetPDO);
    if(residue_secCount>0&&(packetPDO.ufVolAim-G_UF_CUREFINISH>0)){
       uiPDO04.N1_set_UFspeed = (packetPDO.ufVolAim-G_UF_CUREFINISH)*3600/residue_secCount;
       if(residue_secCount<5)uiPDO04.N1_set_UFspeed+=(uiPDO04.N1_set_UFspeed/5);
    }
    else{
        uiPDO04.N1_set_UFspeed = 0;
    }
    if(uiPDO04.N1_set_UFspeed  > 6000){
        uiPDO04.N1_set_UFspeed = 6000;//Params.UF.UF_Rate.attribute.max_value;
    }*/

    MasterCtrol_SendPDO(&uiPDO04);
    mutex.unlock();

    mutex.lock();
    us_sleep(10000);
    MasterCtrol_SendPDO(&uiPDO05);
    mutex.unlock();

    mutex.lock();
    us_sleep(10000);
    MasterCtrol_SendPDO(&uiPDO06);
    mutex.unlock();

    mutex.lock();
    us_sleep(10000);
    MasterCtrol_SendPDO(&uiPDO07);
    mutex.unlock();

    mutex.lock();
    us_sleep(10000);
    MasterCtrol_SendPDO(&uiPDO08);
    mutex.unlock();

    mutex.lock();
    us_sleep(10000);
    MasterCtrol_SendPDO(&uiPDO09);
    mutex.unlock();

    mutex.lock();
    us_sleep(10000);
    MasterCtrol_SendPDO(&uiPDO0A);
    mutex.unlock();

    mutex.lock();
    us_sleep(10000);
    MasterCtrol_SendPDO(&uiPDO0B);
    mutex.unlock();

    mutex.lock();
    us_sleep(10000);
    MasterCtrol_SendPDO(&PDOstate); //发送主控板PDO#/
    mutex.unlock();
//#endif
}

void DataCanSend::ActionHandle(const void *dataPtr, const char *name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("can_send-ActionHandle-in");
#endif

    if ((dataPtr == NULL) || (name == NULL)) {
        return;
    }

    if (name==PACKET_SDO_BLOOD) {
            SdoBlood = *(PacketSdoBlood*)dataPtr;

            uiPDO02.N1_set_BPspeed = SdoBlood.N3_set_BPspeed;
            uiPDO02.N1_set_BPtube = SdoBlood.N3_set_BPtube;

            mutex.lock();
            us_sleep(10000);
            MasterCtrol_SendPDO(&uiPDO02);
            mutex.unlock();
    }

    if (strcmp(name,PACKET_NAME_DIALYSATE) == 0) {
        bool flage = false;
        packetFluid = *(PacketFluid *)dataPtr;

        if (fluidTemp.tempTarget != packetFluid.tempTarget)
        {
            fluidTemp.tempTarget = packetFluid.tempTarget;
            uiPDO02.N1_set_temp = fluidTemp.tempTarget;
//            uiPDO02.N1_set_temp *= 10;
            uiPDO05.N1_set_temp_UP= uiPDO02.N1_set_temp+20;
            uiPDO06.N1_set_temp_DOWN= uiPDO02.N1_set_temp-20;
            flage = true;
        }

        if (fluidTemp.Wspeed != packetFluid.Wspeed){
            fluidTemp.Wspeed = packetFluid.Wspeed;
            uiPDO02.N1_set_Wspeed = fluidTemp.Wspeed;
            flage = true;
        }

        if (flage) {
            uiPDO02.dataType = 0x2;

            mutex.lock();
            us_sleep(10000);
            MasterCtrol_SendPDO(&uiPDO02);
            mutex.unlock();
        }
    }

    if (strcmp(name,PACKET_FORMULA_TARGET) == 0) {

        bool flage = false;
        formulaSet = *(FormulaSet *)dataPtr;

        if (formulaSetTemp.outTarget.scale_b != formulaSet.outTarget.scale_b)
        {
            formulaSetTemp.outTarget.scale_b = formulaSet.outTarget.scale_b;
            uiPDO03.N1_set_Bscale = formulaSetTemp.outTarget.scale_b;
            flage = true;
        }

        if (formulaSetTemp.outTarget.scale_water != formulaSet.outTarget.scale_water)
        {
            formulaSetTemp.outTarget.scale_water = formulaSet.outTarget.scale_water;
            uiPDO03.N1_set_Wscale = formulaSetTemp.outTarget.scale_water;
            flage = true;
        }

        if (formulaSetTemp.outTarget.target_b != formulaSet.outTarget.target_b)
        {
            formulaSetTemp.outTarget.target_b = formulaSet.outTarget.target_b;
            uiPDO03.N1_set_B = formulaSetTemp.outTarget.target_b;
            flage = true;
        }

        if (formulaSetTemp.outTarget.target_mix != formulaSet.outTarget.target_mix)
        {
            formulaSetTemp.outTarget.target_mix = formulaSet.outTarget.target_mix;
            uiPDO04.N1_set_mix = formulaSetTemp.outTarget.target_mix;
            flage = true;
        }

        if (formulaSet.tempBuffer.bnacl>0)
        {
            uiPDO01.byte2.pe = 1;
            flage = true;
        }
        else if(formulaSet.tempBuffer.bnacl==0)
        {
            uiPDO01.byte2.pe = 0;
            flage = true;
        }

        if (flage){

            uiPDO03.dataType = 0x3;
            uiPDO04.dataType = 0x4;

            mutex.lock();
            us_sleep(10000);
            MasterCtrol_SendPDO(&uiPDO03);
            mutex.unlock();

            mutex.lock();
            us_sleep(10000);
            MasterCtrol_SendPDO(&uiPDO04);
            mutex.unlock();

            mutex.lock();
            us_sleep(10000);
            MasterCtrol_SendPDO(&uiPDO01);
            mutex.unlock();
        }
    }

    if (strcmp(name,PACKET_NAME_UF) == 0){
        bool flage = false;
        packetUF = *(PacketUF *)dataPtr;

        if(packetUF.chooseRate){
            if(ufTmp.ufRate != packetUF.ufRate)
            {//选用的治疗超滤泵速度有改变则更新超滤泵的速度#/
                ufTmp.ufRate = packetUF.ufRate;
                uiPDO04.N1_set_UFspeed = ufTmp.ufRate;
                flage = true;
            }
        }
        else if(ufTmp.preRate != packetUF.preRate)
        {//选用的预冲超滤泵速度有改变则更新超滤泵的速度#/
            ufTmp.preRate = packetUF.preRate;
            uiPDO04.N1_set_UFspeed = ufTmp.preRate;
            flage = true;
        }

        if (ufTmp.ufVolAim != packetUF.ufVolAim)
        {
            ufTmp.ufVolAim = packetUF.ufVolAim;
            uiPDO04.N1_set_UFtarget = ufTmp.ufVolAim;
            flage = true;
        }

        if (flage)
        {
            uiPDO04.dataType = 0x4;

            mutex.lock();
            us_sleep(10000);
            MasterCtrol_SendPDO(&uiPDO04);
            mutex.unlock();
        }
    }

    if (name==PACKET_PDO_HEPARINPUMP) {

        bool flage=false;

        packetPDOHeparinPump = *(PacketPDOHeparinPump *)dataPtr;

        if (HeparinPump.dose != packetPDOHeparinPump.dose)
        {
            HeparinPump.dose = packetPDOHeparinPump.dose;
            uiPDO05.N1_set_heparin_first = packetPDOHeparinPump.dose;
            flage = true;
        }

        if (HeparinPump.speed != packetPDOHeparinPump.speed)
        {
            HeparinPump.speed = packetPDOHeparinPump.speed;
            uiPDO05.N1_set_heparin_speed = packetPDOHeparinPump.speed;
            flage = true;
        }

        if (flage)
        {
            uiPDO05.dataType = 0x5;

            mutex.lock();
            us_sleep(10000);
            MasterCtrol_SendPDO(&uiPDO05);
            mutex.unlock();
        }
    }

    if (name==PACKET_NAME_AP) {

        packetAP = *(PacketAP*)dataPtr;

        bool flage=false;

        if (APTemp.apL != packetAP.apL) {
            APTemp.apL = packetAP.apL;
            uiPDO06.N1_set_AP_DOWN = (APTemp.apL) + 700;
            flage = true;
        }

        if (APTemp.apUp != packetAP.apUp) {
            APTemp.apUp = packetAP.apUp;
            uiPDO06.N1_set_AP_UP = (APTemp.apUp) + 700;
            flage = true;
        }

        if (flage) {
            uiPDO06.dataType = 0x6;

            mutex.lock();
            us_sleep(10000);
            MasterCtrol_SendPDO(&uiPDO06);
            mutex.unlock();
        }
    }

    if (name==PACKET_NAME_VP) {
        bool flage=false;
        packetVP = *(PacketVP*)dataPtr;

        if (VPTemp.vpL != packetVP.vpL)
        {
            VPTemp.vpL = packetVP.vpL;
            uiPDO07.N1_set_VP_DOWN = (VPTemp.vpL) + 700;
            flage = true;
        }

        if (VPTemp.vpUp != packetVP.vpUp){
            VPTemp.vpUp = packetVP.vpUp;
            uiPDO07.N1_set_VP_UP = (VPTemp.vpUp) + 700;
            flage = true;
        }

        if (flage) {
            uiPDO07.dataType = 0x7;

            mutex.lock();
            us_sleep(10000);
            MasterCtrol_SendPDO(&uiPDO07);
            mutex.unlock();
        }
    }

    if (name==PACKET_NAME_TMP) {

        packetTMP = *(PacketTMP*)dataPtr;

        if (TMPTemp.tmpL != packetTMP.tmpL) {
            TMPTemp.tmpL = packetTMP.tmpL;
            uiPDO08.N1_set_TMP_DOWN = (TMPTemp.tmpL) + 700;
            uiPDO08.dataType = 0x8;

            mutex.lock();
            us_sleep(10000);
            MasterCtrol_SendPDO(&uiPDO08);
            mutex.unlock();
        }

        if (TMPTemp.tmpUp != packetTMP.tmpUp) {
            TMPTemp.tmpUp = packetTMP.tmpUp;
            uiPDO07.N1_set_TMP_UP = (TMPTemp.tmpUp) + 700;
            uiPDO07.dataType = 0x7;

            mutex.lock();
            us_sleep(10000);
            MasterCtrol_SendPDO(&uiPDO07);
            mutex.unlock();
        }
    }

    if (name==PACKET_NAME_COND) {
#if DEBUG_OTHER_FLAG
    Debug_insert("can_sendNAME_COND-in01");
#endif
        packetCond = *(PacketCond*)dataPtr;
        if (CONTemp.condL != packetCond.condL) {
            CONTemp.condL = packetCond.condL;
            uiPDO08.N1_set_Cond_DOWN = CONTemp.condL*100;
            uiPDO08.N1_set_Cond_UP = packetCond.condUp*100;
            uiPDO08.dataType = 0x8;
            mutex.lock();
            us_sleep(10000);
            MasterCtrol_SendPDO(&uiPDO08);
            mutex.unlock();
        }
#if DEBUG_OTHER_FLAG
    Debug_insert("can_sendNAME_COND-out");
#endif
    }

    if (name==PACKET_PDO_SUBBLOODPUMP) {
        packetSubBloodPump = *(PacketSubBloodPump*)dataPtr;
        if (subBPTmp.N10_set_speed !=packetSubBloodPump.N10_set_speed) {
            subBPTmp.N10_set_speed = packetSubBloodPump.N10_set_speed;
            uiPDO0B.dataType = 0x0B;
            uiPDO0B.N1_set_SubBPspeed = subBPTmp.N10_set_speed;
            mutex.lock();
            us_sleep(10000);
            MasterCtrol_SendPDO(&uiPDO0B);
            mutex.unlock();
        }
    }

    if (strcmp(name,PACKET_NAME_WORKMODE) == 0){
        packetWorkMode = *(PacketWorkMode *)dataPtr;
        memset(&uiPDO01.byte2,0, sizeof(uiPDO01.byte2));
        switch (packetWorkMode.treatmode.current)
        {
            case HD_double:
                uiPDO01.byte1.SN_B= 0;//双针
                uiPDO01.byte1.online_B= 0;
                uiPDO01.byte1.Back_in_B = 0;
                uiPDO01.byte2.isouf = 0;
                uiPDO01.byte3.SN_type_B = 1;//双泵
                break;
            case HD_SNDP:
                uiPDO01.byte1.SN_B= 1;//单针
                uiPDO01.byte1.online_B= 0;
                uiPDO01.byte1.Back_in_B = 0;
                uiPDO01.byte2.isouf = 0;
                uiPDO01.byte3.SN_type_B = 1;//双泵
                break;
            case HD_SNSP:
                uiPDO01.byte1.SN_B= 1;
                uiPDO01.byte1.online_B= 0;
                uiPDO01.byte1.Back_in_B = 0;
                uiPDO01.byte2.isouf = 0;
                uiPDO01.byte3.SN_type_B = 0;//单泵
                break;

            case HDF_pre_dilute:
            case HF_pre_dilute:
                uiPDO01.byte1.SN_B= 0;
                uiPDO01.byte1.online_B= 1;
                uiPDO01.byte1.Back_in_B = 0;
                uiPDO01.byte2.isouf = 0;
                break;
            case HDF_post_dilute:
            case HF_post_dilute:
                uiPDO01.byte1.SN_B= 0;
                uiPDO01.byte1.online_B= 1;
                uiPDO01.byte1.Back_in_B = 1;
                uiPDO01.byte2.isouf = 0;
                break;

            case ISOUF_DNDP: //
                uiPDO01.byte1.SN_B= 0;//双针
                uiPDO01.byte1.online_B= 0;//在线
                uiPDO01.byte1.Back_in_B = 0;//0,前置换;1,后置换;
                uiPDO01.byte2.isouf = 1; //单超
                uiPDO01.byte3.SN_type_B = 1;//双泵
                break;
            case ISOUF_SNDP: //
                uiPDO01.byte1.SN_B= 1;//dan针
                uiPDO01.byte1.online_B= 0;//在线
                uiPDO01.byte1.Back_in_B = 0;//0,前置换;1,后置换;
                uiPDO01.byte2.isouf = 1; //单超
                uiPDO01.byte3.SN_type_B = 1;//双泵
                break;
            case ISOUF_SNSP: //
                uiPDO01.byte1.SN_B= 1;//dan针
                uiPDO01.byte1.online_B= 0;//在线
                uiPDO01.byte1.Back_in_B = 0;//0,前置换;1,后置换;
                uiPDO01.byte2.isouf = 1; //单超
                uiPDO01.byte3.SN_type_B = 0;//双泵
                break;
            case PE: //
                uiPDO01.byte1.SN_B= 0;//单针
                uiPDO01.byte1.online_B= 0;//在线
                uiPDO01.byte1.Back_in_B = 0;//0,前置换;1,后置换;
                //uiPDO01.byte2.pe = 1; //pe
                uiPDO01.byte2.isouf = 0; //非单超
                break;
            default:
                uiPDO01.byte1.SN_B= 0;
                uiPDO01.byte1.online_B= 0;
                uiPDO01.byte1.Back_in_B = 0;
                uiPDO01.byte2.isouf = 0;
                break;
        }

        switch (packetWorkMode.fluidmode.current)
        {
            case A_and_B:
                uiPDO01.byte1.B_powder_B= 0;
                uiPDO01.byte1.Acetate_B = 0;
                uiPDO01.byte1.A_center_B = 0;
                break;

            case A_and_BPower:
                uiPDO01.byte1.B_powder_B= 1;
                uiPDO01.byte1.Acetate_B = 0;
                uiPDO01.byte1.A_center_B = 0;
                break;

            case Acetate:
                uiPDO01.byte1.B_powder_B= 0;
                uiPDO01.byte1.Acetate_B = 1;
                uiPDO01.byte1.A_center_B = 0;
                break;
            case Center:
                uiPDO01.byte1.B_powder_B= 0; //0，1干粉
                uiPDO01.byte1.Acetate_B = 0;  //醋酸盐
                uiPDO01.byte1.A_center_B = 1;  //中央供液
                break;
            default:
                uiPDO01.byte1.B_powder_B= 0;
                uiPDO01.byte1.Acetate_B = 0;
                uiPDO01.byte1.A_center_B = 0;
                break;
        }
        uiPDO01.dataType = 0x1;
        uiPDO01.byte4.b1 = 0x1;
        mutex.lock();
        us_sleep(10000);
        MasterCtrol_SendPDO(&uiPDO01);
        mutex.unlock();
    }

    if (strcmp(name, PACKET_NAME_N1STATE) == 0) {
//        packetN1State = *(PacketN1State*)dataPtr;
//        if ( memcmp(&uiPDO0C.state,&packetN1State,sizeof(PacketN1State)) != 0 ) {
//            //uiPDO0C.state=packetN1State;
//            memcpy(&uiPDO0C.state,&packetN1State, sizeof(PacketN1State));
//            mutex.lock();
//            us_sleep(10000);
//            MasterCtrol_SendPDO(&uiPDO0C);
//            mutex.unlock();
//        }
    }
    if (name==Disinfect_Work_Model){

        //switch ( * ( (DisinfectWorkModel *) dataPtr ) )
        if( Disinfect.step==loop)
          uiPDO09.N1_set_QJspeed= Disinfect.loop_flux;
        else
          uiPDO09.N1_set_QJspeed= Disinfect.flush_flux;

        uiPDO09.N1_set_QJcond_UP=Disinfect.cond_up;
        uiPDO09.N1_set_QJcond_DOWN=Disinfect.cond_down;

        if(Disinfect.step==preflush|| Disinfect.step==hold || Disinfect.step==backflush
                ||Disinfect.step==hotrinse3)
            uiPDO0A.N1_set_QJtemp=0;
        else
            uiPDO0A.N1_set_QJtemp=Disinfect.targetTemp;
        uiPDO0A.N1_set_QJsuck=Disinfect.suck_Volume;
    }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("can_send-ActionHandle-out");
#endif
}

#endif
