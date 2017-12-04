#include <QtGui>
#include "config_ui.h"
#include "xml_parse.h"
#include "scene_setup_layout.h"
#include "scene_setup_heparin.h"
#include "deal.h"
#include "global_var.h"
#include "common_tr_string.h"

#include <unistd.h>

inline void SceneSetupHeparin::setUi()
{
    //title = tr("Heparin");
#define OFFSET_H 0
    SCENE_SETUP_CREATE_TABLE(labelSyringeStyle, 5);
    SCENE_SETUP_CREATE_VALUE(labelSyringeStyleValue, 5);
    SCENE_SETUP_CREATE_UNIT(labelSyringeStyleunit, 5);
    labelSyringeStyleunit.setText("mL");
   // labelSyringeStyle.setGeometry(QRect(SCENE_SETUP_COL_LABEL, SCENE_SETUP_ROW_1,300, 38));
   // labelSyringeStyleValue.setGeometry(QRect(SCENE_SETUP_COL_LINEEDIT, SCENE_SETUP_ROW_1,110, 38));
   // labelSyringeStyleunit.setGeometry(QRect(SCENE_SETUP_COL_UNIT, SCENE_SETUP_ROW_1,100, 38));

    SCENE_SETUP_CREATE_TABLE(labelShotTotal, 6);
    SCENE_SETUP_CREATE_VALUE(labelShotTotalValue, 6);
    SCENE_SETUP_CREATE_UNIT(labelShotTotalUnit, 6);
   // labelShotTotal.setGeometry(QRect(SCENE_SETUP_COL_LABEL, SCENE_SETUP_ROW_2,300, 38));
   // labelShotTotalValue.setGeometry(QRect(SCENE_SETUP_COL_LINEEDIT, SCENE_SETUP_ROW_2,110, 38));
   // labelShotTotalUnit.setGeometry(QRect(SCENE_SETUP_COL_UNIT, SCENE_SETUP_ROW_2,100, 38));
    labelShotTotalValue.setNum(0);
    labelShotTotalUnit.setText("mL");

    SCENE_SETUP_CREATE_TABLE(labelFirstDose, 2);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditFirstDose, 2);
    SCENE_SETUP_CREATE_UNIT(labelFirstDoseUnit, 2);
    //labelFirstDose.setGeometry(QRect(SCENE_SETUP_COL_LABEL, SCENE_SETUP_ROW_3,300, 38));
    //lineEditFirstDose.setGeometry(QRect(SCENE_SETUP_COL_LINEEDIT, SCENE_SETUP_ROW_3,110, 38));
   // labelFirstDoseUnit.setGeometry(QRect(SCENE_SETUP_COL_UNIT, SCENE_SETUP_ROW_3,100, 38));
    labelFirstDoseUnit.setText("mL");

    SCENE_SETUP_CREATE_TABLE(labelShotSpeed, 3);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditShotSpeed, 3);
    SCENE_SETUP_CREATE_UNIT(labelShotSpeedUnit, 3);
    //labelShotSpeed.setGeometry(QRect(SCENE_SETUP_COL_LABEL, SCENE_SETUP_ROW_4,300, 38));
    //lineEditShotSpeed.setGeometry(QRect(SCENE_SETUP_COL_LINEEDIT, SCENE_SETUP_ROW_4,110, 38));
    //labelShotSpeedUnit.setGeometry(QRect(SCENE_SETUP_COL_UNIT, SCENE_SETUP_ROW_4,100, 38));
    labelShotSpeedUnit.setText("mL/h");

    SCENE_SETUP_CREATE_TABLE(labelTimePreStop, 4);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditTimePreStop, 4);
    SCENE_SETUP_CREATE_UNIT(labelTimePreStopUnit, 4);
    //labelTimePreStop.setGeometry(QRect(SCENE_SETUP_COL_LABEL, SCENE_SETUP_ROW_5,300, 38));
    //lineEditTimePreStop.setGeometry(QRect(SCENE_SETUP_COL_LINEEDIT, SCENE_SETUP_ROW_5,110, 38));
    //labelTimePreStopUnit.setGeometry(QRect(SCENE_SETUP_COL_UNIT, SCENE_SETUP_ROW_5,100, 38));
    labelTimePreStopUnit.setText("min");

    SCENE_SETUP_CREATE_TABLE(labelAddSingle, 9);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditAddSingle, 9);
    SCENE_SETUP_CREATE_UNIT(labelAddSingleUnit, 9 );
    labelAddSingleUnit.setText("mL");
    buttonAddSingle.setStyleSwitch(":/resource/images/setup/function_normal.png",
            ":/resource/images/setup/function_active.png", CONFIG_COLOR_BLUE,
            QRect(615, SCENE_SETUP_ROW_9, 64, 39));

    QDoubleValidator *doubleVal = new QDoubleValidator(0, 9.9, 1, this);
    doubleVal->setNotation(QDoubleValidator::StandardNotation);
    lineEditShotSpeed.setValidator(doubleVal);
    lineEditFirstDose.setValidator(doubleVal);

    radioButtonSelectYes.setGeometry(QRect(SCENE_SETUP_COL_LINEEDIT-200,
                SCENE_SETUP_ROW_1, 300, 35));
    radioButtonSelectYes.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    radioButtonSelectYes.setStyleSheet("QCheckBox {color:#dce6f0;}"
                   "QCheckBox::indicator {width: 29px;height: 31px;} "
                   "QCheckBox::indicator::checked {image: url(:/resource/images/radio_button_check.png);} "
                   "QCheckBox::indicator::unchecked {image: url(:/resource/images/radio_button_uncheck.png);}");
    radioButtonSelectYes.setChecked(false);
    heparinData.selstatus=0;
    heparinData.runstatus=0;

    buttonSave.setGeometry(QRect(290+50, SCENE_SETUP_ROW_7+10, 64, 39));
    buttonCancel.setGeometry(QRect(364+50, SCENE_SETUP_ROW_7+10, 64, 39));
    buttonApply.setGeometry(QRect(438+50, SCENE_SETUP_ROW_7+10, 64, 39));

    BUTTON_SIMPLE_STYLE(buttonSave);
    BUTTON_SIMPLE_STYLE(buttonCancel);
    BUTTON_SIMPLE_STYLE(buttonApply);

    curveHeparin.setGeometry(QRect(180, 485, 540, 250));
    curveHeparin.setUnit("mL");
    curveHeparin.setValueZoom(10);
    curveHeparin.setHorizontalScale(4);
    curveHeparin.setVerticalScale(5);
    curveHeparin.delAllTargetCurve();
    curveHeparin.delAllTestPoint();
    curveHeparin.setMode(1);
    curveHeparin.setMinValue(0);
    curveHeparin.setMaxValue(500);
    curveHeparin.setTestTime(60*60*4);
    curveHeparin.setUnitX(Hour);
    curveHeparin.start();

    CustomSetText();
}

