#ifndef CAN_PROTOCOL_H
#define CAN_PROTOCOL_H

#include "can.h"

#define CAN_GET_ID(y)                   (y&0x0F)
#define CAN_GET_FUNC(y)                 ((y&0x7FF)>>7)
#define CAN_SET_ID(x, y)                (((y&0x7F)<<7)+x)

#define CAN_FLASH_PAGE_SIZE             256

#define CAN_UPDATE_FUNC_OPEN            0
#define CAN_UPDATE_FUNC_PROG_CMD        1
#define CAN_UPDATE_FUNC_PROG_DATA       2
#define CAN_UPDATE_FUNC_DISPLAY_DATA    3
#define CAN_UPDATE_FUNC_APP_CMD         4
#define CAN_UPDATE_FUNC_PAGE_SELECT     6
#define CAN_UPDATE_FUNC_ERROR           6

#define CAN_COMM_STATUS_CLOSE           0
#define CAN_COMM_STATUS_OPEN            1

struct CanMessage{
    unsigned char id;
    unsigned char func;
    unsigned char rtr;
    unsigned char len;
    unsigned char data[CAN_FRAME_MAX_DATA_LEN];
};

int CanInit(const char id);
void CanQuit(void);

int CanSend(struct CanMessage *message);
int CanRead(struct CanMessage *message);

int CanOpen_Slave(const char id);
int CanSend_HexPage(const unsigned char *buf,
        const int page,
        const int page1,
        unsigned char chk,
        const int id);
int CanSelect_AppRun(const int id);

#endif
