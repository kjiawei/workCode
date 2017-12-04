#include "canopen_shell.h"
#include "data_manage_can.h"
#include "data_sqllite.h"
#include "common_func.h"
#include "window_boot.h"


#define OPEN_BUFFER_HANDLE  1  // 0：接收到立刻处理  1：接收到定时处理#/



//这个文件，主要是分发和处理can数据#/

//MessageSignal类是一个只发送信号的类，其将底层CAN数据包作为参数#/
//其中的Message是底层CAN数据包结构体，即标示符+控制域+数据域#/
extern MessageSignal messageSignal;

void DataCan::Init(void)
{
    isShowMain = 0;



    //根据数据包结构体和自定义关键字，创建数据节点类，并将创建的数据包指针返回给定的指针变量#/


    battryNode = dataNodeAppend(PACKET1_NAME_BATTERY,
            (void **)&battryData, sizeof(PacketBattry));

    powerNode = dataNodeAppend(PACKET_NAME_POWER,
            (void **)&powerData, sizeof(PacketPower));

    heparinNode = dataNodeAppend(PACKET_NAME_HEPARINDATA,
            (void **)&heparinData, sizeof(PackN15PDO));

    //血泵1#/
    bloodPumpNode = dataNodeAppend(PACKET_NAME_BLOODPUMP,
            (void **)&bloodPumpData, sizeof(PacketBloodPump));
    bloodPumpNodeTube = dataNodeAppend(PACKET_NAME_BLOODPUMPTUBE,
            (void **)&bloodPumpTubeData, sizeof(PacketBloodPumpTube));
    //血泵2#/
    subbloodPumpNode = dataNodeAppend(PACKET_NAME_SUBBLOODPUMP,
            (void **)&subbloodPumpData, sizeof(PacketBloodPump));
    subbloodPumpNodeTube = dataNodeAppend(PACKET_NAME_SUBBLOODPUMPTUBE,
            (void **)&subbloodPumpTubeData, sizeof(PacketBloodPumpTube));
    //主控板#/
    masterCtrlValveNode = dataNodeAppend(PACKET_MASTERCTRL_VALVE,
            (void **)&masterCtrlValve, sizeof(PacketValve));
    masterCtrlPdoErrNode = dataNodeAppend(PACKET_MASTERCTRL_PDOERR,
            (void **)&masterCtrlPdoErrValue,sizeof(PacketPdoErr));

    masterCtrlOutletNode = dataNodeAppend(PACKET_MASTERCTRL_OUTLET,
            (void **)&masterCtrlOutlet, sizeof(PacketOutlet));
    masterCtrlMixNode = dataNodeAppend(PACKET_MASTERCTRL_MIX,
            (void **)&masterCtrlMix, sizeof(PacketMix));
    masterCtrlFluidBNode = dataNodeAppend(PACKET_MASTERCTRL_FLUIDB,
            (void **)&masterCtrlFluidB, sizeof(PacketFluidB));
    masterCtrlFluidAfterNode = dataNodeAppend(PACKET_MASTERCTRL_FLUIDAFTER,
            (void **)&masterCtrlFluidAfter, sizeof(PacketFluidAfter));
    masterCtrlUfNode = dataNodeAppend(PACKET_MASTERCTRL_UFVALUE,
            (void **)&masterCtrlUfValue, sizeof(PacketUfValue));
    masterCtrlUfPumpNode = dataNodeAppend(PACKET_MASTERCTRL_UFPUMP,
            (void **)&masterCtrlUfPump, sizeof(PacketUfPump));
    masterCtrlHeatNode = dataNodeAppend(PACKET_MASTERCTRL_HEAT,
            (void **)&masterCtrlHeat, sizeof(PacketHeat));
    masterCtrlBPumpNode = dataNodeAppend(PACKET_MASTERCTRL_BPUMP,
            (void **)&masterCtrlBPump, sizeof(PacketBPump));
    masterCtrlSdoNode = dataNodeAppend(PACKET_MASTERCTRL_SDO,
            (void **)&masterCtrlSdo, sizeof(PacketMasterCtrlSdo));
    masterCtrlMixPumpNode = dataNodeAppend(PACKET_MASTERCTRL_MIXPUMP,
            (void **)&masterCtrlMixPump, sizeof(PacketMixPump));
    masterCtrlDisplayNode = dataNodeAppend(PACKET_MASTERCTRL_DISPLAY,
            (void **)&masterCtrlDisplay, sizeof(PacketDisplay));
    masterCtrlDisplayNodeTable = dataNodeAppend(PACKET_MASTERCTRL_DISPLAY_TABLE,
            (void **)&masterCtrlDisplayTable, sizeof(PacketDisplay));
    masterCtrlAntrumNode = dataNodeAppend(PACKET_MASTERCTRL_ANTRUM,  //主控板-腔数量#/
            (void **)&masterCtrlAntrum, sizeof(PacketAntrum));
    masterCtrlNowaterNode = dataNodeAppend(PACKET_MASTERCTRL_NOWATER,  //主控板-无液时间#/
            (void **)&masterCtrlNowater, sizeof(PacketNowater));
//监控板#/
    monitorWaterOutNode = dataNodeAppend(PACKET_MONITOR_WATER,
            (void **)&monitorWaterOut, sizeof(PacketWaterOut));
    monitorPresstNode = dataNodeAppend(PACKET_MONITOR_PRESS,
            (void **)&monitorPress, sizeof(PacketPress));
    monitorBloodStateNode = dataNodeAppend(PACKET_MONITOR_BLOODST,
            (void **)&monitorBloodState, sizeof(PacketBloodState));
    monitorBPMStateNode = dataNodeAppend(PACKET_MONITOR_BPMST,
            (void **)&monitorBPMState, sizeof(PacketBPMState));
    monitorBPMMiscNode = dataNodeAppend(PACKET_MONITOR_BPMMISC,
            (void **)&monitorBPMMisc, sizeof(PacketBPMMisc));

    //tmp#/
    statusNoteNode = dataNodeAppend(PACKET_STATUS_NOTE,
            (void **)&statusNoteData,sizeof(PacketStatusNote));

    statusAlarmNode = dataNodeAppend(PACKET_STATUS_ALARM,//监控板返回的报警状态#/
            (void **)&statusAlarmData,sizeof(PacketStatusAlarm));

    status_MastAlarmNode = dataNodeAppend(PACKET_STATUS_MASTALARM,//主控板PDO1返回的报警状态#/
            (void **)&status_MastAlarmData,sizeof(PacketStatusAlarm));


    //血路ok标志#/
    dataNodeAppend(PACKET_BLOODROAD_OK,NULL,sizeof(int));
    bloodroadOk = 0;
    dataNodeUpdate(PACKET_BLOODROAD_OK,&bloodroadOk);

    //水路ok标志#/
    dataNodeAppend(PACKET_WATERROAD_OK,NULL,sizeof(int));
    waterroadOk = 0;
    dataNodeUpdate(PACKET_WATERROAD_OK,&waterroadOk);

    //静脉有汽泡#/
    dataNodeAppend(VEINS_HAVE_BLOOD,NULL,sizeof(int));
    veinsHaveBloood = 1;
    dataNodeUpdate(VEINS_HAVE_BLOOD,&veinsHaveBloood);
    //液位#/
    dataNodeAppend(BLOOD_LEVEL_OK,NULL,sizeof(int));
    bloodlevel = 1;
    dataNodeUpdate(BLOOD_LEVEL_OK,&bloodlevel);


    connect(&messageSignal, SIGNAL(DataComeSignal(const Message&)),
            this, SLOT(DataDispatch(const Message&)), Qt::DirectConnection);

    /*
    connect(&messageSignal, SIGNAL(DataComeSignal(const Message&)),
            this, SLOT(DataDispatch(const Message&)), Qt::QueuedConnection);
              */

}

