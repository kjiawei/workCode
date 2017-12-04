#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <QtGui>
#include <QWidget>
#include <QPainter>

#include "data_timer.h"

class WidgetClock: public QWidget, public timer_handle
{
    Q_OBJECT
public:
    WidgetClock(QWidget* = 0);
    void timer_handler(const int timeOut = 0);
protected:
    void paintEvent(QPaintEvent *event);

private:
};


#endif // _CLOCK_H_
