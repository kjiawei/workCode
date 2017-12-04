#ifndef SCENE_SETUP_HEMOMANOMETER_H
#define SCENE_SETUP_HEMOMANOMETER_H

#include <QtGui>
#include <QLabel>
#include "lang_switch.h"
#include "page_info.h"
#include "widget_bpmcurve.h"
#include "data_custom.h"
#include <QPainter>
#include <QWidget>

#include "keysetready.h"
#include "data_timer.h"
#include "widget_heartbeat.h"

class SceneSetupBPM : public QTableView, public PieceVisible, public LangSetText
,public ActionCustom, public KeySetReady,public timer_handle
{
    Q_OBJECT
public:
    SceneSetupBPM(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

    void lineEdit_init(){}
    void lineEdit_close(const char *name) {
        name = name;
    }
    void lineEdit_data_set(const char *name, const char *data) {
        name = name;
        data = data;
        save();
    }

    void save();

public slots:
    void bmpTest();
private:
    inline void setUi();
    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void timer_handler(const int timeOut = 0);

private slots:
    void button_Interval_clicked();
    void button_Quick_clicked();
    void button_Curve_clicked();
    void button_Show_clicked();
    void button_Cancel_clicked();
    void comboBoxBelt_changed();

private:

    QLabel labelShowTime;

    QLineEdit lineEditShowPulse;
    QLineEdit lineEditShowSysDia;

    QLabel labelHeartRate;
    QLabel labelHeartRateValue;

    QLabel labelBP;
    QLabel labelBPValue;
    QLabel labelBPUnit;

    QLabel labelPositions;
    QComboBox comboBoxPositions;

    QLabel labelBelt;
    QComboBox comboBoxBelt;

    QLabel labelBPTime;
    QTimeEdit timeEditBP;

    QLabel labelSysUpper;
    QLineEdit lineEditSysUpperValue;
    QLabel labelSysUpperUnit;

    QLabel labelSysLower;
    QLineEdit lineEditSysLowerValue;
    QLabel labelSysLowerUnit;

    QLabel labelDiaUpper;
    QLineEdit lineEditDiaUpperValue;
    QLabel labelDiaUpperUnit;

    QLabel labelDiaLower;
    QLineEdit lineEditDiaLowerValue;
    QLabel labelDiaLowerUnit;

    QLabel labelHRUpper;
    QLineEdit lineEditHRUpperValue;

    QLabel labelHRLower;
    QLineEdit lineEditHRLowerValue;

    QLabel labelPreselectPress;
    QLineEdit lineEditPreselectPress;
    QLabel labelPreselectPressUnit;

    QLabel labelHour;
    QLineEdit lineEditHour;
    QLabel labelMin;
    QLineEdit lineEditMin;

    WidgetBpmCurve *curveBPM;

    QPushButton buttonShow;
    QPushButton buttonCancel;
    QPushButton buttonInterval;  //自动测量#/
    QPushButton buttonQuick;  //手工测量#/
    QPushButton buttonCurve;  //曲线/表格#/

    QPushButton buttonsys;
    QPushButton buttondia;
    QPushButton buttonmap;
    QPushButton buttonplu;
    QLabel labelsys;
    QLabel labeldia;
    QLabel labelmap;
    QLabel labelplu;

    QTableWidget  bmpTable;

    PacketBPMState BPMState;
    PacketBPMMisc  BPMMisc;

    int colNum;
    int rowNum;
    int itemCount;
    char itemName[8];
    QString itemValue;
    bool isTable;
    bool isShow;
    bool Quickstate;
    bool IntervalState;
    char comboBoxBelt_flag;

    QTime bmptime;
    int recordCount;
    bool state;       //血压计的运行标志#/
    bool stateUpdate;//测量后，更新表格和曲线，主界面按钮#/
    bool miscUpdate; //测量完血压后，读取心率测量值的标志#/
    bool stopbmp;

   PacketBPMState packetBPMState;

   int secPass;
   QTime timePoint;
   bpmResult resultdata;

   QFrame LineOne;
   QFrame LineTwo;

};



#endif
