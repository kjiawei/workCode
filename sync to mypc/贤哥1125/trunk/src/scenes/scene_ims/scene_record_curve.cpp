#include "config_ui.h"
#include "global_var.h"
#include "scene_record_curve.h"
#include "scene_setup_layout.h"
#include "common_tr_string.h"
#include "data_sqllite.h"


#define DE_REFRESH_CURVE_SEC  60    //刷新曲线的间隔秒数#/

#define TABLE_PAGE_SHOW_SIZE  10      //每页显示的记录条数

inline void SceneRecordCurve::setUi()
{

    labelTitle.setGeometry(280,80,350,40);
    labelTitle.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
//    labelTitle.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_SECOND,
//                                CONFIG_FONT_WEIGHT));
    labelTitle.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                                  CONFIG_FONT_NORMAL));

    labelTitle.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelIDNum.setGeometry(560,80,100,40);
    labelIDNum.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelIDNum.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                             CONFIG_FONT_NORMAL));
    labelIDNum.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    editIDNum.setGeometry(QRect(650,80, 90,SCENE_SETUP_WIDGET_HIGH));
    LINEEDIT_STYLE(editIDNum);

    buttonCheck.setGeometry(760,80,80,40);

    labelstart.setGeometry(170,130,190,40);
    labelstart.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelstart.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                                CONFIG_FONT_NORMAL));
    labelstart.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelstarttime.setGeometry(350,130,200,40);
    labelstarttime.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelstarttime.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                                CONFIG_FONT_NORMAL));
    labelstarttime.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelend.setGeometry(170,160,190,40);
    labelend.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelend.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                                CONFIG_FONT_NORMAL));
    labelend.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelendtime.setGeometry(350,160,200,40);
    labelendtime.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelendtime.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                                CONFIG_FONT_NORMAL));
    labelendtime.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);


    labeldatenum.setGeometry(380,600,150,40);
    labeldatenum.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labeldatenum.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                                CONFIG_FONT_NORMAL));
    labeldatenum.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);



    buttonPreCure.setGeometry(QRect(560, 140,90, 39));
    buttonNxtCure.setGeometry(QRect(740, 140,90, 39));
    labelcurenum.setGeometry(620,140,150,40);
    labelcurenum.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelcurenum.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                                CONFIG_FONT_NORMAL));
    labelcurenum.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelcuretext.setGeometry(580,175,200,40);
    labelcuretext.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelcuretext.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                               CONFIG_FONT_NORMAL));
    labelcuretext.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    buttonCond.setGeometry(QRect(180, 650,90, 39));
    buttonCondDate.setGeometry(QRect(180, 700,90, 39));

    buttonPress.setGeometry(QRect(330, 650,90, 39));
    buttonPressData.setGeometry(QRect(330, 700,90, 39));

    buttonBloodPress.setGeometry(QRect(480, 650,90, 39));
    buttonBloodPressData.setGeometry(QRect(480, 700,90, 39));

    buttonTemp.setGeometry(QRect(640, 650,90, 39));
    buttonTempDate.setGeometry(QRect(640, 700,90, 39));

    buttonPreDate.setGeometry(QRect(315, 600,90, 39));
    buttonNxtDate.setGeometry(QRect(500, 600,90, 39));
//    BUTTON_SIMPLE_STYLE(buttonCond);
//    BUTTON_SIMPLE_STYLE(buttonTemp);

    /*
    //数据记录表#/
    tableWidget->setColumnCount(5);  //设置列数#/
//    tableWidget->sortByColumn(0, Qt::DescendingOrder);  //列排序方式#/
    tableWidget->setGeometry(170,220,660,370);

    tableWidget->setColumnWidth(0, 260);  //time#/
    tableWidget->setColumnWidth(1, 100);  //C701#/
    tableWidget->setColumnWidth(2, 100);  //C702#/
    tableWidget->setColumnWidth(3, 100); //C704#/
    tableWidget->setColumnWidth(4, 100); //C709#/


    tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Fixed);

    */

    tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //关闭编辑功能#/
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableWidget->setStyleSheet("QTableWidget {border-style:none;background-color:transparent;"
          "padding: 2px 4px;}");
    //单选模式#/
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //水平和垂直表头的显示#/
    tableWidget->horizontalHeader()->setHidden(false);
    tableWidget->verticalHeader()->setHidden(true);


    CustomSetText();
}

