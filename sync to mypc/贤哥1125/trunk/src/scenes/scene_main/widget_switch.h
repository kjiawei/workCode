#ifndef WIDGET_SWITCH_H
#define WIDGET_SWITCH_H

#include <QLabel>
#include <QButtonGroup>
#include "lang_switch.h"
#include "widget_button.h"
#include "data_custom.h"
#include "data_timer.h"
#include "widget_time.h"
#include "widget_notice.h"

#include "bloodvolume_task.h" //添加启动血容计算任务#/

#define WIDGETSWITCH_WIDTH                          813
#define WIDGETSWITCH_HIGH                           68

#define FLUID_EMPTY  0  //无液#/
#define FLUID_OK        1  //有液#/

#define PRIME_STOP_FROM_COMPLETE            0 //预冲完成#/
#define PRIME_STOP_FROM_PUMP                    1//暂停预冲#/
#define PRIME_STOP_FROM_BLOOD                  2//有血，停止预冲#/

#define WIDGETSWITCH_BUTTON_PRESHOOT_X              0
#define WIDGETSWITCH_BUTTON_PRESHOOT_Y              0
#define WIDGETSWITCH_BUTTON_PRESHOOT_WIDTH          162

#define WIDGETSWITCH_BUTTON_CURE_X                  (WIDGETSWITCH_BUTTON_PRESHOOT_X+\
        WIDGETSWITCH_BUTTON_PRESHOOT_WIDTH)
#define WIDGETSWITCH_BUTTON_CURE_Y                  0
#define WIDGETSWITCH_BUTTON_CURE_WIDTH              164

#define WIDGETSWITCH_BUTTON_DIALYSISPUMP_X          (WIDGETSWITCH_BUTTON_CURE_X+\
        WIDGETSWITCH_BUTTON_CURE_WIDTH)
#define WIDGETSWITCH_BUTTON_DIALYSISPUMP_Y          0
#define WIDGETSWITCH_BUTTON_DIALYSISPUMP_WIDTH      163

#define WIDGETSWITCH_BUTTON_HEPARINPUMP_X           (WIDGETSWITCH_BUTTON_DIALYSISPUMP_X+\
        WIDGETSWITCH_BUTTON_DIALYSISPUMP_WIDTH)
#define WIDGETSWITCH_BUTTON_HEPARINPUMP_Y           0
#define WIDGETSWITCH_BUTTON_HEPARINPUMP_WIDTH       164

#define WIDGETSWITCH_BUTTON_SIDEROAD_X              (WIDGETSWITCH_BUTTON_HEPARINPUMP_X+\
        WIDGETSWITCH_BUTTON_HEPARINPUMP_WIDTH)
#define WIDGETSWITCH_BUTTON_SIDEROAD_Y              0
#define WIDGETSWITCH_BUTTON_SIDEROAD_WIDTH          159


class MsgBox: public QMessageBox//,public QObject
{
 protected:
 void showEvent(QShowEvent *event)
 {
    QMessageBox::showEvent(event);

    QWidget  *textField = findChild<QWidget*>("qt_msgbox_label");
    if(textField  != NULL)
    {
          textField ->setMinimumSize(200, 60);
    }
    QMessageBox::showEvent(event);
 }
};

class WidgetSwitch : public QWidget, public LangSetText,public ActionCustom
        ,public timer_handle
{
    Q_OBJECT

public:
    WidgetSwitch(QWidget *parent=0);
    void CustomSetText();
    BloodVolumeThread bloodVolumeThread;

private:
    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void timer_handler(const int timeOut = 0);
    void prime_run( const char model);
    void prime_stop(const char model,const char primeState);
    void getblood_run();  //引血开始#/
    void getblood_stop(); //引血暂停#/
    void treatment_run(); //治疗开始#/
    void treatment_stop();
    void BP_start(void);    //启动血泵#/
    void BP_stop(void);     //停止血泵#/
    void SUB_start(void);  //启动置换泵#/
    void SUB_stop(void);   //停止置换泵#/
    void UF_start(void);    //启动超滤泵#/
    void UF_stop(void);     //停止超滤泵#/
    void DRY_start(void);   //启动排空#/
    void DRY_stop(void);    //停止排空#/
    void bypass();           //旁路#/
    void nobypass();        //取消旁路#/
    void HeparinPump_run(); //启动肝素泵#/
    void HeparinPump_stop();//停止肝素泵#/

#if HJX_COMMUNICATION_SWITCH
signals:
    void signalSDO(int id);
#endif

private slots:
    void ClickedPreShoot(void);
    void ClickedCure(void);
    void ClickedUFPump(void);    
    void ClickedHeparinPump(void);
    void ClickedSideWay(void);
    void buttonsHandle(int key);
    void NoteSel(int sel);
    void ReciveCureNote(int id);
    void ReciveCureLeaveNote(int id);
    void ReturnToReady(int sel);  //返回准备状态#/
    void ConfirmSelect(int sel); //是否继续排空或回血#/
    void ConfirmGetBlood(int sel);  //开始引血#/
    void ConfirmStartDrain(int sel);//确认是否开始排空#/

signals:
    void RstLockSig(int flag);

private:
    bool statusPreShoot;   //预冲状态#/
    bool G_BPPUMP_STATUS;  //血泵状态#/
    bool G_SUBPUMP_STATUS;  //置换泵状态#/
    bool G_UFPUMP_STATUS;//超滤泵状态#/
    bool G_HEPARINPUMP_STATUS; //肝素泵状态#/
    bool statusSideWay; //旁路状态#/
    bool getbloodstartflag;   //第一次引血开始标志#/
    char blockbloodflag;   //阻塞标志#/
    bool blood2enableflag;   //血泵2使能标志
    bool isSelfcheckstart;   //自检跳过页面是否已经结束
    WidgetButton widgetButtonPrime;//预冲#/
    WidgetButton widgetButtonTreat;//治疗#/
    WidgetButton widgetButtonUFPump;//超滤泵#/
    WidgetButton widgetButtonHeparinPump;//肝素泵#/
    WidgetButton widgetButtonBypass;//旁路#/

    int bloodroadOk;
    int waterroadOk;
    int bubble;//气泡#/
    int bloodlevel;//液位#/
    enum PreflushModel preflush_model;  //预冲模式#/
    int timeCount;
    int curworkstate;//当前的工作状态#/
    int preworkstate;//先前的工作状态#/
    PacketPreflush Preflush;
    WorkState L_workstate;
    PacketWorkMode workModeData;
    int Timer_state;
    char handle_count;   //联动延时计数器#/
    int interactBp_ReturnBlood;  //回血标志#/
private:
    //struct Params *params;
    PacketSdoBlood sdoBlood;
    PacketFluid packet;

    bool isFlashing;
    bool isFlashShow;
    bool connectstatus;
    PacketStatusNote pNote;
    //MsgBox msgBox;
    bool check_alarm;

    PacketStatusAlarm alarmState;
    int scaleVar;
    PacketUfValue packetUfValue;
};



#endif
