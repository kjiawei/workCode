#ifndef WIDGET_MEASURE_H
#define WIDGET_MEASURE_H

#include <QLabel>
#include "config_ui.h"
#include "data_custom.h"
#include "widget_button.h"
#include <QTimer>


#define  TREATMENT_IS_STOP 0    //治疗停止状态#/
#define  TREATMENT_IS_RUNNING 1 //治疗启动状态#/
#define  TREATMENT_IS_READY 2   //治疗准备状态#/
#define  TREATMENT_CHANGE_STATE 2

#define _WidgetMeasure_WIDTH                200
#define _WidgetMeasure_HIGH                 450

#define _widgetMeasure_Monial_Map_X         78
#define _widgetMeasure_Monial_Map_Y         158

#define WIDGETHEASURE_PUSHBUTTON_MARK_X     40
#define WIDGETHEASURE_PUSHBUTTON_MARK_Y     8

#define _widgetMeasure_Vertical_Available_Len       200

#define _widgetMeasure_Vertival_X1          (_widgetMeasure_Monial_Map_X+16)
#define _widgetMeasure_Vertival_Y1          (_widgetMeasure_Monial_Map_Y+6)
#define _widgetMeasure_Vertival_X2          _widgetMeasure_Vertival_X1
#define _widgetMeasure_Vertival_Y2          (_widgetMeasure_Vertival_Y1+ \
        _widgetMeasure_Vertical_Available_Len-3)

#define _widgetMeasure_CursorLimen_X        (_widgetMeasure_Monial_Map_X - 18)
#define _widgetMeasure_CursorLimen_Lower_Y  (_widgetMeasure_Monial_Map_Y+160)
#define _widgetMeasure_CursorLimen_Upper_Y  (_widgetMeasure_Monial_Map_Y+40)
#define _widgetMeasure_Cursor_X             (_widgetMeasure_Vertival_X1)

#define _widgetMeasure_Label_X              0
#define _widgetMeasure_Label_Y              (_widgetMeasure_Monial_Map_Y+190)

#define _widgetMeasure_PushButton_X         (_widgetMeasure_Monial_Map_X-4)
#define _widgetMeasure_PushButton_Rise_Y    82
#define _widgetMeasure_PushButton_Fall_Y    382
#define BUTONMARK_X 80

#define DELAY_TO_AUTO_SCALE 20000  //定时20秒

#define HJX_NEW_SWITCH 1    //HJX新实现开关


class WidgetMeasure : public QWidget
{
    Q_OBJECT
public:
    WidgetMeasure(QWidget *parent=0);

    bool setValue(const float value=0);
    bool setRangeTop(const float up=100);
    bool setRange(const float low=0, const float up=100,bool flag=true);
    bool setTestUnit(const QString text);
    bool setTextMark(const QString text);
    bool AlarmLock;
    float autoSetRange(void);
//private slots:
     //void ResetLock(int key);

private:
    inline void setUi();

//private slots:
     //void ResetLock(int key);

protected:
    void paintEvent(QPaintEvent *event);

private:
    float k;
    float lower;
    float upper;
    float currentValue;
    float saveNewValue;
    int currentValuePos;

    QPixmap pixmapVertical;
    QPixmap pixmapCursorLower;
    QPixmap pixmapCursorUpper;
    QPixmap pixmapCursorRed;
    QPixmap pixmapCursorGreen;

    QWidget widgetCursor;
    QWidget widgetCursorLower;
    QWidget widgetCursorUpper;

    QLabel labelLower;
    QLabel labelUpper;
    QLabel labelValue;
    QLabel labelUnit;
public:
    //WidgetButton pushButtonMark;
    WidgetButton pushButtonRise;
    WidgetButton pushButtonFall;
};

// AP  动脉压
class WidgetMeasureAP : public WidgetMeasure, public ActionCustom
{
    Q_OBJECT
public:
    WidgetMeasureAP(QWidget *parent=0);
    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void confirmSettingrangeAP(void);
private slots:
    void limitFall(void);
    void limitRise(void);
    void switchScene(void);
    void ResetLock(int key);
    void confirmAutoScale(void);
    void autoScale(int);
private:
    int step;
    QTimer  timer;

    struct PacketAP packetAP;
    PacketAP APData;
    struct AP_Scale_Range{
        float readySafeScaleUp;
        float readySafeScaleLow;
        float runSafeScaleUp;
        float runSafeScaleLow;
        float stopSafeScaleUp;
        float stopSafeScaleLow;
        float runAutoScaleUp;
        float runAutoScaleLow;
    }AP_scale_range;

