#include "scene_setup_preflush.h"
#include "scene_setup_layout.h"
#include "state.h"
#include "xml_parse.h"
#include "global_var.h"

SceneSetupPreflush::SceneSetupPreflush(QWidget *parent) :
    QObject(parent),
    labelManualPreflushBloodSpeed(parent),
    lineEditManualPreflushBloodSpeed(parent),
    labelManualPreflushBloodSpeedUnit(parent),
    labelPreflushBloodSpeed(parent),
    lineEditPreflushBloodSpeed(parent),
    labelPreflushBloodSpeedUnit(parent),
    lineManualPreflushUfRate(parent),
    lineEditManualPreflushUfRate(parent),
    lineManualPreflushUfRateUnit(parent),
    labelPreflushTime(parent),
    lineEditPreflushTime(parent),
    labelPreflushTimeUnit(parent),
    labelPreflushUfRate(parent),
    lineEditPreflushUfRate(parent),
    labelPreflushUfRateUnit(parent),
    labelPreflushUfVolume(parent),
    lineEditPreflushUfVolume(parent),
    labelPreflushUfVolumeUnit(parent),
    labelReturnBloodVolume(parent),
    lineEditReturnBloodVolume(parent),
    labelReturnBloodVolumeUnit(parent),
    labelPreFlushVolume(parent),
    lineEditPreFlushVolume(parent),
    labelPreFlushVolumeUnit(parent),
    groupPreprime(parent),
    radioAutoPreflush(parent),
    radioManualOperation(parent),
    radioOnlinePreflush(parent),
    Blood_lead(parent),
    lead_yes(parent),
    lead_no(parent),
    Blood_return(parent),
    return_yes(parent),
    return_no(parent),
    buttonSave(parent),
    buttonCancel(parent)
{
    setUi();
    PageAppendPiece("Setup Preflush",this);
    connect(&buttonSave,SIGNAL(clicked()),this,SLOT(save()));

    dataNodeAddObject(PACKET_MAINTAIN_PARAMS, this);
    isKeyInit = false;
//    dataNodeAddObject(PACKET_PREFLUSH, this);
    dataNodeAddObject(BLOOD_CHANGE_PREFLUSH,this);
    dataNodeAddObject(PACKET_ENGINEER, this);
    dataNodeAddObject(PACKET_NAME_BLOODPUMP, this);
    dataNodeAddObject(PACKET_REBIRTH_HANDLE,this);  //重生操作处理#/
}

void SceneSetupPreflush::setUi()
{
    //预冲血泵流速#/
    SCENE_SETUP_CREATE_TABLE(labelManualPreflushBloodSpeed, 1);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditManualPreflushBloodSpeed, 1);
    SCENE_SETUP_CREATE_UNIT(labelManualPreflushBloodSpeedUnit, 1);
    labelManualPreflushBloodSpeedUnit.setText("ml/min");
    //预冲血泵流速#/
    SCENE_SETUP_CREATE_TABLE(labelPreflushBloodSpeed, 2);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditPreflushBloodSpeed, 2);
    SCENE_SETUP_CREATE_UNIT(labelPreflushBloodSpeedUnit, 2);
    labelPreflushBloodSpeedUnit.setText("ml/min");
    //lineEditPreflushBloodSpeed.setDisabled(true);
    //手动预冲超滤速度#/
    SCENE_SETUP_CREATE_TABLE(lineManualPreflushUfRate, 2);
    SCENE_SETUP_CREATE_SHOWSTYLE(lineEditManualPreflushUfRate, 2);
    SCENE_SETUP_CREATE_UNIT(lineManualPreflushUfRateUnit, 2);
    lineManualPreflushUfRateUnit.setText("ml/min");
    lineEditManualPreflushUfRate.setDisabled(true);

    //预冲时间#/
    SCENE_SETUP_CREATE_TABLE(labelPreflushTime, 3);
    SCENE_SETUP_CREATE_SHOWSTYLE(lineEditPreflushTime, 3);
    SCENE_SETUP_CREATE_UNIT(labelPreflushTimeUnit, 3);
    labelPreflushTimeUnit.setText("min");
    lineEditPreflushTime.setDisabled(true);
    //预冲超滤率#/
    SCENE_SETUP_CREATE_TABLE(labelPreflushUfRate, 7);
    SCENE_SETUP_CREATE_SHOWSTYLE(lineEditPreflushUfRate, 7);
    SCENE_SETUP_CREATE_UNIT(labelPreflushUfRateUnit, 7);
    labelPreflushUfRateUnit.setText("ml/h");
    lineEditPreflushUfRate.setDisabled(true);
    //预冲超滤总量#/
    SCENE_SETUP_CREATE_TABLE(labelPreflushUfVolume, 6);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditPreflushUfVolume, 6);
    SCENE_SETUP_CREATE_UNIT(labelPreflushUfVolumeUnit, 6);
    labelPreflushUfVolumeUnit.setText("ml");

    //引血血流量#/
    SCENE_SETUP_CREATE_TABLE(labelReturnBloodVolume, 1);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditReturnBloodVolume, 1);
    SCENE_SETUP_CREATE_UNIT(labelReturnBloodVolumeUnit, 1);
    labelReturnBloodVolumeUnit.setText("ml");
    //已预冲量#/
    SCENE_SETUP_CREATE_TABLE(labelPreFlushVolume, 4);
    SCENE_SETUP_CREATE_SHOWSTYLE(lineEditPreFlushVolume, 4);
    SCENE_SETUP_CREATE_UNIT(labelPreFlushVolumeUnit, 4);
    labelPreFlushVolumeUnit.setText("ml");
    lineEditPreFlushVolume.setDisabled(true);

    //自动预冲#/
    radioAutoPreflush.setGeometry(QRect(345, 350+100, 190, 35));
    RADIOBUTTON_STYLE(radioAutoPreflush);
    radioAutoPreflush.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

    //手动预冲#/
    radioManualOperation.setGeometry(QRect(345, 400+100, 190, 35));
    RADIOBUTTON_STYLE(radioManualOperation);
    radioManualOperation.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

    radioOnlinePreflush.setGeometry(QRect(345, 450+100, 250, 35));
    RADIOBUTTON_STYLE(radioOnlinePreflush);
    radioOnlinePreflush.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

    groupPreprime.addButton(&radioAutoPreflush);
    groupPreprime.addButton(&radioManualOperation);
    groupPreprime.addButton(&radioOnlinePreflush);

    lead_yes.setGeometry(QRect(345, 260, 260, 35));
    RADIOBUTTON_STYLE(lead_yes);
    lead_yes.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

    lead_no.setGeometry(QRect(345, 300, 260, 35));
    RADIOBUTTON_STYLE(lead_no);
    lead_no.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

    Blood_lead.addButton(&lead_yes);
    Blood_lead.addButton(&lead_no);
    connect(&Blood_lead,SIGNAL(buttonClicked(int)),this,SLOT(Group_lead(int)));

    return_yes.setGeometry(QRect(345, 340, 260, 35));
    RADIOBUTTON_STYLE(return_yes);
    return_yes.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

    return_no.setGeometry(QRect(345, 380, 260, 35));
    RADIOBUTTON_STYLE(return_no);
    return_no.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

    Blood_return.addButton(&return_yes);
    Blood_return.addButton(&return_no);
    connect(&Blood_return,SIGNAL(buttonClicked(int)),this,SLOT(Group_return(int)));
    //保存按钮与取消按钮#/
    buttonSave.setGeometry(QRect(620, SCENE_SETUP_ROW_8, 64, 39));
    buttonCancel.setGeometry(QRect(694, SCENE_SETUP_ROW_8, 64, 39));

    BUTTON_SIMPLE_STYLE(buttonSave);
    BUTTON_SIMPLE_STYLE(buttonCancel);

    connect(&radioAutoPreflush, SIGNAL(clicked(bool)), this, SLOT(radioChange()));
    connect(&radioManualOperation, SIGNAL(clicked(bool)), this, SLOT(radioChange()));
    connect(&radioOnlinePreflush, SIGNAL(clicked(bool)), this, SLOT(radioChange()));
    Preflush.PreflushBpVolume=0;
}

