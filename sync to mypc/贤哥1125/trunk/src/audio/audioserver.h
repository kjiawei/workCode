#ifndef AUDIOSERVER_H
#define AUDIOSERVER_H

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

/*!
 * \brief The audioserver class 报警音的音频播放#/
 */
typedef struct
{
   FILE * wave;
   int size;
}SData;


class audioserver: public QThread
{
    Q_OBJECT
public:
    audioserver(void);
    ~audioserver();
    void alarmSilence(bool tag=false);
    void playAlarm(QString file,int msDelay);
    void playNotice(QString file,int msDelay);
    void alarmStop();
    void alarmStart();
    int  alarmState();
    bool alarmSilenceState();

public slots:
    void beep();

protected:
    void run();
    QString beep_filename;
    QString alarm_filename;  //静音 true：静音
    QString notice_filename;
    QString currentfile;
    bool alarmBit;   //报警音开关 true：开 false 关
    bool noticeBit;
    FILE * fwave;
    int playsize;  //存放音频播放的数据长度#/
    int savewrited;
    int flag;
    int saveflag;
    QSemaphore audio_sem;

private:
    int initDSP(QString filename);
    int msDelay;
    bool AlarmSilent;
};

#endif // AUDIOSERVER_H

