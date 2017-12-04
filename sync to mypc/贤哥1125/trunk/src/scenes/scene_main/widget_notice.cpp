#include <QtGui>
#include "config_ui.h"
#include "widget_notice.h"
#include "deal.h"
#include "data_sqllite.h"

#include "global_var.h"
#include "sound_adjust.h"
#include "common_tr_string.h"

#define FULL_INFORMATION_SWITCH  1   //全状态信息打开开关  1,开  0，关#/

inline void WidgetNotice::setUi()
{
    widgetBattery.setGeometry(QRect(905, 62, 19, 33));
    /*
    buttonSound.setStyleSwitch(":/resource/images/sound.png",
            ":/resource/images/sound_close.png", 0xffdbe6ef, QRect(955, 63, 31, 31));
            */
//    buttonSound.setGeometry(955,63,51,51);
    buttonSound.setStyleSwitch(":/resource/images/sound.png",
            ":/resource/images/sound_close.png", 0xffdbe6ef, QRect(945, 48, 60, 60));

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/resource/images/battery.png")));
    widgetBattery.setAutoFillBackground(true);
    widgetBattery.setPalette(palette);

    progressSound.setGeometry(QRect(990, 62, 13, 40));
    progressSound.setOrientation(Qt::Vertical);
    progressSound.setTextVisible(false);
    progressSound.hide();

    labelHeaderStatus.adjustSize();
    labelHeaderStatus.setGeometry(QRect(876, 108, 135, 40));
    labelHeaderStatus.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelHeaderStatus.setWordWrap(true);
    labelHeaderStatus.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_FIRST_3, CONFIG_FONT_NORMAL));
    labelHeaderStatus.setStyleSheet(CONFIG_COLOR_BLUE_CHARS);

    labelBloodStatus.adjustSize();
    labelBloodStatus.setGeometry(QRect(876, 148, 135, 40));
    labelBloodStatus.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelBloodStatus.setWordWrap(true);
    labelBloodStatus.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_TEXT, CONFIG_FONT_NORMAL));
    labelBloodStatus.setStyleSheet(CONFIG_COLOR_BLUE_CHARS);

    labelWaterStatus.adjustSize();
    labelWaterStatus.setGeometry(QRect(876, 178, 135, 40));
    labelWaterStatus.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelWaterStatus.setWordWrap(true);
    labelWaterStatus.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_FIRST_3, CONFIG_FONT_NORMAL));
    labelWaterStatus.setStyleSheet(CONFIG_COLOR_BLUE_CHARS);
}

WidgetNotice::WidgetNotice(QWidget *parent) :
    QObject(parent),
    widgetBattery(parent),
    buttonSound(parent),
    progressSound(parent),
    labelHeaderStatus(parent),
    labelBloodStatus(parent),
    labelWaterStatus(parent)
{
    setUi();

    dataNodeAddObject(PACKET_STATUS_NOTE, this);
    dataNodeAddObject(PACKET_NAME_WORKSTATE, this);
    dataNodeAddObject(Preflush_Model, this);
    dataNodeAddObject(ALARM_SILENCE_SET,this);

    connect(&buttonSound, SIGNAL(clicked()), this, SLOT(slotSoundSwitch()));
//    connect(&timerDown, SIGNAL(timeout()), this, SLOT(slotTimerOut()));
}

void WidgetNotice::setHeaderStatus(int code)
{
    switch (code) {
    case 0:
        strHeaderStatus=tr("");
        break;
    case 1:
        strHeaderStatus=tr("Ready St");
        break;
    case 2:
        strHeaderStatus=tr("Treatment");
        break;
    case 3:
        strHeaderStatus=tr("Disconnection");
        break;
    case 4:
        strHeaderStatus=tr("Cleaning");
        break;
    case 5:
        strHeaderStatus=tr("Debug");
        break;
    case 0x0A:
        strHeaderStatus=tr("Drain");
        break;

    case 0x0f:
        strHeaderStatus=tr("Water Stop");
        break;
    default:
        break;
    }

    labelHeaderStatus.setText(strHeaderStatus);
}

