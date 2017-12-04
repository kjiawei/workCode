#include "config_ui.h"
#include "widget_common.h"
#include "scene_setup_layout.h"
#include "scene_setup_pressure.h"

#define PRESSURE_ITEM_LAYOUT(x, y, z, posx, posy)       \
    x.setGeometry(QRect(posx, posy, 107, 22));          \
    x.setAlignment(Qt::AlignVCenter | Qt::AlignRight);  \
    x.setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
    x.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);          \
    y.setGeometry(QRect(posx+109, posy-5, 80, 30));     \
    y.setAlignment(Qt::AlignHCenter);                   \
    LINEEDIT_STYLE(y);                                  \
    z.setGeometry(QRect(posx+190, posy-5, 60, 30));     \
    z.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);   \
    z.setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
    z.setStyleSheet(CONFIG_COLOR_WHITE_CHARS)


inline void SceneSetupPressure::setUi()
{
    //title = tr("Pressure Setup");
    PressureUnit = "mmHg";

    radioButtonAuto.setGeometry(QRect(300, 75, 120, 30));
    radioButtonAuto.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    RADIOBUTTON_STYLE(radioButtonAuto);

    radioButtonManual.setGeometry(QRect(400, 75, 120, 30));
    radioButtonManual.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    RADIOBUTTON_STYLE(radioButtonManual);

    buttonSave.setGeometry(QRect(620, SCENE_SETUP_ROW_10, 64, 39));
    buttonCancel.setGeometry(QRect(694, SCENE_SETUP_ROW_10, 64, 39));

    BUTTON_SIMPLE_STYLE(buttonSave);
    BUTTON_SIMPLE_STYLE(buttonCancel);

    //Artery
    SCENE_SETUP_CREATE_TABLE(labelAP, 2);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditAP, 2);
    SCENE_SETUP_CREATE_UNIT(labelAPUnit, 2);
    labelAPUnit.setText(PressureUnit);

    SCENE_SETUP_CREATE_TABLE(labelAPHigh, 3);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditAPHigh, 3);
    SCENE_SETUP_CREATE_UNIT(labelAPHighUnit, 3);
    labelAPHighUnit.setText(PressureUnit);

    SCENE_SETUP_CREATE_TABLE(labelAPLow, 4);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditAPLow, 4);
    SCENE_SETUP_CREATE_UNIT(labelAPLowUnit, 4);
    labelAPLowUnit.setText(PressureUnit);

    //Vena
    SCENE_SETUP_CREATE_TABLE(labelVP, 5);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditVP, 5);
    SCENE_SETUP_CREATE_UNIT(labelVPUnit, 5);
    labelVPUnit.setText(PressureUnit);

    SCENE_SETUP_CREATE_TABLE(labelVPHigh, 6);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditVPHigh, 6);
    SCENE_SETUP_CREATE_UNIT(labelVPHighUnit, 6);
    labelVPHighUnit.setText(PressureUnit);

    SCENE_SETUP_CREATE_TABLE(labelVPLow, 7);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditVPLow, 7);
    SCENE_SETUP_CREATE_UNIT(labelVPLowUnit, 7);
    labelVPLowUnit.setText(PressureUnit);

    //TMP
    SCENE_SETUP_CREATE_TABLE(labelTMP, 8);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditTMP, 8);
    SCENE_SETUP_CREATE_UNIT(labelTMPUnit, 8);
    labelTMPUnit.setText(PressureUnit);

    SCENE_SETUP_CREATE_TABLE(labelTMPHigh, 9);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditTMPHigh, 9);
    SCENE_SETUP_CREATE_UNIT(labelTMPHighUnit, 9);
    labelTMPHighUnit.setText(PressureUnit);

    SCENE_SETUP_CREATE_TABLE(labelTMPLow, 10);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditTMPLow, 10);
    SCENE_SETUP_CREATE_UNIT(labelTMPLowUnit, 10);
    labelTMPLowUnit.setText(PressureUnit);

    curvePressure.setDrawCurveCount(3);
    curvePressure.setGeometry(QRect(170, 525, 570, 225));
    curvePressure.setHorizontalScale(4);
    curvePressure.setVerticalScale(5);
    curvePressure.setMode(3);
    curvePressure.setTestTime(60*60*4);
    curvePressure.setUnitX(Hour);

    curvePressure.setLineColor(Qt::red,0);
    curvePressure.setUnit(PressureUnit,0);
    curvePressure.setValueZoom(1,0);
    curvePressure.setMinValue(0,0);
    curvePressure.setMaxValue(200,0);

    curvePressure.setLineColor(Qt::blue,1);
    curvePressure.setUnit(PressureUnit,1);
    curvePressure.setValueZoom(1,1);
    curvePressure.setMinValue(0,1);
    curvePressure.setMaxValue(200,1);

    curvePressure.setLineColor(Qt::yellow,2);
    curvePressure.setUnit(PressureUnit,2);
    curvePressure.setValueZoom(1,2);
    curvePressure.setMinValue(0,2);
    curvePressure.setMaxValue(200,2);

    curvePressure.start();

    CustomSetText();

    buttonGroup.addButton(&radioButtonAuto, 0);
    buttonGroup.addButton(&radioButtonManual, 1);
    connect(&buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(buttonGroupClicked(int)));
}

