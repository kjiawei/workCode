#include "config_ui.h"
#include "scene_setup_layout.h"
#include "scene_log_treatment.h"

inline void SceneLogTreatment::setUi()
{
    //title = tr("Treatment log");

    buttonSave.setGeometry(QRect(620, 500, 64, 39));
    buttonSave.setText(tr("Confirm"));
    buttonCancel.setGeometry(QRect(694, 500, 64, 39));
    buttonCancel.setText(tr("Cancel"));
    BUTTON_SIMPLE_STYLE(buttonSave);
    BUTTON_SIMPLE_STYLE(buttonCancel);

#ifndef MULIT_LINE
    query.setTabPosition(QTabWidget::West);
#endif

    query.setGeometry(200,100,500,400);
    query.resize(500,400);

#ifdef MULIT_LINE
    query.setStyleSheet(
                "QTabWidget::tab:!selected {margin-top: 0px;}"
                "QTabBar::tab {color: rgb(84,2,119); background-color: rgb(255,255,255); background-image: url(:/pics/wood.jpg);border: 2px solid rgb(68,66,64);border-bottom-color: rgb(68,66,64);border-top-left-radius: 20px;border-top-right-radius: 20px;max-height: 21px;min-width: 8ex;padding: 2px;}"
                "QTabBar::tab:selected {color: rgb(255,0,128); }"
                );
#else
    query.setStyleSheet(
                "QTabWidget::tab:!selected {margin-top: 0px;}"
                "QTabBar::tab {color: rgb(84,2,119);}"
                "QTabBar::tab:selected {color: rgb(255,0,128); }"
                );
#endif

    //query.setMovable(true);
    //query.setElideMode(Qt::ElideNone);
    //query.setTabPosition(QTabWidget::North);
    //query.setDocumentMode(true);

    //query.setUsesScrollButtons(true);

    query.addTab(&conductance,tr("  conductivity   "));
    query.addTab(&temperature,tr("   temperature  "));
    query.addTab(&DialysisFlux,tr("  DialysisFlux "));
    query.addTab(&ufr,tr("   UFR         "));
    query.addTab(&heparinFlux,tr("   HeparinFlux  "));
    query.addTab(&arterialPressure,tr("ArterialPressure"));
    query.addTab(&venousPressure,tr(" VenousPressure "));
    query.addTab(&transmembrane,tr(" Transmembrane  "));
    query.addTab(&bloodPressure,tr(" blood pressure "));
    query.addTab(&ktv,tr("      ktv       "));
    query.addTab(&alerting,tr("    alerting    "));
}

SceneLogTreatment::SceneLogTreatment(QWidget *parent) :
    QObject(parent),
    buttonSave(parent),
    buttonCancel(parent),
    query(parent),
    conductance(parent),
    temperature(parent),
    DialysisFlux(parent),
    ufr(parent),
    heparinFlux(parent),
    arterialPressure(parent),
    venousPressure(parent),
    transmembrane(parent),
    bloodPressure(parent),
    ktv(parent),
    alerting(parent)
{
    setUi();
    PageAppendPiece("LogTreat", this);
}

void SceneLogTreatment::setCusVisible(bool vis, char* para)
{
    para = para;
    buttonSave.setVisible(vis);
    buttonCancel.setVisible(vis);
    query.setVisible(vis);
}

TabConductance::TabConductance(QWidget *parent) : QWidget(parent),curve(this)
{
    curve.setGeometry(20,60,400,300);
    curve.setTestTime(60 * 60 * 4);//s
    curve.setMinValue(0);
    curve.setMaxValue(1000);
    curve.setValueZoom(10);
    curve.setUnit("ml");
    curve.setHorizontalScale(4);
    curve.setVerticalScale(4);
    curve.setMode(2);
    curve.delAllTargetCurve();
    curve.delAllTestPoint();
    curve.addTargetCurve(0,60 * 60 ,100);
    curve.addTargetCurve(60 * 60 * 1,60 * 60,200);
    curve.addTargetCurve(60 * 60 * 2,60 * 60,100);
    curve.addTargetCurve(60 * 60 * 3,60 * 60,200);
    curve.setUnitX(Hour);
    //curve.start();
}

