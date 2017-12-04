#include "config_ui.h"
#include "scene_setup_layout.h"
#include "scene_setup_oximeter.h"
#include "middle.h"

inline void SceneSetupSpO2::setUi()
{
    SCENE_SETUP_CREATE_TABLE(labelSpO2, 1);
    SCENE_SETUP_CREATE_VALUE(labelSpO2Value, 1);
    SCENE_SETUP_CREATE_UNIT(labelSpO2Unit, 1);
    labelSpO2Value.setNum(0);
    labelSpO2Unit.setText("%");

    curveSpO2.setGeometry(QRect(160, 320, 600, 290));
    curveSpO2.setUnit("%");
    curveSpO2.setLineColor(Qt::green);
    curveSpO2.setValueZoom(1);
    curveSpO2.setHorizontalScale(4);//6
    curveSpO2.setVerticalScale(10);
    curveSpO2.delAllTargetCurve();
    curveSpO2.delAllTestPoint();
    curveSpO2.setMode(1);
    curveSpO2.setMinValue(0.0);
    curveSpO2.setMaxValue(100.0);
    curveSpO2.setTestTime(60*60*4);
    curveSpO2.setUnitX(Hour);
    curveSpO2.start();
    CustomSetText();
}
void SceneSetupSpO2::ActionHandle(const void* dataPtr,int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupSpO2-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name == 0)) {
        return;
    }

    if (name==PACKET_MONITOR_BLOODST) {
        struct PacketBloodState *packet = (struct PacketBloodState*)dataPtr;
        unsigned short int tmp=0;

        if(BloodState.N5_Oxygen !=packet->N5_Oxygen)
        {
            BloodState.N5_Oxygen =packet->N5_Oxygen;

            if(BloodState.N5_Oxygen&0x0080)
            {
                labelSpO2Value.setNum(0);
            }
            else
            {
                tmp=BloodState.N5_Oxygen&0x007f;
                labelSpO2Value.setNum(tmp);
            }

            //curveSpO2.addTestPoint(BloodState.N5_Oxygen);
            //curveSpO2.draw();
        }
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupSpO2-ActionHandle-out");
#endif
}

SceneSetupSpO2::SceneSetupSpO2(QWidget *parent) :
    QObject(parent),
    labelSpO2(parent),
    labelSpO2Value(parent),
    labelSpO2Unit(parent),
    curveSpO2(parent)
{
    setUi();
    PageAppendPiece("Setup SpO2", this);
    dataNodeAddObject(PACKET_MONITOR_BLOODST, this);

    connect(&oxtimer,SIGNAL(timeout()),this,SLOT(curveUpdate()));
    BloodState.N5_Oxygen=0;
    oxtimer.start(1000);
    timePoint = QTime::currentTime();
    secpass=0;
    curveSpO2.addTestPoint(0);
//    QTime time;
//    time= QTime::currentTime();
//    qsrand(time.msec()+time.second()*1000);
}


void SceneSetupSpO2::curveUpdate()
{
    static char flag=0;
    char databuf[][8]={
      {0x01, 0x01 ,0x13, 0x22, 0x01 ,0x00 ,0x00 ,0x30 },//将A管放到A桶
      {0x01, 0x01 ,0x13, 0x22, 0x00 ,0x00 ,0x00 ,0x30 },

    };


    //secpass=timePoint.elapsed()/1000;
     ///*
     if((secpass/60) ==1 && flag==0)
     {
         flag=1;
        #if 1
         Message mess;

//         mess.cob_id = 0x185;
//         mess.data[0] = 0x02;
//         mess.data[1] = 0x0;
//         mess.data[2] = 0x0;
//         mess.data[3] = 0x0;
//         mess.data[4] = 0x0;
//         mess.data[5] = 0x0;
//         mess.data[6] = 0b00011000;
//         mess.data[7] = 0;
         mess.cob_id = 0x183;
         memcpy(mess.data,databuf[0],sizeof(databuf[0]));
         mess.len = 8;
         MessageSignalMiddle(&mess);

         #endif
     }

     if((secpass/60) ==2 && flag==1)
     {
         flag=2;
        #if 0
         Message mess;
         mess.cob_id = 0x185;
         mess.data[0] = 0x02;
         mess.data[1] = 0x0;
         mess.data[2] = 0x0;
         mess.data[3] = 0x0;
         mess.data[4] = 0x0;
         mess.data[5] = 0x0;
         mess.data[6] = 0b00011001;
         mess.data[7] = 0;
         mess.len = 8;
         MessageSignalMiddle(&mess);
         #endif
     }
       //*/

     //BloodState.N5_Oxygen=qrand()%100;
    if( BloodState.N5_Oxygen<=100)
     curveSpO2.addTestPoint(BloodState.N5_Oxygen);
     curveSpO2.draw();

}

void SceneSetupSpO2::CustomSetText()
{
    labelSpO2.setText(tr("SpO2 :"));
}

void SceneSetupSpO2::setCusVisible(bool vis, char* para)
{
    para = para;
    labelSpO2.setVisible(vis);
    labelSpO2Value.setVisible(vis);
    labelSpO2Unit.setVisible(vis);
    curveSpO2.setVisible(vis);
}

