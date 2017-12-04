#include <QtGui>
#include "packet.h"
#include "data_custom.h"
#include "config_ui.h"
#include "widget_cross.h"
#include "state.h"

WidgetCross::WidgetCross(QWidget *parent) :
    QWidget(parent),
    buttonGroupCross(this),
    pushButtonSetup(parent),
    widgetSetupExtern(parent),
    pushButtonDisinfect(this),
    pushButtonInfo(this),
    pushButtonExt(this),
    pushButtonHome(this)
{
    pushButtonDisinfect.setStyleSky(":/resource/images/cross_soap.png",
            ":/resource/images/cross_soap_activate.png",
            QRect(WIDGETCROSS_BUTTON_DISINFECT_X,
                WIDGETCROSS_BUTTON_DISINFECT_Y,
                WIDGETCROSS_BUTTON_DISINFECT_WIDTH,
                WIDGETCROSS_BUTTON_DISINFECT_HIGH));

    pushButtonInfo.setStyleSky(":/resource/images/cross_find.png",
            ":/resource/images/cross_find_active.png",
            QRect(WIDGETCROSS_BUTTON_INFO_X,
                WIDGETCROSS_BUTTON_INFO_Y,
                WIDGETCROSS_BUTTON_INFO_WIDTH,
                WIDGETCROSS_BUTTON_INFO_HIGH));

    pushButtonExt.setStyleSky(":/resource/images/cross_additional.png",
            ":/resource/images/cross_additional_actived.png",
            QRect(WIDGETCROSS_BUTTON_EXT_X,
                WIDGETCROSS_BUTTON_EXT_Y,
                WIDGETCROSS_BUTTON_EXT_WIDTH,
                WIDGETCROSS_BUTTON_EXT_HIGH));

    pushButtonHome.setStyleNormal(":/resource/images/cross_home.png",
            QRect(WIDGETCROSS_BUTTON_HOME_X,
                WIDGETCROSS_BUTTON_HOME_Y,
                WIDGETCROSS_BUTTON_HOME_WIDTH,
                WIDGETCROSS_BUTTON_HOME_HIGH));

    pushButtonSetup.setStyleSky(":/resource/images/cross_setup.png",
            ":/resource/images/cross_setup_activate.png",
            QRect(748, 595, 87, 77));

    widgetSetupExtern.setGeometry(QRect(WIDGET_CROSS_EXTEND_X,
                WIDGET_CROSS_EXTEND_Y,
                WIDGET_CROSS_EXTEND_WIDTH,
                WIDGET_CROSS_EXTEND_HIGH));
    widgetSetupExtern.hide();

    buttonGroupCross.addButton(&pushButtonDisinfect);
    buttonGroupCross.addButton(&pushButtonInfo);
    buttonGroupCross.addButton(&pushButtonExt);
    buttonGroupCross.addButton(&pushButtonHome);
    buttonGroupCross.addButton(&pushButtonSetup);

    pushButtonSetup.setObjectName("pushButtonSetup");

    connect(&pushButtonHome, SIGNAL(pressed()), this, SLOT(pushButtonHomeClicked()));
    connect(&pushButtonSetup, SIGNAL(clicked()), this, SLOT(pushButtonSetupClicked()));
    connect(&pushButtonDisinfect, SIGNAL(clicked()), this, SLOT(pushButtonDisinfectClicked()));
    connect(&pushButtonInfo, SIGNAL(clicked()), this, SLOT(pushButtonInfoClicked()));
    connect(&pushButtonExt, SIGNAL(clicked()), this, SLOT(pushButtonExtClicked())); 

    dataNodeAddObject(PACKET_NAME_WORKSTATE, this);
}

void WidgetCross::pushButtonInfoClicked(void)
{
    widgetSetupExtern.hide();
    PageShow("IMS");//Information Manager System
}

void WidgetCross::pushButtonExtClicked(void)
{
    widgetSetupExtern.hide();
    PageShow("SetupExtersion");
}

void WidgetCross::pushButtonSetupClicked(void)
{
    if (pushButtonSetup.isChecked() == true&& widgetSetupExtern.isHidden()) {
        widgetSetupExtern.show();
        pushButtonSetup.setChecked(true);
    } else {
        widgetSetupExtern.hide();
        buttonGroupCross.setExclusive(false);
        pushButtonSetup.setChecked(false);
        buttonGroupCross.setExclusive(true);
    }
}

void WidgetCross::pushButtonDisinfectClicked(void)
{
    WorkState stateTMP = GetWorkStateManage();
    if(!stateTMP.disinfectEnable)//消毒标志禁止进入消毒界面
    {
        QAbstractButton *button = NULL;
        button = buttonGroupCross.checkedButton();
        if (button != NULL) {
            buttonGroupCross.setExclusive(false);
            button->setChecked(false);
            buttonGroupCross.setExclusive(true);
        }
        return;
    }
    widgetSetupExtern.hide();
    PageShow("Disinfect");
}

void WidgetCross::pushButtonHomeClicked(void)
{
    if (strcmp(PageCurrent(), "Main") == 0) {
        PageShow("Summary");
    } else {
        PageShow("Main");
    }

    QAbstractButton *button = NULL;
    button = buttonGroupCross.checkedButton();
    if (button != NULL) {
        buttonGroupCross.setExclusive(false);
        button->setChecked(false);
        buttonGroupCross.setExclusive(true);
    }
    widgetSetupExtern.hide();
}

void WidgetCross::setCusVisible(bool vis, char* para)
{
    para = para;
    if (vis == false) {
        widgetSetupExtern.hide();
    }
}

void WidgetCross::paintEvent(QPaintEvent *event)
{
    event = event;

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(WIDGETCROSS_CENTRE_X,
            WIDGETCROSS_CENTRE_Y);
    painter.setPen(QPen(QColor(214, 214, 214), 8, Qt::SolidLine));
    painter.drawEllipse(-37, -37, 74, 74);
}

void WidgetCross::ActionHandle(const void* dataPtr,int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("widget_cross-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name == 0)) {
        return;
    }

    if (name==PACKET_NAME_WORKSTATE){
        workstate =*((WorkState*) dataPtr);
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("widget_cross-ActionHandle-out");
#endif
}

void WidgetCross::mouseClicked(int id)
{//隐藏设置菜单
    id = id;
    if(!widgetSetupExtern.isHidden()){//判断是否已经隐藏
        widgetSetupExtern.hide();
        buttonGroupCross.setExclusive(false);
        pushButtonSetup.setChecked(false);
        buttonGroupCross.setExclusive(true);
    }
    emit clickEvent(id);
}