void SceneSetupPreflush::Group_lead(int id)
{
    if(id == Blood_lead.id(&lead_yes))
    {
        Blood_online_lead=true;
    }
    else if(id == Blood_lead.id(&lead_no))
    {
        Blood_online_lead=false;
    }
}

void SceneSetupPreflush::Group_return(int id)
{
    if(id == Blood_return.id(&return_yes))
    {
        Blood_online_return=true;
    }
    else if(id == Blood_return.id(&return_no))
    {
        Blood_online_return=false;
    }
}

void SceneSetupPreflush::radioChange(void)
{
    if(sender() == &radioAutoPreflush)   //自动预冲#/
    {
        choosePreflushModel=auto_model;
        labelManualPreflushBloodSpeed.setVisible(false);
        lineEditManualPreflushBloodSpeed.setVisible(false);
        labelManualPreflushBloodSpeedUnit.setVisible(false);

        labelPreflushBloodSpeed.setVisible(true);
        lineEditPreflushBloodSpeed.setVisible(true);
        labelPreflushBloodSpeedUnit.setVisible(true);

        lineManualPreflushUfRate.setVisible(false);
        lineEditManualPreflushUfRate.setVisible(false);
        lineManualPreflushUfRateUnit.setVisible(false);

        labelPreflushTime.setVisible(true);
        lineEditPreflushTime.setVisible(true);
        labelPreflushTimeUnit.setVisible(true);

        labelPreflushUfRate.setVisible(true);
        lineEditPreflushUfRate.setVisible(true);
        labelPreflushUfRateUnit.setVisible(true);

        labelPreflushUfVolume.setVisible(true);
        lineEditPreflushUfVolume.setVisible(true);
        labelPreflushUfVolumeUnit.setVisible(true);

        labelReturnBloodVolume.setVisible(true);
        lineEditReturnBloodVolume.setVisible(true);
        labelReturnBloodVolumeUnit.setVisible(true);

        labelPreFlushVolume.setVisible(true);
        lineEditPreFlushVolume.setVisible(true);
        labelPreFlushVolumeUnit.setVisible(true);

        lead_yes.setVisible(false);
        lead_no.setVisible(false);
        return_yes.setVisible(false);
        return_no.setVisible(false);
        lineEditPreflushUfRate.setDisabled(true);
        SCENE_SETUP_CREATE_SHOWSTYLE(lineEditPreflushUfRate, 7);
        lineEditPreflushUfRate.setText(QString::number(Preflush.PreflushUfRate));
    }

    if(sender() == &radioManualOperation)   //手段预冲#/
    {
        choosePreflushModel=manual_model;
        labelManualPreflushBloodSpeed.setVisible(true);
        lineEditManualPreflushBloodSpeed.setVisible(true);
        labelManualPreflushBloodSpeedUnit.setVisible(true);

        labelPreflushBloodSpeed.setVisible(false);
        lineEditPreflushBloodSpeed.setVisible(false);
        labelPreflushBloodSpeedUnit.setVisible(false);

        lineManualPreflushUfRate.setVisible(false);
        lineEditManualPreflushUfRate.setVisible(false);
        lineManualPreflushUfRateUnit.setVisible(false);

        labelPreflushTime.setVisible(false);
        lineEditPreflushTime.setVisible(false);
        labelPreflushTimeUnit.setVisible(false);

        labelPreflushUfRate.setVisible(true);
        lineEditPreflushUfRate.setVisible(true);
        labelPreflushUfRateUnit.setVisible(true);

        labelPreflushUfVolume.setVisible(false);
        lineEditPreflushUfVolume.setVisible(false);
        labelPreflushUfVolumeUnit.setVisible(false);

        labelReturnBloodVolume.setVisible(false);
        lineEditReturnBloodVolume.setVisible(false);
        labelReturnBloodVolumeUnit.setVisible(false);

        labelPreFlushVolume.setVisible(false);
        lineEditPreFlushVolume.setVisible(false);
        labelPreFlushVolumeUnit.setVisible(false);

        lead_yes.setVisible(false);
        lead_no.setVisible(false);
        return_yes.setVisible(false);
        return_no.setVisible(false);
        lineEditPreflushUfRate.setDisabled(false);
        SCENE_SETUP_CREATE_LINEEDIT(lineEditPreflushUfRate, 7);
    }

    if(sender() == &radioOnlinePreflush)   //online自动预冲#/
    {
        choosePreflushModel=online_model;
        labelManualPreflushBloodSpeed.setVisible(false);
        lineEditManualPreflushBloodSpeed.setVisible(false);
        labelManualPreflushBloodSpeedUnit.setVisible(false);

        labelPreflushBloodSpeed.setVisible(true);
        lineEditPreflushBloodSpeed.setVisible(true);
        labelPreflushBloodSpeedUnit.setVisible(true);

        lineManualPreflushUfRate.setVisible(false);
        lineEditManualPreflushUfRate.setVisible(false);
        lineManualPreflushUfRateUnit.setVisible(false);

        labelPreflushTime.setVisible(true);
        lineEditPreflushTime.setVisible(true);
        labelPreflushTimeUnit.setVisible(true);

        labelPreflushUfRate.setVisible(false);
        lineEditPreflushUfRate.setVisible(false);
        labelPreflushUfRateUnit.setVisible(false);

        labelPreflushUfVolume.setVisible(false);
        lineEditPreflushUfVolume.setVisible(false);
        labelPreflushUfVolumeUnit.setVisible(false);

        labelReturnBloodVolume.setVisible(true);
        lineEditReturnBloodVolume.setVisible(true);
        labelReturnBloodVolumeUnit.setVisible(true);

        labelPreFlushVolume.setVisible(true);
        lineEditPreFlushVolume.setVisible(true);
        labelPreFlushVolumeUnit.setVisible(true);

        lead_yes.setVisible(true);
        lead_no.setVisible(true);
        return_yes.setVisible(true);
        return_no.setVisible(true);
        lineEditPreflushUfRate.setDisabled(true);
        SCENE_SETUP_CREATE_SHOWSTYLE(lineEditPreflushUfRate, 7);
        lineEditPreflushUfRate.setText(QString::number(Preflush.PreflushUfRate));
    }
    /*
    PacketWorkMode workModeTMP;
    dataRead(PACKET_NAME_WORKMODE,&workModeTMP);
    if(workModeTMP.treatmode.current==HDF_pre_dilute||workModeTMP.treatmode.current==HDF_post_dilute\
       ||workModeTMP.treatmode.current==HF_pre_dilute||workModeTMP.treatmode.current==HF_post_dilute)
    {
        //预冲超滤率
        labelPreflushUfRate.setVisible(false);
        lineEditPreflushUfRate.setVisible(false);
        labelPreflushUfRateUnit.setVisible(false);
        //预冲超滤量
        labelPreflushUfVolume.setVisible(false);
        lineEditPreflushUfVolume.setVisible(false);
        labelPreflushUfVolumeUnit.setVisible(false);
        //手动预冲超滤速度
        lineManualPreflushUfRate.setVisible(false);
        lineEditManualPreflushUfRate.setVisible(false);
        lineManualPreflushUfRateUnit.setVisible(false);
    }
    */
    save();
}

