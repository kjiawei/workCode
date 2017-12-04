#ifndef STATE_DISINFECT_H
#define STATE_DISINFECT_H

#include "state.h"
#include "data_custom.h"
#include "data_timer.h"

class state_disinfect: public StateUnit, public ActionCustom,public timer_handle
{
public:
    state_disinfect(char *name=NULL);
    void ActionEnter(void);
    void ActionExit(void);
    void OtherHandle(void);

private:
    void Init();
    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void timer_handler(const int timeOut = 0);

private:
    PacketWorkMode *packetWorkMode;
    //WorkState workstate;
    int secCount; //消毒进行总计时#/
    int secpass;
    int suck_state;
    int model;
    unsigned char *N3_errorstate;
    QTime timePoint;
    PacketHeat pHeatTmp;
    bool iSHeatTmpok;//加热腔温度
    bool iSPreCondok;//消毒前要冲低电导
    bool iSCondok;//电导
    bool MasterStateOk;   //主控板状态正确标志#/
    char errorCount;      //发生错误计数器#/
    //bool iSPipeok;//AB管、供回液管状态

    PacketStatusNote *pNote;
    WorkState  cleanState;

    PacketMix packetMix;
    PacketOutlet packetOutlet;
    PacketFluidB packetFluidB;
};

//void DisinfectAddObject(void);

#endif // STATE_DISINFECT_H