SceneRecordCurve::SceneRecordCurve(QWidget *parent) :
    QObject(parent),
    labelTitle(parent),
    labelIDNum(parent),
    editIDNum(parent),
    buttonCheck(parent),
    labelstart(parent),
    labelstarttime(parent),
    labelend(parent),
    labelendtime(parent),
    labelcurenum(parent),
    labeldatenum(parent),
    labelcuretext(parent),
    buttonCond(parent),
    buttonTemp(parent),
    buttonPressData(parent),
    buttonBloodPress(parent),
    buttonBloodPressData(parent),
    buttonPress(parent),
    buttonCondDate(parent),
    buttonTempDate(parent),
    buttonPreCure(parent),
    buttonNxtCure(parent),
    buttonPreDate(parent),
    buttonNxtDate(parent)
{
    tableWidget=new QTableWidget(parent);

    setUi();

    curCureNum=0;   //当前的治疗次数#/
    sumCureNum=0;   //总的治疗次数#/
    curDataPage=0;      //当前的页数#/
    sumDataPage=0;      //总的页数#/

#if  Widget_Data_Curve_Switch
    dataNodeAddObject(PACKET_MASTERCTRL_OUTLET,this);
    dataNodeAddObject(PACKET_MASTERCTRL_FLUIDB,this);
    dataNodeAddObject(PACKET_MASTERCTRL_MIX,this);
    dataNodeAddObject(PACKET_MASTERCTRL_FLUIDAFTER,this);
    dataNodeAddObject(PACKET_MONITOR_PRESS,this);
    dataNodeAddObject(PACKET_MONITOR_BPMST,this);
    dataNodeAddObject(PACKET_MONITOR_BPMMISC,this);
    dataNodeAddObject(TREAT_TIMER, this);   //治疗时间的状态标志#/

    connect(&buttonCheck,SIGNAL(clicked()),this,SLOT(buttonCheckSlots()));
    connect(&buttonCond,SIGNAL(clicked()),this,SLOT(buttonCondSlots()));
    connect(&buttonTemp,SIGNAL(clicked()),this,SLOT(buttonTempSlots()));
    connect(&buttonPress,SIGNAL(clicked()),this,SLOT(buttonPressSlots()));
    connect(&buttonPressData,SIGNAL(clicked()),this,SLOT(buttonPressDataSlots()));
    connect(&buttonBloodPress,SIGNAL(clicked()),this,SLOT(buttonBloodPressSlots()));
    connect(&buttonBloodPressData,SIGNAL(clicked()),this,SLOT(buttonBloodPressDataSlots()));

    connect(&buttonCondDate,SIGNAL(clicked()),this,SLOT(buttonCondDateSlots()));
    connect(&buttonTempDate,SIGNAL(clicked()),this,SLOT(buttonTempDateSlots()));
    connect(&buttonPreCure,SIGNAL(clicked()),this,SLOT(buttonPreCureSlots()));
    connect(&buttonNxtCure,SIGNAL(clicked()),this,SLOT(buttonNxtCureSlots()));
    connect(&buttonPreDate,SIGNAL(clicked()),this,SLOT(buttonPreDateSlots()));
    connect(&buttonNxtDate,SIGNAL(clicked()),this,SLOT(buttonNxtDateSlots()));

    timer->createTimer(this, 1000);
#endif
    lastsec=0;
    curve_select=0;
    curidnum=0;
    memset(&outlet,0,sizeof(PacketOutlet));
    memset(&fluidB,0,sizeof(PacketFluidB));
    memset(&mix,0,sizeof(PacketMix));
    memset(&fluidAfter,0,sizeof(PacketFluidAfter));
    PageAppendPiece("RecordCurve",this);    


    ic->appendWidget(&editIDNum, NumInputMethod,LineEdit,NULL,1,10000);

    buttonCond_flag=false;
    buttonTemp_flag=false;
    buttonPress_flag=false;
    buttonBloodPress_flag=false;

    L_idnum=0;
    L_curesum=0;
    L_curenum_now=0;
}

void SceneRecordCurve::CustomSetText()
{
//    labelTitle.setText(DE_TR_CONDRECORDCURVE);

    labelstart.setText(tr("Treatment start time:"));
//    labelstarttime.setText(tr("1970-01-01 10:36:11"));
    labelend.setText(tr("Treatment end time:"));
//    labelendtime.setText(tr("1970-01-01 10:36:16"));
    buttonCond.setText(DE_TR_CONDPROFILE);
    buttonTemp.setText(DE_TR_TEMPPROFILE);

    labelIDNum.setText(DE_TR_IDNUM+" : ");
    buttonCheck.setText(DE_TR_FIND);

    buttonPress.setText(DE_TR_PRESSPROFILE);
    buttonPressData.setText(DE_TR_PRESSDATA);

    buttonBloodPress.setText(DE_TR_BLOODPRESSPROFILE);
    buttonBloodPressData.setText(DE_TR_BLOODPRESSDATA);

    labelcurenum.setText("0/0");
    labeldatenum.setText("0/0");

    buttonCondDate.setText(DE_TR_CONDDATA);
    buttonTempDate.setText(DE_TR_TEMPDATA);

    buttonPreCure.setText(tr("Pre Cure"));
    buttonNxtCure.setText(tr("Nxt Cure"));
    buttonPreDate.setText(tr("Pre Page"));
    buttonNxtDate.setText(tr("Nxt Page"));
}

void SceneRecordCurve::setCusVisible(bool vis, char* para)
{
    static bool onceflag=false;
    para = para;

    labelTitle.setVisible(vis);
    labelIDNum.setVisible(vis);
    editIDNum.setVisible(vis);
    buttonCond.setVisible(vis);
    buttonTemp.setVisible(vis);
    buttonPress.setVisible(vis);
    buttonBloodPress.setVisible(vis);

    buttonCheck.setVisible(vis);

    buttonCondDate.setVisible(false);
    buttonTempDate.setVisible(false);
    buttonPressData.setVisible(false);
    buttonBloodPressData.setVisible(false);

    labelstart.setVisible(vis);
    labelstarttime.setVisible(vis);

    labelend.setVisible(vis);
    labelendtime.setVisible(vis);

    labelcurenum.setVisible(vis);
    labelcuretext.setVisible(vis);

    buttonPreCure.setVisible(vis);
    buttonNxtCure.setVisible(vis);

#if  Widget_Data_Curve_Switch

    if(curve_select==DE_SEL_CONDPROFILE||curve_select==DE_SEL_TEMPPROFILE||curve_select==DE_SEL_PRESSPROFILE||curve_select==DE_SEL_BLOODPRESSPROFILE)
    {
        curveWidget->setVisible(vis);
        tableWidget->setVisible(false);
        labeldatenum.setVisible(false);
        buttonPreDate.setVisible(false);
        buttonNxtDate.setVisible(false);
    }
    else if(curve_select==DE_SEL_CONDDATA||curve_select==DE_SEL_TEMPDATA||curve_select==DE_SEL_PRESSDATA||curve_select==DE_SEL_BLOODPRESSDATA)
    {
        curveWidget->setVisible(false);
        tableWidget->setVisible(vis);
        labeldatenum.setVisible(vis);
        buttonPreDate.setVisible(vis);
        buttonNxtDate.setVisible(vis);
    }


    if(onceflag==false&&vis==true)
    {
        buttonCondSlots();
        onceflag=true;
    }


#endif
}

