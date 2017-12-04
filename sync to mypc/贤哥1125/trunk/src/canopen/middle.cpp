#include "middle.h"
#include "middle_tmp.h"

MessageSignal messageSignal;

void MessageSignalMiddle(const Message *message)
{
    messageSignal.MessageSignalApp(message);
}
