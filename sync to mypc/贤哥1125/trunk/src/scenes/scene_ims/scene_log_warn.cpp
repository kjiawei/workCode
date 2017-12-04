#include "config_ui.h"
#include "scene_log_warn.h"
#include "scene_setup_layout.h"


inline void SceneLogWarn::setUi()
{
    //title = tr("Warn Log");
/*
    buttonSave.setGeometry(QRect(620, 500, 64, 39));
    buttonSave.setText(tr("Confirm"));
    buttonCancel.setGeometry(QRect(694, 500, 64, 39));
    buttonCancel.setText(tr("Cancel"));
    BUTTON_SIMPLE_STYLE(buttonSave);
    BUTTON_SIMPLE_STYLE(buttonCancel);
*/
    /*---------------------历史报警表----------------------------*/

        tableWidgetHistory->setColumnCount(6);
        tableWidgetHistory->sortByColumn(0, Qt::DescendingOrder);//id降序排列#/

        tableWidgetHistory->setGeometry(Widget_Setup_Other_Menu_Width + 50,
                                      80,
                                      600,
                                      500);

        tableWidgetHistory->setColumnWidth(0, 40);  //ID#/
        tableWidgetHistory->setColumnWidth(1, 120); //TIME#/
        tableWidgetHistory->setColumnWidth(2, 80);//who#/
        tableWidgetHistory->setColumnWidth(3, 60);  //pri#/
        tableWidgetHistory->setColumnWidth(4, 220);  //title#/
        tableWidgetHistory->setColumnWidth(5, 80);  //dispose#/

        tableWidgetHistory->horizontalHeader()->setResizeMode(QHeaderView::Fixed);

        tableWidgetHistory->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableWidgetHistory->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);//ScrollBarAlwaysOff

        tableWidgetHistory->setEditTriggers(QAbstractItemView::NoEditTriggers);

        tableWidgetHistory->setStyleSheet("QTableWidget {border-style:none;background-color:transparent;"
              "padding: 2px 4px;}");

        tableWidgetHistory->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidgetHistory->setSelectionBehavior(QAbstractItemView::SelectRows);

        tableWidgetHistory->horizontalHeader()->setHidden(false);
        tableWidgetHistory->verticalHeader()->setHidden(true);

        tableWidgetHistory->setVisible(false);


    CustomSetText();
}

SceneLogWarn::SceneLogWarn(QWidget *parent) :
    QObject(parent)
//    buttonSave(parent),
//    buttonCancel(parent)
{
    tableWidgetHistory = new QTableWidget(parent);
    setUi();
    PageAppendPiece("LogWarn", this);
}

void SceneLogWarn::CustomSetText()
{
//    buttonSave.setText(tr("Confirm"));
//    buttonCancel.setText(tr("Cancel"));
    tableWidgetHistory->setHorizontalHeaderLabels(QStringList() << tr("id") << tr("time") <<  tr("source") << tr("priority")<<tr("title")<<tr("dispose"));
}

void SceneLogWarn::setCusVisible(bool vis, char* para)
{
    para = para;
//    buttonSave.setVisible(vis);
//    buttonCancel.setVisible(vis);
    if(vis==true)
        updateHistoryTable();
    tableWidgetHistory->setVisible(vis);
}

void SceneLogWarn::insertHistory(int row,int id,AlarmInfor alarm)
{
    QString time,source,title,info;
    QDateTime timeobj;

    timeobj.setTime_t(alarm.time);
    time=timeobj.toString("yyyy-MM-dd hh:mm");
    //根据报警来源ID，返回报警来源信息#/
    source=Search_sourceinfo(alarm.who);
    //报警标题和内容#/
    Search_alarminfo(alarm.alarmid,title,info);

    tableWidgetHistory->insertRow(row);

    QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(id));
    QTableWidgetItem *item1 = new QTableWidgetItem(time);
    QTableWidgetItem *item2 = new QTableWidgetItem(source);
    QTableWidgetItem *item3 = new QTableWidgetItem(QString::number(alarm.pri));
    QTableWidgetItem *item4 = new QTableWidgetItem(title);
    QTableWidgetItem *item5 = new QTableWidgetItem(QString::number(alarm.dispose));

    item0->setBackgroundColor(QColor(255, 255, 255));
    item0->setTextColor(QColor(35, 125, 58));
    item0->setTextAlignment(Qt::AlignCenter);
    item1->setBackgroundColor(QColor(255, 255, 255));
    item1->setTextColor(QColor(35, 125, 58));
    item1->setTextAlignment(Qt::AlignCenter);
    item2->setBackgroundColor(QColor(255, 255, 255));
    item2->setTextColor(QColor(35, 125, 58));
    item2->setTextAlignment(Qt::AlignCenter);
    item3->setBackgroundColor(QColor(255, 255, 255));
    item3->setTextColor(QColor(35, 125, 58));
    item3->setTextAlignment(Qt::AlignCenter);
    item4->setBackgroundColor(QColor(255, 255, 255));
    item4->setTextColor(QColor(35, 125, 58));
    item4->setTextAlignment(Qt::AlignCenter);
    item5->setBackgroundColor(QColor(255, 255, 255));
    item5->setTextColor(QColor(35, 125, 58));
    item5->setTextAlignment(Qt::AlignCenter);

    tableWidgetHistory->setItem(row, 0, item0);
    tableWidgetHistory->setItem(row, 1, item1);
    tableWidgetHistory->setItem(row, 2, item2);
    tableWidgetHistory->setItem(row, 3, item3);
    tableWidgetHistory->setItem(row, 4, item4);
    tableWidgetHistory->setItem(row, 5, item5);
}

void SceneLogWarn::updateHistoryTable()
{
    clearTableContents(tableWidgetHistory);

    QMap<int,AlarmInfor>::iterator it;

    for(it = historyAlarmMap.begin(); it != historyAlarmMap.end(); it++)
    {
        insertHistory(0,it.key(),it.value());
    }
}

void SceneLogWarn::clearTableContents(QTableWidget *table)
{
    int rownum,i;

    rownum=table->rowCount();

    for(i=rownum-1;i>=0;i--)
        table->removeRow(i);
}
