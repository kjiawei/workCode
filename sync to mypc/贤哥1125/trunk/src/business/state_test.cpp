#include "deal.h"
#include "state_test.h"
#include "interfix_os.h"
#include <stdlib.h>
#include <unistd.h>
#include "config_ui.h"

StateTest::StateTest(char *name) :
    StateUnit(name)
{
}

void StateTest::Init()
{
    dataNodeAddObject(PACKET_NAME_BLOODPUMP, this);

    int i;
    memset(&checkItem, 0, sizeof(PacketCheckItem));

    checkItem.checkIs = CHECK_HEMO_PUMP;
    for (i=0; i<CHECK_NUM; i++) {
        checkItem.checkState[i] = CHECK_ITEM_CHECK_DOING;//CHECK_ITEM_CHECK_PRE;
    }
    dataNodeUpdate(PACKET_NAME_SELFTEST, &checkItem);
#if 0
    if ((packetBloodPump.canSt == STATE_CAN_Pre_operational) ||
            (packetBloodPump.canSt == STATE_CAN_Stopped)) {
        BloodPump_CanStart();
        usleep(20000);
    }
#endif
}

void StateTest::ActionEnter(void)
{
    Init();

#if 0
    int i, flag;
    /* test hemopump */
    checkItem.checkState[CHECK_HEMO_PUMP] = CHECK_ITEM_CHECK_DOING;
    dataNodeUpdate(PACKET_NAME_SELFTEST, &checkItem);
    flag = -1;
    for (i=0; i<4 && flag==-1; i++) {
        if (packetBloodPump.canSt != STATE_CAN_Operational) {
            BloodPump_CanStart();
            interfix_sleep(1);
            continue;
        }
        if (packetBloodPump.mode == 1) {
            BloodPump_ActStop();
            interfix_sleep(1);
        }

        BloodPump_ActDia((unsigned char)6);
        usleep(40000);
        BloodPump_ActSpeed(400);
        usleep(40000);
        BloodPump_ActStart();
        interfix_sleep(5);
        if (packetBloodPump.mode != 1) {
            checkItem.checkState[CHECK_HEMO_PUMP] = CHECK_ITEM_CHECK_FAILED;
        } else if (abs(packetBloodPump.speed-400)>20) {
            checkItem.checkState[CHECK_HEMO_PUMP] = CHECK_ITEM_CHECK_FAILED;
        } else {
            flag = 1;
            checkItem.checkState[CHECK_HEMO_PUMP] = CHECK_ITEM_CHECK_WELL;
        }
        dataNodeUpdate(PACKET_NAME_SELFTEST, &checkItem);
    }
    BloodPump_ActStop();
#endif

    /* test ... */

    interfix_sleep(2);
    checkItem.checkIs = CHECK_HEMO_PUMP;//CHECK_NUM
    dataNodeUpdate(PACKET_NAME_SELFTEST, &checkItem);
    //StateSwitch("Test");//""

    dataNodeDelObject(PACKET_NAME_BLOODPUMP, this);
}

void StateTest::ActionExit(void)
{
    dataNodeDelObject(PACKET_NAME_BLOODPUMP, this);
}

void StateTest::ActionHandle(const void* dataPtr,int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("state_test-ActionHandle-in");
#endif
    if (name==PACKET_NAME_BLOODPUMP) {
        memcpy(&packetBloodPump, dataPtr, sizeof(struct PacketBloodPump));
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("state_test-ActionHandle-out");
#endif
}
