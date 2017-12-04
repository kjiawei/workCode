#ifndef CANOPEN_CONFIG_H
#define CANOPEN_CONFIG_H

#define CANOPEN_LIBRARY_PATH        "lib/libcanfestival_can_mcp2515.so"
#define CANOPEN_MASTER_NODE_ID      1
#define CANOPEN_DEV_NAME            "0"
#define CANOPEN_DEV_BAUD_RATE       "500K"

/* The nodes states */
#define STATE_CAN_Initialisation    0x00
#define STATE_CAN_Disconnected      0x01
#define STATE_CAN_Connecting        0x02
#define STATE_CAN_Preparing         0x02
#define STATE_CAN_Stopped           0x04
#define STATE_CAN_Operational       0x05
#define STATE_CAN_Pre_operational   0x7F
#define STATE_CAN_Unknown_state     0x0F

//其他设备节点的ID号
#define CANID_UI                    0x1
#define CANID_MASTERCTRL            0x3
#define CANID_MONITOR               0x5
#define CANID_POWER                 0x7
#define CANID_BLOOD_PUMP            0x9
#define CANID_BLOOD_PUMP2           0xA
#define CANID_TEST                  0xF
#define CANID_HEPARIN_PUMP          0xF
#define CANID_HEMOMANO_METER        0x13
//#define CANID_H.._SENSOR          0x15

#endif