SceneSetupPressure::SceneSetupPressure(QWidget *parent) :
    QObject(parent),
    labelAP(parent),
    labelAPHigh(parent),
    labelAPLow(parent),
    lineEditAP(parent),
    lineEditAPHigh(parent),
    lineEditAPLow(parent),
    labelAPUnit(parent),
    labelAPHighUnit(parent),
    labelAPLowUnit(parent),

    labelVP(parent),
    labelVPHigh(parent),
    labelVPLow(parent),
    lineEditVP(parent),
    lineEditVPHigh(parent),
    lineEditVPLow(parent),
    labelVPUnit(parent),
    labelVPHighUnit(parent),
    labelVPLowUnit(parent),

    labelTMP(parent),
    labelTMPHigh(parent),
    labelTMPLow(parent),
    lineEditTMP(parent),
    lineEditTMPHigh(parent),
    lineEditTMPLow(parent),
    labelTMPUnit(parent),
    labelTMPHighUnit(parent),
    labelTMPLowUnit(parent),

    buttonGroup(parent),
    radioButtonAuto(parent),
    radioButtonManual(parent),

    curvePressure(parent),
    buttonSave(parent),
    buttonCancel(parent)
{
    setUi();

    defaultAP.ap = -60;
    defaultAP.apUp = 200;
    defaultAP.apL = -50;

    defaultVP.vp = 30;
    defaultVP.vpUp = 150;
    defaultVP.vpL = -50;

    defaultTMP.tmp = 200;
    defaultTMP.tmpUp = 300;
    defaultTMP.tmpL = 100;

    dataNodeAddObject(PACKET_MAINTAIN_PARAMS, this);

    /*
    dataNodeAddObject(PACKET_NAME_AP, this);
    dataNodeAddObject(PACKET_NAME_VP, this);
    dataNodeAddObject(PACKET_NAME_TMP, this);
    */

    //设置卡标默认值
    /*
    dataNodeUpdate(PACKET_NAME_AP, &defaultAP);
    dataNodeUpdate(PACKET_NAME_VP, &defaultVP);
    dataNodeUpdate(PACKET_NAME_TMP, &defaultTMP);
    dataNodeAddObject(PACKET_NAME_UPDATE_AP, this);
    dataNodeAddObject(PACKET_NAME_UPDATE_VP, this);
    dataNodeAddObject(PACKET_NAME_UPDATE_TMP, this);
    */

    /*dataNodeUpdate(PACKET_NAME_UPDATE_AP, &defaultAP.ap);
    dataNodeUpdate(PACKET_NAME_UPDATE_VP, &defaultVP.vp);
    dataNodeUpdate(PACKET_NAME_UPDATE_TMP, &defaultTMP.tmp);*/
    //scaleVar = treatment_ready_to_scale;
    //dataNodeUpdate(PACKET_NAME_SCALE_AP, &scaleVar);
    //dataNodeUpdate(PACKET_NAME_SCALE_VP, &scaleVar);
    //dataNodeUpdate(PACKET_NAME_SCALE_TMP, &scaleVar);

    buttonGroupClicked(0);
    radioButtonAuto.setChecked(true);

    connect(&buttonSave, SIGNAL(clicked()), this, SLOT(SlotParameterSave()));

    curveTimer = new QTimer(parent);
    curveTimer->start(1000);

    connect(curveTimer,SIGNAL(timeout()),(QWidget *) &curvePressure,SLOT(draw()));    
    PageAppendPiece("Setup Pressure", this);

    lineEditAP.setObjectName("AP");
    lineEditAPHigh.setObjectName("AP High");
    lineEditAPLow.setObjectName("AP Low");
    lineEditVP.setObjectName("VP");
    lineEditVPHigh.setObjectName("VP High");
    lineEditVPLow.setObjectName("VP Low");
    lineEditTMP.setObjectName("TMP");
    lineEditTMPHigh.setObjectName("TMP High");
    lineEditTMPLow.setObjectName("TMP Low");
}

