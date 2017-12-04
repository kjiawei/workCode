#include "deal.h"
#include "interactive.h"

struct CanNode canNode[CANNODE_NUM] = {
    {"Human Interface", 1},
    {"Master Control", 3},
    {"Monitor", 5},
    {"Test", 0xF},
    {"Power", 7},
    {"Hemo Pump", 9},
    {"Heparin Pump", 15},
    {"Hemomano Meter", 19},
    {"Hietlau Sensor", 21}
};


void DealMessage_CAN0F(Message *message)
{
    struct CAN0F *ptr = (struct CAN0F *)message->data;
    printf("N15_all_value:%d\n", ptr->N15_all_value);
    printf("N15_dose_speed:%d\n", ptr->N15_dose_speed);
    printf("N15_mode:%d\n", ptr->N15_mode);
    printf("N15_sub_mode:%d\n", ptr->N15_sub_mode);
}

int DealMessage()
{
    struct InterData interData;
    while (1) {
        if (InterData_Pop(&interData) != 0) {
            break;
        }
        switch (interData.message.cob_id & 0x7f) {
            case 0x3:
                break;
            case 0xF:
                DealMessage_CAN0F(&interData.message);
                break;
#if 0
            case canNode[CANNODE_MASTER_CONTROL].nodeId:
                break;
            case canNode[CANNODE_MASTER_MONITOR].nodeId:
                break;
            case canNode[CANNODE_TEST].nodeId:
                break;
            case canNode[CANNODE_POWER].nodeId:
                break;
            case canNode[CANNODE_HEMOPUMP].nodeId:
                break;
            case canNode[CANNODE_HEPARINPUMP].nodeId:
                break;
            case canNode[CANNODE_HEMOMANO].nodeId:
                break;
            case canNode[CANNODE_HIETLAU].nodeId:
                break;
#endif
            default:
                break;
        }
    }
    return 0;
}






/******************************************************************************
                    Function
 ******************************************************************************/

void HeparinPump_Start(void)
{
    WriteDeviceSdoEntry(canNode[CANNODE_TEST].nodeId, 0x2000, 0x04, 1, 0x01);
}


void HeparinPump_Stop(void)
{
    WriteDeviceSdoEntry(canNode[CANNODE_TEST].nodeId, 0x2000, 0x04, 1, 0x02);
}

void HeparinPump_Speed(float speed)
{
    if ((speed < 0) || (speed > 9.9)) {
        return;
    }

    UNS32 value;
    value = value<<16;
    WriteDeviceSdoEntry(canNode[CANNODE_TEST].nodeId, 0x2000, 0x03, 4, value);
}