SceneSetupHeparin::SceneSetupHeparin(QWidget *parent) :
    QObject(parent),
    labelSyringeStyle(parent),
    labelSyringeStyleValue(parent),
    labelSyringeStyleunit(parent),

    labelShotTotal(parent),
    labelShotTotalValue(parent),
    labelShotTotalUnit(parent),

    labelFirstDose(parent),
    lineEditFirstDose(parent),
    labelFirstDoseUnit(parent),

    labelShotSpeed(parent),
    lineEditShotSpeed(parent),
    labelShotSpeedUnit(parent),

    labelTimePreStop(parent),
    lineEditTimePreStop(parent),
    labelTimePreStopUnit(parent),

    radioButtonSelectYes(parent),
    groupHeparin(parent),

    labelAddSingle(parent),
    lineEditAddSingle(parent),
    labelAddSingleUnit(parent),

    buttonAddSingle(parent),

    curveHeparin(parent),
    buttonSave(parent),
    buttonApply(parent),
    buttonCancel(parent)
{
    setUi();

    connect(&buttonSave, SIGNAL(clicked()), this, SLOT(SlotParameterSave()));
    connect(&buttonCancel, SIGNAL(clicked()), this, SLOT(StopHeparinPump()));
    connect(&buttonApply, SIGNAL(clicked()), this, SLOT(SlotApplyClicked()));

    connect(&radioButtonSelectYes,SIGNAL(stateChanged(int)),this,SLOT(ApplyStatusHeparin(int)));
    connect(&buttonAddSingle,SIGNAL(clicked()),this,SLOT(SlotAddSingleClicked()));


    dataNodeAddObject(PACKET_NAME_HEPARINDATA, this);
    dataNodeAddObject(PACKET_NAME_HEPARINPUMP, this);
    dataNodeAddObject(PACKET_MAINTAIN_PARAMS, this);
    dataNodeAddObject(PACKET_ENGINEER,this);
    dataNodeAddObject(PACKET_REBIRTH_HANDLE,this);  //重生操作处理#/

    PageAppendPiece("Setup Heparin", this);
    memset(&heparinData,0,sizeof(PacketHeparinPump));
    isKeyInit = false;
    AddSingle_flag=false;
}

