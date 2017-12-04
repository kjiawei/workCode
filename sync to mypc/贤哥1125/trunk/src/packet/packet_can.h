#ifndef PACKET_CAN_H
#define PACKET_CAN_H

//本文件是具体和底层设备canopen通信的数据结构内容

//所有结构变量均一一对应协议文件中底层设备中的变量
//详情参考 /pulic/protocols/中的协议文件
//N1：表示发送该PDO的设备号为1
//01H：对应第一个字节的值，即协议文件中的BYTE0的值
//PDO数据结构为 8个字节

#ifdef __cplusplus
extern "C" {
#endif

/*  Function Codes  */
//canopen把canbus数据包的前11位标示符称通讯对象ID（COB-ID）
//COB-ID=4位功能码+7位节点ID
//下面就是各通讯对象的功能码
#define EVENT_NMT           0x0
#define EVENT_SYNC          0x1
#define EVENT_TIME_STAMP    0x2
#define EVENT_PDO1tx        0x3
#define EVENT_PDO1rx        0x4
#define EVENT_PDO2tx        0x5
#define EVENT_PDO2rx        0x6
#define EVENT_PDO3tx        0x7
#define EVENT_PDO3rx        0x8
#define EVENT_PDO4tx        0x9
#define EVENT_PDO4rx        0xA
#define EVENT_SDOtx         0xB    //人机板对其他节点发送SDO命令后，节点应答的SDO，如主控板的0X583#/
#define EVENT_SDOrx         0xC    //人机板接收到其他节点的SDO命令，如主控板的0x603#/
#define EVENT_NODE_GUARD    0xE
#define EVENT_LSS           0xF
#define EVENT_NULL          0x7F

#define CompareAssignFlage(x, y, f)     do{if(x!=y){x=y;f=0;}}while(0)

/*
 * Master Ctrl struct
 */

//
struct PacketStatusNote{
    unsigned char dataType;

    struct PacketStatus{
        unsigned char header_status; //总状态
        unsigned char blood_status; //血路状态
        unsigned char water_status; //水路状态
    }__attribute__ ((__packed__))status;

    struct PacketNote{   //对应主控板的PDO0的4个提示信息字节
        unsigned char note1;
        unsigned char note2;
        unsigned char note3;
        unsigned char note4;
    }__attribute__ ((__packed__))note;
}__attribute__ ((__packed__));//这个添加的字段是告诉编译器，取消编译过程中的优化对齐，按照
//实际占用字节数进行对齐。


struct PacketOwnNote{//本机的提示信息
    unsigned char dataType;
    struct OwnNote{
        unsigned char note1;   //1:护士呼#/
        unsigned char note2;
        unsigned char note3;
        unsigned char note4;    //HJX程序提示信息  bit0:1 单次定时报警 bit1：1 循环定时报警 #/
        unsigned char note5;    //HJX程序提示信息  bit0:1 收缩压范围报警 bit1：1舒张压范围报警 bit2：1脉搏范围报警#/
        unsigned char note6;    //HJX程序提示信息  bit0:N3通讯失败 bit1：N5通讯失败 2：N9 3：N10 4：N15 5：N7 #/
        unsigned char note7;    //HJX程序提示信息  bit0:1 水路工作异常,bit1:1主控板手动预冲状态错误，bit2：自动预冲 bit3：在线预冲 bit4：引血 bit5：在线引血 bit6：回血 bit7：在线回血#/
    }__attribute__ ((__packed__))note;
}__attribute__ ((__packed__));

struct PacketStatusAlarm{
        unsigned char dataType;

        struct PacketAlarm{   //对应主控板和监控板的PDO1的几个字节
            unsigned char errorstate1;
            unsigned char errorstate2;
            unsigned char errorstate3;
            unsigned char errorstate4;
            unsigned char errorstate5;
            unsigned char Iostatus;
            unsigned char errorstate7;
          }__attribute__ ((__packed__))alarm;
}__attribute__ ((__packed__));

struct PacketValve{
    unsigned char dataType;
    unsigned int valve32;
    unsigned char u502a;
    unsigned char u502b;
    unsigned char u503b;
}__attribute__ ((__packed__));

struct PacketPdoErr{
    unsigned char dataType;
    unsigned char byte1;
    unsigned char byte2;
    unsigned char byte3;
    unsigned char byte4;
    unsigned char byte5;
    unsigned char byte6;
    unsigned char byte7;
}__attribute__ ((__packed__));

struct PacketOutlet{
    unsigned char dataType;
    unsigned char N3_P2_Power;
    unsigned short int N3_out_temp;
    unsigned short int N3_out_cond;
    unsigned short int N3_WP;//水路压
}__attribute__ ((__packed__));
struct PacketFluidB{
    unsigned char dataType;
    unsigned char N3_P3_Power;
    unsigned short int N3_B_temp;
    unsigned short int N3_B_cond;
    unsigned short int N3_B_Target;
}__attribute__ ((__packed__));
struct PacketMix{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N3_Mix_temp;
    unsigned short int N3_Mix_cond;
    unsigned short int N3_mix_Target;
}__attribute__ ((__packed__));
struct PacketFluidAfter{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N3_after_temp;
    unsigned short int N3_after_cond;
    unsigned short int noUse1;
}__attribute__ ((__packed__));

struct PacketUfValue{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N3_UF_amount;  //已超虑量
    unsigned short int N3_UF_target;  //超虑目标
    unsigned short int N3_UF_rate;//超滤速度
}__attribute__ ((__packed__));
struct PacketUfPump{
    unsigned char dataType;
    unsigned char N3_P1_Power;
    unsigned short int N3_Degass_P;
    unsigned short int N3_UFP_Rate;
    unsigned short int N3_UFP_Feedback;
}__attribute__ ((__packed__));
struct PacketHeat{
    unsigned char dataType;
    unsigned char N3_heat_Power;
    unsigned short int N3_heat_temp;
    unsigned short int N3_heat_Target;
    unsigned short int noUse;
}__attribute__ ((__packed__));

struct PacketBPump{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N3_BP_Stand;
    unsigned short int N3_BP_Now;
    unsigned short int N3_MixP_Stand;
}__attribute__ ((__packed__));

//struct PacketSubstitutionPump{
//    unsigned char dataType;
//    unsigned char noUse;
//    unsigned short int N3_BP_Stand;
//    unsigned short int N3_BP_Now;
//    unsigned short int N3_MixP_Stand;
//}__attribute__ ((__packed__));

struct PacketMixPump{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N3_MixP_Now;
    unsigned short int N3_balance_T;
    unsigned short int noUse1;
}__attribute__ ((__packed__));
struct PacketDisplay{
    unsigned char dataType;
    unsigned char N3_pos;
    unsigned char N3_display_letter[4];
    unsigned short int N3_display_data;
}__attribute__ ((__packed__));
struct PacketAntrum{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N3_A100;
    unsigned short int N3_B100;
    unsigned short int N3_C1000;
}__attribute__ ((__packed__));
struct PacketNowater{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N3_Nowater_T;
    unsigned short int noUse1;
    unsigned short int noUse2;
}__attribute__ ((__packed__));
struct PacketMasterCtrlSdo{
    unsigned char noUse;
    unsigned short int index;
    unsigned char indexSub;
    unsigned char dataSdo[4];
}__attribute__ ((__packed__));

struct PacketSdoSetup {
    unsigned char N3_set[4];
};
struct PacketSdoCmd {
    unsigned char cmd;
};

struct PacketSdoScale {
    unsigned short int N3_set_Bscale;
    unsigned short int N3_set_Wscale;
};
struct PacketSdoAim {
    unsigned short int N3_set_B;
    unsigned short int N3_set_mix;
};

struct PacketSdoBlood {
    unsigned short int N3_set_BPspeed;
    unsigned char N3_set_BPtube;
    unsigned char noUsed;
};
struct PacketSdoHeparin {
    unsigned short int N3_set_heparin_first;
    unsigned short int N3_set_heparin_speed;
};
struct PacketSdoRangeTemp {
    unsigned short int N3_set_temp_Up;
    unsigned short int N3_set_temp_Lower;
};

struct PacketPDOHeparinPump {
    unsigned short dose;
    unsigned short speed;
    unsigned short preStopTime;
};

/*
 * Monitor struct
 */
struct PacketWaterOut {
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N5_out_temp;//
    unsigned short int N5_out_cond;
    unsigned short int N5_leak;
};
struct PacketPress {
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N5_AP;
    unsigned short int N5_VP;
    unsigned short int N5_TMP;
};
struct PacketBloodState {
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N5_WP;
    unsigned short int N5_Blood_temp;
    unsigned short int N5_Oxygen;
};
struct PacketBPMState {
    unsigned char dataType;
    unsigned char N5_BPM_status;
    unsigned short int N5_BPM_SYS;
    unsigned short int N5_BPM_DIA;
    unsigned short int N5_BPM_MAP;
};
struct PacketBPMMisc {
    unsigned char dataType;
    unsigned char N5_BPM_unit;
    unsigned short int N5_BPM_pulse;
    unsigned short int N5_BPM_inflation;
    unsigned short int N5_Oxi_pulse;
};


/*
 * Power struct
 */

//struct PacketPower {
//    unsigned int canSt;
//    short adc0;
//    short adc1;
//    short adc2;
//    short adc3;
//    char state;
//    char batH;
//    char batL;
//    char charge;
//};


struct PacketBattry{
    unsigned char dataType;
    unsigned char notuse1;
    unsigned char BAT_state1;
    unsigned char BAT_state2;
    unsigned short int adc0;
    unsigned short int adc1;
  }__attribute__ ((__packed__));

struct PacketPower{
    unsigned char dataType;
    unsigned char notuse1;
    unsigned char powerstate1; //电量情况，是否充足
    unsigned char powerstate2; //充电情况，是否完成
    unsigned short int adc2; //正极电压采样值
    unsigned short int adc3; //负极电压采样值
  }__attribute__ ((__packed__));

struct PacketPowerAlarm{
    unsigned char dataType;
    unsigned char notuse1;
    unsigned char error;
    unsigned char notuse2;
    unsigned char notuse3;
    unsigned char notuse4;
    unsigned char notuse5;
    unsigned char notuse6;
  }__attribute__ ((__packed__));

struct PacketPowerMornitor{
    unsigned char dataType;
    unsigned char notuse1;
    unsigned char notuse2;
    unsigned char notuse3;
    unsigned char notuse4;
    unsigned char notuse5;
    unsigned char notuse6;
    unsigned char notuse7;
  }__attribute__ ((__packed__));

//struct PacketPower{

//        struct Packet1{
//            unsigned char dataType;
//            unsigned char notuse1;
//            unsigned char N7_BAT_state1;
//            unsigned char N7_BAT_state2;
//            unsigned short int N7_adc0;
//            unsigned short int N7_adc1;
//          }__attribute__ ((__packed__))packet1;

//        struct Packet2{
//            unsigned char dataType;
//            unsigned char notuse1;
//            unsigned char N7_state1;
//            unsigned char N7_state2;
//            unsigned short int N7_adc2;
//            unsigned short int N7_adc3;
//          }__attribute__ ((__packed__))packet2;

//        struct Packet3{
//            unsigned char dataType;
//            unsigned char notuse1;
//            unsigned char N7_error;
//            unsigned char notuse2;
//            unsigned char notuse3;
//            unsigned char notuse4;
//            unsigned char notuse5;
//            unsigned char notuse6;
//          }__attribute__ ((__packed__))packet3;

//        struct Packet4{
//            unsigned char dataType;
//            unsigned char notuse1;
//            unsigned char notuse2;
//            unsigned char notuse3;
//            unsigned char notuse4;
//            unsigned char notuse5;
//            unsigned char notuse6;
//            unsigned char notuse7;
//          }__attribute__ ((__packed__))packet4;
//}__attribute__ ((__packed__));

/*
 * Heparin Pump struct
 */
struct PacketHeparinPump {
    unsigned int canSt;
    unsigned short int inputAllValue;
    unsigned short int doseSpeed;
    unsigned char workMode;
    unsigned char workModeSub;
    unsigned char senserState;

    int quickSpeed;
    int syringeLen;
    int syringeStyle;
    int doseFirst;
    char selstatus;    //是否选中，1 yes 0 no
    char runstatus;    // 1 run 0 stop
};

/*
 * Blood Pump struct
 */
struct PacketBloodPump {   //为了累计量超过65535的限制定义的类型#/
    unsigned char dataType;
//    unsigned char reserve1;
    unsigned short int N9_speed;
//    unsigned short int N9_amount;//血泵累计量
    unsigned int N9_amount;//血泵累计量
    unsigned char N9_mode;
    unsigned char N9_status;//其他各个状态
};

struct PacketN9N10Pump {  //对应N9N10血泵发上来PDO的类型#/
    unsigned char dataType;
    unsigned char reserve1;
    unsigned short int N9_speed;
    unsigned short int N9_amount;//血泵累计量
    unsigned char N9_mode;
    unsigned char N9_status;//其他各个状态
};

struct PacketBloodPumpTube {
    unsigned char dataType;
    unsigned char      N9_set_tube;
    unsigned short int N9_set_speed;
    unsigned char NoUse4;
    unsigned char NoUse5;
    unsigned char NoUse6;
    unsigned char N9_Key;
};

struct PacketSubBloodPump {
    unsigned short int N10_set_speed;
    unsigned short int N10_goal;
    unsigned short int N10_amount;
    unsigned char N10_mode;
    unsigned char N10_status;
};

struct PacketN1State {
unsigned char N1_status;
unsigned char N1_blood_status;
unsigned char N1_sub_status;
unsigned char N1_water_status;
unsigned char N1_sub2_status;
unsigned char N1_subbit1_status;
unsigned char N1_subbit2_status;
};

/*
 * ui board send data to other board
 */

struct PackN1PDO01H
{
    unsigned char dataType;

    struct N1SetByte1
    {
        unsigned char SN_B :1;          //0,双针;1,单针
        unsigned char B_powder_B :1;    //0,非干粉;1,干粉
        unsigned char U_filter_B :1;    //0,非超净;1,超净
        unsigned char online_B :1;      //0,非online;1,online
        unsigned char KTV_B :1;         //0,非ktv;1,带ktv
        unsigned char Back_in_B :1;     //0,前置换;1,后置换
        unsigned char Acetate_B :1;     //0,非     1,醋酸盐
        unsigned char A_center_B :1;    //0,非    1,中央供A液
    }byte1;

    struct N1SetByte2
    {
        unsigned char pe :1;  //0,   1,B液中含Nacl
        unsigned char notuse1 :1;   //0,    1,热冲
        unsigned char notuse2 :1;  //0,  1,化(过氧乙酸)
        unsigned char notuse3 :1;    //0,  1,化(次氯酸钠)
        unsigned char notuse4:1;     //0,  1,化(柠檬酸)
        unsigned char notuse5 :1;         //0,  1,热(纯水)
        unsigned char notuse6 :1;    //0,  1,热(柠檬酸)
        unsigned char isouf :1;   //0,  1,ISO单超
    }byte2;

    struct N1SetByte3
    {
        unsigned char status_screen_B :1;   //0,非 1,状态界面
        unsigned char engeer_screen_B :1;   //0,非 1,工程界面
        unsigned char no_water_B :1;        //0,有水 1,无水
        unsigned char save_AB_B :1;         //0,非  1,省液
        unsigned char auto_limit_B :1;      //0,手动 1,自动卡标
        unsigned char SN_type_B :1;         //0,单针单泵 1,单针双泵
        unsigned char skip_test_B :1;       //0,自检 1,跳过自检
        unsigned char b7 :1;
    }byte3;

    struct N1SetByte4
    {
        unsigned char Function_Bpowder_B :1;   //0,1,带干粉功能
        unsigned char b1 :1;
        unsigned char b2 :1;
        unsigned char b3 :1;
        unsigned char b4 :1;
        unsigned char b5 :1;
        unsigned char b6 :1;
        unsigned char b7 :1;
    }byte4;

    unsigned char noUse[3];
};

struct PackN1PDO02H
{
    unsigned char dataType;
    unsigned char N1_set_BPtube;
    unsigned short int N1_set_temp;
    unsigned short int N1_set_Wspeed;
    unsigned short int N1_set_BPspeed;
}__attribute__ ((__packed__));

struct PackN1PDO03H
{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N1_set_Bscale;
    unsigned short int N1_set_Wscale;
    unsigned short int N1_set_B;
}__attribute__ ((__packed__));

struct PackN1PDO04H
{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N1_set_mix;
    unsigned short int N1_set_UFspeed;
    unsigned short int N1_set_UFtarget;
}__attribute__ ((__packed__));

struct PackN1PDO05H
{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N1_set_heparin_first;
    unsigned short int N1_set_heparin_speed;
    unsigned short int N1_set_temp_UP;
}__attribute__ ((__packed__));

struct PackN1PDO06H
{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N1_set_temp_DOWN;
    unsigned short int N1_set_AP_UP;
    unsigned short int N1_set_AP_DOWN;
}__attribute__ ((__packed__));

struct PackN1PDO07H
{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N1_set_VP_UP;
    unsigned short int N1_set_VP_DOWN;
    unsigned short int N1_set_TMP_UP;
}__attribute__ ((__packed__));

struct PackN1PDO08H
{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N1_set_TMP_DOWN;
    unsigned short int N1_set_Cond_UP;
    unsigned short int N1_set_Cond_DOWN;
}__attribute__ ((__packed__));

struct PackN1PDO09H
{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N1_set_QJspeed;
    unsigned short int N1_set_QJcond_UP;
    unsigned short int N1_set_QJcond_DOWN;
}__attribute__ ((__packed__));

struct PackN1PDO0AH
{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N1_set_QJtemp;//两字节
    unsigned short int N1_set_QJsuck;
    unsigned short int noUse1;
}__attribute__ ((__packed__));

struct PackN1PDO0BH
{
    unsigned char dataType;
    unsigned char noUse;
    unsigned short int N1_set_SubBPspeed;//两字节
    unsigned short int noUse1;
    unsigned short int noUse2;
}__attribute__ ((__packed__));

struct PackN1PDO0CH   //人机板PDO2对应的字节
{
    unsigned char dataType;

//    unsigned char N1_status;
//    unsigned char N1_blood_status;
//    unsigned char N1_sub_status;
//    unsigned char N1_water_status;
//    unsigned char N1_sub2_status;
//    unsigned char N1_subbit1_status;
//    unsigned char N1_subbit2_status;
  struct PacketN1State state;
}__attribute__ ((__packed__));


struct PackN15PDO
{
    //unsigned char dataType;
    unsigned short int inputAllValue;  //已注入总量
    unsigned short int doseSpeed;  //正常速度
    unsigned char workMode; //工作状态  01H:启动   02H:停止  03H:暂停  04H:自检  06
    unsigned char workModeSub; //子工作状态 具体看协议
    unsigned char senserState;  //限位开关状态
    unsigned char notuse;   //没定义

}__attribute__ ((__packed__));

struct bpmResult{
    unsigned short int systolic;   //心脏收缩压#/
    unsigned short int diastolic;  //心脏舒张压#/
    unsigned short int pulse;   //心跳#/
};

#ifdef __cplusplus
};
#endif

#endif