//初始化can#/
void DataCan::Open(void)
{
    //进行canopen初始化操作#/
    CanOpen_Master_Init();

    this->start();

#if OPEN_BUFFER_HANDLE==1
    timer->createTimer(this, 1000);//定时处理#/
#endif


}

//退出can#/
void DataCan::Quit(void)
{
    CanOpen_Master_Quit();
}

//处理主控板的can数据，只能对主控板发送的PDO进行处理，因为SDO的话，其data[0]=命令字#/
//具体处理看主控板协议#/
void DataCan::DataDispatchMasterCtrl(const char *data)
{
    //if(data[0]>16)return;
    //data【0】为主控板协议中的BYTE0#/
    switch (data[0]) {
        case 0:                 //自己定义的协议#/
            isShowMain =data[1];
            break;

        case 1://血路和水路#/

            if(isShowMain == 1) {   //为了同步进入主界面后，才接收主控板的信息#/
                if ( memcmp(statusNoteData,data,8) != 0 ){

#if OPEN_BUFFER_HANDLE==1
                    memcpy(statusNoteData,data,8);
                    QList_1s.append(PACKET_STATUS_NOTE);
#elif OPEN_BUFFER_HANDLE==0
                    memcpy(statusNoteData,data,8);
                    statusNoteNode->Update();    //主控板的PDO0外发#/

                    //血路准备好#/
                    if ((statusNoteData->note.note4&0x10)==0x10 && bloodroadOk !=1)//tmp == 0x13
                    {
                        bloodroadOk = 1;
                        dataNodeUpdate(PACKET_BLOODROAD_OK,&bloodroadOk);
                    }

                    //水路准备好#/
                    if ((statusNoteData->note.note4&0x20)==0x20 && waterroadOk !=1) //tmp == 0x14
                    {
                        waterroadOk = 1;
                        dataNodeUpdate(PACKET_WATERROAD_OK,&waterroadOk);
                    }
#endif
                }
            }
            break;

        case 2: //告警内容 对应主控板PDO1#/
            if (memcmp(status_MastAlarmData, data, 8)!= 0) {

#if OPEN_BUFFER_HANDLE==1
                memcpy(status_MastAlarmData, data, 8);
                QList_1s.append(PACKET_STATUS_MASTALARM);
#elif OPEN_BUFFER_HANDLE==0
                global_mutex.lock();
                memcpy(status_MastAlarmData, data, 8);
                status_MastAlarmNode->Update();
                global_mutex.unlock();
#endif
            }
            break;

        case 3:
            if (memcmp(masterCtrlValve, data, 8)!= 0)
            {
                memcpy(masterCtrlValve, data, 8);

#if OPEN_BUFFER_HANDLE==1
                QList_1s.append(PACKET_MASTERCTRL_VALVE);
#elif OPEN_BUFFER_HANDLE==0
                masterCtrlValveNode->Update();
#endif
            }
            break;

    case 4:
            if(memcmp(masterCtrlPdoErrValue,data,7)!=0)
            {
                memcpy(masterCtrlPdoErrValue,data,8);

#if OPEN_BUFFER_HANDLE==1
                QList_1s.append(PACKET_MASTERCTRL_PDOERR);
#elif OPEN_BUFFER_HANDLE==0
                masterCtrlPdoErrNode->Update();
#endif
            }
        break;

        case 10:
            if (isShowMain == 1){

                if (memcmp(masterCtrlOutlet, data, 8)!= 0)
                {
                    memcpy(masterCtrlOutlet, data, 8);                                                      
#if OPEN_BUFFER_HANDLE==1
                    QList_1s.append(PACKET_MASTERCTRL_OUTLET);
#elif OPEN_BUFFER_HANDLE==0
                    masterCtrlOutletNode->Update();
#endif
                }
            }
            break;
        case 11:
            if (memcmp(masterCtrlFluidB, data, 8)!= 0) {
                memcpy(masterCtrlFluidB, data, 8);
#if OPEN_BUFFER_HANDLE==1
                    QList_1s.append(PACKET_MASTERCTRL_FLUIDB);
#elif OPEN_BUFFER_HANDLE==0
                    masterCtrlFluidBNode->Update();
#endif
            }
            break;
        case 12:
            if (memcmp(masterCtrlMix, data, 8)!= 0) {
                memcpy(masterCtrlMix, data, 8);                                
#if OPEN_BUFFER_HANDLE==1
                QList_1s.append(PACKET_MASTERCTRL_MIX);
#elif OPEN_BUFFER_HANDLE==0
                masterCtrlMixNode->Update();
#endif
            }
            break;
        case 13:
            if (memcmp(masterCtrlFluidAfter, data, 6)!= 0) {
                memcpy(masterCtrlFluidAfter, data, 8);
#if OPEN_BUFFER_HANDLE==1
                QList_1s.append(PACKET_MASTERCTRL_FLUIDAFTER);
#elif OPEN_BUFFER_HANDLE==0
                masterCtrlFluidAfterNode->Update();
#endif
            }
            break;
        case 14:
            if (memcmp(masterCtrlUfValue, data, 8)!= 0) {
                memcpy(masterCtrlUfValue, data, 8);
#if OPEN_BUFFER_HANDLE==1
                QList_1s.append(PACKET_MASTERCTRL_UFVALUE);
#elif OPEN_BUFFER_HANDLE==0
                masterCtrlUfNode->Update();
#endif
            }
            break;
        case 15:
            if (memcmp(masterCtrlUfPump, data, 8)!= 0) {
                memcpy(masterCtrlUfPump, data, 8);
#if OPEN_BUFFER_HANDLE==1
                QList_1s.append(PACKET_MASTERCTRL_UFPUMP);
#elif OPEN_BUFFER_HANDLE==0
                masterCtrlUfPumpNode->Update();
#endif
            }
            break;

        case 16:
            if (memcmp(masterCtrlHeat, data, 6)!= 0) {
                memcpy(masterCtrlHeat, data, 8);
#if OPEN_BUFFER_HANDLE==1
                QList_1s.append(PACKET_MASTERCTRL_HEAT);
#elif OPEN_BUFFER_HANDLE==0
                masterCtrlHeatNode->Update();
#endif
            }
            break;

        case 17:
            if (memcmp(masterCtrlBPump, data, 8)!= 0) {
                memcpy(masterCtrlBPump, data, 8);
#if OPEN_BUFFER_HANDLE==1
                QList_1s.append(PACKET_MASTERCTRL_BPUMP);
#elif OPEN_BUFFER_HANDLE==0
                masterCtrlBPumpNode->Update();
#endif
            }
            break;

        case 18:
            if (memcmp(masterCtrlMixPump, data, 6)!= 0) {
                memcpy(masterCtrlMixPump, data, 8);
#if OPEN_BUFFER_HANDLE==1
                QList_1s.append(PACKET_MASTERCTRL_MIXPUMP);
#elif OPEN_BUFFER_HANDLE==0
                masterCtrlMixPumpNode->Update();
#endif
            }
            break;
        case 19:
            if (memcmp(masterCtrlDisplay, data, 8)!= 0) {
                memcpy(masterCtrlDisplay, data, 8);
#if OPEN_BUFFER_HANDLE==1
                QList_1s.append(PACKET_MASTERCTRL_DISPLAY);
#elif OPEN_BUFFER_HANDLE==0
                masterCtrlDisplayNode->Update();
#endif
            }
            break;
    case 32://20H
        if (memcmp(masterCtrlAntrum, data, 8)!= 0) {
            memcpy(masterCtrlAntrum, data, 8);
#if OPEN_BUFFER_HANDLE==1
            QList_1s.append(PACKET_MASTERCTRL_ANTRUM);
#elif OPEN_BUFFER_HANDLE==0
            masterCtrlAntrumNode->Update();
#endif
        }
        break;
    case 33://21H
        if (memcmp(masterCtrlNowater, data, 8)!= 0) {
            memcpy(masterCtrlNowater, data, 8);
#if OPEN_BUFFER_HANDLE==1
            QList_1s.append(PACKET_MASTERCTRL_NOWATER);
#elif OPEN_BUFFER_HANDLE==0
            masterCtrlNowaterNode->Update();
#endif
        }
        break;
    default:
        break;
    }
}

