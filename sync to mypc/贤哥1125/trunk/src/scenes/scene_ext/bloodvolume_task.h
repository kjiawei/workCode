
#ifndef BLOODVOLUME_TASK_H
#define BLOODVOLUME_TASK_H

#include <QThread>
#include <QTimer>
#include "data_custom.h"


#define FULLB_LOOD_PARA  0.077
#define BEFORE_THIRTY_MIN_PUR  0.4
#define AFTER_THIRTY_MIN_PUR  0.748
#define INTERBAL_TIME  1
#define THIRTY_MIN  30
enum BloodVolumeState{
    BLOODVOLUME_START,
    BLOODVOLUME_STOP,
    BLOODVOLUME_RUNING
};

class BloodVolumeThread : public QThread
{
    Q_OBJECT

public:
    BloodVolumeThread();
    void stop_BloodVolume();
    int bloodVolumeSendState;
private:
    QTimer timer;
    volatile bool threadState;
protected:
    void run();
private slots:
    void bloodVolumeTimer();
};

#endif

