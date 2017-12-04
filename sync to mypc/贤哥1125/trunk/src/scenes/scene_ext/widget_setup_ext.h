#ifndef WIDGET_SETUP_KTV_H
#define WIDGET_SETUP_KTV_H

#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QButtonGroup>
#include "config_ui.h"
#include "lang_switch.h"
#include "widget_button.h"
#include "page_info.h"
#include "scene_setup_bpm.h"
#include "scene_setup_oximeter.h"
#include "scene_setup_bloodtemp.h"
#include "scene_setup_advert_timer.h"
#include "scene_ktv.h"
#include "scene_ktv2.h"
#include "scene_bloodvolume.h"

#define Widget_Setup_Menu_X_KTV             12
#define Widget_Setup_Menu_Y_KTV             57
#define Widget_Setup_Menu_Width_KTV          147
#define Widget_Setup_Menu_High_KTV           696

class WidgetSetupExt : public QObject, public PieceVisible, public LangSetText
{
    Q_OBJECT
public:
    WidgetSetupExt(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);
    SceneSetupBPM sceneSetupBPM;
public slots:
    void SetupMenuActivate(int id);

private slots:
    void buttonShowClicked(int isHide=2);

private:
    inline void setUi();

private:
    SceneSetupSpO2 sceneSetupSpO2;     //血氧#/
    SceneSetupBloodTemp sceneSetupBloodTemp;   //血温#/
    SceneSetupAdvertTimer sceneSetupAdvertTime; //定时器#/
    SceneKTV sceneKTV;
    SceneKTV2 sceneKTV2;
    SceneBloodVolume sceneBloodVolume;  //

    QWidget widget;
    QLabel labelTitle;
    WidgetButton buttonListShow;

    QButtonGroup buttonGroupSetup;
    QGroupBox groupBox;

    WidgetButton buttonHeartPressure;
    WidgetButton buttonSpO2;
    WidgetButton buttonBloodTemp;
    WidgetButton buttonTimer;
    WidgetButton buttonKtV;
    WidgetButton buttonKtV2;
    WidgetButton buttonBloodVolume;
};

#endif
