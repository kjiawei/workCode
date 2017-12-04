/*****************************************************
** 作者： Huangjunxian
** 创始时间：2014-1-12
** 修改人：Huangjunxian
** 修改时间：2014-1-12
** 描述：
**    5058报警记录表格页面 《源文件》
*****************************************************/
#include "config_ui.h"
#include "scene_history.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include "widget_common.h"
#include "data_sqllite.h"
#include "common_tr_string.h"
#include "state.h"
#include "textitemdelegate.h"


inline void SceneHistory::setUi()
{
    labelTitle.setGeometry(QRect(34, 21, 100, 30));
    labelTitle.setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    labelTitle.setStyleSheet("color:#dbe6ef;font-size:20px;Bold");

    QPalette palette;

    //左侧菜单栏#/
    widget.setGeometry(QRect(Widget_Setup_Other_Menu_X,
                Widget_Setup_Other_Menu_Y,
                Widget_Setup_Other_Menu_Width,
                Widget_Setup_Other_Menu_High));
    widget.setAutoFillBackground(true);
    palette.setBrush(widget.backgroundRole(), QBrush(QPixmap(":/resource/images/setup/menu_back.bmp")));
    widget.setPalette(palette);
    //左侧菜单栏添加按钮#/
    QVBoxLayout *verticalLayout = new QVBoxLayout(&widget);
    verticalLayout->addWidget(&buttonCurrent);
    verticalLayout->addWidget(&buttonHistory);
    verticalLayout->addStretch();

    widget.setLayout(verticalLayout);

    buttonCurrent.setFixedSize(124,45);
    buttonHistory.setFixedSize(124,45);

    buttonCurrent.setTextFontPos(CFG_FONT_SIZE_SECOND, CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonCurrent.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, 124, 45));

    buttonHistory.setTextFontPos(CFG_FONT_SIZE_SECOND, CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonHistory.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, 124, 45));

    buttonFirstPage.setGeometry(200,700,90,30);  //第一页#/
    buttonUpPage.setGeometry(300,700,90,30);   //上一页#/
    labelPageInfo.setGeometry(370,700,150,30);    //页码信息#/
    labelPageInfo.setAlignment(Qt::AlignCenter);
    labelPageInfo.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_TEXT, CONFIG_FONT_NORMAL));
    labelPageInfo.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    buttonDownPage.setGeometry(500,700,90,30);//下一页#/
    buttonLastPage.setGeometry(600,700,90,30); //最后一页#/

    buttonDelete.setGeometry(700,700,90,30);

    buttonGroup.addButton(&buttonCurrent, 1);
    buttonGroup.addButton(&buttonHistory, 2);

    buttonCurrent.setChecked(true);

/*---------------------当前报警表----------------------------*/
    tableWidgetCurrent->setColumnCount(6);  //设置列数#/
    tableWidgetCurrent->sortByColumn(2, Qt::DescendingOrder);  //列排序方式#/
    tableWidgetCurrent->setGeometry(Widget_Setup_Other_Menu_Width + 30,
                                    80,
                                    650,
                                    480);
/*
    tableWidgetCurrent->setColumnWidth(0, 130); //time#/
    tableWidgetCurrent->setColumnWidth(1, 80);  //who#/
    tableWidgetCurrent->setColumnWidth(2, 60);  //pri#/
    tableWidgetCurrent->setColumnWidth(3, 310); //title#/
    tableWidgetCurrent->setColumnWidth(4, 0);   //info#/
    //这个列主要是保存报警内容，当选中行时，将其内容在表格下面显示#/
    tableWidgetCurrent->setColumnHidden(4,true);
    */

    tableWidgetCurrent->setColumnWidth(0, 150); //time#/
    tableWidgetCurrent->setColumnWidth(1, 50);  //who#/
    tableWidgetCurrent->setColumnWidth(2, 50);  //pri#/
    tableWidgetCurrent->setColumnWidth(3, 100); //alarm id#/
    tableWidgetCurrent->setColumnWidth(4, 300); //title#/
    tableWidgetCurrent->setColumnWidth(5, 0);   //info#/
    //这个列主要是保存报警内容，当选中行时，将其内容在表格下面显示#/
    tableWidgetCurrent->setColumnHidden(5,true);

 //   tableWidgetCurrent->horizontalHeader()->setFixedHeight(30);
