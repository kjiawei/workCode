#include "config_ui.h"
#include "scene_setup_layout.h"
#include "scene_setup_subspump.h"
#include "common_func.h"
#include "common_tr_string.h"

inline void SceneSetupSubspump::setUi()
{
    SCENE_SETUP_CREATE_TABLE(labelSubsValue, 1);
    SCENE_SETUP_CREATE_VALUE(labelSubsValueHas, 1);
    SCENE_SETUP_CREATE_UNIT(labelSubsValueUnit, 1);
    labelSubsValueHas.setNum(0);
    labelSubsValueUnit.setText("mL");//已置换量

    SCENE_SETUP_CREATE_TABLE(labelSubsSpeed, 2);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditSubsSpeed, 2);
    SCENE_SETUP_CREATE_UNIT(labelSubsSpeedUnit, 2);
    labelSubsSpeedUnit.setText("mL/min");//置换泵流速

    SCENE_SETUP_CREATE_TABLE(labelSubsTotal, 3);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditSubsTotal, 3);
    SCENE_SETUP_CREATE_UNIT(labelSubsTotalUnit, 3);
    labelSubsTotalUnit.setText("mL");//置换目标量

    SCENE_SETUP_CREATE_TABLE(labelDTtitle, 5);
    SCENE_SETUP_CREATE_VALUE(labelDTtext, 5);

    SCENE_SETUP_CREATE_TABLE(labelUFBPtitle, 6);
    SCENE_SETUP_CREATE_VALUE(labelUFBPtext, 6);

    SCENE_SETUP_CREATE_TABLE(labelBPspeedtitle, 7);
    SCENE_SETUP_CREATE_VALUE(labelBPspeedtext, 7);
    SCENE_SETUP_CREATE_UNIT(labelBPspeedUnit, 7);
    labelBPspeedUnit.setText("mL/min");//血泵流速

    SCENE_SETUP_CREATE_TABLE(labeldialyspeedtitle, 8);
    SCENE_SETUP_CREATE_VALUE(labeldialyspeedtext, 8);
    SCENE_SETUP_CREATE_UNIT(labeldialyspeedUnit, 8);
    labeldialyspeedUnit.setText("mL/min");//透析液流速
    
    SCENE_SETUP_CREATE_TABLE(labelAddValue, 9);
    SCENE_SETUP_CREATE_VALUE(labelAddValueHas, 9);
    SCENE_SETUP_CREATE_UNIT(labelAddValueUnit, 9);
    labelAddValueHas.setNum(0);
    labelAddValueUnit.setText("mL");//已补液量

    //单次在线补液
    SCENE_SETUP_CREATE_TABLE(labelAddQuick, 10);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditAddQuick, 10);
    SCENE_SETUP_CREATE_UNIT(labelAddQuickUnit, 10);
    labelAddQuickUnit.setText("mL");

    SCENE_SETUP_CREATE_COMBOBOX(comboBoxSubBpSpeed, 10);
    comboBoxSubBpSpeed.insertItem(0, trUtf8("80"));
    comboBoxSubBpSpeed.insertItem(1, trUtf8("160"));
    comboBoxSubBpSpeed.insertItem(2, trUtf8("240"));
    comboBoxSubBpSpeed.insertItem(3, trUtf8("320"));
    comboBoxSubBpSpeed.insertItem(4, trUtf8("400"));
    comboBoxSubBpSpeed.insertItem(5, trUtf8("480"));
    comboBoxSubBpSpeed.insertItem(6, trUtf8("560"));
    comboBoxSubBpSpeed.setCurrentIndex(0);

    buttonSave.setGeometry(QRect(290, SCENE_SETUP_ROW_4+5, 64, 39));
    buttonCancel.setGeometry(QRect(364, SCENE_SETUP_ROW_4+5, 64, 39));
    buttonRun.setGeometry(QRect(438, SCENE_SETUP_ROW_4+5, 64, 39));

    BUTTON_SIMPLE_STYLE(buttonSave);
    BUTTON_SIMPLE_STYLE(buttonCancel);
    BUTTON_SIMPLE_STYLE(buttonRun);

    buttonAddQuick.setStyleSwitch(":/resource/images/setup/function_normal.png",
                                  ":/resource/images/setup/function_active.png", 0xff1c2335,
                                  QRect(615, SCENE_SETUP_ROW_10, 64, 39));

    lineEditAddQuick.setDisabled(true);
    lineEditAddQuick.setText("100");
    isRunBtStatus=false;
    isQuickAddStatus = false;
    connect(&buttonAddQuick,SIGNAL(clicked()),this,SLOT(applyAddQuick()));
    connect(&buttonRun, SIGNAL(clicked()), this, SLOT(SlotApply()));
    connect(&buttonSave, SIGNAL(clicked()), this, SLOT(Save()));

    CustomSetText();
}

