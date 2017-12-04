#include "config_ui.h"
#include "widget_clock.h"

#include "global_var.h"

WidgetClock::WidgetClock(QWidget* parent)
    :QWidget(parent)
{
    setParent(parent);

    timer->createTimer(this, 1000);
//    QTimer *timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
//    timer->start(1000);    
    G_SYS_CURTIME=QDateTime().currentDateTimeUtc().toTime_t(); //时间戳#/

    setAutoFillBackground(false);
    setFixedSize(110,48);
}

void WidgetClock::timer_handler(const int timeOut)
{
#if DEBUG_TIMER_HANDLE
    Debug_insert("WidgetClock-timer-in");
#endif
    Q_UNUSED(timeOut);

    this->update();
}

void WidgetClock::paintEvent(QPaintEvent*)
{
    static bool flag=true;
    QPainter painter(this);
    QTime time = QTime::currentTime();
    QDate date = QDate::currentDate();
    G_SYS_CURTIME=QDateTime().currentDateTimeUtc().toTime_t(); //时间戳#/
    painter.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_FIRST_3, CONFIG_FONT_NORMAL));
 //   painter.drawText(-5, 10, 120, 15, Qt::AlignCenter, time.toString(Qt::TextDate));

    if(flag)
        painter.drawText(-5, 10, 120, 15, Qt::AlignCenter, time.toString(QString("hh:mm")));
    else
        painter.drawText(-5, 10, 120, 15, Qt::AlignCenter, time.toString(QString("hh mm")));
    flag=!flag;
    painter.drawText(-5, 30, 120, 15, Qt::AlignCenter, date.toString(Qt::ISODate));
}

