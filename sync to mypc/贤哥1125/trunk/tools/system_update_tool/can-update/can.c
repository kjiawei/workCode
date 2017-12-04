#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "can.h"
#include "protocol.h"
#include "file_hex.h"
#include "config_updatefw.h"

/*半字节查表法 X8+X2+X1+1   key = 0x07*/
const unsigned char CRC8Table[16] = {
    0x00,0x07,0x0e,0x09,
    0x1c,0x1b,0x12,0x15,
    0x38,0x3f,0x36,0x31,
    0x24,0x23,0x2a,0x2d
};

unsigned char cal_crc(unsigned char *ptr, unsigned char len)
{
    //半字节查表
    unsigned char crc = 0;
    unsigned char da;
    while(len-- != 0)
    {
        da = crc / 16;							/* 暂存CRC 的高四位 */
        crc <<= 4;								/* CRC 右移4 位，相当于取CRC 的低12 位）*/
        crc ^= CRC8Table[da ^ (*ptr / 16)];		/* CRC 的高4 位和本字节的前半字节相加后查表计算CRC，然后加上上一次CRC的余数 */
        da = crc / 16;							/* 暂存CRC 的高4 位 */
        crc <<= 4;								/* CRC 右移4 位， 相当于CRC 的低12 位） */
        crc ^= CRC8Table[da ^ (*ptr & 0x0f)];	/* CRC 的高4位和本字节的后半字节相加后查表计算CRC，然后再加上上一次CRC的余数 */
        ptr++;
    }
    return(crc);
}

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
        printf("ret not eque len count: %d\n",count);
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
    struct CanMessage recv_mess;
    
    unsigned char SDO_data[4][8] = {
        {0x23, 0x00, 0x20, 0x01, 0x00, 0x00, 0x00, 0x55},//power pump
        {0x23, 0x00, 0x20, 0x02, 0x55, 0x00, 0x00, 0x00},//肝素
        {0x23, 0x00, 0x20, 0x02, 0x55, 0x00, 0x00, 0x00} //主控  监控
    };

    //如果心跳信号，且是预操作
    if ((msg->func == 0xe) && (msg->data[0] == 0x7f)) {
        //发送NMT信号
        message.id = CAN_MASTER_ID;
        message.rtr = 0;
        message.len = 2;
        message.func = 0;
        message.data[0] = 0x01;
        message.data[1] = id;
        return CanSend(&message);
    }

    
    //如果心跳信号，且是可操作状态
    //或者是pdo
    //或者是紧急对像
    if (((msg->func == 0xe) && (msg->data[0] == 0x05)) ||
            (msg->func == 0x3) ||
            (msg->func == 0x1)) {
        //这个can 的节点
        message.id = id;
        //这个是区分，远程帧与数据帧的
        message.rtr = 0;
        //数据的长度
        message.len = 8;
        //sdo功能码，跟id组合，变成0x600+节点
        message.func = 0xC;
        //zhong shao hua change message 2014-01-11
        //data0是0x23，是sdo写四个字节的命令
        switch (id) {
        case 7:
        case 9:
        case 10:
            memcpy(message.data, &SDO_data[0], 8);
            break;
        case 15:
            memcpy(message.data, &SDO_data[1], 8);
            break;
        case 3:
        case 5:
            memcpy(message.data, &SDO_data[2], 8);
            break;
        default:
            break;
        }

        message.data[1]=cal_crc(message.data,8);

        CanSend(&message);
        
		int i;
		for ( i = 0; i < 5; i++) {
			CanRead(&recv_mess);
			if ((recv_mess.id  == id) && (recv_mess.data[0] == 0x60))
				break;
			else {
				usleep(10000);
				CanSend(&message);
				printf("send goto bootloader cmd times is %d\n", i);
            }
		}
        sleep(1);
        return 1;
    }

    return -1;
}

int CanOpen_Detect(const char id, struct CanMessage *message)
{
    if (message->id == id) {
        return CanOpen_SwitchCan(id, message);
    }
    return -1;
}

//一直读canopen的数据包，通过ID号，判断相应子设备是否子正常工作#/
int CanOpen_Slave(const char id)
{
    int i;
    struct CanMessage message;

    for (i=0; i<10; i++) {
        if (CanRead(&message) == 0) { //随便读一包数据#/
            if (CanOpen_Detect(id, &message) == 1) {//通过ID号检测相应设备#/
                break;
            }
        }
    }
//如果连续读 10 次都没有数据，表示要升级的节点已经进入BootLoader
    memset(&message, 0, sizeof(struct CanMessage));
    message.id = id;
    message.rtr = 1;
    message.len = 1;
    //功能码为0，打开状态
    message.data[0] = CAN_COMM_STATUS_OPEN;

    //发送数据
    if (CanSend(&message) != 0) {
        return -1;
    }

    //功能码为0；打开状态，则返回0
    for (i=0; i<10; i++) {
        if (CanRead(&message) == 0) {
            if ((message.func == CAN_UPDATE_FUNC_OPEN) && (message.data[1] == CAN_COMM_STATUS_OPEN))
                return 0;
        } else {
            printf("canread error\n");
        }
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
    //功能码为0x100;发送编程数据
    message.func = CAN_UPDATE_FUNC_PROG_DATA;
    memcpy(message.data, buf, len);

    //发送数据
    if (CanSend(&message) != 0) {
        return -1;
    }

    //读返回的数据
    if (CanRead(&message) != 0) {
        return -1;
    }

    //接收数据成功且传输结束
    //或者
    //接收数据成功，继续等待接收
    //则返回0
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
    //功能码为0x080;
    message.func = CAN_UPDATE_FUNC_PROG_CMD;
    //由于data[0]为0，所以为选择指定页地址flash编程
    message.data[1] = page;
    message.data[2] = page1;
    message.data[3] = chk;
    
    //发送数据
    if (CanSend(&message) != 0) {
        return -1;
    }

    //接收数据
    if (CanRead(&message) != 0) {
        return -1;
    }

    //zhong shao hua change 2014-01-17
    //功能码为0x080;长度为1
    //以前没有判断data[0]。。。。
    if (message.func == CAN_UPDATE_FUNC_PROG_CMD) {
        if ( (message.len == 1) && ( (message.data[0]) == 0x0) ){
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

//启动应用程序
int CanSelect_AppRun(const int id)
{
    struct CanMessage message;

    memset(&message, 0, sizeof(struct CanMessage));
    message.id = id;
    message.rtr = 1;
    message.len = 2;
    //功能码为0x200;
    message.func = CAN_UPDATE_FUNC_APP_CMD;
    message.data[0] = 3;

    //发送
    if (CanSend(&message) != 0) {
        return -1;
    }

    return -1;
}