//是否启用肝素泵单选按钮#/
void SceneSetupHeparin::ApplyStatusHeparin(int id)
{
    id = id;

    struct PackN15PDO N15PDO;
    QString path;
    QMap<QString, QString> mapV;
    QDomDocument doc;

    dataRead(PACKET_NAME_HEPARINDATA,&N15PDO);

    if(N15PDO.workMode!=0x01)
    {
        if(!(radioButtonSelectYes.isChecked())){  //取消选择肝素泵#/
            //发送停止肝素泵的命令
            MasterCtrol_CMD_HeparinPumpStop();
            usleep(10000);
            //HeparinPump_ActStop();
    //        buttonApply.setText(tr("Run"));
            heparinData.selstatus=0;
            G_HeparinEnable_flag=false;  //肝素泵使能标志#/
        }
        else
        {
            heparinData.selstatus=1;
            G_HeparinEnable_flag=true;  //肝素泵使能标志#/
        }

        xml_DocReadOpen(&doc, DEFAULT_XML_FILE);
        mapV.clear();
        path.clear();
        path.append(tr("/Config/Engineer_Factoryset/HeparinMode"));
        mapV.insert("Model", QString::number(heparinData.selstatus));//heparin_flag
        xml_NodeWriteValue(&doc, path.toLatin1().data(), &mapV);
        waiteToFile(&doc, DEFAULT_XML_FILE);
        dataWrite(PACKET_NAME_HEPARINPUMP,&heparinData);
        setCusVisible(true,NULL);
        dataNodeUpdate(PACKET_NAME_HEPARINPUMP,&heparinData);
    }
    else
    {
        if(radioButtonSelectYes.isVisible()==true)
        {
            MESSAGE_PROMPT_BOX(DE_TR_PROMPT,DE_TR_HEPARINISRUNNING);
        }
        radioButtonSelectYes.setChecked(true);
        setCusVisible(true,NULL);

    }
}

void SceneSetupHeparin::lineEdit_init()
{

    lineEditFirstDose.setObjectName("Heparin FirstDose");
    lineEditShotSpeed.setObjectName("Heparin ShotSpeed");
    lineEditTimePreStop.setObjectName("Heparin PreStopTime");
    lineEditAddSingle.setObjectName("Heparin AddSingle");

    ic->appendWidget(&lineEditFirstDose, NumInputMethod, LineEdit, this,
                     params->Params_Heparin.First_Volume.attribute.min_value,
                     params->Params_Heparin.First_Volume.attribute.max_value,
                     params->Params_Heparin.First_Volume.attribute.step_value,
                     tr("Initial Dose:"),"Initial Dose");

    ic->appendWidget(&lineEditShotSpeed, NumInputMethod, LineEdit, this,
                     params->Params_Heparin.Rate.attribute.min_value,
                     params->Params_Heparin.Rate.attribute.max_value,
                     params->Params_Heparin.Rate.attribute.step_value,
                     tr("Dose rate:"),"Dose rate");

    ic->appendWidget(&lineEditTimePreStop, NumInputMethod, LineEdit, this,
                     params->Params_Heparin.Pre_StopTime.attribute.min_value,
                     params->Params_Heparin.Pre_StopTime.attribute.max_value,
                     params->Params_Heparin.Pre_StopTime.attribute.step_value,
                     tr("Stop time:"),"Stop time");

    /*
    ic->appendWidget(&lineEditAddSingle, NumInputMethod, LineEdit, this,
                     params->Params_Heparin.add_Volume.attribute.min_value,
                     params->Params_Heparin.add_Volume.attribute.max_value,
                     params->Params_Heparin.add_Volume.attribute.step_value,
                     tr("BOLUS:"),"BOLUS");
                     */

    ic->appendWidget(&lineEditAddSingle, NumInputMethod, LineEdit, this,
                     params->Params_Heparin.First_Volume.attribute.min_value,
                     params->Params_Heparin.First_Volume.attribute.max_value,
                     params->Params_Heparin.First_Volume.attribute.step_value,
                     tr("BOLUS:"),"BOLUS");
}


