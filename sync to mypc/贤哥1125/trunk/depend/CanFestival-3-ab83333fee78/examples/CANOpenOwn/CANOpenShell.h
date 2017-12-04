#ifndef CANOPEN_SHELL_H
#define CANOPEN_SHELL_H

#ifdef __cplusplus
extern "C" {
#endif

#define eprintf(...) printf (__VA_ARGS__)

#include "deal.h"
#include "canfestival.h"

int CanNode_Start(UNS8 nodeid);
int CanNode_Stop(UNS8 nodeid);
void CanNode_Reset(UNS8 nodeid);
void CanNode_Discover();

void CheckReadInfoSDO(CO_Data*, UNS8);
void GetSlaveNodeInfo(UNS8);

void CheckReadSDO(CO_Data*, UNS8);
int ReadDeviceSdoEntry(int nodeid, int index, int subindex);
int ReadDeviceSdoIsOk();
unsigned int ReadDeviceSdoData();

void CheckWriteSDO(CO_Data*, UNS8);
int WriteDeviceSdoEntry(int nodeid, int index, int subindex, int size, unsigned int data);
int WriteDeviceSdoIsOk();

int CanOpen_Master_Init(void);
int CanOpen_Master_Quit(void);

e_nodeState CanOpen_GetState(UNS8 nideId);
void ReadHeartBeat(int nodeid);

#ifdef __cplusplus
}
#endif

#endif