void SceneSetupPressure::SlotParameterSave()
{
    bool flage=false;
    CompareAssignBool(packetAP.apUp, lineEditAPHigh.text().toInt(), flage);
    CompareAssignBool(packetAP.apL, lineEditAPLow.text().toInt(), flage);

    if (flage)
        dataNodeUpdate(PACKET_NAME_AP, &packetAP);

    flage=false;
    CompareAssignBool(packetVP.vpUp, lineEditVPHigh.text().toInt(), flage);
    CompareAssignBool(packetVP.vpL, lineEditVPLow.text().toInt(), flage);

    if (flage)
        dataNodeUpdate(PACKET_NAME_VP, &packetVP);

    flage=false;
    CompareAssignBool(packetTMP.tmpUp, lineEditTMPHigh.text().toInt(), flage);
    CompareAssignBool(packetTMP.tmpL, lineEditTMPLow.text().toInt(), flage);

    if (flage)
        dataNodeUpdate(PACKET_NAME_TMP, &packetTMP);
}

void SceneSetupPressure::buttonGroupClicked(int id)
{
    if (id == 0)
    {
        lineEditAP.setReadOnly(true);
        lineEditAPHigh.setReadOnly(true);
        lineEditAPLow.setReadOnly(true);

        lineEditVP.setReadOnly(true);
        lineEditVPHigh.setReadOnly(true);
        lineEditVPLow.setReadOnly(true);

        lineEditTMP.setReadOnly(true);
        lineEditTMPHigh.setReadOnly(true);
        lineEditTMPLow.setReadOnly(true);

        //Remove
        ic->removeWidget(&lineEditAP);
        ic->removeWidget(&lineEditAPHigh);
        ic->removeWidget(&lineEditAPLow);
        ic->removeWidget(&lineEditVP);
        ic->removeWidget(&lineEditVPHigh);
        ic->removeWidget(&lineEditVPLow);
        ic->removeWidget(&lineEditTMP);
        ic->removeWidget(&lineEditTMPHigh);
        ic->removeWidget(&lineEditTMPLow);

        //dataNodeUpdate(PACKET_NAME_AP, &defaultAP);
        //dataNodeUpdate(PACKET_NAME_VP, &defaultVP);
        //dataNodeUpdate(PACKET_NAME_TMP, &defaultTMP);
    }
    else if (id == 1)
    {
        lineEditAP.setReadOnly(false);
        lineEditAPHigh.setReadOnly(false);
        lineEditAPLow.setReadOnly(false);

        lineEditVP.setReadOnly(false);
        lineEditVPHigh.setReadOnly(false);
        lineEditVPLow.setReadOnly(false);

        lineEditTMP.setReadOnly(false);
        lineEditTMPHigh.setReadOnly(false);
        lineEditTMPLow.setReadOnly(false);

        //Input
        ic->appendWidget(&lineEditAP, NumInputMethod, LineEdit, this);
        ic->appendWidget(&lineEditAPHigh, NumInputMethod, LineEdit, this);
        ic->appendWidget(&lineEditAPLow, NumInputMethod, LineEdit, this);
        ic->appendWidget(&lineEditVP, NumInputMethod, LineEdit, this);
        ic->appendWidget(&lineEditVPHigh, NumInputMethod, LineEdit, this);
        ic->appendWidget(&lineEditVPLow, NumInputMethod, LineEdit, this);
        ic->appendWidget(&lineEditTMP, NumInputMethod, LineEdit, this);
        ic->appendWidget(&lineEditTMPHigh, NumInputMethod, LineEdit, this);
        ic->appendWidget(&lineEditTMPLow, NumInputMethod, LineEdit, this);
    }
}

