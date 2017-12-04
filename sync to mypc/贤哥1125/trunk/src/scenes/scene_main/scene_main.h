#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

#include <QtGui>
#include <QWidget>
#include <QMainWindow>
#include <QMouseEvent>

#include <QTimer>
#include <QProgressBar>

#include "config_ui.h"
#include "lang_switch.h"
#include "page_info.h"
#include "widget_time.h"
#include "widget_cross.h"
#include "widget_measure.h"
#include "widget_turntable.h"
#include "widget_button.h"
#include "widget_aim_config.h"
#include "widget_switch.h"
#include "widget_check_self.h"
#include "data_timer.h"
#include "data_custom.h"

class SceneMain : public QObject, public PieceVisible, public LangSetText
,public timer_handle,public ActionCustom
{
    Q_OBJECT
public:
    SceneMain(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

    //WidgetSwitch widgetSwitch;

public slots:
    void SelfCheckFinish(bool flag);

private:
    void timer_handler(const int timeOut = 0);
    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void StartSelfCheckTimer(bool check);
private:
    WidgetMeasureAP widgetMeasureAP; //动脉压自动卡标 包括上下箭头按钮
    WidgetMeasureVP widgetMeasureVP; //静脉压自动卡标
    WidgetMeasureTMP widgetMeasureTMP; //跨膜压自动卡标
    WidgetMeasureCond widgetMeasureCond; //电导度自动卡标
    WidgetAimConfig widgetAimConfig;  //主界面上方按钮和相应的动作
    WidgetSwitch widgetSwitch;       //主界面下方按键和相应的动作
    WidgetCheckSelf widgetCheckSelf; //自检进度条
    WidgetPButtontMarkAP widgetPButtontMarkAP;//主页面的动脉压按钮
    WidgetPButtontMarkVP widgetPButtontMarkVP;//主页面的静脉压按钮
    WidgetPButtontMarkTMP widgetPButtontMarkTMP;//跨膜压按钮
    WidgetPButtontMarkCond widgetPButtontMarkCond;//电导度按钮
    QLabel topLevelLabel;

    int timeCount;
    bool isSelfcheck;
    bool bloodroadOk;
    bool waterroadOk;
    int checkfailinfo;    //自检失败信息
};

#endif