SceneSetupSubspump::SceneSetupSubspump(QWidget *parent) :
    QObject(parent),
    labelDTtitle(parent),  //稀释类型题目#/
    labelDTtext(parent),   //稀释类型内容#/
    labelUFBPtitle(parent),
    labelUFBPtext(parent),
    labelBPspeedtitle(parent),
    labelBPspeedtext(parent),
    labelBPspeedUnit(parent),
    labeldialyspeedtitle(parent),
    labeldialyspeedtext(parent),
    labeldialyspeedUnit(parent),

    labelSubsSpeed(parent),
    lineEditSubsSpeed(parent),
    labelSubsSpeedUnit(parent),

    labelSubsTotal(parent),
    lineEditSubsTotal(parent),
    labelSubsTotalUnit(parent),

    labelSubsValue(parent),
    labelSubsValueHas(parent),
    labelSubsValueUnit(parent),

    labelAddValue(parent),
    labelAddValueHas(parent),
    labelAddValueUnit(parent),

    labelAddQuick(parent),
    lineEditAddQuick(parent),
    labelAddQuickUnit(parent),

    buttonAddQuick(parent),
    comboBoxSubBpSpeed(parent),
    buttonSave(parent),
    buttonCancel(parent),
    buttonRun(parent)
{
    setUi();
    PageAppendPiece("Setup Subspump", this);
    subsValueHas=0; //已置换量#/
    addValueHas=0; //已补液量#/
    SubsAddnum=0;  //补液选择速度编号#/
    addValueTmp=0;
    dataNodeAddObject(PACKET_MAINTAIN_PARAMS, this);
    //    dataNodeAddObject(PACKET_STATUS_NOTE,this);
    dataNodeAddObject(PACKET_NAME_SUBBLOODPUMP,this);
    dataNodeAddObject(PACKET_REBIRTH_HANDLE,this);  //重生操作处理#/

    connect(&comboBoxSubBpSpeed, SIGNAL(currentIndexChanged(int)), this, SLOT(slotVolumeSel(int)));
#if HJX_COMMUNICATION_SWITCH
    connect(this,SIGNAL(signalSDO(int)),dataCanSend,SLOT(sendSDOData(int)),DE_CONNECTION_TYPE);
#endif

    isKeyInit=false;
    setParentName("SceneSetupSubspump");
}

void SceneSetupSubspump::slotVolumeSel(int id)
{
    if(id>=0&&id<=6)
        SubsAddnum=id+1;
}

void SceneSetupSubspump::applyAddQuick()
{
    PacketBloodPump Subpump;
    isQuickAddStatus = !isQuickAddStatus;
    if (isQuickAddStatus == true)
    {
        dataRead(PACKET_NAME_SUBBLOODPUMP,&Subpump);
        addValueTmp=Subpump.N9_amount;
        addValueNow=0;
        G_SUB_RAPID_FLAG=true;
        MasterCtrol_CMD_SubsStart();
    }
    else
    {
        MasterCtrol_CMD_SubsStop();
        addValueNow=0;
        G_SUB_RAPID_FLAG=false;
    }
}

void SceneSetupSubspump::SlotApply()
{
    if (isRunBtStatus==false)
    {
        pal.setColor(QPalette::ButtonText, Qt::green);//QColor(255,0,0)
        buttonRun.setPalette(pal);
        isRunBtStatus=true;
        buttonRun.setText(tr("Stop"));
		
#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_SubsStart);
        emit signalSDO(DE_CMD_BloodStart);
#else
        MasterCtrol_CMD_SubsStart();
        us_sleep(10000);
        MasterCtrol_CMD_BloodStart();
#endif

    }
    else
    {
        isRunBtStatus=false;
        pal.setColor(QPalette::ButtonText, Qt::black);
        buttonRun.setPalette(pal);
        buttonRun.setText(tr("Run"));
		
#if HJX_COMMUNICATION_SWITCH
        emit signalSDO(DE_CMD_SubsStop);
        emit signalSDO(DE_CMD_BloodStop);
#else
        MasterCtrol_CMD_SubsStop();
        us_sleep(10000);
        MasterCtrol_CMD_BloodStop();
#endif
    }
}