void SceneSetupHeparin::Init()
{
    switch (heparinData.canSt) {
        case STATE_CAN_Pre_operational:
            if (HeparinPump_CanStart() != 0) {
                return;
            }
        case STATE_CAN_Operational:
            usleep(20000);
            HeparinPump_ActReadDoseSpeed();//读取肝素注射速率#/
            usleep(40000);
            HeparinPump_ActReadSyring();//读取肝素类型#/
            usleep(10000);
            break;
        case STATE_CAN_Stopped:
            HeparinPump_CanStart();
            break;
    }
}

//设置参数保存#/
void SceneSetupHeparin::SlotParameterSave()
{
    float dose = lineEditFirstDose.text().toFloat();
    float speed = lineEditShotSpeed.text().toFloat();
    float time = lineEditTimePreStop.text().toFloat();
    if (heparinData.doseSpeed != int(speed*10) ||
            heparinData.doseFirst != int(dose*10)) {

        heparinPDO.speed = (unsigned short int) (speed*10);
        heparinPDO.dose = (unsigned short int) (dose*10);
        heparinPDO.preStopTime = (unsigned short int)time;
        heparinPreStop_sec=time*60;
        heparinPreStop =false;
        //利用数据中心去发送数据。。。
        dataNodeUpdate(PACKET_PDO_HEPARINPUMP,&heparinPDO);

        //HeparinPump_ActDoseSpeed(dose, speed);
        //usleep(40000);
    }
}

void SceneSetupHeparin::StopHeparinPump()
{
    Init();
    QString str;
    lineEditFirstDose.setText(str.setNum((float(heparinData.doseFirst))/10));
    lineEditShotSpeed.setText(str.setNum((float(heparinData.doseSpeed))/10));
}

void SceneSetupHeparin::SlotAddSingleClicked()
{
    float vol=0;
    if(AddSingle_flag==false)
    {
//        buttonAddSingle.setDisabled(true);
        AddSingle_flag=true;
        vol=lineEditAddSingle.text().toFloat();
//        MasterCtrol_CMD_HeparinPumpAddSingle((unsigned char)(vol*10));//启动肝素 带快推量的快推#/

        heparinPDO.dose = vol*10;
        //利用数据中心去发送数据。。。
        dataNodeUpdate(PACKET_PDO_HEPARINPUMP,&heparinPDO);
        //设置单次追加首剂量和速度#/
        Heparin_CMD_SetQuickAddVol(vol*10,heparinPDO.speed);
        usleep(20000);
        //单次追加首剂量，速度为快推#/
        Heparin_CMD_HeparinPumpQuickAdd();
    }
    else if(AddSingle_flag==true)
    {
        //       buttonAddSingle.setChecked(true);
        buttonAddSingle.setChecked(false);
        //    buttonAddSingle.setDisabled(false);

        //停止肝素泵，不清总量，能下拉#/
        Heparin_CMD_HeparinPumpStopCanDown();
        usleep(20000);
        heparinPDO.dose = (unsigned short int) (lineEditFirstDose.text().toFloat()*10);
        //利用数据中心去发送数据。。。
        dataNodeUpdate(PACKET_PDO_HEPARINPUMP,&heparinPDO);
        AddSingle_flag=false;
    }
}