TabTemperature::TabTemperature(QWidget *parent) : QWidget(parent),curve(this)
{
    curve.setGeometry(20,60,400,300);
    curve.setTestTime(60 * 60 * 4);//s
    curve.setMinValue(0);
    curve.setMaxValue(1000);
    curve.setValueZoom(10);
    curve.setUnit("ml");
    curve.setHorizontalScale(4);
    curve.setVerticalScale(4);
    curve.setMode(2);
    curve.delAllTargetCurve();
    curve.delAllTestPoint();
    curve.addTargetCurve(0,60 * 60 ,100);
    curve.addTargetCurve(60 * 60 * 1,60 * 60,200);
    curve.addTargetCurve(60 * 60 * 2,60 * 60,100);
    curve.addTargetCurve(60 * 60 * 3,60 * 60,200);
    curve.setUnitX(Hour);
    //curve.start();
}

TabDialysisFlux::TabDialysisFlux(QWidget *parent) : QWidget(parent),curve(this)
{
    curve.setGeometry(20,60,400,300);
    curve.setTestTime(60 * 60 * 4);//s
    curve.setMinValue(0);
    curve.setMaxValue(1000);
    curve.setValueZoom(10);
    curve.setUnit("ml");
    curve.setHorizontalScale(4);
    curve.setVerticalScale(4);
    curve.setMode(2);
    curve.delAllTargetCurve();
    curve.delAllTestPoint();
    curve.addTargetCurve(0,60 * 60 ,100);
    curve.addTargetCurve(60 * 60 * 1,60 * 60,200);
    curve.addTargetCurve(60 * 60 * 2,60 * 60,100);
    curve.addTargetCurve(60 * 60 * 3,60 * 60,200);
    curve.setUnitX(Hour);
    //curve.start();
}

TabUFR::TabUFR(QWidget * parent):QWidget(parent),curve(this)
{
    curve.setGeometry(20,60,400,300);
    curve.setTestTime(60 * 60 * 4);//s
    curve.setMinValue(0);
    curve.setMaxValue(1000);
    curve.setValueZoom(10);
    curve.setUnit("ml");
    curve.setHorizontalScale(4);
    curve.setVerticalScale(4);
    curve.setMode(2);
    curve.delAllTargetCurve();
    curve.delAllTestPoint();
    curve.addTargetCurve(0,60 * 60 ,100);
    curve.addTargetCurve(60 * 60 * 1,60 * 60,200);
    curve.addTargetCurve(60 * 60 * 2,60 * 60,100);
    curve.addTargetCurve(60 * 60 * 3,60 * 60,200);
    curve.setUnitX(Hour);
    //curve.start();
}

TabHeparinFlux::TabHeparinFlux(QWidget * parnet):QWidget(parnet),curve(this)
{
    curve.setGeometry(20,60,400,300);
    curve.setTestTime(60 * 60 * 4);//s
    curve.setMinValue(0);
    curve.setMaxValue(1000);
    curve.setValueZoom(10);
    curve.setUnit("ml");
    curve.setHorizontalScale(4);
    curve.setVerticalScale(4);
    curve.setMode(2);
    curve.delAllTargetCurve();
    curve.delAllTestPoint();
    curve.addTargetCurve(0,60 * 60 ,100);
    curve.addTargetCurve(60 * 60 * 1,60 * 60,200);
    curve.addTargetCurve(60 * 60 * 2,60 * 60,100);
    curve.addTargetCurve(60 * 60 * 3,60 * 60,200);
    curve.setUnitX(Hour);
    //curve.start();
}


TabArterialPressure::TabArterialPressure(QWidget * parent):QWidget(parent),curve(this)
{
    curve.setGeometry(20,60,400,300);
    curve.setTestTime(60 * 60 * 4);//s
    curve.setMinValue(0);
    curve.setMaxValue(1000);
    curve.setValueZoom(10);
    curve.setUnit("ml");
    curve.setHorizontalScale(4);
    curve.setVerticalScale(4);
    curve.setMode(2);
    curve.delAllTargetCurve();
    curve.delAllTestPoint();
    curve.addTargetCurve(0,60 * 60 ,100);
    curve.addTargetCurve(60 * 60 * 1,60 * 60,200);
    curve.addTargetCurve(60 * 60 * 2,60 * 60,100);
    curve.addTargetCurve(60 * 60 * 3,60 * 60,200);
    curve.setUnitX(Hour);
    //curve.start();
}