//显示当前主状态#/
void WidgetNotice::showtipState(int code)
{
    switch(code){
    case START_INIT: //启动初始化#/
        strHeaderStatus=DE_TR_START_INIT;
        break;

    case TREAT_READY:  //准备状态#/
        strHeaderStatus=DE_TR_PREPARESTATE;
        break;

    case TREAT_RUNNING: //治疗状态#/
        strHeaderStatus=DE_TR_TREATMENTSTATE;
        break;

    case TREAT_FINISH: //治疗结束状态#/
        strHeaderStatus=DE_TR_FINISHTREATMENTSTATE;
        break;

    case CLEANING:   //清洁状态#/
        strHeaderStatus=DE_TR_CLEANINGSTATE;
        break;

    case DEBUG:
        strHeaderStatus=DE_TR_DEBUGSTATE;
        break;

    case DRAIN:
        strHeaderStatus=DE_TR_DRAINSTATE;
        break;
    }
    labelHeaderStatus.setText(strHeaderStatus);
}

void WidgetNotice::setBloodStatus(int code)
{
    switch (code) {
    case 0:
        strBloodStatus=tr("");
        break;
    case 0x11:
        strBloodStatus=tr("Ready");
        break;
    case 0x12:
        strBloodStatus=tr("BM_test");
        break;
    case 0x13:
        strBloodStatus=tr("BM test OK");
        break;
    case 0x14:
        strBloodStatus=tr("Prime");
        break;
    case 0x15:
        strBloodStatus=tr("Dialyze ready");
        break;
    case 0x21:
        strBloodStatus=tr("HD DoubleNeedle");
        break;
    case 0x22:
        strBloodStatus=tr("ISO DoubleNeedle");
        break;
    case 0x23:
        strBloodStatus=tr("HDFonline");
        break;
    case 0x24:
        strBloodStatus=tr("HFonline");
        break;
    case 0x25:
        strBloodStatus=tr("PE/PH");
        break;
    case 0x26:
        strBloodStatus=tr("PE/PH");
        break;
    case 0x27:
        strBloodStatus=tr("HD SNSP");
        break;
    case 0x28:
        strBloodStatus=tr("HD SNDP");
        break;
    case 0x29:
        strBloodStatus=tr("ISO SNSP");
        break;
    case 0x2a:
        strBloodStatus=tr("ISO SNDP");
        break;
    case COOL_RINSE://冷冲洗#/
        strBloodStatus=DE_TR_COOL_RINSE;
        break;
    case HOT_RINSE://热冲洗#/
        strBloodStatus=DE_TR_HOT_RINSE;
        break;
    case CHEMICAL_CITRICACID://化消柠檬酸#/
        strBloodStatus=DE_TR_CHEMICAL_CITRICACID;
        break;
    case CHEMICAL_PERACETIC://化消过氧乙酸#/
        strBloodStatus=DE_TR_CHEMICAL_PERACETIC;
        break;
    case CHEMICAL3://化学消毒3#/
        strBloodStatus=DE_TR_CHEMICAL3;
        break;
    case HOT_CHEMICAL://热化学消毒#/
        strBloodStatus=DE_TR_HOT_CHEMICAL;
        break;
    case HOT: //纯热消毒#/
        strBloodStatus=DE_TR_HOT;
        break;

    default:
        break;
    }

    labelBloodStatus.setText(strBloodStatus);
}

void WidgetNotice::setWaterStatus(int code)
{
    switch (code) {
    case 0:
//        strWaterStatus=tr("NULL");
        strWaterStatus=tr(" ");
        break;
    case 0x11:
        strWaterStatus=DE_TR_WATERTOPREPARE;
        break;
    case 0x12:
        strWaterStatus=DE_TR_DRYPREFLUSH;
        break;
    case 0x13:
        strWaterStatus=DE_TR_DMTEST;
        break;
    case 0x14:
        strWaterStatus=DE_TR_DMTESTOK;
        break;
    case 0x15:
        strWaterStatus=DE_TR_PROVINCIALFLUID;
        break;
    case 0x16:
        strWaterStatus=DE_TR_WATERSUSPENSION;
        break;
    case 0x21:
        strWaterStatus=tr("ready");
        break;
    case 0x22:
        strWaterStatus=tr("run");
        break;
    case 0x23:
        strWaterStatus=DE_TR_PROVINCIALFLUIDBYPASS;
        break;
    case 0x24:
        strWaterStatus=DE_TR_STOPFLUID;
        break;
    case 0x25:
        strWaterStatus=DE_TR_BYPASS;
        break;
    case 0x40:
        strWaterStatus=DE_TR_DISINFECTIONPREPARE;
        break;
    case 0x41:
        strWaterStatus=DE_TR_DISINFECTIONPREFLUSH;
        break;
    case 0x42:
        strWaterStatus=DE_TR_SUCKFLUSH;
        break;
    case 0x43:
        strWaterStatus=DE_TR_HOLD;
        break;
    case 0x44:
        strWaterStatus=DE_TR_LOOP;
        break;
    case 0x45:
        strWaterStatus=DE_TR_AFTERRINSE;
        break;
    case 0x46:
        strWaterStatus=DE_TR_COLDRINSE;
        break;
    case 0x47:
        strWaterStatus=DE_TR_HOTRINSE1;
        break;
    case 0x48:
        strWaterStatus=DE_TR_HOTRINSE2;
        break;
    case 0x49:
        strWaterStatus=DE_TR_HOTRINSE3;
        break;
    case 0x4a:
        strWaterStatus=DE_TR_EMPTYING;
        break;
    case 0x4b:
        strWaterStatus=DE_TR_PIPERINSE;
        break;
    case 0x4c:
        strWaterStatus=DE_TR_CCDSUCK;
        break;
    case 0x4d:
        strWaterStatus=DE_TR_CCDRINSE;
        break;
    case 0x4e:
        strWaterStatus=DE_TR_CHD;
        break;
    default:
        break;
    }
    labelWaterStatus.setText(strWaterStatus);
}

