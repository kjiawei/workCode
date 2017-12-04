#include <QtGui>
#include "config_ui.h"
#include "xml_parse.h"
#include "scene_setup_layout.h"
#include "scene_setup_hemopump.h"
#include "deal.h"

#include "common_func.h"

#include <unistd.h>



inline void SceneSetupHemopump::setUi()
{
    SCENE_SETUP_CREATE_TABLE(labelHemoSpeed, 1);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditHemoSpeed, 1);
    SCENE_SETUP_CREATE_UNIT(labelHemoSpeedUnit, 1);
    labelHemoSpeedUnit.setText("ml/min");

    SCENE_SETUP_CREATE_TABLE(labelSupplySpeed, 3);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditSupplySpeed, 3);
    SCENE_SETUP_CREATE_UNIT(labelSupplySpeedUnit, 3);
    labelSupplySpeedUnit.setText("ml/min");

    SCENE_SETUP_CREATE_TABLE(labelHemoTotal, 2);
    SCENE_SETUP_CREATE_VALUE(labelHemoTotalValue, 2);
    SCENE_SETUP_CREATE_UNIT(labelHemoTotalUnit, 2);
    labelHemoTotalUnit.setText("mL");

    labelPumpSelect.setGeometry(QRect(SCENE_SETUP_COL_LABEL+100,
                SCENE_SETUP_ROW_3+55,
                SCENE_SETUP_COL_WIDTH_LABEL,
                SCENE_SETUP_WIDGET_HIGH));;
    labelPumpSelect.setAlignment( Qt::AlignVCenter | Qt::AlignRight);
    labelPumpSelect.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelPumpSelect.setStyleSheet(CONFIG_COLOR_LABEL);

    radioButtonSelectYes.setGeometry(QRect(SCENE_SETUP_COL_LINEEDIT+100,
                SCENE_SETUP_ROW_4, 100, 30));
    radioButtonSelectYes.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    radioButtonSelectYes.setStyleSheet("QRadioButton {color:#ffffff;}"
            "QRadioButton::indicator {width: 29px;height: 31px;} "
            "QRadioButton::indicator::checked {image: url(:/resource/images/radio_button_check.png);} "
            "QRadioButton::indicator::unchecked {image: url(:/resource/images/radio_button_uncheck.png);}");


    radioButtonSelectNo.setGeometry(QRect(SCENE_SETUP_COL_LINEEDIT+100,
                SCENE_SETUP_ROW_4+40, 100, 30));
    radioButtonSelectNo.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    radioButtonSelectNo.setStyleSheet("QRadioButton {color:#ffffff;}"
            "QRadioButton::indicator {width: 29px;height: 31px;} "
            "QRadioButton::indicator::checked {image: url(:/resource/images/radio_button_check.png);} "
            "QRadioButton::indicator::unchecked {image: url(:/resource/images/radio_button_uncheck.png);}");

//    QValidator* validator = new QIntValidator(20, 620, this );
//    lineEditHemoSpeed.setValidator(validator);

    buttonGroup.addButton(&radioButtonSelectYes, 0);
    buttonGroup.addButton(&radioButtonSelectNo, 1);

    buttonSave.setGeometry(QRect(400, SCENE_SETUP_ROW_9, 64, 39));
    buttonCancel.setGeometry(QRect(474, SCENE_SETUP_ROW_9, 64, 39));
    buttonApply.setGeometry(QRect(548, SCENE_SETUP_ROW_9, 64, 39));

    BUTTON_SIMPLE_STYLE(buttonSave);
    BUTTON_SIMPLE_STYLE(buttonCancel);
    BUTTON_SIMPLE_STYLE(buttonApply);

    CustomSetText();
}

