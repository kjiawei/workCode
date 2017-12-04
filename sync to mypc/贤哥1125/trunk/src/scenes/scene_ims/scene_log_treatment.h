#ifndef SCENE_LOG_TREATMENT_H
#define SCENE_LOG_TREATMENT_H

#include <QtGui>
#include <QLabel>
#include "page_info.h"
#include "widget_curve.h"
#include "tabwidget.h"

class TabConductance: public QWidget
{
    Q_OBJECT

public:
    TabConductance(QWidget *parent = 0);
private:
    WidgetCurve curve;
};

class TabTemperature: public QWidget
{
    Q_OBJECT

public:
    TabTemperature(QWidget *parent = 0);
private:
    WidgetCurve curve;
};


class TabDialysisFlux :public QWidget
{
    Q_OBJECT

public:
    TabDialysisFlux(QWidget *parent = 0);
private:
    WidgetCurve curve;
};

class TabUFR:public QWidget
{
    Q_OBJECT

public:
    TabUFR(QWidget *parent = 0);
private:
    WidgetCurve curve;
};


class TabHeparinFlux: public QWidget
{
    Q_OBJECT

public:
    TabHeparinFlux(QWidget *parent = 0);
private:
    WidgetCurve curve;
};

class TabArterialPressure: public QWidget
{
    Q_OBJECT

public:
    TabArterialPressure(QWidget *parent = 0);
private:
    WidgetCurve curve;
};

class TabVenousPressure: public QWidget
{
    Q_OBJECT

public:
    TabVenousPressure(QWidget *parent = 0);
private:
   WidgetCurve curve;
};

class TabTransmembrane: public QWidget
{
    Q_OBJECT

public:
    TabTransmembrane(QWidget *parent = 0);
private:
    WidgetCurve curve;
};

class TabBloodPressure: public QWidget
{
    Q_OBJECT

public:
    TabBloodPressure(QWidget *parent = 0);
private:
    WidgetCurve curve;
};

class TabKtv: public QWidget
{
    Q_OBJECT

public:
    TabKtv(QWidget *parent = 0);
private:
    WidgetCurve curve;
};

class TabAlerting: public QWidget
{
    Q_OBJECT

public:
    TabAlerting(QWidget *parent = 0);
private:
    WidgetCurve curve;
    //QLineEdit lineEdit;
};

class SceneLogTreatment : public QObject, public PieceVisible
{
    Q_OBJECT
public:
    SceneLogTreatment(QWidget *parent=0);
    void setCusVisible(bool vis=false, char* para=NULL);

private:
    inline void setUi();

private:
    QPushButton buttonSave;
    QPushButton buttonCancel;

    TabWidget query;
    TabConductance conductance;
    TabTemperature temperature;
    TabDialysisFlux DialysisFlux;
    TabUFR ufr;
    TabHeparinFlux heparinFlux;
    TabArterialPressure arterialPressure;
    TabVenousPressure venousPressure;
    TabTransmembrane transmembrane;
    TabBloodPressure bloodPressure;
    TabKtv ktv;
    TabAlerting alerting;
};

#endif