//显示当前子状态
void WidgetNotice::showsubState(int code)
{
    switch(code){
    case SUB_STATE_NULL:  //子状态为空，如上电时
        strBloodStatus=DE_TR_SUB_STATE_NULL;
        break;

    case SELFTEST:
        strBloodStatus=DE_TR_SELFTEST;
        break;

    case SELFTEST_COMPLETE_FLUID:
        strBloodStatus=DE_TR_SELFTEST_COMPLETE_FLUID;
        break;

    case SELFTEST_COMPLETE_NO_FLUID:
        strBloodStatus=DE_TR_SELFTEST_COMPLETE_NO_FLUID;
        break;

    case PREFLUSH_RUN_FLUID:
        switch(Startpreflush_mode)
        {
        case AUTO_PREFLUSH_MODE:
            strBloodStatus=DE_TR_AUTOPREFLUSH_RUN_FLUID;
            break;
        case MANUAL_PREFLUSH_MODE:
            strBloodStatus=DE_TR_MANUALPREFLUSH_RUN_FLUID;
            break;
        case ONLINE_PREFLUSH_MODE:
            strBloodStatus=DE_TR_ONLINEPREFLUSH_RUN_FLUID;
            break;
        }
//        strBloodStatus=DE_TR_PREFLUSH_RUN_FLUID;
        break;

    case PREFLUSH_RUN_NO_FLUID:
        switch(Startpreflush_mode)
        {
        case AUTO_PREFLUSH_MODE:
            strBloodStatus=DE_TR_AUTOPREFLUSH_RUN_NO_FLUID;
            break;
        case MANUAL_PREFLUSH_MODE:
            strBloodStatus=DE_TR_MANUALPREFLUSH_RUN_NO_FLUID;
            break;
        case ONLINE_PREFLUSH_MODE:
            strBloodStatus=DE_TR_ONLINEPREFLUSH_RUN_NO_FLUID;
            break;
        }
//        strBloodStatus=DE_TR_PREFLUSH_RUN_NO_FLUID;
        break;

    case PREFLUSH_STOP_FLUID:
        strBloodStatus=DE_TR_PREFLUSH_STOP_FLUID;
        break;

    case PREFLUSH_STOP_NO_FLUID:
        strBloodStatus=DE_TR_PREFLUSH_STOP_NO_FLUID;
        break;

    case PREFLUSH_COMPLETE_FLUID:
        strBloodStatus=DE_TR_PREFLUSH_COMPLETE_FLUID;
        break;

    case PREFLUSH_COMPLETE_NO_FLUID:
        strBloodStatus=DE_TR_PREFLUSH_COMPLETE_NO_FLUID;
        break;

    case TREAT_ENABLE_FLUID:
        strBloodStatus=DE_TR_TREAT_ENABLE_FLUID;
        break;

    case TREAT_ENABLE_NO_FLUID:
        strBloodStatus=DE_TR_TREAT_ENABLE_NO_FLUID;
        break;

    case GETBLOOD_RUN_NO_FLUID:     //引血 无液
        if(Startpreflush_mode==ONLINE_GETBLOOD_MODE)
            strBloodStatus=DE_TR_ONLINEGETBLOOD_RUN_NO_FLUID;
        else
            strBloodStatus=DE_TR_GETBLOOD_RUN_NO_FLUID;
//        strBloodStatus=DE_TR_GETBLOOD_RUN_NO_FLUID;
        break;
    case GETBLOOD_RUN_FLUID: //引血 有液
        if(Startpreflush_mode==ONLINE_GETBLOOD_MODE)
            strBloodStatus=DE_TR_ONLINEGETBLOOD_RUN_FLUID;
        else
            strBloodStatus=DE_TR_GETBLOOD_RUN_FLUID;
//        strBloodStatus=DE_TR_GETBLOOD_RUN_FLUID;
        break;
    case GETBLOOD_STOP_NO_FLUID:   //引血暂停 无液
        strBloodStatus=DE_TR_GETBLOOD_STOP_NO_FLUID;
        break;
    case GETBLOOD_STOP_FLUID: //引血暂停 有液
        strBloodStatus=DE_TR_GETBLOOD_STOP_FLUID;
        break;

    case CLEANING_RUN:
        strBloodStatus=DE_TR_CLEANING_RUN;
        break;

    case CLEANING_STOP:
        strBloodStatus=DE_TR_CLEANING_STOP;
        break;

    case CLEANING_FINISHED:
        strBloodStatus=DE_TR_CLEANING_FINISHED;
        break;

    case CURE_BLOODRETURN_RUN:
        if(Startreturnblood_mode==ONLINE_RETURNBLOOD_MODE)
            strBloodStatus=DE_TR_CURE_ONLINEBLOODRETURN_RUN;
        else
            strBloodStatus=DE_TR_CURE_BLOODRETURN_RUN;

//        strBloodStatus=DE_TR_CURE_BLOODRETURN_RUN;
        break;

    case CURE_BLOODRETURN_RUN_NO_BLOOD:
        if(Startreturnblood_mode==ONLINE_RETURNBLOOD_MODE)
            strBloodStatus=DE_TR_CURE_ONLINEBLOODRETURN_RUN_NO_BLOOD;
        else
            strBloodStatus=DE_TR_CURE_BLOODRETURN_RUN_NO_BLOOD;
//        strBloodStatus=DE_TR_CURE_BLOODRETURN_RUN_NO_BLOOD;
        break;

    case CURE_BLOODRETURN_STOP:
        strBloodStatus=DE_TR_CURE_BLOODRETURN_STOP;
        break;

    case CURE_BLOODRETURN_STOP_NO_BLOOD:
        strBloodStatus=DE_TR_CURE_BLOODRETURN_STOP_NO_BLOOD;
        break;

    case DRAIN_RUN:
        strBloodStatus=DE_TR_DRAIN_RUN;
        break;

    case DRAIN_STOP:
        strBloodStatus=DE_TR_DRAIN_STOP;
        break;

    case CURE_BLOODRETURN_RUN_DRAIN:
        strBloodStatus=DE_TR_CURE_BLOODRETURN_RUN;
        break;

    case CURE_BLOODRETURN_RUN_DRAIN_NO_BLOOD:
        strBloodStatus=DE_TR_CURE_BLOODRETURN_RUN_NO_BLOOD;
        break;

    case CURE_BLOODRETURN_STOP_DRAIN:
        strBloodStatus=DE_TR_CURE_BLOODRETURN_STOP;
        break;

    case CURE_BLOODRETURN_STOP_DRAIN_NO_BLOOD:
        strBloodStatus=DE_TR_CURE_BLOODRETURN_STOP_NO_BLOOD;
        break;
    }
    labelHeaderStatus.setText(strBloodStatus);
}

