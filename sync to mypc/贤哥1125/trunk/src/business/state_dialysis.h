#ifndef STATE_DIALYSIS_H
#define STATE_DIALYSIS_H

#include "state.h"
#include "data_custom.h"
#include "data_timer.h"

class StateDialysis : public StateUnit, public ActionCustom,
 public timer_handle//,public QObject

{
public:
    StateDialysis(char *name=NULL);

    void ActionEnter(void);
    void ActionExit(void);

private:
    void Init();
    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void AdjPresslimit();
    void timer_handler(const int timeOut = 0);

private:

    WorkState workstate;
    PacketAP packetAP;
    PacketVP packetVP;
    PacketTMP packetTMP;
    int apspan;
    int vpspan;
    int tmpspan;
    int secCount;
    int target_apPiont;
    int target_vpPiont;
    int target_tmpPiont;
    int apBuff[100];
    int vpBuff[100];
    int tmpBuff[100];
    int sample_count;
    bool Adjlimit;
};

#endif // STATE_DIALYSIS_H
