#ifndef BUSINESS_H
#define BUSINESS_H

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <QHash>
#include <QThread>
#include <QTimer>
#include <QtGui>
using namespace std;
#include "data_custom.h"

//总状态 N1_status#/
#define    START_INIT                   0x00               //启动初始化#/
#define    TREAT_READY               0x01               //准备状态#/
#define    TREAT_RUNNING           0x03              //治疗状态#/
#define    TREAT_FINISH               0x05              //治疗结束状态#/
#define    CLEANING                     0x07              //清洁状态#/
#define    DEBUG                    0x09     //调试状态#/
#define    DRAIN                    0x0a    //排水状态#/

//子状态 N1_sub_status#/
//血路状态 N1_blood_status#/
#define    FIRST_INTERFACE          0x01            //
#define    SECOND_INTERFACE     0x02
#define    TEST_SKIP                     0x03
#define    TESTING1                      0x11
#define    TESTING2                      0x12
#define    UF_TIME_NOT_COMPLETE  0x35

#define    COOL_RINSE           0x70//冷冲洗#/
#define    HOT_RINSE            0x71//热冲洗#/
#define    CHEMICAL_CITRICACID  0x72//化消柠檬酸#/
#define    CHEMICAL_PERACETIC   0x73//化消过氧乙酸#/
#define    CHEMICAL3            0x74//化学消毒3#/
#define    HOT_CHEMICAL         0x75//热+柠檬酸消毒#/
#define    HOT                  0x76 //纯热消毒#/


//水路状态 N1_water_status#/
#define     WATER_READY             0x21
#define     CLEANING_READY          0x70            //
#define     PRERINSE                0x71 //前冲洗#/
#define     SUCK                    0x72//吸消毒液#/
#define     HOLD                    0x73  //滞留#/
#define     LOOP                    0x74  //循环消毒#/
#define     AFTERRINSE              0x75 //后冲洗#/
#define     COLDRINSE               0x76 //独立冷冲洗#/
#define     HOTRINSE1               0x77 //热冲洗（阶段1）#/
#define     HOTRINSE2               0x78//热冲洗（阶段2）#/
#define     HOTRINSE3               0x79//热冲洗（阶段3）#/
#define     DRY                     0x7A   //清洁后排空#/
#define     RINSEPIPE               0x7B  //冲洗进水管#/


//子状态 N1_sub_status#/
#define     SUB_STATE_NULL                               0x00           //子状态为空，如上电时#/
#define     SELFTEST                                           0x11            //自检#/
#define     SELFTEST_COMPLETE_FLUID             0x13            //自检完成，有液#/
#define     SELFTEST_COMPLETE_NO_FLUID       0x14           //自检完成，无液#/
#define     PREFLUSH_RUN_FLUID                      0x15           //预冲运行状态，有液#/
#define     PREFLUSH_RUN_NO_FLUID                0x16           //预冲运行状态，无液#/
#define     PREFLUSH_STOP_FLUID                     0x17           //预冲暂停状态，有液#/
#define     PREFLUSH_STOP_NO_FLUID              0x18           //预冲暂停状态，无液#/
#define     PREFLUSH_COMPLETE_FLUID            0x19           //预冲完成状态，有液#/
#define     PREFLUSH_COMPLETE_NO_FLUID     0x1A           //预冲完成状态，无液#/
#define     TREAT_ENABLE_FLUID                        0x1B           //有血有液可以进入治疗#/
#define     TREAT_ENABLE_NO_FLUID                 0x1C           //有血无液可以进入治疗#/
#define     GETBLOOD_RUN_NO_FLUID    0x1d     //引血 无液#/
#define     GETBLOOD_RUN_FLUID       0x1e   //引血 有液#/
#define     GETBLOOD_STOP_NO_FLUID   0x1f   //引血暂停 无液#/
#define     GETBLOOD_STOP_FLUID      0x20   //引血暂停 有液#/

#define     CLEANING_RUN               0x70
#define     CLEANING_STOP              0x71
#define     CLEANING_FINISHED          0x72

#define     CURE_ALARM_PAUSE           0x30    //治疗过程中，报警引起暂停,暂时不用#/

#define     CURE_BLOODRETURN_RUN                                        0x52   //回血状态，有血#/
#define     CURE_BLOODRETURN_RUN_NO_BLOOD                    0x53   //回血状态，无血#/
#define     CURE_BLOODRETURN_STOP                                      0x54   //回血停止，有血#/
#define     CURE_BLOODRETURN_STOP_NO_BLOOD                   0x55   //回血停止，无血#/
#define     DRAIN_RUN                                                                 0x56   //排空状态#/
#define     DRAIN_STOP                                                                0x57   //排空停止#/
#define     CURE_BLOODRETURN_RUN_DRAIN                             0x58   //排空后的，回血状态，有血#/
#define     CURE_BLOODRETURN_RUN_DRAIN_NO_BLOOD         0x59   //排空后的，回血状态，无血#/
#define     CURE_BLOODRETURN_STOP_DRAIN                           0x5A   //排空后的，回血状态，有血#/
#define     CURE_BLOODRETURN_STOP_DRAIN_NO_BLOOD        0x5B   //排空后的，回血状态，无血#/