//肝素运行按键#/
void SceneSetupHeparin::SlotApplyClicked()
{
    if(!(radioButtonSelectYes.isChecked())){
        return ;
    }
    else
    {
        if(heparinData.runstatus==0)
        {
            heparinData.runstatus=1;
            //发送启动肝素泵的命令
            if(HeparinPumpFirstflag==false)  //首剂量标志#/
            {
                MasterCtrol_CMD_HeparinPumpStartHaveFirst();
                usleep(20000);
                HeparinPumpFirstflag=true;
            }
            else if(HeparinPumpFirstflag==true)
            {
                MasterCtrol_CMD_HeparinPumpStartNoFirst();
                usleep(20000);
            }

            buttonApply.setText(tr("Stop"));
        }
        else
        {
            heparinData.runstatus=0;
            //发送停止肝素泵的命令
            MasterCtrol_CMD_HeparinPumpStop();
            usleep(20000);
            buttonApply.setText(tr("Run"));
        }
    }
    dataNodeUpdate(PACKET_NAME_HEPARINPUMP,&heparinData);
/*
    if (heparinData.workMode == 2) {
        SlotParameterSave();
        usleep(20000);
        //发送启动肝素泵的命令
        MasterCtrol_CMD_HeparinPumpStartHaveFirst();
        //HeparinPump_ActStart();
        buttonApply.setText(tr("Stop"));
    } else if (heparinData.workMode == 1) {

        //发送停止肝素泵的命令
        MasterCtrol_CMD_HeparinPumpStop();
        //HeparinPump_ActStop();
        buttonApply.setText(tr("Run"));
    }
*/

}

void SceneSetupHeparin::setInputValue(int value)
{
    curveHeparin.addTestPoint(value);
}

void SceneSetupHeparin::StateSwitch(const unsigned int dst)
{
    switch (dst) {
        case STATE_CAN_Initialisation:
        case STATE_CAN_Stopped:
        case STATE_CAN_Pre_operational:
        case STATE_CAN_Operational:
        default:
            break;
    }
}