TabVenousPressure::TabVenousPressure(QWidget * parent):QWidget(parent),curve(this)
{
    curve.setGeometry(20,60,400,300);
    curve.setTestTime(60 * 60 * 4);//s
    curve.setMinValue(0);
    curve.setMaxValue(1000);
    curve.setValueZoom(10);
    curve.setUnit("ml");
    curve.setHorizontalScale(4);
    curve.setVerticalScale(4);
    curve.setMode(2);
    curve.delAllTargetCurve();
    curve.delAllTestPoint();
    curve.addTargetCurve(0,60 * 60 ,100);
    curve.addTargetCurve(60 * 60 * 1,60 * 60,200);
    curve.addTargetCurve(60 * 60 * 2,60 * 60,100);
    curve.addTargetCurve(60 * 60 * 3,60 * 60,200);
    curve.setUnitX(Hour);
    //curve.start();
}

TabTransmembrane::TabTransmembrane(QWidget * parent):QWidget(parent),curve(this)
{
    curve.setGeometry(20,60,400,300);
    curve.setTestTime(60 * 60 * 4);//s
    curve.setMinValue(0);
    curve.setMaxValue(1000);
    curve.setValueZoom(10);
    curve.setUnit("ml");
    curve.setHorizontalScale(4);
    curve.setVerticalScale(4);
    curve.setMode(2);
    curve.delAllTargetCurve();
    curve.delAllTestPoint();
    curve.addTargetCurve(0,60 * 60 ,100);
    curve.addTargetCurve(60 * 60 * 1,60 * 60,200);
    curve.addTargetCurve(60 * 60 * 2,60 * 60,100);
    curve.addTargetCurve(60 * 60 * 3,60 * 60,200);
    curve.setUnitX(Hour);
    //curve.start();
}

TabBloodPressure::TabBloodPressure(QWidget * parent):QWidget(parent),curve(this)
{
    curve.setGeometry(20,60,400,300);
    curve.setTestTime(60 * 60 * 4);//s
    curve.setMinValue(0);
    curve.setMaxValue(1000);
    curve.setValueZoom(10);
    curve.setUnit("ml");
    curve.setHorizontalScale(4);
    curve.setVerticalScale(4);
    curve.setMode(2);
    curve.delAllTargetCurve();
    curve.delAllTestPoint();
    curve.addTargetCurve(0,60 * 60 ,100);
    curve.addTargetCurve(60 * 60 * 1,60 * 60,200);
    curve.addTargetCurve(60 * 60 * 2,60 * 60,100);
    curve.addTargetCurve(60 * 60 * 3,60 * 60,200);
    curve.setUnitX(Hour);
    //curve.start();
}

TabKtv::TabKtv(QWidget *parent):QWidget(parent),curve(this)
{
    curve.setGeometry(20,60,400,300);
    curve.setTestTime(60 * 60 * 4);//s
    curve.setMinValue(0);
    curve.setMaxValue(1000);
    curve.setValueZoom(10);
    curve.setUnit("ml");
    curve.setHorizontalScale(4);
    curve.setVerticalScale(4);
    curve.setMode(2);
    curve.delAllTargetCurve();
    curve.delAllTestPoint();
    curve.addTargetCurve(0,60 * 60 ,100);
    curve.addTargetCurve(60 * 60 * 1,60 * 60,200);
    curve.addTargetCurve(60 * 60 * 2,60 * 60,100);
    curve.addTargetCurve(60 * 60 * 3,60 * 60,200);
    curve.setUnitX(Hour);
    //curve.start();
}

TabAlerting::TabAlerting(QWidget *parent):QWidget(parent),curve(this)//,lineEdit(this)
{
    curve.setGeometry(20,60,400,300);
    curve.setTestTime(60 * 60 * 4);//s
    curve.setMinValue(0);
    curve.setMaxValue(1000);
    curve.setValueZoom(10);
    curve.setUnit("ml");
    curve.setHorizontalScale(4);
    curve.setVerticalScale(4);
    curve.setMode(2);
    curve.delAllTargetCurve();
    curve.delAllTestPoint();
    curve.addTargetCurve(0,60 * 60 ,100);
    curve.addTargetCurve(60 * 60 * 1,60 * 60,200);
    curve.addTargetCurve(60 * 60 * 2,60 * 60,100);
    curve.addTargetCurve(60 * 60 * 3,60 * 60,200);
    curve.setUnitX(Hour);
    //curve.start();
}