//    tableWidgetCurrent->verticalScrollBar()->setFixedWidth(40);


    tableWidgetCurrent->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    tableWidgetCurrent->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    tableWidgetCurrent->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);//ScrollBarAsNeeded   Qt::ScrollBarAlwaysOn
    tableWidgetCurrent->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //关闭编辑功能#/
    tableWidgetCurrent->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableWidgetCurrent->setStyleSheet("QTableWidget {border-style:none;background-color:transparent;"
          "padding: 2px 4px;}");
    //单选模式#/
    tableWidgetCurrent->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidgetCurrent->setSelectionBehavior(QAbstractItemView::SelectRows);
    //水平和垂直表头的显示#/
    tableWidgetCurrent->horizontalHeader()->setHidden(false);
    tableWidgetCurrent->verticalHeader()->setHidden(true);

    //告警内容提示框#/
    labelOperator.setGeometry(Widget_Setup_Other_Menu_Width + 50, 500, 550, 180);
    labelOperator.setAlignment(Qt::AlignLeft);
    labelOperator.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelOperator.setStyleSheet("QLabel {background-color:transparent;color:#ffffff;"
                                "border: 2px groove gray; border-radius: 2px; padding: 2px 4px;}");
    labelOperator.setWordWrap(true);                //自动换行#/

/*---------------------历史报警表----------------------------*/

    tableWidgetHistory->setColumnCount(7);
    tableWidgetHistory->sortByColumn(0, Qt::DescendingOrder);//id降序排列#/

    tableWidgetHistory->setGeometry(Widget_Setup_Other_Menu_Width + 30,
                                  80,
                                  650,
                                  480);

    tableWidgetHistory->setColumnWidth(0, 50);  //ID#/
    tableWidgetHistory->setColumnWidth(1, 130); //TIME#/
    tableWidgetHistory->setColumnWidth(2, 50);  //who#/
    tableWidgetHistory->setColumnWidth(3, 50);  //pri#/
    tableWidgetHistory->setColumnWidth(4, 100); //alarm id#/
    tableWidgetHistory->setColumnWidth(5, 190);  //title#/
    tableWidgetHistory->setColumnWidth(6, 80);  //dispose#/

    tableWidgetHistory->horizontalHeader()->setResizeMode(QHeaderView::Fixed);

    tableWidgetHistory->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    tableWidgetHistory->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);//ScrollBarAlwaysOff
    tableWidgetHistory->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    tableWidgetHistory->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableWidgetHistory->setStyleSheet("QTableWidget {border-style:none;background-color:transparent;"
          "padding: 2px 4px;}");

    tableWidgetHistory->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidgetHistory->setSelectionBehavior(QAbstractItemView::SelectRows);

    tableWidgetHistory->horizontalHeader()->setHidden(false);
    tableWidgetHistory->verticalHeader()->setHidden(true);

    tableWidgetHistory->setVisible(false);

    //接收硬件按键信号#/
     connect(&buttonsirqApp, SIGNAL(BtnSignal(int)),
             this, SLOT(buttonsHandle(int)), Qt::QueuedConnection);//, Qt::DirectConnection
}

