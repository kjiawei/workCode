#ifndef DEAL_H
#define DEAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "canfestival.h"

enum CanNodeEnum {
    CANNODE_MASTER,
    CANNODE_MASTER_CONTROL,
    CANNODE_MASTER_MONITOR,
    CANNODE_TEST,
    CANNODE_POWER,
    CANNODE_HEMOPUMP,
    CANNODE_HEPARINPUMP,
    CANNODE_HEMOMANO,
    CANNODE_HIETLAU,
    CANNODE_NUM
};

struct CanNode {
    char name[20];
    unsigned char nodeId;
    /* ... */
};

struct CAN0F{
    UNS16 N15_all_value;
    UNS16 N15_dose_speed;
    UNS8  N15_mode;
    UNS8  N15_sub_mode;
    UNS16 N15_NULL;
};

int DealMessage();

void HeparinPump_Start(void);
void HeparinPump_Stop(void);
void HeparinPump_Speed(float speed);

#ifdef __cplusplus
};
#endif


#endif
