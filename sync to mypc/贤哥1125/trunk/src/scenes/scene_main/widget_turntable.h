#ifndef WIDGET_TURNTABLE_H
#define WIDGET_TURNTABLE_H

#include <QLabel>
#include "widget_button.h"
#include "data_custom.h"
#include "data_timer.h"

#define WIDGET_TURNTABLE_X                  845
#define WIDGET_TURNTABLE_Y                  204
#define WIDGETTURNTABLE_WIDTH               171
#define WIDGETTURNTABLE_HIGH                168

#define WIDGETTURNTABLE_MAP_X               0
#define WIDGETTURNTABLE_MAP_Y               40

#define WIDGETTURNTABLE_LABEL_VALUE_X       40
#define WIDGETTURNTABLE_LABEL_VALUE_Y       0
#define WIDGETTURNTABLE_LABEL_VALUE_WIDTH   85
#define WIDGETTURNTABLE_LABEL_VALUE_HIGH    30

#define WIDGETTURNTABLE_LABEL_UNITS_X       125
#define WIDGETTURNTABLE_LABEL_UNITS_Y       0
#define WIDGETTURNTABLE_LABEL_UNITS_WIDTH   30
#define WIDGETTURNTABLE_LABEL_UNITS_HIGH    30

#define WIDGETTURNTABLE_NEEDLE_CENTRE_X     85
#define WIDGETTURNTABLE_NEEDLE_CENTRE_Y     130

#define WIDGETTURNTABLE_OVER_SPEED_X        (WIDGETTURNTABLE_NEEDLE_CENTRE_X-22+5)
#define WIDGETTURNTABLE_OVER_SPEED_Y        (WIDGETTURNTABLE_NEEDLE_CENTRE_Y-23)
#define WIDGETTURNTABLE_OVER_SPEED_WIDTH    45
#define WIDGETTURNTABLE_OVER_SPEED_HIGH     40

class WidgetTurntable : public QWidget,public ActionCustom, public timer_handle
{
    Q_OBJECT

public:
    WidgetTurntable(QWidget *parent = 0);

    void scaleSet(const int min=0,
		    const int max=100, const int degree=180);
    void valueSet(const int value=0);
    void setOverSpeed(const int value=0);

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void timer_handler(const int timeOut);

private:
    int needleMin;
    int needleMax;
    int needleDegree;
    int needleValue;
    int overSpeed;

    QColor colorNeedle;
    QLabel labelValue;
    QLabel labelUnits;
    QLabel labelOverSpeed;
    QPixmap pixmapNeedle;
    QPixmap pixmapCap;
    PacketUfValue *packetUfValue;
};

#endif
