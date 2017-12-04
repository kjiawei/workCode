#include "data_timer.h"

#include <QDebug>

data_timer::data_timer(void)
{
    timer_center = new QTimer(this);

    connect(timer_center,SIGNAL(timeout()),this,SLOT(timerCenter()));

    mapValue.clear();
    removeMapValue.clear();
    timer_center->start(10);
}

data_timer::~data_timer()
{
    timer_center->stop();
}

void data_timer::createTimer(timer_handle *handler, int timeOut) //timeOut 为ms#/
{
    PacketTimer* pTimer = new PacketTimer;
    pTimer->timeOut = timeOut/10;
    pTimer->timeCount = 0;
    mapValue.insert(handler, pTimer);
    removeMapValue.insert(handler, pTimer);
//   qDebug() << "timeOut" << timeOut;
}

void data_timer::stopTimer(timer_handle *handler)
{
 //   PacketTimer* pTime=removeMapValue.value(handler);    //20151022 添加#/
//    delete pTime;     //20151022 添加#/
    this->removeMapValue.remove(handler);
}

void data_timer::closeTimer()
{
    timer_center->stop();
}

void data_timer::timerCenter()
{
    if(mapValue.size() > 0) {
        QMap<timer_handle *, PacketTimer *>::iterator it;
        for ( it = mapValue.begin(); it != mapValue.end(); ++it ){
            PacketTimer* pTimer = (PacketTimer *)it.value();
            if(pTimer == NULL || pTimer->timeOut == 0) {
                continue;
            }
            ++ pTimer->timeCount;

            if(pTimer->timeCount >= pTimer->timeOut) {

                pTimer->timeCount = 0;
                ((timer_handle*)it.key())->timer_handler(pTimer->timeOut);
            }
        }
    }

    if(removeMapValue.size() != mapValue.size()) {
//        qDeleteAll(mapValue);     //20151022 添加#/
        mapValue.clear();         //20151022 添加#/
        mapValue = removeMapValue;
    }
}