//收到监控板报警数据#/
void DataCan::DataDispatchMonitor(const char *data)//收到监控板报警数据#/
{

    switch (data[0]) {
        case 2:  //监控板的PDO1，即报警内容#/
            if(isShowMain == 1)
            {
                if ( memcmp(statusAlarmData,data,8) != 0 ) //判断两者内容是否相等#/
                {

#if OPEN_BUFFER_HANDLE==1
                    memcpy(statusAlarmData,data,8);
                    QList_1s.append(PACKET_STATUS_ALARM);
#elif OPEN_BUFFER_HANDLE==0
#if DEBUG_MUTEXDIE_FLAG
    Debug_insert("DataCan::001");
#endif
                    global_mutex.lock();   //20151022 修改#/
                 //   global_mutex.tryLock(10);  //20151022 修改#/
                    memcpy(statusAlarmData,data,8);
                    statusAlarmNode->Update();
                    global_mutex.unlock();
#if DEBUG_MUTEXDIE_FLAG
    Debug_insert("DataCan::002");
#endif

    if ( (data[6] & 0x1) == 1) { //无血#/
        veinsHaveBloood = 1;
        dataNodeUpdate(VEINS_HAVE_BLOOD,&veinsHaveBloood);
    }
    else
    {
        veinsHaveBloood = 0;
        dataNodeUpdate(VEINS_HAVE_BLOOD,&veinsHaveBloood);
    }

    if ( (data[6] & 0x4)==0x4) { //液位满#/
        bloodlevel = 1;
        dataNodeUpdate(BLOOD_LEVEL_OK,&bloodlevel);
    }
    else
    {
        bloodlevel = 0;
        dataNodeUpdate(BLOOD_LEVEL_OK,&bloodlevel);
    }
#endif
                }
            }
            break;

        case 10:   //对应监控板的PDO2的 byte0=10的内容，即出水口温度和电导#/
            if (memcmp(monitorWaterOut, data, 8)!= 0) {
                memcpy(monitorWaterOut, data, 8);
#if OPEN_BUFFER_HANDLE==1
                    QList_1s.append(PACKET_MONITOR_WATER);
#elif OPEN_BUFFER_HANDLE==0
                    monitorWaterOutNode->Update();
#endif
            }
            break;
        case 11:
            if (isShowMain == 1) {

                if (memcmp(monitorPress, data, 8)!= 0) {
          //          global_mutex.lock();
                    memcpy(monitorPress, data, 8);

#if OPEN_BUFFER_HANDLE==1
                    QList_1s.append(PACKET_MONITOR_PRESS);
#elif OPEN_BUFFER_HANDLE==0
                    monitorPresstNode->Update();

#endif
                }
            }
            break;
        case 12:
            if (memcmp(monitorBloodState, data, 8)!= 0) {
                memcpy(monitorBloodState, data, 8);
#if OPEN_BUFFER_HANDLE==1
                QList_1s.append(PACKET_MONITOR_BLOODST);
#elif OPEN_BUFFER_HANDLE==0
                monitorBloodStateNode->Update();
                dataRead(PACKET_NAME_AP,&packetAP);
                packetAP.ap = monitorPress->N5_AP - 700;
                //dataNodeUpdate(PACKET_NAME_AP,&packetAP);
                dataNodeUpdate(PACKET_NAME_UPDATE_AP, &packetAP.ap);//原来用packetAP，将临时收到的数据单独update#/
                us_sleep(10);

                dataRead(PACKET_NAME_VP,&packetVP);
                packetVP.vp = monitorPress->N5_VP - 700;
                //dataNodeUpdate(PACKET_NAME_VP,&packetVP);
                dataNodeUpdate(PACKET_NAME_UPDATE_VP, &packetVP.vp);//原来用packetAP，将临时收到的数据单独update#/
                us_sleep(10);


                dataRead(PACKET_NAME_TMP,&packetTMP);
                packetTMP.tmp = monitorPress->N5_TMP - 700;
                //dataNodeUpdate(PACKET_NAME_TMP,&packetTMP);
                dataNodeUpdate(PACKET_NAME_UPDATE_TMP,&packetTMP.tmp);//原来用packetAP，将临时收到的数据单独update#/
                us_sleep(10);
#endif
            }
            break;
        case 13:
            if (memcmp(monitorBPMState, data, 8)!= 0) {
                memcpy(monitorBPMState, data, 8);
                monitorBPMStateNode->Update();
            }
            break;
        case 14:
            if (memcmp(monitorBPMMisc, data, 8)!= 0) {
                memcpy(monitorBPMMisc, data, 8);
                monitorBPMMiscNode->Update();
            }
            break;
    default:
        break;
    }
}

