#ifndef SCENE_SETUP_PRESSURE_H
#define SCENE_SETUP_PRESSURE_H

#include <QtGui>
#include <QLabel>
#include "lang_switch.h"
#include "data_custom.h"
#include "page_info.h"
#include "widget_curve.h"
#include "keysetready.h"
class SceneSetupPressure : public QObject, public PieceVisible, public ActionCustom, public LangSetText, public KeySetReady
{
    Q_OBJECT
public:
    SceneSetupPressure(QWidget *parent=0);
    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

private:
    inline void setUi();

private slots:
    void buttonGroupClicked(int id=0);
    void SlotParameterSave();
    void lineEdit_init(){}
    void lineEdit_close(const char *name){name = name;}
    void lineEdit_data_set(const char *name, const char *data){
            name = name;
            data = data;
            SlotParameterSave();
    }

private:
    QLabel labelAP;
    QLabel labelAPHigh;
    QLabel labelAPLow;
    QLineEdit lineEditAP;
    QLineEdit lineEditAPHigh;
    QLineEdit lineEditAPLow;
    QLabel labelAPUnit;
    QLabel labelAPHighUnit;
    QLabel labelAPLowUnit;

    QLabel labelVP;
    QLabel labelVPHigh;
    QLabel labelVPLow;
    QLineEdit lineEditVP;
    QLineEdit lineEditVPHigh;
    QLineEdit lineEditVPLow;
    QLabel labelVPUnit;
    QLabel labelVPHighUnit;
    QLabel labelVPLowUnit;

    QLabel labelTMP;
    QLabel labelTMPHigh;
    QLabel labelTMPLow;
    QLineEdit lineEditTMP;
    QLineEdit lineEditTMPHigh;
    QLineEdit lineEditTMPLow;
    QLabel labelTMPUnit;
    QLabel labelTMPHighUnit;
    QLabel labelTMPLowUnit;

    QButtonGroup buttonGroup;
    QRadioButton radioButtonAuto;
    QRadioButton radioButtonManual;

    WidgetCurve curvePressure;
    QPushButton buttonSave;
    QPushButton buttonCancel;

private:
    QString PressureUnit;

    struct PacketAP packetAP;
    struct PacketVP packetVP;
    struct PacketTMP packetTMP;

private:
    struct PacketAP defaultAP;
    struct PacketVP defaultVP;
    struct PacketTMP defaultTMP;
    int scaleVar;

private:
    QTimer * curveTimer;
};

#endif