void SceneSetupSubspump::CustomSetText()
{
    labelDTtitle.setText(tr("Dilute Type:"));  //稀释类型题目#/
    labelUFBPtitle.setText(tr("UF/BP:"));
    labelBPspeedtitle.setText(tr("Blood Flow:"));
    labeldialyspeedtitle.setText(tr("Dialysate Flow:"));
    labelSubsSpeed.setText(tr("Sub. Flux:"));
    labelSubsTotal.setText(tr("Sub. Goal:"));
    labelSubsValue.setText(tr("Sub. Vol:"));
    labelAddValue.setText(tr("Rapid Infusion Vol:"));
    labelAddQuick.setText(tr("Rapid Infusion Rate:"));
    buttonSave.setText(tr("Confirm"));
    buttonCancel.setText(tr("Cancel"));
    buttonRun.setText(tr("Run"));
}

void SceneSetupSubspump::setCusVisible(bool vis, char* para)
{
    para = para;
    struct PacketSdoBlood sdoBlood;
    struct PacketUF packetTmp;
    float rate=0;
    float subspeed=0;
    int subtotal=0;

    if(vis && !isKeyInit) {
        isKeyInit = true;
        lineEdit_init();
    }
    if(vis==true)
    {
        labelSubsValueHas.setText(QString::number(SubsPump_finishvolume));
        labelAddValueHas.setText(QString::number(addValueHas));
        comboBoxSubBpSpeed.setCurrentIndex(SubsAddnum);
        dataRead(PACKET_PDO_SUBBLOODPUMP,&SubBPump);
        lineEditSubsSpeed.setText(QString::number(SubBPump.N10_set_speed));
        lineEditSubsTotal.setText(QString::number(SubsPump_totalvolume));

        dataRead(PACKET_SDO_BLOOD, &sdoBlood);
        labelBPspeedtext.setText(QString::number(sdoBlood.N3_set_BPspeed));

        dataRead(PACKET_NAME_UF,&packetTmp);
        rate=(packetTmp.ufRate+SubBPump.N10_set_speed*1.0)/sdoBlood.N3_set_BPspeed;
        labelUFBPtext.setText(QString::number(rate,'f',2));

        PacketWorkMode workModeTMP;
        dataRead(PACKET_NAME_WORKMODE,&workModeTMP);
        if(workModeTMP.treatmode.current==HDF_pre_dilute||workModeTMP.treatmode.current==HF_pre_dilute)
        {
            labelDTtext.setText(tr("Pre Dilute"));
        }
        else if(workModeTMP.treatmode.current==HDF_post_dilute||workModeTMP.treatmode.current==HF_post_dilute)
        {
            labelDTtext.setText(tr("Post Dilute"));
        }

        PacketFluid packet;
        dataRead(PACKET_NAME_DIALYSATE,&packet);
        labeldialyspeedtext.setText(QString::number(packet.Wspeed));
        subspeed=lineEditSubsSpeed.text().toFloat();
        subtotal=subspeed*(total_secCount/60.0);
        lineEditSubsTotal.setText(QString::number(subtotal));
    }

    labelDTtitle.setVisible(vis);  //稀释类型题目#/
    labelDTtext.setVisible(vis);   //稀释类型内容#/
    labelUFBPtitle.setVisible(vis);
    labelUFBPtext.setVisible(vis);
    labelBPspeedUnit.setVisible(vis);
    labelBPspeedtitle.setVisible(vis);
    labelBPspeedtext.setVisible(vis);
    labeldialyspeedtitle.setVisible(vis);
    labeldialyspeedtext.setVisible(vis);
    labeldialyspeedUnit.setVisible(vis);

    labelSubsSpeed.setVisible(vis);
    labelSubsSpeedUnit.setVisible(vis);
    labelSubsTotal.setVisible(vis);
    labelSubsTotalUnit.setVisible(vis);
    lineEditSubsSpeed.setVisible(vis);
    lineEditSubsTotal.setVisible(vis);
    labelSubsValue.setVisible(vis);
    labelSubsValueHas.setVisible(vis);
    labelSubsValueUnit.setVisible(vis);
    labelAddValue.setVisible(vis);
    labelAddValueHas.setVisible(vis);
    labelAddValueUnit.setVisible(vis);

    labelAddQuick.setVisible(vis);
    lineEditAddQuick.setVisible(false);
    labelAddQuickUnit.setVisible(vis);
    buttonAddQuick.setVisible(vis);

    buttonSave.setVisible(false);
    buttonCancel.setVisible(false);
    buttonRun.setVisible(false);
    comboBoxSubBpSpeed.setVisible(vis);
}