//电源板通信处理
void DataCan::DataDispatchPower(const char *data)
{
    if (isShowMain != 1)
         return;

    switch (data[0]) {

        case 0:
        if (memcmp(battryData, data, 8)!= 0) {
            memcpy(battryData, data, 8);

#if OPEN_BUFFER_HANDLE==1
            QList_1s.append(PACKET1_NAME_BATTERY);
#elif OPEN_BUFFER_HANDLE==0
            battryNode->Update();
#endif
            //dataNodeUpdate(PACKET1_NAME_BATTERY,&battryData);
        }
            break;

        case 1:
                if (memcmp(powerData, data, 8)!= 0) {
                    memcpy(powerData, data, 8);

#if OPEN_BUFFER_HANDLE==1
                    QList_1s.append(PACKET_NAME_POWER);
#elif OPEN_BUFFER_HANDLE==0
                    powerNode->Update();
#endif
                    //dataNodeUpdate(PACKET_NAME_POWER,&powerData);
                }

            break;

        case 2:

            break;

      case 3:

        break;

      default:
        break;
    }

}

void DataCan::CleanData(int which)
{
  which=which;
  global_mutex.lock();
  memset(statusAlarmData,0,sizeof(PacketStatusAlarm));
  memset(status_MastAlarmData,0,sizeof(PacketStatusAlarm));
  memset(monitorPress,0,sizeof(PacketPress));

#if OPEN_BUFFER_HANDLE==1
    QList_1s.clear();   //1S处理一次的队列#/
    QList_5s.clear();  //5s处理一次的队列#/
#endif

  global_mutex.unlock();
}

