#ifndef DATA_CAN_H
#define DATA_CAN_H

#include <QObject>
#include "data_custom.h"
#include "middle_tmp.h"
#include "data_set.h"


//class DataCan : public QObject,public QThread
class DataCan : public QThread,public timer_handle
{
    Q_OBJECT
public:
    //DataCan(QObject *parent=0) : QObject(parent){}
    DataCan(QObject *parent=0) {parent=parent;}
    void Init(void);
    void Open(void);
    void Quit(void);
    void CleanData(int which=0);

private slots:
    void DataDispatch(const Message &message);

private:
    void DataDispatchMasterCtrl(const char *data=NULL);
    void DataDispatchMonitor(const char *data=NULL);
    void DataDispatchPower(const char *data=NULL);
    void run();//模拟canfestival对CAN设备接收的数据进行处理#/
    void DataHandle_ById(int id);   //根据ID号进行处理#/
    void timer_handler(const int timeOut = 0);//定时周期处理#/

public:
    DataCustom *masterCtrlValveNode;
    PacketValve *masterCtrlValve;

    DataCustom *masterCtrlPdoErrNode;
    PacketPdoErr *masterCtrlPdoErrValue;

    DataCustom *masterCtrlOutletNode;
    PacketOutlet *masterCtrlOutlet;

    DataCustom *masterCtrlMixNode;
    PacketMix *masterCtrlMix;

    DataCustom *masterCtrlFluidBNode;
    PacketFluidB *masterCtrlFluidB;

    DataCustom *masterCtrlFluidAfterNode;
    PacketFluidAfter *masterCtrlFluidAfter;

    DataCustom *masterCtrlUfNode;
    PacketUfValue *masterCtrlUfValue;

    DataCustom *masterCtrlUfPumpNode;
    PacketUfPump *masterCtrlUfPump;

    DataCustom *masterCtrlHeatNode;
    PacketHeat *masterCtrlHeat;

    DataCustom *masterCtrlBPumpNode;
    PacketBPump *masterCtrlBPump;

    DataCustom *masterCtrlMixPumpNode;
    PacketMixPump *masterCtrlMixPump;

    DataCustom *masterCtrlDisplayNode;
    PacketDisplay *masterCtrlDisplay;

    DataCustom *masterCtrlDisplayNodeTable;
    PacketDisplay *masterCtrlDisplayTable;

    DataCustom *masterCtrlAntrumNode;
    PacketAntrum *masterCtrlAntrum;

    DataCustom *masterCtrlNowaterNode;
    PacketNowater *masterCtrlNowater;

    DataCustom *masterCtrlSdoNode;
    PacketMasterCtrlSdo *masterCtrlSdo;

    DataCustom *monitorWaterOutNode;
    PacketWaterOut *monitorWaterOut;

    DataCustom *monitorPresstNode;
    PacketPress *monitorPress;

    DataCustom *monitorBloodStateNode;
    PacketBloodState *monitorBloodState;

    DataCustom *monitorBPMStateNode;
    PacketBPMState *monitorBPMState;

    DataCustom *monitorBPMMiscNode;
    PacketBPMMisc *monitorBPMMisc;

    DataCustom *battryNode;
    PacketBattry *battryData;

    DataCustom *powerNode;
    PacketPower *powerData;

    DataCustom *heparinNode;
    PackN15PDO *heparinData;

    DataCustom *bloodPumpNode;
    PacketBloodPump *bloodPumpData;
    PacketN9N10Pump N9PDOData;

    DataCustom *bloodPumpNodeTube;
    PacketBloodPumpTube *bloodPumpTubeData;

    DataCustom *subbloodPumpNode;
    PacketBloodPump *subbloodPumpData;
    PacketN9N10Pump N10PDOData;

    DataCustom *subbloodPumpNodeTube;
    PacketBloodPumpTube *subbloodPumpTubeData;

    DataCustom *statusNoteNode;
    PacketStatusNote *statusNoteData;

    DataCustom *statusAlarmNode;
    PacketStatusAlarm *statusAlarmData;

    DataCustom *status_MastAlarmNode;
    PacketStatusAlarm *status_MastAlarmData;

    PacketMasterCtrlSdo VerNodedata;


    int bloodroadOk;
    int waterroadOk;
    int veinsHaveBloood;
    int bloodlevel;
    PacketAP packetAP;
    PacketVP packetVP;
    PacketTMP packetTMP;
    PacketCond packCond;
    QList<int> QList_1s;   //1S处理一次的队列#/
    QList<int> QList_5s;  //5s处理一次的队列#/

private:
    char isShowMain;
};

#endif
