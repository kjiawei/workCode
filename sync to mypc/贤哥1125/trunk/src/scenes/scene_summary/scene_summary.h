#ifndef SCENE_SUMMARY_H
#define SCENE_SUMMARY_H

#include <QtGui>
#include <QLabel>
#include "lang_switch.h"
#include "page_info.h"
#include "widget_curve.h"
#include "version.h"
#include "data_custom.h"
#include "data_set.h"

class SceneSummary : public QObject, public PieceVisible, public LangSetText
,public ActionCustom
{
    Q_OBJECT
public:
    SceneSummary(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);
    void ActionHandle(const void* dataPtr=NULL, int name=0);
public slots:
    void timeRemain(QString tm);
private:
    inline void setUi();

private:
    QLabel labelCurveType;
    QLabel labelCurveValue;  //治疗类型#/

    QLabel labelDilutionType;
    QLabel labelDilutionValue;  //稀释类型#/

    QLabel labelTimeLeft;
    QLabel labelTimeLeftValue;  //治疗剩余时间#/

    QLabel labelUfRate;
    QLabel labelUfRateValue;  //超滤率#/
    QLabel labelUfRateUnit;

    QLabel labelUfGoal;
    QLabel labelUfGoalValue; //超滤目标#/
    QLabel labelUfGoalUnit;

    QLabel labelUfVolume;
    QLabel labelUfVolumeValue; //已超滤量#/
    QLabel labelUfVolumeUnit;

    QLabel labelAP;
    QLabel labelAPValue;  //动脉压#/
    QLabel labelAPUnit;

    QLabel labelVP;
    QLabel labelVPValue; //静脉压#/
    QLabel labelVPUnit;

    QLabel labelTMP;
    QLabel labelTMPValue;  //跨膜压#/
    QLabel labelTMPUnit;

    QLabel labelNa;
    QLabel labelNaValue;//NA离子#/
    QLabel labelNaUnit;

    QLabel labelCond;
    QLabel labelCondValue;  //出水口电导#/
    QLabel labelCondUnit;

    QLabel labelTemp;
    QLabel labelTempValue;//透析夜温度#/
    QLabel labelTempUnit;

    QLabel labelHeparinFlux;
    QLabel labelHeparinFluxValue;//肝素泵流速#/
    QLabel labelHeparinFluxUnit;

    QLabel labelHeparinVol;
    QLabel labelHeparinVolValue;//肝素已注入量#/
    QLabel labelHeparinVolUnit;

    QLabel labelBPSpeed;
    QLabel labelBPSpeedValue;  //血泵速度#/
    QLabel labelBPSpeedUnit;
    //QLabel labelBPTotal;

    QLabel labelSubsTotal;
    QLabel labelSubsTotalValue;  //SUB泵目标量#/
    QLabel labelSubsTotalUnit;

    QLabel labelSubsAcc;
    QLabel labelSubsAccValue;  //SUB泵累计量#/
    QLabel labelSubsAccUnit;

    QLabel labelDialysisFlus;
    QLabel labelDialysisFlusValue;  //透析夜流速#/
    QLabel labelDialysisFlusUnit;

    QLabel labelHCO3;
    QLabel labelHCO3Value;  //HCO3浓度#/
    QLabel labelHCO3Unit;

    QLabel labelBloodTEMP;
    QLabel labelBloodTEMPValue;   //血温#/
    QLabel labelBloodTEMPUnit;

    QLabel labelCondTitle;
    QLabel labelCondText;   //Na曲线运行信息#/
    WidgetCurve curveCond;  //电导曲线#/
    QLabel labelUFTitle;
    QLabel labelUFText;     //UF曲线运行信息#/
    WidgetCurve curveUF;    //超虑曲线

    QLabel labelTitle;
    QLabel rebirthinfo;

    PacketUF packetUF;
    PacketAP packetAP;
    PacketVP packetVP;
    PacketTMP packetTMP;

    FormulaSet formulaSet;
    PacketFluid packetFluid;
    PacketSdoBlood packetSdoBlood;
    PacketPDOHeparinPump packetPDOHeparinPump;
    PacketHeparinPump packetHeparinPump;
};

#endif