void DataCan::run()
{
    int res;
    char event = 0;//获取通讯对象的功能码#/
    Message rx_data;

    while(1)
    {
        if(CAN_FD_FLAG>0)
        {
            res=CanReceive_Handle(CAN_FD_FLAG,&rx_data);
            if(res==0)
            {

                event = rx_data.cob_id>>7;//获取通讯对象的功能码#/

                if(event==EVENT_PDO1tx)      //接收到PDO的处理#/
                {
                    if(rx_data.data[0]<=50)  //反码的内容暂时不处理
                    {
                        messageSignal.MessageSignalApp(&rx_data);
                    }
                }
                else if(event==EVENT_SDOtx||event==EVENT_SDOrx) //接收到SDO的处理#/
                {
                    if(event==EVENT_SDOtx)   //对SDO应答命令进行处理#/
                    {
                        //接收SDO时的状态标志设置，并且保存接收数据#/
                        if(rx_data.data[0]==0x60||rx_data.data[0]==0x43)
                        {
                            SetRecSDO_State(rx_data.cob_id&0x7f,DE_SDO_OK,rx_data.data);
                        }
                        else if(rx_data.data[0]==0x80)
                        {
                            SetRecSDO_State(rx_data.cob_id&0x7f,DE_SDO_ERROR,rx_data.data);
                        }
                    }

                    messageSignal.MessageSignalApp(&rx_data);
                }

                res=1;
            }
        }
        else
        {
                 sleep(10);
        }
    }
}