void SceneSetupHeparin::ActionHandle(const void* dataPtr,int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupHeparin-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name ==0) ) {
        return;
    }

    /*
    WorkState workstateTMP;
    static char hep_count=0;
    workstateTMP = GetWorkStateManage();  //读肝素的容量类型#/
    if(workstateTMP.tipState.current==TREAT_READY&&workstateTMP.subState.current>=SELFTEST&&G_HEPARIN_STYLE==0&&(hep_count<7))
    {
        if((hep_count%2==0)||hep_count==0)
        {
            MasterCtrol_CMD_EEporm_read(1552);
            us_sleep(100000);
            ActReadEEprom(3,1552);
        }
        hep_count++;        
    }
    */

    if(name==PACKET_NAME_HEPARINDATA){
        PackN15PDO *heparnPtr = (PackN15PDO*)dataPtr;

        heparinData.senserState=heparnPtr->senserState; //限位开关状态

        if(heparinData.workMode!=heparnPtr->workMode)
        {
            heparinData.workMode=heparnPtr->workMode;//工作状态  01H:启动   02H:停止  03H:暂停  04H:自检  06

            if(heparinData.workMode==0x01)
            {
           //     buttonApply.setText(tr("Stop"));
                heparinData.runstatus=1;
            }
            else
            {
            //    buttonApply.setText(tr("Run"));
                heparinData.runstatus=0;
            }
        }

        if (heparinData.inputAllValue != heparnPtr->inputAllValue) {  //累计注入量#/
            heparinData.inputAllValue = heparnPtr->inputAllValue;
            labelShotTotalValue.setNum((float(heparinData.inputAllValue))/10);
 //           curveHeparin.addTestPoint(heparinData.inputAllValue);
//            curveHeparin.draw();
        }

        if (heparinData.doseSpeed != heparnPtr->doseSpeed) {  //注入速度#/
            heparinData.doseSpeed = heparnPtr->doseSpeed;
        }

        if(heparinData.workModeSub != heparnPtr->workModeSub) {
            heparinData.workModeSub = heparnPtr->workModeSub;
        }

        if(this->buttonAddSingle.isVisible())
        {
            if((heparinData.workModeSub==0x02||heparinData.workModeSub==0x03)&&(heparinData.workMode==0x01))
            {
                buttonAddSingle.setChecked(true);
    //            buttonAddSingle.setDisabled(true);
            }
            else
            {
                buttonAddSingle.setChecked(false);
    //            buttonAddSingle.setDisabled(false);
                if(AddSingle_flag==true)
                {
                    heparinPDO.dose = (unsigned short int) (lineEditFirstDose.text().toFloat()*10);
                    //利用数据中心去发送数据。。。
                    dataNodeUpdate(PACKET_PDO_HEPARINPUMP,&heparinPDO);
                    AddSingle_flag=false;
                }
            }
        }

        labelShotTotalValue.setNum(heparinData.inputAllValue/10.0);
        lineEditShotSpeed.setText(QString().setNum(heparinData.doseSpeed/10.0));

        return;
    }
    else if(name==PACKET_ENGINEER) {
        struct Engineer engineer=*((Engineer*)dataPtr);

        if(engineer.factoryset.HeparinSelect==false)
        {
            radioButtonSelectYes.setChecked(false);
            heparinData.selstatus=0;
            G_HeparinEnable_flag=false;  //肝素泵使能标志#/
        }
        else
        {
            radioButtonSelectYes.setChecked(true);
            heparinData.selstatus=1;
            G_HeparinEnable_flag=true;  //肝素泵使能标志#/
        }

        dataWrite(PACKET_NAME_HEPARINPUMP,&heparinData);
        setCusVisible(false,NULL);
        dataNodeUpdate(PACKET_NAME_HEPARINPUMP,&heparinData);
    }
    else if(name==PACKET_NAME_HEPARINPUMP) {

        PacketHeparinPump *heparnPtr = (PacketHeparinPump*)dataPtr;
/*
        if(heparinData.selstatus==0)
            radioButtonSelectYes.setChecked(false);
        else
            radioButtonSelectYes.setChecked(true);

            */

        heparinData.runstatus=heparnPtr->runstatus;
        /*
        if(heparinData.runstatus==0)
            buttonApply.setText(tr("Run"));
        else
            buttonApply.setText(tr("Stop"));
            */

        /*
        if (heparinData.inputAllValue != heparnPtr->inputAllValue) {  //累计注入量#/
            heparinData.inputAllValue = heparnPtr->inputAllValue;
            labelShotTotalValue.setNum((float(heparinData.inputAllValue))/10);
 //           curveHeparin.addTestPoint(heparinData.inputAllValue);
//            curveHeparin.draw();
        }

        if (heparinData.doseSpeed != heparnPtr->doseSpeed) {  //注入速度#/
            heparinData.doseSpeed = heparnPtr->doseSpeed;
            lineEditShotSpeed.setText(str.setNum((float(heparinData.doseSpeed))/10));

            //tmp
            //主要思路是由下面更新了速度，我们的pdo也是同步更新
            heparinPDO.speed = heparinData.doseSpeed;
            changePDO = true;
        }

        if (heparinData.workMode != heparnPtr->workMode) {
            heparinData.workMode = heparnPtr->workMode;
        }

        if (heparinData.quickSpeed != heparnPtr->quickSpeed) {
            heparinData.quickSpeed = heparnPtr->quickSpeed;
        }

        if(heparinData.workModeSub != heparnPtr->workModeSub) {
            heparinData.workModeSub = heparnPtr->workModeSub;
        }

        if(heparinData.syringeLen != heparnPtr->syringeLen) {
            heparinData.syringeLen = heparnPtr->syringeLen;
        }

        if(heparinData.syringeStyle != heparnPtr->syringeStyle) {
            heparinData.syringeStyle = heparnPtr->syringeStyle;
            labelSyringeStyleValue.setNum((float(heparinData.syringeStyle))/10);
        }

        if(heparinData.doseFirst != heparnPtr->doseFirst) {  //首剂量#/
            heparinData.doseFirst = heparnPtr->doseFirst;
            lineEditFirstDose.setText(str.setNum((float(heparinData.doseFirst))/10));

            //tmp
            //主要思路是由下面更新了首剂量，我们的pdo也是同步更新
            heparinPDO.dose = heparinData.doseFirst;
            changePDO = true;
        }

        if (heparinData.canSt != heparnPtr->canSt) {
            heparinData.canSt = heparnPtr->canSt;
        }

        //如果下面更新了速度或首剂量，我们的pdo也要同步更新。
        if (changePDO) {
            dataNodeUpdate(PACKET_PDO_HEPARINPUMP,&heparinPDO);
        }
        */
    } else if(name==PACKET_MAINTAIN_PARAMS) {
        params = (Params*)dataPtr;

        labelSyringeStyleValue.setNum(6);
        lineEditFirstDose.setText(QString::number(params->Params_Heparin.First_Volume.attribute.default_value));
        lineEditShotSpeed.setText(QString::number(params->Params_Heparin.Rate.attribute.default_value));
        lineEditTimePreStop.setText(QString::number(params->Params_Heparin.Pre_StopTime.attribute.default_value));
        lineEditAddSingle.setText(QString::number(params->Params_Heparin.add_Volume.attribute.default_value));
        heparinPreStop_sec=params->Params_Heparin.Pre_StopTime.attribute.default_value*60;       

        heparinPDO.dose = (unsigned short int) (params->Params_Heparin.First_Volume.attribute.default_value*10);
        heparinPDO.speed= (unsigned short int) (params->Params_Heparin.Rate.attribute.default_value*10);
        heparinPDO.preStopTime = (unsigned short int)params->Params_Heparin.Pre_StopTime.attribute.default_value;
        heparinPreStop_sec=heparinPDO.preStopTime*60;
        //利用数据中心去发送数据。。。
        dataNodeUpdate(PACKET_PDO_HEPARINPUMP,&heparinPDO);


        if(G_HEPARIN_STYLE==0)
        {

            MasterCtrol_CMD_EEporm_read(1552);
            us_sleep(100000);
            ActReadEEprom(3,1552);
        }
    }
    else  if(name==PACKET_REBIRTH_HANDLE)//重生恢复操作处理#/
    {
        heparinPreStop_sec=G_P_REBIRTH->heparinPreStop_sec;  //肝素泵预先停止时间 秒#/
        heparinPreStop=G_P_REBIRTH->heparinPreStop;     //肝素泵的运行时间到标志#/
        HeparinPumpFirstflag=G_P_REBIRTH->HeparinPumpFirstflag; //肝素泵首剂量是否已运行标志
        heparinPDO=G_P_REBIRTH->heparinPDO;  //肝素泵参数#/

        lineEditFirstDose.setText(QString::number(heparinPDO.dose/10.0));
        lineEditShotSpeed.setText(QString::number(heparinPDO.speed/10.0));
        lineEditTimePreStop.setText(QString::number(heparinPreStop_sec/60));

        dataNodeUpdate(PACKET_PDO_HEPARINPUMP,&heparinPDO);
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupHeparin-ActionHandle-out");
#endif
}