void SceneRecordCurve::timer_handler(const int timeOut)
{
    Q_UNUSED(timeOut);

    stcuredatainfor infor;

    WorkState  stateTMP=GetWorkStateManage();
    if(TREAT_RUNNING == stateTMP.tipState.current){
    //距离上一次调用currentime函数过去的秒数#/
#if  Widget_Data_Curve_Switch
        if((G_SYS_CURTIME-lastsec)>DE_REFRESH_CURVE_SEC)
        {
            lastsec=G_SYS_CURTIME;

            infor.idnum=G_PATIENT_IDNUM_NOW;
            if(G_PATIENT_CUREID_NOW<1)
                G_PATIENT_CUREID_NOW=1;
            infor.curenum=G_PATIENT_CUREID_NOW;
            infor.timesec=G_SYS_CURTIME;
            infor.c701=fluidB.N3_B_cond;
            if(infor.c701>100000)
                infor.c701=100000;
            else if(infor.c701<0)
                infor.c701=0;
            infor.c702=outlet.N3_out_cond;
            if(infor.c702>100000)
                infor.c702=100000;
            else if(infor.c702<0)
                infor.c702=0;
            infor.c704=fluidAfter.N3_after_cond;
            if(infor.c704>100000)
                infor.c704=100000;
            else if(infor.c704<0)
                infor.c704=0;
            infor.c709=mix.N3_Mix_cond;
            if(infor.c709>100000)
                infor.c709=100000;
            else if(infor.c709<0)
                infor.c709=0;

            infor.t201=fluidB.N3_B_temp;
            if(infor.t201>100000)
                infor.t201=100000;
            else if(infor.t201<0)
                infor.t201=0;
            infor.t203=mix.N3_Mix_temp;
            if(infor.t203>100000)
                infor.t203=100000;
            else if(infor.t203<0)
                infor.t203=0;
            infor.t204=mix.N3_Mix_temp;
            if(infor.t204>100000)
                infor.t204=100000;
            else if(infor.t204<0)
                infor.t204=0;
            infor.t205=fluidAfter.N3_after_temp;
            if(infor.t205>100000)
                infor.t205=100000;
            else if(infor.t205<0)
                infor.t205=0;

            infor.ap=press.N5_AP;
            if(infor.ap>100000)
                infor.ap=100000;
            else if(infor.ap<0)
                infor.ap=0;

            infor.vp=press.N5_VP;
            if(infor.vp>100000)
                infor.vp=100000;
            else if(infor.vp<0)
                infor.vp=0;
            infor.tmp=press.N5_TMP;
            if(infor.tmp>100000)
                infor.tmp=100000;
            else if(infor.tmp<0)
                infor.tmp=0;
            infor.pulse=BPMmisc.N5_BPM_pulse;
            if(infor.pulse>100000)
                infor.pulse=100000;
            else if(infor.pulse<0)
                infor.pulse=0;
            infor.dia=BPMstate.N5_BPM_DIA;
            if(infor.dia>100000)
                infor.dia=100000;
            else if(infor.dia<0)
                infor.dia=0;
            infor.sys=BPMstate.N5_BPM_SYS;
            if(infor.sys>100000)
                infor.sys=100000;
            else if(infor.sys<0)
                infor.sys=0;


/*
            infor.idnum=11;
            infor.curenum=1;
            infor.timesec=G_SYS_CURTIME;
            infor.c701=1100;
            infor.c702=1200;
            infor.c704=1300;
            infor.c709=1400;

            infor.t201=300;
            infor.t203=310;
            infor.t204=320;
            infor.t205=330;

            infor.ap=1660;
            infor.vp=1670;
            infor.tmp=1680;
            infor.pulse=70;
            infor.dia=88;
            infor.sys=99;
*/
            //往病人信息数据表里面插入一条数据  插入数据库缓冲列表#/
            DataList_Insertcuredatainfor(infor);
        }
    }
#endif
}

void SceneRecordCurve::ActionHandle(const void* dataPtr,int name)
{
    if (name==PACKET_MASTERCTRL_OUTLET) {
        outlet=*((PacketOutlet*)dataPtr);
        return;
    } else if (name==PACKET_MASTERCTRL_FLUIDB) {
        fluidB=*((PacketFluidB*)dataPtr);
        return;
    } else if (name==PACKET_MASTERCTRL_MIX) {
        mix=*((PacketMix*)dataPtr);
        return;
    } else if (name==PACKET_MASTERCTRL_FLUIDAFTER) {
        fluidAfter=*((PacketFluidAfter*)dataPtr);
    }else if(name==PACKET_MONITOR_PRESS){
        press=*((PacketPress*)dataPtr);
    }else if(name==PACKET_MONITOR_BPMST){
        BPMstate=*((PacketBPMState*)dataPtr);
    }else if(name==PACKET_MONITOR_BPMMISC){
        BPMmisc=*((PacketBPMMisc*)dataPtr);
    }
}

 //显示指定治疗的电导曲线#/
void SceneRecordCurve::show_CondCurve()
{
    float timesec=0;
    unsigned int firtime=0;
    QMap<int,stcuredatainfor>::iterator it;

    pointC01.clear();
    pointC02.clear();
    pointC03.clear();
    pointC04.clear();
    pointC05.clear();

    //给曲线变量赋值#/
    for(it=G_MAP_CUREDATA.begin();it!=G_MAP_CUREDATA.end();it++)
    {


        if(it==G_MAP_CUREDATA.begin())
        {
            pointC01.append(QPointF(0,it.value().c701/100.0));
            pointC02.append(QPointF(0,it.value().c702/100.0));
            pointC03.append(QPointF(0,it.value().c704/100.0));
            pointC04.append(QPointF(0,it.value().c709/100.0));

            firtime=it.value().timesec;
        }
        else
        {
            timesec=(it.value().timesec-firtime)/3600.0;
            pointC01.append(QPointF(timesec,it.value().c701/100.0));
            pointC02.append(QPointF(timesec,it.value().c702/100.0));
            pointC03.append(QPointF(timesec,it.value().c704/100.0));
            pointC04.append(QPointF(timesec,it.value().c709/100.0));
        }
    }


    curve_Handler(DE_SEL_CONDPROFILE);  //选择电导曲线刷新处理#/
}

