#ifndef WIDGET_AIM_CONFIG_H
#define WIDGET_AIM_CONFIG_H

#include <QLabel>
#include "data_custom.h"
#include "config_ui.h"
#include "lang_switch.h"
#include "widget_button.h"
#include "data_timer.h"

#define WIDGETAIMCONFIG_WIDTH                   851
#define WIDGETAIMCONFIG_HIGH                    184

class WidgetAimConfig : public QWidget, public ActionCustom,public timer_handle,public LangSetText
{
    Q_OBJECT
public:
    WidgetAimConfig(QWidget *parent=0);
    bool setDialysisLiquid(const float value=0.0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

private:
    inline void setUi();

#if HJX_COMMUNICATION_SWITCH
signals:
    void signalSDO(int id);
#endif

public slots:
    void configHemoPump();
    void configSubsPump();
    void configTemp();
    void configUfTarget();

private:
    inline void WorkModeSwitch(const void* dataPtr);
    void timer_handler(const int timeOut = 0);
    void ActionHandle(const void* dataPtr=NULL, int name=0);

protected:
    void paintEvent(QPaintEvent *event);

private:
    float dialysisLiquid;

    QLabel labelISOText;
    QLabel labelISOData;

    QLabel labelHemoPump;
    QLabel labelSubsSpeed;
    QLabel labelDialysateTmp;
    QLabel labelUltrafiltrationTarget;
    WidgetButton widgetButtonBloodPump;   //血泵按钮#/
    WidgetButton widgetButtonSubsSpeed;   //置换泵或者ISO目标#/
    WidgetButton widgetButtonDialysateTemp;  //透析液#/
    WidgetButton widgetButtonUFTarget;  //超虑目标#/

    QPixmap *pixmapBack;
    QPixmap pixmapAimConfig;
    QPixmap pixmapAimConfigOnline;

private:
    struct PacketUF packetUF;
    struct PacketSdoBlood sdoBlood;
    PacketSubBloodPump SubBPump;
};

#endif
