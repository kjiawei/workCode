#include "config_ui.h"
#include "global_var.h"
#include "scene_status_table.h"
#include "canopen_shell.h"

#define STATUS_SWITCH_FLAG 1

static QStandardItem statustablemap[33][16];

bool Status_show_flag=false;

inline void SceneStatusTable::setUi()
{
    setModel(&model);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setGeometry(0, 0, 1024, 698);

    setStyleSheet("QTableView { border: none;"
                                   "background-color: #1f2637;"
                                   "color: #ffffff;"
                                   "selection-background-color: #ff0000}");
    horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    verticalHeader()->setResizeMode(QHeaderView::Fixed);
    horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

    CustomSetColRowCount(31, 14);
    memset(itemName, 0, 8);

    memset(&BPMState_PDO13, 0, sizeof(PacketBPMState));
    memset(&BPMMisc_PDO14, 0, sizeof(PacketBPMMisc));
}

void SceneStatusTable::CustomSetColRowCount(int row, int col)
{
    int i;
    colNum = col;
    rowNum = row;
    itemCount = col*row/2;
    model.setColumnCount(colNum);
    model.setRowCount(rowNum);

#if STATUS_SWITCH_FLAG==0
    QColor color(CONFIG_COLOR_YELLOW);
    QVariant variant = color;
    for (i=0; i<col; i++) {
        setColumnWidth(i, 60);
        if (i%2 == 0) {
            model.setHeaderData(i, Qt::Horizontal, "Name:");
        } else {
            model.setHeaderData(i, Qt::Horizontal, "Value:");
        }
    }
#else
    for (i=0; i<col; i++) {
        setColumnWidth(i, 72);
        if (i%2 == 0) {
            switch(i)
            {
            case 0:
                model.setHeaderData(i, Qt::Horizontal, "UI-N1");
                break;
            case 2:
                 model.setHeaderData(i, Qt::Horizontal, "MC-N3");
                break;
            case 4:
                  model.setHeaderData(i, Qt::Horizontal, "MN-N5");
                break;
            case 6:
                  model.setHeaderData(i, Qt::Horizontal, "BL1-N9");
                break;
            case 8:
                   model.setHeaderData(i, Qt::Horizontal, "BL2-N10");
                break;
            case 10:
                   model.setHeaderData(i, Qt::Horizontal, "HP-N15");
                break;
            default:
                model.setHeaderData(i, Qt::Horizontal, "NULL");
             break;
            }
        } else {
            model.setHeaderData(i, Qt::Horizontal, "Value:");
        }
    }
#endif
    for (i=0; i<row; i++) {
        setRowHeight(i, 22);
    }
}

bool SceneStatusTable::SetCusContent(const char *name, const unsigned value, int row)
{
    if ((name == NULL) || (row >= itemCount)) {
        return false;
    }

    int rowIndex, colIndex;
    colIndex = (row/rowNum)*2;
    rowIndex = row%rowNum-1;
    itemValue.setNum(value);
    model.setItem(rowIndex, colIndex, new QStandardItem(name));
    model.setItem(rowIndex, colIndex+1, new QStandardItem(itemValue));
    //model.item(row, col)->setTextAlignment(Qt::AlignCenter);
    return true;
}