void SceneSetupPreflush::CustomSetText()
{
    //预冲血泵流速
    labelManualPreflushBloodSpeed.setText(tr("Blood Pump Flow"));
    //预冲血泵流速
    labelPreflushBloodSpeed.setText(tr("Blood Pump Flow"));
    //预冲超滤率
    lineManualPreflushUfRate.setText(tr("Uf Rate"));
    //预冲时间
    labelPreflushTime.setText(tr("Preflush Time"));
    //预冲超滤率
    labelPreflushUfRate.setText(tr("Uf Rate"));
    //预冲超滤量
    labelPreflushUfVolume.setText(tr("Uf Volume"));
    //引血血流量
    labelReturnBloodVolume.setText(tr("Preflush Volume"));
    //
    labelPreFlushVolume.setText(tr("Acc Preflush Volume"));

    //自动预冲
    radioAutoPreflush.setText(tr("Auto"));
    //手动预冲
    radioManualOperation.setText(tr("Manual"));
    radioOnlinePreflush.setText(tr("Online"));
    lead_yes.setText(tr("Enable online lead-in"));
    lead_no.setText(tr("Disable online lead-in"));
    return_yes.setText(tr("Enable online return"));
    return_no.setText(tr("Disable online return"));
    //保存按钮#/
    buttonSave.setText(tr("Confirm"));
    //取消按钮#/
    buttonCancel.setText(tr("Cancel"));
}