void DataCan::DataHandle_ById(int id)
{
    switch(id)
    {
    case PACKET1_NAME_BATTERY:
        battryNode->Update();
        break;
    case PACKET_NAME_POWER:
        powerNode->Update();
        break;
    case PACKET_NAME_HEPARINDATA:
        heparinNode->Update();
        break;
    case PACKET_NAME_BLOODPUMP:
        bloodPumpNode->Update();
        break;
    case PACKET_NAME_BLOODPUMPTUBE:
        bloodPumpNodeTube->Update();
        break;
    case PACKET_NAME_SUBBLOODPUMP:
        subbloodPumpNode->Update();
        break;
    case PACKET_NAME_SUBBLOODPUMPTUBE:
        subbloodPumpNodeTube->Update();
        break;
    case PACKET_MASTERCTRL_VALVE:
        masterCtrlValveNode->Update();
        break;
    case PACKET_MASTERCTRL_PDOERR:
        masterCtrlPdoErrNode->Update();
        break;
    case PACKET_MASTERCTRL_OUTLET:
        masterCtrlOutletNode->Update();
        break;
    case PACKET_MASTERCTRL_MIX:
        masterCtrlMixNode->Update();
        break;
    case PACKET_MASTERCTRL_FLUIDB:
        masterCtrlFluidBNode->Update();
        break;
    case PACKET_MASTERCTRL_FLUIDAFTER:
        masterCtrlFluidAfterNode->Update();
        break;
    case PACKET_MASTERCTRL_UFVALUE:
        masterCtrlUfNode->Update();
        break;
    case PACKET_MASTERCTRL_UFPUMP:
        masterCtrlUfPumpNode->Update();
        break;
    case PACKET_MASTERCTRL_HEAT:
        masterCtrlHeatNode->Update();
        break;
    case PACKET_MASTERCTRL_BPUMP:
        masterCtrlBPumpNode->Update();
        break;
    case PACKET_MASTERCTRL_SDO:
        masterCtrlSdoNode->Update();
        break;
    case PACKET_MASTERCTRL_MIXPUMP:
        masterCtrlMixPumpNode->Update();
        break;
    case PACKET_MASTERCTRL_DISPLAY:
        masterCtrlDisplayNode->Update();
        break;
    case PACKET_MASTERCTRL_DISPLAY_TABLE:
        masterCtrlDisplayNodeTable->Update();
        break;
    case PACKET_MONITOR_WATER:
        monitorWaterOutNode->Update();
        break;
    case PACKET_MONITOR_PRESS:
        monitorPresstNode->Update();
        break;
    case PACKET_MONITOR_BLOODST:
        monitorBloodStateNode->Update();
        dataNodeUpdate(PACKET_NAME_BLOODTEMP,&monitorBloodState->N5_Blood_temp);
        dataRead(PACKET_NAME_AP,&packetAP);
        if(monitorPress->N5_AP!=0)
            packetAP.ap = monitorPress->N5_AP - 700;
        //dataNodeUpdate(PACKET_NAME_AP,&packetAP);
        dataNodeUpdate(PACKET_NAME_UPDATE_AP, &packetAP.ap);//原来用packetAP，将临时收到的数据单独update#/
        us_sleep(10);

        dataRead(PACKET_NAME_VP,&packetVP);
        if(monitorPress->N5_VP!=0)
            packetVP.vp = monitorPress->N5_VP - 700;
        //dataNodeUpdate(PACKET_NAME_VP,&packetVP);
        dataNodeUpdate(PACKET_NAME_UPDATE_VP, &packetVP.vp);//原来用packetAP，将临时收到的数据单独update#/
        us_sleep(10);


        dataRead(PACKET_NAME_TMP,&packetTMP);
        if(monitorPress->N5_TMP!=0)
            packetTMP.tmp = monitorPress->N5_TMP - 700;
        //dataNodeUpdate(PACKET_NAME_TMP,&packetTMP);
        dataNodeUpdate(PACKET_NAME_UPDATE_TMP,&packetTMP.tmp);//原来用packetAP，将临时收到的数据单独update#/
        us_sleep(10);
        break;
    case PACKET_MONITOR_BPMST:
        monitorBPMStateNode->Update();
        break;
    case PACKET_MONITOR_BPMMISC:
        monitorBPMMiscNode->Update();
        break;
    case PACKET_STATUS_NOTE:
        statusNoteNode->Update();    //主控板的PDO0外发#/
        //血路准备好#/
        if ((statusNoteData->note.note4&0x10)==0x10 && bloodroadOk !=1)//tmp == 0x13
        {
            bloodroadOk = 1;

            dataNodeUpdate(PACKET_BLOODROAD_OK,&bloodroadOk);
        }
        //水路准备好#/
        if ((statusNoteData->note.note4&0x20)==0x20 && waterroadOk !=1) //tmp == 0x14
        {
            waterroadOk = 1;
            dataNodeUpdate(PACKET_WATERROAD_OK,&waterroadOk);
        }
        break;
    case PACKET_STATUS_ALARM:
        statusAlarmNode->Update();

        if ( (statusAlarmData->alarm.Iostatus & 0x1) == 1) { //无血#/
            veinsHaveBloood = 1;
            dataNodeUpdate(VEINS_HAVE_BLOOD,&veinsHaveBloood);
        }
        else
        {
            veinsHaveBloood = 0;
            dataNodeUpdate(VEINS_HAVE_BLOOD,&veinsHaveBloood);
        }

        if ( (statusAlarmData->alarm.Iostatus & 0x4)==0x4) { //无液#/
            bloodlevel = 1;
            dataNodeUpdate(BLOOD_LEVEL_OK,&bloodlevel);
        }
        else
        {
            bloodlevel = 0;
            dataNodeUpdate(BLOOD_LEVEL_OK,&bloodlevel);
        }
        break;
    case PACKET_STATUS_MASTALARM:
        status_MastAlarmNode->Update();
        break;
    case PACKET_BLOODROAD_OK:
        dataNodeUpdate(PACKET_BLOODROAD_OK,&bloodroadOk);
        break;
    case PACKET_WATERROAD_OK:
        dataNodeUpdate(PACKET_WATERROAD_OK,&waterroadOk);
        break;
    case VEINS_HAVE_BLOOD:
        dataNodeUpdate(VEINS_HAVE_BLOOD,&veinsHaveBloood);
        break;
    case BLOOD_LEVEL_OK:
        dataNodeUpdate(BLOOD_LEVEL_OK,&bloodlevel);
        break;
    case PACKET_MASTERCTRL_ANTRUM:
        masterCtrlAntrumNode->Update();
        break;
    case PACKET_MASTERCTRL_NOWATER:
        masterCtrlNowaterNode->Update();
        break;
    }
}