    int APAutoScaleMAX;
    int APAutoScaleMIN;
    int APAutoScaleDefault;
    int treatmentStateAP; //记录是否处于治疗状态#/
    int ap;
};

// VP
class WidgetMeasureVP : public WidgetMeasure, public ActionCustom
{
    Q_OBJECT
public:
    WidgetMeasureVP(QWidget *parent=0);
    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void confirmSettingrangeVP(void);
private slots:
    void limitFall(void);
    void limitRise(void);
    void switchScene(void);
    void ResetLock(int key);
    void confirmAutoScale(void);
    void autoScale(int);
private:
    int VPAutoScaleMAX;
    int VPAutoScaleMIN;
    int VPAutoScaleDefault;
    int step;
    int treatmentStateVP; //记录是否处于治疗状态#/
    bool autolock_flag;  //自动卡标的状态#/
    QTimer timer;

    struct PacketVP packetVP;
     PacketVP VPData;
     struct VP_Scale_Range{
         float readySafeScaleUp;
         float readySafeScaleLow;
         float runSafeScaleUp;
         float runSafeScaleLow;
         float stopSafeScaleUp;
         float stopSafeScaleLow;
         float runAutoScaleUp;
         float runAutoScaleLow;
     }VP_scale_range;
};

// TMP
class WidgetMeasureTMP : public WidgetMeasure, public ActionCustom
{
    Q_OBJECT
public:
    WidgetMeasureTMP(QWidget *parent=0);
    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void confirmSettingrangeTMP(void);
private slots:
    void limitFall(void);
    void limitRise(void);
    void switchScene(void);
    void ResetLock(int key);
    void confirmAutoScale(void);
    void autoScale(int);
private:
    int TMPAutoScaleMAX;
    int TMPAutoScaleMIN;
    int TMPAutoScaleDefault;
    int step;
    int treatmentStateTMP; //记录是否处于治疗状态#/
    QTimer timer;
    struct PacketTMP packetTMP;
    PacketTMP TMPData;
    struct TMP_Scale_Range{
        float readySafeScaleUp;
        float readySafeScaleLow;
        float runSafeScaleUp;
        float runSafeScaleLow;
        float stopSafeScaleUp;
        float stopSafeScaleLow;
        float runAutoScaleUp;
        float runAutoScaleLow;
    }TMP_scale_range;
};

// Cond
class WidgetMeasureCond : public WidgetMeasure, public ActionCustom
{
    Q_OBJECT
public:
    WidgetMeasureCond(QWidget *parent=0);
    void ActionHandle(const void* dataPtr=NULL, int name=0);

private slots:
    void limitFall(void);
    void limitRise(void);
    void switchScene(void);
    //void ResetLock(int key);
private:
    float step;

    struct PacketCond packetCond;
    PacketCond CondData;
};

// btmakr
///*
//class WidgetPButtontMark : public WidgetMeasure, public ActionCustom
class WidgetPButtontMark :public QWidget//,public ActionCustom
{
    Q_OBJECT
public:
    WidgetPButtontMark(QWidget *parent=0);
    //void ActionHandle(const void* dataPtr=NULL, int name=0);
    bool setTextMark(const QString text);
    WidgetButton pushButtonMark;
    //WidgetButton pushButtonMarkVP;
private slots:
    //void switchScene(void);

private:

};

class WidgetPButtontMarkAP :public WidgetPButtontMark,public ActionCustom
{
    Q_OBJECT
public:
    WidgetPButtontMarkAP(QWidget *parent=0);
    void ActionHandle(const void* dataPtr=NULL, int name=0);
private slots:
    void switchScene(void);

private:


};

class WidgetPButtontMarkVP :public WidgetPButtontMark,public ActionCustom
{
    Q_OBJECT
public:
    WidgetPButtontMarkVP(QWidget *parent=0);
    void ActionHandle(const void* dataPtr=NULL, int name=0);
private slots:
    void switchScene(void);

private:


};

class WidgetPButtontMarkTMP :public WidgetPButtontMark,public ActionCustom
{
    Q_OBJECT
public:
    WidgetPButtontMarkTMP(QWidget *parent=0);
    void ActionHandle(const void* dataPtr=NULL, int name=0);
private slots:
    void switchScene(void);

private:


};

class WidgetPButtontMarkCond :public WidgetPButtontMark,public ActionCustom
{
    Q_OBJECT
public:
    WidgetPButtontMarkCond(QWidget *parent=0);
    void ActionHandle(const void* dataPtr=NULL, int name=0);
private slots:
    void switchScene(void);

private:


};
//*/
#endif