void SceneRecordCurve::show_TempCurve()   //显示指定治疗的温度曲线#/
{
    float timesec=0;
    unsigned int firtime=0;
    QMap<int,stcuredatainfor>::iterator it;


    pointC01.clear();
    pointC02.clear();
    pointC03.clear();
    pointC04.clear();
    pointC05.clear();

    //给曲线变量赋值#/
    for(it=G_MAP_CUREDATA.begin();it!=G_MAP_CUREDATA.end();it++)
    {
        if(it==G_MAP_CUREDATA.begin())
        {
            pointC01.append(QPointF(0,it.value().t201/10.0));
            pointC02.append(QPointF(0,it.value().t204/10.0));
            pointC03.append(QPointF(0,it.value().t205/10.0));
            pointC04.append(QPointF(0,it.value().t203/10.0));
            firtime=it.value().timesec;
        }
        else
        {
            timesec=(it.value().timesec-firtime)/3600.0;
            pointC01.append(QPointF(timesec,it.value().t201/10.0));
            pointC02.append(QPointF(timesec,it.value().t204/10.0));
            pointC03.append(QPointF(timesec,it.value().t205/10.0));
            pointC04.append(QPointF(timesec,it.value().t203/10.0));
        }
    }

    curve_Handler(DE_SEL_TEMPPROFILE);  //选择温度曲线刷新处理#/
}

//显示压力曲线#/
void SceneRecordCurve::show_PressCurve()
{
    float timesec=0;
    unsigned int firtime=0;
    QMap<int,stcuredatainfor>::iterator it;


    pointC01.clear();
    pointC02.clear();
    pointC03.clear();
    pointC04.clear();
    pointC05.clear();

    //给曲线变量赋值#/
    for(it=G_MAP_CUREDATA.begin();it!=G_MAP_CUREDATA.end();it++)
    {
        if(it==G_MAP_CUREDATA.begin())
        {
            pointC01.append(QPointF(0,it.value().vp-700.0));
            pointC02.append(QPointF(0,it.value().ap-700.0));
            pointC03.append(QPointF(0,it.value().tmp-700.0));
            firtime=it.value().timesec;
        }
        else
        {
            timesec=(it.value().timesec-firtime)/3600.0;
            pointC01.append(QPointF(timesec,it.value().vp-700.0));
            pointC02.append(QPointF(timesec,it.value().ap-700.0));
            pointC03.append(QPointF(timesec,it.value().tmp-700.0));
        }
    }

    curve_Handler(DE_SEL_PRESSPROFILE);  //选择温度曲线刷新处理#/

}


void SceneRecordCurve::show_BloodPressCurve()  //显示血压曲线#/
{
    float timesec=0;
    unsigned int firtime=0;
    QMap<int,stcuredatainfor>::iterator it;


    pointC01.clear();
    pointC02.clear();
    pointC03.clear();
    pointC04.clear();
    pointC05.clear();

    //给曲线变量赋值#/
    for(it=G_MAP_CUREDATA.begin();it!=G_MAP_CUREDATA.end();it++)
    {
        if(it==G_MAP_CUREDATA.begin())
        {
            pointC01.append(QPointF(0,it.value().sys));
            pointC02.append(QPointF(0,it.value().dia));
            pointC03.append(QPointF(0,it.value().pulse));
            firtime=it.value().timesec;
        }
        else
        {
            timesec=(it.value().timesec-firtime)/3600.0;
            pointC01.append(QPointF(timesec,it.value().sys));
            pointC02.append(QPointF(timesec,it.value().dia));
            pointC03.append(QPointF(timesec,it.value().pulse));
        }
    }

    curve_Handler(DE_SEL_BLOODPRESSPROFILE);  //选择温度曲线刷新处理#/

}

void SceneRecordCurve::show_CondData()    //显示指定治疗的电导数据#/
{
    curDataPage=1;
    sumDataPage= G_MAP_CUREDATA.size()/TABLE_PAGE_SHOW_SIZE;
    if((G_MAP_CUREDATA.size()%TABLE_PAGE_SHOW_SIZE)>0)
        sumDataPage +=1;

    showDataTable(DE_SEL_CONDDATA,curDataPage,sumDataPage);  //显示指定的数据表#/
}

void SceneRecordCurve::show_TempData()    //显示指定治疗的温度数据#/
{
    curDataPage=1;
    sumDataPage=G_MAP_CUREDATA.size()/TABLE_PAGE_SHOW_SIZE;
    if((G_MAP_CUREDATA.size()%TABLE_PAGE_SHOW_SIZE)>0)
        sumDataPage +=1;

    showDataTable(DE_SEL_TEMPDATA,curDataPage,sumDataPage);  //显示指定的数据表#/
}

//显示压力数据#/
void SceneRecordCurve::show_PressData()
{
    curDataPage=1;
    sumDataPage=G_MAP_CUREDATA.size()/TABLE_PAGE_SHOW_SIZE;
    if((G_MAP_CUREDATA.size()%TABLE_PAGE_SHOW_SIZE)>0)
        sumDataPage +=1;

    showDataTable(DE_SEL_PRESSDATA,curDataPage,sumDataPage);  //显示指定的数据表#/

}

//显示血压数据#/
void SceneRecordCurve::show_BloodPressData()
{
    curDataPage=1;
    sumDataPage=G_MAP_CUREDATA.size()/TABLE_PAGE_SHOW_SIZE;
    if((G_MAP_CUREDATA.size()%TABLE_PAGE_SHOW_SIZE)>0)
        sumDataPage +=1;

    showDataTable(DE_SEL_BLOODPRESSDATA,curDataPage,sumDataPage);  //显示指定的数据表#/
}



void SceneRecordCurve::clearTableContents(QTableWidget *table)
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
}

