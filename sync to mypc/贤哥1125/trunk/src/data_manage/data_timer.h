#ifndef DATA_TIMER_H
#define DATA_TIMER_H

#include <QtGui>

#include "data_set.h"

class timer_handle {
public :
    timer_handle() {}
    virtual void timer_handler(const int timeOut = 0) = 0;
};

class data_timer : public QObject
{
    Q_OBJECT
public:
    data_timer(void);
    ~data_timer();

    void createTimer(timer_handle* handler = NULL, int timeOut = 0);

    void stopTimer(timer_handle *handler = NULL);

    void closeTimer();
public slots:
    void timerCenter();

private:
    QTimer*  timer_center;
    QMap<timer_handle *, struct PacketTimer *> mapValue;
    QMap<timer_handle *, struct PacketTimer *> removeMapValue;
};



#endif // DATA_TIMER_H
