#ifndef WIDGET_SETUP_OTHER_H
#define WIDGET_SETUP_OTHER_H

#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QButtonGroup>
#include "lang_switch.h"
#include "page_info.h"
#include "scene_setup_lang.h"
#include "scene_setup_sysinfo.h"
#include "scene_setup_network.h"
#include "scene_setup_system_time.h"
#include "scene_setup_autowakeup.h"
#include "../../RFCard/RFCard.h"

#include "widget_button.h"
#include "engineering_mode.h"

#define Widget_Setup_Other_Menu_X             12
#define Widget_Setup_Other_Menu_Y             57
#define Widget_Setup_Other_Menu_Width         147
#define Widget_Setup_Other_Menu_High          696


class WidgetSetupOther : public QObject, public PieceVisible, public LangSetText
{
    Q_OBJECT
public:
    WidgetSetupOther(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

private:
    inline void setUi();
    inline void setSubpageHide();

public slots:
    void SlotSceneActivate(int id);

private:
    SceneSetupLang sceneSetupLang;   //语言设置
    SceneSetupNetwork sceneSetupNetwork;  //网络设置  //HJX网络模块禁止#/
    SceneSetupSysinfo sceneSetupSysinfo; //软件版本信息
    SceneSetupSystemTime sceneSetupSystemTime; //系统时间设置
    SceneSetupAutoWakeUp sceneSetupAutoWakeUp; //自动开机
//    PatientMessage patientMessage;  //病人信息


    QWidget widget;
    QLabel labelTitle;

    QGroupBox groupBox;
    QButtonGroup buttonGroupSetup;
    WidgetButton buttonSystemTime;
    WidgetButton buttonSetupNetwork;
    WidgetButton buttonSetupLang;
    WidgetButton buttonSystemInfo;
    WidgetButton buttonMaintain;
    WidgetButton buttonStatusMap;
//    WidgetButton buttonRFCard;
    WidgetButton buttonUnit;
    WidgetButton buttonautoWakeUp;
    WidgetButton buttonEngineeringMode;  //进入工程模式设置界面#/

    QString title;
};

#endif