SceneHistory::SceneHistory(QWidget *parent) :
    QObject(parent) ,
    labelTitle(parent) ,
    widget(parent) ,
    buttonCurrent(&widget) ,
    buttonHistory(&widget) ,
    buttonFirstPage(parent),  //第一页#/
    buttonLastPage(parent), //最后一页#/
    buttonUpPage(parent),   //上一页#/
    buttonDownPage(parent), //下一页#/
    buttonDelete(parent),
    buttonGroup(&widget),
    labelPageInfo(parent),    //页码信息#/
    labelOperator(parent)
{
    tableWidgetCurrent = new QTableWidget(parent);
    tableWidgetHistory = new QTableWidget(parent);

    setUi();

    CustomSetText();

    connect(&buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(SlotSceneActivate(int)));

    connect(&buttonFirstPage,SIGNAL(clicked()),this,SLOT(firstPageHandle()));
    connect(&buttonLastPage,SIGNAL(clicked()),this,SLOT(lastPageHandle()));
    connect(&buttonUpPage,SIGNAL(clicked()),this,SLOT(upPageHandle()));
    connect(&buttonDownPage,SIGNAL(clicked()),this,SLOT(downPageHandle()));
    connect(&buttonDelete,SIGNAL(clicked()),this,SLOT(buttonDeleteClick()));

    //选中某一行时，以该行的对象做为参数发送，根据该选中行#/
    //在下面显示报警内容信息#/
    connect(tableWidgetCurrent,SIGNAL(itemClicked(QTableWidgetItem*)),this, SLOT(currentTableWidgetClick(QTableWidgetItem*)));

    connect(tableWidgetHistory,SIGNAL(itemPressed(QTableWidgetItem *)),this,SLOT(historyTableWidgetClick(QTableWidgetItem *)));

    connect(this, SIGNAL(selectRow(int)), this, SLOT(selectTableRow(int)));

//    connect(this, SIGNAL(selectRow(int)), tableWidgetCurrent, SLOT(selectRow(int)));

    currentTablePageNum=0;   //当前记录第几页#/
    currentTablePageSum=0;   //当前记录总页数#/
    historyTablePageNum=0;   //历史记录第几页#/
    historyTablePageSum=0;   //历史记录总页数#/

    //本页面关键词#/
    PageAppendPiece("Warn History", this);

    dataNodeAddObject(PACKET_NAME_MESSAGES, this);
}

//根据选中行，在表格下方显示报警内容#/
void SceneHistory::currentTableWidgetClick(QTableWidgetItem * item)
{
    //根据行和列，返回单元格对象#/
    QTableWidgetItem *itemCol5 = tableWidgetCurrent->item(item->row(), 5);

    QString data = tr("Introduce:");
    data.append("\r\n        ");
    data.append(itemCol5->data(Qt::DisplayRole).toString());
    labelOperator.setText(data);
}

//根据选中行，将该行对象保存，作为删除选中行的依据#/
void SceneHistory::historyTableWidgetClick(QTableWidgetItem * item)
{
    this->seldel=item;
}

void SceneHistory::buttonsHandle(int key)
{
    if(key==1)   //reset#/
    {
        clearTableContents(tableWidgetCurrent);
   //     updateHistoryTable();
    }
}

void SceneHistory::firstPageHandle()   //第一页按钮处理
{
    if(tableWidgetCurrent->isVisible())//当前报警表显示时
    {
        currentTablePageNum=1;
        updateCurrentTable();
    }
    else if(tableWidgetHistory->isVisible()) //历史报警表显示时
    {
        historyTablePageNum=1;
        updateHistoryTable();
    }
}

void SceneHistory::lastPageHandle()   //最后一页
{
    if(tableWidgetCurrent->isVisible())//当前报警表显示时
    {
        currentTablePageNum=100000;  //故意设置一个大的值，让其大于计算出的总页数
        updateCurrentTable();
    }
    else if(tableWidgetHistory->isVisible()) //历史报警表显示时
    {
        historyTablePageNum=100000;
        updateHistoryTable();
    }
}

void SceneHistory::upPageHandle()  //上一页
{
    if(tableWidgetCurrent->isVisible())//当前报警表显示时
    {
        currentTablePageNum -=1;
        updateCurrentTable();
    }
    else if(tableWidgetHistory->isVisible()) //历史报警表显示时
    {
        historyTablePageNum -=1;
        updateHistoryTable();
    }
}

void SceneHistory::downPageHandle()   //下一页
{
    if(tableWidgetCurrent->isVisible())//当前报警表显示时
    {
        currentTablePageNum +=1;
        updateCurrentTable();
    }
    else if(tableWidgetHistory->isVisible()) //历史报警表显示时
    {
        historyTablePageNum +=1;
        updateHistoryTable();
    }
}

//隐藏弹出消息框，在浏览报警信息页面时，不弹出提示框#/
void SceneHistory::selectTableRow(int row)
{
    currentTableWidgetClick(tableWidgetCurrent->item(row, 0));
}