void SceneRecordCurve::showDataTable(int type,int curpage,int sumpage)  //显示指定的数据表#/
{
    QString str;

    QMap<int,stcuredatainfor>::iterator it;
    int count=0,begin=0,end=0;

    clearTableContents(tableWidget);

    if(type==DE_SEL_CONDDATA||type==DE_SEL_TEMPDATA)
    {
        //数据记录表#/
        tableWidget->setColumnCount(5);  //设置列数#/
    //    tableWidget->sortByColumn(0, Qt::DescendingOrder);  //列排序方式#/
        tableWidget->setGeometry(170,220,660,370);

        tableWidget->setColumnWidth(0, 260);  //time#/
        tableWidget->setColumnWidth(1, 100);
        tableWidget->setColumnWidth(2, 100);
        tableWidget->setColumnWidth(3, 100);
        tableWidget->setColumnWidth(4, 100);

        tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    }
    else if(type==DE_SEL_PRESSDATA||type==DE_SEL_BLOODPRESSDATA)
    {
        //数据记录表#/
        tableWidget->setColumnCount(4);  //设置列数#/
    //    tableWidget->sortByColumn(0, Qt::DescendingOrder);  //列排序方式#/
        tableWidget->setGeometry(170,220,660,370);

        tableWidget->setColumnWidth(0, 260);  //time#/
        tableWidget->setColumnWidth(1, 133);
        tableWidget->setColumnWidth(2, 133);
        tableWidget->setColumnWidth(3, 133);

        tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    }


    if(type==DE_SEL_CONDDATA)
    {                       
        //表头列名#/
        tableWidget->setHorizontalHeaderLabels(QStringList() << tr("time") << tr("C701") <<  tr("C702") << tr("C704")<<tr("C709"));
    }
    else if(type==DE_SEL_TEMPDATA)
    {
        //表头列名#/
        tableWidget->setHorizontalHeaderLabels(QStringList() << tr("time") << tr("T201") <<  tr("T203") << tr("T204")<<tr("T205"));
    }
    else if(type==DE_SEL_PRESSDATA)
    {
        //表头列名#/
        tableWidget->setHorizontalHeaderLabels(QStringList() << DE_TR_TIME << DE_TR_VP << DE_TR_AP <<DE_TR_TMP);
    }
    else if(type==DE_SEL_BLOODPRESSDATA)
    {
        //表头列名#/
        tableWidget->setHorizontalHeaderLabels(QStringList() << DE_TR_TIME << DE_TR_PULSE << DE_TR_SYS <<DE_TR_DIA);

    }

    begin=(curpage-1)*TABLE_PAGE_SHOW_SIZE;   //当前页开始行,一页多少行#/
    end=curpage*TABLE_PAGE_SHOW_SIZE;

    str.append(QString::number(curpage));
    str.append("/");
    str.append(QString::number(sumpage));
    labeldatenum.setText(str);

    for(it = G_MAP_CUREDATA.begin(); it != G_MAP_CUREDATA.end(); ++it)
    {
        if((count>=begin)&&(count<end))
        {
            insertCurrent(type,count-begin,it.value());
        }
        count++;
        if(count>end)
            break;
    }
}

void SceneRecordCurve::insertCurrent(int type,int row,stcuredatainfor info) //将一行指定数据插入指定表#/
{
    QString time;
    QDateTime timeobj;
    QTableWidgetItem *item0=NULL;
    QTableWidgetItem *item1=NULL;
    QTableWidgetItem *item2=NULL;
    QTableWidgetItem *item3=NULL;
    QTableWidgetItem *item4=NULL;


    timeobj.setTime_t(info.timesec);
    time=timeobj.toString("yyyy-MM-dd hh:mm");

    tableWidget->insertRow(row);

    if(type==DE_SEL_CONDDATA)
    {
        item0 = new QTableWidgetItem(time);
        item1 = new QTableWidgetItem(QString::number(info.c701/100.0,'f',2));
        item2 = new QTableWidgetItem(QString::number(info.c702/100.0,'f',2));
        item3 = new QTableWidgetItem(QString::number(info.c704/100.0,'f',2));
        item4 = new QTableWidgetItem(QString::number(info.c709/100.0,'f',2));
    }
    else if(type==DE_SEL_TEMPDATA)
    {
        item0 = new QTableWidgetItem(time);
        item1 = new QTableWidgetItem(QString::number(info.t201/10.0,'f',1));
        item2 = new QTableWidgetItem(QString::number(info.t203/10.0,'f',1));
        item3 = new QTableWidgetItem(QString::number(info.t204/10.0,'f',1));
        item4 = new QTableWidgetItem(QString::number(info.t205/10.0,'f',1));
    }
    else if(type==DE_SEL_PRESSDATA)
    {
        item0 = new QTableWidgetItem(time);
        item1 = new QTableWidgetItem(QString::number(info.vp-700,'f',1));
        item2 = new QTableWidgetItem(QString::number(info.ap-700,'f',1));
        item3 = new QTableWidgetItem(QString::number(info.tmp-700,'f',1));

    }
    else if(type==DE_SEL_BLOODPRESSDATA)
    {
        item0 = new QTableWidgetItem(time);
        item1 = new QTableWidgetItem(QString::number(info.pulse,'f',1));
        item2 = new QTableWidgetItem(QString::number(info.sys,'f',1));
        item3 = new QTableWidgetItem(QString::number(info.dia,'f',1));
    }

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


    if(type==DE_SEL_CONDDATA||type==DE_SEL_TEMPDATA)
    {
        item4->setBackgroundColor(QColor(255, 255, 255));
        item4->setTextColor(QColor(35, 125, 58));
        item4->setTextAlignment(Qt::AlignCenter);


        tableWidget->setItem(row, 0, item0);
        tableWidget->setItem(row, 1, item1);
        tableWidget->setItem(row, 2, item2);
        tableWidget->setItem(row, 3, item3);
        tableWidget->setItem(row, 4, item4);
    }
    else if(type==DE_SEL_PRESSDATA||type==DE_SEL_BLOODPRESSDATA)
    {
        tableWidget->setItem(row, 0, item0);
        tableWidget->setItem(row, 1, item1);
        tableWidget->setItem(row, 2, item2);
        tableWidget->setItem(row, 3, item3);
    }
}

