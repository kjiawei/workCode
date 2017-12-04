#include "config_ui.h"
#include "widget_switch.h"
#include "deal.h"
#include "global_var.h"
#include "state.h"
#include "common_func.h"
#include "middle.h"
#include <QApplication>
#include <ktv2.h>
#include "stdlib.h"
#include "scene_setup_layout.h"
#include "Alarm_module.h"
#include "common_tr_string.h"
#include "Daemon.h"
#include "RFCard.h"


static char S_Delay_Count=2;

bool S_Clear_flag=false;     //指示自检之后，是否进行了累计量的清除，只运行一次#/

WidgetSwitch::WidgetSwitch(QWidget *parent) :
    QWidget(parent),
    widgetButtonPrime(this),
    widgetButtonTreat(this),
    widgetButtonUFPump(this),
    widgetButtonHeparinPump(this),
    widgetButtonBypass(this)
    //cureTime(parent)
{
    //预冲键样式#/
    widgetButtonPrime.setStyleDisabled(":/resource/images/switch_preshoot.png",
                                       ":/resource/images/switch_preshoot_active.png",
                                       ":/resource/images/switch_preshoot_disable.png",
                                       CONFIG_COLOR_WHITE,
                                       QRect(WIDGETSWITCH_BUTTON_PRESHOOT_X,
                                           WIDGETSWITCH_BUTTON_PRESHOOT_Y,
                                           WIDGETSWITCH_BUTTON_PRESHOOT_WIDTH,
                                           WIDGETSWITCH_HIGH));

    widgetButtonPrime.setDisabled(true);//改为false可以启动#/

    widgetButtonPrime.setTextFontPos(CFG_FONT_SIZE_FIRST_2,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_BLUE);


    widgetButtonTreat.setStyleDisabled(":/resource/images/switch_cure.png",
                                       ":/resource/images/switch_cure_active.png",
                                       ":/resource/images/switch_cure_Disable.png",
                                       CONFIG_COLOR_WHITE,
                                       QRect(WIDGETSWITCH_BUTTON_CURE_X,
                                           WIDGETSWITCH_BUTTON_CURE_Y,
                                           WIDGETSWITCH_BUTTON_CURE_WIDTH,
                                           WIDGETSWITCH_HIGH));

    widgetButtonTreat.setDisabled(true);//改为false上电可以点击治疗按钮#/

    widgetButtonTreat.setTextFontPos(CFG_FONT_SIZE_FIRST_2,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_BLUE);

    widgetButtonUFPump.setStyleDisabled(":/resource/images/switch_dialysis_pump.png",
                                       ":/resource/images/switch_dialysis_pump_active.png",
                                       ":/resource/images/switch_dialysis_pump_disable.png",
                                       CONFIG_COLOR_WHITE,
                                       QRect(WIDGETSWITCH_BUTTON_DIALYSISPUMP_X,
                                           WIDGETSWITCH_BUTTON_DIALYSISPUMP_Y,
                                           WIDGETSWITCH_BUTTON_DIALYSISPUMP_WIDTH,
                                           WIDGETSWITCH_HIGH));

    widgetButtonUFPump.setDisabled(true);

    widgetButtonUFPump.setTextFontPos(CFG_FONT_SIZE_FIRST_2,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_BLUE);

    widgetButtonHeparinPump.setStyleDisabled(":/resource/images/switch_heparin_pump.png",
            ":/resource/images/switch_heparin_pump_active.png",
                                             ":/resource/images/switch_heparin_pump_disable.png",
                                             CONFIG_COLOR_WHITE,
            QRect(WIDGETSWITCH_BUTTON_HEPARINPUMP_X,
                WIDGETSWITCH_BUTTON_HEPARINPUMP_Y,
                WIDGETSWITCH_BUTTON_HEPARINPUMP_WIDTH,
                WIDGETSWITCH_HIGH));
    widgetButtonHeparinPump.setTextFontPos(CFG_FONT_SIZE_FIRST_2,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_BLUE);
    widgetButtonHeparinPump.setDisabled(true);

    widgetButtonBypass.setStyleDisabled(":/resource/images/switch_side_road.png",
            ":/resource/images/switch_side_road_active.png",
                                        ":/resource/images/switch_side_road_disable.png",
                                        CONFIG_COLOR_WHITE,
            QRect(WIDGETSWITCH_BUTTON_SIDEROAD_X,
                WIDGETSWITCH_BUTTON_SIDEROAD_Y,
                WIDGETSWITCH_BUTTON_SIDEROAD_WIDTH,
                WIDGETSWITCH_HIGH));
    widgetButtonBypass.setTextFontPos(CFG_FONT_SIZE_FIRST_2,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_BLUE);
    widgetButtonBypass.setDisabled(true);

    CustomSetText();

    statusPreShoot = false;
    G_BPPUMP_STATUS = false;
    G_SUBPUMP_STATUS=false;  //置换泵状态#/
    G_UFPUMP_STATUS = false;
    G_HEPARINPUMP_STATUS = false;
    statusSideWay = false;   //初始化#/
    connectstatus=false;
    check_alarm=false;
    getbloodstartflag=false;
    blockbloodflag=0;
    handle_count=0;
    blood2enableflag=true;   //血泵2使能标志
    isSelfcheckstart=false;
    G_BYPASS_BUT_FLAG=DE_BYPASS_NOCHECKED;

    connect(&widgetButtonPrime, SIGNAL(clicked()), this, SLOT(ClickedPreShoot()));
    connect(&widgetButtonTreat, SIGNAL(clicked()), this, SLOT(ClickedCure()));
    connect(&widgetButtonUFPump, SIGNAL(clicked()), this, SLOT(ClickedUFPump()));
    connect(&widgetButtonHeparinPump, SIGNAL(clicked()), this, SLOT(ClickedHeparinPump()));
    connect(&widgetButtonBypass, SIGNAL(clicked()), this, SLOT(ClickedSideWay()));
#if HJX_COMMUNICATION_SWITCH
    connect(this,SIGNAL(signalSDO(int)),dataCanSend,SLOT(sendSDOData(int)),DE_CONNECTION_TYPE);
#endif

    dataNodeAddObject(PACKET_BLOODROAD_OK, this);  //血路标记#/
    dataNodeAddObject(PACKET_WATERROAD_OK, this);  //水路标记#/
    dataNodeAddObject(VEINS_HAVE_BLOOD, this);  //静脉有血标记#/
    dataNodeAddObject(BLOOD_LEVEL_OK, this);  //液位有液标记#/
    dataNodeAddObject(Preflush_Model, this);  //预冲页面设置的预冲模式标记#/
    dataNodeAddObject(PACKET_NAME_BLOODPUMP, this);  //对应血泵的PDO0#/
    dataNodeAddObject(PACKET_NAME_SUBBLOODPUMP, this);  //对应血泵2的PDO0#/
    dataNodeAddObject(PACKET_PREFLUSH, this);  //预冲页面的相关预冲参数#/
    dataNodeAddObject(PACKET_NAME_WORKMODE, this);//工作模式#/
    dataNodeAddObject(TREAT_TIMER, this);   //治疗时间的状态标志#/
    dataNodeAddObject(DE_STR_SAVECUREPARAM,this);  //治疗参数是否清空#/
    dataNodeAddObject(PACKET_NAME_WORKSTATE, this);  //工作状态 在state.h中更新#/
    dataNodeAddObject(PACKET_MASTERCTRL_UFVALUE, this);//主控板 PDO2上传的超滤量信息#/
//    dataNodeAddObject(PACKET_MAINTAIN_PARAMS, this);  //工程页面设置变化标记#/
//    dataNodeAddObject(PACKET_STATUS_NOTE, this);//对应主控板的PDO0信息#/

//    dataNodeAddObject(PACKET_RESUME_ALARM, this); //复位报警标记 可remove#/
    dataNodeAddObject(PACKET_STATUS_MASTALARM, this); //主控板报警PDO1#/
    dataNodeAddObject(OWNNOTE, this);   //有本机程序生成的提示#/
//    dataNodeAddObject(PACKET_NAME_HEPARINDATA, this);//对应肝素泵的PDO#/
    dataNodeAddObject(PACKET_NAME_HEPARINPUMP,this); //肝素泵参数改变时#/
    dataNodeAddObject(SelfCheck_Timer, this);  //自检操作处理标志#/
//    dataNodeAddObject(PACKET_REBIRTH_HANDLE,this);  //重生操作处理#/
    dataNodeAddObject(PACKET_REBIRTH_START,this); //重生启动开始#/

    preflush_model=auto_model;
    bubble =0;
    //cureTime.setCountDown(1*60*60);
    timer->createTimer(this, 500);//按钮闪烁用#/
    isFlashShow=false;
    L_workstate.current = selfcheck; //初始化为自检状态#/
    interactBp_ReturnBlood = 0;

    /*
    msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No );
    msgBox->setDefaultButton(QMessageBox::No);
    msgBox->setWindowFlags(Qt::Widget);
    msgBox->setStyleSheet("background-color:rgb(255,85,0)");//transparent white
    msgBox->setWindowFlags(Qt::FramelessWindowHint);
    msgBox->setIcon(QMessageBox::Question);//NoIcon
    msgBox->move(300,300);
//    msgBox.setStyleSheet("background-color:qlineargradient(x1:0, y1:1, x2:0, y2:0,stop:0 rgba(255, 255, 255, 100%),stop: 1 rgba(111, 111, 111, 100%));");

    QFont font;
    font.setPointSize(20);
    msgBox->setFont(font);
    */

    msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox->setDefaultButton(QMessageBox::No);
    msgBox->setIcon(QMessageBox::Question);
    msgBox->setWindowFlags(Qt::Widget);
//    msgBox->setGeometry(300,300,300,100);
    msgBox->setWindowFlags(Qt::FramelessWindowHint);
    msgBox->move(300,300);

    //接收硬件按键信号#/
     connect(&buttonsirqApp, SIGNAL(BtnSignal(int)),
             this, SLOT(buttonsHandle(int)), Qt::QueuedConnection);//, Qt::DirectConnection

     //治疗结束提示框#/
     CureFinishDlg = new Window_Note(0, QObject::tr("CureFinish"), ":/resource/test.png");//治疗完成提示按钮#/
     CureFinishDlg -> raise();
     connect(CureFinishDlg, SIGNAL(NoteSelSignal(int)), this, SLOT(NoteSel(int)),Qt::DirectConnection);

     NoteCureMode = new Window_Note(0, QObject::tr("Cure Mode"), ":/resource/test.png");//进入治疗的提示按钮#/
     NoteCureMode->raise();
     connect(NoteCureMode, SIGNAL(NoteSelSignal(int)), this, SLOT(ReciveCureNote(int)),Qt::DirectConnection);

     CureLeaveDlg = new Window_Note(0, QObject::tr("CureLeave"), ":/resource/test.png");//停止治疗提示按钮#/
     CureLeaveDlg->raise();
     connect(CureLeaveDlg, SIGNAL(NoteSelSignal(int)), this, SLOT(ReciveCureLeaveNote(int)),Qt::DirectConnection);

     NoteReturnReady = new Window_Note(0, QObject::tr("Return to ready"), ":/resource/test.png");//回到准备状态提示按钮#/
     NoteReturnReady->raise();
     connect(NoteReturnReady, SIGNAL(NoteSelSignal(int)), this, SLOT(ReturnToReady(int)),Qt::DirectConnection);    
}

void WidgetSwitch::ReciveCureNote(int id)
{
    if(id==1){//接收到治疗提示按钮选项，启用当前治疗模式确认命令#/
        isFlashShow=false;
        WorkState  stateTMP=GetWorkStateManage();
        //PDOstate.state.N1_status=0x03;
        //PDOstate.state.N1_sub_status =0x35;
        stateTMP.tipState.current = TREAT_RUNNING;
        stateTMP.subState.current = UF_TIME_NOT_COMPLETE;
        stateTMP.disinfectEnable = false;
        SetWorkStateManage(&stateTMP);//总状态变为治疗状态#/
        treatment_run();               //开始进行治疗#/
        if(Blood_online_return==true&&preflush_model == online_model)
            widgetButtonPrime.setText(DE_TR_ONLINERETURNBLOOD);
        else
            widgetButtonPrime.setText(tr("Return Blood"));//预冲键变为回血键#/
        widgetButtonPrime.setDisabled(false);

        //更新超滤量#/
        dataRead(PACKET_MASTERCTRL_UFVALUE,&packetUfValue);
        dataNodeUpdate(PACKET_MASTERCTRL_UFVALUE,&packetUfValue);
    }
    else{//否，不启动治疗，还原治疗按键#/
        widgetButtonTreat.setChecked(false);
    }
}

void WidgetSwitch::ClickedCure(void)
{

    logInfo log;

    log.opration="ClickedCure";
    log.type=DE_LOGINFO_OPER;
    log.status=PDOstate.state.N1_status;
    log.note="warning";
    LogRun->addLog(log);

    handle_count=0;
    WorkState stateTMP=GetWorkStateManage();
    if(stateTMP.tipState.current == TREAT_RUNNING){//治疗运行中#/
        widgetButtonTreat.setChecked(true);
        return;//治疗状态治疗按钮只显示#/
    }
    else if(stateTMP.tipState.current == TREAT_READY){//准备状态弹出治疗提示框#/
        if(NoAlarm == bubble)  //有血
        {
            showNoteDialog("Cure Mode");//弹出确认启动的治疗模式#/
        }
        else  //引血处理#/
        {
            if(stateTMP.subState.current==GETBLOOD_RUN_FLUID||stateTMP.subState.current==GETBLOOD_RUN_NO_FLUID)
            {
                getblood_stop();
            }
            else
            {
                if(getbloodstartflag==false)  //第一次开始引血#/
                {
                    if(connectstatus==false)
                    {
                        connect(YesNoDialog,SIGNAL(NoteSelSignal(int)),this,SLOT(ConfirmGetBlood(int)));
                        connectstatus=true;
                        showYesNoDialog(DE_TR_PLEASECONFIRM,DE_TR_AREYOUSUREPREFLUSH);
                    }
                }
                else
                {
                    isFlashShow=false;
                    getblood_run();
                    handle_count=0;
                    S_Delay_Count=5;
                }
            }
        }
    }
    else if(stateTMP.tipState.current == TREAT_FINISH){//治疗结束状态#/
        if(stateTMP.subState.current == CURE_BLOODRETURN_STOP_NO_BLOOD || CURE_BLOODRETURN_STOP == stateTMP.subState.current){
            widgetButtonTreat.setChecked(false);//没有启动过排空，且回血停止状态才能回到准备状态#/
            showNoteDialog("Return to ready");//弹出确认启动的返回准备状态提示框#/
            return;
        }
        widgetButtonTreat.setChecked(false);//不能回到准备状态#/
        return;
    }
   /* if(workstate.current == HD_double_needle_stop)
    {//不需要作此判断#/
        widgetButtonTreat.setChecked(false);
        return;
     }

    if (statusPreShoot == true) {
        //true 正在预冲,由于只有检测到血才能启动治疗，而检测到血，预冲就完成，所以不需要在这里对预冲进行处理#/
        statusPreShoot = false;
        widgetButtonPrime.setChecked(false);
    }*/
}

void WidgetSwitch::ReturnToReady(int sel)//返回到准备状态时初始化各个参数#/
{    

    if(sel==1||sel==2)
    {
        if (preflush_model == auto_model)widgetButtonPrime.setText(tr("Auto Prime"));//预冲按钮的模式
        else if(preflush_model == online_model)widgetButtonPrime.setText(DE_TR_ONLINEPRIME);
        else if ( (preflush_model == manual_model))widgetButtonPrime.setText(tr("Manual Prime"));
//        widgetButtonTreat.setText(tr("Treatment"));

        widgetButtonUFPump.setText(tr("UF Pump"));//准备状态下各按钮的功能#/
        getbloodstartflag=false;   //引血开始标志#/
        widgetButtonTreat.setChecked(false);
        widgetButtonPrime.setChecked(false);
        widgetButtonUFPump.setDisabled(false);

        WorkState  stateTMP=GetWorkStateManage();
        stateTMP.tipState.current = TREAT_READY;
        stateTMP.disinfectEnable = true;

        heparinPreStop=false;  //重设肝素的状态#/

        if((waterroadOk == 1) && (bloodroadOk == 1)){//血路水路ok#/          
            if(NoAlarm == bubble){//有血闪动治疗按键#/
                widgetButtonPrime.setDisabled(true);
                widgetButtonTreat.setText(tr("Treatment"));
                widgetButtonTreat.setDisabled(false);                
                isFlashShow = true;
                if(!bloodlevel)
                    stateTMP.subState.current=TREAT_ENABLE_FLUID;//有血有液可以进入治疗#/
                else
                    stateTMP.subState.current=TREAT_ENABLE_NO_FLUID;//有血无液可以进入治疗#/
            }
            else{

                if(Blood_online_lead==true&&preflush_model == online_model)
                    widgetButtonTreat.setText(DE_TR_ONLINECONNECTPT);
                else
                    widgetButtonTreat.setText(tr("Connect P.t"));

                widgetButtonPrime.setDisabled(false);
//                widgetButtonTreat.setDisabled(true);
                isFlashShow = false;
                if(!bloodlevel)
                    stateTMP.subState.current = SELFTEST_COMPLETE_FLUID;
                else
                    stateTMP.subState.current = SELFTEST_COMPLETE_NO_FLUID;
            }
        }
        else{  //血路水路 not ok#/
            isFlashShow=false;
            stateTMP.subState.current = SUB_STATE_NULL;
            widgetButtonPrime.setDisabled(true);
            widgetButtonTreat.setDisabled(true);
            widgetButtonUFPump.setDisabled(true);
            widgetButtonHeparinPump.setDisabled(true);
            widgetButtonBypass.setDisabled(true);
        }

//        G_CURE_STARTTIME=0;  //治疗开始时间#/
//        G_CURE_ENDTIME=0;    //治疗结束时间#/

        //准备状态#/
#if HJX_COMMUNICATION_SWITCH
                emit signalSDO(DE_CMD_Pass_Checkself);
#else
                MasterCtrol_CMD_Pass_Checkself();
#endif
        //清空相关参数，再返回准备状态#/
#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_ResetBloodAmount);
        emit signalSDO(DE_CMD_ResetUFAmount);
#else
        //清空相关参数
        MasterCtrol_CMD_ResetBloodAmount();
        us_sleep(100000);
        MasterCtrol_CMD_ResetUFAmount();
#endif

        SetWorkStateManage(&stateTMP);//返回准备状态#/
    }

    if(sel==1){   //治疗参数保留 返回准备状态#/
//        UFPump_Cure_Amount += (G_UF_CUREFINISH-UFPump_Cure_Amount);
        G_UF_CUREFINISH=0;//返回准备状态，清0#/
//        UFPump_Pre_Amount += G_UF_PREFINISH;
        G_UF_PREFINISH=0;

        G_SAVECUREPARAM_FLAG=1;  //保存参数标志#/
        dataNodeUpdate(DE_STR_SAVECUREPARAM,&G_SAVECUREPARAM_FLAG);
    }
    else if(sel == 2){   //不保留，清空相关参数，再返回准备状态#/
        BloodPump_Pre_Amount=0;   //血泵预冲累计量#/
        BloodPump_Cure_Amount=0;  //血泵治疗累计量#/
        UFPump_Pre_Amount=0;   //超滤泵预冲累计量#/
        UFPump_Cure_Amount=0;   //超滤泵治疗累计量#/
        G_UF_PREFINISH=0;     //预冲超滤量清0#/
        G_UF_CUREFINISH=0;//返回准备状态，清0#/
        BloodPump_Volume=0;
        HeparinPumpFirstflag=false;  //肝素泵首剂量标志

#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_ResetHeparinPumpAmount);
        emit signalSDO(DE_CMD_ResetSubsAmount);   //ReturnToReady() 不保留参数时#/
#else
        MasterCtrol_CMD_ResetHeparinPumpAmount();
        us_sleep(10000);
        MasterCtrol_CMD_ResetSubsAmount();
        us_sleep(100000);
#endif

        G_SAVECUREPARAM_FLAG=0;  //保存参数标志#/

        struct Params params;
        dataRead(PACKET_MAINTAIN_PARAMS, &params);
        dataNodeUpdate(PACKET_MAINTAIN_PARAMS,&params);   

        //根据xml文件更新参数#/
  //      ReUpdateXmlConfig();
    }
    else{  //取消并返回#/
        widgetButtonTreat.setChecked(false);
    }
}

void WidgetSwitch::ConfirmSelect(int sel)
{
    WorkState workstateTMP;
    workstateTMP = GetWorkStateManage();

    if(workstateTMP.subState.current==DRAIN_STOP)
    {
        if(sel==true)  //yes 继续排空#/
        {
            ClickedUFPump();
        }
    }
    else
    {
        if(sel==true)  //继续回血#/
        {
            ClickedPreShoot();
        }
        else
        {
            timeCount=0;  //否继续回血#/
        }
    }
    YesNoDialog->disconnect(this);
    connectstatus=false;
}

void WidgetSwitch::ConfirmGetBlood(int sel)  //开始引血#/
{
    if(sel==true)
    {
        getblood_run();
    }
    else
    {
        widgetButtonTreat.setChecked(false);
    }
    YesNoDialog->disconnect(this);
    connectstatus=false;
}