void WidgetNotice::slotSoundSwitch(void)
{
    if (buttonSound.isChecked() == true) {
        buttonSound.setDisabled(true);
        timeCount=2*60;//7
        progressSound.setMaximum(timeCount);
        progressSound.setMinimum(0);
        progressSound.setValue(timeCount);
        progressSound.show();
        timer->createTimer(this, 1000);
        audio->alarmSilence(true);

        curAlarmSave=alarmmodule->Get_curtimeAlarmMap();//保存当前需消音的报警#/
    }
}

void WidgetNotice::timer_handler(const int timeOut)
{
    Q_UNUSED(timeOut);
#if DEBUG_TIMER_HANDLE
    Debug_insert("WidgetNotice-timer-in");
#endif

    QMap<int,AlarmInfor> curAlarmTmp;
    QMap<int,AlarmInfor>::iterator it;

    progressSound.setValue(--timeCount);
    progressSound.update();  //更新显示#/
    if (timeCount == 0) {
        //SoundAdjustMode(2);
        CancelAlarmSilence();  //取消消音操作#/
    }

    if(curAlarmSave.size()==0 && alarmmodule->Get_CurrentAlarmNum()>0 && audio->alarmSilenceState()==true)  //有报警来，取消消音#/
    {
        CancelAlarmSilence();  //取消消音操作#/
    }
    else if(curAlarmSave.size()>0 && alarmmodule->Get_CurrentAlarmNum()>0 && audio->alarmSilenceState()==true)
    {
        curAlarmTmp=alarmmodule->Get_curtimeAlarmMap();

        if(curAlarmTmp.size()>curAlarmSave.size())  //有新报警来 取消消音#/
        {
            CancelAlarmSilence();  //取消消音操作#/
        }
        else
        {
            for(it=curAlarmTmp.begin();it != curAlarmTmp.end();it++)
            {
                if(curAlarmSave.contains(it.key())==false)  //有新的报警，取消消音#/
                {
                    CancelAlarmSilence();  //取消消音操作#/
                    break;
                }
            }
        }
    }


#if DEBUG_TIMER_HANDLE
    Debug_insert("WidgetNotice-timer-out");
#endif
}