void SceneRecordCurve::buttonCheckSlots()
{

    QDateTime timeobj;
    QString str;

    L_idnum=editIDNum.text().toInt();

    if(L_idnum==0)
    {
        showYesDialog(DE_TR_PROMPT,DE_TR_READPARAMFAIL);
        return;
    }

    G_MAP_PATIENT=DataSql_GetpatientinforByIdnum(L_idnum);

    L_curesum=G_MAP_PATIENT.size();


    if(L_curesum==0)
    {
        showYesDialog(DE_TR_PROMPT,DE_TR_NOTTHISIDNUM);
        return;
    }
    else if(L_curesum>0)
    {
        L_curenum_now=1;
        labelcurenum.setText(QString::number(L_curenum_now)+"/"+QString::number(L_curesum));
        G_MAP_CUREDATA.clear(); //病人治疗记录数据
        //根据ID号和治疗次号将该次治疗的所有数据从病人信息数据表读取出来#/
        G_MAP_CUREDATA=DataSql_GetcuredatainforByIdnum(L_idnum,L_curenum_now);



        //显示开始和结束治疗时间#/
        timeobj.setTime_t(G_MAP_PATIENT.begin().value().timesec);
        str=timeobj.toString("yyyy-MM-dd hh:mm");
        labelstarttime.setText(str);

        timeobj.setTime_t(G_MAP_PATIENT.begin().value().timesec+G_MAP_PATIENT.begin().value().time*3600);
        str=timeobj.toString("yyyy-MM-dd hh:mm");
        labelendtime.setText(str);

        if(curve_select==DE_SEL_CONDPROFILE)//标志选择温度或电导曲线或数据表 #/
            show_CondCurve();
        else if(curve_select==DE_SEL_TEMPPROFILE)
            show_TempCurve();
        else if(curve_select==DE_SEL_PRESSPROFILE)
            show_PressCurve();
        else if(curve_select==DE_SEL_BLOODPRESSPROFILE)
            show_BloodPressCurve();
        else if(curve_select==DE_SEL_CONDDATA)
            show_CondData();
        else if(curve_select==DE_SEL_TEMPDATA)
            show_TempData();
        else if(curve_select==DE_SEL_PRESSDATA)
            show_PressData();
        else if(curve_select==DE_SEL_BLOODPRESSDATA)
            show_BloodPressData();
    }

    if(L_curesum>0)
        showYesDialog(DE_TR_PROMPT,DE_TR_READINFORMATIONSUCESS);

}

void SceneRecordCurve::buttonCondSlots()
{

    if(buttonCond_flag==false)
    {
        buttonCond.setText(DE_TR_CONDDATA);

        curve_select=DE_SEL_CONDPROFILE;
        labelTitle.setText(DE_TR_CONDRECORDCURVE);
        curveWidget->setVisible(true);
        tableWidget->setVisible(false);
        labeldatenum.setVisible(false);
        buttonPreDate.setVisible(false);
        buttonNxtDate.setVisible(false);
        show_CondCurve();   //显示指定治疗的电导曲线#/
    }
    else
    {
        buttonCond.setText(DE_TR_CONDPROFILE);

        curve_select=DE_SEL_CONDDATA;
        labelTitle.setText(DE_TR_CONDRECORDDATA);

        curveWidget->setVisible(false);
        tableWidget->setVisible(true);
        labeldatenum.setVisible(true);
        buttonPreDate.setVisible(true);
        buttonNxtDate.setVisible(true);
        show_CondData();    //显示指定治疗的电导数据#/
    }

    buttonCond_flag = !buttonCond_flag;
}

void SceneRecordCurve::buttonPressSlots()
{
    if(buttonPress_flag==false)
    {
        buttonPress.setText(DE_TR_PRESSDATA);
        curve_select=DE_SEL_PRESSPROFILE;
        labelTitle.setText(DE_TR_PRESSPROFILE);
        curveWidget->setVisible(true);
        tableWidget->setVisible(false);
        labeldatenum.setVisible(false);
        buttonPreDate.setVisible(false);
        buttonNxtDate.setVisible(false);
        show_PressCurve();   //显示压力曲线#/
    }
    else
    {
        buttonPress.setText(DE_TR_PRESSPROFILE);
        curve_select=DE_SEL_PRESSDATA;
        labelTitle.setText(DE_TR_PRESSDATA);

        curveWidget->setVisible(false);
        tableWidget->setVisible(true);
        labeldatenum.setVisible(true);
        buttonPreDate.setVisible(true);
        buttonNxtDate.setVisible(true);
        show_PressData();    //显示压力数据#/
    }

    buttonPress_flag = !buttonPress_flag;
}

void SceneRecordCurve::buttonPressDataSlots()
{
    curve_select=DE_SEL_PRESSDATA;
    labelTitle.setText(DE_TR_PRESSDATA);

    curveWidget->setVisible(false);
    tableWidget->setVisible(true);
    labeldatenum.setVisible(true);
    buttonPreDate.setVisible(true);
    buttonNxtDate.setVisible(true);
 //   show_CondData();    //显示数据#/
    showDataTable(DE_SEL_PRESSDATA,curDataPage,sumDataPage);  //显示指定的数据表#/
}


