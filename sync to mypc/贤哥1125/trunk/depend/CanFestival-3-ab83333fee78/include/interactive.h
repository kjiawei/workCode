#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include "can.h"

#define EVENT_DATA_PDO               1
#define EVENT_DATA_SDO               2
#define EVENT_DATA_NMT               3
#define EVENT_DATA_SDO_READ          4
#define EVENT_DATA_SDO_WRITE         5
#define EVENT_DATA_SDO_RETURN        6

typedef void (*MessageEmitApp)(const Message *);

int InterData_Init(MessageEmitApp app);

#endif
