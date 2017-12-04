#ifndef DATA_CAN_SEND_H
#define DATA_CAN_SEND_H

#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QSemaphore>
#include "data_custom.h"
#include "deal.h"
#include "data_set.h"



#define DE_CONNECTION_TYPE Qt::QueuedConnection  //信号连接类型#/

#define DE_SENDPDO00_FLAG  0
#define DE_SENDPDO01_FLAG  1
#define DE_SENDPDO02_FLAG  2
#define DE_SENDPDO03_FLAG  3
#define DE_SENDPDO04_FLAG  4
#define DE_SENDPDO05_FLAG  5
#define DE_SENDPDO06_FLAG  6
#define DE_SENDPDO07_FLAG  7
#define DE_SENDPDO08_FLAG  8
#define DE_SENDPDO09_FLAG  9
#define DE_SENDPDO0A_FLAG  10
#define DE_SENDPDO0B_FLAG  11
#define DE_SENDPDO0C_FLAG  12
#define DE_PDO_MAXNUM      13

#define DE_CMD_Checkself 1
#define DE_CMD_Pass_Checkself 2
#define DE_CMD_Stop_water 3
#define DE_CMD_Treat_start 4
#define DE_CMD_WaterReboot 5
#define DE_CMD_Treat_stop 6
#define DE_CMD_Treat_end 7
#define DE_CMD_Dry_start 8
#define DE_CMD_Dry_stop 9
#define DE_CMD_Disinfect_start 10
#define DE_CMD_Disinfect_stop 11
#define DE_CMD_BloodStart 12
#define DE_CMD_BloodStop 13
#define DE_CMD_ResetBloodAmount 14
#define DE_CMD_SubsStart 15
#define DE_CMD_SubsStop 16
#define DE_CMD_SubsAdd 17
#define DE_CMD_ResetSubsAmount 18
#define DE_CMD_HeparinPumpStartHaveFirst 19     //开启肝素泵，有首计量#/
#define DE_CMD_HeparinPumpStop 20   //肝素停止#/
#define DE_CMD_ResetHeparinPumpAmount 21  //清空肝素累计量#/
#define DE_CMD_UFStart 22
#define DE_CMD_UFStop 23
#define DE_CMD_ResetUFAmount 24
#define DE_CMD_Bypass 25
#define DE_CMD_Nobypass 26
#define DE_CMD_ClipStart 27
#define DE_CMD_ClipStop 28
#define DE_CMD_Clip2Start 29
#define DE_CMD_Clip2Stop 30
#define DE_SetFormulaB 31
#define DE_SetFormulaTarget 32
#define DE_SetHeparin 33
#define DE_SetRangeTemp 34
#define DE_CMD_Shutdown 35
#define DE_CMD_Reboot 36
#define DE_CMD_EEporm_read 37
#define DE_CMD_EEporm_write 38
#define DE_CMD_BMP_Start 39
#define DE_CMD_BMP_Stop 40
#define DE_CMD_Startpreflush 41
#define DE_CMD_Stoppreflush 42
#define DE_CMD_Startreturnblood 43
#define DE_CMD_Stopreturnblood 44
#define DE_CMD_Blood1EnableButton 45  //血泵1 启用物理开关#/
#define DE_CMD_Blood1DisableButton 46  //血泵1 禁止物理开关#/
#define DE_CMD_SubsDisableButton 47  //置换液泵 禁止物理开关#/
#define DE_CMD_SubsEnableButton  48  //置换液泵 启用物理开关#/
#define DE_CMD_HeparinPumpStartNoFirst 49  //开启肝素泵，无首计量#/
#define DE_CMD_OPENVALVE 50  //开启相应的电磁阀#/
#define DE_CMD_CLOSEVALVE 51  //关闭相应的电磁阀#/
#define DE_CMD_P1PUMPACTION 52  //控制P1泵动作#/
#define DE_CMD_P2PUMPACTION 53  //控制P2泵动作#/
#define DE_CMD_P3PUMPACTION 54  //控制P3泵动作#/
#define DE_CMD_P4PUMPACTION 55  //控制P4泵动作#/
#define DE_CMD_JP1PUMPACTION 56  //控制JP1泵动作#/
#define DE_CMD_JP2PUMPACTION 57  //控制JP2泵动作#/
#define DE_CMD_JP3PUMPACTION 58  //控制JP3泵动作#/
#define DE_CMD_BALANCEACTION 59  //控制平衡腔动作#/
#define DE_CMD_HEATINGACTION 60  //控制加热腔动作#/
#define DE_CMD_DEGASSINGPUMP 61  //控制除气腔动作#/
#define DE_SDO_FORMULA_STARTTEST 62 //配方测试启动
#define DE_SDO_FORMULA_STOPTEST 63 //配方测试停止
#define DE_SDO_DIALYSATE_STARTTEST 64 //透析夜测试启动
#define DE_SDO_DIALYSATE_STOPTEST 65 //透析夜测试停止
#define DE_SDO_APUMP_STARTTEST 66 //A液泵测试
#define DE_SDO_APUMP_STOPTEST 67 //
#define DE_SDO_BPUMP_STARTTEST 68 //B液泵测试
#define DE_SDO_BPUMP_STOPTEST 69 //
#define DE_SDO_TEMP_STARTTEST 70 //温度测试
#define DE_SDO_TEMP_STOPTEST 71 //
#define DE_SDO_COND_STARTTEST 72 //电导测试
#define DE_SDO_COND_STOPTEST 73 //
#define DE_SDO_UF_STARTTEST 74 //超滤泵测试
#define DE_SDO_UF_STOPTEST 75 //