void SceneRecordCurve::buttonBloodPressSlots()
{
    if(buttonBloodPress_flag==false)
    {
        buttonBloodPress.setText(DE_TR_BLOODPRESSDATA);
        curve_select=DE_SEL_BLOODPRESSPROFILE;
        labelTitle.setText(DE_TR_BLOODPRESSPROFILE);
        curveWidget->setVisible(true);
        tableWidget->setVisible(false);
        labeldatenum.setVisible(false);
        buttonPreDate.setVisible(false);
        buttonNxtDate.setVisible(false);
        show_BloodPressCurve();  //显示血压曲线#/
    }
    else
    {
        buttonBloodPress.setText(DE_TR_BLOODPRESSPROFILE);
        curve_select=DE_SEL_BLOODPRESSDATA;
        labelTitle.setText(DE_TR_BLOODPRESSDATA);
        curveWidget->setVisible(false);
        tableWidget->setVisible(true);
        labeldatenum.setVisible(true);
        buttonPreDate.setVisible(true);
        buttonNxtDate.setVisible(true);
        show_BloodPressData();//显示血压数据#/
    }

    buttonBloodPress_flag = !buttonBloodPress_flag;
}

void SceneRecordCurve::buttonBloodPressDataSlots()
{
    curve_select=DE_SEL_BLOODPRESSDATA;
    labelTitle.setText(DE_TR_BLOODPRESSDATA);
    curveWidget->setVisible(false);
    tableWidget->setVisible(true);
    labeldatenum.setVisible(true);
    buttonPreDate.setVisible(true);
    buttonNxtDate.setVisible(true);
//    show_CondData();    //显示数据#/
    showDataTable(DE_SEL_BLOODPRESSDATA,curDataPage,sumDataPage);  //显示指定的数据表#/
}


void SceneRecordCurve::buttonTempSlots()
{

    if(buttonTemp_flag==false)
    {
        buttonTemp.setText(DE_TR_TEMPDATA);
        curve_select=DE_SEL_TEMPPROFILE;
        labelTitle.setText(DE_TR_TEMPRECORDCURVE);
        curveWidget->setVisible(true);
        tableWidget->setVisible(false);
        labeldatenum.setVisible(false);
        buttonPreDate.setVisible(false);
        buttonNxtDate.setVisible(false);
        show_TempCurve();  //选择温度曲线刷新处理#/
    }
    else
    {
        buttonTemp.setText(DE_TR_TEMPPROFILE);
        curve_select=DE_SEL_TEMPDATA;
        labelTitle.setText(DE_TR_TEMPRECORDDATA);

        curveWidget->setVisible(false);
        tableWidget->setVisible(true);
        labeldatenum.setVisible(true);
        buttonPreDate.setVisible(true);
        buttonNxtDate.setVisible(true);
        show_TempData();    //显示指定治疗的温度数据#/
    }

    buttonTemp_flag = !buttonTemp_flag;
}

//电导数据表选择按钮#/
void SceneRecordCurve::buttonCondDateSlots()
{
    curve_select=DE_SEL_CONDDATA;
    labelTitle.setText(DE_TR_CONDRECORDDATA);

    curveWidget->setVisible(false);
    tableWidget->setVisible(true);
    labeldatenum.setVisible(true);
    buttonPreDate.setVisible(true);
    buttonNxtDate.setVisible(true);
    show_CondData();    //显示指定治疗的电导数据#/
}
//温度数据表选择按钮#/
void SceneRecordCurve::buttonTempDateSlots()
{
    curve_select=DE_SEL_TEMPDATA;
    labelTitle.setText(DE_TR_TEMPRECORDDATA);

    curveWidget->setVisible(false);
    tableWidget->setVisible(true);
    labeldatenum.setVisible(true);
    buttonPreDate.setVisible(true);
    buttonNxtDate.setVisible(true);
    show_TempData();    //显示指定治疗的温度数据#/
}
//上一次治疗#/
void SceneRecordCurve::buttonPreCureSlots()
{
    QDateTime timeobj;
    QString str;

    L_curenum_now--;
    if(L_curenum_now<1)
        L_curenum_now=L_curesum;

    labelcurenum.setText(QString::number(L_curenum_now)+"/"+QString::number(L_curesum));
    G_MAP_CUREDATA.clear(); //病人治疗记录数据
    //根据ID号和治疗次号将该次治疗的所有数据从病人信息数据表读取出来#/
    G_MAP_CUREDATA=DataSql_GetcuredatainforByIdnum(L_idnum,L_curenum_now);

    //显示开始和结束治疗时间#/
    timeobj.setTime_t(G_MAP_PATIENT.value(L_curenum_now-1).timesec);
    str=timeobj.toString("yyyy-MM-dd hh:mm");
    labelstarttime.setText(str);

    timeobj.setTime_t(G_MAP_PATIENT.value(L_curenum_now-1).timesec+G_MAP_PATIENT.value(L_curenum_now-1).time*3600);
    str=timeobj.toString("yyyy-MM-dd hh:mm");
    labelendtime.setText(str);

    if(curve_select==DE_SEL_CONDPROFILE)//标志选择温度或电导曲线或数据表 #/
        show_CondCurve();
    else if(curve_select==DE_SEL_TEMPPROFILE)
        show_TempCurve();
    else if(curve_select==DE_SEL_PRESSPROFILE)
        show_PressCurve();
    else if(curve_select==DE_SEL_BLOODPRESSPROFILE)
        show_BloodPressCurve();
    else if(curve_select==DE_SEL_CONDDATA)
        show_CondData();
    else if(curve_select==DE_SEL_TEMPDATA)
        show_TempData();
    else if(curve_select==DE_SEL_PRESSDATA)
        show_PressData();
    else if(curve_select==DE_SEL_BLOODPRESSDATA)
        show_BloodPressData();
}
//下一次治疗#/
void SceneRecordCurve::buttonNxtCureSlots()
{

    QDateTime timeobj;
    QString str;

    L_curenum_now++;

    if(L_curenum_now>L_curesum)
    {
        L_curenum_now=1;
    }

    labelcurenum.setText(QString::number(L_curenum_now)+"/"+QString::number(L_curesum));
    G_MAP_CUREDATA.clear(); //病人治疗记录数据
    //根据ID号和治疗次号将该次治疗的所有数据从病人信息数据表读取出来#/
    G_MAP_CUREDATA=DataSql_GetcuredatainforByIdnum(L_idnum,L_curenum_now);

    //显示开始和结束治疗时间#/
    timeobj.setTime_t(G_MAP_PATIENT.value(L_curenum_now-1).timesec);
    str=timeobj.toString("yyyy-MM-dd hh:mm");
    labelstarttime.setText(str);

    timeobj.setTime_t(G_MAP_PATIENT.value(L_curenum_now-1).timesec+G_MAP_PATIENT.value(L_curenum_now-1).time*3600);
    str=timeobj.toString("yyyy-MM-dd hh:mm");
    labelendtime.setText(str);

    if(curve_select==DE_SEL_CONDPROFILE)//标志选择温度或电导曲线或数据表 #/
        show_CondCurve();
    else if(curve_select==DE_SEL_TEMPPROFILE)
        show_TempCurve();
    else if(curve_select==DE_SEL_PRESSPROFILE)
        show_PressCurve();
    else if(curve_select==DE_SEL_BLOODPRESSPROFILE)
        show_BloodPressCurve();
    else if(curve_select==DE_SEL_CONDDATA)
        show_CondData();
    else if(curve_select==DE_SEL_TEMPDATA)
        show_TempData();
    else if(curve_select==DE_SEL_PRESSDATA)
        show_PressData();
    else if(curve_select==DE_SEL_BLOODPRESSDATA)
        show_BloodPressData();
}
//数据表上一页#/
void SceneRecordCurve::buttonPreDateSlots()
{
    curDataPage--;
    if(curDataPage<=0)
        curDataPage=sumDataPage;
    else if(curDataPage>sumDataPage)
        curDataPage=1;
    showDataTable(curve_select,curDataPage,sumDataPage);
}
//数据表下一页#/
void SceneRecordCurve::buttonNxtDateSlots()
{
    curDataPage++;
    if(curDataPage<=0)
        curDataPage=sumDataPage;
    else if(curDataPage>sumDataPage)
        curDataPage=1;
    showDataTable(curve_select,curDataPage,sumDataPage);
}

