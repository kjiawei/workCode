#ifndef CANOPEN_SHELL_H
#define CANOPEN_SHELL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "config_canopen.h"
#include "applicfg.h"
#include "can.h"

#define DE_SDO_SENDING  1
#define DE_SDO_OK   2
#define DE_SDO_ERROR 3

extern char CAN_MUTEX_STATE;   //CAN SDO 锁状态#/

int CanNode_Start(unsigned char nodeid);
int CanNode_Stop(unsigned char nodeid);
void CanNode_Reset(unsigned char nodeid);
void CanNode_Discover();

void GetSlaveNodeInfo(unsigned char);

int ReadDeviceSdoEntry(int nodeid, int index, int subindex);
int ReadDeviceSdoIsOk();
unsigned int ReadDeviceSdoData();

//int WriteDeviceSdoEntry(int nodeid, int index, int subindex, int size, unsigned int data);
int WriteDeviceSdoEntry(int nodeid, int index, int subindex, int size, void *data);
int WriteDeviceSdoIsOk();

int CanOpen_Master_Init(void);
int CanOpen_Master_Quit(void);

unsigned char CanOpen_GetState(unsigned char nodeId);

unsigned char TrinsmitPDO(void);
unsigned char setPDOContent(unsigned char *pData);

//发送SDO时的状态标志设置#/
void SetSendSDO_State(int id,unsigned char state);
//接收SDO时的状态标志设置，并且保存接收数据#/
void SetRecSDO_State(int id,unsigned char state,void *ptr);
//获取发送SDO的状态标志设置，并读取接收数据#/
unsigned char GetSDO_State(int id,void *gptr);

extern int CAN_FD_FLAG; //CAN设备打开文件描述符#/

unsigned char cal_crc(unsigned char *ptr, unsigned char len);

//打开can设备#/
int CanOpen_Handle(char* baud);
//关闭can设备#/
int CanClose_Handle(int fd0);
//CAN设备发送数据#/
UNS8 CanSend_Handle(int fd0, Message const *m);
//CAN设备读取接收数据#/
UNS8 CanReceive_Handle(int fd0, Message *m);

//----HJX实现直接操作#/
//发送本设备所有T-PDO的数据#/
unsigned char HJX_TrinsmitPDO(unsigned char *pData);

#ifdef __cplusplus
}
#endif

#endif
