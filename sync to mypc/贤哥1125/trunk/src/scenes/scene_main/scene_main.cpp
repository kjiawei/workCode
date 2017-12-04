#include <QtGui>
#include "scene_main.h"
#include "global_var.h"
#include "data_timer.h"
#include "common_tr_string.h"

SceneMain::SceneMain(QWidget *parent) :
    QObject(parent),
    widgetMeasureAP(parent),
    widgetMeasureVP(parent),
    widgetMeasureTMP(parent),
    widgetMeasureCond(parent),
    widgetAimConfig(parent),
    widgetSwitch(parent),
    widgetCheckSelf(parent),
    widgetPButtontMarkAP(parent),
    widgetPButtontMarkVP(parent),
    widgetPButtontMarkTMP(parent),
    widgetPButtontMarkCond(parent),
    topLevelLabel(parent)
{
    //wSelfcheck = new WidgetCheckSelf();
    //connect(wSelfcheck,SIGNAL(SelfCheckFinish(bool)),
                           //this,SLOT(SelfCheckFinish(bool))); //消息发送与接收#/

    topLevelLabel.setGeometry(QRect(779, 656, 70, 98));
    topLevelLabel.setPixmap(QPixmap(":/resource/images/additional.png"));

    widgetMeasureAP.setGeometry(QRect(40, 210,
                _WidgetMeasure_WIDTH, _WidgetMeasure_HIGH));

    //自检进度条#/
    widgetCheckSelf.setGeometry(QRect(140, 310,
                _WidgetMeasure_WIDTH+300, _WidgetMeasure_HIGH-200));

    widgetMeasureVP.setGeometry(QRect(220, 210,
                _WidgetMeasure_WIDTH, _WidgetMeasure_HIGH));

    widgetMeasureTMP.setGeometry(QRect(400, 210,
                _WidgetMeasure_WIDTH, _WidgetMeasure_HIGH));

    widgetMeasureCond.setGeometry(QRect(580, 210,
                _WidgetMeasure_WIDTH, _WidgetMeasure_HIGH));

    widgetPButtontMarkAP.setGeometry(QRect(BUTONMARK_X, 210,
                107, 42));
    widgetPButtontMarkAP.setTextMark(QObject::tr("A.P."));

    widgetPButtontMarkVP.setGeometry(QRect(BUTONMARK_X+180, 210,
                107, 42));
    widgetPButtontMarkVP.setTextMark(QObject::tr("V.P."));

    widgetPButtontMarkTMP.setGeometry(QRect(BUTONMARK_X+360, 210,
                107, 42));
    widgetPButtontMarkTMP.setTextMark(QObject::tr("TMP"));

    widgetPButtontMarkCond.setGeometry(QRect(BUTONMARK_X+540, 210,
                107, 42));
    widgetPButtontMarkCond.setTextMark(QObject::tr("Cond."));
    //
    widgetAimConfig.setGeometry(QRect(14, 14,
                WIDGETAIMCONFIG_WIDTH, WIDGETAIMCONFIG_HIGH));

    widgetSwitch.setGeometry(QRect(14, 685,
                WIDGETSWITCH_WIDTH, WIDGETSWITCH_HIGH));

    dataNodeAddObject(PACKET_BLOODROAD_OK, this);
    dataNodeAddObject(PACKET_WATERROAD_OK, this);
    dataNodeAddObject(PACKET_STATUS_NOTE,this);
    dataNodeAddObject(SelfCheck_Timer, this);

    checkfailinfo=0;

    PageAppendPiece("Main", this);
    CustomSetText();

    connect(&widgetSwitch, SIGNAL(RstLockSig(int)),&widgetMeasureAP, SLOT(ResetLock(int)));
    connect(&widgetSwitch, SIGNAL(RstLockSig(int)),&widgetMeasureVP, SLOT(ResetLock(int)));
    connect(&widgetSwitch, SIGNAL(RstLockSig(int)),&widgetMeasureTMP, SLOT(ResetLock(int)));
    //widgetMeasureAP.ResetLock(2);    
}

void SceneMain::CustomSetText()
{
    widgetPButtontMarkAP.setTextMark(QObject::tr("A.P."));
    widgetPButtontMarkVP.setTextMark(QObject::tr("V.P."));
    widgetPButtontMarkTMP.setTextMark(QObject::tr("TMP"));
    widgetPButtontMarkCond.setTextMark(QObject::tr("Cond."));
}

