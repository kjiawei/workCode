#ifndef WIDGET_NOTICE_H
#define WIDGET_NOTICE_H

#include <QLabel>
#include <QTimer>
#include <QProgressBar>
#include "widget_button.h"
#include "data_custom.h"

#include "data_timer.h"
#include "lang_switch.h"

#define WIDGET_NOTICE_X                 874
#define WIDGET_NOTICE_Y                 62
#define WIDGET_NOTICE_WIDTH             139
#define WIDGET_NOTICE_HIGH              134

class WidgetNotice : public QObject,public ActionCustom, public timer_handle
 , public LangSetText
{
    Q_OBJECT

public:
    WidgetNotice(QWidget *parent=0);
    void CustomSetText();

    void setHeaderStatus(int code);  //设置总状态
    void setBloodStatus(int code);   //设置工作模式
    void setWaterStatus(int code);   //水路状态

    void showtipState(int code);   //显示当前主状态
    void showsubState(int code);   //显示当前子状态    

private:
    inline void setUi();

private slots:
    void slotSoundSwitch(void);
//    void slotTimerOut();

private:
    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void timer_handler(const int timeOut = 0);
    void CancelAlarmSilence();  //取消消音操作#/

private:
    QWidget widgetBattery;
    WidgetButton buttonSound;
    QProgressBar progressSound;   //消音按键冻结2分钟的进度条
    //总状态
    QLabel labelHeaderStatus;
    //血路状态
    QLabel labelBloodStatus;
    //水路状态
    QLabel labelWaterStatus;

    WorkState workstate;
    bool preflush_model;
    QMap<int,AlarmInfor> curAlarmSave;  //保存当前报警，为了实现消音后，有新报警来时恢复报警音#/

private:
    int timeCount;

    int iHeaderStatus;
    int iBloodStatus;
    int iWaterStatus;

    QString strHeaderStatus;
    QString strBloodStatus;
    QString strWaterStatus;
};

#endif