#if HJX_COMMUNICATION_SWITCH==2

//PDO数据结构为 8个字节#/
extern PackN1PDO01H uiPDO01; //将UI的运行状态和工作模式设置发送给主控板#/
extern PackN1PDO02H uiPDO02; //血泵速度和透析夜流速#/
extern PackN1PDO03H uiPDO03;
extern PackN1PDO04H uiPDO04;
extern PackN1PDO05H uiPDO05;
extern PackN1PDO06H uiPDO06;
extern PackN1PDO07H uiPDO07;
extern PackN1PDO08H uiPDO08;
extern PackN1PDO09H uiPDO09;
extern PackN1PDO0AH uiPDO0A;
extern PackN1PDO0BH uiPDO0B;

class DataCanSend:public QThread,public ActionCustom
{
    Q_OBJECT

public:
    DataCanSend(QObject *parent=0);
    void init();
    bool test_mutex(bool vis=false);   //测试死锁的函数#/

public slots:
    void AppendPDOData(int id);
    void sendSDOData(int id);
    void sendSDOData2(int id,unsigned int vol1=0,unsigned int vol2=0);
    void timerHandle();

private:
    void run();
    void send_PDO_handle(int id);
    void send_SDO_handle(int id,unsigned int vol1=0,unsigned int vol2=0);
    void try_CanSend_Mutex();
    void ActionHandle(const void* dataPtr=NULL, int name=0);

private:
    /*
    bool G_BPPUMP_OPEN;  //血泵打开命令标志#/
    bool G_SUBPUMP_OPEN; //置换泵打开命令标志#/
    bool G_UFPUMP_OPEN;  //超滤打开命令标志#/
    bool G_HPPUMP_OPEN;  //肝素打开命令标志#/
    bool G_VPCLIP_OPEN;  //静脉夹打开命令标志#/
    */

    QTimer sendPDOTimer;

    unsigned char PDO_LIST[DE_PDO_MAXNUM];  //PDO发送操作队列#/
//    QList<int>SDO_LIST;  //SDO发送操作队列#/

    //各个设备和设置对应的数据结构，进行相应数据保存#/
    //当相应的数据管理包执行相应的操作时，则将这些结构的内容读到上面#/
    //相应的PDO结构中，然后发送该PDO#/
    PacketSdoBlood SdoBlood;
    PacketFluid packetFluid;
    FormulaSet formulaSet;
    PacketUF packetUF;
    PacketPDOHeparinPump packetPDOHeparinPump;
    PacketAP packetAP;
    PacketVP packetVP;
    PacketTMP packetTMP;
    PacketCond  packetCond;
    PacketSubBloodPump packetSubBloodPump ;
    PacketWorkMode packetWorkMode;
    PacketN1State  packetN1State;
    PacketPDOHeparinPump HeparinPump;//肝素泵#/
    PacketFluid fluidTemp;
    PacketAP APTemp; //动脉压#/
    PacketVP VPTemp; //静脉压#/
    PacketTMP TMPTemp;
    PacketCond CONTemp;
    FormulaSet formulaSetTemp;
    PacketUF ufTmp;
    PacketSubBloodPump subBPTmp;
    //PacketN1State  N1State;
};

#elif HJX_COMMUNICATION_SWITCH==1

class DataCanSend:public QThread,public ActionCustom
{
    Q_OBJECT

public:
    DataCanSend(QObject *parent=0);
    void init();

private slots:
    void sendPDOData(int id);
    void sendSDOData(int id);
    void timerHandle();

private:
    void run();
    void send_PDO_handle(int id);
    void send_SDO_handle(int id);
    void try_PDO_mutex();
    void try_SDO_mutex();
    void ActionHandle(const void* dataPtr=NULL, int name=0);

private:    
    QTimer sendPDOTimer;

