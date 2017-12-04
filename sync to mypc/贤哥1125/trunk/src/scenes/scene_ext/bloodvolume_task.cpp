#include <QDebug>
#include "bloodvolume_task.h"

BloodVolumeThread::BloodVolumeThread()
{
     //  start(QThread::LowPriority);
   QObject::connect(&timer,SIGNAL(timeout()),this,SLOT(bloodVolumeTimer()));
}

void BloodVolumeThread::run()
{

    timer.start(60000);
    bloodVolumeSendState = BLOODVOLUME_START;
    dataNodeUpdate(PACKET_NAME_BLOODVOLUME,&bloodVolumeSendState);
    threadState = true;
    while(threadState){
    }
    bloodVolumeSendState = BLOODVOLUME_STOP;
    dataNodeUpdate(PACKET_NAME_BLOODVOLUME,&bloodVolumeSendState);
    timer.stop();

}
void BloodVolumeThread::bloodVolumeTimer()
{
    bloodVolumeSendState = BLOODVOLUME_RUNING;
    dataNodeUpdate(PACKET_NAME_BLOODVOLUME,&bloodVolumeSendState);
}
void BloodVolumeThread::stop_BloodVolume()
{
    threadState = false;
}
