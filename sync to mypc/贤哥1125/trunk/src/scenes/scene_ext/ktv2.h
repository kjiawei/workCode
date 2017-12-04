#ifndef _KTV2_H
#define _KTV2_H

#include <QHash>
#include <QThread>
#include <QTimer>

///定义V的计算公式

#define V_MALE(year,height,weight)  (2.447-0.09516*(year)+0.1074*(height)+0.3362*(weight))
#define V_FEMALE(height,weight)     ((-2.097)+0.1069*(height)+0.2466*(weight))
#define KTV_REFRESH_INTEVAL         1 //定义Kt/V的默认刷新间隔为1分钟
#define CLEARANCE_RATE              0.97 //(194/200) 有效清除率，这里暂时只默认使用200ml透析器

#define MALE                        1
#define FEMALE                      2
#define SEX_UNKNOWN                 0

class Ktv2Thread :public QThread
{
    Q_OBJECT

public:
    Ktv2Thread(QObject *parent=0);

    void run();
    void Start() { if (!running) running = true; }
    void Stop() { running = false; Kt_Values.clear(); _kt = 0.0; current_ktv = 0.0; }

    void patient_info_save ( int sex, int years, int height, int weight)
    {
        patient_saved = true;
        patient_sex = sex;
        patient_years = years;
        patient_height = height;
        patient_weight = weight;
    }

    float ktv(){return current_ktv;}
    float ktv_up_to ( int time);
    bool is_running () { return running; }

private slots:
    void ktv_update();

private:
    volatile bool running;
    QHash<time_t, float> Kt_Values; //这里用QHash保存时间点和Kt的值，用于绘制Kt/V曲线
    QTimer *refresh_timer;          //默认按照KTV_REFRESH_INTEVAL分钟的时间间隔刷新
    int CurrentBloodFlowRate;       //当前血流速
    float _kt;                      //Kt垒加值
    int timer_interval;

    //病人身体值
    bool patient_saved;
    int patient_sex;
    int patient_years;              // years
    int patient_height;             // cm
    int patient_weight;             // kg

    float current_v;
    float current_ktv;
};

extern Ktv2Thread   ktv2_module_thread;

extern void KtV2_Start();
extern void KtV2_Stop();
extern void KtV2_Patient_Info_Save(int s, int y, int h, int w);
extern float KtV2_CurrentKtv();
extern float KtV2_KtVUpTo(int time);

#endif