//通过按钮选择当前或者历史报警#/
void SceneHistory::SlotSceneActivate(int id)
{
    switch(id) {
    case 1: //current
        tableWidgetHistory->setVisible(false);
        tableWidgetCurrent->setVisible(true);
        updateCurrentTable();
        if(curtimeAlarmMap.size() > 0)
            labelOperator.setVisible(true);
        else if(curtimeAlarmMap.size() == 0)
            labelOperator.setVisible(false);
        buttonDelete.setVisible(false);
        Set_AlarmWidgetShow(DE_CURRENTALARMSHOW);
        break;

    case 2: //history#/
        tableWidgetCurrent->setVisible(false);
        tableWidgetHistory->setVisible(true);
        updateHistoryTable();
        labelOperator.setVisible(false);
        buttonDelete.setVisible(true);
        Set_AlarmWidgetShow(DE_HISTORYALARMSHOW);
        break;
    }
}

//删除按键的操作#/
void SceneHistory::buttonDeleteClick()
{
    if(this->seldel==NULL)
    {
        MESSAGE_PROMPT_BOX(DE_TR_PROMPT,DE_TR_PLEASESELECTAROW);
        return;
    }

    QMessageBox box(QMessageBox::Question,DE_TR_PROMPT,DE_TR_AREYOUSUREDELETE);
    QAbstractButton *bno =box.addButton(DE_TR_NO,QMessageBox::YesRole);
    box.addButton(DE_TR_YES,QMessageBox::YesRole);
    box.setWindowFlags(Qt::Widget);
    box.move(300,300);
    //消息框运行，等待点击按键退出#/
    box.exec();
    if (box.clickedButton() == bno)
        return;

    int id=-1,time,alarmid;
    QTableWidgetItem *itemCol0 = tableWidgetHistory->item(seldel->row(),0);
    id=itemCol0->data(Qt::DisplayRole).toInt();

    if(id>=0)
    {
        tableWidgetHistory->removeRow(seldel->row());

        Try_Map_mutex();
        if(historyAlarmMap.contains(id))
        {
            time=historyAlarmMap.value(id).time;
            alarmid=historyAlarmMap.value(id).alarmid;            
            //    数据库删除操作，根据time alarmid#/
            DataSql_DeletewarnTable(time,alarmid);
            historyAlarmMap.remove(id);
        }
        Map_mutex.unlock();

        this->seldel=NULL;
        updateHistoryTable();
    }
}

//数据包更新动作接口#/
void SceneHistory::ActionHandle(const void *dataPtr, int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneHistory-ActionHandle-in");
#endif
    dataPtr=dataPtr;
    if(name==PACKET_NAME_MESSAGES) {
        if(alarmWidgetShow==DE_CURRENTALARMSHOW)
            updateCurrentTable(); //更新当前报警表#/
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneHistory-ActionHandle-out");
#endif
}

//将一条报警内容插入当前报警表#/
void SceneHistory::insertCurrent(int row,AlarmInfor alarm)
{
    QString time,source,title,info;
    QDateTime timeobj;

    timeobj.setTime_t(alarm.time);
    time=timeobj.toString("yyyy-MM-dd hh:mm");
    //根据报警来源ID，返回报警来源信息#/
    source=Search_sourceinfo(alarm.who);
    //报警标题和内容#/
    Search_alarminfo(alarm.alarmid,title,info);

    tableWidgetCurrent->insertRow(row);

    QTableWidgetItem *item0 = new QTableWidgetItem(time);
    QTableWidgetItem *item1 = new QTableWidgetItem(source);
    QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(alarm.pri));
    QTableWidgetItem *item3 = new QTableWidgetItem(QString::number(alarm.alarmid));
    QTableWidgetItem *item4 = new QTableWidgetItem(title);
    QTableWidgetItem *item5 = new QTableWidgetItem(info);

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

    tableWidgetCurrent->setItem(row, 0, item0);
    tableWidgetCurrent->setItem(row, 1, item1);
    tableWidgetCurrent->setItem(row, 2, item2);
    tableWidgetCurrent->setItem(row, 3, item3);
    tableWidgetCurrent->setItem(row, 4, item4);
    tableWidgetCurrent->setItem(row, 5, item5);
}