void SceneSetupPreflush::ActionHandle(const void *dataPtr, int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupPreflush-ActionHandle-in");
#endif

    if(workstate.tipState.current==DEBUG)
        return;

    if(dataPtr == NULL || name == 0) {
        return;
    }

    if (name==BLOOD_CHANGE_PREFLUSH) {//更新预冲参数#/
        PacketSdoBlood blood1;
        blood1 = *((PacketSdoBlood *)dataPtr);

        if(choosePreflushModel==auto_model||choosePreflushModel==online_model)
        {
            lineEditPreflushBloodSpeed.setText(QString::number(blood1.N3_set_BPspeed));
        }
        else if(choosePreflushModel==manual_model)
        {
            lineEditManualPreflushBloodSpeed.setText(QString::number(blood1.N3_set_BPspeed));
        }
        save();
        return;
    }

    if (name==PACKET_NAME_BLOODPUMP) {//血路流量更新#/

        struct PacketBloodPump *packet = (struct PacketBloodPump*)dataPtr;

        if(G_rebirth_flag==true)  //重生操作时取消处理#/
            return;

        if(auto_model ==choosePreflushModel||online_model==choosePreflushModel){
            WorkState  stateTMP=GetWorkStateManage();
            static int updateOneTimeAgain=0;
            static unsigned short int PreflushBasicBpvol = 0;//预冲前血泵已运行量#/

            if(stateTMP.tipState.current==TREAT_READY)
            {
                if(stateTMP.subState.current<PREFLUSH_RUN_FLUID)
                {
                    if((packet->N9_amount<PreflushBasicBpvol)&&PreflushBasicBpvol!=0)//清空血泵累计量时的处理#/
                    {
                        PreflushBasicBpvol = packet->N9_amount;
                        Preflush.PreflushBpVolume=0;
                        lineEditPreFlushVolume.setText(QString::number(0));//更新预冲界面的预冲量#/
                    }
                }

            if((PREFLUSH_RUN_FLUID == stateTMP.subState.current)||(PREFLUSH_RUN_NO_FLUID == stateTMP.subState.current)){
                updateOneTimeAgain=3;
            }

            if(((PREFLUSH_RUN_FLUID == stateTMP.subState.current)||(PREFLUSH_RUN_NO_FLUID == stateTMP.subState.current))\
                    || updateOneTimeAgain>0){
                updateOneTimeAgain--;//正常预冲时，updateOneTimeAgain=2；stateTMP.subState.current=PREFLUSH_STOP时再更新一次#/

                if(packet->N9_amount < PreflushBasicBpvol)
                    PreflushBasicBpvol = packet->N9_amount;

                if((packet->N9_amount - PreflushBasicBpvol) >= Preflush.TractBloodVolume){
                    Preflush.PreflushBpVolume += Preflush.TractBloodVolume;
                    PreflushBasicBpvol=packet->N9_amount;
                    lineEditPreFlushVolume.setText(QString::number(Preflush.PreflushBpVolume));//该作用保证已预冲量不超过设置量
                }else{
                    lineEditPreFlushVolume.setText(QString::number(Preflush.PreflushBpVolume+packet->N9_amount - PreflushBasicBpvol));//更新预冲界面的预冲量
                }

                lineEditPreFlushVolume.setText(QString::number(BloodPump_Pre_Amount+BloodPump_Volume));
            }
            else if(PREFLUSH_STOP_FLUID != stateTMP.subState.current\
                    && PREFLUSH_STOP_NO_FLUID != stateTMP.subState.current)
                {
                    PreflushBasicBpvol=packet->N9_amount;
                }
            }//if(stateTMP.tipState.current==TREAT_READY)
        }
        return;
    }

    if(name==PACKET_MAINTAIN_PARAMS) {
        params = (Params*)dataPtr;

        Preflush.BPRate.ph1=params->Params_PreFlush.BpRate_phase1.attribute.default_value;
          if(Preflush.BPRate.ph1<10)Preflush.BPRate.ph1=10;
        Preflush.BPRate.ph2=params->Params_PreFlush.BpRate_phase2.attribute.default_value;
          if(Preflush.BPRate.ph2<10)Preflush.BPRate.ph2=10;
        Preflush.BPRate.ph3=params->Params_PreFlush.BpRate_phase3.attribute.default_value;
          if(Preflush.BPRate.ph3<10)Preflush.BPRate.ph3=10;

        //手动预冲的超滤速度，只能在工程界面设置#/
        Preflush.PreflushManualUF_Rate = params->Params_PreFlush.PF_UF_Rate.attribute.default_value;
        Preflush.PreflushManualBloodRate = params->Params_PreFlush.BpRate.attribute.default_value;

        Preflush.PreflushUfVolume = params->Params_PreFlush.PF_UF_Volume.attribute.default_value;
        Preflush.TractBloodVolume = params->Params_PreFlush.PF_Volume.attribute.default_value;
        Preflush.PreflushBloodSpeed = params->Params_PreFlush.BpRate_phase1.attribute.default_value;
        Preflush.PreflushBpVolume = 0;
        Preflush.PreflushTime=15;
        Preflush.PreflushUfRate=1000;

        lineEditPreflushBloodSpeed.setText(QString::number(Preflush.PreflushBloodSpeed));//auto显示自动预冲第一段速率
        lineEditPreflushTime.setText(QString::number(Preflush.PreflushTime,'f',1));   //auto，时间
        lineEditPreflushUfRate.setText(QString::number(Preflush.PreflushUfRate));   //auto，超滤率
        lineEditPreflushUfVolume.setText(QString::number(Preflush.PreflushUfVolume));//超滤总量
        lineEditReturnBloodVolume.setText(QString::number(Preflush.TractBloodVolume));//冲注量 ，引血量
        lineEditPreFlushVolume.setText(QString::number(Preflush.PreflushBpVolume));//已预冲量

        lineEditManualPreflushBloodSpeed.setText(QString::number(Preflush.PreflushManualBloodRate));//manual
        lineEditManualPreflushUfRate.setText(QString::number(Preflush.PreflushManualUF_Rate));//manual

        /*
        Engineer tmpset;
        dataRead(PACKET_ENGINEER,&tmpset);
        if(tmpset.factoryset.PrimeModel==true)
        {
              radioManualOperation.setChecked(true);
              choosePreflushModel=manual_model;
        }
        else
        {
              radioAutoPreflush.setChecked(true);
              choosePreflushModel=auto_model;
        }
        */
        save();//通过save()计算时间、超滤速率;#/
    }
    else if(name==PACKET_ENGINEER) {
        engineerset = (Engineer*)dataPtr;
        if(engineerset->factoryset.PrimeModel==1)
           {
              radioManualOperation.setChecked(true);
              choosePreflushModel=manual_model;
              //radioManualOperation.click();//.animateClick();
           }
        else if(engineerset->factoryset.PrimeModel==0)
        {
              radioAutoPreflush.setChecked(true);
              choosePreflushModel=auto_model;
              //radioAutoPreflush.click();
        }
        else if(engineerset->factoryset.PrimeModel==2)
        {
            if(engineerset->factoryset.CureModeSelect==HDF_pre_dilute||engineerset->factoryset.CureModeSelect==HDF_post_dilute||\
               engineerset->factoryset.CureModeSelect==HF_pre_dilute||engineerset->factoryset.CureModeSelect==HF_post_dilute)
            {
                radioOnlinePreflush.setChecked(true);
                choosePreflushModel=online_model;
            }
            else
            {
                radioAutoPreflush.setChecked(true);
                choosePreflushModel=auto_model;
            }
        }


        if(engineerset->factoryset.onlinegetblood==1)
        {
            Blood_online_lead=true;
            lead_yes.setChecked(true);
        }
        else if(engineerset->factoryset.onlinegetblood==0)
        {
            Blood_online_lead=false;
            lead_no.setChecked(true);
        }

        if(engineerset->factoryset.onlinereturnblood==1)
        {
            Blood_online_return=true;
            return_yes.setChecked(true);
        }
        else if(engineerset->factoryset.onlinereturnblood==0)
        {
            Blood_online_return=false;
            return_no.setChecked(true);
        }

        save();//通过save()计算时间、超滤速率;#/
    }
    else if(name==PACKET_REBIRTH_HANDLE) //重生恢复操作处理#/
    {
        Blood_online_lead=G_P_REBIRTH->Blood_online_lead;   //在线引血标志#/
        Blood_online_return=G_P_REBIRTH->Blood_online_return; //在线回血标志#/
        Startpreflush_mode=G_P_REBIRTH->Startpreflush_mode;  //启动预冲或引血的方式#/
        Startreturnblood_mode=G_P_REBIRTH->Startreturnblood_mode; //启动回血的方式#/

        Preflush=G_P_REBIRTH->Preflush;   //预冲参数#/

        lineEditPreflushBloodSpeed.setText(QString::number(Preflush.PreflushBloodSpeed));//auto显示自动预冲第一段速率
        lineEditPreflushTime.setText(QString::number(Preflush.PreflushTime,'f',1));   //auto，时间
        lineEditPreflushUfRate.setText(QString::number(Preflush.PreflushUfRate));   //auto，超滤率
        lineEditPreflushUfVolume.setText(QString::number(Preflush.PreflushUfVolume));//超滤总量
        lineEditReturnBloodVolume.setText(QString::number(Preflush.TractBloodVolume));//冲注量 ，引血量
        lineEditPreFlushVolume.setText(QString::number(Preflush.PreflushBpVolume));//已预冲量

        lineEditManualPreflushBloodSpeed.setText(QString::number(Preflush.PreflushManualBloodRate));//manual
        lineEditManualPreflushUfRate.setText(QString::number(Preflush.PreflushManualUF_Rate));//manual

        if(Preflush.PreflushModel==manual_model)
        {
            radioManualOperation.setChecked(true);
            choosePreflushModel=manual_model;
        }
        else if(Preflush.PreflushModel==auto_model)
        {
            radioAutoPreflush.setChecked(true);
            choosePreflushModel=auto_model;
        }
        else
        {
            if(engineerset->factoryset.CureModeSelect==HDF_pre_dilute||engineerset->factoryset.CureModeSelect==HDF_post_dilute||\
               engineerset->factoryset.CureModeSelect==HF_pre_dilute||engineerset->factoryset.CureModeSelect==HF_post_dilute)
            {
                radioOnlinePreflush.setChecked(true);
                choosePreflushModel=online_model;
            }
            else
            {
                radioAutoPreflush.setChecked(true);
                choosePreflushModel=auto_model;
            }
        }

        if(Blood_online_lead==true)
            lead_yes.setChecked(true);
        else
            lead_no.setChecked(true);

        if(Blood_online_return==true)
            return_yes.setChecked(true);
        else
           return_no.setChecked(true);

        save();//通过save()计算时间、超滤速率;#/
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupPreflush-ActionHandle-out");
#endif
}

void SceneSetupPreflush::lineEdit_init()
{
    ic->appendWidget(&lineEditManualPreflushBloodSpeed, NumInputMethod, LineEdit, this,
                     params->Params_PreFlush.BpRate.attribute.min_value,
                     params->Params_PreFlush.BpRate.attribute.max_value,
                     params->Params_PreFlush.BpRate.attribute.step_value,
                     tr("Preflush BP Speed:"),"Preflush BP Speed");

    ic->appendWidget(&lineEditPreflushBloodSpeed, NumInputMethod, LineEdit, this,
                     params->Params_PreFlush.BpRate.attribute.min_value,
                     params->Params_PreFlush.BpRate.attribute.max_value,
                     params->Params_PreFlush.BpRate.attribute.step_value,
                     tr("Preflush Blood Pump Speed:"),"Preflush Blood Pump Speed");

    ic->appendWidget(&lineEditManualPreflushUfRate, NumInputMethod, LineEdit, this,
                     params->Params_PreFlush.Dialyser_Rate.attribute.min_value,
                     params->Params_PreFlush.Dialyser_Rate.attribute.max_value,
                     params->Params_PreFlush.Dialyser_Rate.attribute.step_value,
                     tr("Dialysate Flow:"),"Dialysate Flow");

//    ic->appendWidget(&lineEditPreflushTime, NumInputMethod, LineEdit, this,
//                     params->Params_PreFlush.Time.attribute.min_value,
//                     params->Params_PreFlush.Time.attribute.max_value,
//                     params->Params_PreFlush.Time.attribute.step_value,
//                     tr("Preflush Time:"),"Preflush Time");

    ic->appendWidget(&lineEditPreflushUfRate, NumInputMethod, LineEdit, this,
                     params->Params_PreFlush.PF_UF_Rate.attribute.min_value,
                     params->Params_PreFlush.PF_UF_Rate.attribute.max_value,
                     params->Params_PreFlush.PF_UF_Rate.attribute.step_value,
                     tr("Preflush Uf Rate:"),"Preflush Uf Rate");

    ic->appendWidget(&lineEditPreflushUfVolume, NumInputMethod, LineEdit, this,
                     params->Params_PreFlush.PF_UF_Volume.attribute.min_value,
                     params->Params_PreFlush.PF_UF_Volume.attribute.max_value,
                     params->Params_PreFlush.PF_UF_Volume.attribute.step_value,
                     tr("Preflush Uf Volume:"),"Preflush Uf Volume");

    ic->appendWidget(&lineEditReturnBloodVolume, NumInputMethod, LineEdit, this,
                     params->Params_PreFlush.PF_Volume.attribute.min_value,
                     params->Params_PreFlush.PF_Volume.attribute.max_value,
                     params->Params_PreFlush.PF_Volume.attribute.step_value,
                     tr("Return Blood Volume:"),"Return Blood Volume");
}

void SceneSetupPreflush::setCusVisible(bool vis, char *para)
{
    para = para;
    if(vis && !isKeyInit) {
        isKeyInit = true;
        lineEdit_init();
    }
    if(vis==false)
    {
        labelPreflushBloodSpeed.setVisible(vis);
        lineEditPreflushBloodSpeed.setVisible(vis);
        labelPreflushBloodSpeedUnit.setVisible(vis);

        lineManualPreflushUfRate.setVisible(vis);
        lineEditManualPreflushUfRate.setVisible(vis);
        lineManualPreflushUfRateUnit.setVisible(vis);

        labelPreflushTime.setVisible(vis);
        lineEditPreflushTime.setVisible(vis);
        labelPreflushTimeUnit.setVisible(vis);

        labelPreflushUfRate.setVisible(vis);
        lineEditPreflushUfRate.setVisible(vis);
        labelPreflushUfRateUnit.setVisible(vis);

        labelPreflushUfVolume.setVisible(vis);
        lineEditPreflushUfVolume.setVisible(vis);
        labelPreflushUfVolumeUnit.setVisible(vis);

        labelReturnBloodVolume.setVisible(vis);
        lineEditReturnBloodVolume.setVisible(vis);
        labelReturnBloodVolumeUnit.setVisible(vis);

        labelPreFlushVolume.setVisible(vis);
        lineEditPreFlushVolume.setVisible(vis);
        labelPreFlushVolumeUnit.setVisible(vis);


        labelManualPreflushBloodSpeed.setVisible(vis);
        lineEditManualPreflushBloodSpeed.setVisible(vis);
        labelManualPreflushBloodSpeedUnit.setVisible(vis);

        radioOnlinePreflush.setVisible(vis);
        lead_yes.setVisible(vis);
        lead_no.setVisible(vis);
        return_yes.setVisible(vis);
        return_no.setVisible(vis);
        //radioAutoPreflush.setVisible(vis);
        //radioManualOperation.setVisible(vis);
    }
    else
    {
        struct PacketSdoBlood sdoBlood;
        dataRead(PACKET_SDO_BLOOD,&sdoBlood);
        lineEditPreflushBloodSpeed.setText(QString::number(sdoBlood.N3_set_BPspeed));

        PacketWorkMode workModeTMP;
        dataRead(PACKET_NAME_WORKMODE,&workModeTMP);
        if(workModeTMP.treatmode.current==HDF_pre_dilute||workModeTMP.treatmode.current==HDF_post_dilute\
           ||workModeTMP.treatmode.current==HF_pre_dilute||workModeTMP.treatmode.current==HF_post_dilute)
        {
            radioOnlinePreflush.setVisible(true);
        }
        else
        {
            radioOnlinePreflush.setVisible(false);
            if(choosePreflushModel==online_model)
                choosePreflushModel=auto_model;
        }

        if(choosePreflushModel==auto_model)
       {
        lineEditPreflushBloodSpeed.setText(QString::number(Preflush.PreflushBloodSpeed));//auto显示自动预冲第一段速率
        radioAutoPreflush.setChecked(true);
        labelManualPreflushBloodSpeed.setVisible(false);
        lineEditManualPreflushBloodSpeed.setVisible(false);
        labelManualPreflushBloodSpeedUnit.setVisible(false);

        labelPreflushBloodSpeed.setVisible(true);
        lineEditPreflushBloodSpeed.setVisible(true);
        labelPreflushBloodSpeedUnit.setVisible(true);

        lineManualPreflushUfRate.setVisible(false);
        lineEditManualPreflushUfRate.setVisible(false);
        lineManualPreflushUfRateUnit.setVisible(false);

        labelPreflushTime.setVisible(true);
        lineEditPreflushTime.setVisible(true);
        labelPreflushTimeUnit.setVisible(true);

        labelPreflushUfRate.setVisible(true);
        lineEditPreflushUfRate.setVisible(true);
        labelPreflushUfRateUnit.setVisible(true);

        labelPreflushUfVolume.setVisible(true);
        lineEditPreflushUfVolume.setVisible(true);
        labelPreflushUfVolumeUnit.setVisible(true);

        labelReturnBloodVolume.setVisible(true);
        lineEditReturnBloodVolume.setVisible(true);
        labelReturnBloodVolumeUnit.setVisible(true);

        labelPreFlushVolume.setVisible(true);
        lineEditPreFlushVolume.setVisible(true);
        labelPreFlushVolumeUnit.setVisible(true);

//        radioOnlinePreflush.setVisible(false);
        lead_yes.setVisible(false);
        lead_no.setVisible(false);
        return_yes.setVisible(false);
        return_no.setVisible(false);
     }
    else if(choosePreflushModel==manual_model){
        SCENE_SETUP_CREATE_LINEEDIT(lineEditPreflushUfRate, 7);
        lineEditManualPreflushBloodSpeed.setText(QString::number(Preflush.PreflushManualBloodRate));//manual
        radioManualOperation.setChecked(true);
        labelManualPreflushBloodSpeed.setVisible(true);
        lineEditManualPreflushBloodSpeed.setVisible(true);
        labelManualPreflushBloodSpeedUnit.setVisible(true);

        labelPreflushBloodSpeed.setVisible(false);
        lineEditPreflushBloodSpeed.setVisible(false);
        labelPreflushBloodSpeedUnit.setVisible(false);

        lineManualPreflushUfRate.setVisible(false);
        lineEditManualPreflushUfRate.setVisible(false);
        lineManualPreflushUfRateUnit.setVisible(false);

        labelPreflushTime.setVisible(false);
        lineEditPreflushTime.setVisible(false);
        labelPreflushTimeUnit.setVisible(false);

        labelPreflushUfRate.setVisible(true);
        lineEditPreflushUfRate.setVisible(true);
        labelPreflushUfRateUnit.setVisible(true);

        labelPreflushUfVolume.setVisible(false);
        lineEditPreflushUfVolume.setVisible(false);
        labelPreflushUfVolumeUnit.setVisible(false);

        labelReturnBloodVolume.setVisible(false);
        lineEditReturnBloodVolume.setVisible(false);
        labelReturnBloodVolumeUnit.setVisible(false);

        labelPreFlushVolume.setVisible(false);
        lineEditPreFlushVolume.setVisible(false);
        labelPreFlushVolumeUnit.setVisible(false);

        lineEditPreflushUfRate.setDisabled(false);
 //       radioOnlinePreflush.setVisible(false);
        lead_yes.setVisible(false);
        lead_no.setVisible(false);
        return_yes.setVisible(false);
        return_no.setVisible(false);
      }
      else if(choosePreflushModel==online_model)
      {
            lineEditPreflushBloodSpeed.setText(QString::number(Preflush.PreflushBloodSpeed));//auto显示自动预冲第一段速率
            radioOnlinePreflush.setChecked(true);
            labelManualPreflushBloodSpeed.setVisible(false);
            lineEditManualPreflushBloodSpeed.setVisible(false);
            labelManualPreflushBloodSpeedUnit.setVisible(false);

            labelPreflushBloodSpeed.setVisible(true);
            lineEditPreflushBloodSpeed.setVisible(true);
            labelPreflushBloodSpeedUnit.setVisible(true);

            lineManualPreflushUfRate.setVisible(false);
            lineEditManualPreflushUfRate.setVisible(false);
            lineManualPreflushUfRateUnit.setVisible(false);

            labelPreflushTime.setVisible(true);
            lineEditPreflushTime.setVisible(true);
            labelPreflushTimeUnit.setVisible(true);

            labelPreflushUfRate.setVisible(false);
            lineEditPreflushUfRate.setVisible(false);
            labelPreflushUfRateUnit.setVisible(false);

            labelPreflushUfVolume.setVisible(false);
            lineEditPreflushUfVolume.setVisible(false);
            labelPreflushUfVolumeUnit.setVisible(false);

            labelReturnBloodVolume.setVisible(true);
            lineEditReturnBloodVolume.setVisible(true);
            labelReturnBloodVolumeUnit.setVisible(true);

            labelPreFlushVolume.setVisible(true);
            lineEditPreFlushVolume.setVisible(true);
            labelPreFlushVolumeUnit.setVisible(true);

            lead_yes.setVisible(true);
            lead_no.setVisible(true);
            return_yes.setVisible(true);
            return_no.setVisible(true);
      }
    }
    radioAutoPreflush.setVisible(vis);
    radioManualOperation.setVisible(vis);
    buttonSave.setVisible(false);
    buttonCancel.setVisible(false);
}

void SceneSetupPreflush::save()
{
    if (radioAutoPreflush.isChecked() == true)//自动预冲#/
    {
        WorkState  stateTMP=GetWorkStateManage();
        if(SELFTEST_COMPLETE_FLUID <= stateTMP.subState.current && stateTMP.subState.current<= GETBLOOD_STOP_FLUID){//修改血泵速率hou，不分段运行
            if((Preflush.PreflushBloodSpeed!=lineEditPreflushBloodSpeed.text().toUShort())\
                    ||(Preflush.TractBloodVolume!=lineEditReturnBloodVolume.text().toUShort())\
                    ||(Preflush.PreflushUfVolume!=lineEditPreflushUfVolume.text().toUShort())){//自动预冲的设置参数没有变化则，不影响分段
                Preflush.PreflushBloodSpeed = lineEditPreflushBloodSpeed.text().toUShort();
                Preflush.BPRate.ph1 = Preflush.BPRate.ph2 = Preflush.BPRate.ph3 = Preflush.PreflushBloodSpeed;
            }
        }
        else{
            lineEditPreflushBloodSpeed.setText(QString::number(Preflush.PreflushBloodSpeed));
        }

        if(Preflush.BPRate.ph1<10)Preflush.BPRate.ph1=10;
        if(Preflush.BPRate.ph2<10)Preflush.BPRate.ph2=10;
        if(Preflush.BPRate.ph3<10)Preflush.BPRate.ph3=10;

        Preflush.PreflushUfVolume= lineEditPreflushUfVolume.text().toUShort();//获取设置的预冲超滤量#/
        Preflush.TractBloodVolume= lineEditReturnBloodVolume.text().toUShort();//获取设置的预冲量#/

        float  PreflushTimeTMP = (float)(Preflush.TractBloodVolume*10.0/3/(Preflush.BPRate.ph1) +
                               Preflush.TractBloodVolume*10.0/3/(Preflush.BPRate.ph2) +
                               Preflush.TractBloodVolume*10.0/3/(Preflush.BPRate.ph3) );
        PreflushTimeTMP /= 10; //单位分钟#/
        Preflush.PreflushTime =  (unsigned short int)PreflushTimeTMP;
        lineEditPreflushTime.setText(QString::number(PreflushTimeTMP,'f',1));//计算预冲时间#/

        if(PreflushTimeTMP>0)
        {
            Preflush.PreflushUfRate =(unsigned short int)(Preflush.PreflushUfVolume*60/PreflushTimeTMP);
            lineEditPreflushUfRate.setText(QString::number(Preflush.PreflushUfRate));//计算超滤速率#/
        }        
        choosePreflushModel=auto_model;//
        Preflush.PreflushModel=auto_model;
    }
    else if(radioManualOperation.isChecked() == true){   //手动预冲#/
        Preflush.PreflushManualBloodRate = lineEditManualPreflushBloodSpeed.text().toUShort();
        Preflush.PreflushUfVolume= lineEditPreflushUfVolume.text().toUShort();//获取设置的预冲超滤量#/
        Preflush.PreflushManualUF_Rate=lineEditPreflushUfRate.text().toUShort(); //获取设置的超滤速度#/
        choosePreflushModel=manual_model;
        Preflush.PreflushModel=manual_model;
    }
    else if(radioOnlinePreflush.isChecked()==true)    //在线自动预冲#/
    {
        WorkState  stateTMP=GetWorkStateManage();
        if(SELFTEST_COMPLETE_FLUID <= stateTMP.subState.current && stateTMP.subState.current<= GETBLOOD_STOP_FLUID){//修改血泵速率hou，不分段运行
            if((Preflush.PreflushBloodSpeed!=lineEditPreflushBloodSpeed.text().toUShort())\
                    ||(Preflush.TractBloodVolume!=lineEditReturnBloodVolume.text().toUShort())\
                    ||(Preflush.PreflushUfVolume!=lineEditPreflushUfVolume.text().toUShort())){//自动预冲的设置参数没有变化则，不影响分段
                Preflush.PreflushBloodSpeed = lineEditPreflushBloodSpeed.text().toUShort();
                Preflush.BPRate.ph1 = Preflush.BPRate.ph2 = Preflush.BPRate.ph3 = Preflush.PreflushBloodSpeed;
            }
        }
        else{
            lineEditPreflushBloodSpeed.setText(QString::number(Preflush.PreflushBloodSpeed));
        }
        if(Preflush.BPRate.ph1<10)Preflush.BPRate.ph1=10;
        if(Preflush.BPRate.ph2<10)Preflush.BPRate.ph2=10;
        if(Preflush.BPRate.ph3<10)Preflush.BPRate.ph3=10;

        Preflush.PreflushUfVolume= lineEditPreflushUfVolume.text().toUShort();//获取设置的预冲超滤量
        Preflush.TractBloodVolume= lineEditReturnBloodVolume.text().toUShort();//获取设置的预冲量

        float  PreflushTimeTMP = (float)(Preflush.TractBloodVolume*10/3/(Preflush.BPRate.ph1) +
                               Preflush.TractBloodVolume*10/3/(Preflush.BPRate.ph2) +
                               Preflush.TractBloodVolume*10/3/(Preflush.BPRate.ph3) );
        PreflushTimeTMP /= 10; //单位分钟#/
        Preflush.PreflushTime =  (unsigned short int)PreflushTimeTMP;
        lineEditPreflushTime.setText(QString::number(PreflushTimeTMP,'f',1));//计算预冲时间#/

        if(PreflushTimeTMP>0)
        {

            Preflush.PreflushUfRate =(unsigned short int)(Preflush.PreflushUfVolume*60/PreflushTimeTMP);
            lineEditPreflushUfRate.setText(QString::number(Preflush.PreflushUfRate));//计算超滤速率
        }

        choosePreflushModel=online_model;//
        Preflush.PreflushModel=online_model;

    }
//    UFPump_Pre_Amount += G_UF_PREFINISH;
//    G_UF_PREFINISH=0;

    G_UF_PRETARGET=Preflush.PreflushUfVolume;  //预冲阶段超滤目标量#/

    WorkState stateTMP=GetWorkStateManage();
    if(stateTMP.tipState.current <= TREAT_READY){//准备状态#/

        G_UF_STARTTIME=G_SYS_CURTIME;      //超滤开启时间 单位秒#/
        G_UF_ENDTIME=G_SYS_CURTIME+Preflush.PreflushTime*60;  //超滤的总时间 单位秒#/
        G_UF_FINISHVOL_FLAG=false;

        if(choosePreflushModel==manual_model)
            G_UF_SETRATE=Preflush.PreflushManualUF_Rate;//预冲参数改变时#/
        else
            G_UF_SETRATE=Preflush.PreflushUfRate;  //预冲参数改变时#/

    }

    dataNodeUpdate(PACKET_PREFLUSH, &Preflush);
    dataNodeUpdate(Preflush_Model,&choosePreflushModel);
}

//void SceneSetupPreflush::ActionHandle(const void* dataPtr,int name)
//{
//    if ((dataPtr == NULL) || (name == NULL)) {
//        return;
//    }

//    if (strcmp(name,PACKET_PREFLUSH) == 0){
//        Preflush = *((PacketPreflush*) dataPtr);
//        lineEditPreflushBloodSpeed.setText(QString::number(Preflush.PreflushBloodSpeed));
//        lineEditManualPreflushUfRate.setText(QString::number(Preflush.PreflushDialysateFlow));
//        lineEditPreflushTime.setText(QString::number(Preflush.PreflushTime));
//        lineEditPreflushUfRate.setText(QString::number(Preflush.PreflushUfRate));
//        lineEditPreflushUfVolume.setText(QString::number(Preflush.PreflushUfVolume));
//        lineEditReturnBloodVolume.setText(QString::number(Preflush.TractBloodVolume));
//    }
//}

//void SceneSetupPreflush::CustomXmlConfig(const char *file, const char *cmd)
//{
//    file=file;
//    QMap<QString, QString> mapV;
//    if (strcmp(cmd, DEFAULT_XML_LOAD) == 0) {
//        /*
//        if ((xml_NodeReadValue(file, (char *)"/Config/Heparin/Default", &mapV) ==0) &&
//                (mapV.isEmpty() == false)) {
//            labelSyringeStyleValue.setNum(mapV["syringType"].toInt());
//            lineEditFirstDose.setText(mapV["firstDose"]);

//        }*/
//    } else if (strcmp(cmd, DEFAULT_XML_SAVE) == 0) {
//   ;
//    }
//    mapV.clear();
//}

//void SceneSetupPreflush::WriteXmlPreflushConfig(const char *file, const char *cmd)
//{
//    QMap<QString, QString> mapV;

//  if (strcmp(cmd, DEFAULT_XML_SAVE) == 0) {
//        mapV.clear();
//        mapV.insert("bloodspeed", lineEditPreflushBloodSpeed.text());
//        mapV.insert("dialysateflow", lineEditManualPreflushUfRate.text());
//        mapV.insert("time", lineEditPreflushTime.text());
//        mapV.insert("Ufrate", lineEditPreflushUfRate.text());
//        mapV.insert("Ufvolume", lineEditPreflushUfVolume.text());
//        mapV.insert("bloodvolume", lineEditReturnBloodVolume.text());
//        xml_NodeWriteValue(file, "/Config/Preflush/Default", &mapV);

//    }
//    mapV.clear();
//}
