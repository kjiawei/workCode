#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include "deal.h"
#include "CANOpenShell.h"

extern struct CanNode canNode[];

int main(int argc, char** argv)
{
    argc = argc;
    argv = argv;


    CanOpen_Master_Init();

    while (CanNode_Start(canNode[CANNODE_TEST].nodeId) != 0) ;

    while (CanOpen_GetState(canNode[CANNODE_TEST].nodeId) != Operational)
        DealMessage();

    ReadHeartBeat(canNode[CANNODE_TEST].nodeId);

    while (ReadDeviceSdoIsOk() != 0)
        DealMessage();
    //usleep(4000);
    HeparinPump_Start();
    while (ReadDeviceSdoIsOk() != 0)
        DealMessage();
    HeparinPump_Speed(9.0);
    sleep(6);
    DealMessage();
    HeparinPump_Stop();

    CanOpen_Master_Quit();

    return 0;
}
