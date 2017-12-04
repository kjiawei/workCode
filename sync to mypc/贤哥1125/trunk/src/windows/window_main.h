#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QVector>
#include <QWidget>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPushButton>

#include "config_ui.h"
#include "widget_ims.h"
#include "widget_time.h"
#include "widget_warn.h"
#include "widget_clock.h"
#include "widget_notice.h"
#include "widget_cross.h"
#include "widget_measure.h"
#include "widget_heartbeat.h"
#include "widget_turntable.h"
#include "widget_aim_config.h"
#include "widget_setup_cure.h"
#include "widget_setup_advanced.h"
#include "widget_setup_other.h"

#include "scene_main.h"
#include "scene_ktv.h"
#include "scene_ktv2.h"
#include "scene_history.h"
#include "scene_summary.h"
#include "scene_disinfect.h"
#include "scene_setup_workmode.h"
#include "scene_maintain.h"
#include "window_status_map.h"
#include "widget_setup_ext.h"

#include "page_info.h"
#include "widget_common.h"
#include "../debug_mode/widget_debug_mode.h"


class WindowMain : public QWidget, public PieceVisible
{
    Q_OBJECT

public:
    WindowMain(QWidget *parent=0);
    void setCusVisible(bool vis=false, char* para=NULL) {
        para = para;
        setVisible(vis);
    }

private:
    //所有的相关显示页面#/
    Widget_Debug_Mode c_Debug_Mode;     //调试模块#/
    SceneMain sceneMain;                //主页面显示-》自动卡标、上下界面按钮、自检进度条#/
    SceneDisinfect sceneDisinfect;      //消毒页面#/
    SceneHistory sceneHistory;          //报警记录页面-》当前报警记录、历史报警记录#/
    SceneSummary sceneSummary;          //第二主界面 点击主页面按钮后出现的信息页面的各文字标签label#/
    SceneSetupWorkmode sceneSetupWorkmode;  //工作模式#/
    WidgetIms widgetIms;                //信息管理页面-》运行日志、治疗日志、报警日志，病例库#/
    WidgetSetupCure widgetSetupCure;    //治疗参数页面-》治疗时间、超虑、血泵、肝素泵等#/
    WidgetSetupAdvanced widgetSetupAdvanced; //高级设置页面-》电导设置、钠和超滤曲线设置、预冲设置#/
    WidgetSetupOther widgetSetupOther;  //其他设置页面-》系统时间、网络、语言设置、软件版本等#/
    WidgetSetupExt  widgetSetupExt;  //扩展功能页面-》BPM、血容、血氧、KTV等#/

private:
    //主页面上的操作控件#/
    WidgetWarn widgetWarn;      //主页面右上角的告警按钮#/
    WidgetNotice widgetNotice;  //主页面右侧状态信息栏内容和报警消音按钮#/
    WidgetTime widgetTime;      //右侧治疗时间按钮#/
    WidgetClock widgetClock;    //右下方系统时间#/

    WidgetHeartbeat widgetHeartbeat;//心跳#/
    WidgetTurntable widgetTurntable;//刻度盘#/
    SceneMaintain sceneMaintain;//维护#/
public:
    WidgetCross widgetCross;  //菜单选择按钮#/
private:
    DataCustom *nodeSceneSw;
    PacketSceneSw packetSceneSw;
};

#endif // MAIN_WINDOW_H