void SceneSetupPressure::ActionHandle(const void* dataPtr, int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupPressure-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name ==0))
        return;

    QString str;

    if ( name==PACKET_MAINTAIN_PARAMS){
        usr_params = *((Params*)dataPtr);

        lineEditAP.setText(QString::number(usr_params.auto_scale.ap_run_auto_scale_up.attribute.default_value - usr_params.auto_scale.ap_run_auto_scale_low.attribute.default_value));  //动脉压报警宽度#/
        lineEditAPHigh.setText(QString::number(usr_params.auto_scale.vp_run_auto_scale_up.attribute.default_value - usr_params.auto_scale.vp_run_auto_scale_low.attribute.default_value));//静脉压报警宽度#/
        lineEditAPLow.setText(QString::number(usr_params.auto_scale.tmp_run_auto_scale_up.attribute.default_value - usr_params.auto_scale.tmp_run_auto_scale_low.attribute.default_value));//跨膜压报警宽度#/
  //      lineEditVP.setText(QString::number(params->auto_scale.tmp_run_auto_scale_up.attribute.default_value-700));//最大跨膜压#/
         lineEditVP.setText("500");

        return;
    }

    if (name==PACKET_NAME_AP)
    {
        struct PacketAP *pAP = (struct PacketAP*)dataPtr;
        packetAP.ap = pAP->ap;
        packetAP.apUp = pAP->apUp;
        packetAP.apL = pAP->apL;
        lineEditAP.setText(str.setNum(packetAP.ap));
        lineEditAPHigh.setText(str.setNum(packetAP.apUp));
        lineEditAPLow.setText(str.setNum(packetAP.apL));
        return;
    }
    else if (name==PACKET_NAME_UPDATE_AP){
           int AP_value = *(int *)dataPtr;
           packetAP.ap = AP_value;
           lineEditAP.setText(str.setNum(packetAP.ap));
        }
    else if ( (name==PACKET_NAME_UPDATE_VP)){
           int VP_value = *(int *)dataPtr;
           packetVP.vp = VP_value;
           lineEditVP.setText(str.setNum(packetVP.vp));
        }
    else if (name==PACKET_NAME_UPDATE_TMP){
           int TMP_value = *(int *)dataPtr;
           packetTMP.tmp = TMP_value;
           lineEditTMP.setText(str.setNum(packetTMP.tmp));
        }
    if (name==PACKET_NAME_VP)
    {
        struct PacketVP *pVP = (struct PacketVP*)dataPtr;
        packetVP.vp = pVP->vp;
        packetVP.vpUp = pVP->vpUp;
        packetVP.vpL = pVP->vpL;
        lineEditVP.setText(str.setNum(packetVP.vp));
        lineEditVPHigh.setText(str.setNum(packetVP.vpUp));
        lineEditVPLow.setText(str.setNum(packetVP.vpL));
        return;
    }

    if (name==PACKET_NAME_TMP)
    {
        struct PacketTMP *pTMP = (struct PacketTMP*)dataPtr;
        packetTMP.tmp = pTMP->tmp;
        packetTMP.tmpUp = pTMP->tmpUp;
        packetTMP.tmpL = pTMP->tmpL;
        lineEditTMP.setText(str.setNum(packetTMP.tmp));
        lineEditTMPHigh.setText(str.setNum(packetTMP.tmpUp));
        lineEditTMPLow.setText(str.setNum(packetTMP.tmpL));
        return;
    }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupPressure-ActionHandle-out");