void SceneRecordCurve::curve_Handler(int type)  //选择相应曲线刷新处理#/
{
    //位置和大小#/
 //   curveWidget->setParent(parent);
    curveWidget->resetSize(170,220,660,370);
    curveWidget->setMargin(50,20);
    curveWidget->enableYaxis02(false);
    curveWidget->setMaxPointNum(1000);
    curveWidget->setFontWidth(15,8);//设置指示区、坐标区的字体大小,0为使用默认#/
    curveWidget->setAxisTickNum(5,6,7);
    curveWidget->delAllCurveLine();
    curveWidget->setXunitName(QString("H"));//设置X轴单位名称#/

    if(type==DE_SEL_CONDPROFILE)   //电导曲线#/
    {
        curveWidget->setY1unitName(QString("mS/cm"));//设置Y1轴单位名称#/
        //点连线类型#/
        curveWidget->addCurveLine(1,Linetype,QString("C701"),QColor(Qt::green));
        curveWidget->setCurveData(1,pointC01);

        curveWidget->addCurveLine(2,Linetype,QString("C702"),QColor(Qt::yellow));
        curveWidget->setCurveData(2,pointC02);

        curveWidget->addCurveLine(3,Linetype,QString("C704"),QColor(Qt::blue));
        curveWidget->setCurveData(3,pointC03);

        curveWidget->addCurveLine(4,Linetype,QString("C709"),QColor(Qt::red));
        curveWidget->setCurveData(4,pointC04);
    }
    else if(type==DE_SEL_TEMPPROFILE) //温度曲线#/
    {
        curveWidget->setY1unitName(QString(" C "));//设置Y1轴单位名称#/
        //点连线类型#/
        curveWidget->addCurveLine(1,Linetype,QString("T201"),QColor(Qt::green));
        curveWidget->setCurveData(1,pointC01);

        curveWidget->addCurveLine(2,Linetype,QString("T204"),QColor(Qt::yellow));
        curveWidget->setCurveData(2,pointC02);

        curveWidget->addCurveLine(3,Linetype,QString("T205"),QColor(Qt::blue));
        curveWidget->setCurveData(3,pointC03);

        curveWidget->addCurveLine(4,Linetype,QString("T203"),QColor(Qt::red));
        curveWidget->setCurveData(4,pointC04);
    }
    else if(type==DE_SEL_PRESSPROFILE) //压力曲线#/
    {
        curveWidget->setY1unitName(QString(" mmHg "));//设置Y1轴单位名称#/
        //点连线类型#/
        curveWidget->addCurveLine(1,Linetype,QString("VP"),QColor(Qt::green));
        curveWidget->setCurveData(1,pointC01);

        curveWidget->addCurveLine(2,Linetype,QString("AP"),QColor(Qt::yellow));
        curveWidget->setCurveData(2,pointC02);

        curveWidget->addCurveLine(3,Linetype,QString("TMP"),QColor(Qt::blue));
        curveWidget->setCurveData(3,pointC03);
    }
    else if(type==DE_SEL_BLOODPRESSPROFILE) //血压曲线#/
    {
        curveWidget->setY1unitName(QString(" mmHg "));//设置Y1轴单位名称#/

        curveWidget->enableYaxis02(true);
        curveWidget->setY2unitName(QString(" HZ/min "));
        //点连线类型#/
        curveWidget->addCurveLine(1,Linetype,QString("SYS"),QColor(Qt::green));
        curveWidget->setCurveData(1,pointC01);

        curveWidget->addCurveLine(2,Linetype,QString("DIA"),QColor(Qt::yellow));
        curveWidget->setCurveData(2,pointC02);

        curveWidget->addCurveLine(3,Linetype,QString("Pulse"),QColor(Qt::blue));
        curveWidget->setCurveData(3,pointC03);

        curveWidget->setCurveYtype(3,2);
    }

    curveWidget->setCurvePointWid(1,1);
    curveWidget->setCurvePointWid(2,1);
    curveWidget->setCurvePointWid(3,1);
    curveWidget->setCurvePointWid(4,1);
    curveWidget->refresh(); //刷新并开始曲线绘制#/
}
