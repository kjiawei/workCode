#ifndef WIDGET_SETUP_CURE_H
#define WIDGET_SETUP_CURE_H

#include <QGroupBox>
#include <QVBoxLayout>
#include <QButtonGroup>
#include "data_custom.h"
#include "config_ui.h"
#include "lang_switch.h"
#include "widget_button.h"
#include "page_info.h"
#include "scene_setup_isouf.h"
#include "scene_setup_heparin.h"
#include "scene_setup_hemopump.h"
#include "scene_setup_cure_time.h"
#include "scene_setup_subspump.h"
#include "scene_setup_pressure.h"
#include "scene_setup_dialysate.h"
#include "scene_setup_ultrafiltration.h"
#include "scene_setup_otheroper.h"

#define Widget_Setup_Menu_X             12
#define Widget_Setup_Menu_Y             57
#define Widget_Setup_Menu_Width         147
#define Widget_Setup_Menu_High          696

class WidgetSetupCure : public QObject, public PieceVisible, public ActionCustom, public LangSetText
{
    Q_OBJECT
public:
    WidgetSetupCure(QWidget *parent=0);
    void setCusVisible(bool vis=false, char* para=NULL);
    void BusinessWorkmodeSwitchHandle(const int dest=-1, const int src=-1);
    void CustomSetText();

private slots:
    void buttonShowClicked(int isHide=2);
    void SlotSceneActivate(int id=1);

private:
    inline void setUi();
    void ActionHandle(const void* dataPtr=NULL, int name=0);

private:
    SceneSetupCureTime sceneSetupCureTime; //治疗时间
    SceneSetupUf sceneSetupUf;             //超虑
    SceneSetupIsoUF sceneSetupIsoUF;       //单超
    SceneSetupHemopump sceneSetupHemopump;  //血泵
    SceneSetupHeparin sceneSetupHeparin;   //肝素泵
    SceneSetupPressure sceneSetupPressure;   //压力
    SceneSetupDialysate sceneSetupDialysate;  //透析夜
    SceneSetupSubspump sceneSetupSubspump;   //备用泵
    SceneSetupOherOper sceneSetupOtherOper;  //其他操作#/

    QWidget widget;
    QLabel labelTitle;
    WidgetButton buttonListShow;

    QButtonGroup buttonGroupSetup;
    QGroupBox groupBox;
    WidgetButton buttonCureTime;
    WidgetButton buttonUf;
    WidgetButton buttonHemopump;
    WidgetButton buttonHeparin;
    WidgetButton buttonPressure;
    WidgetButton buttonDialysate;
    WidgetButton buttonSubstitution;
    WidgetButton buttonIsoUF;
    WidgetButton buttonOtherOper;

    int menuActivate;
};

#endif