void DataCan::timer_handler(const int timeOut)
{
    Q_UNUSED(timeOut);
    int id=0;
    QList<int> QList_tmp;

#if DEBUG_DATA_SQL
    Debug_insert("DataCan::timer_handler-in");
#endif

    QList_tmp.clear();
    if(QList_1s.size()>0)
    {
        QList_tmp=QList_1s;
        QList_1s.clear();
    }

    while(QList_tmp.size()>0)//1S处理一次的队列#/
    {
        id=QList_tmp.first();
        DataHandle_ById(id);   //根据ID号进行处理#/
        QList_tmp.removeFirst();
    }

#if DEBUG_DATA_SQL
    Debug_insert("DataCan::timer_handler-out");
#endif
}

//发分处理can数据#/
//获取canopen真正发送和接收的底层CAN数据包，并根据canopen的协议分析出#/
//具体是与那个设备节点进行SDO或PDO的通信，然后进行相应的处理#/
//这样可以更及时获取发送或接收操作是否成功，并且获取数据进行处理#/
void DataCan::DataDispatch(const Message &message)
{
    char event = message.cob_id>>7;//获取通讯对象的功能码#/
    if ((event == EVENT_NODE_GUARD))return;
#if SYSTEM_DEBUG_FLAG
    Debug_insert("DataCan::DataDispatch-in");
#endif
    //获取设备节点ID号#/
    //主机发送PDO时，message.cob_id&0x7f=0，收到PDO时，则可识别到设备ID#/
    //这都根据字典中PDO Parameter的设置决定#/
    switch (message.cob_id&0x7f) {
        case CANID_UI:  //本设备--人机板#/
            //接收到别的节点的读写请求数据包时，对应字典---0X1200的601#/
           //0x23是数据长度为4个字节的写入命令字#/
            if ((event == EVENT_SDOrx) && (message.data[0] == 0x23)) {
                memcpy(masterCtrlSdo, message.data, 8);
                masterCtrlSdoNode->Update();
            }
        case CANID_MASTERCTRL:  //主控板#/
            N3_CANOPEN_FLAG=0;
            //接收到主控板的发送数据包时，对应字典---0X1282的583#/
            if ((event == EVENT_SDOtx))
            {
             if((message.data[2]==0x20))  //主索引 20开头#/
             {
                memcpy(masterCtrlSdo, message.data, 8);
                masterCtrlSdoNode->Update();
             }
             else if((message.data[2]==0x10)) //主索引 10开头#/
                {
                    memcpy(&VerNodedata, message.data, 8);
                    //通过发送SDO写主控制板的版本号#/
                    enginerDialog->version(CANNODE_MASTER_CONTROL,message.data[3],&VerNodedata);
                }
            }
            else   //PDO#/
            {


                DataDispatchMasterCtrl((char *)message.data);

            }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("DataCan::DataDispatch-break01");
#endif
            break;

        case CANID_MONITOR: //监控板#/
           N5_CANOPEN_FLAG=0;
        //接收到监控板的发送数据包时，对应字典---0X1285的585#/
           if ((event == EVENT_SDOtx))
           {
             if((message.data[2]==0x10))
                {
                    memcpy(&VerNodedata, message.data, 8);
                    enginerDialog->version(CANNODE_MASTER_MONITOR,message.data[3],&VerNodedata);

                }
             else if((message.data[2]==0x20))
             {
                memcpy(masterCtrlSdo, message.data, 8);
                masterCtrlSdoNode->Update();
             }
            }
            else if ((event==EVENT_PDO1tx))  //接收到监控板的PDO1发送数据包时#/
             {

                DataDispatchMonitor((char *)message.data);

             }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("DataCan::DataDispatch-break02");
#endif
            break;

        case CANID_BLOOD_PUMP://血泵1#/
            N9_CANOPEN_FLAG=0;
          if(isShowMain){  //进入主页面后#/
            if ((event==EVENT_PDO1tx))//接收到血泵1的PDO1发送数据包时#/
            {
                if((message.data[0]==1) &&(memcmp(&N9PDOData, message.data, 8)!=0))
                {
                    memcpy(&N9PDOData, message.data, 8);
                    bloodPumpData->N9_mode=N9PDOData.N9_mode;
                    bloodPumpData->N9_speed=N9PDOData.N9_speed;
                    bloodPumpData->N9_status=N9PDOData.N9_status;
                    bloodPumpData->N9_amount=0;
                    //为了解决血泵原来采用16位记录累计量会溢出的问题，加多了一个字节做累计量#/
                    bloodPumpData->N9_amount=(bloodPumpData->N9_amount)|(N9PDOData.reserve1<<16)|N9PDOData.N9_amount;
                    bloodPumpNode->Update();
                }
                if((message.data[0]==2) &&(memcmp(bloodPumpTubeData, message.data, 8)!=0))
                {
                    memcpy(bloodPumpTubeData, message.data, 8);
                    bloodPumpNodeTube->Update();
                }
            }
            else if ((event == EVENT_SDOtx))
            {

                if((message.data[2]==0x20))  //主索引 2000H  字节0：命令 字节1、2：索引号  字节3：子索引
                {
                    memcpy(masterCtrlSdo, message.data, 8);
                    masterCtrlSdoNode->Update();
                }
                else if((message.data[2]==0x10))
                {
                   {
                    memcpy(&VerNodedata, message.data, 8);
                    enginerDialog->version(CANNODE_BLOODPUMP,message.data[3],&VerNodedata);

                   }
                }
            }
             }
          else
          {
               if ((event == EVENT_SDOtx))
              {
                  if((message.data[2]==0x10))
                  {
                     {
                      memcpy(&VerNodedata, message.data, 8);
                      enginerDialog->version(CANNODE_BLOODPUMP,message.data[3],&VerNodedata);
                     }

                  }
              }
           }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("DataCan::DataDispatch-break03");
#endif
           break;

        case CANID_BLOOD_PUMP2://血泵2#/
            N10_CANOPEN_FLAG=0;
        if(isShowMain){  //进入主页面后#/
          if ((event==EVENT_PDO1tx))
          {
              if((message.data[0]==1) &&(memcmp(&N10PDOData, message.data, 8)!=0))
              {
                  memcpy(&N10PDOData, message.data, 8);

                  subbloodPumpData->N9_mode=N10PDOData.N9_mode;
                  subbloodPumpData->N9_speed=N10PDOData.N9_speed;
                  subbloodPumpData->N9_status=N10PDOData.N9_status;
                  subbloodPumpData->N9_amount=0;
                  subbloodPumpData->N9_amount=(subbloodPumpData->N9_amount)|(N10PDOData.reserve1<<16)|N10PDOData.N9_amount;
                  subbloodPumpNode->Update();
              }
              if((message.data[0]==2) &&(memcmp(subbloodPumpTubeData, message.data, 8)!=0))
              {
                  memcpy(subbloodPumpTubeData, message.data, 8);
                  subbloodPumpNodeTube->Update();
              }
          }else if ((event == EVENT_SDOtx)){

                if((message.data[2]==0x20))
                {
                    memcpy(masterCtrlSdo, message.data, 8);
                    masterCtrlSdoNode->Update();
                }
                else if((message.data[2]==0x10))
                {
                    {
                        memcpy(&VerNodedata, message.data, 8);
                        enginerDialog->version(CANNODE_BLOODPUMP2,message.data[3],&VerNodedata);
                    }
                }
            }
        }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("DataCan::DataDispatch-break04");
#endif
            break;

        case CANID_HEPARIN_PUMP:  //肝素泵#/
            N15_CANOPEN_FLAG=0;
            if ((event == EVENT_SDOtx))
            {
                if((message.data[2]==0x20))
                {
                    memcpy(masterCtrlSdo, message.data, 8);
                    masterCtrlSdoNode->Update();
                }
                else if((message.data[2]==0x10))
                {
                    {
                        memcpy(&VerNodedata, message.data, 8);
                        enginerDialog->version(CANNODE_HEPARINPUMP,message.data[3],&VerNodedata);
                    }

                }
            }
            else if(event==EVENT_PDO1tx)
            {
                if (( memcmp(heparinData, message.data,8) != 0 ) ){
                    {
                        memcpy(heparinData, message.data, 8);
#if OPEN_BUFFER_HANDLE==1
                        QList_1s.append(PACKET_NAME_HEPARINDATA);
#elif OPEN_BUFFER_HANDLE==0
                        //heparinNode->Update();
                        dataNodeUpdate(PACKET_NAME_HEPARINDATA,heparinData);
#endif
                    }
                }
            }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("DataCan::DataDispatch-break05");
#endif
            break;

        case CANID_POWER:  //电源控制板#/
            N7_CANOPEN_FLAG=0;
            if (event == EVENT_SDOtx)
            {
                if((message.data[2]==0x10))
                {
                    memcpy(&VerNodedata, message.data, 8);
                    enginerDialog->version(CANNODE_POWER,message.data[3],&VerNodedata);
                }
                else
                    dataNodeFind(PACKET_NAEME_POWER_WAKEUP)->Update(const_cast<Message *>(&message));
            }
            else//读到PDO时#/
            {
                DataDispatchPower((char *)message.data);
            }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("DataCan::DataDispatch-break06");
#endif
            break;

        default:
            break;
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("DataCan::DataDispatch-out");
#endif
    us_sleep(2000);//200000
}
