#ifndef STATE_PRIME_H
#define STATE_PRIME_H

#include "state.h"

class StatePrime : public StateUnit
{
public:
    StatePrime(char *name=NULL);

    void ActionEnter(void);
    void ActionExit(void);
};

#endif
