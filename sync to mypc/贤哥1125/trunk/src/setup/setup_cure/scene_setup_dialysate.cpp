#include "config_ui.h"
#include "scene_setup_layout.h"
#include "scene_setup_dialysate.h"

inline void SceneSetupDialysate::setUi()
{
    SCENE_SETUP_CREATE_TABLE(labelConcentrationNa, 1);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditConcentrationNa, 1);
    SCENE_SETUP_CREATE_UNIT(labelConcentrationNaUnit, 1);
    labelConcentrationNaUnit.setText("mmol/L");

    SCENE_SETUP_CREATE_TABLE(labelConcentrationHCO3, 2);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditConcentrationHCO3, 2);
    SCENE_SETUP_CREATE_UNIT(labelConcentrationHCO3Unit, 2);
    labelConcentrationHCO3Unit.setText("mmol/L");

    SCENE_SETUP_CREATE_TABLE(labelVelocity, 3);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditVelocity, 3);
    SCENE_SETUP_CREATE_UNIT(labelVelocityUnit, 3);
    labelVelocityUnit.setText("ml/min");

    SCENE_SETUP_CREATE_TABLE(labelTemp, 4);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditTemp, 4);
    SCENE_SETUP_CREATE_UNIT(labelTempUnit, 4);
    labelTempUnit.setText("'C");

    buttonSave.setGeometry(QRect(620, 500, 64, 39));
    buttonCancel.setGeometry(QRect(694, 500, 64, 39));
    buttonCurve.setGeometry(QRect(620, 80, 64, 39));

    BUTTON_SIMPLE_STYLE(buttonSave);
    BUTTON_SIMPLE_STYLE(buttonCancel);
    BUTTON_SIMPLE_STYLE(buttonCurve);


    lineEditConcentrationNa.setObjectName("Dialy Na");
    lineEditConcentrationHCO3.setObjectName("Dialy HCO3");
    lineEditVelocity.setObjectName("Dialy Velocity");
    lineEditTemp.setObjectName("Dialy Temp");

    ic->appendWidget(&lineEditConcentrationNa, NumInputMethod, LineEdit, this,136,145,1);//NA 离子的上下限#/
    ic->appendWidget(&lineEditConcentrationHCO3, NumInputMethod, LineEdit, this,20,40,1);
    ic->appendWidget(&lineEditVelocity, NumInputMethod, LineEdit, this,0,1000,1);
    ic->appendWidget(&lineEditTemp, NumInputMethod, LineEdit, this,35,39,0.1);

    CustomSetText();
}

SceneSetupDialysate::SceneSetupDialysate(QWidget *parent) :
    QObject(parent),
    labelConcentrationNa(parent),
    lineEditConcentrationNa(parent),
    labelConcentrationNaUnit(parent),

    labelConcentrationHCO3(parent),
    lineEditConcentrationHCO3(parent),
    labelConcentrationHCO3Unit(parent),

    labelVelocity(parent),
    lineEditVelocity(parent),
    labelVelocityUnit(parent),

    labelTemp(parent),
    lineEditTemp(parent),
    labelTempUnit(parent),

    buttonSave(parent),
    buttonCancel(parent),
    buttonCurve(parent)

{
    setUi();

    connect(&buttonCurve,SIGNAL(clicked()),this,SLOT(on_curve_clicked()));
    connect(&buttonSave,SIGNAL(clicked()),this,SLOT(Confirm()));
    dataNodeAddObject(PACKET_FORMULA_TARGET, this);
    dataNodeAddObject(PACKET_NAME_WORKMODE,this);
    dataNodeAddObject(PACKET_MAINTAIN_PARAMS,this);
    dataNodeAddObject(PACKET_NAME_DIALYSATE, this);
    dataNodeAddObject(PACKET_REBIRTH_HANDLE,this);  //重生操作处理#/

    PageAppendPiece("Setup Dialysate", this);
}

void SceneSetupDialysate::on_curve_clicked()
{
    PageShow("Setup CurveNa");
}

void SceneSetupDialysate::Confirm()
{
    float Na = lineEditConcentrationNa.text().toFloat();
    float HCO3 = lineEditConcentrationHCO3.text().toFloat();

    enginerDialog->showNaHCO3(QString::number(Na),QString::number(HCO3));

    formulaSet.tempBuffer.na = Na;
    formulaSet.tempBuffer.hco3 = HCO3;
    updateFaisonFormulaCond(&formulaSet);

    PacketFluid packet;
//    packet.Wspeed = lineEditVelocity.text().toShort();
//    packet.tempTarget = lineEditTemp.text().toShort();
    packet.Wspeed = lineEditVelocity.text().toFloat();
    packet.tempTarget = (lineEditTemp.text().toFloat()*10);  //精确到小数点后一位#/
    dataNodeUpdate(PACKET_NAME_DIALYSATE,&packet);
}

