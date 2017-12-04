#ifndef BUTTONSIRQ_H
#define BUTTONSIRQ_H

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

#include <QObject>

class buttonsirq: public QObject
{
    Q_OBJECT
public:
    void ButtonsSignal(const int key) {
        emit BtnSignal(key);
    }
signals:
    void BtnSignal(const int key);

};

#endif

