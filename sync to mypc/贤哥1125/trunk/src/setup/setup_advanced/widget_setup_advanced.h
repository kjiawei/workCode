#ifndef WIDGET_SETUP_ADVANCED_H
#define WIDGET_SETUP_ADVANCED_H

#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QButtonGroup>
#include "config_ui.h"
#include "lang_switch.h"
#include "widget_button.h"
#include "page_info.h"
#include "scene_setup_EC.h"
#include "scene_setup_bpm.h"
#include "scene_setup_nauf.h"
#include "scene_setup_unit.h"
#include "scene_setup_oximeter.h"
#include "scene_setup_bloodtemp.h"
#include "scene_setup_curve_na.h"
#include "scene_setup_curve_uf.h"
#include "scene_setup_sequential.h"
#include "scene_setup_dialysateflowcurve.h"
#include "scene_setup_condcurve.h"
#include "scene_setup_tempcurve.h"
#include "scene_setup_hco3curve.h"
#include "scene_setup_xmlconfig.h"

#include "scene_setup_heparincurve.h"
#include "scene_setup_curve_test.h"
#include "scene_setup_preflush.h"
#include "scene_setup_advert_timer.h"

#define Widget_Setup_Menu_X             12
#define Widget_Setup_Menu_Y             57
#define Widget_Setup_Menu_Width         147
#define Widget_Setup_Menu_High          696

class WidgetSetupAdvanced : public QObject, public PieceVisible, public LangSetText
{
    Q_OBJECT
public:
    WidgetSetupAdvanced(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

public slots:
    void SetupMenuActivate(int id);

private slots:
    void buttonShowClicked(int isHide=2);

private:
    inline void setUi();

private:
    SceneSetupEC sceneSetupEC;  //电导设置页面#/ //屏蔽#/
    SceneSetupNaUF sceneSetupNaUF; //钠和超虑曲线，选择页面#/
    SceneSetupUnit sceneSetupUnit;  //？？长度，已废弃#/
    SceneSetupCurveNa sceneSetupCurveNa;  //钠曲线页面#/
    SceneSetupCurveUF sceneSetupCurveUF;  //超滤曲线页面#/
    SceneSetupSequential sceneSetupSequential; //超滤曲线，选择页面#/
    SceneSetupDialysateFlowCurve sceneSetupDiaFlowCurve;//透析夜流量曲线页面+#/
//    SceneSetupXmlconfig sceneSetupXmlconfig;//打开配置文件页面，屏蔽#/
    SceneSetupHeparinCurve sceneSetupHeparinCur;  //肝素曲线+#/
    SceneSetupCurveTest sceneSetupCurveTest; //曲线测试页面，已废弃#/
    SceneSetupPreflush sceneSetupPreflush;  //预冲设置#/
    SceneSetupCondCurve sceneSetupCondCurve; //电导曲线#/
    SceneSetupTempCurve sceneSetupTempCurve;      //温度曲线#/
    SceneSetupHco3Curve sceneSetupHco3Curve;  //HCO3曲线#/


    QWidget widget;  //菜单栏窗口#/
    QLabel labelTitle;  //当前显示页面的标题#/
    WidgetButton buttonListShow; //菜单栏显示按钮#/

    QButtonGroup buttonGroupSetup;
    QGroupBox groupBox;

    WidgetButton buttonNaUf;
    WidgetButton buttonSequent;
    WidgetButton buttonConduction;//屏蔽#/
//    WidgetButton buttonXmlconfig;

    WidgetButton buttonPreflush;
    WidgetButton buttonDialysateFlow;    //透析夜流量曲线#/
    WidgetButton buttonHeparinCurve;    //肝素曲线#/
    WidgetButton buttonCondCurve;      //电导曲线#/
    WidgetButton buttonTempCurve;      //温度曲线#/
    WidgetButton buttonHco3Curve;     //HCO3曲线#/
};

#endif
