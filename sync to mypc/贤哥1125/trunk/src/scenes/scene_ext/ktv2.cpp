#include <cstdio>
#include <ctime>
#include "ktv2.h"
#include "data_custom.h"

Ktv2Thread::Ktv2Thread(QObject* parent) :
    QThread(parent)
{
    _kt = 0.0;
    running = false;
    patient_saved = false;
    refresh_timer = new QTimer();
    timer_interval = KTV_REFRESH_INTEVAL*1000*60; //毫秒
    refresh_timer->setInterval(timer_interval);
    connect(refresh_timer, SIGNAL(timeout()),this, SLOT(ktv_update()));
}

void Ktv2Thread::run()
{
    running = true;
    refresh_timer->start();
}

void Ktv2Thread::ktv_update()
{
    if (!running)
        return;

    PacketSdoBlood sdoBlood;
    time_t t;

    time(&t);

    dataRead(PACKET_SDO_BLOOD,&sdoBlood);
    this->CurrentBloodFlowRate = sdoBlood.N3_set_BPspeed;

    if (this->CurrentBloodFlowRate < 200) //200ml是透析器标定默认流量，跟透析器有关，此处暂时固定
        _kt = _kt + (CLEARANCE_RATE * this->CurrentBloodFlowRate) * timer_interval  / (1000 * 1000 * 60 );
    else
        _kt = _kt + (CLEARANCE_RATE * ( 200 + 0.6 * (this->CurrentBloodFlowRate - 200))) * timer_interval  / (1000 * 1000 * 60 );

    Kt_Values.insert(t, _kt);

    if (patient_saved)
    {
        if (patient_sex == MALE)
            current_v = V_MALE(patient_years,patient_height, patient_weight);
        else if (patient_sex == FEMALE)
            current_v = V_FEMALE(patient_height, patient_weight);

        current_ktv = _kt / current_v;
    }

#if 0 // for debug
    if (patient_saved)
        printf("patient information: %d years old: %d height: %d cm weight: %d kg\n",
               patient_sex, patient_years,patient_height, patient_weight);

    printf("blood flowrate: %d kt: %f v: %f Kt/V: %f\n", this->CurrentBloodFlowRate, _kt, current_v, current_ktv);
#endif
}

float Ktv2Thread::ktv_up_to ( int time) //time为分钟
{
    if (!running)
        return 0.0;

    if ( current_v == 0.0)
        return 0.0;

    float _kt;

    PacketSdoBlood sdoBlood;

    dataRead(PACKET_SDO_BLOOD,&sdoBlood);
    this->CurrentBloodFlowRate = sdoBlood.N3_set_BPspeed;

    if (this->CurrentBloodFlowRate < 200) //200ml是透析器标定默认流量，跟透析器有关，此处暂时固定
        _kt = CLEARANCE_RATE * this->CurrentBloodFlowRate * time /1000;
    else
        _kt = CLEARANCE_RATE * ( 200 + 0.6 * (this->CurrentBloodFlowRate - 200)) * time  / 1000;

    return (_kt / current_v);
}

Ktv2Thread ktv2_module_thread;
volatile int init=0;

void KtV2_Start()
{
    if (init==0)
    {
        ktv2_module_thread.start();
        init = 1;
    }
    ktv2_module_thread.Start();
}

void KtV2_Stop()
{
    ktv2_module_thread.Stop();
}

void KtV2_Patient_Info_Save(int s, int y, int h, int w)
{
    if (ktv2_module_thread.is_running())
    {
        printf("-->patient_info_save!\n");
        ktv2_module_thread.patient_info_save(s, y, h, w);
    }
}

float KtV2_CurrentKtv()
{
    return ktv2_module_thread.ktv();
}

float KtV2_KtVUpTo(int time)
{
    return ktv2_module_thread.ktv_up_to(time);
}
