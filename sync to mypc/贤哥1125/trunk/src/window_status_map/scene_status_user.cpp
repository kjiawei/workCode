#include "config_ui.h"
#include "global_var.h"
#include "scene_status_user.h"

static QStandardItem statustablemap[33][16];


inline void SceneStatusUser::setUi()
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
}

void SceneStatusUser::CustomSetColRowCount(int row, int col)
{
    int i;
    colNum = col;
    rowNum = row;
    itemCount = col*row/2;
    model.setColumnCount(colNum);
    model.setRowCount(rowNum);

    for (i=0; i<col; i++) {
        setColumnWidth(i, 72);

        switch(i)
        {
        case 0:
            model.setHeaderData(i, Qt::Horizontal, "time");
            break;
        case 1:
            model.setHeaderData(i, Qt::Horizontal, "error1");
            break;
        case 2:
            model.setHeaderData(i, Qt::Horizontal, "error2");
            break;
        case 3:
            model.setHeaderData(i, Qt::Horizontal, "error3");
            break;
        case 4:
            model.setHeaderData(i, Qt::Horizontal, "error4");
            break;
        case 5:
            model.setHeaderData(i, Qt::Horizontal, "error5");
            break;
        case 6:
            model.setHeaderData(i, Qt::Horizontal, "IOstate");
            break;
        case 7:
            model.setHeaderData(i, Qt::Horizontal, "err_code");
            break;
        default:
            model.setHeaderData(i, Qt::Horizontal, "NULL");
            break;
        }
    }

    for (i=0; i<row; i++) {
        setRowHeight(i, 22);
    }
}

bool SceneStatusUser::SetCusContent(const char *name, const unsigned value, int row)
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

void SceneStatusUser::SetTableContent(int flag)
{
    int rowIndex, colIndex;
    PacketStatusAlarm tmpval;
    QHash<unsigned int,PacketStatusAlarm>::iterator it;
    QDateTime timeobj;

    switch(flag)
    {
    case MASTER_PDO1_FLAG:   //主控板报警PDO1#/
        rowIndex=0;    //row
        colIndex=0;   //col
        statustablemap[rowIndex][colIndex].setText(QString("N3-PDO1:"));
        model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
        rowIndex=1;    //row
        for(it=masterHash.begin();it!=masterHash.end();it++)
        {
            tmpval=it.value();
            timeobj.setTime_t(it.key());

            colIndex=0;   //col
            statustablemap[rowIndex][colIndex].setText(timeobj.toString("hh:mm:ss"));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            colIndex=1;   //col
            statustablemap[rowIndex][colIndex].setText(QString::number(tmpval.alarm.errorstate1,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            colIndex=2;   //col
            statustablemap[rowIndex][colIndex].setText(QString::number(tmpval.alarm.errorstate2,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            colIndex=3;   //col
            statustablemap[rowIndex][colIndex].setText(QString::number(tmpval.alarm.errorstate3,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            colIndex=4;   //col
            statustablemap[rowIndex][colIndex].setText(QString::number(tmpval.alarm.errorstate4,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            colIndex=5;   //col
            statustablemap[rowIndex][colIndex].setText(QString::number(tmpval.alarm.errorstate5,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            colIndex=6;   //col
            statustablemap[rowIndex][colIndex].setText(QString::number(tmpval.alarm.Iostatus,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            colIndex=7;   //col
            statustablemap[rowIndex][colIndex].setText(QString::number(tmpval.alarm.errorstate7));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);

            rowIndex++;
        }
        break;
    case MONIT_PDO1_FLAG:     //监控板报警PDO1#/
        rowIndex=13;    //row
        colIndex=0;   //col
        statustablemap[rowIndex][colIndex].setText(QString("N5-PDO1:"));
        model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
        rowIndex=14;    //row
        for(it=monitHash.begin();it!=monitHash.end();it++)
        {
            tmpval=it.value();
            timeobj.setTime_t(it.key());

            colIndex=0;   //col
            statustablemap[rowIndex][colIndex].setText(timeobj.toString("hh:mm:ss"));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            colIndex=1;   //col
            statustablemap[rowIndex][colIndex].setText(QString::number(tmpval.alarm.errorstate1,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            colIndex=2;   //col
            statustablemap[rowIndex][colIndex].setText(QString::number(tmpval.alarm.errorstate2,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            colIndex=3;   //col
            statustablemap[rowIndex][colIndex].setText(QString::number(tmpval.alarm.errorstate3,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            colIndex=4;   //col
            statustablemap[rowIndex][colIndex].setText(QString::number(tmpval.alarm.errorstate4,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            colIndex=5;   //col
            statustablemap[rowIndex][colIndex].setText(QString::number(tmpval.alarm.errorstate5,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            colIndex=6;   //col
            statustablemap[rowIndex][colIndex].setText(QString::number(tmpval.alarm.Iostatus,2).toAscii());
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);
            colIndex=7;   //col
            statustablemap[rowIndex][colIndex].setText(QString::number(tmpval.alarm.errorstate7));
            model.setItem(rowIndex, colIndex, &statustablemap[rowIndex][colIndex]);

            rowIndex++;
        }
        break;
    }
}

SceneStatusUser::SceneStatusUser(QWidget *parent) :
    QTableView(parent),
    model(parent)
{
    setUi();

//    dataNodeAddObject(PACKET_STATUS_MASTALARM,this);    //主控板PDO1#/
//    dataNodeAddObject(PACKET_STATUS_ALARM,this);        //监控板报警PDO1#/

    masterHash.clear();
    monitHash.clear();

    PageAppendPiece("Status User", this);
}

void SceneStatusUser::ActionHandle(const void* dataPtr,int name)
{
    dataPtr=dataPtr;
    name=name;
/*

#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneStatusUser-ActionHandle-in");
#endif

    unsigned int time;      //时间戳#/

    time=QDateTime().currentDateTimeUtc().toTime_t(); //时间戳#/

    if (name==PACKET_STATUS_MASTALARM){
        masterAlarmData=*((PacketStatusAlarm *)dataPtr);

        if(masterHash.size()>10)
        {
            masterHash.remove(masterHash.begin().key());
        }
        masterHash.insert(time,masterAlarmData);

        if(this->isVisible())
            SetTableContent(MASTER_PDO1_FLAG);
    }
    else if (name==PACKET_STATUS_ALARM){
        monitAlarmData=*((PacketStatusAlarm *)dataPtr);

        if(monitHash.size()>10)
        {
            monitHash.remove(monitHash.begin().key());
        }
        monitHash.insert(time,monitAlarmData);

        if(this->isVisible())
            SetTableContent(MONIT_PDO1_FLAG);
    }


#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneStatusUser-ActionHandle-out");
#endif

*/
}

void SceneStatusUser::setCusVisible(bool vis, char* para)
{
    para = para;
    this->setVisible(vis);
    /*
    if(vis==true)
    {
        SetTableContent(MASTER_PDO1_FLAG);
        SetTableContent(MONIT_PDO1_FLAG);
    }
    */
}

void SceneStatusUser::CustomSetText()
{

}