    QList<int>PDO_LIST;  //PDO发送操作队列#/
    QList<int>SDO_LIST;  //SDO发送操作队列#/
    //所有结构变量均一一对应协议文件中底层设备中的变量#/
    //详情参考 /pulic/protocols/中的协议文件#/
    //N1：表示发送该PDO的节点设备号为1的设备节点，#/
    //01H：对应第一个字节的值，即协议文件中的BYTE0的值#/
    //PDO数据结构为 8个字节#/
    PackN1PDO01H uiPDO01; //将UI的运行状态和工作模式设置发送给主控板#/
    PackN1PDO02H uiPDO02; //血泵速度和透析夜流速#/
    PackN1PDO03H uiPDO03;
    PackN1PDO04H uiPDO04;
    PackN1PDO05H uiPDO05;
    PackN1PDO06H uiPDO06;
    PackN1PDO07H uiPDO07;
    PackN1PDO08H uiPDO08;
    PackN1PDO09H uiPDO09;
    PackN1PDO0AH uiPDO0A;
    PackN1PDO0BH uiPDO0B;
//    PackN1PDO0CH uiPDO0C;

    //各个设备和设置对应的数据结构，进行相应数据保存#/
    //当相应的数据管理包执行相应的操作时，则将这些结构的内容读到上面#/
    //相应的PDO结构中，然后发送该PDO#/
    PacketSdoBlood SdoBlood;
    PacketFluid packetFluid;
    FormulaSet formulaSet;
    PacketUF packetUF;
    PacketPDOHeparinPump packetPDOHeparinPump;
    PacketAP packetAP;
    PacketVP packetVP;
    PacketTMP packetTMP;
    PacketCond  packetCond;
    PacketSubBloodPump packetSubBloodPump ;
    PacketWorkMode packetWorkMode;
    PacketN1State  packetN1State;
    PacketPDOHeparinPump HeparinPump;//肝素泵#/
    PacketFluid fluidTemp;
    PacketAP APTemp; //动脉压#/
    PacketVP VPTemp; //静脉压#/
    PacketTMP TMPTemp;
    PacketCond CONTemp;
    FormulaSet formulaSetTemp;
    PacketUF ufTmp;
    PacketSubBloodPump subBPTmp;
    //PacketN1State  N1State;
};
#elif HJX_COMMUNICATION_SWITCH==0
class DataCanSend : public QObject,public ActionCustom
{
    Q_OBJECT

public:
    DataCanSend(QObject *parent=0);
    //将几乎所有涉及底层通信的数据包管理类，绑定了本类的ActionHandle#/
    //为其操作处理#/
    void init();

private slots:
    //N1（主机）发送相关的所有PDO#/
    //主控板，血泵等子设备则对接收到的PDO进行处理#/
    //主要是根据 dataType这个字段识别是否应该是自己处理的#/
    void sendPDOData();

private:
    //实现了几乎所有上层UI对底层通信的操作#/
    void ActionHandle(const void* dataPtr=NULL, int name=0);

private:
    PacketPDOHeparinPump HeparinPump;//肝素泵#/
    PacketFluid fluidTemp;
    PacketAP APTemp; //动脉压#/
    PacketVP VPTemp; //静脉压#/
    PacketTMP TMPTemp;
    PacketCond CONTemp;
    FormulaSet formulaSetTemp;
    PacketUF ufTmp;

    PacketSubBloodPump subBPTmp;

    //所有结构变量均一一对应协议文件中底层设备中的变量#/
    //详情参考 /pulic/protocols/中的协议文件#/
    //N1：表示发送该PDO的节点设备号为1的设备节点，#/
    //01H：对应第一个字节的值，即协议文件中的BYTE0的值#/
    //PDO数据结构为 8个字节#/
    PackN1PDO01H uiPDO01; //将UI的运行状态和工作模式设置发送给主控板#/
    PackN1PDO02H uiPDO02; //血泵速度和透析夜流速#/
    PackN1PDO03H uiPDO03;
    PackN1PDO04H uiPDO04;
    PackN1PDO05H uiPDO05;
    PackN1PDO06H uiPDO06;
    PackN1PDO07H uiPDO07;
    PackN1PDO08H uiPDO08;
    PackN1PDO09H uiPDO09;
    PackN1PDO0AH uiPDO0A;
    PackN1PDO0BH uiPDO0B;
    PackN1PDO0CH uiPDO0C;

    QTimer sendPDOTimer;

    QMutex mutex;

    //各个设备和设置对应的数据结构，进行相应数据保存#/
    //当相应的数据管理包执行相应的操作时，则将这些结构的内容读到上面#/
    //相应的PDO结构中，然后发送该PDO#/
    PacketSdoBlood SdoBlood;
    PacketFluid packetFluid;
    FormulaSet formulaSet;
    PacketUF packetUF;
    PacketPDOHeparinPump packetPDOHeparinPump;
    PacketAP packetAP;
    PacketVP packetVP;
    PacketTMP packetTMP;
    PacketCond  packetCond;
    PacketSubBloodPump packetSubBloodPump ;
    PacketWorkMode packetWorkMode;
    PacketN1State  packetN1State;
    //PacketN1State  N1State;

};
#endif

#endif // DATA_CAN_SEND_H
