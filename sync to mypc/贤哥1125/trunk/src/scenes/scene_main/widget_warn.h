#ifndef WIDGET_WARN_H
#define WIDGET_WARN_H

#include "config_ui.h"
#include "widget_button.h"

#include "data_timer.h"

#include "data_custom.h"

#define WIDGET_WARN_X                   874
#define WIDGET_WARN_Y                   13
#define WIDGETWARN_WIDTH                139
#define WIDGETWARN_HIGH                 82

#define WIDGETWARN_BUTTON_X             2
#define WIDGETWARN_BUTTON_Y             1
#define WIDGETWARN_BUTTON_WIDTH         132
#define WIDGETWARN_BUTTON_HIGH          34

#define WIDGET_WARN_SHADOW_X            -1
#define WIDGET_WARN_SHADOW_Y            -1

class WidgetWarn : public QWidget, public timer_handle, public ActionCustom
{
    Q_OBJECT

public:
    WidgetWarn(QWidget *parent=0);
    void setWarnText(const QString& text);
    void timer_handler(const int timeOut = 0);
    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void setAlarmColor(unsigned int type);
private slots:
    void SignalPageSwitch();

protected:
    void paintEvent(QPaintEvent *event);

private:
    QPixmap pixmapShadow;
    QPixmap pixmapCommon;
    QPixmap pixmapMessage;
    QPixmap pixmapWarn;
    QPixmap pixmapAlarm;
    QPixmap pixmapCurrent;
    WidgetButton widgetButtonWarn;

    bool isFlashing;
    bool isFlashShow;

    int flashType;
    PacketStatusNote pnote;
    PacketStatusAlarm palarm;
    int alamr_prepro;
    int alamr_curpro;


};

#endif