//将一条报警内容插入历史记录报警表#/
void SceneHistory::insertHistory(int row,int id,AlarmInfor alarm)
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
    QTableWidgetItem *item4 = new QTableWidgetItem(QString::number(alarm.alarmid));
    QTableWidgetItem *item5 = new QTableWidgetItem(title);
    QTableWidgetItem *item6 = new QTableWidgetItem(QString::number(alarm.dispose));

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
    item6->setBackgroundColor(QColor(255, 255, 255));
    item6->setTextColor(QColor(35, 125, 58));
    item6->setTextAlignment(Qt::AlignCenter);

    tableWidgetHistory->setItem(row, 0, item0);
    tableWidgetHistory->setItem(row, 1, item1);
    tableWidgetHistory->setItem(row, 2, item2);
    tableWidgetHistory->setItem(row, 3, item3);
    tableWidgetHistory->setItem(row, 4, item4);
    tableWidgetHistory->setItem(row, 5, item5);
    tableWidgetHistory->setItem(row, 6, item6);
}

//更新当前报警表#/
void SceneHistory::updateCurrentTable()
{
    clearTableContents(tableWidgetCurrent);
    QMap<int,AlarmInfor> alarmMap;
//    QMap<int,AlarmInfor>::iterator it;

    Try_Map_mutex();
    alarmMap=alarmmodule->Get_curtimeAlarmMap();
    Map_mutex.unlock();

    int sum=0;

    sum=alarmMap.size();

    currentTablePageSum=sum/TABLE_PAGE_SHOW_SIZE;   //页数
    if((sum%TABLE_PAGE_SHOW_SIZE)>0)
        currentTablePageSum +=1;

    if(currentTablePageNum<1)
        currentTablePageNum=1;
    else if(currentTablePageNum>currentTablePageSum)
        currentTablePageNum=currentTablePageSum;

    setLabelPageInfor(currentTablePageNum,currentTablePageSum);
    showCurrentTable(currentTablePageNum);
}

//更新历史记录报警表#/
void SceneHistory::updateHistoryTable()
{
    clearTableContents(tableWidgetHistory);
    QMap<int,AlarmInfor> alarmMap;
    QMap<int,AlarmInfor>::iterator it;

    Try_Map_mutex();
    alarmMap=alarmmodule->Get_historyALarmMap();
    Map_mutex.unlock();

    int sum=0;

    sum=alarmMap.size();

    historyTablePageSum=sum/TABLE_PAGE_SHOW_SIZE;   //页数
    if((sum%TABLE_PAGE_SHOW_SIZE)>0)
        historyTablePageSum +=1;

    if(historyTablePageNum<1)
        historyTablePageNum=1;
    else if(historyTablePageNum>historyTablePageSum)
        historyTablePageNum=historyTablePageSum;

    setLabelPageInfor(historyTablePageNum,historyTablePageSum);
    showHistoryTable(historyTablePageNum);

    this->seldel=NULL;
}

void SceneHistory::clearTableContents(QTableWidget *table)
{

    int rownum,colnum,i,j;
    QTableWidgetItem *item=0;

    rownum=table->rowCount();
    colnum=table->columnCount();

    for(i=rownum-1;i>=0;i--)
    {
        for(j=colnum-1;j>=0;j--)
        {
            item=table->item(i,j);
            if(item!=0)
                delete item;
            item=0;
        }
        table->removeRow(i);
    }

/*
    int rownum,i;

    rownum=table->rowCount();

    for(i=rownum-1;i>=0;i--)
    {
        table->removeRow(i);
    }
*/
}

void SceneHistory::setLabelPageInfor(int cur,int sum)
{
    QString info;
    info=QString::number(cur);
    info.append(" / ");
    info.append(QString::number(sum));
    labelPageInfo.setText(info);
}