SceneSetupHemopump::SceneSetupHemopump(QWidget *parent) :
    QObject(parent),
    labelHemoSpeed(parent),
    lineEditHemoSpeed(parent),
    labelHemoSpeedUnit(parent),

    labelSupplySpeed(parent),
    lineEditSupplySpeed(parent),
    labelSupplySpeedUnit(parent),

    labelHemoTotal(parent),
    labelHemoTotalValue(parent),
    labelHemoTotalUnit(parent),

    labelPumpSelect(parent),
    buttonGroup(parent),
    radioButtonSelectYes(parent),
    radioButtonSelectNo(parent),

    buttonSave(parent),
    buttonApply(parent),
    buttonCancel(parent)
{
    setUi();

    dataNodeAddObject(PACKET_NAME_WORKMODE, this);
    dataNodeAddObject(PACKET_NAME_BLOODPUMP, this);
    dataNodeAddObject(PACKET_NAME_BLOODPUMPTUBE, this);
    dataNodeAddObject(PACKET_MAINTAIN_PARAMS, this);
    dataNodeAddObject(PACKET_REBIRTH_HANDLE, this); //重生操作处理#/

    connect(&buttonApply, SIGNAL(clicked()), this, SLOT(SlotApply()));
    connect(&buttonSave, SIGNAL(clicked()), this, SLOT(SlotParameterSave()));
    connect(&buttonCancel, SIGNAL(clicked()), this, SLOT(SlotParameterCancel()));
#if HJX_COMMUNICATION_SWITCH
    connect(this,SIGNAL(signalSDO(int)),dataCanSend,SLOT(sendSDOData(int)),DE_CONNECTION_TYPE);
#endif

    PageAppendPiece("Setup Hemopump", this);
    setParentName("SceneSetupHemopump");

    isKeyInit = false;
    setspeed_clicked_flag=false;   //血泵设置速度按键按下标志#/

}

void SceneSetupHemopump::Init()
{
}

void SceneSetupHemopump::SlotParameterSave()
{
    Init();
    if (sdoBlood.N3_set_BPspeed != lineEditHemoSpeed.text().toUShort()) {
        sdoBlood.N3_set_BPspeed = lineEditHemoSpeed.text().toUShort();
        G_SET_BPSPEED=sdoBlood.N3_set_BPspeed;

        dataNodeUpdate(PACKET_SDO_BLOOD, &sdoBlood);

        WorkState  stateTMP=GetWorkStateManage();
        if(stateTMP.tipState.current<=TREAT_READY)  //准备阶段时，才改变预冲血泵速度#/
        {
            dataNodeUpdate(BLOOD_CHANGE_PREFLUSH,&sdoBlood);
        }
    }
}

void SceneSetupHemopump::SlotParameterCancel()
{
    QString str;
    Init();
    lineEditHemoSpeed.setText(str.setNum(packetBloodPumpTube.N9_set_speed));
}

void SceneSetupHemopump::SlotApply()
{
    if (packetBloodPump.N9_mode == 1) {
#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_BloodStop);
        emit signalSDO(DE_CMD_SubsStop);
#else
        MasterCtrol_CMD_BloodStop();
        us_sleep(100000);
        MasterCtrol_CMD_SubsStop();
#endif
    } else if (packetBloodPump.N9_mode == 2) {

#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_BloodStart);
        emit signalSDO(DE_CMD_SubsStart);
#else
        MasterCtrol_CMD_BloodStart();
        us_sleep(100000);
        MasterCtrol_CMD_SubsStart();
#endif
    }
}


void SceneSetupHemopump::lineEdit_init()
{
    lineEditHemoSpeed.setObjectName("HemoSpeed");

    ic->appendWidget(&lineEditHemoSpeed, NumInputMethod, LineEdit, this,
                     usr_params.Params_BP.Rate_ThreatMent.attribute.min_value,
                     usr_params.Params_BP.Rate_ThreatMent.attribute.max_value,
                     usr_params.Params_BP.Rate_ThreatMent.attribute.step_value,
                     tr("Blood Flow:"),"Blood Flow");


//    ic->appendWidget(&lineEditSupplySpeed, NumInputMethod, LineEdit, this,
//                     0,
//                     65535,
//                     100,
//                     tr("Supply Flux:"),"Supply Flux:");
}

void SceneSetupHemopump::lineEdit_close(const char *name)
{
    name = name;
}

void SceneSetupHemopump::lineEdit_data_set(const char *name, const char *data)
{
    name = name;
    data = data;
    SlotParameterSave();
}
void SceneSetupHemopump::lineEdit_start(const char *name, float *max, float *min)
{
    if(strcmp(name,"Blood Flow")==0){
        *max = usr_params.Params_BP.Rate_ThreatMent.attribute.max_value;
        *min =usr_params.Params_BP.Rate_ThreatMent.attribute.min_value;
    }
}

