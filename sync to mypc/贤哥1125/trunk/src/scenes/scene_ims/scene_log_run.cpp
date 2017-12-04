#include "config_ui.h"
#include "scene_log_run.h"
#include "scene_setup_layout.h"
#include "common_tr_string.h"
#include "Daemon.h"

inline void SceneLogRun::setUi()
{
    //title = tr("Run Log");

    nextPage.setGeometry(QRect(600, 600, 80, 39));
    prePage.setGeometry(QRect(350, 600, 80, 39));
    BUTTON_SIMPLE_STYLE(nextPage);
    BUTTON_SIMPLE_STYLE(prePage);

    pageInfo.setGeometry(450,600,100,40);
    pageInfo.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    pageInfo.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                           CONFIG_FONT_NORMAL));

    pageInfo.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    tableWidgetLog->setColumnCount(5);
    tableWidgetLog->sortByColumn(0, Qt::AscendingOrder);//id降序排列

    tableWidgetLog->setGeometry(Widget_Setup_Other_Menu_Width + 30,
                                  80,
                                  630,
                                  500);

    tableWidgetLog->setColumnWidth(0, 40);  //ID
    tableWidgetLog->setColumnWidth(1, 140); //TIME
    tableWidgetLog->setColumnWidth(2, 280);//option
    tableWidgetLog->setColumnWidth(3, 60);  //type
    tableWidgetLog->setColumnWidth(4, 110);  //status
    //tableWidgetLog->setColumnWidth(5, 80);  //note

    tableWidgetLog->horizontalHeader()->setResizeMode(QHeaderView::Fixed);

    tableWidgetLog->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableWidgetLog->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//ScrollBarAlwaysOff

    tableWidgetLog->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableWidgetLog->setStyleSheet("QTableWidget {border-style:none;background-color:transparent;"
          "padding: 2px 4px;}");

    tableWidgetLog->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidgetLog->setSelectionBehavior(QAbstractItemView::SelectRows);

    tableWidgetLog->horizontalHeader()->setHidden(false);
    tableWidgetLog->verticalHeader()->setHidden(true);
    tableWidgetLog->setWordWrap(true);//
    tableWidgetLog->setVisible(false);     

    //tableWidgetLog->setStyleSheet("selection-background-color:pink");//设置选中行颜色
    connect(&nextPage,SIGNAL(clicked()),this,SLOT(nextPg()));
    connect(&prePage,SIGNAL(clicked()),this,SLOT(prePg()));

    CustomSetText();
}

SceneLogRun::SceneLogRun(QWidget *parent) :
    QObject(parent),
    nextPage(parent),
    prePage(parent),
    pageInfo(parent)
{
    tableWidgetLog = new QTableWidget(parent);
    setUi();
    PageAppendPiece("LogRun", this);
    initTableLog();
    CurrentPage=1;
//    logInfo info;
//    info.logId=1;
//    info.time="12";
//    info.action="change na from 136 to 142";
//    info.type=0x0015;
//    info.status="treatmen";
//    info.note="warning";
//    setTableLog(3,info);

}

void SceneLogRun::CustomSetText()
{
    nextPage.setText(tr("Next Page"));
    prePage.setText(tr("Pre Page"));
    tableWidgetLog->setHorizontalHeaderLabels(QStringList() << tr("id") << tr("time") <<  tr("option") << tr("type")<<tr("workstatus")<<tr("note"));

}

void SceneLogRun::setCusVisible(bool vis, char* para)
{
    para = para;
    nextPage.setVisible(vis);
    prePage.setVisible(vis);
    pageInfo.setVisible(vis);
    if(vis)
        UpdateTableLog(CurrentPage);
    tableWidgetLog->setVisible(vis);

}

void SceneLogRun::insertLog(int row,int id,logInfo log)
{
    id=id;
    tableWidgetLog->insertRow(row);
    tableWidgetLog->setRowHeight(row,40);
    QTableWidgetItem *item0 = new QTableWidgetItem("");//QString::number(id)
    QTableWidgetItem *item1 = new QTableWidgetItem(log.time);
    QTableWidgetItem *item2 = new QTableWidgetItem(log.opration);
    QTableWidgetItem *item3 = new QTableWidgetItem("");//QString::number(log.type)
    QTableWidgetItem *item4 = new QTableWidgetItem(log.status);
    //QTableWidgetItem *item5 = new QTableWidgetItem(log.note);

    QColor bkcolor=Qt::darkBlue;//QColor(0, 0, 0)
    QColor textcolor=Qt::green;//QColor(35, 125, 58)
    item0->setBackgroundColor(bkcolor);
    item0->setTextColor(textcolor);
    item0->setTextAlignment(Qt::AlignCenter);
    item1->setBackgroundColor(bkcolor);
    item1->setTextColor(textcolor);
    item1->setTextAlignment(Qt::AlignCenter);
    item2->setBackgroundColor(bkcolor);
    item2->setTextColor(Qt::yellow);
    item2->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    item3->setBackgroundColor(bkcolor);
    item3->setTextColor(textcolor);
    item3->setTextAlignment(Qt::AlignCenter);
    item4->setBackgroundColor(bkcolor);
    item4->setTextColor(textcolor);
    item4->setTextAlignment(Qt::AlignCenter);
    //item5->setBackgroundColor(QColor(255, 255, 255));
    //item5->setTextColor(QColor(35, 125, 58));
    //item5->setTextAlignment(Qt::AlignCenter);

    tableWidgetLog->setItem(row, 0, item0);
    tableWidgetLog->setItem(row, 1, item1);
    tableWidgetLog->setItem(row, 2, item2);
    tableWidgetLog->setItem(row, 3, item3);
    tableWidgetLog->setItem(row, 4, item4);
    //tableWidgetLog->setItem(row, 5, item5);
}