#endif
}

void SceneSetupPressure::CustomSetText()
{
    /*
    radioButtonAuto.setText(tr("Auto"));
    radioButtonManual.setText(tr("Manual"));
    buttonSave.setText(tr("Confirm"));
    buttonCancel.setText(tr("Cancel"));
    labelAP.setText(tr("A.P. :"));
    labelAPHigh.setText(tr("A.P. Upper:"));
    labelAPLow.setText(tr("A.P. Lower:"));
    labelVP.setText(tr("V.P. :"));
    labelVPHigh.setText(tr("V.P. Upper:"));
    labelVPLow.setText(tr("V.P. Lower:"));
    labelTMP.setText(tr("TMP :"));
    labelTMPHigh.setText(tr("TMP Upper:"));
    labelTMPLow.setText(tr("TMP Lower:"));
    */

    labelAP.setText(tr("A.P Alarm Range:"));
    labelAPHigh.setText(tr("V.P Alarm Range:"));
    labelAPLow.setText(tr("TMP Alarm Range:"));
    labelVP.setText(tr("Max TMP:"));
}

void SceneSetupPressure::setCusVisible(bool vis, char* para)
{
    para = para;
    /*
    labelAP.setVisible(vis);
    labelAPHigh.setVisible(vis);
    labelAPLow.setVisible(vis);
    lineEditAP.setVisible(vis);
    lineEditAPHigh.setVisible(vis);
    lineEditAPLow.setVisible(vis);
    labelVP.setVisible(vis);
    labelVPHigh.setVisible(vis);
    labelVPLow.setVisible(vis);
    lineEditVP.setVisible(vis);
    lineEditVPHigh.setVisible(vis);
    lineEditVPLow.setVisible(vis);
    labelTMP.setVisible(vis);
    labelTMPHigh.setVisible(vis);
    labelTMPLow.setVisible(vis);
    lineEditTMP.setVisible(vis);
    lineEditTMPHigh.setVisible(vis);
    lineEditTMPLow.setVisible(vis);
    radioButtonAuto.setVisible(vis);
    radioButtonManual.setVisible(vis);
    buttonSave.setVisible(false);
    buttonCancel.setVisible(false);
    labelAPUnit.setVisible(vis);
    labelAPHighUnit.setVisible(vis);
    labelAPLowUnit.setVisible(vis);
    labelVPUnit.setVisible(vis);
    labelVPHighUnit.setVisible(vis);
    labelVPLowUnit.setVisible(vis);
    labelTMPUnit.setVisible(vis);
    labelTMPHighUnit.setVisible(vis);
    labelTMPLowUnit.setVisible(vis);
    curvePressure.setVisible(vis);
    */

    labelAP.setVisible(vis);
    labelAPHigh.setVisible(vis);
    labelAPLow.setVisible(vis);
    lineEditAP.setVisible(vis);
    lineEditAPHigh.setVisible(vis);
    lineEditAPLow.setVisible(vis);

    labelVP.setVisible(vis);
    labelVPHigh.setVisible(false);
    labelVPLow.setVisible(false);
    lineEditVP.setVisible(vis);
    lineEditVPHigh.setVisible(false);
    lineEditVPLow.setVisible(false);

    labelTMP.setVisible(false);
    labelTMPHigh.setVisible(false);
    labelTMPLow.setVisible(false);
    lineEditTMP.setVisible(false);
    lineEditTMPHigh.setVisible(false);
    lineEditTMPLow.setVisible(false);
    radioButtonAuto.setVisible(false);
    radioButtonManual.setVisible(false);
    buttonSave.setVisible(false);
    buttonCancel.setVisible(false);

    labelAPUnit.setVisible(vis);
    labelAPHighUnit.setVisible(vis);
    labelAPLowUnit.setVisible(vis);
    labelVPUnit.setVisible(vis);
    labelVPHighUnit.setVisible(false);
    labelVPLowUnit.setVisible(false);
    labelTMPUnit.setVisible(false);
    labelTMPHighUnit.setVisible(false);
    labelTMPLowUnit.setVisible(false);

    curvePressure.setVisible(false);
}