void WidgetSwitch::ConfirmStartDrain(int sel)//确认是否开始排空#/
{
    WorkState stateTMP = GetWorkStateManage();
    if(sel==true)
    {
        DRY_start();//排空#/
        timeCount=0;     //排空#/
        stateTMP.subState.current = DRAIN_RUN;
        widgetButtonPrime.setDisabled(true);  //回血键永远变灰#/
        widgetButtonTreat.setDisabled(true);  //准备键永远变灰#/
        widgetButtonUFPump.setChecked(true);
        stateTMP.disinfectEnable = false;//禁止进入消毒界面#/
        SetWorkStateManage(&stateTMP);//排空设置#/
    }
    YesNoDialog->disconnect(this);
    connectstatus=false;
}


void WidgetSwitch::ReciveCureLeaveNote(int id)
{
    if(id==1){//接收到停止治疗提示框的信号，停止治疗#/

        widgetButtonPrime.setChecked(false);
        isFlashShow=true;//闪烁回血键#/
        WorkState  stateTMP=GetWorkStateManage();
        stateTMP.tipState.current = TREAT_FINISH;
        if(bubble)stateTMP.subState.current = CURE_BLOODRETURN_STOP_NO_BLOOD;//没有血
        else{stateTMP.subState.current = CURE_BLOODRETURN_STOP;}//有血#/
        SetWorkStateManage(&stateTMP);//子状态变为回血状态#/

        treatment_stop();//停止治疗的相关PUMP#/
        handle_count=0;
        S_Delay_Count=4;

        /*
        dataRead(PACKET_SDO_BLOOD,&sdoBlood);
        sdoBlood.N3_set_BPspeed=100;
        G_SET_BPSPEED=sdoBlood.N3_set_BPspeed;
        dataNodeUpdate(PACKET_SDO_BLOOD, &sdoBlood);//停止治疗#/
        */
        if(G_BPRATE_RETURNBLOOD>0)
        {
            CHANGED_BP_SPEED(G_BPRATE_RETURNBLOOD);//改变为回血默认速度#/
        }

        widgetButtonTreat.setText(tr("Ready"));
        widgetButtonUFPump.setText(tr("Drain"));
        widgetButtonUFPump.setDisabled(true);  //不可用，回血后方可使用#/
        showYesDialog(DE_TR_PROMPT,DE_TR_TREATMENTFINISH);
        //应该添加回血相关动作#/
    }
    else{//不停止治疗#/
        widgetButtonPrime.setChecked(false);
    }
}

void WidgetSwitch::ClickedPreShoot(void)
{
    handle_count=0;
    WorkState  stateTMP=GetWorkStateManage();
    if(stateTMP.tipState.current == TREAT_READY)
    {   //准备状态下启动/停止预冲#/
        if((PREFLUSH_RUN_FLUID != stateTMP.subState.current) && (PREFLUSH_RUN_NO_FLUID != stateTMP.subState.current)){
            if(NoAlarm == bubble){//有血预冲已完成#/
                prime_stop(preflush_model,PRIME_STOP_FROM_BLOOD);
                return;
            }
            prime_run(preflush_model);//没有血则启动预冲#/
            handle_count=0;
            S_Delay_Count=5;
        }
        else{
            prime_stop(preflush_model,PRIME_STOP_FROM_PUMP);//停止预冲#/
            return;
        }
    }
    else if(stateTMP.tipState.current == TREAT_RUNNING)
    {//治疗状态停止治疗进入回血#/
        widgetButtonPrime.setChecked(true);
        showNoteDialog("CureLeave");//治疗过程中点击回血键，弹出是否中断治疗提示框#/
        return;
    }
    else if((stateTMP.tipState.current == TREAT_FINISH))
    {//治疗结束状态下，点击回血键进行回血/停止回血#/
        if(DRAIN_RUN == stateTMP.subState.current){
            widgetButtonPrime.setChecked(false);//排空中，点击回血键无效，#/
            return;
        }

        blockbloodflag=1;

        if(stateTMP.subState.current == CURE_BLOODRETURN_STOP \
                || stateTMP.subState.current == CURE_BLOODRETURN_STOP_NO_BLOOD\
                || stateTMP.subState.current == CURE_BLOODRETURN_STOP_DRAIN\
                || stateTMP.subState.current == CURE_BLOODRETURN_STOP_DRAIN_NO_BLOOD\
                || stateTMP.subState.current == DRAIN_STOP)
        {//子状态 处于停止回血，停止排空状态#/
            /*
            if (sdoBlood.N3_set_BPspeed != usr_params.Params_BP.Rate_ReturnBlood.attribute.default_value)
            {   //更新回血速度#/
                interactBp_ReturnBlood = 1;
                sdoBlood.N3_set_BPspeed = usr_params.Params_BP.Rate_ReturnBlood.attribute.default_value;
                dataNodeUpdate(PACKET_SDO_BLOOD, &sdoBlood);  //回血#/
            }
            */
            isFlashShow=false;
           L_workstate.current =Cure_bloodreturn_run;
            widgetButtonPrime.setChecked(true);//按键置于按下状态#/
      //      timeCount=0;

            PacketWorkMode workModeTMP;
            dataRead(PACKET_NAME_WORKMODE,&workModeTMP);
            if(workModeTMP.treatmode.current==HDF_pre_dilute||workModeTMP.treatmode.current==HDF_post_dilute\
               ||workModeTMP.treatmode.current==HF_pre_dilute||workModeTMP.treatmode.current==HF_post_dilute)
            {
                if(preflush_model==online_model)
                {
                    if(Blood_online_return==true)
                        Startreturnblood_mode=ONLINE_RETURNBLOOD_MODE;
                    else
                        Startreturnblood_mode=RETURNBLOOD_MODE;
                }
                else
                {
                    Startreturnblood_mode=RETURNBLOOD_MODE;
                }
            }
            else
            {
                   Startreturnblood_mode=RETURNBLOOD_MODE;
            }

#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_Startreturnblood);
#else
        us_sleep(100000);
        MasterCtrol_CMD_Startreturnblood(Startreturnblood_mode);//启动回血#/
        us_sleep(10000);
#endif

            widgetButtonTreat.setDisabled(true);  //准备键变灰#/
            widgetButtonUFPump.setDisabled(true);
            stateTMP.disinfectEnable = false;//不可已进入消毒界面#/

            if(bubble){//无血#/
                if(stateTMP.subState.current == CURE_BLOODRETURN_STOP || stateTMP.subState.current == CURE_BLOODRETURN_STOP_NO_BLOOD){
                    stateTMP.subState.current =  CURE_BLOODRETURN_RUN_NO_BLOOD;//没有启动过排空#/
                }
                else{stateTMP.subState.current = CURE_BLOODRETURN_RUN_DRAIN_NO_BLOOD;}//启动过排空#/
            }
            else{//有血#/
                if(stateTMP.subState.current == CURE_BLOODRETURN_STOP || stateTMP.subState.current == CURE_BLOODRETURN_STOP_NO_BLOOD){
                    stateTMP.subState.current = CURE_BLOODRETURN_RUN;//没有启动过排空#/
                }
                else{stateTMP.subState.current = CURE_BLOODRETURN_RUN_DRAIN;}//曾经启动派空#/
            }

            BP_start();          //启动回血#/

        }
        else if(stateTMP.subState.current == CURE_BLOODRETURN_RUN\
                ||stateTMP.subState.current == CURE_BLOODRETURN_RUN_NO_BLOOD\
                ||stateTMP.subState.current == CURE_BLOODRETURN_RUN_DRAIN\
                ||stateTMP.subState.current == CURE_BLOODRETURN_RUN_DRAIN_NO_BLOOD){//子状态 处于回血运行状态#/

            L_workstate.current =Cure_bloodreturn_stop;
            interactBp_ReturnBlood = 0;

            BP_stop();//停止回血#/
            isFlashShow=false;                 

#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_Stopreturnblood);
#else
        us_sleep(100000);
        MasterCtrol_CMD_Stopreturnblood();//停止回血#/
        us_sleep(10000);
#endif
            widgetButtonPrime.setChecked(false);
            widgetButtonUFPump.setDisabled(false);  //排空可用#/

            if(bubble){
                if(stateTMP.subState.current == CURE_BLOODRETURN_RUN || stateTMP.subState.current == CURE_BLOODRETURN_RUN_NO_BLOOD){
                    stateTMP.subState.current = CURE_BLOODRETURN_STOP_NO_BLOOD;//没有启动过排空#/
                }
                else{stateTMP.subState.current = CURE_BLOODRETURN_STOP_DRAIN_NO_BLOOD;}//曾经启动派空#/
            }
            else{
                if(stateTMP.subState.current == CURE_BLOODRETURN_RUN || stateTMP.subState.current == CURE_BLOODRETURN_RUN_NO_BLOOD){
                    stateTMP.subState.current = CURE_BLOODRETURN_STOP;//没有启动过排空#/
                }
                else{stateTMP.subState.current = CURE_BLOODRETURN_STOP_DRAIN;}//曾经启动派空#/
            }

            if(stateTMP.subState.current == CURE_BLOODRETURN_STOP||stateTMP.subState.current == CURE_BLOODRETURN_STOP_NO_BLOOD)
                widgetButtonTreat.setDisabled(false);
            if(stateTMP.subState.current == CURE_BLOODRETURN_STOP_NO_BLOOD||stateTMP.subState.current == CURE_BLOODRETURN_STOP_DRAIN_NO_BLOOD)
                stateTMP.disinfectEnable = true;   //回血后要无血时可以进入消毒#/
        }
        SetWorkStateManage(&stateTMP);  //保存当前的回血状态#/
    }
}

void WidgetSwitch::ClickedUFPump(void)
{
    WorkState stateTMP = GetWorkStateManage();
    if(TREAT_RUNNING == stateTMP.tipState.current||TREAT_READY == stateTMP.tipState.current)
    {//准备状态和治疗状态作为启动和停止超滤泵用#/
        if ((G_UFPUMP_STATUS==false))
        {//启动超滤泵#/
            UF_start();
            /*
            if(Alarm_UFPump_Flag==true)
            {
                alarmmodule->Afresh_Alarm();
            }
            */
            Alarm_UFPump_Flag=false;
            prestatusUFPump=true;    //超滤按键处理--打开了超滤泵标示
        }
        else
        {
            //停止超滤泵#/
            UF_stop();   //超滤泵按键操作#/
            prestatusUFPump=false;  //超滤按键处理--关闭超滤泵标示
        }
        handle_count=0;
        S_Delay_Count=4;
    }
    else if(TREAT_FINISH == stateTMP.tipState.current)
    {//作为排空键#/
        if(G_BPPUMP_STATUS || (CURE_BLOODRETURN_RUN == stateTMP.subState.current)\
                || (CURE_BLOODRETURN_RUN_NO_BLOOD == stateTMP.subState.current)\
                || (CURE_BLOODRETURN_RUN_DRAIN == stateTMP.subState.current)\
                || (CURE_BLOODRETURN_RUN_DRAIN_NO_BLOOD == stateTMP.subState.current)){//回血状态，点击没作用
            widgetButtonUFPump.setChecked(false); //回血时，点击排空无用#/
            return;
        }
        else if(DRAIN_RUN != stateTMP.subState.current && G_BPPUMP_STATUS==false){//治疗状态结束和血泵停止才能启动排空

            if(stateTMP.subState.current==DRAIN_STOP)
            {
                DRY_start();//排空#/
                timeCount=0;    //点击排空时#/
                stateTMP.subState.current = DRAIN_RUN;
                widgetButtonTreat.setDisabled(true);  //准备键永远变灰#/
                widgetButtonUFPump.setChecked(true);
                stateTMP.disinfectEnable = false;//禁止进入消毒界面#/
                SetWorkStateManage(&stateTMP);
            }
            else if(stateTMP.subState.current==CURE_BLOODRETURN_STOP||stateTMP.subState.current==CURE_BLOODRETURN_STOP_NO_BLOOD)
            {
                connect(YesNoDialog,SIGNAL(NoteSelSignal(int)),this,SLOT(ConfirmStartDrain(int)));
                connectstatus=true;
                showYesNoDialog(DE_TR_PLEASECONFIRM,DE_TR_WHETHERHASENDEDRETURNBLOOD);
            }
        }
        else{
            DRY_stop();//停止排空#/
//            widgetButtonPrime.setDisabled(false);  //回血键#/
            widgetButtonUFPump.setChecked(false);
            stateTMP.subState.current = DRAIN_STOP;
            stateTMP.disinfectEnable = true;//可以进入消毒界面#/
            SetWorkStateManage(&stateTMP);
        }
    }
    handle_count=0;
}

void WidgetSwitch::ClickedHeparinPump(void)
{
    if(G_HEPARINPUMP_STATUS==false)
    {
   //      dataCanSend->test_mutex(true); //线程死锁测试#/

         HeparinPump_run();
         /*
         if(Alarm_HeparinPump_Flag==true)
         {
            alarmmodule->Afresh_Alarm();
         }
         */
         Alarm_HeparinPump_Flag=false;
         prestatusHeparinPump=true;
    }
    else
    {
         HeparinPump_stop();
         prestatusHeparinPump=false;
    }
    handle_count=0;
    S_Delay_Count=5;
}

//点击旁路#/
void WidgetSwitch::ClickedSideWay(void)
{
/*
    if(G_BYPASS_BUT_FLAG==DE_BYPASS_MASTERPASS && statusSideWay==true)
    {
        statusSideWay=true;
    }
    else
        statusSideWay = !statusSideWay;    //ClickedSideWay()，点击旁路键时改变#/
*/

    statusSideWay = !statusSideWay;    //ClickedSideWay()，点击旁路键时改变#/

    widgetButtonBypass.setStyleDisabled(":/resource/images/switch_side_road.png",
            ":/resource/images/switch_side_road_active.png",
            ":/resource/images/switch_side_road_disable.png",
            CONFIG_COLOR_WHITE,
            QRect(WIDGETSWITCH_BUTTON_SIDEROAD_X,
                WIDGETSWITCH_BUTTON_SIDEROAD_Y,
                WIDGETSWITCH_BUTTON_SIDEROAD_WIDTH,
                WIDGETSWITCH_HIGH));

    if(statusSideWay){
        bypass();
#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_Bypass);
#else
        MasterCtrol_CMD_Bypass();
#endif
        G_BYPASS_BUT_FLAG=DE_BYPASS_CHECKED;
        prestatusSideWay=true;
    }
    else{
        nobypass();
#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_Nobypass);
#else
        MasterCtrol_CMD_Nobypass();
#endif
        G_BYPASS_BUT_FLAG=DE_BYPASS_NOCHECKED;
        prestatusSideWay=false;
    }
    handle_count=0;
    S_Delay_Count=4;
}

void WidgetSwitch::bypass()
{
        if(!statusSideWay)
        {
            statusSideWay=true;   //bypass()，赋值旁路状态#/
            widgetButtonBypass.setChecked(true);
            //this->update();
            //us_sleep(100000);
            //MasterCtrol_CMD_Bypass();
        }
}

void WidgetSwitch::nobypass()
{
        if(statusSideWay)
         {
            statusSideWay=false;   //bypass()，赋值无旁路状态#/
            widgetButtonBypass.setChecked(false);
            //this->update();
            //us_sleep(100000);
            //MasterCtrol_CMD_Nobypass();
        }
}

void WidgetSwitch::HeparinPump_run()
{
//        struct PacketHeparinPump heparinData;
//        dataRead(PACKET_NAME_HEPARINPUMP,&heparinData);
        G_HEPARINPUMP_STATUS=true;
        widgetButtonHeparinPump.setChecked(true);
        if(HeparinPumpFirstflag==false)  //首剂量标志#/
        {
#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_HeparinPumpStartHaveFirst);
#else
        MasterCtrol_CMD_HeparinPumpStartHaveFirst();
#endif
        HeparinPumpFirstflag=true;
        }
        else if(HeparinPumpFirstflag==true)
        {
#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_HeparinPumpStartNoFirst);
#else
        MasterCtrol_CMD_HeparinPumpStartNoFirst();
#endif
        }
//        heparinData.runstatus=1;
//        dataNodeUpdate(PACKET_NAME_HEPARINPUMP,&heparinData);
}

void WidgetSwitch::HeparinPump_stop()
{
//    struct PacketHeparinPump heparinData;
//    dataRead(PACKET_NAME_HEPARINPUMP,&heparinData);
    G_HEPARINPUMP_STATUS=false;
    widgetButtonHeparinPump.setChecked(false);
#if HJX_COMMUNICATION_SWITCH
    emit signalSDO(DE_CMD_HeparinPumpStop);
#else
    MasterCtrol_CMD_HeparinPumpStop();
#endif
//    heparinData.runstatus=0;
//    dataNodeUpdate(PACKET_NAME_HEPARINPUMP,&heparinData);
}

void WidgetSwitch::CustomSetText()
{
    //widgetButtonPrime.setText(tr("Manual Prime"));
    if ( (preflush_model == auto_model)) {
        widgetButtonPrime.setText(tr("Auto Prime"));
    }else if ( (preflush_model == manual_model)) {
        widgetButtonPrime.setText(tr("Manual Prime"));
    }
    else if(preflush_model == online_model)
    {
        widgetButtonPrime.setText(DE_TR_ONLINEPRIME);
    }

//    widgetButtonTreat.setText(tr("Treatment"));
    if(Blood_online_lead==true&&preflush_model == online_model)
        widgetButtonTreat.setText(DE_TR_ONLINECONNECTPT);
    else
        widgetButtonTreat.setText(tr("Connect P.t"));
    widgetButtonUFPump.setText(tr("UF Pump"));
    widgetButtonHeparinPump.setText(tr("Heparin Pump"));
    widgetButtonBypass.setText(tr("Bypass"));
}