void SceneLogRun::initTableLog()
{
    logInfo info;
    info.id=1;
    info.time=0;
    info.opration="";
    info.type=0;//0x0013
    info.status=0;//treatmen
    info.note="";//warning
    for(int i=0;i<RUNLOG_PAGEITEMS;i++)
        insertLog(i,i+1,info);
}

//读取内容更新表格#/
void SceneLogRun::UpdateTableLog(int offSet)
{
    if(offSet<=0)
        return;

  QMap< int,logInfo> map= DataSql_GetRunLog((offSet-1)*RUNLOG_PAGEITEMS,RUNLOG_PAGEITEMS);
  QMap<int,logInfo>::iterator it;
  logInfo info;
  QString pagestr;
  int i=0;
  cleanTable();
  for(it = map.begin(); it != map.end(); it++)
  {
      info=it.value();
      info.id=(offSet-1)*RUNLOG_PAGEITEMS+i;
      setTableLog(i,info);
      i++;
  }
  //总页数#/
  int sum=0;
  int totalItem=DataSql_GetRunLogNum();
  sum=totalItem/RUNLOG_PAGEITEMS;
  if((totalItem-(sum*RUNLOG_PAGEITEMS))>0)
      sum +=1;

  pagestr.append(QString::number(CurrentPage));
  pagestr.append(" / ");
  pagestr.append(QString::number(sum));
  pageInfo.setText(pagestr);

}

void SceneLogRun::setTableLog(int row,logInfo log)
{
    QString timestr;
    QDateTime timeobj;

    timeobj.setTime_t(log.time);
    timestr=timeobj.toString("yyyy-MM-dd hh:mm");

    tableWidgetLog->item(row,0)->setText(QString::number(log.id));
    tableWidgetLog->item(row,1)->setText(timestr);
    tableWidgetLog->item(row,2)->setText(log.opration);
    tableWidgetLog->item(row,3)->setText(QString::number(log.type));

    switch(log.status)
    {
    case START_INIT: //启动初始化#/
        timestr=DE_TR_START_INIT;
        break;
    case TREAT_READY :        //准备状态#/
        timestr=DE_TR_PREPARESTATE;
        break;
    case TREAT_RUNNING://治疗状态#/
        timestr=DE_TR_TREATMENTSTATE;
        break;
    case TREAT_FINISH://治疗结束状态#/
        timestr=DE_TR_FINISHTREATMENTSTATE;
        break;
    case CLEANING://清洁状态#/
        timestr=DE_TR_CLEANINGSTATE;
        break;
    case DEBUG://调试状态#/
        timestr=DE_TR_DEBUGSTATE;
        break;
    case DRAIN://排水状态#/
        timestr=DE_TR_DRAINSTATE;
        break;
    }
     tableWidgetLog->item(row,4)->setText(timestr);
     //tableWidgetLog->item(row,5)->setText(log.note);
}

void SceneLogRun::nextPg()
{
    int totalItem=DataSql_GetRunLogNum();
    if(totalItem>(CurrentPage*RUNLOG_PAGEITEMS))
        CurrentPage++;
    else
        CurrentPage=1;

    UpdateTableLog(CurrentPage);
}

void SceneLogRun::prePg()
{
    if(CurrentPage>1)
        CurrentPage--;
    else
    {
        //总页数#/
        int sum=0;
        int totalItem=DataSql_GetRunLogNum();
        sum=totalItem/RUNLOG_PAGEITEMS;
        if((totalItem-(sum*RUNLOG_PAGEITEMS))>0)
            sum +=1;

        CurrentPage=sum;
    }
    UpdateTableLog(CurrentPage);
}

void SceneLogRun::cleanTable()
{
    for(int row=0;row<RUNLOG_PAGEITEMS;row++)
    {
    tableWidgetLog->item(row,0)->setText("");
    tableWidgetLog->item(row,1)->setText("");
    tableWidgetLog->item(row,2)->setText("");
    tableWidgetLog->item(row,3)->setText("");
    tableWidgetLog->item(row,4)->setText("");
    //tableWidgetLog->item(row,5)->setText("");
    }
}

LogRunModel::LogRunModel(void)
{
    enginLogMap=new QMap<QString,logInfo>[6];
    connect(ic->numberInputPanel,SIGNAL(RunLog(logInfo)),this,SLOT(insertLog(logInfo)));
    start(QThread::TimeCriticalPriority);
}