void SceneHistory::showCurrentTable(int page)  //显示当前报警表指定页
{
    clearTableContents(tableWidgetCurrent);
    QMap<int,AlarmInfor> alarmMap;
    QMap<int,AlarmInfor>::iterator it;
    int count=0,begin=0,end=0;

    Try_Map_mutex();
    alarmMap=alarmmodule->Get_curtimeAlarmMap();
    Map_mutex.unlock();

    begin=(page-1)*TABLE_PAGE_SHOW_SIZE;   //当前页开始行
    end=page*TABLE_PAGE_SHOW_SIZE;

    for(it = alarmMap.begin(); it != alarmMap.end(); ++it)
    {
        if((count>=begin)&&(count<end))
        {
            insertCurrent(0,it.value());
        }
        count++;
        if(count>end)
            break;
    }
}

void SceneHistory::showHistoryTable(int page)  //显示历史报警表指定页
{
    clearTableContents(tableWidgetHistory);
    QMap<int,AlarmInfor> alarmMap;
    QMap<int,AlarmInfor>::iterator it;
    int count=0,begin=0,end=0;

    Try_Map_mutex();
    alarmMap=alarmmodule->Get_historyALarmMap();
    Map_mutex.unlock();

    begin=(page-1)*TABLE_PAGE_SHOW_SIZE;   //当前页开始行
    end=page*TABLE_PAGE_SHOW_SIZE;

    /* 时间从小到大排列
    for(it = alarmMap.begin(); it != alarmMap.end(); it++)
    {
        if((count>=begin)&&(count<end))
        {
            insertHistory(0,it.key(),it.value());
        }
        count++;
        if(count>end)
            break;
    }
    */

    //时间从大到小排列
    for(it=(alarmMap.end()-1);it != alarmMap.begin();it--)
    {
        if((count>=begin)&&(count<end))
        {
            insertHistory(count-begin,it.key(),it.value());
        }
        count++;
        if(count>end)
            break;
    }

    this->seldel=NULL;
}


void SceneHistory::CustomSetText()
{
    labelTitle.setText(tr("Alarm record"));
    buttonHistory.setText(tr("history"));
    buttonDelete.setText(tr("delete"));    
    buttonFirstPage.setText(tr("first page"));  //第一页#/
    buttonUpPage.setText(tr("up"));            //上一页#/
    buttonDownPage.setText(tr("down"));        //下一页#/
    buttonLastPage.setText(tr("last page"));   //最后一页#/
    //表头列名#/
    tableWidgetHistory->setHorizontalHeaderLabels(QStringList() << tr("id") << tr("time") <<  tr("source") << tr("priority")<<tr("code")<<tr("title")<<tr("dispose"));
    tableWidgetCurrent->setHorizontalHeaderLabels(QStringList()<<tr("time")<<tr("source")<<tr("priority")<<tr("code")<<tr("title")<<tr("info"));
}

void SceneHistory::setCusVisible(bool vis, char* para)
{
    para = para;
    labelTitle.setVisible(vis);
    widget.setVisible(vis);
    tableWidgetCurrent->setVisible(vis);
    labelOperator.setVisible(vis);

    buttonFirstPage.setVisible(vis);  //第一页#/
    buttonUpPage.setVisible(vis);   //上一页#/
    labelPageInfo.setVisible(vis);    //页码信息#/
    buttonDownPage.setVisible(vis);//下一页#/
    buttonLastPage.setVisible(vis); //最后一页#/

    if(vis == false)
    {
        if(tableWidgetHistory->isVisible())
            tableWidgetHistory->setVisible(false);
        buttonDelete.setVisible(false);
        //设置报警页面显示状态#/
        Set_AlarmWidgetShow(DE_ALARMWIDGETHIDDEN);
        showAllDialog();
    }

    if(vis == true && curtimeAlarmMap.size() == 0)
        labelOperator.setVisible(false);
    else if(vis == true &&(curtimeAlarmMap.size() > 0||historyAlarmID>0))
    {
        updateCurrentTable();
    }

    if(vis == true)
    {
        buttonCurrent.setChecked(true);
        buttonHistory.setChecked(false);
        Set_AlarmWidgetShow(DE_CURRENTALARMSHOW);
        closeAllDialog();

        if(PDOstate.state.N1_status==0x01&&(PDOstate.state.N1_sub_status<=0x12))
        {
            buttonCurrent.setText(tr("selfcheck"));
        }
        else
        {
            buttonCurrent.setText(tr("current"));
        }
    }
}
