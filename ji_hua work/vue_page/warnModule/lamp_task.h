#ifndef LAMPTASK_H
#define LAMPTASK_H

#include <QtGui>

#ifdef OS_LINUX

#include <QWSServer>
#include <QApplication>
#include <QWidget>
#include <QPixmap>
#include <QBitmap>
#include <QPoint>
#include <QPalette>
#include <QMouseEvent>
#include <QWSServer>
#include <stdlib.h>
#include <sys/time.h>

#endif

#include <errno.h>

#ifdef OS_LINUX

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#ifdef OS_LINUX

#include <linux/soundcard.h>

#endif

class lampTask: public QThread
{
    Q_OBJECT
public:
    lampTask(void);
    ~lampTask();
    void flash(int model,int interval = 1000);
    void stop(int model);
    void ledoff();  //关闭LED
    void ledon(); //开启LED

public slots:

protected:
    void run();
    bool isAlarm;
    bool isLampOn;
    int lampState;//1红 2黄 3绿
    QSemaphore lamp_sem;

private:
    int delayMs;
};

#endif