void SceneMain::setCusVisible(bool vis, char* para)
{
    para = para;
    if(!selfcheck_enable)   //不进行自检时#/
    {
        widgetMeasureAP.setVisible(vis);
        widgetMeasureVP.setVisible(vis);
        widgetMeasureTMP.setVisible(vis);
        widgetMeasureCond.setVisible(vis);

        widgetAimConfig.setVisible(vis);
        widgetSwitch.setVisible(vis);

        widgetCheckSelf.setVisible(false);
        //widgetCheckSelf.show();

        widgetPButtontMarkAP.setVisible(vis);
        widgetPButtontMarkVP.setVisible(vis);
        widgetPButtontMarkTMP.setVisible(vis);
        widgetPButtontMarkCond.setVisible(vis);
        topLevelLabel.setVisible(vis);
    }
    else if(selfcheck_enable)
    {
        widgetMeasureAP.setVisible(false);
        widgetMeasureVP.setVisible(false);
        widgetMeasureTMP.setVisible(false);
        widgetMeasureCond.setVisible(false);//

        widgetAimConfig.setVisible(vis);
        widgetSwitch.setVisible(vis);

        widgetCheckSelf.setVisible(vis);//

        widgetPButtontMarkAP.setVisible(vis);
        widgetPButtontMarkVP.setVisible(vis);
        widgetPButtontMarkTMP.setVisible(vis);
        widgetPButtontMarkCond.setVisible(vis);
        topLevelLabel.setVisible(vis);
    }
}
void SceneMain::SelfCheckFinish(bool flag)
{
    setCusVisible(flag);
}

void SceneMain::timer_handler(const int timeOut)
{
#if DEBUG_TIMER_HANDLE
    Debug_insert("SceneMain-timer-in");
#endif
    Q_UNUSED(timeOut);
    widgetCheckSelf.progressSelfcheck.setValue(timeCount++);//
    widgetCheckSelf.progressSelfcheck.update();

    if(timeCount > 2400)   //自检超时失败  40min#/
    {
        if(isSelfcheck==true)
        {
            showYesDialog(DE_TR_PROMPT,DE_TR_SELFCHECKTIMEOUT);
            widgetCheckSelf.labelTitle.setText(DE_TR_SELFCHECKTIMEOUT);
        }
        timer->stopTimer(this);
    }
    else if(checkfailinfo != 0)  //自检出错#/
    {
        showYesDialog(DE_TR_PROMPT,DE_TR_SELFCHECKERROR);
        widgetCheckSelf.labelTitle.setText(DE_TR_SELFCHECKERROR);
        timer->stopTimer(this);
    }
    else if(checkfailinfo==0&&bloodroadOk && waterroadOk && selfcheck_enable)  //成功#/
    {
        timeCount=0;
        bloodroadOk=false;
        waterroadOk=false;
        widgetCheckSelf.progressSelfcheck.hide();
        widgetCheckSelf.labelTitle.hide();
        timer->stopTimer(this);
        selfcheck_enable=false;     //自检完成后停止#/
        isSelfcheck=false;
        if (strcmp(PageCurrent(), "Main") == 0)
            SelfCheckFinish(true);
    }
    else if(selfcheck_enable==false)
    {
        timer->stopTimer(this);
    }
#if DEBUG_TIMER_HANDLE
    Debug_insert("SceneMain-timer-out");
#endif
}

void SceneMain::StartSelfCheckTimer(bool check)
{
    checkfailinfo=0;
    if(check==true){
    timeCount=0;
    timer->createTimer(this, 1000);
    }
}

void SceneMain::ActionHandle(const void* dataPtr, int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("scene_main-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name == 0)) {
        return;
    }

    if (name==SelfCheck_Timer){  //接收倒计时页面的自检使能信号#/
        isSelfcheck = *((int*) dataPtr);
        StartSelfCheckTimer(isSelfcheck);   //开始自检计时#/
        checkfailinfo = 0;
    }

    if (name==PACKET_BLOODROAD_OK){  //血路OK标志#/
        bloodroadOk = *((int*) dataPtr);
    }

    if (name==PACKET_WATERROAD_OK){  //水路OK标志#/
        waterroadOk = *((int*) dataPtr);
    }    

    if ((name==PACKET_STATUS_NOTE)&&(bloodroadOk==0||waterroadOk==0))
    {  //获取主控板的PDO0，判断是否自检失败#/
        if(((PacketStatusNote*)dataPtr)->note.note4&0x02)  //血路自检失败#/
            checkfailinfo=1;
        else if(((PacketStatusNote*)dataPtr)->note.note4&0x80)
            checkfailinfo=2;
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("scene_main-ActionHandle-out");
#endif
}