void WidgetSwitch::ActionHandle(const void* dataPtr,int name)
{

#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetSwitch-ActionHandle-in");
#endif

    if(workstate.tipState.current==DEBUG)
        return;

    if ((dataPtr == NULL) || (name == 0)) {
        return;
    }

    if (name==SelfCheck_Timer){  //接收倒计时页面的自检使能信号#/
        isSelfcheckstart=true;
        return;
    }

    if(name==PACKET_NAME_HEPARINPUMP){  //肝素泵页面变化时#/
        PacketHeparinPump *heparnPtr = (PacketHeparinPump*)dataPtr;
        WorkState  stateTMP=GetWorkStateManage();
        if((stateTMP.tipState.current == TREAT_READY||stateTMP.tipState.current==TREAT_RUNNING)&&(stateTMP.subState.current>SELFTEST)){
            if(heparnPtr->selstatus==0&&(widgetButtonHeparinPump.isEnabled()==true))
            {
                G_HEPARINPUMP_STATUS=false;
                widgetButtonHeparinPump.setChecked(false);
            #if HJX_COMMUNICATION_SWITCH
                emit signalSDO(DE_CMD_HeparinPumpStop);
            #else
                MasterCtrol_CMD_HeparinPumpStop();
            #endif
                widgetButtonHeparinPump.setDisabled(true);
            }
            else if(heparnPtr->selstatus==1&&(widgetButtonHeparinPump.isEnabled()==false))
            {
                G_HEPARINPUMP_STATUS=false;
                widgetButtonHeparinPump.setChecked(false);
            #if HJX_COMMUNICATION_SWITCH
                emit signalSDO(DE_CMD_HeparinPumpStop);
            #else
                MasterCtrol_CMD_HeparinPumpStop();
            #endif
                widgetButtonHeparinPump.setDisabled(false);
            }
        }
    }

    if (name==PACKET_NAME_WORKSTATE){//工作状态改变#/
        L_workstate =*((WorkState*) dataPtr);

        WorkState  stateTMP=GetWorkStateManage();
        if(stateTMP.tipState.previous == TREAT_READY&&stateTMP.tipState.current==TREAT_RUNNING){
            if(check_alarm==false)  //报警升级#/
            {
                /*
                clearMessagesDialogInfo();
                closeMessagesDialog();
                clearAlarmDialogInfo();
                closeAlarmDialog();
                clearWarnDialogInfo();
                closeWarnDialog();
                curtimeAlarmMap.clear();
                curAlarmMap.clear();
                preAlarmMap.clear();
                */
                check_alarm=true;
            }
        }
        return;
    }

    if (name==PACKET_BLOODROAD_OK){//获取血路状态，1为正常#/
        bloodroadOk = *((int*) dataPtr);
        return;
    }

    if (name==PACKET_WATERROAD_OK){//获取水路状态，1为正常#/
        waterroadOk = *((int*) dataPtr);
        return;
    }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetSwitch-ActionHandle-01");
#endif

    //--------------------processs C
    if ((waterroadOk == 1) && (bloodroadOk == 1) &&(G_rebirth_flag==false)){//水路和血路正常#/
        WorkState stateTMP = GetWorkStateManage();
        //使能主界面的预冲，超滤泵，肝素泵，旁路按钮#/
        if(stateTMP.tipState.current == TREAT_READY&&(!widgetButtonBypass.isEnabled()))
        {
//            struct PacketHeparinPump heparinData;
//           dataRead(PACKET_NAME_HEPARINPUMP,&heparinData);

            widgetButtonPrime.setDisabled(false);
            widgetButtonTreat.setDisabled(false);

            if(preflush_model!=online_model)
                widgetButtonUFPump.setDisabled(false);

//            if(heparinData.selstatus==1)
            if(G_HeparinEnable_flag==true)
                widgetButtonHeparinPump.setDisabled(false);

            widgetButtonBypass.setDisabled(false);
            if(!bloodlevel)
                stateTMP.subState.current = SELFTEST_COMPLETE_FLUID; //自检完成，有液#/
            else
                stateTMP.subState.current = SELFTEST_COMPLETE_NO_FLUID; //自检完成，无液#/
            SetWorkStateManage(&stateTMP);
//            this->update();
            BP_stop();     //停止血泵#/

            if(S_Clear_flag==false) //清除累计量#/
            {    //清空相关参数，再返回准备状态#/
                #if HJX_COMMUNICATION_SWITCH
                    emit signalSDO(DE_CMD_ResetBloodAmount);
                    emit signalSDO(DE_CMD_ResetSubsAmount);//自检后，清累计量#/
                    emit signalSDO(DE_CMD_ResetHeparinPumpAmount);
                    emit signalSDO(DE_CMD_ResetUFAmount);
                #else
                    //清空相关参数
                    MasterCtrol_CMD_ResetBloodAmount();
                    us_sleep(100000);
                    MasterCtrol_CMD_ResetSubsAmount();
                    us_sleep(100000);
                    MasterCtrol_CMD_ResetHeparinPumpAmount();
                    us_sleep(10000);
                    MasterCtrol_CMD_ResetUFAmount();
                #endif

                S_Clear_flag=true;
            }
        }
        else if(stateTMP.tipState.current == CLEANING&&(widgetButtonPrime.isEnabled()||widgetButtonTreat.isEnabled()))
        {
            widgetButtonPrime.setDisabled(true);
            widgetButtonTreat.setDisabled(true);
            widgetButtonUFPump.setDisabled(true);
            widgetButtonHeparinPump.setDisabled(true);
            widgetButtonBypass.setDisabled(true);
        }
    }
    else if(G_rebirth_flag==false)  //水路或血路不稳定时失能按键
    {
        isFlashShow=false;
        WorkState stateTMP = GetWorkStateManage();
        if(stateTMP.tipState.current == TREAT_READY&&(widgetButtonBypass.isEnabled()))
        {
            if(Blood_online_lead==true&&preflush_model == online_model)
                widgetButtonTreat.setText(DE_TR_ONLINECONNECTPT);
            else
                widgetButtonTreat.setText(tr("Connect P.t"));
            widgetButtonPrime.setDisabled(true);
            widgetButtonTreat.setDisabled(true);
            widgetButtonUFPump.setDisabled(true);
            widgetButtonHeparinPump.setDisabled(true);
            widgetButtonBypass.setDisabled(true);
            stateTMP.subState.current = SUB_STATE_NULL; //NULL#/
            SetWorkStateManage(&stateTMP);
        }
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetSwitch-ActionHandle-02");
#endif
    //--------------------processs B
    if (name==Preflush_Model){//获取预冲模式，更新界面显示#/
        preflush_model = *((PreflushModel*) dataPtr);
        WorkState stateTMP = GetWorkStateManage();
        if(stateTMP.tipState.current == TREAT_READY)
        {
            if (preflush_model == auto_model) {
                widgetButtonPrime.setText(tr("Auto Prime"));
            }
            else if(preflush_model == online_model)
            {
                widgetButtonPrime.setText(DE_TR_ONLINEPRIME);
            }
            else if ( (preflush_model == manual_model)) {
                widgetButtonPrime.setText(tr("Manual Prime"));
            }

            if(preflush_model==online_model)
            {
                widgetButtonUFPump.setDisabled(true); //online预冲禁用超滤泵#/
                UF_stop();     //预冲模式改变时---在线预冲停止超滤泵#/
            }
            else
            {
                 if ( (waterroadOk == 1) && (bloodroadOk == 1) ){//水路和血路正常#/
                    widgetButtonUFPump.setDisabled(false);
                 }
            }

            if(stateTMP.subState.current!=TREAT_ENABLE_FLUID&&stateTMP.subState.current!=TREAT_ENABLE_NO_FLUID)
            {
                if(Blood_online_lead==true&&preflush_model == online_model)
                    widgetButtonTreat.setText(DE_TR_ONLINECONNECTPT);
                else
                    widgetButtonTreat.setText(tr("Connect P.t"));
            }
        }
        else if(stateTMP.tipState.current==TREAT_RUNNING||stateTMP.tipState.current==TREAT_FINISH)
        {
            if(Blood_online_return==true&&preflush_model == online_model)
                widgetButtonPrime.setText(DE_TR_ONLINERETURNBLOOD);
            else
                widgetButtonPrime.setText(tr("Return Blood"));//预冲键变为回血键#/
        }
//        this->update();
         return;
    }    

    //当静脉有血时，则先停血泵，然后出提示框#/
    if (name==VEINS_HAVE_BLOOD){//获取是否有气泡，没有气泡表示有血#/
        bubble = *((int*) dataPtr);//bubble 默认为NoAlarm  1有气泡告警，0无气泡告警#/
        WorkState  stateTMP=GetWorkStateManage();

        if(NoAlarm == bubble){//没有气泡报警,有血#/
            if(stateTMP.tipState.current == TREAT_READY)
            {//准备状态下#/
                if((PREFLUSH_RUN_FLUID == stateTMP.subState.current) || \
                        (PREFLUSH_RUN_NO_FLUID == stateTMP.subState.current)){//有血,预冲结束#/
                    prime_stop(preflush_model,PRIME_STOP_FROM_BLOOD);//有血，预冲结束#/
                }
                else
                {
                    if((GETBLOOD_RUN_FLUID == stateTMP.subState.current)||(GETBLOOD_RUN_NO_FLUID == stateTMP.subState.current))
                    {
                        getblood_stop();
                    }
                }

         //       if((waterroadOk == 1) && (bloodroadOk == 1)){
                 if((waterroadOk == 1) && (bloodroadOk == 1)){
                    if(isFlashShow==false)
                    {
                        if((stateTMP.subState.current!=TREAT_ENABLE_FLUID)&&(stateTMP.subState.current!=TREAT_ENABLE_NO_FLUID))
                        {
                            if(!bloodlevel)
                                stateTMP.subState.current=TREAT_ENABLE_FLUID;
                            else
                                stateTMP.subState.current=TREAT_ENABLE_NO_FLUID;
                            SetWorkStateManage(&stateTMP);
                        }

                        //有血时，禁用预冲键#/
                        widgetButtonPrime.setDisabled(true);
                        check_alarm=false;
                        /*
                        struct PacketUF packetTmp;
                        dataRead(PACKET_NAME_UF,&packetTmp);
                        packetTmp.ufRate=50;
                        packetTmp.chooseRate = true;           //选用治疗的超滤速度#/
                        dataNodeUpdate(PACKET_NAME_UF, &packetTmp);
                        */

                        /*
                        dataRead(PACKET_SDO_BLOOD,&sdoBlood);
                        sdoBlood.N3_set_BPspeed=100;
                        dataNodeUpdate(PACKET_SDO_BLOOD, &sdoBlood);  //开始治疗#/
                        */
//                        if(G_SAVECUREPARAM_FLAG==0)
                        CHANGED_BP_SPEED(G_BPRATE_TREAT);

                        /*               //20151022 修改#/
                        struct PacketSubBloodPump SubBPump;
                        dataRead(PACKET_PDO_SUBBLOODPUMP,&SubBPump);
                        SubBPump.N10_set_speed=100;
                        dataNodeUpdate(PACKET_PDO_SUBBLOODPUMP, &SubBPump);
                        */

                        widgetButtonTreat.setText(tr("Treatment"));
                        widgetButtonTreat.setDisabled(false);//使按钮可以按#/
                        showYesDialog(DE_TR_PROMPT,DE_TR_CANBETREATED);

                        if(alarmmodule->Get_CurrentAlarmNum()==0)  //如当前有报警时，不屏蔽#/
                        {
                            alarmmodule->light(2);   //黄灯#/
                            alarmmodule->sound(2);
                       //     Alarm_sound_shielding=true;
                       //     Alarm_light_shielding=true;
                        }
                        isFlashShow=true;//闪烁治疗键#/
                    }
                }
                else
                {
                    isFlashShow=false;
                    Alarm_sound_shielding=false;
                    Alarm_light_shielding=false;
                }
            }
            else if(TREAT_FINISH == stateTMP.tipState.current){
                bool stateUptateFlag = true;//更改回血状态#/
                if(CURE_BLOODRETURN_RUN_NO_BLOOD == stateTMP.subState.current)stateTMP.subState.current = CURE_BLOODRETURN_RUN;
                else if(CURE_BLOODRETURN_STOP_NO_BLOOD == stateTMP.subState.current)stateTMP.subState.current = CURE_BLOODRETURN_STOP;
                else if(CURE_BLOODRETURN_RUN_DRAIN_NO_BLOOD == stateTMP.subState.current)stateTMP.subState.current = CURE_BLOODRETURN_RUN_DRAIN;
                else if(CURE_BLOODRETURN_STOP_DRAIN_NO_BLOOD == stateTMP.subState.current)stateTMP.subState.current = CURE_BLOODRETURN_STOP_DRAIN;
                else{stateUptateFlag = false;}
                if(stateUptateFlag)SetWorkStateManage(&stateTMP);//state need to update
            }
        }  // if(NoAlarm == bubble){//没有气泡报警#/
        else{  //有气泡报警 无血#/
            if(stateTMP.tipState.current == TREAT_READY){
                if(isFlashShow==true)
                {
                    if(Blood_online_lead==true&&preflush_model == online_model)
                        widgetButtonTreat.setText(DE_TR_ONLINECONNECTPT);
                    else
                        widgetButtonTreat.setText(tr("Connect P.t"));

                    if(getbloodstartflag==false)  //没开启过引血时,恢复预冲#/
                    {
                        widgetButtonPrime.setDisabled(false);//无血，开启预冲按钮#/
                        if(!bloodlevel)
                            stateTMP.subState.current = PREFLUSH_STOP_FLUID;
                        else
                            stateTMP.subState.current = PREFLUSH_STOP_NO_FLUID;
                    }
                    else
                    {
                        if(!bloodlevel)
                            stateTMP.subState.current = GETBLOOD_STOP_FLUID;
                        else
                            stateTMP.subState.current = GETBLOOD_STOP_NO_FLUID;
                    }
                    widgetButtonTreat.setChecked(false);
                    isFlashShow=false;
                    SetWorkStateManage(&stateTMP);
                }
            }
            else if(TREAT_FINISH == stateTMP.tipState.current){
                bool stateUptateFlag = true;//更改回血状态#/
                if(CURE_BLOODRETURN_RUN == stateTMP.subState.current)stateTMP.subState.current = CURE_BLOODRETURN_RUN_NO_BLOOD;
                else if(CURE_BLOODRETURN_STOP == stateTMP.subState.current)stateTMP.subState.current = CURE_BLOODRETURN_STOP_NO_BLOOD;
                else if(CURE_BLOODRETURN_RUN_DRAIN == stateTMP.subState.current)stateTMP.subState.current = CURE_BLOODRETURN_RUN_DRAIN_NO_BLOOD;
                else if(CURE_BLOODRETURN_STOP_DRAIN == stateTMP.subState.current)stateTMP.subState.current = CURE_BLOODRETURN_STOP_DRAIN_NO_BLOOD;
                else{stateUptateFlag = false;}
                if(stateTMP.subState.current == CURE_BLOODRETURN_STOP_NO_BLOOD||stateTMP.subState.current == CURE_BLOODRETURN_STOP_DRAIN_NO_BLOOD)
                    stateTMP.disinfectEnable = true;   //回血后要无血时可以进入消毒#/
                if(stateUptateFlag)SetWorkStateManage(&stateTMP);//state need to update
            }
        }

        /*
        if(bubble == HappenAlarm && workstate.current ==Cure_bloodreturn_run && check_alarm==true )
        {//有气泡报警#/

            MasterCtrol_CMD_BloodStop();
            workstate.current =Cure_stop;
            msgBox->setText(tr("Please confirm"));
            msgBox->setInformativeText(tr("Buble alarm ! \r\nContinut ReturnBlood ?"));
            msgBox->setDetailedText(tr("Differences here..."));

            msgBox->setModal(false);
            msgBox->setWindowFlags(Qt::WindowStaysOnTopHint);
            int ret=msgBox->exec();
            if(ret==QMessageBox::Yes)
              {
                check_alarm=false;
                MasterCtrol_CMD_BloodStart();
                workstate.current =Cure_bloodreturn_run;
              }
            else if(ret==QMessageBox::No)
             {
                workstate.current =Cure_bloodreturn_stop;
              }
         }            */
         return;
      }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetSwitch-ActionHandle-03");
#endif

    if (name==BLOOD_LEVEL_OK){//获取液位状态#/
        bloodlevel = *((int*) dataPtr);
        bool updataFlag = true;
        WorkState  stateTMP=GetWorkStateManage();
        if(!bloodlevel){//是否有液判断#/
            //有液#/
           if(SELFTEST_COMPLETE_NO_FLUID == stateTMP.subState.current)stateTMP.subState.current = SELFTEST_COMPLETE_FLUID;
           else if(PREFLUSH_RUN_NO_FLUID == stateTMP.subState.current)stateTMP.subState.current = PREFLUSH_RUN_FLUID;
           else if(PREFLUSH_STOP_NO_FLUID == stateTMP.subState.current)stateTMP.subState.current = PREFLUSH_STOP_FLUID;
           else if(PREFLUSH_COMPLETE_NO_FLUID == stateTMP.subState.current)stateTMP.subState.current = PREFLUSH_COMPLETE_FLUID;
           else if(TREAT_ENABLE_NO_FLUID == stateTMP.subState.current)stateTMP.subState.current = TREAT_ENABLE_FLUID;
           else if(GETBLOOD_RUN_NO_FLUID == stateTMP.subState.current)stateTMP.subState.current = GETBLOOD_RUN_FLUID;
           else if(GETBLOOD_STOP_NO_FLUID == stateTMP.subState.current)stateTMP.subState.current = GETBLOOD_STOP_FLUID;
           else{updataFlag = false;}
        }
        else{//无液#/
            if(SELFTEST_COMPLETE_FLUID == stateTMP.subState.current)stateTMP.subState.current = SELFTEST_COMPLETE_NO_FLUID;
            else if(PREFLUSH_RUN_FLUID == stateTMP.subState.current)stateTMP.subState.current = PREFLUSH_RUN_NO_FLUID;
            else if(PREFLUSH_STOP_FLUID == stateTMP.subState.current)stateTMP.subState.current = PREFLUSH_STOP_NO_FLUID;
            else if(PREFLUSH_COMPLETE_FLUID == stateTMP.subState.current)stateTMP.subState.current = PREFLUSH_COMPLETE_NO_FLUID;
            else if(TREAT_ENABLE_FLUID == stateTMP.subState.current)stateTMP.subState.current = TREAT_ENABLE_NO_FLUID;
            else if(GETBLOOD_RUN_FLUID == stateTMP.subState.current)stateTMP.subState.current = GETBLOOD_RUN_NO_FLUID;
            else if(GETBLOOD_STOP_FLUID == stateTMP.subState.current)stateTMP.subState.current = GETBLOOD_STOP_NO_FLUID;            
            else{updataFlag = false;}
        }
        if(updataFlag)SetWorkStateManage(&stateTMP);//state need to update#/
         return;
    }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetSwitch-ActionHandle-04");
#endif

    if (name==PACKET_PREFLUSH) {//更新预冲参数#/
        Preflush  = *((struct PacketPreflush*)dataPtr);
        WorkState  stateTMP=GetWorkStateManage();
        if(stateTMP.subState.current<= GETBLOOD_STOP_FLUID){//预冲中，需要更新下发参数#/
            struct PacketUF packetTmp;
            dataRead(PACKET_NAME_UF,&packetTmp);
            /*
            if(auto_model == preflush_model)packetTmp.ufRate =Preflush.PreflushUfRate;//自动预冲时，超虑率根据超滤量计算得到#/
            else{packetTmp.ufRate =Preflush.PreflushManualUF_Rate;} //手动预冲时，超虑率等于设置值#/
            */
            if(auto_model == Preflush.PreflushModel||online_model == Preflush.PreflushModel)packetTmp.preRate=Preflush.PreflushUfRate;//自动预冲时，超虑率根据超滤量计算得到#/
            else{packetTmp.preRate=Preflush.PreflushManualUF_Rate;} //手动预冲时，超虑率等于设置值#/
//            packetTmp.ufVolAim=Preflush.PreflushUfVolume;
            packetTmp.chooseRate=false;
            G_UF_SETRATE=packetTmp.preRate; //更新预冲参数时，UF速度#/
            dataNodeUpdate(PACKET_NAME_UF, &packetTmp);

            dataRead(PACKET_SDO_BLOOD,&sdoBlood);
            if(auto_model == Preflush.PreflushModel||online_model == Preflush.PreflushModel)sdoBlood.N3_set_BPspeed =Preflush.PreflushBloodSpeed;
            else{sdoBlood.N3_set_BPspeed =Preflush.PreflushManualBloodRate;}

            G_SET_BPSPEED=sdoBlood.N3_set_BPspeed;
            dataNodeUpdate(PACKET_SDO_BLOOD, &sdoBlood);//预冲设置#/
        }
        return;
    }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetSwitch-ActionHandle-05");
#endif

    if (name==PACKET_MAINTAIN_PARAMS) {//获取工程设置下的默认参数#/
    }

    /*
    if(name==PACKET_NAME_HEPARINDATA){ //肝素泵PDO#/
        struct PackN15PDO *packet=(struct PackN15PDO*)dataPtr;
        if(packet->workMode==0x01)
        {
            G_HEPARINPUMP_STATUS=true;
            if(widgetButtonHeparinPump.isEnabled()&&(!widgetButtonHeparinPump.isChecked()))
                widgetButtonHeparinPump.setChecked(true);
        }
        else if(packet->workMode==0x02||packet->workMode==0x03)
        {
            G_HEPARINPUMP_STATUS=false;
            if(widgetButtonHeparinPump.isEnabled()&&widgetButtonHeparinPump.isChecked())
                widgetButtonHeparinPump.setChecked(false);
        }
         return;
    }
    */

    /*
    if(strcmp(name,PACKET_STATUS_NOTE)==0){ //超虑泵和超虑按键的同步#/
        struct PacketStatusNote *packet=(struct PacketStatusNote*)dataPtr;
        WorkState  stateTMP=GetWorkStateManage();
        if(stateTMP.tipState.current==TREAT_READY||stateTMP.tipState.current==TREAT_RUNNING)
        {
            if(packet->note.note2&0x80)
            {
                if(G_UFPUMP_STATUS==false||(!widgetButtonUFPump.isChecked()))
                {
                    G_UFPUMP_STATUS=true;
                    widgetButtonUFPump.setChecked(true);
                }
            }
            else
            {
                if(G_UFPUMP_STATUS==true||widgetButtonUFPump.isChecked())
                {
                    G_UFPUMP_STATUS=false;
                    widgetButtonUFPump.setChecked(false);
                }
            }
        }
         return;
    }
    */

    if(name==PACKET_NAME_SUBBLOODPUMP){ //血泵2#/
        PacketBloodPump *packet = (struct PacketBloodPump*)dataPtr;
        PacketWorkMode workModeData;
        dataRead(PACKET_NAME_WORKMODE,&workModeData);
        PacketBloodPump blood1;
        dataRead(PACKET_NAME_BLOODPUMP,&blood1);
        //切换到online动作#/
        if(workModeData.treatmode.current == HDF_pre_dilute||workModeData.treatmode.current == HDF_post_dilute||\
           workModeData.treatmode.current == HF_pre_dilute||workModeData.treatmode.current == HF_post_dilute)
        {
            WorkState  stateTMP=GetWorkStateManage();
            if(BloodPump_link_flag==true&&stateTMP.subState.current != GETBLOOD_RUN_FLUID&&stateTMP.subState.current != GETBLOOD_RUN_NO_FLUID&&\
               CURE_BLOODRETURN_RUN != stateTMP.subState.current&&CURE_BLOODRETURN_RUN_NO_BLOOD != stateTMP.subState.current)
            {
                if(packet->N9_mode==BLOOD_PUMP_RUN&&blood1.N9_mode==BLOOD_PUMP_STOP)
                {
                    BP_start();
                }
            }
            else
            {
                BloodPump_link_flag=true;
            }
        }
        else
        {
            BloodPump_link_flag=false;
        }

        /*
        if(packet->N9_status&0x04&&Alarm_SubsPump_Flag==true)
        {
        //    alarmmodule->Afresh_Alarm();
            Alarm_SubsPump_Flag=false;
        }
        */
    }

    int temp=0;   
    if (name==PACKET_NAME_BLOODPUMP) {//血泵PDO更新#/
        struct PacketBloodPump *packet = (struct PacketBloodPump*)dataPtr;
        WorkState  stateTMP=GetWorkStateManage();

        if((TREAT_READY == stateTMP.tipState.current)&&(stateTMP.subState.current>=SELFTEST_COMPLETE_FLUID)){//准备状态#/
/*
            if(BLOOD_PUMP_RUN == packet->N9_mode)
            {
                if(((PREFLUSH_RUN_FLUID != stateTMP.subState.current)&&(PREFLUSH_RUN_NO_FLUID != stateTMP.subState.current)\
                   &&(stateTMP.subState.current<=PREFLUSH_COMPLETE_NO_FLUID))&&getbloodstartflag==false)
                {
                    if(bubble)  //无血的侍候才联动#/
                    {
                        prime_run(preflush_model);//血泵与预冲联动#/
                    }
                }
                else if((GETBLOOD_STOP_FLUID == stateTMP.subState.current)&&(GETBLOOD_STOP_NO_FLUID == stateTMP.subState.current)&&getbloodstartflag==true)
                {
                    if(bubble)  //无血的侍候才联动#/
                    {
                        getblood_run();
                    }
                }
                else if((TREAT_ENABLE_FLUID == stateTMP.subState.current)&&(TREAT_ENABLE_NO_FLUID == stateTMP.subState.current))
                {
                    if(NoAlarm == bubble)  //有血#/
                    {
                        BP_stop();     //先停止血泵，再询问是否进入治疗#/
                        showNoteDialog("Cure Mode");//弹出确认启动的治疗模式#/
                    }
                }
            }  // if(BLOOD_PUMP_RUN == packet->N9_mode)


            if(BLOOD_PUMP_STOP == packet->N9_mode)
            {
                if(((PREFLUSH_RUN_FLUID == stateTMP.subState.current)||(PREFLUSH_RUN_NO_FLUID == stateTMP.subState.current))\
                   &&getbloodstartflag==false)
                {
                    if(bubble)  //无血的侍候才联动#/
                    {
                        prime_stop(preflush_model,PRIME_STOP_FROM_PUMP);//血泵停止，暂停预冲#/
                    }
                }
                else if(((GETBLOOD_RUN_FLUID == stateTMP.subState.current)||(GETBLOOD_RUN_NO_FLUID == stateTMP.subState.current))&&getbloodstartflag==true)
                {
                    if(bubble)  //无血的侍候才联动#/
                    {
                        getblood_stop();
                    }
                }
            }
*/
            if((PREFLUSH_RUN_FLUID == stateTMP.subState.current)||(PREFLUSH_RUN_NO_FLUID == stateTMP.subState.current))
            {                    
                if(packet->N9_amount > static_BP_amount)
                {
                    temp = packet->N9_amount - static_BP_amount;//预冲量#/
                    BloodPump_Volume=temp;
                }
                else if(G_rebirth_flag!=true)
                {
                    static_BP_amount=packet->N9_amount;
                }

                if(preflush_model==auto_model||preflush_model==online_model){//计算预冲量，修改血泵流速#/

                    if (temp >= Preflush.TractBloodVolume ) //预冲量到，预冲停#/
                    {
                        BloodPump_Pre_Amount+=Preflush.TractBloodVolume;
                        BloodPump_Volume=0;
                        if(G_UF_PREFINISH!=0)
                        {
                       //     UFPump_Pre_Amount += G_UF_PREFINISH;   //预冲超滤量累计#/
                            G_UF_PREFINISH=0;
                            static_UF_amount=packetUfValue.N3_UF_amount;   //预冲量到，重计超滤量#/
                        }
                        static_BP_amount=packet->N9_amount;//记录血泵累计量
                        prime_stop(preflush_model,PRIME_STOP_FROM_COMPLETE);
                    }
                    else if((temp  >= Preflush.TractBloodVolume/3) && Preflush.step==1)
                    {
                      Preflush.step=2;
                      dataRead(PACKET_SDO_BLOOD,&sdoBlood);
                      sdoBlood.N3_set_BPspeed =Preflush.BPRate.ph2;
                      G_SET_BPSPEED=sdoBlood.N3_set_BPspeed;
                      dataNodeUpdate(PACKET_SDO_BLOOD, &sdoBlood); //预冲中#/

                    }else if((temp  >= Preflush.TractBloodVolume/3*2) && Preflush.step==2)
                     {
                          Preflush.step=3;
                          dataRead(PACKET_SDO_BLOOD,&sdoBlood);
                          sdoBlood.N3_set_BPspeed =Preflush.BPRate.ph3;
                          G_SET_BPSPEED=sdoBlood.N3_set_BPspeed;
                          dataNodeUpdate(PACKET_SDO_BLOOD, &sdoBlood);  //预冲中#/
                     }
                }
            }
            else if(PREFLUSH_STOP_FLUID != stateTMP.subState.current\
                    &&PREFLUSH_STOP_NO_FLUID != stateTMP.subState.current)
            {
                if(G_rebirth_flag!=true)
                    static_BP_amount=packet->N9_amount;//记录血泵累计量
            }
        }//有可能改变状态；#/
        else if(stateTMP.tipState.current==TREAT_RUNNING)  //治疗过程中#/
        {
            if(packet->N9_mode==BLOOD_PUMP_RUN)
            {
                if(G_BPPUMP_STATUS==false)   //消除报警，重新启动血泵时#/
                {
                   BP_start();

                   if(prestatusUFPump==true)  //血泵状态变化处理--上一次超滤泵打开标示#/
                   {
                     UF_start();    //启动超滤泵#/
                   }
                   if(prestatusHeparinPump==true)
                   {
                     HeparinPump_run();//启动肝素泵#/
                   }

                   handle_count=0;
                   S_Delay_Count=5;
                }

                if(packet->N9_amount > static_BP_amount)
                {
                    BloodPump_Volume = packet->N9_amount - static_BP_amount;
                }
                else if(G_rebirth_flag!=true)
                {
                    static_BP_amount=packet->N9_amount;
                }
            }
            //异常导致的血泵停止---#/
            if(BLOOD_PUMP_STOP == packet->N9_mode&&stateTMP.subState.current==UF_TIME_NOT_COMPLETE)
            {
                if(G_BPPUMP_STATUS==true)
                {
                    widgetButtonUFPump.setChecked(false);
                    widgetButtonHeparinPump.setChecked(false);

#if HJX_COMMUNICATION_SWITCH
              //      emit signalSDO(DE_CMD_SubsStop);
                    SUB_stop();   //停止置换泵#/
                    emit signalSDO(DE_CMD_ClipStop);
//                    emit signalSDO(DE_CMD_Clip2Stop);
                    HeparinPump_stop();//停止肝素泵#/
                    UF_stop();
#else
                    MasterCtrol_CMD_SubsStop();  //置换泵#/
                    us_sleep(10000);
                    HeparinPump_stop();//停止肝素泵#/
                    us_sleep(1000);
                    UF_stop();
                    MasterCtrol_CMD_ClipStop();
                    us_sleep(100000);
                    MasterCtrol_CMD_Clip2Stop();
                    us_sleep(100000);
#endif
                //    showYesDialog(DE_TR_PROMPT,DE_TR_HAPPENALARMONCURE); //暂时不需要#/
                    G_BPPUMP_STATUS=false;
                    handle_count=0;
                    S_Delay_Count=5;
                }
            }
        }
        else if(TREAT_FINISH == stateTMP.tipState.current){  //治疗结束#/

            /*
            if((BLOOD_PUMP_RUN == packet->N9_mode)&&( DRAIN_RUN == stateTMP.subState.current)){
                //排空与血泵不能同时启动#/
                DRY_stop();//停止排空#/
                BP_stop();  //停止血泵#/
                stateTMP.subState.current = DRAIN_STOP;
                SetWorkStateManage(&stateTMP);
            }
            else if((BLOOD_PUMP_RUN == packet->N9_mode)&&((CURE_BLOODRETURN_STOP == stateTMP.subState.current)\
                    || (CURE_BLOODRETURN_STOP_NO_BLOOD == stateTMP.subState.current)\
                    ||(CURE_BLOODRETURN_STOP_DRAIN == stateTMP.subState.current)\
                    ||(DRAIN_STOP == stateTMP.subState.current)\
                    ||(CURE_BLOODRETURN_STOP_DRAIN_NO_BLOOD == stateTMP.subState.current))){//血泵运行
                    if(blockbloodflag>0)
                        blockbloodflag=0;
                    else
                        ClickedPreShoot();//与血泵联动#/
            }
            else if((BLOOD_PUMP_STOP == packet->N9_mode)&&((CURE_BLOODRETURN_RUN == stateTMP.subState.current)\
                    ||(CURE_BLOODRETURN_RUN_NO_BLOOD == stateTMP.subState.current)\
                    ||(CURE_BLOODRETURN_RUN_DRAIN == stateTMP.subState.current)\
                    || (CURE_BLOODRETURN_RUN_DRAIN_NO_BLOOD == stateTMP.subState.current))){//血泵停止，
                if(blockbloodflag>0)
                    blockbloodflag=0;
                else
                    ClickedPreShoot();//与血泵联动#/
            }
            */
        }

        PacketBloodPump blood2;
        dataRead(PACKET_NAME_SUBBLOODPUMP,&blood2);
        if(BloodPump_link_flag==true&&stateTMP.subState.current != GETBLOOD_RUN_FLUID&&stateTMP.subState.current != GETBLOOD_RUN_NO_FLUID&&\
           stateTMP.subState.current<CURE_BLOODRETURN_RUN)
        {
//            if(packet->N9_mode==BLOOD_PUMP_RUN&&blood2.N9_mode==BLOOD_PUMP_STOP&&Waterway_alarm_flag==false&&statusSideWay==false)
            if(packet->N9_mode==BLOOD_PUMP_RUN&&blood2.N9_mode==BLOOD_PUMP_STOP&&Waterway_alarm_flag==false)
            {
#if HJX_COMMUNICATION_SWITCH
        //        emit signalSDO(DE_CMD_SubsStart);  //血泵PDO更新时，同步开启#/
                SUB_start();  //启动置换泵#/
#else
                us_sleep(10000);
                MasterCtrol_CMD_SubsStart();//启动血泵2#/
#endif
            }
            else if(packet->N9_mode==BLOOD_PUMP_STOP&&blood2.N9_mode==BLOOD_PUMP_RUN)
            {
#if HJX_COMMUNICATION_SWITCH
       //         emit signalSDO(DE_CMD_SubsStop);
                SUB_stop();   //停止置换泵#/
#else
                us_sleep(10000);
                MasterCtrol_CMD_SubsStop();
#endif
            }
        }
/*
        if(packet->N9_mode==BLOOD_PUMP_RUN&&Alarm_BloodPump_Flag==true)
        {
            alarmmodule->Afresh_Alarm();
        }
        */

        /*
        if((packet->N9_status&0x04)&&Alarm_BloodPump_Flag==true)  //启动键按下时
        {
        //    alarmmodule->Afresh_Alarm();
            Alarm_BloodPump_Flag=false;
        }
        */

        dataRead(PACKET_STATUS_ALARM,&alarmState);
        if((packet->N9_mode)==0x01)  //血泵运行时#/
        {
            //if(workstate.tip==Cure_run)
            //alarmState
            /*
            if(alarmState.alarm.Iostatus&0x18)  //动静脉阻流夹关闭#/
            {
#if HJX_COMMUNICATION_SWITCH
                emit signalSDO(DE_CMD_ClipStart);
                emit signalSDO(DE_CMD_Clip2Start);
#else
                MasterCtrol_CMD_ClipStart();  //开启静脉阻流夹#/
                usleep(100000);
                MasterCtrol_CMD_Clip2Start(); //开启动脉#/
                usleep(100000);
#endif
            }
            */
            if(alarmState.alarm.Iostatus&0x08)  //静脉阻流夹关闭#/
            {
#if HJX_COMMUNICATION_SWITCH
                emit signalSDO(DE_CMD_ClipStart);
#else
                MasterCtrol_CMD_ClipStart();  //开启静脉阻流夹#/
                usleep(100000);
#endif
            }
        }
        else if((packet->N9_mode)==0x02)  //血泵停止时#/
        {
            /*
            if((alarmState.alarm.Iostatus&0x18)!=0x18)  //阻流夹打开时#/
            {
#if HJX_COMMUNICATION_SWITCH
                emit signalSDO(DE_CMD_ClipStop);
                emit signalSDO(DE_CMD_Clip2Stop);
#else
                MasterCtrol_CMD_ClipStop();
                usleep(100000);
                MasterCtrol_CMD_Clip2Stop();
                usleep(100000);
#endif
            }
            */
            if((alarmState.alarm.Iostatus&0x08)==0x00)  //阻流夹打开时#/
            {
#if HJX_COMMUNICATION_SWITCH
                emit signalSDO(DE_CMD_ClipStop);
#else
                MasterCtrol_CMD_ClipStop();
                usleep(100000);
#endif
            }
        }
        return;
    }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetSwitch-ActionHandle-06");
#endif

    if (name==PACKET_NAME_WORKMODE) {//改变治疗模式#/
        workModeData = *(PacketWorkMode *)dataPtr;
        WorkState  stateTMP=GetWorkStateManage();
        if((workModeData.flag==setok_state)&&(workModeData.treatmode.current==workModeData.treatmode.target))
        {
            //切换到online动作#/
            if(workModeData.treatmode.current == HDF_pre_dilute||workModeData.treatmode.current == HDF_post_dilute||\
               workModeData.treatmode.current == HF_pre_dilute||workModeData.treatmode.current == HF_post_dilute)
            {
                BloodPump_link_flag=true;
                if(stateTMP.tipState.current==TREAT_RUNNING)
                {
                    BP_start();    //启动血泵#/
//                    UF_stop();     //停止超滤泵#/
                }
            }
            else
            {
                 BloodPump_link_flag=false;
#if HJX_COMMUNICATION_SWITCH
           //      emit signalSDO(DE_CMD_SubsStop);
                 SUB_stop();   //停止置换泵#/
#else
                 us_sleep(10000);
                 MasterCtrol_CMD_SubsStop();
#endif
            }

            if(workModeData.treatmode.current==PE)
            {
                UF_stop();     //停止超滤泵#/
            }

            //切换到单超动作#/
            if(workModeData.treatmode.current == ISOUF_SNSP||workModeData.treatmode.current == ISOUF_SNDP||\
                    workModeData.treatmode.current == ISOUF_DNDP)
            {
                if(stateTMP.tipState.current==TREAT_RUNNING&&G_UFPUMP_STATUS==false)
                {
                    UF_start();
                }
            }
            if(stateTMP.tipState.current==TREAT_RUNNING)
            {
            workModeData.flag=runing_state;
   //         dataNodeUpdate(PACKET_NAME_WORKMODE,&workModeData);
            dataWrite(PACKET_NAME_WORKMODE,&workModeData);

#if HJX_COMMUNICATION_SWITCH
            emit signalSDO(DE_CMD_Treat_start);
#else
            MasterCtrol_CMD_Treat_start(workModeData.treatmode.current);
#endif
            }
        }
        return;
    }

    if (name==TREAT_TIMER)//治疗时间到#/
    {
         Timer_state = *((int*) dataPtr);
         WorkState  stateTMP=GetWorkStateManage();
         if(Timer_state==Timestop && TREAT_RUNNING == stateTMP.tipState.current)
         {//应该改变超滤速度，时间继续往负数走#/
            //treatment_stop();
            showNoteDialog("CureFinish");//弹出是否结束治疗提示框#/
         }
         return;
    }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetSwitch-ActionHandle-07");
#endif

    if (name==PACKET_MASTERCTRL_UFVALUE) {//超滤量更新#/
        packetUfValue = *(PacketUfValue*) dataPtr;

        WorkState  stateTMP=GetWorkStateManage();
        if(stateTMP.tipState.current==TREAT_READY)    //预冲阶段超滤量处理#/
        {
            if(stateTMP.subState.current>=PREFLUSH_RUN_FLUID&&stateTMP.subState.current<TREAT_ENABLE_FLUID)
            {
                if((packetUfValue.N3_UF_amount-static_UF_amount)>0)   //预冲超滤比较#/
                {
                    G_UF_PREFINISH=packetUfValue.N3_UF_amount-static_UF_amount;
                }
                else if(G_rebirth_flag!=true)
                {
                    static_UF_amount=packetUfValue.N3_UF_amount;
                }

                if(stateTMP.subState.current>=PREFLUSH_RUN_FLUID&&stateTMP.subState.current<PREFLUSH_COMPLETE_FLUID&&preflush_model==auto_model)
                {
                     //超滤速度和时间同步#/
          //          UF_SYNTIME_HANDLE(G_UF_STARTTIME,G_SYS_CURTIME,G_UF_ENDTIME,G_UF_PREFINISH,G_UF_PRETARGET,G_UF_SETRATE);

                    //判断预冲超滤量是否已经达到#/
                    if(G_UF_PREFINISH>=G_UF_PRETARGET)
                    {
                    //    UFPump_Pre_Amount += G_UF_PRETARGET;
                        G_UF_PREFINISH=0;
                        static_UF_amount=packetUfValue.N3_UF_amount;
                        G_UF_FINISHVOL_FLAG=true;//预冲超滤量已到#/
                        UF_stop();     //停止超滤泵#/
                    }
                }
            }
            else
            {
                if(G_rebirth_flag!=true)
                    static_UF_amount=packetUfValue.N3_UF_amount;
            }
        }
        else if(stateTMP.tipState.current==TREAT_RUNNING)  //治疗阶段，超滤量的处理#/
        {
      //      PacketUF packetPDO;
 //           dataRead(PACKET_NAME_UF,&packetPDO);

            PacketWorkMode workModeData;
            dataRead(PACKET_NAME_WORKMODE,&workModeData);

            if(G_UFPUMP_STATUS ==true)
            {
                if((packetUfValue.N3_UF_amount-static_UF_amount)>0)
                {
                    if(G_SAVECUREPARAM_FLAG==0)  //未保存上一次治疗参数的时候#/
                        G_UF_CUREFINISH=packetUfValue.N3_UF_amount-static_UF_amount;
                    else if(G_SAVECUREPARAM_FLAG==1)
                        G_UF_CUREFINISH=(packetUfValue.N3_UF_amount-static_UF_amount)+UFPump_Cure_Amount;//保存参数后，进行超滤量累计#/
                }
                else if(G_rebirth_flag!=true)
                {
                    static_UF_amount=packetUfValue.N3_UF_amount;
                }


                if(workModeData.treatmode.current == ISOUF_SNSP||workModeData.treatmode.current == ISOUF_SNDP||\
                   workModeData.treatmode.current == ISOUF_DNDP)//单超模式#/
                {

                    ISO_finishvolume=G_UF_CUREFINISH-UFPump_Now_volume;

                    if(ISO_finishvolume>=ISO_totalvolume&&G_UF_FINISHVOL_FLAG==false)  //ISO的超滤目标量到#/
                    {
                        G_UF_FINISHVOL_FLAG=true;  //ISO治疗模式下，超滤量已到#/
                        UF_stop();
                    }

                    //超滤速度和时间同步#/
              //      UF_SYNTIME_HANDLE(G_UF_STARTTIME,G_SYS_CURTIME,G_UF_ENDTIME,ISO_finishvolume,ISO_totalvolume,G_UF_SETRATE);
                }
                else //其他治疗模式下#/
                {
                    //超滤速度和时间同步#/
             //       UF_SYNTIME_HANDLE(G_UF_STARTTIME,G_SYS_CURTIME,G_UF_ENDTIME,G_UF_CUREFINISH,G_UF_CURETARGET,G_UF_SETRATE);
                    if(G_UF_CUREFINISH>=G_UF_CURETARGET&&G_UF_FINISHVOL_FLAG==false) //其他治疗模式下，超滤量到达时#/
                    {
                        G_UF_FINISHVOL_FLAG=true;  //ISO外其他治疗模式下，超滤量已到#/
                      //  if(G_UFPUMP_STATUS == true)
                            UF_stop();

                        prestatusUFPump=false;  //关闭超滤泵标示#/
                    }
                }
            }  //if(G_UFPUMP_STATUS ==true)#/
        }
        else if(stateTMP.tipState.current==TREAT_FINISH)
        {
            static_UF_amount=packetUfValue.N3_UF_amount;
        }
        return;
    }

    /*
    if (strcmp(name, PACKET_STATUS_MASTALARM) == 0) {//A/B液判断#/
    }
    */

    //本机的提示报警信息#/
    if (name==OWNNOTE){//报警分析#/
        PacketOwnNote alarm_own= *(PacketOwnNote *) dataPtr;
        alarm_own.note.note1 |=alarm_own.note.note1;
       if((alarm_own.note.note1&0x01)==0x01)//肝素泵时间到，或者停止#/
        HeparinPump_stop();
    }
    else if(name==PACKET_REBIRTH_START)//重生恢复开始#/
    {
        L_workstate=G_P_REBIRTH->workstate;   //工作状态设置#/
        SetWorkStateManage(&L_workstate);//恢复运行状态#/
        G_SAVECUREPARAM_FLAG=G_P_REBIRTH->G_SAVECUREPARAM_FLAG; //返回准备状态时，是否保存治疗参数 0：不保存 1保存#/

        static_BP_amount=G_P_REBIRTH->static_BP_amount;//血泵流量计算基数#/
        static_UF_amount=G_P_REBIRTH->static_UF_amount;  //UF流量计算基数#/
        BloodPump_Pre_Amount=G_P_REBIRTH->BloodPump_Pre_Amount;   //血泵预冲累计量#/
        BloodPump_Cure_Amount=G_P_REBIRTH->BloodPump_Cure_Amount;  //血泵治疗累计量#/

        G_UF_PRETARGET=G_P_REBIRTH->G_UF_PRETARGET; //预冲阶段超滤目标量#/
        G_UF_CURETARGET=G_P_REBIRTH->G_UF_CURETARGET;  //治疗阶段目标超滤量#/
        G_UF_STARTTIME=G_P_REBIRTH->G_UF_STARTTIME;    //超滤开启时间 单位秒#/
        G_UF_ENDTIME=G_P_REBIRTH->G_UF_ENDTIME;        //超滤的结束时间#/
        G_UF_SETRATE=G_P_REBIRTH->G_UF_SETRATE;        //UF的设置速度#/

        prestatusUFPump=G_P_REBIRTH->prestatusUFPump;  //上一次超滤泵状态#/
        prestatusHeparinPump=G_P_REBIRTH->prestatusHeparinPump;
        prestatusSideWay=G_P_REBIRTH->prestatusSideWay;

        if(L_workstate.tipState.current<=TREAT_FINISH&&L_workstate.subState.current>= SELFTEST_COMPLETE_FLUID)
        {
            widgetButtonPrime.setDisabled(false);
            widgetButtonTreat.setDisabled(false);
            widgetButtonUFPump.setDisabled(false);
            if(G_HeparinEnable_flag==true)
                widgetButtonHeparinPump.setDisabled(false);
            widgetButtonBypass.setDisabled(false);

            waterroadOk = 1;
            bloodroadOk = 1;//水路和血路正常#/

            S_Clear_flag=true;  //不清累计量#/

            if(L_workstate.tipState.current==TREAT_READY)  //准备状态#/
            {
                if (preflush_model == auto_model)widgetButtonPrime.setText(tr("Auto Prime"));//预冲按钮的模式
                else if(preflush_model == online_model)widgetButtonPrime.setText(DE_TR_ONLINEPRIME);
                else if ( (preflush_model == manual_model))widgetButtonPrime.setText(tr("Manual Prime"));

                  if((PREFLUSH_RUN_FLUID == L_workstate.subState.current)||(PREFLUSH_RUN_NO_FLUID == L_workstate.subState.current))
                  {
                    widgetButtonPrime.setChecked(true);//按键置于按下状态#/
                  }
            }
            else if(L_workstate.tipState.current==TREAT_RUNNING)  //治疗状态#/
            {
                G_UF_CURERESIDUE=G_P_REBIRTH->G_UF_CURERESIDUE;  //治疗剩余的超滤量#/
                G_UF_FINISHVOL_FLAG=G_P_REBIRTH->G_UF_FINISHVOL_FLAG;    //超滤泵已完成本次超滤目标的标志#/
                G_UF_PREFINISH=G_P_REBIRTH->G_UF_PREFINISH;   //实时已预冲超滤量#/
                G_UF_CUREFINISH=G_P_REBIRTH->G_UF_CUREFINISH;  //治疗阶段已完成超滤量#/
                UFPump_Pre_Amount=G_P_REBIRTH->UFPump_Pre_Amount;   //超滤泵预冲累计量#/
                UFPump_Cure_Amount=G_P_REBIRTH->UFPump_Cure_Amount;   //超滤泵治疗累计量#/

                G_BYPASS_BUT_FLAG=G_P_REBIRTH->G_BYPASS_BUT_FLAG;  //旁路按键标志#/
                G_CURE_STARTTIME=G_P_REBIRTH->G_CURE_STARTTIME;  //治疗开始时间#/
                G_CURE_ENDTIME=G_P_REBIRTH->G_CURE_ENDTIME;    //治疗结束时间#/

                widgetButtonTreat.setChecked(true);
                widgetButtonTreat.setText(tr("Treatment"));

                alarmmodule->light(0);   //close light and sound#/
                alarmmodule->sound(0);

                if(Blood_online_return==true&&preflush_model == online_model)
                    widgetButtonPrime.setText(DE_TR_ONLINERETURNBLOOD);
                else
                    widgetButtonPrime.setText(tr("Return Blood"));//预冲键变为回血键#/
            }
            else if(L_workstate.tipState.current==TREAT_FINISH)
            {
                widgetButtonTreat.setChecked(false);
                widgetButtonUFPump.setChecked(false);
                widgetButtonHeparinPump.setChecked(false);

                if(((CURE_BLOODRETURN_RUN == L_workstate.subState.current)\
                        ||(CURE_BLOODRETURN_RUN_NO_BLOOD == L_workstate.subState.current)\
                        ||(CURE_BLOODRETURN_RUN_DRAIN == L_workstate.subState.current)\
                        || (CURE_BLOODRETURN_RUN_DRAIN_NO_BLOOD == L_workstate.subState.current))){
                    widgetButtonPrime.setChecked(true);//按键置于按下状态#/
                }

                if(Blood_online_return==true&&preflush_model == online_model)
                    widgetButtonPrime.setText(DE_TR_ONLINERETURNBLOOD);
                else
                    widgetButtonPrime.setText(tr("Return Blood"));//预冲键变为回血键#/
                widgetButtonTreat.setText(tr("Ready"));
                widgetButtonUFPump.setText(tr("Drain"));

                UF_stop();
                #if HJX_COMMUNICATION_SWITCH
                    emit signalSDO(DE_CMD_Treat_stop);
                #else
                    MasterCtrol_CMD_Treat_stop();
                #endif
                widgetButtonUFPump.setDisabled(true);  //不可用，回血后方可使用#/
                scaleVar = treatment_stop_to_scale;
                dataNodeUpdate(PACKET_NAME_SCALE_AP, &scaleVar);
                dataNodeUpdate(PACKET_NAME_SCALE_VP, &scaleVar);
                dataNodeUpdate(PACKET_NAME_SCALE_TMP, &scaleVar);
            }
        }
        else
        {
            widgetButtonPrime.setDisabled(true);
            widgetButtonTreat.setDisabled(true);
            widgetButtonUFPump.setDisabled(true);
            widgetButtonHeparinPump.setDisabled(true);
            widgetButtonBypass.setDisabled(true);

            if(L_workstate.tipState.current==CLEANING)
            {
                PageShow("Disinfect"); //进入消毒#/
            }
        }
    } // if(name==PACKET_REBIRTH_START)//重生恢复开始#/



#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetSwitch-ActionHandle-out");
#endif
}

void WidgetSwitch::prime_run( const char model)
{//启动预冲   #/
    dataRead(PACKET_PREFLUSH, &Preflush);
    if(model==auto_model||model==online_model){//处于自动预冲模式#/
        struct PacketUF packetTmp;
        dataRead(PACKET_NAME_UF,&packetTmp);        
        packetTmp.preRate = Preflush.PreflushUfRate;
        packetTmp.chooseRate = false;               //选用预冲时的超虑速度#/
        G_UF_SETRATE=packetTmp.preRate;  //开启预冲时 自动速度#/
        dataNodeUpdate(PACKET_NAME_UF, &packetTmp);                 //修改下发的超滤速度#/

        dataRead(PACKET_SDO_BLOOD,&sdoBlood);
        sdoBlood.N3_set_BPspeed = Preflush.BPRate.ph1;//100;//Preflush.PreflushBloodSpeed;#/
        G_SET_BPSPEED=sdoBlood.N3_set_BPspeed;
        dataNodeUpdate(PACKET_SDO_BLOOD, &sdoBlood);//开始预冲 下发血泵速度，更新label显示的速度#/
        Preflush.step=1;

        logInfo log;
        log.opration="Begin Auto Prime ";
        log.type=DE_LOGINFO_OPER;
        log.status=PDOstate.state.N1_status;
        log.note="Null";
        LogRun->addLog(log);
        
    }    
    if(model==manual_model){//手动预冲模式#/
        struct PacketUF packetTmp;
        dataRead(PACKET_NAME_UF,&packetTmp);
        packetTmp.preRate = Preflush.PreflushManualUF_Rate;
        packetTmp.chooseRate = false;           //选用预冲的超滤速度#/
        G_UF_SETRATE=packetTmp.preRate;  //开启预冲时 手动速度#/
        dataNodeUpdate(PACKET_NAME_UF, &packetTmp);                 //修改超滤速度为预冲的超滤速度#/

        dataRead(PACKET_SDO_BLOOD,&sdoBlood);
        sdoBlood.N3_set_BPspeed = Preflush.PreflushManualBloodRate;//100;//Preflush.PreflushBloodSpeed;#/
        G_SET_BPSPEED=sdoBlood.N3_set_BPspeed;
        dataNodeUpdate(PACKET_SDO_BLOOD, &sdoBlood);//开始预冲 下发血泵速度，更新label显示的速度#/

        logInfo log;
        log.opration="Begin Manual Prime ";
        log.type=DE_LOGINFO_OPER;
        log.status=PDOstate.state.N1_status;
        log.note="Null";
        LogRun->addLog(log);
    }

    BP_start();                                                    //启动血泵进行预冲#/

    /*
    PacketWorkMode workModeTMP;
    dataRead(PACKET_NAME_WORKMODE,&workModeTMP);
    if(workModeTMP.treatmode.current==HDF_pre_dilute||workModeTMP.treatmode.current==HDF_post_dilute\
       ||workModeTMP.treatmode.current==HF_pre_dilute||workModeTMP.treatmode.current==HF_post_dilute)
    {
        UF_stop();//online模式不启动超滤#/
        if(model==online_model)
            Startpreflush_mode=ONLINE_PREFLUSH_MODE;
    }
    else
    {
        UF_start();//非online模式启动超滤#/
    }
    */

    if(preflush_model==online_model)
        UF_stop();//online模式不启动超滤#/
    else
        UF_start();//非online模式启动超滤#/


    S_Delay_Count=4;

    if(model==auto_model)
        Startpreflush_mode=AUTO_PREFLUSH_MODE;
    else if(model==manual_model)
        Startpreflush_mode=MANUAL_PREFLUSH_MODE;
    else if(model==online_model)
        Startpreflush_mode=ONLINE_PREFLUSH_MODE;

#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_Startpreflush);
#else
        us_sleep(100000);
        MasterCtrol_CMD_Startpreflush(Startpreflush_mode);//启动预冲#/
        us_sleep(10000);
#endif

    if(model==auto_model||model==online_model){
    timeCount=0;    //启动预冲#/
    //timer->createTimer(this,1000);
    }
    widgetButtonPrime.setChecked(true);
    WorkState  stateTMP=GetWorkStateManage();

    //除了预冲暂停外，其他的状态算是预冲重新开始#/
    if(stateTMP.subState.current!=PREFLUSH_STOP_FLUID&&stateTMP.subState.current!=PREFLUSH_STOP_NO_FLUID\
      &&stateTMP.subState.current!=PREFLUSH_RUN_FLUID&&stateTMP.subState.current!=PREFLUSH_RUN_NO_FLUID)
    {
        G_UF_FINISHVOL_FLAG=false;
//        UFPump_Pre_Amount +=  G_UF_PREFINISH;
        G_UF_PREFINISH=0;
        G_UF_STARTTIME=G_SYS_CURTIME;      //超滤开启时间 单位秒#/
        G_UF_ENDTIME=G_SYS_CURTIME+Preflush.PreflushTime*60;  //超滤的总时间 单位秒#/
        if(model==manual_model)
            G_UF_SETRATE=Preflush.PreflushManualUF_Rate; //预冲重新开始#/
        else
            G_UF_SETRATE=Preflush.PreflushUfRate; //预冲重新开始#/

        static_UF_amount=packetUfValue.N3_UF_amount;
    }

    if(!bloodlevel)stateTMP.subState.current = PREFLUSH_RUN_FLUID;
    else{stateTMP.subState.current = PREFLUSH_RUN_NO_FLUID;}
    stateTMP.disinfectEnable = false;
    SetWorkStateManage(&stateTMP);//子状态改为预冲运行状态#/
 }

void WidgetSwitch::prime_stop(const char model,const char primeState)
{
    if(model==auto_model)
    {
        logInfo log;
        log.opration="Stop Auto Prime ";
        log.type=DE_LOGINFO_OPER;
        log.status=PDOstate.state.N1_status;
        log.note="Null";
        LogRun->addLog(log);
    }
    else if(model==manual_model)
    {
        logInfo log;
        log.opration="Stop Manual Prime ";
        log.type=DE_LOGINFO_OPER;
        log.status=PDOstate.state.N1_status;
        log.note="Null";
        LogRun->addLog(log);
    }

    BP_stop();//停止血泵#/
    UF_stop();//停止 超滤泵#/
    S_Delay_Count=4;

    widgetButtonPrime.setChecked(false);//恢复预冲键未选中状态#/
    WorkState  stateTMP=GetWorkStateManage();
    switch(primeState){
    case PRIME_STOP_FROM_COMPLETE://预冲完成#/
        if(!bloodlevel)stateTMP.subState.current = PREFLUSH_COMPLETE_FLUID;//有液#/
        else{stateTMP.subState.current = PREFLUSH_COMPLETE_NO_FLUID;}
        break;
    case PRIME_STOP_FROM_PUMP://预冲暂停#/
        if(!bloodlevel)stateTMP.subState.current = PREFLUSH_STOP_FLUID;//有液#/
        else{stateTMP.subState.current = PREFLUSH_STOP_NO_FLUID;}
        break;
    case PRIME_STOP_FROM_BLOOD://有血，预冲停止#/
        if(!bloodlevel)stateTMP.subState.current = TREAT_ENABLE_FLUID;//有液#/
        else{stateTMP.subState.current = TREAT_ENABLE_NO_FLUID;}
        break;
    }    

#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_Stoppreflush);
#else
        us_sleep(100000);
        MasterCtrol_CMD_Stoppreflush();//停止预冲
        us_sleep(10000);
#endif

    stateTMP.disinfectEnable = true;//禁止进入消毒界面#/
    SetWorkStateManage(&stateTMP);//子状态改为预冲停止状态#/
}

void WidgetSwitch::BP_start(void)
{//启动血泵#/
        G_BPPUMP_STATUS = true;

        WorkState  stateTMP=GetWorkStateManage();

#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_BloodStart);
        emit signalSDO(DE_CMD_ClipStart);
        emit signalSDO(DE_CMD_Clip2Start);

        if(stateTMP.tipState.current==TREAT_READY)
        {
            /*
            if(BloodPump_link_flag==true&&getbloodstartflag==false&&preflush_model==online_model)  //引血时不开血泵2#/
            {
            emit signalSDO(DE_CMD_SubsStart);
            }
            */
            if(BloodPump_link_flag==true&&(stateTMP.subState.current == GETBLOOD_RUN_FLUID||stateTMP.subState.current == GETBLOOD_RUN_NO_FLUID))
            {
            //    emit signalSDO(DE_CMD_SubsStop);   //引血不开置换泵#/
                SUB_stop();   //停止置换泵#/
            }
//            else if(BloodPump_link_flag==true && statusSideWay==false)
            else if(BloodPump_link_flag==true)
            {
        //        emit signalSDO(DE_CMD_SubsStart);  //BP_start( )中准备状态开启同步#/
                SUB_start();  //启动置换泵#/
            }
        }
        else if(stateTMP.tipState.current==TREAT_RUNNING)
        {
        //    if(BloodPump_link_flag==true && statusSideWay==false)
            if(BloodPump_link_flag==true)
            {
            //emit signalSDO(DE_CMD_SubsStart);   //BP_start( )中治疗状态开启同步#/
                SUB_start();  //启动置换泵#/
            }
        }
        else if(stateTMP.tipState.current==TREAT_FINISH)
        {
            //回血不开置换泵#/
           if((stateTMP.subState.current>=CURE_BLOODRETURN_RUN)&&(stateTMP.subState.current<=CURE_BLOODRETURN_STOP_DRAIN_NO_BLOOD))
           {
           //     emit signalSDO(DE_CMD_SubsStop);
               SUB_stop();   //停止置换泵#/
           }
        }
#else
        us_sleep(100000);
        MasterCtrol_CMD_BloodStart();
        us_sleep(10000);
        MasterCtrol_CMD_ClipStart();//阻流夹#/
        us_sleep(10000);
        MasterCtrol_CMD_Clip2Start();//阻流夹#/
        us_sleep(1000);

        if(stateTMP.tipState.current==TREAT_READY)
        {
            if(BloodPump_link_flag==true&&getbloodstartflag==false&&preflush_model==online_model)  //引血时不开血泵2#/
            {
                us_sleep(1000);
                MasterCtrol_CMD_SubsStart();//启动血泵2#/
            }
        }
        else
        {
            if(BloodPump_link_flag==true)  //引血时不开血泵2#/
            {
                us_sleep(1000);
                MasterCtrol_CMD_SubsStart();//启动血泵2#/
            }
        }
#endif
}

void WidgetSwitch::BP_stop(void)
{//停止血泵#/
        G_BPPUMP_STATUS = false;
#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_BloodStop);
        emit signalSDO(DE_CMD_ClipStop);
//        emit signalSDO(DE_CMD_Clip2Stop);
        if(BloodPump_link_flag==true)
        {
       //     emit signalSDO(DE_CMD_SubsStop);
            SUB_stop();   //停止置换泵#/
        }
#else
        us_sleep(100000);
        MasterCtrol_CMD_BloodStop();
        us_sleep(10000);
        MasterCtrol_CMD_ClipStop();
        us_sleep(10000);
        MasterCtrol_CMD_Clip2Stop();
        us_sleep(1000);
        if(BloodPump_link_flag==true)
        {
            us_sleep(1000);
            MasterCtrol_CMD_SubsStop();
        }
#endif
}


void WidgetSwitch::SUB_start(void)  //启动置换泵#/
{

 //   PacketWorkMode workModeTMP;
    PacketSubBloodPump SubBPump;
    float subrate=0,tmprate=0;

//    dataRead(PACKET_NAME_WORKMODE,&workModeTMP);
    WorkState  stateTMP=GetWorkStateManage();

    if(G_SUB_RAPID_FLAG==false)
    {
        //解决治疗置换泵临时停止，重新启动后，重新计速度#/
        if(stateTMP.tipState.current==TREAT_RUNNING&&((G_SYS_CURTIME-G_UF_STARTTIME)>8)&&(residue_secCount>10)\
                &&((G_UF_ENDTIME-G_SYS_CURTIME)>10))
        {
            dataRead(PACKET_PDO_SUBBLOODPUMP,&SubBPump);

            subrate=((SubsPump_totalvolume-SubsPump_finishvolume)*1.0/residue_secCount)*3600;

            if(subrate>usr_params.Params_SubP.Flux.attribute.max_value)
                subrate=usr_params.Params_SubP.Flux.attribute.max_value;
            else if(subrate<usr_params.Params_SubP.Flux.attribute.min_value)
                subrate=usr_params.Params_SubP.Flux.attribute.min_value;

            tmprate=SubBPump.N10_set_speed*(1+G_SUB_autochangelimit); //sub速度自动改变时，限制的百分比范围#/

            if(subrate>tmprate)
            {
                subrate=tmprate;
            }
            else
            {
                tmprate=SubBPump.N10_set_speed*(1-G_SUB_autochangelimit);
                if(subrate<tmprate)
                {
                    subrate=tmprate;
                }
            }
            G_SUB_SETSPEED=subrate;
            SubBPump.N10_set_speed=G_SUB_SETSPEED;
            dataNodeUpdate(PACKET_PDO_SUBBLOODPUMP,&SubBPump);
        }

    }

     G_SUBPUMP_STATUS=true;  //置换泵状态#/
     emit signalSDO(DE_CMD_SubsStart);
}

void WidgetSwitch::SUB_stop(void)   //停止置换泵#/
{
    G_SUBPUMP_STATUS=false;  //置换泵状态#/
    emit signalSDO(DE_CMD_SubsStop);
//    SUB_stop();   //停止置换泵#/
}

void WidgetSwitch::UF_start(void)
{//启动超滤泵#/

 //   if(G_UF_FINISHVOL_FLAG==true)   //超滤目标已经达到，不能开启超滤泵#/
 //       return;


    PacketWorkMode workModeTMP;
    static struct PacketUF packetTmp;
    float ufrate=0,tmprate=0;
    dataRead(PACKET_NAME_WORKMODE,&workModeTMP);
    WorkState  stateTMP=GetWorkStateManage();

    //解决治疗超滤临时停止，重新启动后，重新计速度#/
    if(stateTMP.tipState.current==TREAT_RUNNING&&((G_SYS_CURTIME-G_UF_STARTTIME)>8)&&G_UF_PROFILE_FLAG==false&&(residue_secCount>10)\
            &&((G_UF_ENDTIME-G_SYS_CURTIME)>10))
    {

        dataRead(PACKET_NAME_UF,&packetTmp);
        if(workModeTMP.treatmode.current == ISOUF_SNSP||workModeTMP.treatmode.current == ISOUF_SNDP||\
                workModeTMP.treatmode.current == ISOUF_DNDP)
        {
            ufrate=((ISO_totalvolume-ISO_finishvolume)*1.0/(G_UF_ENDTIME-G_SYS_CURTIME))*3600.0;
        }
        else
        {
            ufrate=((G_UF_CURETARGET-G_UF_CUREFINISH)*1.0/(G_UF_ENDTIME-G_SYS_CURTIME))*3600.0;
        }

        if(ufrate>usr_params.Params_UF.UF_Rate.attribute.max_value)
            ufrate=usr_params.Params_UF.UF_Rate.attribute.max_value;
        else if(ufrate<usr_params.Params_UF.UF_Rate.attribute.min_value)
            ufrate=usr_params.Params_UF.UF_Rate.attribute.min_value;

        tmprate=G_UF_SETRATE*(1+G_UF_autochangelimit);   //超滤速度自动改变时，限制的百分比范围#/
        if(ufrate>tmprate)
        {
            ufrate=tmprate;
        }
        else
        {
            tmprate=G_UF_SETRATE*(1-G_UF_autochangelimit);
            if(ufrate<tmprate)
            {
                ufrate=tmprate;
            }
        }

        G_UF_SETRATE=ufrate;   //治疗中，每次重新开启UF，重新计算速度#/
        packetTmp.ufRate=ufrate;
        dataNodeUpdate(PACKET_NAME_UF, &packetTmp); //修改超滤速度为治疗的超滤速度#/
    }
        G_UFPUMP_STATUS =true;
        widgetButtonUFPump.setChecked(true);

#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_UFStart);
#else
        us_sleep(100000);
        MasterCtrol_CMD_UFStart();
#endif
}

void WidgetSwitch::UF_stop(void)
{//停止超滤泵#/
        G_UFPUMP_STATUS = false;
        widgetButtonUFPump.setChecked(false);        
#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_UFStop);
#else
        us_sleep(100000);
        MasterCtrol_CMD_UFStop();
#endif
 }

void WidgetSwitch::DRY_start(void)
{//启动排空#/
#if HJX_COMMUNICATION_SWITCH
    emit signalSDO(DE_CMD_Dry_start);
#else
    us_sleep(10000);
    MasterCtrol_CMD_Dry_start();//排空透析器#/
#endif
}

void WidgetSwitch::DRY_stop(void)
{//停止排空#/
#if HJX_COMMUNICATION_SWITCH
    emit signalSDO(DE_CMD_Dry_stop);
#else
    us_sleep(10000);
    MasterCtrol_CMD_Dry_stop();//停排空透析器#/
#endif
}

void WidgetSwitch::timer_handler(const int timeOut)
{//主要用于闪动按钮#/
    Q_UNUSED(timeOut);

#if DEBUG_TIMER_HANDLE
    Debug_insert("WidgetSwitch-timer-in");
#endif

    bool operBloodPump_flag=0;    
    static char s_bpstop_count=0;//治疗时血泵停止时间计数#/
    static char s_daemon_count=0;

    WorkState workstateTMP;
    workstateTMP = GetWorkStateManage();
    PacketOwnNote palarm_own;
    dataRead(OWNNOTE, &palarm_own);

    if(s_daemon_count>=5)
    {
        //主进程---心跳处理函数#/
        Parentprocess_handle();
        s_daemon_count=0;
    }
    else
        s_daemon_count++;

    if(G_rebirth_flag==true)  //重生操作时取消处理#/
        return;


    if(workstate.tipState.current==DEBUG)
        return;

    //肝素泵打开提示#/
    if(residue_secCount>heparinPreStop_sec)  //剩余治疗时间比肝素预停止时间小，肝素不起作用#/
    {
        if(workstateTMP.tipState.current == TREAT_RUNNING&&heparinPreStop!=true&&G_BPPUMP_STATUS==true)
        {
//            struct PacketHeparinPump heparinData;
//            dataRead(PACKET_NAME_HEPARINPUMP,&heparinData);
            if(G_HeparinEnable_flag==true&&G_HEPARINPUMP_STATUS==false)
            {
                if(timeCount==0)  //肝素打开提示#/
                {
                //    showYesDialog(DE_TR_PROMPT,DE_TR_NOOPENHEPARINPUMP);
                    alarmmodule->Custom_Prompt(PROMPT_HEPARIN_NOOPEN);
                }
                timeCount++;
                if(timeCount>=120)
                    timeCount=0;
            }
        }
    }
    else if((residue_secCount<=heparinPreStop_sec)&&(heparinPreStop==false)&&workstateTMP.tipState.current==TREAT_RUNNING&&G_HeparinEnable_flag==true)
    {
        heparinPreStop=true;
        prestatusHeparinPump=false;  //肝素时间到#/
        HeparinPump_stop();
        alarmmodule->Custom_Prompt(PROMPT_HEPARIN_TIMESTOP);
    }

    /*
    //治疗过程中的报警，引起血泵停止，进行提示#/
    if(workstateTMP.tipState.current == TREAT_RUNNING&&G_BPPUMP_STATUS==false)
    {
        if(timeCount==0)
            showYesDialog(DE_TR_PROMPT,DE_TR_HAPPENALARMONCURE);
        timeCount++;
        if(timeCount>=60)
            timeCount=0;
    }
    */

//血泵和血泵2、阻流夹联动处理--+#/
    PacketBloodPump blood1;
    dataRead(PACKET_NAME_BLOODPUMP,&blood1);
    PacketBloodPump blood2;
    dataRead(PACKET_NAME_SUBBLOODPUMP,&blood2);
    dataRead(PACKET_STATUS_ALARM,&alarmState);
    PacketStatusNote MasterStatus;
    dataRead(PACKET_STATUS_NOTE,&MasterStatus);
    struct PackN15PDO N15PDO;
    dataRead(PACKET_NAME_HEPARINDATA,&N15PDO);

    operBloodPump_flag=false;
    handle_count++;
    if(handle_count>=4)
    {
         //血泵1和血泵2联动处理-----准备状态#/
         if(TREAT_READY == workstateTMP.tipState.current)
         {
             if(BloodPump_link_flag==true&&(GETBLOOD_RUN_FLUID != workstateTMP.subState.current)&&(GETBLOOD_RUN_NO_FLUID != workstateTMP.subState.current))
             {
         //        if(blood1.N9_mode==BLOOD_PUMP_RUN&&blood2.N9_mode==BLOOD_PUMP_STOP&&Alarm_SubsPump_Flag==false && statusSideWay==false)
                 if(blood1.N9_mode==BLOOD_PUMP_RUN&&blood2.N9_mode==BLOOD_PUMP_STOP&&Alarm_SubsPump_Flag==false)
                 {
         #if HJX_COMMUNICATION_SWITCH
             //        emit signalSDO(DE_CMD_SubsStart);  //timer_handler()中，准备状态，和血泵1定时同步#/
                     SUB_start();  //启动置换泵#/
         #else
                     us_sleep(10000);
                     MasterCtrol_CMD_SubsStart();//启动血泵2#/
         #endif
                 }
                 else if(blood1.N9_mode==BLOOD_PUMP_STOP&&blood2.N9_mode==BLOOD_PUMP_RUN)
                 {
         #if HJX_COMMUNICATION_SWITCH
                //     emit signalSDO(DE_CMD_SubsStop);
                     SUB_stop();   //停止置换泵#/
         #else
                     us_sleep(10000);
                     MasterCtrol_CMD_SubsStop();
         #endif
                 }
             } //引血不开置换泵#/
             else if(((GETBLOOD_RUN_FLUID == workstateTMP.subState.current)||(GETBLOOD_RUN_NO_FLUID == workstateTMP.subState.current))&&getbloodstartflag==true)  //
             {
                 if(blood2.N9_mode==BLOOD_PUMP_RUN)
                 {
         #if HJX_COMMUNICATION_SWITCH
               //      emit signalSDO(DE_CMD_SubsStop);
                     SUB_stop();   //停止置换泵#/
         #else
                     us_sleep(10000);
                     MasterCtrol_CMD_SubsStop();
         #endif
                 }
             }
         }
         else if(workstateTMP.tipState.current==TREAT_RUNNING)//血泵1和血泵2联动处理-----治疗状态#/
         {

             if(BloodPump_link_flag==true)
             {
              //   if(blood1.N9_mode==BLOOD_PUMP_RUN&&blood2.N9_mode==BLOOD_PUMP_STOP&&Alarm_SubsPump_Flag==false && statusSideWay==false)
                 if(blood1.N9_mode==BLOOD_PUMP_RUN&&blood2.N9_mode==BLOOD_PUMP_STOP&&Alarm_SubsPump_Flag==false)
                 {
         #if HJX_COMMUNICATION_SWITCH
              //       emit signalSDO(DE_CMD_SubsStart);   //timer_handler()中，治疗状态，和血泵定时同步#/
                     SUB_start();  //启动置换泵#/
         #else
                     us_sleep(10000);
                     MasterCtrol_CMD_SubsStart();//启动血泵2#/
         #endif
                 }
                 else if(blood1.N9_mode==BLOOD_PUMP_STOP&&blood2.N9_mode==BLOOD_PUMP_RUN)
                 {
         #if HJX_COMMUNICATION_SWITCH
               //      emit signalSDO(DE_CMD_SubsStop);
                     SUB_stop();   //停止置换泵#/
         #else
                     us_sleep(10000);
                     MasterCtrol_CMD_SubsStop();
         #endif
                 }
             }
         }
         else if(workstateTMP.tipState.current==TREAT_FINISH)//血泵1和血泵2联动处理-----结束治疗状态#/
         {
             //回血不开置换泵#/
            if((CURE_BLOODRETURN_RUN == workstateTMP.subState.current)||(CURE_BLOODRETURN_RUN_NO_BLOOD == workstateTMP.subState.current))
            {
                if(blood2.N9_amount==BLOOD_PUMP_RUN)
                {
            #if HJX_COMMUNICATION_SWITCH
             //           emit signalSDO(DE_CMD_SubsStop);
                    SUB_stop();   //停止置换泵#/
            #else
                        us_sleep(10000);
                        MasterCtrol_CMD_SubsStop();
            #endif
                }
            }
         }
         else if(workstateTMP.tipState.current==CLEANING)//消毒不开置换泵#/
         {
             if(blood2.N9_amount==BLOOD_PUMP_RUN)
             {
         #if HJX_COMMUNICATION_SWITCH
              //       emit signalSDO(DE_CMD_SubsStop);
                 SUB_stop();   //停止置换泵#/
         #else
                     us_sleep(10000);
                     MasterCtrol_CMD_SubsStop();
         #endif
             }
         }

        //血泵和预冲按钮联动处理#/
        if((TREAT_READY == workstateTMP.tipState.current)&&(workstateTMP.subState.current>=SELFTEST_COMPLETE_FLUID)){//准备状态#/

            if(blood1.N9_mode == BLOOD_PUMP_RUN)
            {
                if(((PREFLUSH_RUN_FLUID != workstateTMP.subState.current)&&(PREFLUSH_RUN_NO_FLUID != workstateTMP.subState.current)\
                   &&(workstateTMP.subState.current<=PREFLUSH_COMPLETE_NO_FLUID))&&getbloodstartflag==false)
                {
                    if(bubble)  //无血的侍候才联动#/
                    {
                        prime_run(preflush_model);//血泵与预冲联动#/
                        operBloodPump_flag=true;
                    }
                }
                else if(((GETBLOOD_STOP_FLUID == workstateTMP.subState.current)||(GETBLOOD_STOP_NO_FLUID == workstateTMP.subState.current))&&getbloodstartflag==true)
                {
                    if(bubble)  //无血的侍候才联动#/
                    {
                        getblood_run();
                        operBloodPump_flag=true;
                    }
                }
                else if((TREAT_ENABLE_FLUID == workstateTMP.subState.current)||(TREAT_ENABLE_NO_FLUID == workstateTMP.subState.current))
                {
                    if(NoAlarm == bubble)  //有血#/
                    {
                        BP_stop();     //先停止血泵，再询问是否进入治疗#/
                        showNoteDialog("Cure Mode");//弹出确认启动的治疗模式#/
                        operBloodPump_flag=true;
                    }
                }
            }  //   if(blood1.N9_mode == BLOOD_PUMP_RUN)#/
            else if(blood1.N9_mode == BLOOD_PUMP_STOP)
            {
                if(((PREFLUSH_RUN_FLUID == workstateTMP.subState.current)||(PREFLUSH_RUN_NO_FLUID == workstateTMP.subState.current))\
                   &&getbloodstartflag==false)
                {
                    if(bubble)  //无血的侍候才联动#/
                    {
                        prime_stop(preflush_model,PRIME_STOP_FROM_PUMP);//血泵停止，暂停预冲#/
                        operBloodPump_flag=true;
                    }
                }
                else if(((GETBLOOD_RUN_FLUID == workstateTMP.subState.current)||(GETBLOOD_RUN_NO_FLUID == workstateTMP.subState.current))&&getbloodstartflag==true)
                {
                    if(bubble)  //无血的侍候才联动#/
                    {
                        getblood_stop();
                        operBloodPump_flag=true;
                    }
                }

                if(MasterStatus.note.note2&0x80)  //血泵停止，超滤泵停止#/
                {
                   UF_stop();     //停止超滤泵#/
                }

            }  // if(blood1.N9_mode == BLOOD_PUMP_STOP)

             if(((PREFLUSH_RUN_FLUID == workstateTMP.subState.current)||(PREFLUSH_RUN_NO_FLUID == workstateTMP.subState.current))&&(blood1.N9_mode == BLOOD_PUMP_RUN))
             {
                 //超滤泵无报警停止时，重新启动超滤泵#/
                 if(((MasterStatus.note.note2&0x80)==0)&&(Alarm_UFPump_Flag==false)&&preflush_model!=online_model)
                 {
                      UF_start();     //启动超滤泵#/
//                    operBloodPump_flag=true;
                 }
             }
        }
        else if(TREAT_RUNNING == workstateTMP.tipState.current){//治疗状态----各按键状态处理#/
             if(blood1.N9_mode == BLOOD_PUMP_RUN&&Alarm_BloodPump_Flag==true)
             {
                BP_stop();     //停止血泵#/
                G_BPPUMP_STATUS = true;  //为了通过硬件按键启动血泵时能触发启动超滤和肝素等处理#/
                operBloodPump_flag=true;
                s_bpstop_count=0;
             }
             else if(blood1.N9_mode == BLOOD_PUMP_RUN&&Alarm_BloodPump_Flag==false)  //血泵无报警正常运行时#/
             {
                 //超滤泵无报警停止时，重新启动超滤泵#/
                 if(((MasterStatus.note.note2&0x80)==0)&&(prestatusUFPump==true)&&(Alarm_UFPump_Flag==false))
                 {
                    UF_start();     //开超滤泵#/
                    operBloodPump_flag=true;
                 }

                 if(prestatusHeparinPump==true&&N15PDO.workMode!=0x01)
                 {
                    HeparinPump_run();//启动肝素泵#/
                 }
                 s_bpstop_count=0;
             }
             else if(blood1.N9_mode == BLOOD_PUMP_STOP&&Alarm_BloodPump_Flag==false) //停止血泵的报警处理完，又不打开血泵，进行提示#/
             {
                s_bpstop_count++;
                if(s_bpstop_count>5)
                {
                //    showYesDialog(DE_TR_PROMPT,DE_TR_STOPBLOODPUMPALARMISPROCESSED);
                    alarmmodule->Custom_Prompt(PROMPT_PLEASEOPENBLOOD);
                    s_bpstop_count=0;
                }
             }

             if(blood1.N9_mode == BLOOD_PUMP_STOP)
             {
                 if(MasterStatus.note.note2&0x80)
                 {
                    UF_stop();     //停止超滤泵#/
                 }

                 if(N15PDO.workMode==0x01)
                 {
                    HeparinPump_stop();//停止肝素泵#/
                 }
             }

             if((MasterStatus.note.note2&0x80)&&Alarm_UFPump_Flag==true)//有报警时停超滤泵#/
             {
                UF_stop();     //停止超滤泵#/
             }
        }
        else if(TREAT_FINISH == workstateTMP.tipState.current) //结束治疗状态#/
        {
            if((BLOOD_PUMP_RUN == blood1.N9_mode)&&( DRAIN_RUN == workstateTMP.subState.current)){
                //排空与血泵不能同时启动#/
                DRY_stop();//停止排空#/
                BP_stop();  //停止血泵#/
                workstateTMP.subState.current = DRAIN_STOP;
                SetWorkStateManage(&workstateTMP);
                operBloodPump_flag=true;
            }
            else if((BLOOD_PUMP_RUN == blood1.N9_mode)&&((CURE_BLOODRETURN_STOP == workstateTMP.subState.current)\
                    || (CURE_BLOODRETURN_STOP_NO_BLOOD == workstateTMP.subState.current)\
                    ||(CURE_BLOODRETURN_STOP_DRAIN == workstateTMP.subState.current)\
                    ||(DRAIN_STOP == workstateTMP.subState.current)\
                    ||(CURE_BLOODRETURN_STOP_DRAIN_NO_BLOOD == workstateTMP.subState.current))){//血泵运行#/
                        ClickedPreShoot();//与血泵联动#/
                        operBloodPump_flag=true;
            }
            else if((BLOOD_PUMP_STOP == blood1.N9_mode)&&((CURE_BLOODRETURN_RUN == workstateTMP.subState.current)\
                    ||(CURE_BLOODRETURN_RUN_NO_BLOOD == workstateTMP.subState.current)\
                    ||(CURE_BLOODRETURN_RUN_DRAIN == workstateTMP.subState.current)\
                    || (CURE_BLOODRETURN_RUN_DRAIN_NO_BLOOD == workstateTMP.subState.current))){//血泵停止，#/
                    ClickedPreShoot();//与血泵联动#/
                    operBloodPump_flag=true;
            }
        }

        static char pass_count=0;
//        static char pass_count=0,pass_count2=0;
        PacketStatusNote L_data;
        dataRead(PACKET_STATUS_NOTE,&L_data);

        //水路或者血路没有启动时，则发启动准备命令SDO#/
        if ( (waterroadOk == 0) || (bloodroadOk == 0) ){
            int waterState=PDOstate.state.N1_water_status;  //水路状态#/
            if(waterState==0&&(workstateTMP.tipState.current==TREAT_READY)&&isSelfcheckstart==true&&(pass_count<20))
            {
            if(L_data.status.header_status==0)
            {
                pass_count++;
                if(pass_count>=19)
                {
                    palarm_own.note.note7 |= 0x01;
                    dataNodeUpdate(OWNNOTE, &palarm_own);
                }
                if(selfcheck_enable==false)  //跳过自检的情况下
                {
#if HJX_COMMUNICATION_SWITCH
                emit signalSDO(DE_CMD_Pass_Checkself);
#else
                MasterCtrol_CMD_Pass_Checkself();
#endif
             //   isSelfcheckstart=false;
                }
                else
                {
#if HJX_COMMUNICATION_SWITCH
                emit signalSDO(DE_CMD_Checkself);
#else
                MasterCtrol_CMD_Checkself();
#endif
            //    isSelfcheckstart=false;
                }
            }
            }
            else
            {
                if((palarm_own.note.note7&=0x01)&&(L_data.status.header_status!=0))
                {
                    palarm_own.note.note7&=0xfe;
                    dataNodeUpdate(OWNNOTE, &palarm_own);
                }
            }
        }
        else if(workstateTMP.tipState.current==TREAT_RUNNING)//同步治疗状态，确保主控板治疗状态正确#/
        {
            if((L_data.status.header_status!=0x02)&&(pass_count<20))
            {
                pass_count++;
                if(pass_count%5==0)
                {
                //发送治疗状态#/
            #if HJX_COMMUNICATION_SWITCH
                        emit signalSDO(DE_CMD_Treat_start);
            #else
                        MasterCtrol_CMD_Treat_start(workModeData.treatmode.current);
            #endif
                }
            }
            else if(L_data.status.header_status == 0x02)
                pass_count=0;
        }
        else if(workstateTMP.tipState.current==TREAT_FINISH)//同步结束治疗状态，确保主控板治疗状态正确#/
        {
            if((L_data.status.header_status!=0x03)&&(pass_count<20))
            {
                pass_count++;
                if(pass_count%5==0)
                {
                //发送治疗状态#/
            #if HJX_COMMUNICATION_SWITCH
                        emit signalSDO(DE_CMD_Treat_stop);
            #else
                        MasterCtrol_CMD_Treat_stop();
            #endif
                }
            }
            else if(L_data.status.header_status == 0x03)
                pass_count=0;
        }
        else if( (waterroadOk != 0) && (bloodroadOk != 0)&&(workstateTMP.tipState.current<=TREAT_READY))
        {
            pass_count=0;
        }

        /*
        //判断SDO异常时，重新初始化canopen#/
        if(G_CANSDONORMAL_FLAG==false&&(workstateTMP.tipState.current==TREAT_READY))
        {
            if(pass_count2>=20)
            {
                dataManage->CanopenOpen();
                us_sleep(1000);
                G_CANSDONORMAL_FLAG=true;
            }
            else if(pass_count2<20)
                pass_count2++;
        }
        */

        //确保引血的SDO主控板收到，同步状态#/
        static char start_err_count=0;
        if(workstateTMP.tipState.current==TREAT_READY)
        {
            //引血
            if(workstateTMP.subState.current==GETBLOOD_RUN_NO_FLUID||workstateTMP.subState.current==GETBLOOD_RUN_FLUID)
            {
                if(Startpreflush_mode==ONLINE_GETBLOOD_MODE&&L_data.status.blood_status!=0X18)
                {
                    start_err_count++;
                    if(start_err_count>=3)
                    {
                        palarm_own.note.note7 |= 0x20;
                    }
            #if HJX_COMMUNICATION_SWITCH
                    emit signalSDO(DE_CMD_Startpreflush);
            #else
                    us_sleep(100000);
                    MasterCtrol_CMD_Startpreflush(Startpreflush_mode);//启动预冲#/
                    us_sleep(10000);
            #endif
                }
                else if(Startpreflush_mode==GETBLOOD_MODE&&L_data.status.blood_status!=0X17)
                {
                    start_err_count++;
                    if(start_err_count>=3)
                    {
                        palarm_own.note.note7 |= 0x10;
                    }
            #if HJX_COMMUNICATION_SWITCH
                    emit signalSDO(DE_CMD_Startpreflush);
            #else
                    us_sleep(100000);
                    MasterCtrol_CMD_Startpreflush(Startpreflush_mode);//启动预冲#/
                    us_sleep(10000);
            #endif
                }
            }
            else if(workstateTMP.subState.current==PREFLUSH_RUN_FLUID||workstateTMP.subState.current==PREFLUSH_RUN_NO_FLUID)//预冲#/
            {
                if(Startpreflush_mode==MANUAL_PREFLUSH_MODE&&L_data.status.blood_status!=0x14)
                {
                    start_err_count++;
                    if(start_err_count>=4)
                    {
                        palarm_own.note.note7 |= 0x02;
                    }
            #if HJX_COMMUNICATION_SWITCH
                    emit signalSDO(DE_CMD_Startpreflush);
            #else
                    us_sleep(100000);
                    MasterCtrol_CMD_Startpreflush(Startpreflush_mode);//启动预冲#/
                    us_sleep(10000);
            #endif
                }
                else if(Startpreflush_mode==AUTO_PREFLUSH_MODE&&L_data.status.blood_status!=0x15)
                {
                    start_err_count++;
                    if(start_err_count>=4)
                    {
                        palarm_own.note.note7 |= 0x04;
                    }
            #if HJX_COMMUNICATION_SWITCH
                    emit signalSDO(DE_CMD_Startpreflush);
            #else
                    us_sleep(100000);
                    MasterCtrol_CMD_Startpreflush(Startpreflush_mode);//启动预冲#/
                    us_sleep(10000);
            #endif
                }
                else if(Startpreflush_mode==ONLINE_PREFLUSH_MODE&&L_data.status.blood_status!=0x16)
                {
                    start_err_count++;
                    if(start_err_count>=4)
                    {
                        palarm_own.note.note7 |= 0x08;
                    }
            #if HJX_COMMUNICATION_SWITCH
                    emit signalSDO(DE_CMD_Startpreflush);
            #else
                    us_sleep(100000);
                    MasterCtrol_CMD_Startpreflush(Startpreflush_mode);//启动预冲#/
                    us_sleep(10000);
            #endif
                }
            }
            else
            {
                start_err_count=0;
                if(palarm_own.note.note7&0xfe)
                {
                    palarm_own.note.note7 &= 0x01;
                    dataNodeUpdate(OWNNOTE, &palarm_own);
                }
            }
        }
        else if(workstateTMP.tipState.current==TREAT_FINISH)
        {
            //回血
            if(workstateTMP.subState.current==CURE_BLOODRETURN_RUN||workstateTMP.subState.current==CURE_BLOODRETURN_RUN_NO_BLOOD)
            {
                /* // 暂时屏蔽回血同步#/
                if(Startreturnblood_mode==ONLINE_RETURNBLOOD_MODE&&L_data.status.blood_status!=0x1a)
                {
                    start_err_count++;
                    if(start_err_count>=3)
                    {
                        palarm_own.note.note7 |= 0x80;
                    }
            #if HJX_COMMUNICATION_SWITCH
                    emit signalSDO(DE_CMD_Startreturnblood);
            #else
                    us_sleep(100000);
                    MasterCtrol_CMD_Startreturnblood(Startreturnblood_mode);//启动回血#/
                    us_sleep(10000);
            #endif
                }
                else if(Startreturnblood_mode==RETURNBLOOD_MODE&&L_data.status.blood_status!=0x19)
                {
                    start_err_count++;
                    if(start_err_count>=3)
                    {
                        palarm_own.note.note7 |= 0x40;
                    }
            #if HJX_COMMUNICATION_SWITCH
                    emit signalSDO(DE_CMD_Startreturnblood);
            #else
                    us_sleep(100000);
                    MasterCtrol_CMD_Startreturnblood(Startreturnblood_mode);//启动回血#/
                    us_sleep(10000);
            #endif
                }
                */
            }
            else
            {
                start_err_count=0;
                if(palarm_own.note.note7&0xfe)
                {
                    palarm_own.note.note7 &= 0x01;
                    dataNodeUpdate(OWNNOTE, &palarm_own);
                }
            }
        }

        //解决 治疗过程中超滤泵速度突变的问题#/
        if(workstateTMP.tipState.current==TREAT_RUNNING)
        {
             if(MasterStatus.note.note2&0x80)  //超滤泵 运行时#/
             {
                 struct PacketUF packetUF;
                 dataRead(PACKET_NAME_UF, &packetUF);

                 if(G_UF_SETRATE!=(unsigned int)packetUF.ufRate)
                 {
                    CHANGED_UF_SPEED(true,G_UF_SETRATE);
                 }
             }
        }

        if(start_err_count>=3)
        {
            start_err_count=0;
            dataNodeUpdate(OWNNOTE, &palarm_own);
        }

        if(handle_count>(S_Delay_Count+1))
            handle_count=0;
    }//  if(handle_count>=4)#/

    if(handle_count>=S_Delay_Count)
    {
        //血泵1和阻流夹的同步
        if(operBloodPump_flag==false)
        {
            if(blood1.N9_mode==BLOOD_PUMP_RUN)  //血泵运行时#/
            {
                //if(workstate.tip==Cure_run)
                //alarmState
                /*
                if(alarmState.alarm.Iostatus&0x18)  //动静脉阻流夹关闭#/
                {
        #if HJX_COMMUNICATION_SWITCH
                    emit signalSDO(DE_CMD_ClipStart);
                    emit signalSDO(DE_CMD_Clip2Start);
        #else
                    MasterCtrol_CMD_ClipStart();  //开启静脉阻流夹#/
                    usleep(100000);
                    MasterCtrol_CMD_Clip2Start(); //开启动脉#/
                    usleep(100000);
        #endif
                }
                */
                if(alarmState.alarm.Iostatus&0x08)  //动静脉阻流夹关闭#/
                {
        #if HJX_COMMUNICATION_SWITCH
                    emit signalSDO(DE_CMD_ClipStart);
        #else
                    MasterCtrol_CMD_ClipStart();  //开启静脉阻流夹#/
                    usleep(100000);
        #endif
                }
            }
            else if(blood1.N9_mode==BLOOD_PUMP_STOP)  //血泵停止时#/
            {
                if((alarmState.alarm.Iostatus&0x08)!=0x08)  //阻流夹打开时#/
                {
        #if HJX_COMMUNICATION_SWITCH
                    emit signalSDO(DE_CMD_ClipStop);
 //                   emit signalSDO(DE_CMD_Clip2Stop);
        #else
                    MasterCtrol_CMD_ClipStop();
                    usleep(100000);
//                    MasterCtrol_CMD_Clip2Stop();
//                    usleep(100000);
        #endif
                }
            }
        }

        /*
        //超滤泵在online预冲下禁用#/
        if(workstateTMP.tipState.current<=TREAT_READY)
        {
            if(widgetButtonUFPump.isEnabled()&&(preflush_model==online_model))
            {
                UF_stop();
                widgetButtonUFPump.setDisabled(true);
            }
            else if(!widgetButtonUFPump.isEnabled()&&(preflush_model!=online_model))
            {
                widgetButtonUFPump.setDisabled(false);
            }
        }
        */

        //超滤泵和按键同步#/
        if((workstateTMP.tipState.current<=TREAT_FINISH)&&(operBloodPump_flag==false))
        {

            if(workstateTMP.tipState.current==TREAT_READY&&preflush_model==online_model)//在线预冲停泵#/
            {
                if(MasterStatus.note.note2&0x80)
                    UF_stop();
            }
            else
            {
                if(MasterStatus.note.note2&0x80)
                {
                    if(G_UFPUMP_STATUS==false||(!widgetButtonUFPump.isChecked()))
                    {
                        G_UFPUMP_STATUS =true;
                        widgetButtonUFPump.setChecked(true);
     //                   UF_start();    //启动超滤泵#/
                    }
                }
                else
                {
                    if(G_UFPUMP_STATUS==true||widgetButtonUFPump.isChecked())
                    {
                        G_UFPUMP_STATUS = false;
                        widgetButtonUFPump.setChecked(false);
    //                    UF_stop();
                    }
                }
            }
        }

        /*
        //旁路和按键同步#/
        if(workstateTMP.tipState.current<=TREAT_FINISH)
        {
            if(MasterStatus.note.note4&0x40)
            {
                if(statusSideWay==false||(!widgetButtonBypass.isChecked()))
                {
                    statusSideWay=true;
                    widgetButtonBypass.setChecked(true);
                }
            }
            else
            {
                if(statusSideWay==true||widgetButtonBypass.isChecked())
                {
                    statusSideWay=false;
                    widgetButtonBypass.setChecked(false);
                }
            }
        }
        */

        //旁路按键状态的处理#/
        if(workstateTMP.tipState.current>=TREAT_READY&&workstateTMP.tipState.current<=TREAT_FINISH&&workstateTMP.subState.current>=SELFTEST_COMPLETE_FLUID)
        {
            if((MasterStatus.note.note4&0x40)&&(G_BYPASS_BUT_FLAG==DE_BYPASS_NOCHECKED))  //旁路按钮未点击，主控板旁路，变黄色#/
            {
                G_BYPASS_BUT_FLAG=DE_BYPASS_MASTERPASS;
//                statusSideWay=true;   //timer_Handle(),旁路键未点击时，状态同步#/
                widgetButtonBypass.setStyleDisabled(":/resource/images/switch_side_yellow.png",
                        ":/resource/images/switch_side_yellow.png",
                        ":/resource/images/switch_side_road_disable.png",
                        CONFIG_COLOR_WHITE,
                        QRect(WIDGETSWITCH_BUTTON_SIDEROAD_X,
                            WIDGETSWITCH_BUTTON_SIDEROAD_Y,
                            WIDGETSWITCH_BUTTON_SIDEROAD_WIDTH,
                            WIDGETSWITCH_HIGH));
            }
            else if(((MasterStatus.note.note4&0x40)==0)&&(G_BYPASS_BUT_FLAG==DE_BYPASS_MASTERPASS))//旁路按钮未点击，主控板取消旁路，恢复原色#/
            {
                G_BYPASS_BUT_FLAG=DE_BYPASS_NOCHECKED;
 //               statusSideWay=false;   //timer_Handle(),旁路键未点击时，状态同步#/
                widgetButtonBypass.setStyleDisabled(":/resource/images/switch_side_road.png",
                        ":/resource/images/switch_side_road_active.png",
                        ":/resource/images/switch_side_road_disable.png",
                        CONFIG_COLOR_WHITE,
                        QRect(WIDGETSWITCH_BUTTON_SIDEROAD_X,
                            WIDGETSWITCH_BUTTON_SIDEROAD_Y,
                            WIDGETSWITCH_BUTTON_SIDEROAD_WIDTH,
                            WIDGETSWITCH_HIGH));
            }
            else if(((MasterStatus.note.note4&0x40)==0)&&(G_BYPASS_BUT_FLAG==DE_BYPASS_CHECKED)&&(prestatusSideWay==true))
            {
                statusSideWay=true;   //timer_Handle(),旁路键点击时，状态同步#/
                widgetButtonBypass.setChecked(true);
                #if HJX_COMMUNICATION_SWITCH
                        emit signalSDO(DE_CMD_Bypass);
                #else
                        MasterCtrol_CMD_Bypass();
                #endif
            }
/*
            //旁路开启时，不能开启置换液泵#/
            if((MasterStatus.note.note4&0x40)&&blood2.N9_mode==BLOOD_PUMP_RUN)
            {
#if HJX_COMMUNICATION_SWITCH
        //    emit signalSDO(DE_CMD_SubsStop);
               SUB_stop();   //停止置换泵#/
#else
            us_sleep(10000);
            MasterCtrol_CMD_SubsStop();
#endif
            }
*/

        }

        //肝素泵和按键同步#/
        if(workstateTMP.tipState.current<=TREAT_FINISH)
        {
            if(N15PDO.workMode==0x01&&(G_HEPARINPUMP_STATUS==false||(!widgetButtonHeparinPump.isChecked())))
            {
                G_HEPARINPUMP_STATUS=true;
                widgetButtonHeparinPump.setChecked(true);
             //   HeparinPump_run();//启动肝素泵#/
            }
            else if(N15PDO.workMode!=0x01)
            {
                if(G_HEPARINPUMP_STATUS==true||(widgetButtonHeparinPump.isChecked()))
                {
                    G_HEPARINPUMP_STATUS=false;
                    widgetButtonHeparinPump.setChecked(false);
            //        HeparinPump_stop();//停止肝素泵#/
                }
            }

            /*
            if(prestatusHeparinPump==true&&G_HEPARINPUMP_STATUS==true)  //打开肝素泵不成功，则再打开一次
            {
                if(N15PDO.workMode==0x02||N15PDO.workMode==0x03)
                {
                    HeparinPump_run();//启动肝素泵#/
                }
            }
            else if(prestatusHeparinPump==false&&G_HEPARINPUMP_STATUS==false)
            {
                if(N15PDO.workMode==0x01)
                {
                    HeparinPump_stop();//停止肝素泵#/
                }
            }
            */
        }

        if(workstateTMP.tipState.current==TREAT_RUNNING)
        {
            if(G_NA_PROFILE_FLAG==true)  //timer_handler(const int timeOut)#/
            {
                //NA曲线处理#/
                NA_PROFILE_HANDLE(G_NA_PROFILE_START,G_SYS_CURTIME,G_NA_PROFILE_END);
            }

            if(G_UF_PROFILE_FLAG==true)
            {
                //UF曲线处理#/
                UF_PROFILE_HANDLE(G_UF_PROFILE_START,G_SYS_CURTIME,G_UF_PROFILE_END);

                //该时段的超滤目标量到#/
                if(((unsigned int)G_UF_CUREFINISH>=G_UF_PROFILE_PERTOTALVOL)&&(G_UF_FINISHVOL_FLAG==false))
                {
                    G_UF_SETRATE=50;   //超滤目标量到#/
                    CHANGED_UF_SPEED(true,G_UF_SETRATE);
                    G_UF_FINISHVOL_FLAG=true;
                }

                //时间过去一半，但是已超滤量不到总目标量的1/3，则判定曲线失败#/
                if(((G_SYS_CURTIME-G_UF_PROFILE_START)>G_UF_PROFILE_END/2)&&\
                    ((G_UF_CUREFINISH-G_UF_PROFILE_STARTVOL)<(G_UF_CURETARGET-G_UF_PROFILE_STARTVOL)*0.33))
                {
                    showYesDialog(DE_TR_PROMPT,DE_TR_REMAININGTIMETOOLESSPROFILECANCEL);
                    G_UF_PROFILE_FLAG=false;
                }
            }

            //透析液曲线处理函数#/
            if(G_DIA_PROFILE_FLAG==true)
            {
                DIA_PROFILE_HANDLE();
            }

            //肝素曲线处理函数#/
            if(G_HEP_PROFILE_FLAG==true)
            {
                HEP_PROFILE_HANDLE();
            }

            //电导曲线处理函数#/
            if(G_COND_PROFILE_FLAG==true)
            {
                COND_PROFILE_HANDLE();
            }

            //温度曲线处理函数#/
            if(G_TEMP_PROFILE_FLAG==true)
            {
                TEMP_PROFILE_HANDLE();
            }

            //HCO3曲线处理函数#/
            if(G_HCO3_PROFILE_FLAG==true)
            {
                HCO3_PROFILE_HANDLE();
            }
        }

        //防止报警不刷新，而专门设置，但是较消耗资源,相当于定时刷新报警#/
        if(alarmmodule->Get_CurrentAlarmNum()>0)
        {
            dataNodeUpdate(OWNNOTE, &palarm_own);
        }

        S_Delay_Count=2;
    }      //    if(handle_count>=3)

    //血泵和血泵2、阻流夹联动处理--#/

        //++血泵2的物理按键使能和失能处理===============================================V1.5
        PacketWorkMode workModeTmp;
        if(workstateTMP.tipState.current >= TREAT_READY)
        {
        dataRead(PACKET_NAME_WORKMODE,&workModeTmp);
        //切换到online动作#/
        if(workModeTmp.treatmode.current == HDF_pre_dilute||workModeTmp.treatmode.current == HDF_post_dilute||\
           workModeTmp.treatmode.current == HF_pre_dilute||workModeTmp.treatmode.current == HF_post_dilute)
        {
            if(blood1.N9_mode==BLOOD_PUMP_RUN)  //血泵1启动时，血泵2可以硬件开关
            {
                if(blood2enableflag==false)
                {
                    blood2enableflag=true;
    #if HJX_COMMUNICATION_SWITCH
                    emit signalSDO(DE_CMD_SubsEnableButton);
    #else
                    MasterCtrol_CMD_SubsEnableButton();
    #endif
                }
            }
            else  //血泵1关闭时，血泵2不可以硬件开关
            {
                if(blood2enableflag==true)
                {
                    blood2enableflag=false;
    #if HJX_COMMUNICATION_SWITCH
                    emit signalSDO(DE_CMD_SubsDisableButton);
    #else
                    MasterCtrol_CMD_SubsDisableButton();
    #endif
                }
            }
        }// if(workModeData.treatmode.current == HDF_pre_dilute
        else
        {

            if(workstateTMP.tipState.current == TREAT_READY||workstateTMP.tipState.current == TREAT_FINISH||workstateTMP.tipState.current == CLEANING)
            {
                if(blood2enableflag==true)
                {
                    blood2enableflag=false;
    #if HJX_COMMUNICATION_SWITCH
                    emit signalSDO(DE_CMD_SubsDisableButton);
    #else
                    MasterCtrol_CMD_SubsDisableButton();
    #endif
                }
            }
            else if(workstateTMP.tipState.current == TREAT_RUNNING)
            {
                if(blood2enableflag==false)
                {
                    blood2enableflag=true;
    #if HJX_COMMUNICATION_SWITCH
                    emit signalSDO(DE_CMD_SubsEnableButton);
    #else
                    MasterCtrol_CMD_SubsEnableButton();
    #endif
                }
            }
        }
        } // if(workstateTMP.tipState.current >= TREAT_READY)
        //--血泵2的物理按键使能和失能处理===============================================V1.5

    //判断各个板子canopen通信失败报警
#ifdef HW_ARM
    unsigned char byte=0;
    if(N3_CANOPEN_FLAG>=15)
    {
        byte |=0x01;
    }
    else
    {
        N3_CANOPEN_FLAG++;
    }

    if(N5_CANOPEN_FLAG>=20)
    {
        byte |=0x02;
    }
    else
    {
        N5_CANOPEN_FLAG++;
    }

    if(N9_CANOPEN_FLAG>=15)
    {
        byte |=0x04;
    }
    else
    {
        N9_CANOPEN_FLAG++;
    }

    //切换到online动作#/
    if(workModeTmp.treatmode.current == HDF_pre_dilute||workModeTmp.treatmode.current == HDF_post_dilute||\
       workModeTmp.treatmode.current == HF_pre_dilute||workModeTmp.treatmode.current == HF_post_dilute)
    {
        if(N10_CANOPEN_FLAG>=15)
        {
            byte |=0x08;
        }
        else
        {
            N10_CANOPEN_FLAG++;
        }
    }

    if(N15_CANOPEN_FLAG>=50)
    {
        byte |=0x10;
    }
    else
    {
        N15_CANOPEN_FLAG++;
    }

    if(N7_CANOPEN_FLAG>=15)
    {
        byte |=0x20;
    }
    else
    {
        N7_CANOPEN_FLAG++;
    }

    if(byte!=palarm_own.note.note6)
    {
        palarm_own.note.note6=byte;
        dataNodeUpdate(OWNNOTE, &palarm_own);
    }

#endif


    //回血时，检测到无血提示#/
    if(bubble)//检测到无血#/
    {
        //回血时，检测到无血，进行提醒#/
        if(workstateTMP.subState.current==CURE_BLOODRETURN_RUN_NO_BLOOD||workstateTMP.subState.current==CURE_BLOODRETURN_RUN_DRAIN_NO_BLOOD)
        {
            if(timeCount==0)   //回血提示#/
            {
                if(connectstatus==false)
                {
                    connect(YesNoDialog,SIGNAL(NoteSelSignal(int)),this,SLOT(ConfirmSelect(int)));
                    connectstatus=true;
                    ClickedPreShoot();   //结束回血#/
                    showYesNoDialog(DE_TR_PLEASECONFIRM,DE_TR_WHETHERTORETURNBLOOD);
                }
            }
            timeCount++;
            if(timeCount>=40)
                timeCount=0;
        }
    } //    if(bubble)//检测到无血#/
    else if(workstateTMP.subState.current==CURE_BLOODRETURN_RUN||workstateTMP.subState.current==CURE_BLOODRETURN_RUN_DRAIN)
    {
        timeCount=0;//检测到无血#/
    }

    //排空30S后自动停止#/
    if(workstateTMP.tipState.current==TREAT_FINISH && workstateTMP.subState.current==DRAIN_RUN)
    {
        if(timeCount>=30)   //排空提示#/
        {
            if(connectstatus==false)
            {
                connect(YesNoDialog,SIGNAL(NoteSelSignal(int)),this,SLOT(ConfirmSelect(int)));
                connectstatus=true;
            }
            ClickedUFPump();
            showYesNoDialog(DE_TR_PLEASECONFIRM,DE_TR_WHETHERTODRAIN);
            timeCount=0;
        }
        timeCount++;
    }

    /*
    timeCount++;
    if(timeCount>(Preflush.PreflushTime*60))
    {
     timeCount=0;
     //prime_stop(preflush_model);
    }
    */

    if (strcmp(PageCurrent(), "Main") == 0)//
    {
        if(isFlashShow)
        {
            isFlashing = !isFlashing;

            if(workstateTMP.tipState.current == TREAT_READY)
            {//在准备状态，检测到血，可以启动治疗，闪烁治疗键#/
                 if(isFlashing)
                   widgetButtonTreat.setChecked(true);
                 else
                   widgetButtonTreat.setChecked(false);
             }
            else if(workstateTMP.tipState.current == TREAT_FINISH && \
                    (workstateTMP.subState.current == CURE_BLOODRETURN_STOP|| \
                     workstateTMP.subState.current == CURE_BLOODRETURN_STOP_NO_BLOOD))
            {//在结束治疗状态下，没有启动回血，则闪烁回血键（之前的预冲键）#/
                 if(isFlashing)
                   widgetButtonPrime.setChecked(true);
                 else
                   widgetButtonPrime.setChecked(false);
            }
        }
   }



#if DEBUG_TIMER_HANDLE
    Debug_insert("WidgetSwitch-timer-out");
#endif
}

void WidgetSwitch::getblood_run()  //引血开始#/
{
    getbloodstartflag=true;
    widgetButtonPrime.setDisabled(true);
    widgetButtonTreat.setChecked(true);
    WorkState  stateTMP=GetWorkStateManage();
    if(!bloodlevel)stateTMP.subState.current = GETBLOOD_RUN_FLUID;
    else{stateTMP.subState.current = GETBLOOD_RUN_NO_FLUID;}
    stateTMP.disinfectEnable = false;
    SetWorkStateManage(&stateTMP);//子状态改为预冲运行状态#/

    CHANGED_BP_SPEED(G_BPRATE_GETBLOOD);//改变引血血泵速度#/
    BP_start();    //启动血泵#/
    if(G_UFPUMP_STATUS==true)
        UF_stop();

    PacketWorkMode workModeTMP;
    dataRead(PACKET_NAME_WORKMODE,&workModeTMP);
    if(workModeTMP.treatmode.current==HDF_pre_dilute||workModeTMP.treatmode.current==HDF_post_dilute\
       ||workModeTMP.treatmode.current==HF_pre_dilute||workModeTMP.treatmode.current==HF_post_dilute)
    {
        if(Blood_online_lead==true&&preflush_model == online_model)
            Startpreflush_mode=ONLINE_GETBLOOD_MODE;
        else
            Startpreflush_mode=GETBLOOD_MODE;
    }
    else
    {
        Startpreflush_mode=GETBLOOD_MODE;
    }

//    emit signalSDO(DE_CMD_SubsStop);   //引血不开置换泵#/

     widgetButtonHeparinPump.setDisabled(true); //引血的时候，停止使用肝书泵#/

#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_Startpreflush);
#else
        us_sleep(100000);
        MasterCtrol_CMD_Startpreflush(Startpreflush_mode);//启动引血#/
        us_sleep(10000);
#endif
}

void WidgetSwitch::getblood_stop() //引血暂停#/
{
//    struct PacketHeparinPump heparinData;
//    dataRead(PACKET_NAME_HEPARINPUMP,&heparinData);

    WorkState  stateTMP=GetWorkStateManage();
    widgetButtonTreat.setChecked(false);
    if(!bloodlevel)stateTMP.subState.current = GETBLOOD_STOP_FLUID;
    else{stateTMP.subState.current = GETBLOOD_STOP_NO_FLUID;}
    stateTMP.disinfectEnable = false;
    SetWorkStateManage(&stateTMP);//子状态改为预冲运行状态#/
    BP_stop();     //停止血泵#/

//    if(heparinData.selstatus==1)
    if(G_HeparinEnable_flag==true)
        widgetButtonHeparinPump.setDisabled(false); //引血停止的时候，可使用肝书泵#/

#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_Stoppreflush);
#else
        us_sleep(100000);
        MasterCtrol_CMD_Stoppreflush();//停止预冲#/
        us_sleep(10000);
#endif
}

void WidgetSwitch::treatment_run()
{
    widgetButtonPrime.setChecked(false);
    widgetButtonPrime.setDisabled(false);
    widgetButtonTreat.setChecked(true);
    widgetButtonUFPump.setDisabled(false);
    widgetButtonTreat.setText(tr("Treatment"));

//    struct PacketHeparinPump heparinData;
//    dataRead(PACKET_NAME_HEPARINPUMP,&heparinData);
 //   if(heparinData.selstatus==1)
    if(G_HeparinEnable_flag==true)
        widgetButtonHeparinPump.setDisabled(false);
    else
    {
        widgetButtonHeparinPump.setDisabled(true);
        prestatusHeparinPump=false;
    }

    alarmmodule->light(0);   //close light and sound#/
    alarmmodule->sound(0);
    Alarm_sound_shielding=false;
    Alarm_light_shielding=false;
    getbloodstartflag=false;

    if (BloodPump_Volume < Preflush.TractBloodVolume)
        BloodPump_Pre_Amount += BloodPump_Volume;

    BloodPump_Volume=0;

    ISO_finishvolume=0;
    G_UF_CUREFINISH=0;     //开始治疗，超滤完成量从0开始计算#/

    //为了防止状态切换引发的超滤量计算不对#/
    static_UF_amount=packetUfValue.N3_UF_amount;
//    static_UF_amount=0;
    UFPump_Pre_Amount += packetUfValue.N3_UF_amount;   //预冲超滤累计量 加上本次预冲超滤量
    G_UF_PREFINISH=0;


    //清空相关参数，再返回准备状态#/
#if HJX_COMMUNICATION_SWITCH
    emit signalSDO(DE_CMD_ResetBloodAmount);
    emit signalSDO(DE_CMD_ResetSubsAmount);  //treatment_run()，进入治疗时#/
    emit signalSDO(DE_CMD_ResetHeparinPumpAmount);
    emit signalSDO(DE_CMD_ResetUFAmount);
#else
    //清空相关参数
    MasterCtrol_CMD_ResetBloodAmount();
    us_sleep(100000);
    MasterCtrol_CMD_ResetSubsAmount();
    us_sleep(100000);
    MasterCtrol_CMD_ResetHeparinPumpAmount();
    us_sleep(10000);
    MasterCtrol_CMD_ResetUFAmount();
#endif

    dataRead(PACKET_NAME_WORKMODE,&workModeData);
    //发送治疗状态#/
#if HJX_COMMUNICATION_SWITCH
            emit signalSDO(DE_CMD_Treat_start);
#else
            MasterCtrol_CMD_Treat_start(workModeData.treatmode.current);
#endif

    us_sleep(100000);
    BP_start();    //启动血泵#/

    PacketWorkMode workModeTMP;
    dataRead(PACKET_NAME_WORKMODE,&workModeTMP);

    MasterCtrol_CMD_HeparinPumpStop(); //关肝素#/
    us_sleep(100000);

//    HeparinPumpFirstflag=false;//肝素泵开始有首剂量的#/
#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_ResetHeparinPumpAmount);
#else
        //清空相关参数
        MasterCtrol_CMD_ResetHeparinPumpAmount();
        us_sleep(10000);
#endif


    /*启动肝素泵 手动控制
    //MasterCtrol_CMD_HeparinPumpStartHaveFirst();
     //us_sleep(100000);*/    

    //workstate.current = HD_double_needle_run;
    //workstate.tip = Cure_run;
    //dataNodeUpdate(PACKET_NAME_WORKSTATE, &workstate);
    Timer_state =Timestart;
    dataNodeUpdate(TREAT_TIMER, &Timer_state);  //启动治疗时间计时#/

    G_UF_FINISHVOL_FLAG=false;
    G_UF_STARTTIME=G_SYS_CURTIME;      //超滤开启时间 单位秒#/
    G_UF_ENDTIME=G_SYS_CURTIME+residue_secCount;  //超滤的总时间 单位秒#/

    struct PacketUF packetTmp;
    dataRead(PACKET_NAME_UF,&packetTmp);
    packetTmp.chooseRate = true;           //选用治疗的超滤速度#/

    if(packetTmp.ufRate<0||packetTmp.ufRate>10000)
    {
        packetTmp.ufRate=100;
    }

    if(packetTmp.ufVolAim<0||packetTmp.ufVolAim>10000)
    {
        packetTmp.ufVolAim=2000;
    }

    G_UF_SETRATE=packetTmp.ufRate; //治疗启动时#/
    dataNodeUpdate(PACKET_NAME_UF, &packetTmp); //修改超滤速度为治疗的超滤速度#/
    UF_start();//启动超滤泵#/
    prestatusUFPump=true;   //治疗开始处理函数内--超滤泵打开标示#/

    us_sleep(10000);
    //StateSwitch("Dialysis");

    bloodVolumeThread.start();
//    KtV2_Start();    //开始KTV#/  //暂时暂停#/

    scaleVar = treatment_start_to_scale;
    dataNodeUpdate(PACKET_NAME_SCALE_AP, &scaleVar);
    dataNodeUpdate(PACKET_NAME_SCALE_VP, &scaleVar);
    dataNodeUpdate(PACKET_NAME_SCALE_TMP, &scaleVar);

    alarmmodule->Afresh_Alarm();    //进入治疗的时候刷新一次报警

    if(G_SAVECUREPARAM_FLAG==0)
    {
        G_CURE_STARTTIME=G_SYS_CURTIME;  //治疗开始时间#/
        G_CURE_ENDTIME=G_CURE_STARTTIME+residue_secCount;    //治疗结束时间#/
    }

    if(G_NA_PROFILE_FLAG==true)     //NA曲线开始标志 treatment_run() #/
    {
        G_NA_PROFILE_START=G_SYS_CURTIME;  //NA曲线开始时间#/
        G_NA_PROFILE_END=G_SYS_CURTIME+residue_secCount;    //NA曲线结束时间#/
    }

    if(G_UF_PROFILE_FLAG==true)  //UF曲线开始标志#/
    {
      //  G_UF_PROFILE_STARTVOL=0; //UF曲线时段初始超滤量#/
        G_UF_PROFILE_STARTVOL=UFPump_Cure_Amount; //UF曲线时段初始超滤量#/
        G_UF_PROFILE_START=G_SYS_CURTIME;  //UF曲线开始时间#/
        G_UF_PROFILE_END=G_SYS_CURTIME+residue_secCount;   //UF曲线结束时间#/

        unsigned char i;
        float perufvol=0;
        perufvol=G_UF_CURETARGET/10.0; //每段平均超滤量#/
        //曲线应用处理#/
        for(i=0;i<10;i++)
        {
            A_UF_PROFILE[i].ufvol=(A_UF_PROFILE[i].ufrate/100.0)*perufvol; //该时段的超滤目标量#/
        }
    }

    //超净滤器使用次数处理#/
    FilterTimes filterInfo;
    filterInfo=DataSql_GetFilterTimes(1);
    if(filterInfo.remainTime>0&&(filterInfo.usedTime<filterInfo.usedableTime))
    {
        filterInfo.remainTime--;
        filterInfo.usedTime++;
        DataSql_UpdateFilterTimes(filterInfo);
    }

    filterInfo=DataSql_GetFilterTimes(2);
    if(filterInfo.remainTime>0&&(filterInfo.usedTime<filterInfo.usedableTime))
    {
        filterInfo.remainTime--;
        filterInfo.usedTime++;
        DataSql_UpdateFilterTimes(filterInfo);
    }

    timeCount=5;   //进入治疗时初始化#/
    handle_count=0;
    S_Delay_Count=5;

    alarmmodule->Reset_AlarmHandle();//复位报警操作#/

//    MarkDisinfectFinish(false); //记录消毒状态#/


    //+++++++病人信息处理+++++病人信息记录进入治疗时，对默认和没有保存的病人该次治疗信息进行保存#/
    stpatientinfor infor;
    QList<int> list;
    infor.idnum=0;
    infor.timesec=0;

    list=DataSql_patientinforGetNumByIdnum(G_PATIENT_IDNUM_NOW);
    if(list.size()<=0) //尚未写入病人的治疗信息
    {
        G_PATIENT_CUREID_NOW=1;    //当前病人治疗次号
    }
    else
    {
        //从病人信息数据表里面读取一条数据#/
        infor=DataSql_Readpatientinfor(G_PATIENT_IDNUM_NOW,list.last());
        if((infor.idnum!=0)&&(infor.timesec==0))  //有病人本次治疗的信息，但没开始过治疗
        {
            //根据ID删除某一次治疗的病人信息数据内容#/
            DataSql_DeletepatientinforByIdnum(G_PATIENT_IDNUM_NOW,list.last());
            //根据ID删除某一次治疗的病人数据#/
            DataSql_DeletecuredatainforByIdnum(G_PATIENT_IDNUM_NOW,list.last());

            infor.timesec=G_SYS_CURTIME;
            //往病人信息数据表里面插入一条数据  插入数据库缓冲列表#/
            DataList_Insertpatientinfor(infor);

            G_PATIENT_CUREID_NOW=list.last();
        }
        else if((infor.idnum==0)&&(infor.timesec==0))//没有病人本次治疗的信息
        {
           G_PATIENT_CUREID_NOW=list.last()+1;    //当前病人治疗次号
        }
    }

    if((list.size()<=0)||((infor.idnum==0)&&(infor.timesec==0)))
    {
        infor.idnum=G_PATIENT_IDNUM_NOW;
        if(G_PATIENT_CUREID_NOW<1)
            G_PATIENT_CUREID_NOW=1;
        infor.curenum=G_PATIENT_CUREID_NOW;
        infor.timesec=G_SYS_CURTIME;   //没进行过治疗，则时间戳为0
        infor.name="xxx";
        infor.gender=0;
        infor.age=0;
        infor.depart=0;
        infor.weight=0;
        infor.diagno=0;

        infor.time=total_secCount/3600;

        PacketFluid packet;
        dataRead(PACKET_NAME_DIALYSATE,&packet);
        infor.temp=packet.tempTarget;

        FormulaSet formula;
        dataRead(PACKET_FORMULA_TARGET,&formula);
        infor.na=formula.tempBuffer.na;
        infor.hco3=formula.tempBuffer.hco3;

        PacketUF packetUF;
        dataRead(PACKET_NAME_UF, &packetUF);
        infor.ufgoal=packetUF.ufVolAim;

        infor.bldflow=G_SET_BPSPEED;
        infor.subvol=SubsPump_totalvolume;
        PacketPDOHeparinPump heparinPDO;
        dataRead(PACKET_PDO_HEPARINPUMP,&heparinPDO);
        infor.heprate=heparinPDO.speed;
        PacketWorkMode workModeTMP;
        dataRead(PACKET_NAME_WORKMODE,&workModeTMP);
        infor.treatmode=workModeTMP.treatmode.current;

        unsigned int ProfileBit=0;

        if(G_NA_PROFILE_FLAG==true)
        {
            ProfileBit |= DE_NAPROFILE_BIT;
        }
        if(G_UF_PROFILE_FLAG==true)
        {
            ProfileBit |= DE_UFPROFILE_BIT;
        }
        if(G_DIA_PROFILE_FLAG==true)
        {
            ProfileBit |= DE_DIAPROFILE_BIT;
        }
        if(G_HEP_PROFILE_FLAG==true)
        {
            ProfileBit |= DE_HEPPROFILE_BIT;
        }
        if(G_HEP_PROFILE_FLAG==true)
        {
            ProfileBit |= DE_HEPPROFILE_BIT;
        }
        if(G_COND_PROFILE_FLAG==true)
        {
            ProfileBit |= DE_CONDPROFILE_BIT;
        }
        if(G_TEMP_PROFILE_FLAG==true)
        {
            ProfileBit |= DE_TEMPPROFILE_BIT;
        }
        if(G_HCO3_PROFILE_FLAG==true)
        {
            ProfileBit |= DE_HCO3PROFILE_BIT;
        }

        infor.profilemode=ProfileBit;

        //往病人信息数据表里面插入一条数据  插入数据库缓冲列表#/
        DataList_Insertpatientinfor(infor);
    }

    //-----病人信息处理-----
 }

void WidgetSwitch::treatment_stop()
{
    widgetButtonTreat.setChecked(false);
    widgetButtonUFPump.setChecked(false);
    widgetButtonHeparinPump.setChecked(false);


    G_NA_PROFILE_FLAG=false;    //NA曲线治疗结束 treatment_stop() #/
    G_UF_PROFILE_FLAG=false;    //UF曲线治疗结束#/
        //透析液曲线处理函数#/
    G_DIA_PROFILE_FLAG=false;
        //肝素曲线处理函数#/
    G_HEP_PROFILE_FLAG=false;
        //电导曲线处理函数#/
    G_COND_PROFILE_FLAG=false;
        //温度曲线处理函数#/
    G_TEMP_PROFILE_FLAG=false;
        //HCO3曲线处理函数#/
    G_HCO3_PROFILE_FLAG=false;

#if HJX_COMMUNICATION_SWITCH
    emit signalSDO(DE_CMD_Treat_stop);
    emit signalSDO(DE_CMD_ResetBloodAmount);
    emit signalSDO(DE_CMD_ResetUFAmount);
#else
    MasterCtrol_CMD_Treat_stop();
    //清空相关参数
    MasterCtrol_CMD_ResetBloodAmount();
    us_sleep(100000);
    MasterCtrol_CMD_ResetUFAmount();
#endif

    BloodPump_Cure_Amount += BloodPump_Volume;
    UFPump_Cure_Amount += (G_UF_CUREFINISH-UFPump_Cure_Amount);     //治疗结束，累计治疗超滤量#/
    G_UF_CUREFINISH=0;//治疗结束，清0#/

    us_sleep(100000);

    BP_stop();     //停止血泵#/
    us_sleep(100000);

//    MasterCtrol_CMD_SubsStop();  //置换泵#/
//    us_sleep(100000);

    HeparinPump_stop();//停止肝素泵#/

    UF_stop();

    bloodVolumeThread.stop_BloodVolume();
    KtV2_Stop();

    //workstate.current = HD_double_needle_stop;
    //workstate.tip = Cure_stop;
    //dataNodeUpdate(PACKET_NAME_WORKSTATE, &workstate);

    //workstate.current =Cure_stop;

    WorkState  stateTMP=GetWorkStateManage();
    stateTMP.tipState.current = TREAT_FINISH;
    SetWorkStateManage(&stateTMP);//总状态变为结束治疗状态#/

    //为了防止状态切换引发的超滤量计算不对#/
    dataRead(PACKET_MASTERCTRL_UFVALUE,&packetUfValue);
    static_UF_amount=packetUfValue.N3_UF_amount;

    alarmmodule->Custom_Prompt_Clear(PROMPT_HEPARIN_NOOPEN);
    alarmmodule->Afresh_Alarm();    //结束治疗的时候刷新一次报警

    //showNoteDialog("CureFinish");//停止治疗弹出是否结束回血对话框#/

    scaleVar = treatment_stop_to_scale;
    dataNodeUpdate(PACKET_NAME_SCALE_AP, &scaleVar);
    dataNodeUpdate(PACKET_NAME_SCALE_VP, &scaleVar);
    dataNodeUpdate(PACKET_NAME_SCALE_TMP, &scaleVar);

    G_CURE_ENDTIME=G_SYS_CURTIME;    //治疗结束时间#/

 }

void WidgetSwitch::NoteSel(int sel)
{   //"CureFinish"提示框回应函数 和点击回血键处理一样#/
    ReciveCureLeaveNote(sel);
}

//1：消除报警#/
//2：静音#/
//3：护士呼#/
void WidgetSwitch::buttonsHandle(int key)
{
    static char sem_count=0;
    PacketOwnNote palarm_own;
    dataRead(OWNNOTE, &palarm_own);
    int mute=1;

    if(key==1)   //reset#/
    {

        if(sem_count>0&&sem_count<6)  //防止不断重复的按复位键#/
        {
            sem_count++;
            return;
        }
        else
        {
            sem_count++;
        }

        //20151022 修改#/
        if(Alarm_Reset_Finish==false)   //报警复位操作是否完成标志 true 完成
            return;

#if DEBUG_MUTEXDIE_FLAG
    Debug_insert("WidgetSwitch::001");
#endif

        emit RstLockSig(2);

        alarmmodule->Set_WorkState(DE_RESET_FLAG);

        if(audio->alarmSilenceState())   //按复位后，取消静音#/
        {
            mute=2;
            dataNodeUpdate(ALARM_SILENCE_SET,&mute);
        }

        sem_count=0;              
    }
    else if(key==2) //mute#/
    {
//        alarmmodule->Set_WorkState(DE_MUTE_FLAG);#/
        dataNodeUpdate(ALARM_SILENCE_SET,&mute);
    }
    else if(key==3)  //nuse call#/
    {
        palarm_own.note.note1 |=0x02;
        dataNodeUpdate(OWNNOTE, &palarm_own);
    }

#if DEBUG_MUTEXDIE_FLAG
    Debug_insert("WidgetSwitch::002");
#endif
}


