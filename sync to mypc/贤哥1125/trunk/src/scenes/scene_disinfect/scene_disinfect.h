#ifndef SCENE_DISINFECT_H
#define SCENE_DISINFECT_H

#include <QtGui>
#include <QWidget>
#include <QMainWindow>
#include <QMouseEvent>

#include "config_ui.h"
#include "lang_switch.h"
#include "page_info.h"
#include "widget_tempo.h"
#include "widget_button.h"
#include "widget_radio_button.h"
#include "widget_temperature.h"

#include "packet.h"
#include "data_timer.h"
#include "data_custom.h"

#define WIDGET_BUTTON_DISINFECT_RUN_X       14
#define WIDGET_BUTTON_DISINFECT_RUN_Y       684
#define WIDGET_BUTTON_DISINFECT_RUN_WIDTH   272
#define WIDGET_BUTTON_DISINFECT_RUN_HIGH    68

#define WIDGET_DISINFECT_TITLE_X            10
#define WIDGET_DISINFECT_TITLE_Y            13
#define WIDGET_DISINFECT_TITLE_WIDTH        856
#define WIDGET_DISINFECT_TITLE_HIGH         45

#define WIDGET_BUTTON_DISINFECT_RETURN_X        (WIDGET_BUTTON_DISINFECT_RUN_X+\
        WIDGET_BUTTON_DISINFECT_RUN_WIDTH)
#define WIDGET_BUTTON_DISINFECT_RETURN_Y       684
#define WIDGET_BUTTON_DISINFECT_RETURN_WIDTH   270
#define WIDGET_BUTTON_DISINFECT_RETURN_HIGH    68

#define WIDGET_BUTTON_DISINFECT_SHUTDOWN_X        (WIDGET_BUTTON_DISINFECT_RETURN_X+\
        WIDGET_BUTTON_DISINFECT_RETURN_WIDTH)
#define WIDGET_BUTTON_DISINFECT_SHUTDOWN_Y       684
#define WIDGET_BUTTON_DISINFECT_SHUTDOWN_WIDTH   271
#define WIDGET_BUTTON_DISINFECT_SHUTDOWN_HIGH    68

struct DisinfcetInfo
{
    int time;
    int type;
};

struct FilterTimes
{
    unsigned int id;
    unsigned int usedableTime;
    unsigned int usedTime;
    unsigned int remainTime;

};

bool DisinfectFinishLastTime(void);
class SceneDisinfect : public QObject, public PieceVisible, public LangSetText
        ,public timer_handle,public ActionCustom
{
    Q_OBJECT
public:
    SceneDisinfect(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);
    ~SceneDisinfect();
public slots:
    void clickEvent(int key);
    void updateFilterTimes(FilterTimes ft1,FilterTimes ft2);
private:
    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void Run();
    void Stop();
    void timer_handler(const int timeOut = 0);
    void ShowLastRecord();
    void insertRecord( DisinfectWorkModel model);
    void ShowFilterTimes();
    void ShowFilterPara(FilterTimes filter1Info,FilterTimes filter2Info);
private slots:
    void buttonShutdown();
    void buttonRun();
    void buttonReturn();
    void on_FilterChange_clicked(void);
    void ConfirmStop(int sel);
    void ConfirmShotDown(int sel);
    void ConfirmDisinfectTyp(int sel);
    void ResetFilter(bool sel);
    void DryFilter(bool sel);
    void ChangeFilterCancel(int sel);
    void ConfirmResetFilter(int sel);
private:
    QLabel labelTitle;
    QLabel labelNote;
    QLabel labelNote2;
    QLabel labelCountDown;  //定时关机时间#/
    WidgetTempo widgetTempo;    //消毒转盘图#/
    WIDGET_TEMPERATURE widgetTemperature;  //消毒温度计#/
    QFrame frameLine;
    WidgetButton widgetButtonRun;
    WidgetButton widgetButtonReturn;
    WidgetButton widgetButtonShutoff;
    WidgetDisinfectRadioButton widgetDisinfectRadioButton;//消毒页面左边的选择按钮#/
    QLabel topLevelLabel;
    QLineEdit lineEditAutoOff;
    QLineEdit lineEditRemainingTime;
    QTextEdit textEditLastPrograms;
    QLabel    LastProgramsLabelTitle;
    QLabel    LastProgramsLabelT;
    QLabel    LastProgramsLabel1;
    QLabel    LastProgramsLabel2;
    QLabel    LastProgramsLabel1_type;
    QLabel    LastProgramsLabel2_type;

    QTextEdit textEditFilterChange;
    QLabel    FilterLabelTitle;
    QLabel    FilterNameLabel;
    QLabel    FilterAllLabel;
    QLabel    FilterUsedLabel;
    QLabel    FilterRemainLabel;
    QLabel    AllLTimesLabel;
    QLabel    UsedTimesLabel;
    QLabel    RemainTimesLabel;

    QPushButton   buttonFilterChange1;
    QPushButton   buttonFilterChange2;
    int FilterID;
    QPushButton   buttonMaskPageSwitch;//该按钮只用于进入消毒模式后屏蔽相关的按钮
    bool buttonRunState;
    int timeCount;
    //WorkState workstate;
    enum DisinfectWorkModel chooseDisinfectWorkModel;
    Params * params;
    int N3_errorstate;
    QTime timePoint;
    int secCount;
    int secpass;
    WorkState  stateTemp;
    bool powerShutdown;
    bool setCountDown;
    int ShutOffDelay;
    DisinfectWorkModel model;
};

#endif
