#ifndef SCENE_MAINTAIN_H
#define SCENE_MAINTAIN_H

#include <QtGui>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QButtonGroup>
#include "config_ui.h"
#include "lang_switch.h"
#include "page_info.h"
#include "widget_button.h"

#include "data_custom.h"



#include "scene_maintain_project_usbstroage_button.h"
#include "scene_maintain_config_language.h"
#include "scene_maintain_project_eeprom.h"

#define Widget_Setup_Other_Menu_X             12
#define Widget_Setup_Other_Menu_Y             57
#define Widget_Setup_Other_Menu_Width         147
#define Widget_Setup_Other_Menu_High          696

class SceneMaintain : public QWidget, public PieceVisible, public LangSetText
        , public timer_handle, public ActionCustom
{
    Q_OBJECT
public:
    SceneMaintain(QWidget *parent=0);
    static bool isUsbStroageExist;
    void CustomSetText();
    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void setCusVisible(bool vis=false, char* para=NULL);
    void timer_handler(int timeOut);
private:
    inline void setUi();

public slots:
    void SlotSceneActivate(int id);
    void usbStorageRemove();
    void buttonReturnClick();
    void testMessage();
    void testMessage2();

private:
    //SceneProjectModeFormulaSet sceneProjectModeFormulaSet;
    SceneMaintainConfigLanguage sceneMaintainConfigLanguage;


    QWidget widget;
    QLabel labelTitle;

    WidgetButton buttonLanguage;
    WidgetButton buttonReturn;

    QButtonGroup buttonGroupSetup;

private:

};

#endif