void WidgetNotice::CancelAlarmSilence()  //取消消音操作#/
{
    progressSound.hide();
    buttonSound.setDisabled(false);
    buttonSound.setChecked(false);
    timer->stopTimer(this);
    audio->alarmSilence(false);
    curAlarmSave.clear();
}

void WidgetNotice::ActionHandle(const void* dataPtr,int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetNotice-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name == 0)) {
        return;
    }

    if (name==PACKET_NAME_WORKSTATE){  //工作状态改变时#/
       workstate  = *(WorkState *) dataPtr;

       showtipState(workstate.tipState.current);   //显示当前主状态#/
#if FULL_INFORMATION_SWITCH
       if(workstate.tipState.current!=TREAT_RUNNING&&workstate.tipState.current<=TREAT_FINISH)  //不是治疗的时候，则显示子状态#/
           showsubState(workstate.subState.current);
       else if(workstate.tipState.current==CLEANING)
           setBloodStatus(workstate.bloodState.current);
#endif
       return;
    }

    if (name==Preflush_Model){
        preflush_model = *((PreflushModel*) dataPtr);
        return;
    }

    //根据主控板的状态PDO，显示信息#/
    if(name==PACKET_STATUS_NOTE){
        PacketStatusNote *p = (PacketStatusNote *) dataPtr;
        iHeaderStatus = p->status.header_status;
        iBloodStatus = p->status.blood_status;
        iWaterStatus = p->status.water_status;

        struct WorkState workstate;
        workstate=GetWorkStateManage();
#if FULL_INFORMATION_SWITCH
        if(workstate.tipState.current==TREAT_RUNNING)
        {
            if(G_NA_PROFILE_FLAG==true||G_UF_PROFILE_FLAG==true||G_DIA_PROFILE_FLAG==true\
               ||G_HEP_PROFILE_FLAG==true||G_COND_PROFILE_FLAG==true||G_TEMP_PROFILE_FLAG==true||G_HCO3_PROFILE_FLAG==true)   //曲线信息#/
            {
                if(G_NA_PROFILE_FLAG&&G_UF_PROFILE_FLAG)
                {
                    labelBloodStatus.setText(DE_TR_NAANDUFPROFILE);
                }
                else if(G_NA_PROFILE_FLAG)
                {
                    labelBloodStatus.setText(DE_TR_NAPROFILE);
                }
                else if(G_UF_PROFILE_FLAG)
                {
                    labelBloodStatus.setText(DE_TR_UFPROFILE);
                }
                else if(G_DIA_PROFILE_FLAG)
                {
                    labelBloodStatus.setText(DE_TR_DIAFLOWPROFILE);
                }
                else if(G_HEP_PROFILE_FLAG)
                {
                    labelBloodStatus.setText(DE_TR_HEPARINPROFILEE);
                }
                else if(G_COND_PROFILE_FLAG)
                {
                    labelBloodStatus.setText(DE_TR_CONDPROFILE);
                }
                else if(G_TEMP_PROFILE_FLAG)
                {
                    labelBloodStatus.setText(DE_TR_TEMPPROFILE);
                }
                else if(G_HCO3_PROFILE_FLAG)
                {
                    labelBloodStatus.setText(DE_TR_HCO3PROFILE);
                }
            }
            else
            setBloodStatus(iBloodStatus);   //血路信息#/
        }
        else if(workstate.tipState.current<=TREAT_FINISH)
            setBloodStatus(0);

        setWaterStatus(iWaterStatus);    //水路信息#/
#else
        if(workstate.tipState.current==TREAT_RUNNING&&(G_NA_PROFILE_FLAG==true||G_UF_PROFILE_FLAG==true))
        {
            if(G_NA_PROFILE_FLAG&&G_UF_PROFILE_FLAG)
            {
                labelBloodStatus.setText(DE_TR_NAANDUFPROFILE);
            }
            else if(G_NA_PROFILE_FLAG)
            {
                labelBloodStatus.setText(DE_TR_NAPROFILE);
            }
            else if(G_UF_PROFILE_FLAG)
            {
                labelBloodStatus.setText(DE_TR_UFPROFILE);
            }
        }
        else
        {
            switch(iBloodStatus)
            {
                case 0x12:
                    labelBloodStatus.setText(tr("Self testing"));
                break;
                case 0x21:
                    labelBloodStatus.setText(tr("HD"));
                    break;
                case 0x22:
                    labelBloodStatus.setText(tr("ISO"));
                    break;
                case 0x23:
                    labelBloodStatus.setText(tr("HDFonline"));
                    break;
                case 0x24:
                    labelBloodStatus.setText(tr("HFonline"));
                    break;
                case 0x25:
                    labelBloodStatus.setText(tr("PE/PH"));
                    break;
                case 0x26:
                    labelBloodStatus.setText(tr("PE/PH"));
                    break;
                case 0x27:
                    labelBloodStatus.setText(tr("HD SNSP"));
                    break;
                case 0x28:
                    labelBloodStatus.setText(tr("HD SNDP"));
                    break;
                case 0x29:
                    labelBloodStatus.setText(tr("ISO SNSP"));
                    break;
                case 0x2a:
                    labelBloodStatus.setText(tr("ISO SNDP"));
                    break;

               default:
                    if(PDOstate.state.N1_sub_status>=0x15&&PDOstate.state.N1_sub_status<=0x1c)
                    {
                        labelBloodStatus.setText(tr("Priming"));
                    }
                    else
                    {
                        labelBloodStatus.setText(tr("   "));
                    }
                    break;
            }
        }


        switch(iWaterStatus)
        {
            case 0x24:
                labelWaterStatus.setText(tr("Stop"));
                break;
            case 0x25:
                labelWaterStatus.setText(tr("Bypass"));
                break;
            default:
                labelWaterStatus.setText(tr("  "));
                break;
        }
#endif
        /*
        //假设我收到的数据，是总状态是0就退出，并重新发要求下面自检的#/
        if (iHeaderStatus == 0)
        {
            setHeaderStatus(0);
            setBloodStatus(0);
            setWaterStatus(0);
            return ;
        }

        setHeaderStatus(iHeaderStatus);
        if (iHeaderStatus == 0x04 || iHeaderStatus == 0x0f)//清洁时不显示血路状态
        {
            setBloodStatus(iBloodStatus);
        }
        else
        {
            setBloodStatus(iBloodStatus);
        }

        if(iHeaderStatus == 0x01 && iBloodStatus==0x13) // 自检完成
        {
            if( preflush_model==manual_model)
            labelWaterStatus.setText(tr("Prime"));
            else if(preflush_model==auto_model)
               labelWaterStatus.setText(tr("Auto Prime"));
            WorkState stateTMP = GetWorkStateManage();
            stateTMP.waterState.current = WATER_READY;
            SetWorkStateManage(&stateTMP);
            //PDOstate.state.N1_water_status=0x21;
         }
        else
        {
          setWaterStatus(iWaterStatus);
        }
        */
    }

    if(name==ALARM_SILENCE_SET){

        int val=*((int*)dataPtr);

        if(val==1)    //静音操作#/
        {
            buttonSound.setChecked(true);
            slotSoundSwitch();
        }
        else if(val==2)   //消除静音#/
        {
            CancelAlarmSilence();  //取消消音操作#/
        }
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetNotice-ActionHandle-out");
#endif
}

//
void WidgetNotice::CustomSetText()
{
    /*
    if(!strHeaderStatus.isEmpty())
     setHeaderStatus(iHeaderStatus);
    if(!strBloodStatus.isEmpty())
     setBloodStatus(iBloodStatus);
    if(!strWaterStatus.isEmpty())
     setWaterStatus(iWaterStatus);
     */
}
