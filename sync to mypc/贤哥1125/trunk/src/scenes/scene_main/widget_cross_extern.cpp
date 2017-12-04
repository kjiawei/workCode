#include <QtGui>
#include "config_ui.h"
#include "page_info.h"
#include "widget_cross_extern.h"

WidgetCrossExtern::WidgetCrossExtern(QWidget *parent) :
    QWidget(parent),
    pushButtonWorkMode(this),
    pushButtonCureInfor(this),
    pushButtonAdvanced(this),
    pushButtonOther(this)
{
    pushButtonWorkMode.setStyleNormal(":/resource/images/setup_button.png",
            QRect(WIDGET_CROSS_EXTEND_BUTTON_X,
                WIDGET_CROSS_EXTEND_WORKMODE_Y,
                WIDGET_CROSS_EXTEND_BUTTON_WIDTH,
                WIDGET_CROSS_EXTEND_BUTTON_HIGH));
    pushButtonWorkMode.setTextFontPos(CONFIG_FONT_POINTSIZE_SMALL2,
            CONFIG_FONT_NORMAL, 0xFF031e37);

    pushButtonCureInfor.setStyleNormal(":/resource/images/setup_button.png",
            QRect(WIDGET_CROSS_EXTEND_BUTTON_X,
                WIDGET_CROSS_EXTEND_CUREINFO_Y,
                WIDGET_CROSS_EXTEND_BUTTON_WIDTH,
                WIDGET_CROSS_EXTEND_BUTTON_HIGH));
    pushButtonCureInfor.setTextFontPos(CONFIG_FONT_POINTSIZE_SMALL2,
            CONFIG_FONT_NORMAL, 0xFF031e37);

    pushButtonAdvanced.setStyleNormal(":/resource/images/setup_button.png",
            QRect(WIDGET_CROSS_EXTEND_BUTTON_X,
                WIDGET_CROSS_EXTEND_DIALYSYSLIQUID_Y,
                WIDGET_CROSS_EXTEND_BUTTON_WIDTH,
                WIDGET_CROSS_EXTEND_BUTTON_HIGH));
    pushButtonAdvanced.setTextFontPos(CONFIG_FONT_POINTSIZE_SMALL2,
            CONFIG_FONT_NORMAL, 0xFF031e37);

    pushButtonOther.setStyleNormal(":/resource/images/setup_button.png",
            QRect(WIDGET_CROSS_EXTEND_BUTTON_X,
                WIDGET_CROSS_EXTEND_OTHER_Y,
                WIDGET_CROSS_EXTEND_BUTTON_WIDTH,
                WIDGET_CROSS_EXTEND_BUTTON_HIGH));
    pushButtonOther.setTextFontPos(CONFIG_FONT_POINTSIZE_SMALL2,
            CONFIG_FONT_NORMAL, 0xFF031e37);

    connect(&pushButtonWorkMode, SIGNAL(clicked()),
            this, SLOT(pushButtonWorkModeClicked()));
    connect(&pushButtonCureInfor, SIGNAL(clicked()),
            this, SLOT(pushButtonCureInforClicked()));
    connect(&pushButtonAdvanced, SIGNAL(clicked()),
            this, SLOT(pushButtonAdvancedClicked()));
    connect(&pushButtonOther, SIGNAL(clicked()),
            this, SLOT(pushButtonOtherClicked()));

    pixmapBackGround = QPixmap(":/resource/images/setup_background.png");

    CustomSetText();
}

void WidgetCrossExtern::pushButtonWorkModeClicked(void)
{
    hide();
    PageShow("SetupWorkmode");
}

void WidgetCrossExtern::pushButtonCureInforClicked(void)
{
    hide();
    PageShow("Setup Cure");
}

void WidgetCrossExtern::pushButtonAdvancedClicked(void)
{
    hide();
    PageShow("SetupAdvanced");
}

void WidgetCrossExtern::pushButtonOtherClicked(void)
{
    hide();
    PageShow("SetupOther");
}

void WidgetCrossExtern::paintEvent(QPaintEvent *event)
{
    event = event;

    if (!pixmapBackGround.isNull()) {
        QPainter painter(this);
        painter.drawPixmap(0, 0, pixmapBackGround);
    }
}

void WidgetCrossExtern::CustomSetText()
{
    pushButtonWorkMode.setText(tr("Work mode"));
    pushButtonCureInfor.setText(tr("Parameters"));
    pushButtonAdvanced.setText(tr("Advanced"));
    pushButtonOther.setText(tr("Other"));
}