void SceneSetupSubspump::ActionHandle(const void* dataPtr, int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupSubspump-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name ==0)) {
        return;
    }
    if (name==PACKET_MAINTAIN_PARAMS) {
        Params *params = (Params*)dataPtr;
        lineEditSubsSpeed.setText(QString::number(params->Params_SubP.Flux.attribute.default_value));
        lineEditSubsTotal.setText(QString::number(params->Params_SubP.Goal.attribute.default_value));
        SubBPump.N10_set_speed=params->Params_SubP.Flux.attribute.default_value;
        SubsPump_totalvolume=params->Params_SubP.Goal.attribute.default_value;

        G_SUB_autochangelimit=params->Params_SubP.AutoChange_Limit.attribute.default_value;
        G_SUB_autochangelimit=G_SUB_autochangelimit/100;
        dataNodeUpdate(PACKET_PDO_SUBBLOODPUMP, &SubBPump);
    }
    else if(name==PACKET_STATUS_NOTE){    //对应主控板PDO0的4字节#/
        /*
        if(statusAddQuick == false)
        {
            statusAddQuick=true;
            PacketStatusNote note_N3 = *((PacketStatusNote *)dataPtr);

            if(note_N3.note.note3&0x80)
            {
                buttonAddQuick.setDisabled(true);
            }
        }
        else
        {
            if(statusAddQuick == true)
            {
                statusAddQuick=false;
                buttonAddQuick.setDisabled(false);
            }
        }
        */
    }
    else if(name==PACKET_NAME_SUBBLOODPUMP){
        PacketBloodPump Subpumptmp = *((PacketBloodPump *)dataPtr);
        //        WorkState  stateTMP=GetWorkStateManage();
        SubsPump_finishvolume=Subpumptmp.N9_amount;

        /*
        if(stateTMP.tipState.current!=TREAT_RUNNING)
        {
            subsValueHas=Subpumptmp.N9_amount;
            SubsPump_finishvolume=Subpumptmp.N9_amount;
        }
        else
        {
            if(statusBtRun==true)
                SubsPump_finishvolume=Subpumptmp.N9_amount-subsValueHas;
        }

        */
        if(Subpumptmp.N9_mode==0x01) //run
        {
            if(isRunBtStatus==false)
            {
                pal.setColor(QPalette::ButtonText, Qt::green);//QColor(255,0,0)
                buttonRun.setPalette(pal);
                isRunBtStatus=true;
                buttonRun.setText(tr("Stop"));
            }
        }
        else
        {
            if(isRunBtStatus==true)
            {
                pal.setColor(QPalette::ButtonText, Qt::black);
                buttonRun.setPalette(pal);
                buttonRun.setText(tr("Run"));
                isRunBtStatus=false;
            }
        }
        /*
        if(SubsPump_finishvolume>SubsPump_totalvolume && statusBtRun==true&&stateTMP.tipState.current==TREAT_RUNNING)
        {
            SubsPump_finishvolume=SubsPump_totalvolume;
            pal.setColor(QPalette::ButtonText, Qt::black);
            buttonRun.setPalette(pal);
            buttonRun.setText(tr("Run"));
            MasterCtrol_CMD_SubsStop();
            us_sleep(10000);
            statusBtRun=false;
            showYesDialog(DE_TR_PROMPT,DE_TR_THESUBSPUMPGOAL);
        }
        */

        if(G_SUB_RAPID_FLAG==true)
        {
            addValueNow=Subpumptmp.N9_amount-addValueTmp;
            if(addValueNow<SubsAddnum*80)
            {
                labelAddValueHas.setText(QString::number(addValueHas+addValueNow));
            }
            else
            {
                buttonAddQuick.setDown(false);
                isQuickAddStatus=false;
                addValueNow=0;
                G_SUB_RAPID_FLAG=false;
                addValueHas += addValueNow;
                MasterCtrol_CMD_SubsStop();
                labelAddValueHas.setText(QString::number(addValueHas));
            }
        }

        labelSubsValueHas.setText(QString::number(SubsPump_finishvolume));
    }else if(name==PACKET_REBIRTH_HANDLE)//重生恢复操作处理#/
    {
        SubBPump=G_P_REBIRTH->SubBPump;
        SubsAddnum=G_P_REBIRTH->SubsAddnum;
        SubsPump_totalvolume=G_P_REBIRTH->SubsPump_totalvolume;

        SubBPump.N10_goal=SubsPump_totalvolume;
        dataNodeUpdate(PACKET_PDO_SUBBLOODPUMP, &SubBPump);

        comboBoxSubBpSpeed.setCurrentIndex(SubsAddnum);
        lineEditSubsSpeed.setText(QString::number(SubBPump.N10_set_speed));
        lineEditSubsTotal.setText(QString::number(SubsPump_totalvolume));
        //        printf("SubBPump02==%d \n", G_P_REBIRTH->SubBPump.N10_set_speed);
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupSubspump-ActionHandle-out");
#endif
}

