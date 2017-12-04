#include "config_ui.h"
#include "widget_common.h"
#include "scene_setup_layout.h"
#include "scene_setup_bpm.h"

#include "common_func.h"
#include "global_var.h"
#include "QTextCodec"
#include "stdio.h"

#define SCENE_BPM_CREATE_T(x, pos_x, pos_y, pos_w)  \
    x.setGeometry(QRect(pos_x, SCENE_SETUP_ROW_##pos_y, \
                pos_w, SCENE_SETUP_WIDGET_HIGH));       \
    x.setAlignment( Qt::AlignVCenter | Qt::AlignRight); \
    x.setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
    x.setStyleSheet(CONFIG_COLOR_LABEL)

#define SCENE_BPM_CREATE_L(x, pos_x, pos_y)      \
    x.setGeometry(QRect(pos_x, SCENE_SETUP_ROW_##pos_y, \
                SCENE_SETUP_COL_WIDTH_EDIT-30, SCENE_SETUP_WIDGET_HIGH)); \
    x.setFont(QFont(QString(CONFIG_FONT_STYLE),     \
                    CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
    x.setAlignment(Qt::AlignVCenter | Qt::AlignCenter);    \
    LINEEDIT_STYLE(x)

#define SCENE_BPM_CREATE_U(x,pos_x, pos_y, pos_w)    \
    x.setGeometry(QRect(pos_x, SCENE_SETUP_ROW_##pos_y, \
                pos_w, SCENE_SETUP_WIDGET_HIGH));       \
    x.setAlignment( Qt::AlignVCenter | Qt::AlignLeft);  \
    x.setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
    x.setStyleSheet(CONFIG_COLOR_LABEL)


#define SCENE_BPM_CREATE_TABLE(x,y)             \
    do {                                        \
        x.setGeometry(QRect(460, SCENE_SETUP_ROW_##y,       \
                    150, SCENE_SETUP_WIDGET_HIGH));;        \
        x.setAlignment( Qt::AlignVCenter | Qt::AlignRight); \
        x.setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                    CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
        x.setStyleSheet(CONFIG_COLOR_LABEL);                \
    } while(0)

#define SCENE_BPM_CREATE_LABLE(x,pos_x,y)             \
    do {                                        \
        x.setGeometry(QRect(pos_x, SCENE_SETUP_ROW_##y,       \
                    150, SCENE_SETUP_WIDGET_HIGH));;        \
        x.setAlignment( Qt::AlignVCenter | Qt::AlignRight); \
        x.setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                    CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
        x.setStyleSheet(CONFIG_COLOR_LABEL);                \
    } while(0)

#define SCENE_BPM_CREATE_LINEEDIT(x,y)          \
    do {                                        \
        x.setGeometry(QRect(620, SCENE_SETUP_ROW_##y,   \
                    SCENE_SETUP_COL_WIDTH_EDIT,         \
                    SCENE_SETUP_WIDGET_HIGH));          \
        x.setFont(QFont(QString(CONFIG_FONT_STYLE),     \
                    CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
        x.setAlignment(Qt::AlignVCenter | Qt::AlignCenter);    \
        LINEEDIT_STYLE(x);                      \
    } while(0)

#define SCENE_BPM_CREATE_UNIT(x,y)              \
        do {                                    \
            x.setGeometry(QRect(745, SCENE_SETUP_ROW_##y,       \
                        60, SCENE_SETUP_WIDGET_HIGH));          \
            x.setAlignment( Qt::AlignVCenter | Qt::AlignLeft);  \
            x.setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                        CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
            x.setStyleSheet(CONFIG_COLOR_LABEL);        \
        } while(0)

#define SCENE_BPM_CREATE_SYM(x, pos_x, pos_y, pos_w)  \
    x.setGeometry(QRect(pos_x, pos_y, \
                pos_w, SCENE_SETUP_WIDGET_HIGH));       \
    x.setAlignment( Qt::AlignVCenter | Qt::AlignLeft); \
    x.setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
    x.setStyleSheet(CONFIG_COLOR_LABEL)



#define SCENE_BPM_SETUP_CREATE_SHOWSTYLE(x,pos_x, pos_y)        \
    do {                                        \
        SCENE_SETUP_WIDGET_XYW((x), pos_x, pos_y, SCENE_SETUP_COL_WIDTH_EDIT) ;\
        LINEEDIT_SHOWSTYLE(x); \
    } while(0)


inline void SceneSetupBPM::setUi()
{
    SCENE_BPM_CREATE_T(labelShowTime, 180, 2, 250);
    labelShowTime.setStyleSheet("color:#c6181e" );

    SCENE_BPM_SETUP_CREATE_SHOWSTYLE(lineEditShowPulse, 300,3);
    lineEditShowPulse.setText("65");
    lineEditShowPulse.setDisabled(true);

    SCENE_BPM_SETUP_CREATE_SHOWSTYLE(lineEditShowSysDia, 300,5);
    lineEditShowSysDia.setText("0/0");
    lineEditShowSysDia.setDisabled(true);

    SCENE_BPM_CREATE_T(labelHeartRate, 45, 3, 250);
    SCENE_BPM_CREATE_T(labelBP, 45, 5, 250);
    SCENE_BPM_CREATE_U(labelBPUnit, 415, 5, 100);
    labelBPUnit.setText("mmHg");

    SCENE_BPM_CREATE_T(labelPositions, 350, 9, 250);
    comboBoxPositions.setGeometry(QRect(620, SCENE_SETUP_ROW_9+4,
                    SCENE_SETUP_COL_WIDTH_COMBOBOX, SCENE_SETUP_WIDGET_HIGH-4));
    comboBoxPositions.setFont(QFont(QString(CONFIG_FONT_STYLE),
                    CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    COMBOBOX_STYLE(comboBoxPositions);
    comboBoxPositions.insertItem(0, tr("Lying"));
    comboBoxPositions.insertItem(1, tr("Sitting"));
    comboBoxPositions.insertItem(2, tr("Standing"));

    SCENE_BPM_CREATE_T(labelBelt, 350, 11, 250);
    comboBoxBelt.setGeometry(QRect(620, SCENE_SETUP_ROW_11,
                    SCENE_SETUP_COL_WIDTH_COMBOBOX, SCENE_SETUP_WIDGET_HIGH-4));
    comboBoxBelt.setFont(QFont(QString(CONFIG_FONT_STYLE),
                    CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    COMBOBOX_STYLE(comboBoxBelt);
    comboBoxBelt.insertItem(0, tr("ADult"));
    comboBoxBelt.insertItem(1, tr("Child"));

    SCENE_BPM_CREATE_T(labelSysUpper, 300, 3, 250);
    SCENE_BPM_CREATE_L(lineEditSysUpperValue, 550, 3);
    SCENE_BPM_CREATE_U(labelSysUpperUnit, 415, 5, 100);
    lineEditSysUpperValue.setText("200");

    SCENE_BPM_CREATE_TABLE(labelSysLower, 2);
    SCENE_BPM_CREATE_L(lineEditSysLowerValue, 650,3);
    SCENE_BPM_CREATE_UNIT(labelSysLowerUnit, 3);
    lineEditSysLowerValue.setText("90");
    labelSysLowerUnit.setText("mmHg");

    SCENE_BPM_CREATE_T(labelDiaUpper, 300, 5, 250);
    SCENE_BPM_CREATE_L(lineEditDiaUpperValue, 550, 5);
    SCENE_BPM_CREATE_UNIT(labelDiaUpperUnit, 5);
    lineEditDiaUpperValue.setText("100");
    labelDiaUpperUnit.setText("mmHg");

    SCENE_BPM_CREATE_LABLE(labelDiaLower, 460+100,2);
    SCENE_BPM_CREATE_L(lineEditDiaLowerValue, 650,5);
    SCENE_BPM_CREATE_UNIT(labelDiaLowerUnit, 7);
    lineEditDiaLowerValue.setText("50");
    labelDiaLowerUnit.setText("bpm");

    SCENE_BPM_CREATE_T(labelHRUpper, 300, 7, 250);
    SCENE_BPM_CREATE_L(lineEditHRUpperValue, 550, 7);
    lineEditHRUpperValue.setText("120");

    SCENE_BPM_CREATE_L(lineEditHRLowerValue,650,7);
    lineEditHRLowerValue.setText("40");

    SCENE_BPM_CREATE_T(labelBPTime, 220, 10, 200);
    SCENE_BPM_CREATE_T(labelHour, 150, 11, 100);
    SCENE_BPM_CREATE_L(lineEditHour, 250, 11);
    lineEditHour.setText("2");

    SCENE_BPM_CREATE_T(labelMin, 330, 11, 50);
    SCENE_BPM_CREATE_L(lineEditMin, 390, 11);
    lineEditMin.setText("30");

    buttonShow.setGeometry(QRect(620, SCENE_SETUP_ROW_12, 80, 39));
    buttonCancel.setGeometry(QRect(680, 600, 64, 39));
    buttonInterval.setGeometry(QRect(280, SCENE_SETUP_ROW_9, 145, 39));
    buttonQuick.setGeometry(QRect(280, SCENE_SETUP_ROW_7, 145, 39));
    buttonCurve.setGeometry(QRect(600, 600, 64, 39));

    buttonsys.setGeometry(QRect(200-30, 80, 20, 18));
    buttonsys.setStyleSheet("background-color:green;border-radius:0px;"\
                            "border:1px groove white");
    buttondia.setGeometry(QRect(200+120-30, 80, 20, 18));
    buttondia.setStyleSheet("background-color:blue;border-radius:0px;"\
                            "border:1px groove white");
    buttonmap.setGeometry(QRect(200+240-30, 80, 20, 18));
    buttonmap.setStyleSheet("background-color:yellow;border-radius:0px;"\
                            "border:1px groove white");
    buttonplu.setGeometry(QRect(200+360-30, 80, 20, 18));
    buttonplu.setStyleSheet("background-color:red;border-radius:0px;"\
                            "border:1px groove white");
    SCENE_BPM_CREATE_SYM(labelsys, 225-30, 80-12, 70);
    SCENE_BPM_CREATE_SYM(labeldia, 225+120-30, 80-12, 70);
    SCENE_BPM_CREATE_SYM(labelmap, 225+240-30, 80-12, 70);
    SCENE_BPM_CREATE_SYM(labelplu, 225+360-30, 80-12, 70);

    BUTTON_SIMPLE_STYLE(buttonShow);
    BUTTON_SIMPLE_STYLE(buttonCancel);
    BUTTON_SIMPLE_STYLE(buttonInterval);
    BUTTON_SIMPLE_STYLE(buttonQuick);
    BUTTON_SIMPLE_STYLE(buttonCurve);

    ic->appendWidget(&lineEditSysUpperValue, NumInputMethod,LineEdit, this);
    ic->appendWidget(&lineEditSysLowerValue, NumInputMethod,LineEdit, this);
    ic->appendWidget(&lineEditDiaUpperValue, NumInputMethod,LineEdit, this);
    ic->appendWidget(&lineEditDiaLowerValue, NumInputMethod,LineEdit, this);
    ic->appendWidget(&lineEditHRUpperValue, NumInputMethod,LineEdit, this);
    ic->appendWidget(&lineEditHRLowerValue, NumInputMethod,LineEdit, this);
    ic->appendWidget(&lineEditHour, NumInputMethod,LineEdit, this);
    ic->appendWidget(&lineEditMin, NumInputMethod,LineEdit, this);

    ///*
    curveBPM->setDrawCurveCount(4);
    curveBPM->setGeometry(QRect(170, 80+20, 570, 500));
    curveBPM->setLineColor(Qt::green,0);
    curveBPM->setLineColor(Qt::blue,1);
    curveBPM->setLineColor(Qt::yellow,2);
    curveBPM->setLineColor(Qt::red,3);
    curveBPM->setUnit("mmHg",0);
    curveBPM->setUnit("mmHg",1);
    curveBPM->setUnit("mmHg",2);
    curveBPM->setUnit("PLU",3);

    curveBPM->setValueZoom(1,0);
    curveBPM->setHorizontalScale(4);
    curveBPM->setVerticalScale(4);

    curveBPM->setValueZoom(1,1);
    curveBPM->setValueZoom(1,2);
    curveBPM->setValueZoom(1,3);

    curveBPM->setMode(3);
    curveBPM->setMinValue(0,0);
    curveBPM->setMaxValue(200,0);
    curveBPM->setMinValue(0,1);
    curveBPM->setMaxValue(200,1);
    curveBPM->setMinValue(0,2);
    curveBPM->setMaxValue(200,2);
    curveBPM->setMinValue(0,3);
    curveBPM->setMaxValue(200,3);

    curveBPM->setTestTime(60*60*4);
    curveBPM->setUnitX(bpmHour);
    curveBPM->start();

      //*/
    CustomSetText();

    connect(&buttonInterval, SIGNAL(clicked()), this, SLOT(button_Interval_clicked()));
    connect(&buttonQuick, SIGNAL(clicked()), this, SLOT(button_Quick_clicked()));
    connect(&buttonCurve, SIGNAL(clicked()), this, SLOT(button_Curve_clicked()));
    connect(&buttonShow, SIGNAL(clicked()), this, SLOT(button_Show_clicked()));
    connect(&buttonCancel, SIGNAL(clicked()), this, SLOT(button_Cancel_clicked()));
    connect(&comboBoxBelt,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxBelt_changed()));

      bmpTable.setRowCount(20);//设置行数为7
      bmpTable.setColumnCount(5);//设置列数为5
      bmpTable.setGeometry(QRect(170, 80, 570, 520));//设置tableWidget的大小
      //bmpTable.verticalScrollBar()->setFixedWidth(50);
      bmpTable.horizontalHeader()->setResizeMode(QHeaderView::Stretch);
      bmpTable.setEditTriggers(QAbstractItemView::NoEditTriggers);
      bmpTable.setHorizontalHeaderLabels(QStringList() <<tr("Time")<<tr("Systolic \n mmHg")<<tr("Diastolic \n mmHg")<<tr("MAP \n mmHg")<<tr("Pulse"));

      LineOne.setGeometry(QRect(480, 80, 4, 600));
      //LineOne.setGeometry(QRect(30, 500, 350, 4));
      LineOne.setFrameShape(QFrame::VLine);
      LineOne.setStyleSheet("color: rgb(240, 240, 240);");


      packetBPMparam.systolic_upper = lineEditSysUpperValue.text().toUShort();
      packetBPMparam.systolic_lower = lineEditSysLowerValue.text().toUShort();
      packetBPMparam.diastolic_upper = lineEditDiaUpperValue.text().toUShort();
      packetBPMparam.diastolic_lower = lineEditDiaLowerValue.text().toUShort();
      packetBPMparam.Max_pulse = lineEditHRUpperValue.text().toUShort();
      packetBPMparam.Min_pulse = lineEditHRLowerValue.text().toUShort();

}


SceneSetupBPM::SceneSetupBPM(QWidget *parent) :
    //QObject(parent),
    //QTableView(parent),
    labelShowTime(parent),
    lineEditShowPulse(parent),
    lineEditShowSysDia(parent),
    labelHeartRate(parent),
    //labelHeartRateValue(parent),
    labelBP(parent),
    //labelBPValue(parent),
    labelBPUnit(parent),
    labelPositions(parent),
    comboBoxPositions(parent),
    labelBelt(parent),
    comboBoxBelt(parent),
    labelBPTime(parent),
    //timeEditBP(parent),
    labelSysUpper(parent),
    lineEditSysUpperValue(parent),
    labelSysUpperUnit(parent),
    labelSysLower(parent),
    lineEditSysLowerValue(parent),
    labelSysLowerUnit(parent),
    labelDiaUpper(parent),
    lineEditDiaUpperValue(parent),
    labelDiaUpperUnit(parent),
    labelDiaLower(parent),
    lineEditDiaLowerValue(parent),
    labelDiaLowerUnit(parent),
    labelHRUpper(parent),
    lineEditHRUpperValue(parent),
    //labelHRLower(parent),
    lineEditHRLowerValue(parent),
    labelHour(parent),
    lineEditHour(parent),
    labelMin(parent),
    lineEditMin(parent),
    //labelPreselectPress(parent),
    //lineEditPreselectPress(parent),
    //labelPreselectPressUnit(parent),

    buttonShow(parent),
    buttonCancel(parent),
    buttonInterval(parent),
    buttonQuick(parent),
    buttonCurve(parent),
    buttonsys(parent),
    buttondia(parent),
    buttonmap(parent),
    buttonplu(parent),
    labelsys(parent),
    labeldia(parent),
    labelmap(parent),
    labelplu(parent),
    bmpTable(parent),
    LineOne(parent)
{
    curveBPM=new WidgetBpmCurve(parent);
    setUi();
    PageAppendPiece("Setup BPM", this);
    dataNodeAddObject(PACKET_MONITOR_BPMST, this);
    dataNodeAddObject(PACKET_MONITOR_BPMMISC, this);
    dataNodeAddObject(PACKET_REBIRTH_HANDLE,this);  //重生操作处理#/
    bmpTable.setVisible(false);
    isTable=false;
    recordCount=0;
    miscUpdate=false;
    stateUpdate=false;
    state=false;
    Quickstate=false;
    IntervalState=false;
    comboBoxBelt_flag=0;

}
void SceneSetupBPM::save()
{
    packetBPMparam.systolic_upper = lineEditSysUpperValue.text().toUShort();
    packetBPMparam.systolic_lower = lineEditSysLowerValue.text().toUShort();
    packetBPMparam.diastolic_upper = lineEditDiaUpperValue.text().toUShort();
    packetBPMparam.diastolic_lower = lineEditDiaLowerValue.text().toUShort();
    packetBPMparam.Max_pulse = lineEditHRUpperValue.text().toUShort();
    packetBPMparam.Min_pulse = lineEditHRLowerValue.text().toUShort();
    //packetBPMparam.presel_pressure = lineEditPreselectPress.text().toUShort();
    packetBPMparam.time_hour=lineEditHour.text().toInt();
    packetBPMparam.time_min=lineEditMin.text().toInt();
}

void SceneSetupBPM::button_Interval_clicked()
{
    //static bool flag;
//    IntervalState=!IntervalState;
   if(IntervalState==false)//启动自动测量
    {
       IntervalState=true;
      buttonInterval.setText(tr("Auto Measure \n OFF"));
      timer->createTimer(this, 1000);
      timePoint = QTime::currentTime();
//      MasterCtrol_CMD_BMP_Start();
     }
    else//停止自动测量
    {
      IntervalState=false;
      buttonInterval.setText(tr("Auto Measure \n ON"));
      timer->stopTimer(this);
      }
}

void SceneSetupBPM::button_Quick_clicked()
{
    if(Quickstate==false)//启动测量
    {
        Quickstate=true;
        buttonQuick.setText(tr("Cancel Measure"));

        if(comboBoxBelt_flag==0) //成人模式#/
        {
            MONITOR_CMD_BMP_Start(0);
        }
        else if(comboBoxBelt_flag==1) //小孩模式#/
        {
            MONITOR_CMD_BMP_Start(1);
        }

//        MasterCtrol_CMD_BMP_Start();
     }
    else//停止测量
    {
        state=false;
        Quickstate=false;
        buttonQuick.setText(tr("Start Measure"));
//        MasterCtrol_CMD_BMP_Stop();
        MONITOR_CMD_BMP_Stop();
        stopbmp=true;
     }
}

void SceneSetupBPM::button_Curve_clicked()
{

    isTable=!isTable;
    if(isTable==true)//显示表格
    {
        buttonCurve.setText(tr("Profile"));

        curveBPM->setVisible(false);

        bmpTable.setVisible(true);

     }
    else//隐藏表格
    {
        buttonCurve.setText(tr("Table"));

        curveBPM->setVisible(true);

        bmpTable.setVisible(false);

      }
}

void SceneSetupBPM::button_Show_clicked()//显示曲线
{

    //isShow=!isShow;
    //if(isShow==true)
    {
        buttonCurve.setText(tr("Profile"));

        curveBPM->setVisible(true);

        bmpTable.setVisible(false);
        buttonsys.setVisible(true);
        buttondia.setVisible(true);
        buttonmap.setVisible(true);
        buttonplu.setVisible(true);
        labelsys.setVisible(true);
        labeldia.setVisible(true);
        labelmap.setVisible(true);
        labelplu.setVisible(true);

        buttonCancel.setVisible(true);
        buttonCurve.setVisible(true);
        buttonShow.setVisible(false);
        LineOne.setVisible(false);

     }

}

void SceneSetupBPM::button_Cancel_clicked()//隐藏曲线
{

    //isShow=!isShow;
    //if(isShow==true)
    {
        //buttonCurve.setText(tr("Curve"));

        curveBPM->setVisible(false);

        bmpTable.setVisible(false);

        buttonsys.setVisible(false);
        buttondia.setVisible(false);
        buttonmap.setVisible(false);
        buttonplu.setVisible(false);
        labelsys.setVisible(false);
        labeldia.setVisible(false);
        labelmap.setVisible(false);
        labelplu.setVisible(false);
        buttonCancel.setVisible(false);
        buttonCurve.setVisible(false);
        buttonShow.setVisible(true);
        LineOne.setVisible(true);
     }
}

void SceneSetupBPM::comboBoxBelt_changed()
{
    comboBoxBelt_flag=comboBoxBelt.currentIndex();
}

void SceneSetupBPM::CustomSetText()
{
    //labelShowTime.setText(tr("----/--/--   --:--:--"));
    labelHeartRate.setText(tr("Pulse"));
//    labelBP.setText(tr("DIA/SYS"));
    labelBP.setText(tr("SYS/DIA"));
    labelPositions.setText(tr("Patient Position"));
    comboBoxPositions.setItemText(0, tr("Lying"));
    comboBoxPositions.setItemText(1, tr("Sitting"));
    comboBoxPositions.setItemText(2, tr("Standing"));
    labelBelt.setText(tr("Belt"));
    comboBoxBelt.setItemText(0, tr("Adult"));
    comboBoxBelt.setItemText(1, tr("Child"));

    labelBPTime.setText(tr("Measure interval"));
    labelSysUpper.setText(tr("Systolic"));
    labelSysLower.setText(tr("Upper"));
    labelDiaUpper.setText(tr("Diastolic"));
    labelDiaLower.setText(tr("Lower"));
    labelHRUpper.setText(tr("Pulse"));
    labelHour.setText(tr("Hour"));
    labelMin.setText(tr("Min"));
    //labelHRLower.setText(tr("Min Pulse:"));

    //labelPreselectPress.setText(tr("Presel pressure:"));

    buttonShow.setText(tr("Curve/Table"));
    buttonCancel.setText(tr("Close"));
    buttonInterval.setText(tr("Auto Measure \n ON"));
    buttonQuick.setText(tr("Start Measure"));
    buttonCurve.setText(tr("Table"));
    //buttonsys.setText(tr("Systolic"));
    labelsys.setText(tr("Systolic"));
    labeldia.setText(tr("Diastolic"));
    labelmap.setText(tr("MAP"));
    labelplu.setText(tr("Pulse"));
}

void SceneSetupBPM::ActionHandle(const void* dataPtr,int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupBPM-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name == 0)) {
        return;
    }
    if(stopbmp==true)
    {
      stopbmp=false;
//      state=false;
      return;
     }

    if (name==PACKET_MONITOR_BPMST) {  //监控板血压计PDO#/
        packetBPMState = *(struct PacketBPMState*)dataPtr;

//        stateUpdate=true;
        BPMState=packetBPMState;

        if(state==true&&packetBPMState.N5_BPM_status==0)//血压计从运行到停止#/
        {
            miscUpdate=true;
            stateUpdate=true;
            Quickstate=false;
            buttonQuick.setText(tr("Start Measure"));
        }

        if(state!=packetBPMState.N5_BPM_status)
        {
         //   state=!state;//packet->N5_BPM_status;
            state=packetBPMState.N5_BPM_status;
        }

        if(packetBPMState.N5_BPM_status==0)
        {
            Quickstate=false;
            buttonQuick.setText(tr("Start Measure"));

        //    lineEditShowSysDia.setText(QString::number(packetBPMState.N5_BPM_DIA)+"/"+QString::number(packetBPMState.N5_BPM_SYS));
            lineEditShowSysDia.setText(QString::number(packetBPMState.N5_BPM_SYS)+"/"+QString::number(packetBPMState.N5_BPM_DIA));

            resultdata.systolic=BPMState.N5_BPM_SYS;
            resultdata.diastolic=BPMState.N5_BPM_DIA;
            resultdata.pulse=BPMMisc.N5_BPM_pulse;

            QTime time = QTime::currentTime();
            QDate date = QDate::currentDate();
            QString timestr=date.toString(Qt::ISODate)+"   "+time.toString(Qt::TextDate);
            labelShowTime.setText(timestr);
            dataNodeUpdate(PACKET_BMPRESULT,&resultdata);
        }

        ///*
        if(state==false && stateUpdate==true)//测量成功#/
        {
            //state=!state;
//            Quickstate=!Quickstate;
//            IntervalState=!IntervalState;
            stateUpdate=false;
            bmptime = QTime::currentTime();
            bmpTable.setItem(recordCount,0,new QTableWidgetItem(bmptime.toString("hh:mm:ss")));
            bmpTable.item(recordCount,0)->setTextAlignment(Qt::AlignCenter);
            bmpTable.setItem(recordCount,1,new QTableWidgetItem(QString::number(BPMState.N5_BPM_SYS)));
            bmpTable.item(recordCount,1)->setTextAlignment(Qt::AlignCenter);
            bmpTable.setItem(recordCount,2,new QTableWidgetItem(QString::number(BPMState.N5_BPM_DIA)));
            bmpTable.item(recordCount,2)->setTextAlignment(Qt::AlignCenter);
            bmpTable.setItem(recordCount,3,new QTableWidgetItem(QString::number(BPMState.N5_BPM_MAP)));
            bmpTable.item(recordCount,3)->setTextAlignment(Qt::AlignCenter);
            bmpTable.setItem(recordCount,4,new QTableWidgetItem(QString::number(BPMMisc.N5_BPM_pulse)));
            bmpTable.item(recordCount,4)->setTextAlignment(Qt::AlignCenter);

            if(recordCount<19)recordCount++;
            else
              recordCount=0;

            curveBPM->addTestPoint(BPMState.N5_BPM_SYS,0);
            curveBPM->draw();
            curveBPM->addTestPoint(BPMState.N5_BPM_DIA,1);
            curveBPM->draw();
            curveBPM->addTestPoint(BPMState.N5_BPM_MAP,2);
            curveBPM->draw();
            curveBPM->addTestPoint(BPMMisc.N5_BPM_pulse,3);
            curveBPM->draw();

            resultdata.systolic=BPMState.N5_BPM_SYS;
            resultdata.diastolic=BPMState.N5_BPM_DIA;
            resultdata.pulse=BPMMisc.N5_BPM_pulse;

            QTime time = QTime::currentTime();
            QDate date = QDate::currentDate();
            QString timestr=date.toString(Qt::ISODate)+"   "+time.toString(Qt::TextDate);
            labelShowTime.setText(timestr);
            dataNodeUpdate(PACKET_BMPRESULT,&resultdata);

            //报警处理#/           
            if(BPMState.N5_BPM_SYS>=packetBPMparam.systolic_upper) //收缩压#/
            {
                 alarmmodule->Custom_Prompt(PROMPT_SYSTOLIC_UP);//收缩压上限报警#/
            }
            else if(BPMState.N5_BPM_SYS<=packetBPMparam.systolic_lower)
            {
                alarmmodule->Custom_Prompt(PROMPT_SYSTOLIC_DOWN);//收缩压下限报警#/
            }

            if(BPMState.N5_BPM_DIA>=packetBPMparam.diastolic_upper) //舒张压#/
            {
                alarmmodule->Custom_Prompt(PROMPT_DIASTOLIC_UP);//舒张压UP报警#/
            }
            else if(BPMState.N5_BPM_DIA<=packetBPMparam.diastolic_lower)
            {
                alarmmodule->Custom_Prompt(PROMPT_DIASTOLIC_DOWN);//舒张压DOWN报警#/
            }

            /*

            if(BPMMisc.N5_BPM_pulse>=packetBPMparam.Max_pulse) //脉搏#/
            {
                alarmmodule->Custom_Prompt(PROMPT_PULSE_UP); //脉搏UP报警#/
            }
            else if(BPMMisc.N5_BPM_pulse<=packetBPMparam.Min_pulse)
            {
                alarmmodule->Custom_Prompt(PROMPT_PULSE_DOWN); //脉搏DOWN报警#/
            }
            */

        }
        //*/
    }

    if (name==PACKET_MONITOR_BPMMISC) {
        struct PacketBPMMisc *packet = (struct PacketBPMMisc*)dataPtr;

        if(BPMMisc.N5_BPM_pulse !=packet->N5_BPM_pulse)
        {
            BPMMisc =*packet;
            //labelHeartRateValue.setNum(BPMMisc.N5_BPM_pulse);
            lineEditShowPulse.setText(QString::number(BPMMisc.N5_BPM_pulse));

            resultdata.systolic=BPMState.N5_BPM_SYS;
            resultdata.diastolic=BPMState.N5_BPM_DIA;
            resultdata.pulse=BPMMisc.N5_BPM_pulse;

            QTime time = QTime::currentTime();
            QDate date = QDate::currentDate();
            QString timestr=date.toString(Qt::ISODate)+"   "+time.toString(Qt::TextDate);
            labelShowTime.setText(timestr);
            dataNodeUpdate(PACKET_BMPRESULT,&resultdata);
        }

        if(state==false && miscUpdate==true)
        {
            if (recordCount<0)
                return;
//            if(Quickstate==false && IntervalState==false)
//                return;

//            if(Quickstate==false && IntervalState==false)
//                return;
//            Quickstate=!Quickstate;
//            IntervalState=!IntervalState;
            miscUpdate=false;
            //stateUpdate=false;

            //更新表格
            bmptime = QTime::currentTime();
            bmpTable.setItem(recordCount,0,new QTableWidgetItem(bmptime.toString("hh:mm:ss")));
            bmpTable.item(recordCount,0)->setTextAlignment(Qt::AlignCenter);
            bmpTable.setItem(recordCount,1,new QTableWidgetItem(QString::number(BPMState.N5_BPM_SYS)));
            bmpTable.item(recordCount,1)->setTextAlignment(Qt::AlignCenter);
            bmpTable.setItem(recordCount,2,new QTableWidgetItem(QString::number(BPMState.N5_BPM_DIA)));
            bmpTable.item(recordCount,2)->setTextAlignment(Qt::AlignCenter);
            bmpTable.setItem(recordCount,3,new QTableWidgetItem(QString::number(BPMState.N5_BPM_MAP)));
            bmpTable.item(recordCount,3)->setTextAlignment(Qt::AlignCenter);
            bmpTable.setItem(recordCount,4,new QTableWidgetItem(QString::number(BPMMisc.N5_BPM_pulse)));
            bmpTable.item(recordCount,4)->setTextAlignment(Qt::AlignCenter);
            bmpTable.sortItems(0, Qt::DescendingOrder);
            if(recordCount<19)recordCount++;
            else
              recordCount=0;

            //更新曲线
            curveBPM->addTestPoint(BPMState.N5_BPM_SYS,0);
            curveBPM->draw();
            curveBPM->addTestPoint(BPMState.N5_BPM_DIA,1);
            curveBPM->draw();
            curveBPM->addTestPoint(BPMState.N5_BPM_MAP,2);
            curveBPM->draw();
            curveBPM->addTestPoint(BPMMisc.N5_BPM_pulse,3);
            curveBPM->draw();

            resultdata.systolic=BPMState.N5_BPM_SYS;
            resultdata.diastolic=BPMState.N5_BPM_DIA;
            resultdata.pulse=BPMMisc.N5_BPM_pulse;

            /*
            if(stateUpdate)
            {
                stateUpdate=false;
                lineEditShowSysDia.setText(QString::number(packetBPMState.N5_BPM_DIA)+"/"+QString::number(packetBPMState.N5_BPM_SYS));
            }
            */
   //         lineEditShowSysDia.setText(QString::number(packetBPMState.N5_BPM_DIA)+"/"+QString::number(packetBPMState.N5_BPM_SYS));
            lineEditShowSysDia.setText(QString::number(packetBPMState.N5_BPM_SYS)+"/"+QString::number(packetBPMState.N5_BPM_DIA));

            QTime time = QTime::currentTime();
            QDate date = QDate::currentDate();
            QString timestr=date.toString(Qt::ISODate)+"   "+time.toString(Qt::TextDate);
            labelShowTime.setText(timestr);
            dataNodeUpdate(PACKET_BMPRESULT,&resultdata);

            //报警处理#/
            if(BPMState.N5_BPM_SYS>=packetBPMparam.systolic_upper) //收缩压#/
            {
                 alarmmodule->Custom_Prompt(PROMPT_SYSTOLIC_UP);//收缩压上限报警#/
            }
            else if(BPMState.N5_BPM_SYS<=packetBPMparam.systolic_lower)
            {
                alarmmodule->Custom_Prompt(PROMPT_SYSTOLIC_DOWN);//收缩压下限报警#/
            }

            if(BPMState.N5_BPM_DIA>=packetBPMparam.diastolic_upper) //舒张压#/
            {
                alarmmodule->Custom_Prompt(PROMPT_DIASTOLIC_UP);//舒张压UP报警#/
            }
            else if(BPMState.N5_BPM_DIA<=packetBPMparam.diastolic_lower)
            {
                alarmmodule->Custom_Prompt(PROMPT_DIASTOLIC_DOWN);//舒张压DOWN报警#/
            }

            if(BPMMisc.N5_BPM_pulse>=packetBPMparam.Max_pulse) //脉搏#/
            {
                alarmmodule->Custom_Prompt(PROMPT_PULSE_UP); //脉搏UP报警#/
            }
            else if(BPMMisc.N5_BPM_pulse<=packetBPMparam.Min_pulse)
            {
                alarmmodule->Custom_Prompt(PROMPT_PULSE_DOWN); //脉搏DOWN报警#/
            }

        }
    }
    else if(name==PACKET_REBIRTH_HANDLE)//重生恢复操作处理#/
    {
        packetBPMparam=G_P_REBIRTH->packetBPMparam;

        lineEditSysUpperValue.setText(QString::number(packetBPMparam.systolic_upper));
        lineEditSysLowerValue.setText(QString::number(packetBPMparam.systolic_lower));
        lineEditDiaUpperValue.setText(QString::number(packetBPMparam.diastolic_upper));
        lineEditDiaLowerValue.setText(QString::number(packetBPMparam.diastolic_lower));
        lineEditHRUpperValue.setText(QString::number(packetBPMparam.Max_pulse));
        lineEditHRLowerValue.setText(QString::number(packetBPMparam.Min_pulse));

        lineEditHour.setText(QString::number(packetBPMparam.time_hour));
        lineEditMin.setText(QString::number(packetBPMparam.time_min));
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupBPM-ActionHandle-out");
#endif
}

void SceneSetupBPM::setCusVisible(bool vis, char* para)
{
    para = para;

    if(vis==false)
    {
      bmpTable.setVisible(vis);
      curveBPM->setVisible(vis);
      buttonCancel.setVisible(vis);
      buttonCurve.setVisible(vis);
      buttonsys.setVisible(vis);
      buttondia.setVisible(vis);
      buttonmap.setVisible(vis);
      buttonplu.setVisible(vis);
      labelsys.setVisible(vis);
      labeldia.setVisible(vis);
      labelmap.setVisible(vis);
      labelplu.setVisible(vis);

     }

    labelShowTime.setVisible(vis);
    lineEditShowSysDia.setVisible(vis);
    lineEditShowPulse.setVisible(vis);
    labelHeartRate.setVisible(vis);
    //labelHeartRateValue.setVisible(vis);
    labelBP.setVisible(vis);
    //labelBPValue.setVisible(vis);
    labelBPUnit.setVisible(vis);
    labelPositions.setVisible(vis);
    comboBoxPositions.setVisible(vis);
    labelBelt.setVisible(vis);
    comboBoxBelt.setVisible(vis);
    labelBPTime.setVisible(vis);
    //timeEditBP.setVisible(vis);
    labelSysUpper.setVisible(vis);
    lineEditSysUpperValue.setVisible(vis);
    labelSysUpperUnit.setVisible(vis);
    labelSysLower.setVisible(vis);
    lineEditSysLowerValue.setVisible(vis);
    labelSysLowerUnit.setVisible(vis);
    labelDiaUpper.setVisible(vis);
    lineEditDiaUpperValue.setVisible(vis);
    labelDiaUpperUnit.setVisible(vis);
    labelDiaLower.setVisible(vis);
    lineEditDiaLowerValue.setVisible(vis);
    labelDiaLowerUnit.setVisible(vis);
    labelHRUpper.setVisible(vis);
    lineEditHRUpperValue.setVisible(vis);
    //labelHRLower.setVisible(vis);
    lineEditHRLowerValue.setVisible(vis);
    labelHour.setVisible(vis);
    lineEditHour.setVisible(vis);
    labelMin.setVisible(vis);
    lineEditMin.setVisible(vis);
    //labelPreselectPress.setVisible(vis);
    //lineEditPreselectPress.setVisible(vis);
    //labelPreselectPressUnit.setVisible(vis);


    buttonShow.setVisible(vis);
    //buttonCancel.setVisible(vis);
    buttonInterval.setVisible(vis);
    buttonQuick.setVisible(vis);

    LineOne.setVisible(vis);
}

void SceneSetupBPM::timer_handler(const int timeOut)
{
#if DEBUG_TIMER_HANDLE
    Debug_insert("SceneSetupBPM-timer-in");
#endif
    Q_UNUSED(timeOut);
    int iintervalTimeSec=lineEditHour.text().toInt()*3600+lineEditMin.text().toInt()*60;

    if(iintervalTimeSec ==0)
    {
        IntervalState=false;
        buttonInterval.setText(tr("Auto Measure \n ON"));
        timer->stopTimer(this);
        return;
    }

    secPass=timePoint.elapsed()/1000;
    if(secPass>=iintervalTimeSec)
     {
        if(packetBPMState.N5_BPM_status==0)//true忙 ,false空闲
        {
             timePoint = QTime::currentTime();
 //            IntervalState=false;//!IntervalState;
    //         MasterCtrol_CMD_BMP_Start();//自动测量
             if(comboBoxBelt_flag==0) //成人模式#/
             {
                 MONITOR_CMD_BMP_Start(0);
             }
             else if(comboBoxBelt_flag==1) //小孩模式#/
             {
                 MONITOR_CMD_BMP_Start(1);
             }
        }
    }
#if DEBUG_TIMER_HANDLE
    Debug_insert("SceneSetupBPM-timer-out");
#endif
}

void SceneSetupBPM::bmpTest()
{
    button_Quick_clicked();
}


