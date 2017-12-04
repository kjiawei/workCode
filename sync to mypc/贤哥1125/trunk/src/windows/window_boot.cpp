#include <QApplication>
#include <QPainter>
#include <QWidget>
#include <QtGui>

#include "config_ui.h"
#include "window_boot.h"

#include "common_func.h"

#include "middle.h"

#include "global_var.h"
#include "common_tr_string.h"


WindowBoot::WindowBoot(QWidget *parent) :
    QWidget(parent),
    label(this),
    widgetButton(this),
    ButtonDebugEnter(this)
{
    resize(CONFIG_SCREEN_WIDTH, CONFIG_SCREEN_HIGH);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/resource/images/boot/boot_background.bmp")));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    label.setGeometry(QRect(BOOT_SCREEN_LABEL_X,
                BOOT_SCREEN_LABEL_Y,
                BOOT_SCREEN_LABEL_WIDTH,
                BOOT_SCREEN_LABEL_HIGH));
    label.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    label.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_FIRST_2, CONFIG_FONT_NORMAL));
    label.setStyleSheet(CONFIG_COLOR_LABEL);

    widgetButton.setStyleNormal(":/resource/images/boot/button.png",
            QRect(BOOT_SCREEN_BUTTON_X,
                BOOT_SCREEN_BUTTON_Y,
                BOOT_SCREEN_BUTTON_WIDTH,
                BOOT_SCREEN_BUTTON_HIGH));
    widgetButton.setTextFontPos(CFG_FONT_SIZE_FIRST_2, CONFIG_FONT_NORMAL, CONFIG_COLOR_BLUE,
            QRect(0, 0, BOOT_SCREEN_BUTTON_WIDTH, BOOT_SCREEN_BUTTON_HIGH));

    ButtonDebugEnter.setGeometry(800,660,150,50);

    timeCount = 5000;
    timeNow = timeCount;
    pixmapShaft = QPixmap(":/resource/images/boot/locating_shaft.png");

    CustomSetText();
    //多窗口显示的关键，通过hash表，把该窗口和关键字 Boot关联起来#/
    //然后通过pageManage类，使用字符串Boot调度该窗口显示#/
    PageAppendPiece((char*)"Boot", this);

    PDOstate.state.N1_sub_status=0x02;

    startDelay=false;
    startDelayCnt=0;

    YesNoDialog=new Window_YESNO(window_parent);
    YesDialog=new Window_YES(window_parent);

    timer->createTimer(this, 50);//该定时器时间到显示主页面，由于消毒没有完成，开机直接进入消毒界面#/

    connect(&widgetButton, SIGNAL(clicked()), this, SLOT(PassCheck()));
    connect(&ButtonDebugEnter,SIGNAL(clicked()),this,SLOT(DebugMode_Enter()));

    closeYesNoDialog();
    closeYesDialog();
}

//时间到，跳到主页面，进入自检#/
void WindowBoot::timer_handler(const int timeOut)
{
    Q_UNUSED(timeOut);
#if DEBUG_TIMER_HANDLE
    Debug_insert("WindowBoot-timer-in");
#endif

    if(G_rebirth_flag==true)  //重生操作时取消处理#/
        return;


        if(startDelay==false)
        {
            timeNow -= 50;
            if (timeNow <= 0) {    //-----------------process B

               if(Disinfection_No_completed==false) //上次消毒完成#/
               {
                    startDelay=true;
                    selfcheck_enable = true;//启动自检#/
//                    dataNodeUpdate(SelfCheck_Timer,&selfcheck_enable);

                    PageShow("Main");

                    WorkState stateTMP = GetWorkStateManage();
                    stateTMP.tipState.current = TREAT_READY;
                    stateTMP.subState.current = SELFTEST;
                    stateTMP.disinfectEnable = true;
                    SetWorkStateManage(&stateTMP);//设置为准备状态下的正在自检#/
               }
               else if(Disinfection_No_completed==true)
               {
                   selfcheck_enable=false;//跳过自检#/

                     timer->stopTimer(this);
                     startMsg();

                      PageShow("Disinfect");

               }
            }

            update();
        }
        else
        {
             startDelayCnt++;
             if(startDelayCnt>=(20))
             {
               timer->stopTimer(this);
               startMsg();
             }
        }

#if DEBUG_TIMER_HANDLE
    Debug_insert("WindowBoot-timer-out");
#endif
}

//重生时操作 self_flag:自检标志#/
void WindowBoot::Selfcheck_Pass(void)
{
    selfcheck_enable=false;//跳过自检#/
    startDelay=true;

    PageShow("Main");

    timer->stopTimer(this);

    setAlarmDialogOnTop();

    dataNodeUpdate(SelfCheck_Timer,&selfcheck_enable);

    usleep(2000);

    enginerDialog->getversion();

    alarmmodule->Reset_AlarmHandle();//复位报警操作#/

    YesNoDialog->disconnect(this);

//    usleep(10000);

}

void WindowBoot::startRecive(void)
{
    Message mess;
    memset(&mess,0,sizeof(mess));
    mess.cob_id = 0x183;
    mess.data[0] = 0;
    mess.data[1] = 1;//只是为了使能isShowMain#/
    MessageSignalMiddle(&mess);//模拟接收到主控板的PDO，就是为了设置DataCan的isShowMain#/
}

