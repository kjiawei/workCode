#ifndef WIDGET_TIME_H
#define WIDGET_TIME_H

#include <QLabel>
#include <QTime>
#include <QTimer>
#include "config_ui.h"
#include "lang_switch.h"
#include "widget_button.h"
#include "data_custom.h"
#include "data_timer.h"

#define WIDGET_TIME_X               864
#define WIDGET_TIME_Y               380
#define WIDGETTIME_WIDTH            139
#define WIDGETTIME_HIGH             66

#define WIDGETTIME_LABEL_X          0
#define WIDGETTIME_LABEL_Y          0
#define WIDGETTIME_LABEL_HIGH       25

#define WIDGETTIME_BUTTON_X         4
#define WIDGETTIME_BUTTON_Y         (WIDGETTIME_LABEL_Y+\
                                        WIDGETTIME_LABEL_HIGH+3)
#define WIDGETTIME_BUTTON_WIDTH     132
#define WIDGETTIME_BUTTON_HIGH      36

#define WIDGETTIME_SHADOW_X         0
#define WIDGETTIME_SHADOW_Y         (WIDGETTIME_LABEL_Y+\
                                        WIDGETTIME_LABEL_HIGH)

class WidgetTime : public QWidget, public LangSetText, public timer_handle
,public ActionCustom
{
    Q_OBJECT

public:
    WidgetTime(QWidget *parent=0);
    bool setCountDown(const int msec=0);
    void CustomSetText();
    void timer_handler(const int timeOut = 0);
    void ActionHandle(const void* dataPtr=NULL, int name=0);
signals:
    void timeSignal(QString tm);
protected:
    void paintEvent(QPaintEvent *event);

private slots:
//    void timerDone();
    void buttonTimeSlot();

private:
    int secCount;
    QTime timeLeave;
    QTime timePoint;
//    QTimer timer;#/
    QLabel labelTime;
    QPixmap pixmapShadow;
    WidgetButton widgetButtonTime;

    int Timer_state;
    struct Params *params;
    int cursec;//总治疗时间#/
    PacketOwnNote ownNote;  //本机的提示信息#/
};

#endif