void SceneSetupHemopump::ActionHandle(const void* dataPtr,int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupHemopump-ActionHandle-in");
#endif

    if(workstate.tipState.current==DEBUG)
        return;

    if ((dataPtr == NULL) || (name == 0)) {
        return;
    }

    if (name==PACKET_NAME_BLOODPUMP) {
        struct PacketBloodPump *packet = (struct PacketBloodPump*)dataPtr;

        if (packetBloodPump.N9_mode != packet->N9_mode) {
            if (packet->N9_mode == 2) {
                buttonApply.setText(tr("Run"));
            } else if (packet->N9_mode == 1) {
                buttonApply.setText(tr("Stop"));
            }
        }

        /*
        //通过血泵按键设置速度#/
        if(packet->N9_status & 0x18)
        {            
            if(packetBloodPumpTube.N9_set_speed != G_SET_BPSPEED)
            {
                BPspeed_Handle(packetBloodPumpTube.N9_set_speed);      //速度相关处理#/
            }
        }
        */

        WorkState  stateTMP=GetWorkStateManage();
        if(stateTMP.tipState.current==TREAT_READY)
        {
            labelHemoTotalValue.setNum(BloodPump_Pre_Amount+BloodPump_Volume);
        }
        else
            labelHemoTotalValue.setNum(BloodPump_Cure_Amount+BloodPump_Volume);

        /*
        if (packetBloodPump.N9_amount != packet->N9_amount) {
            labelHemoTotalValue.setNum(packet->N9_amount);
        }
        */
        packetBloodPump=*((struct PacketBloodPump*)dataPtr);

//        memcpy(&packetBloodPump, dataPtr, 8);
    }else if (name==PACKET_NAME_BLOODPUMPTUBE) {
        struct PacketBloodPumpTube *packet = (struct PacketBloodPumpTube*)dataPtr;
        if((packet->N9_set_speed<=0)&&(packet->N9_set_speed>620))   //防止乱码设置血泵速度#/
            return;

        if(packet->N9_Key&0x18) //有血泵速度设置按键按下#/
        {
            if(packet->N9_set_speed != G_SET_BPSPEED)
                BPspeed_Handle(packet->N9_set_speed);      //速度相关处理#/
        }

        if(sdoBlood.N3_set_BPtube!=packet->N9_set_tube&&packet->N9_set_tube>0\
                &&packet->N9_set_tube<11){
            sdoBlood.N3_set_BPtube=packet->N9_set_tube;
            dataNodeUpdate(PACKET_BP_WIDTH, &sdoBlood.N3_set_BPtube);
        }

        packetBloodPumpTube=*((struct PacketBloodPumpTube*)dataPtr);

    } else if (name==PACKET_MAINTAIN_PARAMS) {
        usr_params = *((Params*)dataPtr);
        //Disinfect.
        sdoBlood.N3_set_BPspeed = usr_params.Params_BP.Rate_ThreatMent.attribute.default_value;
        sdoBlood.N3_set_BPtube = usr_params.Params_BP.BP_Width.attribute.default_value;

        G_BPRATE_GETBLOOD=usr_params.Params_BP.Rate_GetBlood.attribute.default_value;
        G_BPRATE_TREAT=usr_params.Params_BP.Rate_ThreatMent.attribute.default_value;
        G_BPRATE_RETURNBLOOD=usr_params.Params_BP.Rate_ReturnBlood.attribute.default_value;

        lineEditHemoSpeed.setText(QString::number(sdoBlood.N3_set_BPspeed));

        G_SET_BPSPEED=sdoBlood.N3_set_BPspeed;

        dataNodeUpdate(PACKET_SDO_BLOOD, &sdoBlood);

    } else if (name==PACKET_NAME_WORKMODE) {
        PacketWorkMode *packetWorkMode = (PacketWorkMode *)dataPtr;
        if (strcmp(packetWorkMode->modeNow, "HD") == 0) {
#if 0
            listWidgetAdd(&labelPumpSelect);
            listWidgetAdd(&radioButtonSelectNo);
            listWidgetAdd(&radioButtonSelectYes);
        } else if (strcmp(packetWorkMode->modeNow, "HDF") == 0) {
            listWidgetAdd(&labelPumpSelect, 1);
            listWidgetAdd(&radioButtonSelectNo, 1);
            listWidgetAdd(&radioButtonSelectYes, 1);
#endif
        }
    }else if(name==PACKET_REBIRTH_HANDLE){     //重生恢复操作处理#/
        sdoBlood=G_P_REBIRTH->sdoBlood;
        G_SET_BPSPEED=sdoBlood.N3_set_BPspeed;
        dataNodeUpdate(PACKET_SDO_BLOOD, &sdoBlood);
    }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupHemopump-ActionHandle-out");
#endif
}