void SceneSetupHeparin::CustomSetText()
{
    labelSyringeStyle.setText(tr("Syringe Type:"));
    labelShotTotal.setText(tr("Acc heparin:"));
    labelFirstDose.setText(tr("Initial Dose:"));
    labelShotSpeed.setText(tr("Dose rate:"));
    labelTimePreStop.setText(tr("Stop time:"));
    labelAddSingle.setText(tr("BOLUS:"));
    radioButtonSelectYes.setText(tr("Heparin Enable"));
    buttonSave.setText(tr("Confirm"));
    buttonCancel.setText(tr("Cancel"));
    buttonApply.setText(tr("Run"));
}

void SceneSetupHeparin::setCusVisible(bool vis, char* para)
{
    para = para;
    if(vis && !isKeyInit) {
        isKeyInit = true;
        lineEdit_init();
    }

    if(vis)
        labelSyringeStyleValue.setText(QString::number(G_HEPARIN_STYLE/10));

    radioButtonSelectYes.setVisible(vis);

    if(vis==true)
    {
        if(!(radioButtonSelectYes.isChecked()))
        {
            if(G_HeparinEnable_flag==true)
            {
                radioButtonSelectYes.setChecked(true);
            }
            else
                vis=false;
        }
    }
    labelSyringeStyle.setVisible(vis);
    labelSyringeStyleValue.setVisible(vis);
    labelSyringeStyleunit.setVisible(vis);
    labelFirstDose.setVisible(vis);
    labelFirstDoseUnit.setVisible(vis);
    lineEditFirstDose.setVisible(vis);
    labelShotSpeed.setVisible(vis);
    lineEditShotSpeed.setVisible(vis);
    labelShotSpeedUnit.setVisible(vis);
    labelTimePreStop.setVisible(vis);
    lineEditTimePreStop.setVisible(vis);
    labelTimePreStopUnit.setVisible(vis);
    labelAddSingle.setVisible(vis);
    lineEditAddSingle.setVisible(vis);
    labelAddSingleUnit.setVisible(vis);
    buttonAddSingle.setVisible(vis);
    labelShotTotal.setVisible(vis);
    labelShotTotalValue.setVisible(vis);
    labelShotTotalUnit.setVisible(vis);
    curveHeparin.setVisible(vis);    
    buttonSave.setVisible(false);
    buttonApply.setVisible(false);
    buttonCancel.setVisible(false);
}
