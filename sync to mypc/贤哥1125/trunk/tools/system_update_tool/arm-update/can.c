#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "can.h"
#include "protocol.h"
#include "file_hex.h"
#include "config_updatefw.h"

static void can_show_header(struct can_frame_header *header)
{
    printf("Header: id=0X%x srr=%d ide=%d eid=%d rtr=%d rb1=%d rb0=%d dlc=%d\n",
            header->id,
            header->srr,
            header->ide,
            header->eid,
            header->rtr,
            header->rb1,
            header->rb0,
            header->dlc);
}
static void can_show_data(unsigned char *data)
{
    printf("Data: 0=%02x 1=%02x 2=%02x 3=%02x 4=%02x 5=%02x 6=%02x 7=%02x\n",
            data[0],
            data[1],
            data[2],
            data[3],
            data[4],
            data[5],
            data[6],
            data[7]);
}

static int device = -1;

void delay(void)
{
    int i =10000;
    while(i--);	
}

int CanInit(const char id)
{
    struct can_filter filter = {
        .fid = {
            {0, 0, 0, 1},
        },
        .sidmask = 0x7F,
        .eidmask = 0x00,
        .mode = 0,
    };
    device = open(CAN_DEV, O_RDWR | O_NONBLOCK);
    if (device < 0){
        perror("open:device");
        exit(-1);
    }

    filter.fid[0].id = id;
    ioctl(device, CAN_IOCSFILTER, &filter);
    ioctl(device, CAN_IOCSRATE, CAN_BAUDRATE);
    ioctl(device, CAN_IOCTNORMALMODE);

    return device;
}

void CanQuit(void)
{
    close(device);
    device = -1;
}

int CanSend(struct CanMessage *message)
{
    if (message->len > CAN_FRAME_MAX_DATA_LEN) {
        return -1;
    }

    struct can_frame frame;
    const int frameLen =  sizeof(struct can_frame);

    memset(&frame, 0, frameLen);
    frame.header.id = CAN_SET_ID(message->id, message->func);
    frame.header.dlc = message->len;
    memcpy(frame.data, message->data, message->len);
    if (write(device, &frame, frameLen) != frameLen) {
        return -1;
    }
    printf("Send: \n");
    can_show_header(&frame.header);
    can_show_data(frame.data);
    return 0;
}

int CanRead(struct CanMessage *message)
{
    int ret;
    int count;
    struct can_frame frame_recv;
    const int len = sizeof(struct can_frame);

    memset(&frame_recv, 0, len);
    ret = -1;
    count = 0;
    while ((count<2000) && (ret <1)) {
        usleep(100);
        ret = read(device, &frame_recv, len);
        count++;
    }
    if (ret != len) {
        return -1;
    }
    printf("RECV: \n");
    can_show_header(&frame_recv.header);
    can_show_data(frame_recv.data);
    message->id = CAN_GET_ID(frame_recv.header.id);
    message->func = CAN_GET_FUNC(frame_recv.header.id);
    message->len = frame_recv.header.dlc;
    memcpy(message->data, frame_recv.data, message->len);

    return 0;
}

inline int CanOpen_SwitchCan(const char id, struct CanMessage *msg)
{
    struct CanMessage message;

    if ((msg->func == 0xe) && (msg->data[0] == 0x7f)) {
        message.id = CAN_MASTER_ID;
        message.rtr = 0;
        message.len = 2;
        message.func = 0;
        message.data[0] = 0x01;
        message.data[1] = id;
        return CanSend(&message);
    }

    if (((msg->func == 0xe) && (msg->data[0] == 0x05)) ||
            (msg->func == 0x3) ||
            (msg->func == 0x1)) {
        if ((msg->data[0] == 0x60) && (msg->data[1] == 0x00) &&
                (msg->data[2] == 0x20) && (msg->data[3] == 0x03)) {
            return 1;
        }
        message.id = id;
        message.rtr = 0;
        message.len = 8;
        message.func = 0xC;
        message.data[0] = 0x2f;
        message.data[1] = 0x00;
        message.data[2] = 0x20;
        message.data[3] = 0x03;
        message.data[4] = 0x01;
        if (CanSend(&message) == 0) {
            CanRead(&message);
        }
    }

    return 0;
}

int CanOpen_Detect(const char id, struct CanMessage *message)
{
    if (message->id == id) {
        return CanOpen_SwitchCan(id, message);
    }
    return -1;
}

int CanOpen_Slave(const char id)
{
    int i;
    struct CanMessage message;

    for (i=0; i<5; i++) {
        if (CanRead(&message) == 0) {
            if (CanOpen_Detect(id, &message) == 1) {
                break;
            }
        }
    }

    memset(&message, 0, sizeof(struct CanMessage));
    message.id = id;
    message.rtr = 1;
    message.len = 1;
    message.data[0] = CAN_COMM_STATUS_OPEN;

    if (CanSend(&message) != 0) {
        return -1;
    }

    if (CanRead(&message) != 0) {
        return -1;
    }

    if ((message.func == CAN_UPDATE_FUNC_OPEN) &&
            (message.data[1] == CAN_COMM_STATUS_OPEN)) {
        return 0;
    }
    return -1;
}

int CanSend_data(const unsigned char *buf, const int len, const int id)
{
    struct CanMessage message;
    memset(&message, 0, sizeof(struct CanMessage));
    message.id = id;
    message.rtr = 1;
    message.len = len;
    message.func = CAN_UPDATE_FUNC_PROG_DATA;
    memcpy(message.data, buf, len);

    if (CanSend(&message) != 0) {
        return -1;
    }

    if (CanRead(&message) != 0) {
        return -1;
    }

    if ((message.func == CAN_UPDATE_FUNC_PROG_DATA) &&
            ((message.len==1 && message.data[0]==0) ||
             (message.len==2 && message.data[0]==2))) {
        return 0;
    }
    return -1;
}

int CanSend_AdressSelect(const int id,
        const int page,
        const int page1,
        unsigned char chk)
{
    struct CanMessage message;
    memset(&message, 0, sizeof(struct CanMessage));
    message.id = id;
    message.rtr = 1;
    message.len = 4;
    message.func = CAN_UPDATE_FUNC_PROG_CMD;
    message.data[1] = page;
    message.data[2] = page1;
    message.data[3] = chk;

    if (CanSend(&message) != 0) {
        return -1;
    }

    if (CanRead(&message) != 0) {
        return -1;
    }
    if (message.func == CAN_UPDATE_FUNC_PROG_CMD) {
        if (message.len == 1) {
            return 0;
        }
    }
    return -1;
}

int CanSend_HexPage(const unsigned char *buf,
        const int page,
        const int page1,
        unsigned char chk,
        const int id)
{
    if (buf == NULL) {
        return -1;
    }

    int count = 0;
    unsigned char *pbuf = (unsigned char *)buf;

    CanSend_AdressSelect(id, page, page1, chk);
    for (count=0; count < CAN_FLASH_PAGE_SIZE; count += CAN_FRAME_MAX_DATA_LEN) {
        if (CanSend_data(pbuf+count, CAN_FRAME_MAX_DATA_LEN, id) != 0) {
            return -1;
        }
    }
    return 0;
}

int CanSelect_AppRun(const int id)
{
    struct CanMessage message;

    memset(&message, 0, sizeof(struct CanMessage));
    message.id = id;
    message.rtr = 1;
    message.len = 2;
    message.func = CAN_UPDATE_FUNC_APP_CMD;
    message.data[0] = 3;

    if (CanSend(&message) != 0) {
        return -1;
    }

    return -1;
}
