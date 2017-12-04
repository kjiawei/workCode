/*
   This file is part of CanFestival, a library implementing CanOpen Stack.

   Copyright (C): Edouard TISSERANT and Francis DUPIN

   See COPYING file for copyrights details.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
   */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include "canmsg.h"
#include "mcp2515.h"

#include "can_driver.h"

static void can_show(struct canmsg_t *canmsg)
{
    printf("id=%x srr=%d ide=%d eid=%d rtr=%d rb1=%d rb0=%d dlc=%d\n",
	   canmsg->header.id,
	   canmsg->header.srr,
	   canmsg->header.ide,
	   canmsg->header.eid,
	   canmsg->header.rtr,
	   canmsg->header.rb1,
	   canmsg->header.rb0,
	   canmsg->header.length);
    printf("Data 0:%02x 1:%02x 2:%02x 3:%02x 4:%02x 5:%02x 6:%02x 7:%02x\n\n",
            canmsg->data[0],
            canmsg->data[1],
            canmsg->data[2],
            canmsg->data[3],
            canmsg->data[4],
            canmsg->data[5],
            canmsg->data[6],
            canmsg->data[7]);
}

/*********functions which permit to communicate with the board****************/
UNS8 canReceive_driver(CAN_HANDLE fd0, Message *m)
{
    int res;
    struct canmsg_t canmsg;

    memset(&canmsg, 0, sizeof(struct canmsg_t));
    do{
        res = read(fd0, &canmsg, sizeof(canmsg_t));
        if((res<0)&&(errno == -EAGAIN)) res = 0;
    }while(res==0);

    //printf("Receive:\n");
    //can_show(&canmsg);

    if(res != sizeof(canmsg_t)) // No new message
        return 1;

    m->cob_id = canmsg.header.id;
    m->len = canmsg.header.length;
    if(canmsg.header.rtr&MSG_RTR){
        m->rtr = 1;
    }else{
        m->rtr = 0;
        memcpy(m->data,canmsg.data,8);
    }

    return 0;
}

/***************************************************************************/
UNS8 canSend_driver(CAN_HANDLE fd0, Message const *m)
{
    int res;
    struct canmsg_t canmsg;
    memset(&canmsg, 0, sizeof(struct canmsg_t));

    canmsg.header.id = m->cob_id;
    canmsg.header.length = m->len;
    if(m->rtr){
        canmsg.header.rtr = MSG_RTR;
    } else {
        memcpy(canmsg.data, m->data, m->len);
    }

    //if(canmsg.id >= 0x800){
    //    canmsg.flags |= MSG_EXT;
    //}

    //printf("Send:\n");
    //can_show(&canmsg);
    res = write(fd0, &canmsg, sizeof(canmsg_t));
    if(res!=sizeof(canmsg_t))
        return 1;

    return 0;
}

/***************************************************************************/
UNS8 canReset_driver(CAN_HANDLE fd0)
{
    ioctl((int)fd0, CAN_IOCTRESET, 0);
}

/***************************************************************************/
#define CAN_NORMALMODE          1   /* go normal mode */
#define CAN_LOOPBACKMODE        2   /* go loopback mode */
#define CAN_LISTENONLYMODE      3   /* go listen only mode */
#define CAN_SLEEPMODE           4   /* go sleep mode */
UNS8 canModeset_driver(CAN_HANDLE fd0, int mode)
{
    switch (mode) {
        case CAN_NORMALMODE:
            ioctl((int)fd0, CAN_IOCTNORMALMODE, 0);     /* go normal mode */
            break;
        case CAN_LOOPBACKMODE:
            ioctl((int)fd0, CAN_IOCTLOOPBACKMODE, 0);   /* go loopback mode */
            break;
        case CAN_LISTENONLYMODE:
            ioctl((int)fd0, CAN_IOCTLISTENONLYMODE, 0); /* go listen only mode */
            break;
        case CAN_SLEEPMODE:
            ioctl((int)fd0, CAN_IOCTSLEEPMODE, 0);      /* go sleep mode */
            break;
    }
    return 1;
}

/***************************************************************************/
int TranslateBaudeRate(char* optarg)
{
    if(!strcmp( optarg, "1M")) return 1000000;
    if(!strcmp( optarg, "500K")) return 500000;
    if(!strcmp( optarg, "250K")) return 250000;
    if(!strcmp( optarg, "125K")) return 125000;
    if(!strcmp( optarg, "100K")) return 100000;
    if(!strcmp( optarg, "50K")) return 50000;
    if(!strcmp( optarg, "20K")) return 20000;
    if(!strcmp( optarg, "10K")) return 10000;
    if(!strcmp( optarg, "none")) return 0;
    return 0x0000;
}

/***************************************************************************/
UNS8 canChangeBaudRate_driver( CAN_HANDLE fd0, char* baud)
{
    const int baudrate = TranslateBaudeRate(baud);
    int ret = -1;

    if(baudrate == 0)
        return 0;
    ioctl((int)fd0, CAN_IOCSRATE, baudrate);
    ioctl((int)fd0, CAN_IOCGRATE, &ret);
    if(ret != baudrate) {
        fprintf(stderr, "canOpen_driver (lincan): IOCTL set speed failed\n");
        return 0;
    }
    return 1;
}

/***************************************************************************/
static const char lnx_can_dev_prefix[] = "/dev/can";

CAN_HANDLE canOpen_driver(s_BOARD *board)
{
    int name_len = strlen(board->busname);
    int prefix_len = strlen(lnx_can_dev_prefix);
    char dev_name[prefix_len+name_len+1];
    int o_flags = 0;
    int fd;

    /*o_flags = O_NONBLOCK;*/

    memcpy(dev_name,lnx_can_dev_prefix,prefix_len);
    memcpy(dev_name+prefix_len,board->busname,name_len);
    dev_name[prefix_len+name_len] = 0;

    fd = open(dev_name, O_RDWR|o_flags);
    if(fd < 0) {
        fprintf(stderr,"!!! Board %s is unknown. See can_lincan.c\n", board->busname);
        goto error_ret;
    }

    // set baudrate
    if (canChangeBaudRate_driver((CAN_HANDLE)fd, board->baudrate) < 1) {
        fprintf(stderr, "canOpen_driver (lincan): IOCTL set speed failed\n");
        goto error_ret;
    }
    canModeset_driver((CAN_HANDLE)fd, CAN_NORMALMODE);

    return (CAN_HANDLE)fd;

error_ret:
    return NULL;
}

/***************************************************************************/
int canClose_driver(CAN_HANDLE fd0)
{
    if(!fd0)
        return 0;
    close(fd0);
    return 0;
}
