#ifndef SCENE_STATUS_TABLE_H
#define SCENE_STATUS_TABLE_H

#include <QtGui>
#include "page_info.h"
#include "lang_switch.h"
#include "data_custom.h"



#define BLOODPUMP_FLAG  1
#define BLOODPUMPTUBE_FLAG   2
#define SUBBLOODPUMP_FLAG    3
#define SUBBLOODPUMPTUBE_FLAG 4
#define STATUS_ALARM_FLAG   5
#define STATUS_MASTALARM_FLAG 6
#define MASTERCTRL_PDO0_FLAG 7
#define MASTERCTRL_PDOERR_FLAG 8
#define USERCTRL_PDO1_FLAG 9
#define USERCTRL_PDO2_FLAG 10
#define HEPARIN_PDO0_FLAG 11
#define MASTERCTRL_PDO2_FLAG 12
#define UFVALUE_PDO_FLAG 13
#define BPMState_PDO13_FLAG 14   //血压计状态#/
#define BPMMisc_PDO14_FLAG 15    //血压计心率#/
#define MONITOR_WATEROUT_FLAG 16   //监控板的出水口电导和温度#/


extern bool Status_show_flag;

class SceneStatusTable : public QTableView, public LangSetText,
    public PieceVisible, public ActionCustom
{
    Q_OBJECT
public:
    SceneStatusTable(QWidget *parent=0);
    void CustomSetColRowCount(int row=0, int col=0);
    bool SetCusContent(const char *name=NULL, const unsigned value=0, int row=0);
    void SetTableContent(int flag);

private:
    void CustomSetText();
    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void setCusVisible(bool vis=false, char* para=NULL);

private:
    inline void setUi();

private:
    QStandardItemModel model;

    int colNum;
    int rowNum;
    int itemCount;
    PacketBloodPump bloodPump1;
    PacketBloodPumpTube bloodPumpTube1;
    PacketBloodPump bloodPump2;
    PacketBloodPumpTube bloodPumpTube2;
    PacketStatusAlarm statusAlarmData;
    PacketStatusAlarm status_MastAlarmData;
    PacketStatusNote masterCtrl_PDO0;
    PacketUfValue UF_valuePDO;
    PacketPdoErr masterPdoErr;
    PackN15PDO heparinData;
    PacketFluidAfter dataAfter;
    PacketBPMState BPMState_PDO13;
    PacketBPMMisc BPMMisc_PDO14;
    PacketWaterOut monitorWaterOut;

    char itemName[8];
    QString itemValue;
};

#endif
