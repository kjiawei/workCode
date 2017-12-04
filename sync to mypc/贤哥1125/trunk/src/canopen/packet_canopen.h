#ifndef PACKAGE_H
#define PACKAGE_H

#include <stdio.h>

struct CAN_Package_Sdo_Header {
    unsigned char cs;
    unsigned short index;
    unsigned char indexSub;
    unsigned char str[4];
}__attribute__ ((__packed__));

#define CAN_SDO_INDEX                               0x2000

/**************************************************************
  HeparinPump CanId: 0x0F
 **************************************************************/
#define CAN_HEPARIN_PUMP_SDO_INDEX                  0x2000
#define CAN_HEPARIN_PUMP_SDO_INDEXSUB_QUICKSPEED    0x01
#define CAN_HEPARIN_PUMP_SDO_INDEXSUB_SYRING        0x02
#define CAN_HEPARIN_PUMP_SDO_INDEXSUB_DOSESPEED     0x03
#define CAN_HEPARIN_PUMP_SDO_INDEXSUB_CONTROL       0x04

#define CAN_HEPARIN_PUMP_SDO_LEN_QUICKSPEED         0x04
#define CAN_HEPARIN_PUMP_SDO_LEN_SYRING             0x04
#define CAN_HEPARIN_PUMP_SDO_LEN_DOSESPEED          0x04
#define CAN_HEPARIN_PUMP_SDO_LEN_CONTROL            0x01

struct CAN_HeparinPump_PDO1_0F {
    unsigned short N15_all_value;
    unsigned short N15_dose_speed;
    unsigned char  N15_mode;
    unsigned char  N15_sub_mode;
    unsigned short N15_NULL;
}__attribute__ ((__packed__));

struct CAN_HeparinPump_QuickSpeed_SDO_0F {
    unsigned short mm_per_cycle;
    unsigned short quick_speed;
}__attribute__ ((__packed__));

struct CAN_HeparinPump_DoseSpeed_SDO_0F {
    unsigned short first_dose;
    unsigned short dose_speed;
}__attribute__ ((__packed__));

struct CAN_HeparinPump_Syring_SDO_0F {
    unsigned short syringe_length;
    unsigned short syringe_style;
}__attribute__ ((__packed__));


/**************************************************************
  Power   CanId: 0x07
 **************************************************************/
#define CAN_POWER_SDO_INDEX                 0x6200
#define CAN_POWER_SDO_INDEXSUB_SHUTDOWN     0x01

struct CAN_Power_PDO1_07 {
    unsigned short N7_adc0;
    unsigned short N7_adc1;
    unsigned short N7_adc2;
    unsigned short N7_adc3;
}__attribute__ ((__packed__));

struct CAN_Power_PDO2_07 {
    unsigned char  N7_state;
    unsigned char  N7_bat_h;
    unsigned char  N7_bat_L;
    unsigned char  N7_charge;
}__attribute__ ((__packed__));


/**************************************************************
  Blood Pump   CanId: 0x09
 **************************************************************/
#define CAN_BLOOD_PUMP_SDO_INDEX                0x2000
#define CAN_BLOOD_PUMP_SDO_INDEXSUB_SPEED       0x01
#define CAN_BLOOD_PUMP_SDO_INDEXSUB_DIA         0x02
#define CAN_BLOOD_PUMP_SDO_INDEXSUB_MODE        0x02

struct CAN_BloodPump09_PDO {
    unsigned short N9_all_value;
    unsigned short N9_speed;
    unsigned char N9_mode;
    unsigned char N9_sub_mode;
    unsigned char N9_door_status;
}__attribute__ ((__packed__));


/**************************************************************
  MasterCtrl   CanId: 0x03
 **************************************************************/
#define CAN_SDO_INDEXSUB_SETUP              0x01
#define CAN_SDO_INDEXSUB_CMD                0x02
#define CAN_SDO_INDEXSUB_FLUID              0x03
#define CAN_SDO_INDEXSUB_SCALE              0x04
#define CAN_SDO_INDEXSUB_AIM                0x05
#define CAN_SDO_INDEXSUB_UF                 0x06
#define CAN_SDO_INDEXSUB_BLOOD              0x07
#define CAN_SDO_INDEXSUB_HEPARIN            0x08
#define CAN_SDO_INDEXSUB_RANGETEMP          0x09
#define CAN_SDO_INDEXSUB_RANGEAP            0x0A
#define CAN_SDO_INDEXSUB_RANGEVP            0x0B
#define CAN_SDO_INDEXSUB_RANGETMP           0x0C


/**************************************************************
  MasterCtrl   CanId: 0x07
 **************************************************************/
#define CAN_SDO_INDEXSUB_SHUTDOWN              0x01

#endif