void SceneSetupDialysate::ActionHandle(const void* dataPtr,int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupDialysate-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name == 0)) {
        return;
    }

    if (name==PACKET_NAME_DIALYSATE) {
        PacketFluid *packet = (PacketFluid*)dataPtr;
        lineEditVelocity.setText(QString::number(packet->Wspeed));
        lineEditTemp.setText(QString::number(packet->tempTarget/10.0,'f',1));
    } else if (name==PACKET_FORMULA_TARGET) {
         formulaSet = *((struct FormulaSet*)dataPtr);
         lineEditConcentrationNa.setText(QString::number(formulaSet.tempBuffer.na));
         lineEditConcentrationHCO3.setText(QString::number(formulaSet.tempBuffer.hco3));
         enginerDialog->showNaHCO3(QString::number(formulaSet.tempBuffer.na),QString::number(formulaSet.tempBuffer.hco3));
    }
    else if (name==PACKET_NAME_WORKMODE) {//改变治疗模式#/
        PacketWorkMode workModeData = *(PacketWorkMode *)dataPtr;
        PacketFluid packet;

        dataRead(PACKET_NAME_DIALYSATE,&packet);

        if(workModeData.treatmode.current==workModeData.treatmode.target)
        {
            if(workModeData.treatmode.current == HDF_pre_dilute)
            {
                lineEditVelocity.setText(QString::number(usr_params.Params_Dialyser.OnlineRate.attribute.default_value));
            }
            else if(workModeData.treatmode.current == HDF_post_dilute)
            {
                lineEditVelocity.setText(QString::number(usr_params.Params_Dialyser.OnlineRate.attribute.default_value));
            }
            else
            {
                lineEditVelocity.setText(QString::number(usr_params.Params_Dialyser.Rate.attribute.default_value));
            }

            packet.Wspeed = lineEditVelocity.text().toFloat();
            packet.tempTarget = (lineEditTemp.text().toFloat()*10);  //精确到小数点后一位#/
            dataNodeUpdate(PACKET_NAME_DIALYSATE,&packet);
        }
    }
    else if(name==PACKET_MAINTAIN_PARAMS)
    {
         struct Params *params = (Params*)dataPtr;
         PacketWorkMode workModeData = *(PacketWorkMode *)dataPtr;
         PacketFluid packet;

         if(G_SAVECUREPARAM_FLAG==0)
         {
             formulaSet.tempBuffer.na=G_PRE_NA_VALUE;
             lineEditConcentrationNa.setText(QString::number(formulaSet.tempBuffer.na));
             enginerDialog->showNaHCO3(QString::number(formulaSet.tempBuffer.na),QString::number(formulaSet.tempBuffer.hco3));
             updateFaisonFormulaCond(&formulaSet);
         }

         lineEditTemp.setText(QString::number(params->Params_Dialyser.Temp.attribute.default_value,'f',1));

         dataRead(PACKET_NAME_DIALYSATE,&packet);

         if(workModeData.treatmode.current==workModeData.treatmode.target)
         {
             if(workModeData.treatmode.current == HDF_pre_dilute)
             {
                 lineEditVelocity.setText(QString::number(params->Params_Dialyser.OnlineRate.attribute.default_value));
             }
             else if(workModeData.treatmode.current == HDF_post_dilute)
             {
                 lineEditVelocity.setText(QString::number(params->Params_Dialyser.OnlineRate.attribute.default_value));
             }
             else
             {
                 lineEditVelocity.setText(QString::number(params->Params_Dialyser.Rate.attribute.default_value));
             }
         }
         packet.Wspeed = lineEditVelocity.text().toFloat();
         packet.tempTarget = (lineEditTemp.text().toFloat()*10);  //精确到小数点后一位#/
         dataNodeUpdate(PACKET_NAME_DIALYSATE,&packet);
    }else if(name==PACKET_REBIRTH_HANDLE) //重生恢复操作处理#/
    {
        PacketFluid Fluid;
        formulaSet=G_P_REBIRTH->formulaSet;//配方参数表#/
        Fluid=G_P_REBIRTH->Fluid; //透析液流速和温度#/

        updateFaisonFormulaCond(&formulaSet);


        lineEditTemp.setText(QString::number((float)Fluid.tempTarget/10,'f',1));

        lineEditVelocity.setText(QString::number(Fluid.Wspeed));

//        printf("FLuid02==%d \n",G_P_REBIRTH->Fluid.Wspeed);

        dataNodeUpdate(PACKET_NAME_DIALYSATE,&Fluid);

    }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupDialysate-ActionHandle-out");
#endif
}



void SceneSetupDialysate::CustomSetText()
{
    labelConcentrationNa.setText(tr("Na+:"));
    labelConcentrationHCO3.setText(tr("HCO3-:"));
    labelVelocity.setText(tr("Dialysate Flow:"));
    labelTemp.setText(tr("Dialysate Temp:"));
    buttonSave.setText(tr("Confirm"));
    buttonCancel.setText(tr("Cancel"));
    buttonCurve.setText(tr("Profile"));
}

void SceneSetupDialysate::setCusVisible(bool vis, char* para)
{
    para = para;

    labelConcentrationNa.setVisible(vis);
    labelConcentrationNaUnit.setVisible(vis);
    labelConcentrationHCO3.setVisible(vis);
    labelConcentrationHCO3Unit.setVisible(vis);
    labelVelocity.setVisible(vis);
    labelVelocityUnit.setVisible(vis);
    labelTemp.setVisible(vis);
    labelTempUnit.setVisible(vis);
    lineEditConcentrationNa.setVisible(vis);
    lineEditConcentrationHCO3.setVisible(vis);
    lineEditVelocity.setVisible(vis);
    lineEditTemp.setVisible(vis);
    buttonSave.setVisible(false);
    buttonCancel.setVisible(false);
    buttonCurve.setVisible(vis);
}