void SceneStatusTable::SetTableContent(int flag)
{
    int rowIndex, colIndex;    
    void *vp=0;

    switch(flag)
    {
        case USERCTRL_PDO1_FLAG:  //人机板PDO1中的相关PDO内容#/
            rowIndex=0;    //row
            colIndex=0;   //col
            statustablemap[rowIndex][colIndex].setText(QString("Wspeed"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(uiPDO02.N1_set_Wspeed));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=1;
            colIndex=0;
            statustablemap[rowIndex][colIndex].setText(QString("BPspeed"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(uiPDO02.N1_set_BPspeed));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=2;
            colIndex=0;
            statustablemap[rowIndex][colIndex].setText(QString("Bscale"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(uiPDO03.N1_set_Bscale));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=3;
            colIndex=0;
            statustablemap[rowIndex][colIndex].setText(QString("Wscale"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(uiPDO03.N1_set_Wscale));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=4;    //row
            colIndex=0;   //col
            statustablemap[rowIndex][colIndex].setText(QString("set_B"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(uiPDO03.N1_set_B));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=5;
            colIndex=0;
            statustablemap[rowIndex][colIndex].setText(QString("minx"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(uiPDO04.N1_set_mix));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=6;
            colIndex=0;
            statustablemap[rowIndex][colIndex].setText(QString("UFspeed"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(uiPDO04.N1_set_UFspeed));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=7;
            colIndex=0;
            statustablemap[rowIndex][colIndex].setText(QString("UFtarget"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(uiPDO04.N1_set_UFtarget));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=8;
            colIndex=0;
            statustablemap[rowIndex][colIndex].setText(QString("first"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(uiPDO05.N1_set_heparin_first));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=9;
            colIndex=0;
            statustablemap[rowIndex][colIndex].setText(QString("hep_speed"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(uiPDO05.N1_set_heparin_speed));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=10;    //row
            colIndex=0;   //col
            statustablemap[rowIndex][colIndex].setText(QString("T_UP"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(uiPDO05.N1_set_temp_UP));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=11;
            colIndex=0;
            statustablemap[rowIndex][colIndex].setText(QString("T_DOWN"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(uiPDO06.N1_set_temp_DOWN));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=12;
            colIndex=0;
            statustablemap[rowIndex][colIndex].setText(QString("AP_UP"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(uiPDO06.N1_set_AP_UP));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=13;
            colIndex=0;
            statustablemap[rowIndex][colIndex].setText(QString("AP_DOWN"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(uiPDO06.N1_set_AP_DOWN));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);

            rowIndex=14;
            colIndex=0;
            statustablemap[rowIndex][colIndex].setText(QString("VP_UP"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(uiPDO07.N1_set_VP_UP));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=15;
            colIndex=0;
            statustablemap[rowIndex][colIndex].setText(QString("VP_DOWN"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(uiPDO07.N1_set_VP_DOWN));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=16;
            colIndex=0;
            statustablemap[rowIndex][colIndex].setText(QString("TMPUP"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(uiPDO07.N1_set_TMP_UP));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=17;    //row
            colIndex=0;   //col
            statustablemap[rowIndex][colIndex].setText(QString("TMPDOWN"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(uiPDO08.N1_set_TMP_DOWN));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=18;
            colIndex=0;
            statustablemap[rowIndex][colIndex].setText(QString("CondUP"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(uiPDO08.N1_set_Cond_UP));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=19;
            colIndex=0;
            statustablemap[rowIndex][colIndex].setText(QString("CondDOWN"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(uiPDO08.N1_set_Cond_DOWN));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=20;
            colIndex=0;
            statustablemap[rowIndex][colIndex].setText(QString("QJ_UP"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(uiPDO09.N1_set_QJcond_UP));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=21;
            colIndex=0;
            statustablemap[rowIndex][colIndex].setText(QString("QJ_DOWN"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(uiPDO09.N1_set_QJcond_DOWN));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);

            rowIndex=26;
            colIndex=0;

            vp=&uiPDO01.byte1;

            statustablemap[rowIndex][colIndex].setText(QString("N1_set1"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(*((unsigned char*)vp),2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);

            rowIndex=27;
            colIndex=0;
            statustablemap[rowIndex][colIndex].setText(QString("CAN_MUTEX"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(CAN_MUTEX_STATE));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);

            break;

        case USERCTRL_PDO2_FLAG: //人机板PDO2中的相关PDO内容#/
            rowIndex=22;
            colIndex=0;
            statustablemap[rowIndex][colIndex].setText(QString("status"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(PDOstate.state.N1_status));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=23;    //row
            colIndex=0;   //col
            statustablemap[rowIndex][colIndex].setText(QString("sblood"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(PDOstate.state.N1_blood_status,16));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=24;
            colIndex=0;
            statustablemap[rowIndex][colIndex].setText(QString("sub-status"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(PDOstate.state.N1_sub_status,16));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=25;
            colIndex=0;
            statustablemap[rowIndex][colIndex].setText(QString("swater"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(PDOstate.state.N1_water_status,16));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);               
            break;

        case BLOODPUMP_FLAG:
            rowIndex=0;    //row
            colIndex=6;   //col
            statustablemap[rowIndex][colIndex].setText(QString("speed"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(bloodPump1.N9_speed));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=1;
            colIndex=6;
            statustablemap[rowIndex][colIndex].setText(QString("amount"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(bloodPump1.N9_amount));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=2;
            colIndex=6;
            statustablemap[rowIndex][colIndex].setText(QString("mode"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(bloodPump1.N9_mode));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=3;
            colIndex=6;
            statustablemap[rowIndex][colIndex].setText(QString("status"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(bloodPump1.N9_status,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            break;
        case BLOODPUMPTUBE_FLAG:
            rowIndex=5;
            colIndex=6;
            statustablemap[rowIndex][colIndex].setText(QString("tube"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(bloodPumpTube1.N9_set_tube));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=6;
            colIndex=6;
            statustablemap[rowIndex][colIndex].setText(QString("setspeed"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(bloodPumpTube1.N9_set_speed));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=7;
            colIndex=6;
            statustablemap[rowIndex][colIndex].setText(QString("N9_Key"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(bloodPumpTube1.N9_Key,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=9;
            colIndex=6;
            statustablemap[rowIndex][colIndex].setText(QString("SET_SPEED"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(G_SET_BPSPEED));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            break;
        case SUBBLOODPUMP_FLAG:
            rowIndex=0;
            colIndex=8;
            statustablemap[rowIndex][colIndex].setText(QString("speed"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(bloodPump2.N9_speed));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=1;
            colIndex=8;
            statustablemap[rowIndex][colIndex].setText(QString("amount"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(bloodPump2.N9_amount));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=2;
            colIndex=8;
            statustablemap[rowIndex][colIndex].setText(QString("mode"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(bloodPump2.N9_mode));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=3;
            colIndex=8;
            statustablemap[rowIndex][colIndex].setText(QString("status"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(bloodPump2.N9_status,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            break;
        case SUBBLOODPUMPTUBE_FLAG:
            rowIndex=5;
            colIndex=8;
            statustablemap[rowIndex][colIndex].setText(QString("tube"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(bloodPumpTube2.N9_set_tube));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=6;
            colIndex=8;
            statustablemap[rowIndex][colIndex].setText(QString("setspeed"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(bloodPumpTube2.N9_set_speed));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=7;
            colIndex=8;
            statustablemap[rowIndex][colIndex].setText(QString("N9_Key"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(bloodPumpTube2.N9_Key,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            break;

        case STATUS_ALARM_FLAG:
            rowIndex=0;
            colIndex=4;
            statustablemap[rowIndex][colIndex].setText(QString("error1"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(statusAlarmData.alarm.errorstate1,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=1;
            colIndex=4;
            statustablemap[rowIndex][colIndex].setText(QString("error2"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(statusAlarmData.alarm.errorstate2,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=2;
            colIndex=4;
            statustablemap[rowIndex][colIndex].setText(QString("error3"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(statusAlarmData.alarm.errorstate3,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=3;
            colIndex=4;
            statustablemap[rowIndex][colIndex].setText(QString("error4"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(statusAlarmData.alarm.errorstate4,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=4;
            colIndex=4;
            statustablemap[rowIndex][colIndex].setText(QString("error5"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(statusAlarmData.alarm.errorstate5,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=5;
            colIndex=4;
            statustablemap[rowIndex][colIndex].setText(QString("IOstatus"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(statusAlarmData.alarm.Iostatus,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=6;
            colIndex=4;
            statustablemap[rowIndex][colIndex].setText(QString("testerr"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(statusAlarmData.alarm.errorstate7));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);

            //超滤相关参数显示#/
            rowIndex=13;
            colIndex=4;
            statustablemap[rowIndex][colIndex].setText(QString("STIME"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(G_UF_STARTTIME%100000));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=14;
            colIndex=4;
            statustablemap[rowIndex][colIndex].setText(QString("ETIME"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(G_UF_ENDTIME%100000));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=15;
            colIndex=4;
            statustablemap[rowIndex][colIndex].setText(QString("SYSTIME"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(G_SYS_CURTIME%100000));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);

            rowIndex=17;
            colIndex=4;
            statustablemap[rowIndex][colIndex].setText(QString("UFCURETAR"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(G_UF_CURETARGET));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=18;
            colIndex=4;
            statustablemap[rowIndex][colIndex].setText(QString("UFFINISH"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(G_UF_CUREFINISH));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=19;
            colIndex=4;
            statustablemap[rowIndex][colIndex].setText(QString("ISOTOTAL"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(ISO_totalvolume));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=20;
            colIndex=4;
            statustablemap[rowIndex][colIndex].setText(QString("ISOFINISH"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(ISO_finishvolume));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=21;
            colIndex=4;
            statustablemap[rowIndex][colIndex].setText(QString("UF_SET"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(G_UF_SETRATE));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=23;
            colIndex=4;
            statustablemap[rowIndex][colIndex].setText(QString("static_BP"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(static_BP_amount));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=24;
            colIndex=4;
            statustablemap[rowIndex][colIndex].setText(QString("static_UF"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(static_UF_amount));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            break;

        case MONITOR_WATEROUT_FLAG://监控板的出水口电导和温度#/
            rowIndex=26;
            colIndex=4;
            statustablemap[rowIndex][colIndex].setText(QString("N5_temp"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(monitorWaterOut.N5_out_temp));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=27;
            colIndex=4;
            statustablemap[rowIndex][colIndex].setText(QString("N5_cond"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(monitorWaterOut.N5_out_cond));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=28;
            colIndex=4;
            statustablemap[rowIndex][colIndex].setText(QString("N5_leak"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(monitorWaterOut.N5_leak));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            break;

        case BPMState_PDO13_FLAG:  //血压计状态#/
            rowIndex=8;
            colIndex=4;
            statustablemap[rowIndex][colIndex].setText(QString("BPM_status"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(BPMState_PDO13.N5_BPM_status));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=9;
            colIndex=4;
            statustablemap[rowIndex][colIndex].setText(QString("BPM_SYS"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(BPMState_PDO13.N5_BPM_SYS));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=10;
            colIndex=4;
            statustablemap[rowIndex][colIndex].setText(QString("BPM_DIA"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(BPMState_PDO13.N5_BPM_DIA));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            break;

        case BPMMisc_PDO14_FLAG:    //血压计心率#/
            rowIndex=11;
            colIndex=4;
            statustablemap[rowIndex][colIndex].setText(QString("BPM_pulse"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(BPMMisc_PDO14.N5_BPM_pulse));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            break;

        case STATUS_MASTALARM_FLAG:
            rowIndex=0;
            colIndex=2;
            statustablemap[rowIndex][colIndex].setText(QString("error1"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(status_MastAlarmData.alarm.errorstate1,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=1;
            colIndex=2;
            statustablemap[rowIndex][colIndex].setText(QString("error2"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(status_MastAlarmData.alarm.errorstate2,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=2;
            colIndex=2;
            statustablemap[rowIndex][colIndex].setText(QString("error3"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(status_MastAlarmData.alarm.errorstate3,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=3;
            colIndex=2;
            statustablemap[rowIndex][colIndex].setText(QString("error4"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(status_MastAlarmData.alarm.errorstate4,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=4;
            colIndex=2;
            statustablemap[rowIndex][colIndex].setText(QString("error5"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(status_MastAlarmData.alarm.errorstate5,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=5;
            colIndex=2;
            statustablemap[rowIndex][colIndex].setText(QString("IOstatus"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(status_MastAlarmData.alarm.Iostatus,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
            rowIndex=6;
            colIndex=2;
            statustablemap[rowIndex][colIndex].setText(QString("testerr"));
            statustablemap[rowIndex][colIndex+1].setText(QString::number(status_MastAlarmData.alarm.errorstate7));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);

            break;
    case MASTERCTRL_PDO0_FLAG:
        rowIndex=7;
        colIndex=2;
        statustablemap[rowIndex][colIndex].setText(QString("hstatus"));
        statustablemap[rowIndex][colIndex+1].setText(QString::number(masterCtrl_PDO0.status.header_status,16).toAscii());
        model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
        model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
        rowIndex=8;
        colIndex=2;
        statustablemap[rowIndex][colIndex].setText(QString("bstatus"));
        statustablemap[rowIndex][colIndex+1].setText(QString::number(masterCtrl_PDO0.status.blood_status,16).toAscii());
        model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
        model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
        rowIndex=9;
        colIndex=2;
        statustablemap[rowIndex][colIndex].setText(QString("wstatus"));
        statustablemap[rowIndex][colIndex+1].setText(QString::number(masterCtrl_PDO0.status.water_status,16).toAscii());
        model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
        model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
        rowIndex=10;
        colIndex=2;
        statustablemap[rowIndex][colIndex].setText(QString("note1"));
        statustablemap[rowIndex][colIndex+1].setText(QString::number(masterCtrl_PDO0.note.note1,2).toAscii());
        model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
        model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
        rowIndex=11;
        colIndex=2;
        statustablemap[rowIndex][colIndex].setText(QString("note2"));
        statustablemap[rowIndex][colIndex+1].setText(QString::number(masterCtrl_PDO0.note.note2,2).toAscii());
        model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
        model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
        rowIndex=12;
        colIndex=2;
        statustablemap[rowIndex][colIndex].setText(QString("note3"));
        statustablemap[rowIndex][colIndex+1].setText(QString::number(masterCtrl_PDO0.note.note3,2).toAscii());
        model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
        model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
        rowIndex=13;
        colIndex=2;
        statustablemap[rowIndex][colIndex].setText(QString("note4"));
        statustablemap[rowIndex][colIndex+1].setText(QString::number(masterCtrl_PDO0.note.note4,2).toAscii());
        model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
        model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
        break;

    case MASTERCTRL_PDOERR_FLAG:
        rowIndex=15;
        colIndex=2;
        statustablemap[rowIndex][colIndex].setText(QString("PDOErr1"));
        statustablemap[rowIndex][colIndex+1].setText(QString::number(masterPdoErr.byte1,16).toAscii());
        model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
        model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
        rowIndex=16;
        colIndex=2;
        statustablemap[rowIndex][colIndex].setText(QString("PDOErr2"));
        statustablemap[rowIndex][colIndex+1].setText(QString::number(masterPdoErr.byte2,2).toAscii());
        model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
        model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
        rowIndex=17;
        colIndex=2;
        statustablemap[rowIndex][colIndex].setText(QString("PDOErr3"));
        statustablemap[rowIndex][colIndex+1].setText(QString::number(masterPdoErr.byte3,2).toAscii());
        model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
        model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
        break;

    case MASTERCTRL_PDO2_FLAG:
        rowIndex=18;
        colIndex=2;
        statustablemap[rowIndex][colIndex].setText(QString("aftertemp"));
        statustablemap[rowIndex][colIndex+1].setText(QString::number(dataAfter.N3_after_temp));
        model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
        model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
        rowIndex=19;
        colIndex=2;
        statustablemap[rowIndex][colIndex].setText(QString("aftercond"));
        statustablemap[rowIndex][colIndex+1].setText(QString::number(dataAfter.N3_after_cond));
        model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
        model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
        rowIndex=20;
        colIndex=2;
        statustablemap[rowIndex][colIndex].setText(QString("UFtime"));
        statustablemap[rowIndex][colIndex+1].setText(QString::number(dataAfter.noUse1));
        model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
        model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
        break;

    case UFVALUE_PDO_FLAG:
        rowIndex=22;
        colIndex=2;
        statustablemap[rowIndex][colIndex].setText(QString("UF_amount"));
        statustablemap[rowIndex][colIndex+1].setText(QString::number(UF_valuePDO.N3_UF_amount));
        model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
        model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
        rowIndex=23;
        colIndex=2;
        statustablemap[rowIndex][colIndex].setText(QString("UF_rate"));
        statustablemap[rowIndex][colIndex+1].setText(QString::number(UF_valuePDO.N3_UF_rate));
        model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
        model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
        break;

    case HEPARIN_PDO0_FLAG:
        rowIndex=0;
        colIndex=10;
        statustablemap[rowIndex][colIndex].setText(QString("allvalue"));
        statustablemap[rowIndex][colIndex+1].setText(QString::number(heparinData.inputAllValue));
        model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
        model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
        rowIndex=1;
        colIndex=10;
        statustablemap[rowIndex][colIndex].setText(QString("speed"));
        statustablemap[rowIndex][colIndex+1].setText(QString::number(heparinData.doseSpeed));
        model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
        model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
        rowIndex=2;
        colIndex=10;
        statustablemap[rowIndex][colIndex].setText(QString("mode"));
        statustablemap[rowIndex][colIndex+1].setText(QString::number(heparinData.workMode,16));
        model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
        model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
        rowIndex=3;
        colIndex=10;
        statustablemap[rowIndex][colIndex].setText(QString("submode"));
        statustablemap[rowIndex][colIndex+1].setText(QString::number(heparinData.workModeSub,16));
        model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
        model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
        rowIndex=4;
        colIndex=10;
        statustablemap[rowIndex][colIndex].setText(QString("senserstate"));
        statustablemap[rowIndex][colIndex+1].setText(QString::number(heparinData.senserState));
        model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
        model.setItem(rowIndex, colIndex+1,&statustablemap[rowIndex][colIndex+1]);
        break;
    }
}

SceneStatusTable::SceneStatusTable(QWidget *parent) :
    QTableView(parent),
    model(parent)
{
    setUi();
#if STATUS_SWITCH_FLAG==0
    dataNodeAddObject(PACKET_MASTERCTRL_DISPLAY_TABLE, this);
    dataNodeAddObject(PACKET_STATUS_MASTALARM, this);
#else
    dataNodeAddObject(PACKET_NAME_BLOODPUMP,this);  //血泵1#/
    dataNodeAddObject(PACKET_NAME_BLOODPUMPTUBE,this); //血泵1管径#/
    dataNodeAddObject(PACKET_NAME_SUBBLOODPUMP,this); //血泵2#/
    dataNodeAddObject(PACKET_NAME_SUBBLOODPUMPTUBE,this); //血泵2管径#/
    dataNodeAddObject(PACKET_STATUS_ALARM,this); //监控板报警PDO1#/
    dataNodeAddObject(PACKET_STATUS_MASTALARM,this);    //主控板PDO1#/
    dataNodeAddObject(PACKET_STATUS_NOTE,this); //主控板PDO0#/
    dataNodeAddObject(PACKET_MASTERCTRL_PDOERR,this);//主控板PDO ERR#/
    dataNodeAddObject(PACKET_NAME_HEPARINDATA,this);//肝书泵PDO#/
    dataNodeAddObject(PACKET_MASTERCTRL_FLUIDAFTER,this);//主控板PDO#/
    dataNodeAddObject(PACKET_MASTERCTRL_UFVALUE,this);  //主控板超滤PDO#/
    dataNodeAddObject(PACKET_MONITOR_BPMST,this);    //血压计状态#/
    dataNodeAddObject(PACKET_MONITOR_BPMMISC,this);  //血压计脉搏#/
    dataNodeAddObject(PACKET_MONITOR_WATER,this);
#endif
    PageAppendPiece("Status Table", this);
}

void SceneStatusTable::ActionHandle(const void* dataPtr,int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneStatusTable-ActionHandle-in");
#endif
/*
     if(Status_show_flag==false)
         return;
*/

    if(workstate.tipState.current==DEBUG)
        return;

#if STATUS_SWITCH_FLAG==0
    //if(!this->isVisible())
        //return;
    if ((dataPtr == NULL) || (name == NULL)) {
        return;
    }
//    PacketDisplay *ptr = (PacketDisplay*)dataPtr;
//    memcpy(itemName, ptr->N3_display_letter, 4);
    //SetCusContent(itemName, ptr->N3_display_data, ptr->N3_pos);
    if (name==PACKET_STATUS_MASTALARM){
       PacketStatusAlarm sdostate =*((PacketStatusAlarm*) dataPtr);
       SetCusContent("sdoerror", (sdostate.alarm.errorstate3& 0x03), 1);
      }
#else

    if(this->isVisible())
    {
        SetTableContent(USERCTRL_PDO1_FLAG);
        SetTableContent(USERCTRL_PDO2_FLAG);
    }

    if (name==PACKET_NAME_BLOODPUMP){
        bloodPump1=*((PacketBloodPump *)dataPtr);
        if(this->isVisible())
            SetTableContent(BLOODPUMP_FLAG);
    }
    else if (name==PACKET_NAME_BLOODPUMPTUBE){
        bloodPumpTube1=*((PacketBloodPumpTube *)dataPtr);
        if(this->isVisible())
            SetTableContent(BLOODPUMPTUBE_FLAG);
    }
    else if (name==PACKET_NAME_SUBBLOODPUMP){
        bloodPump2=*((PacketBloodPump *)dataPtr);
        if(this->isVisible())
            SetTableContent(SUBBLOODPUMP_FLAG);
    }
    else if (name==PACKET_NAME_SUBBLOODPUMPTUBE){
        bloodPumpTube2=*((PacketBloodPumpTube *)dataPtr);
        if(this->isVisible())
            SetTableContent(SUBBLOODPUMPTUBE_FLAG);
    }
    else if (name==PACKET_STATUS_ALARM){
        statusAlarmData=*((PacketStatusAlarm *)dataPtr);
        if(this->isVisible())
            SetTableContent(STATUS_ALARM_FLAG);
    }
    else if (name==PACKET_STATUS_MASTALARM){
        status_MastAlarmData=*((PacketStatusAlarm *)dataPtr);
        if(this->isVisible())
            SetTableContent(STATUS_MASTALARM_FLAG);
    }
    else if(name==PACKET_STATUS_NOTE){    //对应主控板PDO0#/
        masterCtrl_PDO0 = *((PacketStatusNote *)dataPtr);
        if(this->isVisible())
            SetTableContent(MASTERCTRL_PDO0_FLAG);
    }
    else if(name==PACKET_MASTERCTRL_UFVALUE){
        UF_valuePDO =*((PacketUfValue*)dataPtr);
        if(this->isVisible())
            SetTableContent(UFVALUE_PDO_FLAG);
    }
    else if(name==PACKET_MASTERCTRL_PDOERR)
    {
        masterPdoErr=*((PacketPdoErr *)dataPtr);
        if(this->isVisible())
            SetTableContent(MASTERCTRL_PDOERR_FLAG);
    }
    else if(name==PACKET_NAME_HEPARINDATA)
    {
        heparinData=*((PackN15PDO *)dataPtr);
        if(this->isVisible())
            SetTableContent(HEPARIN_PDO0_FLAG);
    }
    else if (name==PACKET_MASTERCTRL_FLUIDAFTER){    //对应主控板PDO2#/
         dataAfter=*((PacketFluidAfter*)dataPtr);
         if(this->isVisible())
             SetTableContent(MASTERCTRL_PDO2_FLAG);
    }
    else if (name==PACKET_MONITOR_BPMST){    //血压计状态#/
         BPMState_PDO13=*((PacketBPMState*)dataPtr);
         if(this->isVisible())
             SetTableContent(BPMState_PDO13_FLAG);
    }
    else if (name==PACKET_MONITOR_BPMMISC){   //血压计脉搏#/
         BPMMisc_PDO14=*((PacketBPMMisc*)dataPtr);
         if(this->isVisible())
             SetTableContent(BPMMisc_PDO14_FLAG);
    }
    else if (name==PACKET_MONITOR_WATER){   //监控板的出水口电导和温度#/
         monitorWaterOut=*((PacketWaterOut*)dataPtr);
         if(this->isVisible())
             SetTableContent(MONITOR_WATEROUT_FLAG);
    }


#endif

#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneStatusTable-ActionHandle-out");
#endif
}

void SceneStatusTable::CustomSetText()
{
}

void SceneStatusTable::setCusVisible(bool vis, char* para) {
    para = para;
    Status_show_flag=vis;
    this->setVisible(vis);
    if(vis==true)
    {
        SetTableContent(BLOODPUMP_FLAG);
        SetTableContent(BLOODPUMPTUBE_FLAG);
        SetTableContent(SUBBLOODPUMP_FLAG);
        SetTableContent(SUBBLOODPUMPTUBE_FLAG);
        SetTableContent(STATUS_ALARM_FLAG);
        SetTableContent(STATUS_MASTALARM_FLAG);
        SetTableContent(MASTERCTRL_PDO0_FLAG);
        SetTableContent(MASTERCTRL_PDOERR_FLAG);
        SetTableContent(USERCTRL_PDO1_FLAG);
        SetTableContent(USERCTRL_PDO2_FLAG);
        SetTableContent(HEPARIN_PDO0_FLAG);
        SetTableContent(MASTERCTRL_PDO2_FLAG);
        SetTableContent(UFVALUE_PDO_FLAG);
        SetTableContent(BPMState_PDO13_FLAG);
        SetTableContent(BPMMisc_PDO14_FLAG);
        SetTableContent(MONITOR_WATEROUT_FLAG);//监控板的出水口电导和温度#/
    }
}