void SceneSetupHemopump::BPspeed_Handle(unsigned short int set_speed)      //速度相关处理#/
{
    WorkState  stateTMP=GetWorkStateManage();
    if (sdoBlood.N3_set_BPspeed != set_speed)
    {
        bool upflag=true;
        sdoBlood.N3_set_BPspeed = set_speed;
        //引血时，血泵速度不能超过设置值#/
        if(stateTMP.tipState.current==TREAT_READY&&(stateTMP.subState.current==GETBLOOD_RUN_NO_FLUID||stateTMP.subState.current==GETBLOOD_RUN_FLUID))
        {

            if(sdoBlood.N3_set_BPspeed<usr_params.Params_BP.Rate_GetBlood.attribute.min_value)
                sdoBlood.N3_set_BPspeed=usr_params.Params_BP.Rate_GetBlood.attribute.min_value;
            else if(sdoBlood.N3_set_BPspeed>usr_params.Params_BP.Rate_GetBlood.attribute.max_value)
                sdoBlood.N3_set_BPspeed=usr_params.Params_BP.Rate_GetBlood.attribute.max_value;

            upflag=false;
        }
        else if(stateTMP.tipState.current==TREAT_FINISH&&(stateTMP.subState.current==CURE_BLOODRETURN_RUN||stateTMP.subState.current== CURE_BLOODRETURN_RUN_NO_BLOOD\
                ||stateTMP.subState.current==CURE_BLOODRETURN_RUN_DRAIN||stateTMP.subState.current==CURE_BLOODRETURN_RUN_DRAIN_NO_BLOOD))
        {
            if(sdoBlood.N3_set_BPspeed<usr_params.Params_BP.Rate_ReturnBlood.attribute.min_value)
                sdoBlood.N3_set_BPspeed=usr_params.Params_BP.Rate_ReturnBlood.attribute.min_value;
            else if(sdoBlood.N3_set_BPspeed>usr_params.Params_BP.Rate_ReturnBlood.attribute.max_value)
                sdoBlood.N3_set_BPspeed=usr_params.Params_BP.Rate_ReturnBlood.attribute.max_value;
            upflag=false;
        }

        if(sdoBlood.N3_set_BPspeed<=0)
            sdoBlood.N3_set_BPspeed = set_speed;

        lineEditHemoSpeed.setText(QString::number(sdoBlood.N3_set_BPspeed));

        //tmp
        //由下面发上来的数据，导致了血泵参数的更新。我们要及时更新发下去的数据。比如 + - 键改变血泵流速后#/
        G_SET_BPSPEED=sdoBlood.N3_set_BPspeed;
        dataNodeUpdate(PACKET_SDO_BLOOD, &sdoBlood);

        if((stateTMP.tipState.current<=TREAT_READY)&&(upflag==true))  //准备阶段时，才改变预冲血泵速度#/
        {
            dataNodeUpdate(BLOOD_CHANGE_PREFLUSH,&sdoBlood);
        }
    }
}

void SceneSetupHemopump::CustomSetText()
{
    labelHemoSpeed.setText(tr("Blood Flow:"));
    labelSupplySpeed.setText(tr("Supply Flux:"));
    labelHemoTotal.setText(tr("Blood Volume:"));
    labelPumpSelect.setText(tr("Regard spare pump as blood pump:"));
    radioButtonSelectYes.setText(tr("yes"));
    radioButtonSelectNo.setText(tr("no"));
    buttonSave.setText(tr("Confirm"));
    buttonCancel.setText(tr("Cancel"));
    buttonApply.setText(tr("Run"));
}

void SceneSetupHemopump::setCusVisible(bool vis, char* para)
{
    para = para;

    if(vis && !isKeyInit) {
        isKeyInit = true;
        lineEdit_init();
    }

    if(vis==true)
    {
        dataRead(PACKET_SDO_BLOOD, &sdoBlood);

        lineEditHemoSpeed.setText(QString::number(sdoBlood.N3_set_BPspeed));
    }

    labelHemoSpeed.setVisible(vis);
    lineEditHemoSpeed.setVisible(vis);
    labelHemoSpeedUnit.setVisible(vis);

    labelSupplySpeed.setVisible(false);
    lineEditSupplySpeed.setVisible(false);
    labelSupplySpeedUnit.setVisible(false);

    labelHemoTotal.setVisible(vis);
    labelHemoTotalUnit.setVisible(vis);
    labelHemoTotalValue.setVisible(vis);
    labelPumpSelect.setVisible(false);

    radioButtonSelectYes.setVisible(false);
    radioButtonSelectNo.setVisible(false);

    buttonSave.setVisible(false);
    buttonApply.setVisible(false);
    buttonCancel.setVisible(false);
}