//开始自检前的操作#/
void WindowBoot::startMsg(void)
{
    setAlarmDialogOnTop();
    YesNoDialog->disconnect(this);

    if(Disinfection_No_completed==false)
    {
        if(true == selfcheck_enable){
            MasterCtrol_CMD_Checkself();
        }
        else{
            MasterCtrol_CMD_Pass_Checkself();
        }

        dataNodeUpdate(SelfCheck_Timer,&selfcheck_enable);

        usleep(1000);

        enginerDialog->getversion();

//      MasterCtrol_CMD_Checkself(); //开启主控板，并发送自检SDO#/

    }
    else if(Disinfection_No_completed==true)
    {
        enginerDialog->getversion();

        WorkState  cleanState;
        cleanState=GetWorkStateManage();
        cleanState.tipState.current= CLEANING;
        cleanState.subState.current=CLEANING_STOP;
        cleanState.disinfectEnable = true;
        SetWorkStateManage(&cleanState);
    }

    alarmmodule->Reset_AlarmHandle();//复位报警操作#/

    Message mess;
    memset(&mess,0,sizeof(mess));
    mess.cob_id = 0x183;
    mess.data[0] = 0;
    mess.data[1] = 1;//只是为了使能isShowMain#/
    MessageSignalMiddle(&mess);//模拟接收到主控板的PDO，就是为了设置DataCan的isShowMain#/


    //超净滤器次数检测#/
    FilterTimes filterInfo;
    filterInfo=DataSql_GetFilterTimes(1);
    FilterTimes filterInfo2;
    filterInfo2=DataSql_GetFilterTimes(2);
    if(filterInfo.remainTime<=3||filterInfo2.remainTime<=3)
    {
        showYesDialog(DE_TR_PROMPT,DE_TR_SUPERCLEANTIMESLESS3);
    }
    else if(filterInfo.remainTime==0||filterInfo2.remainTime==0)
    {
        showYesDialog(DE_TR_PROMPT,DE_TR_SUPERCLEANTIMESIS0);
    }
}

//跳过自检按钮--------------------process A#/
void WindowBoot::PassCheck(void)
{
    connect(YesNoDialog,SIGNAL(NoteSelSignal(int)),this,SLOT(ConfirmSelect(int)));
    showYesNoDialog(DE_TR_PLEASECONFIRM,DE_TR_PASSCHECK);
    timer->stopTimer(this);
}

void WindowBoot::DebugMode_Enter(void)
{
    connect(YesNoDialog,SIGNAL(NoteSelSignal(int)),this,SLOT(ConfirmDebugMode(int)));
    showYesNoDialog(DE_TR_PLEASECONFIRM,DE_TR_WHETHERACCESSDEBUG);
    timer->stopTimer(this);
}

void WindowBoot::ConfirmDebugMode(int sel)
{
    if(sel==true)
    {
        startRecive();
        workstate.tipState.current=DEBUG;  //设置调试状态模式#/
 //       enginerDialog->showFormulaPage();
        PageShow("Widget DebugMode");
    }
    else
    {
        timer->createTimer(this, 50);
    }
    YesNoDialog->disconnect(this);
}

void WindowBoot::ConfirmSelect(int sel)
{
    if(sel==true)
    {
        selfcheck_enable=false;//跳过自检#/
        startDelay=true;

        PageShow("Main");

        WorkState stateTMP = GetWorkStateManage();
        stateTMP.tipState.current = TREAT_READY;
        stateTMP.subState.current = SUB_STATE_NULL;
        stateTMP.disinfectEnable = true;//跳过自检可以进入消毒#/
        SetWorkStateManage(&stateTMP);//设置为准备状态下的自检完成#/
    }
    timer->createTimer(this, 50);
    YesNoDialog->disconnect(this);
}

void WindowBoot::paintEvent(QPaintEvent *event)
{
    event = event;
    int num;
    QPainter painter(this);

    painter.translate(BOOT_RING_CENTRE_X, BOOT_RING_CENTRE_Y);
    painter.save();
    painter.drawPixmap(50, -3, pixmapShaft);
    painter.rotate(90);
    painter.drawPixmap(50, -3, pixmapShaft);
    painter.rotate(90);
    painter.drawPixmap(50, -3, pixmapShaft);
    painter.rotate(90);
    painter.drawPixmap(50, -3, pixmapShaft);
    painter.restore();

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor(145, 145, 145), 1, Qt::SolidLine));
    painter.drawEllipse(-62, -62, 124, 124);
    painter.drawEllipse(-73, -73, 146, 146);
    painter.setPen(QPen(QColor(145, 145, 145), 11, Qt::SolidLine));
    painter.drawArc(-68, -68, 136, 136, -270*16, 360*16*timeNow/(float)timeCount);

    painter.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CONFIG_FONT_POINTSIZE_BIG, CONFIG_FONT_WEIGHT));
    painter.setPen(QPen(QColor(112, 210, 255)));
    if (timeNow%1000 >0) {
        num = timeNow/1000 +1;
    } else {
        num = timeNow/1000;
    }
    painter.drawText(QRect(-30, -36, 60, 72), Qt::AlignCenter,
            QString::number(num, 10));
}

void WindowBoot::CustomSetText()
{
    label.setText(tr("Enter Self-test menu..."));
    widgetButton.setText(tr("Skip"));
    ButtonDebugEnter.setText(tr("Debug mode"));
}

void WindowBoot::setCusVisible(bool vis, char* para)
{
    para = para;

    setVisible(vis);

    if(Disinfection_No_completed==true)
    {
        widgetButton.setVisible(false);
    }
}

