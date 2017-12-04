#ifndef STATE_TEST_H
#define STATE_TEST_H

#include "state.h"
#include "data_custom.h"

class StateTest : public StateUnit, public ActionCustom
{
public:
    StateTest(char *name=NULL);

    void ActionEnter(void);
    void ActionExit(void);

private:
    void Init();
    void ActionHandle(const void* dataPtr=NULL, int name=0);

private:
    struct PacketBloodPump packetBloodPump;
    PacketCheckItem checkItem;
};

#endif