LogRunModel::~LogRunModel()
{
  delete [] enginLogMap;
}

bool LogRunModel::addLog(logInfo log)
{ 
    insertLog(log);
    return true;
}

// not use yet
bool LogRunModel::addLog(QString action,int type,int status)
{
    logInfo log;
    log.opration=action;
    log.type=type;
    log.status=status;
    log.time=QDateTime().currentDateTimeUtc().toTime_t(); //时间戳#/

    //插入数据库缓冲列表#/
    DataList_InsertRunLogTabel(log);
    return true;
}

//与键盘确认信号相连接#/
void LogRunModel::insertLog(logInfo log)
{
    //由于工程界面的参数设置后，不是立刻改变成效的，要按保存后才生效#/
    //因此要先将改变放到 enginLogMap中，然后点击保存后调用 LogRun->insertLog()进行保存#/
    if(enginerDialog->isVisible())//工程界面特别处理
    {
        log.type=DE_LOGINFO_ENGINEDITOR;    //工程页面编辑#/
        enginTabIndex=enginerDialog->curTabIndex();

        if(enginTabIndex>5)
            return;

//        if(enginTabIndex!=enginerDialog->curTabIndex())
//        {
//          enginTabIndex=enginerDialog->curTabIndex();
//          enginLogMap[enginTabIndex].clear();
//        }
        if(enginTabIndex==4)//factory set
        {
          logHandle(&enginLogMap[enginTabIndex],&log);
          enginLogMap[enginTabIndex].insert(log.objName,log);
          return;
        }

        if (strcmp(PageCurrent(), "Maintain Curve") == 0
            ||strcmp(PageCurrent(), "Maintain EEprom") == 0
                )
        {
            return;
        }

        else if (strcmp(PageCurrent(), "paramsEngineer") == 0)
           {
            ;//
           }

        else if (strcmp(PageCurrent(), "Maintain Formula") == 0)
           {
                if(formulaID!=enginerDialog->getFormulaID())
                {
                  formulaID=enginerDialog->getFormulaID();
                  enginLogMap[enginTabIndex].clear();
                }
                log.objName.prepend(tr("Formula")+QString::number(formulaID+1)+": ");
           }

        logHandle(&enginLogMap[enginTabIndex],&log);
        enginLogMap[enginTabIndex].insert(log.objName,log);

 //       qDebug()<<"log.objName"<<log.objName;

        return;

    }

    log.time=QDateTime().currentDateTimeUtc().toTime_t(); //时间戳#/
    if(logHandle(&log))  //将日志内容格式化#/
    {
        //插入数据库缓冲列表#/
        DataList_InsertRunLogTabel(log);
    }
}

void LogRunModel::insertLog(void)
{
  if(enginLogMap[enginTabIndex].size()<=0)
      return;

  logInfo info;
  int timesec=0;
  timesec=QDateTime().currentDateTimeUtc().toTime_t(); //时间戳#/

  QMap<QString,logInfo>::iterator it;
  for(it = enginLogMap[enginTabIndex].begin(); it != enginLogMap[enginTabIndex].end(); it++)
  {
      info=it.value();
      info.time=timesec;
      if(logHandle(&info)) //将日志内容格式化#/
      {
          //插入数据库缓冲列表#/
          DataList_InsertRunLogTabel(info);
      }
  }

  enginLogMap[enginTabIndex].clear();
}

void LogRunModel::run()
{
  for(int i=0;i<6;i++)
    enginLogMap[i].clear();

  enginTabIndex=0;
  formulaID=0;

  while(1)
  {
      msleep(500);
      //数据库缓冲列表处理 num:每次处理条数#/
      DataSql_ListHandle(2);
  }
}

//内容显示#/
bool LogRunModel::logHandle(logInfo *log)
{
    if(log->objName.isEmpty())
    {
        if(!log->opration.isEmpty())
            return true;
        return false;
    }

    if (log->strOld.compare(log->strNew)==0)
    {
     return false;
    }
    QString tmp=tr("Change: ")+log->objName;
    tmp.append(tr(" from "));
    tmp.append(log->strOld);
    tmp.append(tr(" to "));
    tmp.append(log->strNew);
    log->opration=tmp;

    log->status=PDOstate.state.N1_status;

    return true;
}

bool LogRunModel::logHandle(QMap<QString,logInfo> *map,logInfo *log)
{
    if(map->contains(log->objName))
      {
        log->strOld=map->value(log->objName).strOld;
        return true;
      }
    return false;
}

void LogRunModel::cleanEngMap(int id)
{
    if(formulaID!=id)//更改配方
    {
      enginTabIndex=enginerDialog->curTabIndex();

      logInfo log;
      log.opration.clear();
      log.objName=tr("Formula");//
      if(!logHandle(&enginLogMap[enginTabIndex],&log))
        log.strOld=tr("Formula")+QString::number(formulaID+1);
      log.strNew=tr("Formula")+QString::number(id+1);

      enginLogMap[enginTabIndex].clear();
      enginLogMap[enginTabIndex].insert(log.objName,log);
      formulaID=enginerDialog->getFormulaID();

    }
}

