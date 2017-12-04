#ifndef WIDGET_HEARTBEAT_H
#define WIDGET_HEARTBEAT_H

#include <QLabel>
#include "config_ui.h"
#include "widget_button.h"
#include "data_custom.h"

#define WIDGET_HEARTBEAT_X                  864
#define WIDGET_HEARTBEAT_Y                  459
#define WIDGETHEARTBEAT_WIDTH               139
#define WIDGETHEARTBEAT_HIGH                66

#define WIDGET_HEARTBEAT_HEARTBEAT_X        3
#define WIDGET_HEARTBEAT_HEARTBEAT_Y        4
#define WIDGET_HEARTBEAT_HEARTBEAT_WIDTH    132
#define WIDGET_HEARTBEAT_HEARTBEAT_HIGH     36

#define WIDGET_HEARTBEAT_SHADOW_X           0
#define WIDGET_HEARTBEAT_SHADOW_Y           0


class WidgetHeartbeat : public QWidget,public ActionCustom
{
    Q_OBJECT
public:
    WidgetHeartbeat(QWidget *parent=0);
    void ActionHandle(const void* dataPtr=NULL, int name=0);
signals:
    void bmpTest();
private slots:
    void SignalPageSwitch();
    void showResult(bpmResult bpm);
protected:
    void paintEvent(QPaintEvent *event);

private:
    //void showResult(void);
    QPixmap pixmapShadow;
    WidgetButton widgetButtonHeartbeat;
    QLabel labelTime;
    QLabel labelResult;
    QLabel labelPResult;
};

#endif