#define     DISINFECT_STOP  0x71

//
#define     BLOOD_PUMP_RUN        0x01      //血泵运行状态#/
#define     BLOOD_PUMP_STOP       0x02      //血泵停止状态#/
#define     BLOOD_PUMP_HALT        0x03     //血泵暂停状态#/

#define     MANUAL_PREFLUSH_MODE    0X00 //手动预冲模式#/
#define     AUTO_PREFLUSH_MODE  0X01  //自动预冲#/
#define     ONLINE_PREFLUSH_MODE 0X02 //在线自动预冲#/
#define     GETBLOOD_MODE   0X03  //引血模式#/
#define     ONLINE_GETBLOOD_MODE 0X04 //在线引血#/

#define     RETURNBLOOD_MODE 0X00  //非在线回血#/
#define     ONLINE_RETURNBLOOD_MODE 0X01  //在线回血#/

//重设标志#/
#define  DE_KEEP_PARAMETERS    0x00   //保留参数#/
#define  DE_RESET_PARAMETERS   0x01   //清空恢复默认参数#/

//旁路按键标志#/
#define DE_BYPASS_NOCHECKED     0X00   //旁路按钮未点击#/
#define DE_BYPASS_CHECKED       0X01   //旁路按钮已点击#/
#define DE_BYPASS_MASTERPASS    0X02   //主控板旁路状态#/


class StateUnit
{
public:
    StateUnit(char *name=NULL) {
        SetName(name);
    }
    virtual ~StateUnit() {}

    void SetName(char *name=NULL) {
        if ((name != NULL) && (strlen(name)<20)) {
            strcpy(dataName, name);
        }
    }
    char *ReadName() {
        return dataName;
    }

    //virtual void ActionEvent(void)=0;
    //virtual void ActionPause(void)=0;
    virtual void ActionEnter(void)=0;
    virtual void ActionExit(void)=0;

private:
    /* State Name */
    char dataName[20];
};

/*!
 * \brief The StateManage class
 */
class StateManage : public QThread,public ActionCustom
{
    Q_OBJECT
public:
    StateManage(QObject* parent=0);
    ~StateManage();
    bool Stop() {
        if (running == false) {
            return false;
        }
        running = false;
        return true;
    }

    bool StateSwitch(QString name=NULL);

    void run();

    void upDatePDOstate(void);

    /*****************************/
    WorkState GetWorkState(void){return workstate;}//返回状态结构体#/
    bool SetWorkState(WorkState * state){//把输入状态state保存在wrokstate，同时更新上一次状态和当前状态#/
        if(state->tipState.current != workstate.tipState.current){
            workstate.tipState.previous = workstate.tipState.current;
            workstate.tipState.current = state->tipState.current;
        }
        if(state->bloodState.current != workstate.bloodState.current){
            workstate.bloodState.previous = workstate.bloodState.current;
            workstate.bloodState.current = state->bloodState.current;
        }
        if(state->waterState.current != workstate.waterState.current){
            workstate.waterState.previous = workstate.waterState.current;
            workstate.waterState.current = state->waterState.current;
        }

        if(state->subState.current != workstate.subState.current){
            workstate.subState.previous = workstate.subState.current;
            workstate.subState.current = state->subState.current;
        }
        PacketWorkMode workModeData;
        dataRead(PACKET_NAME_WORKMODE,&workModeData);
        workstate.workMode=workModeData.treatmode.current;
        /*
        if(state->workMode != workstate.workMode){
            workstate.workMode = state->workMode;

        }
        */

        if(state->disinfectEnable != workstate.disinfectEnable){
            workstate.disinfectEnable = state->disinfectEnable;
        }
        upDatePDOstate();
        dataNodeUpdate(PACKET_NAME_WORKSTATE,&workstate);
        return true;
    }
    /*****************************/
public:
    WorkState  workstate;
private:
    void ActionHandle(const void* dataPtr=NULL, int name=0);
private slots:
    void StateFeedback();
private:
    QThread *FatherThread;
    bool running;
    QString NameLast;
    QString NameCurrent;
    QHash<QString, StateUnit*> hashState;
//    QTimer monitor;
    int   secpass ;
//    QTime timePoint;
    //WorkState  workstate;
    PacketStatusNote feedbackState;
};

WorkState GetWorkStateManage(void);
bool SetWorkStateManage(WorkState * state);

void StateManageStart();
void StateManageStop();
bool StateSwitch(const QString name=NULL);
#endif