void SceneSetupSubspump::lineEdit_init()//设置流速 总量
{
    lineEditSubsSpeed.setObjectName(tr("SUB speed:"));
    lineEditSubsTotal.setObjectName(tr("SUB total:"));

    ic->appendWidget(&lineEditSubsSpeed,NumInputMethod, LineEdit, this,
                     usr_params.Params_BP.Rate_ThreatMent.attribute.min_value,
                     usr_params.Params_BP.Rate_ThreatMent.attribute.max_value,
                     usr_params.Params_BP.Rate_ThreatMent.attribute.step_value,
                     tr("SUB speed:"),"SUB speed");

    ic->appendWidget(&lineEditSubsTotal,NumInputMethod, LineEdit, this,
                     usr_params.Params_BP.Rate_ThreatMent.attribute.min_value,
                     usr_params.Params_BP.Rate_ThreatMent.attribute.max_value,
                     usr_params.Params_BP.Rate_ThreatMent.attribute.step_value,
                     tr("SUB total:"),"SUB total");

    ic->appendWidget(&lineEditAddQuick,NumInputMethod, LineEdit, this,
                     usr_params.Params_BP.Rate_ThreatMent.attribute.min_value,
                     usr_params.Params_BP.Rate_ThreatMent.attribute.max_value,
                     usr_params.Params_BP.Rate_ThreatMent.attribute.step_value,
                     tr("SUB Add:"),"SUB Add");
}

void SceneSetupSubspump::lineEdit_start(const char *name, float *max, float *min)
{
    if(name == NULL)
        return;

    if(strcmp(name, "SUB speed") == 0) {
        *max = (unsigned int)usr_params.Params_SubP.Flux.attribute.max_value;
        *min = (unsigned int)usr_params.Params_SubP.Flux.attribute.min_value;
    } else if(strcmp(name, "SUB total") == 0) {
        *max = (unsigned int)(usr_params.Params_SubP.Flux.attribute.max_value*(total_secCount/60.0));
        *min = (unsigned int)(usr_params.Params_SubP.Flux.attribute.min_value*(total_secCount/60.0));
    }
}

void SceneSetupSubspump::lineEdit_close(const char *name)
{
	name = name;
}

//change置换泵速度总量
void SceneSetupSubspump::lineEdit_data_set(const char *name, const char *data)
{
    data=data;
    int subSpeed=0,subTotal=0;
    subSpeed=lineEditSubsSpeed.text().toFloat();
    if(strcmp(name, "SUB speed") == 0) //变速度#/
    {
        subSpeed=lineEditSubsSpeed.text().toInt();
        subTotal=subSpeed*(total_secCount/60.0);
        lineEditSubsTotal.setText(QString::number(subTotal));
    }
    else if(strcmp(name, "SUB total") == 0)  //变目标量#/
    {
        subTotal=lineEditSubsTotal.text().toInt();
        subSpeed=subTotal/(total_secCount/60.0);
        lineEditSubsSpeed.setText(QString::number(subSpeed));
    }
    dataRead(PACKET_PDO_SUBBLOODPUMP,&SubBPump);
    SubBPump.N10_set_speed=lineEditSubsSpeed.text().toInt();
    SubBPump.N10_goal=lineEditSubsTotal.text().toInt();
    dataNodeUpdate(PACKET_PDO_SUBBLOODPUMP, &SubBPump);
    SubsPump_totalvolume=SubBPump.N10_goal;
    BloodPump_link_flag=true;
}

void SceneSetupSubspump::Save()
{
    dataRead(PACKET_PDO_SUBBLOODPUMP,&SubBPump);
    SubBPump.N10_set_speed=lineEditSubsSpeed.text().toInt();
    SubBPump.N10_goal=lineEditSubsTotal.text().toInt();
    dataNodeUpdate(PACKET_PDO_SUBBLOODPUMP, &SubBPump);
    SubsPump_totalvolume=SubBPump.N10_goal;
    BloodPump_link_flag=true;
    showYesDialog(DE_TR_PROMPT,DE_TR_THEPARAMETERSAFRESH);
}
