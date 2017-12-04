#include <stdio.h>
#include "widget_common.h"
#include "widget_setup_cure.h"
#include "common_tr_string.h"

inline void WidgetSetupCure::setUi()
{
    widget.setGeometry(QRect(Widget_Setup_Menu_X,
                Widget_Setup_Menu_Y,
                Widget_Setup_Menu_Width,
                Widget_Setup_Menu_High));
    widget.setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(widget.backgroundRole(),
            QBrush(QPixmap(":/resource/images/setup/menu_back.bmp")));
    widget.setPalette(palette);

    buttonListShow.setGeometry(QRect(0, 0, 124, 30));

    groupBox.setStyleSheet(" QGroupBox {"
            "border: 0px solid gray;"
            "border-radius: 0px;"
            "margin-top: 0ex; /* leave space at the top for the title */"
            "}"
            );

    groupBox.setGeometry(QRect(0, 30, 147, 666));

    QVBoxLayout *verticalLayout = new QVBoxLayout(&groupBox);
    verticalLayout->addWidget(&buttonCureTime);
    verticalLayout->addWidget(&buttonUf);
    verticalLayout->addWidget(&buttonHemopump);
    verticalLayout->addWidget(&buttonHeparin);
    verticalLayout->addWidget(&buttonPressure);
    verticalLayout->addWidget(&buttonDialysate);
    verticalLayout->addWidget(&buttonSubstitution);
    verticalLayout->addWidget(&buttonIsoUF);
    verticalLayout->addWidget(&buttonOtherOper);
    verticalLayout->addStretch();

    buttonListShow.setFixedSize(100, 30);
    buttonCureTime.setFixedSize(124, 45);
    buttonUf.setFixedSize(124, 45);
    buttonHemopump.setFixedSize(124, 45);
    buttonHeparin.setFixedSize(124, 45);
   buttonPressure.setFixedSize(124, 45);
    buttonDialysate.setFixedSize(124, 45);
    buttonSubstitution.setFixedSize(124, 45);
    buttonIsoUF.setFixedSize(124, 45);
    buttonOtherOper.setFixedSize(124, 45);

    buttonListShow.setStyleNormal(":/resource/images/setup/menu_button_normal.bmp");
    buttonListShow.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE,
            QRect(0, 0, buttonListShow.width(), buttonListShow.height()));

    buttonCureTime.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonCureTime.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonCureTime.width(),
                buttonCureTime.height()));

    buttonUf.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonUf.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonUf.width(),
                buttonUf.height()));

    buttonHemopump.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonHemopump.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonHemopump.width(),
                buttonHemopump.height()));

    buttonHeparin.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonHeparin.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonHeparin.width(),
                buttonHeparin.height()));

    buttonPressure.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonPressure.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonPressure.width(),
                buttonPressure.height()));

    buttonDialysate.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonDialysate.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonDialysate.width(),
                buttonDialysate.height()));

    buttonSubstitution.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonSubstitution.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonSubstitution.width(),
                buttonSubstitution.height()));

    buttonIsoUF.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonIsoUF.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonIsoUF.width(),
                buttonIsoUF.height()));

    buttonOtherOper.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonOtherOper.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonIsoUF.width(),
                buttonIsoUF.height()));

    buttonGroupSetup.addButton(&buttonCureTime, 1);
    buttonGroupSetup.addButton(&buttonUf, 2);
    buttonGroupSetup.addButton(&buttonHemopump, 3);
    buttonGroupSetup.addButton(&buttonHeparin, 4);
    buttonGroupSetup.addButton(&buttonPressure, 5);
    buttonGroupSetup.addButton(&buttonDialysate, 6);
    buttonGroupSetup.addButton(&buttonSubstitution, 7);
    buttonGroupSetup.addButton(&buttonIsoUF, 8);
    buttonGroupSetup.addButton(&buttonOtherOper, 9);

    labelTitle.setGeometry(QRect(34, 21, 250, 30));
    LABEL_TITLE_STYLE(labelTitle);
}

void WidgetSetupCure::CustomSetText()
{
    buttonListShow.setText(tr(" "));
    buttonCureTime.setText(tr("Treat Time"));
    buttonUf.setText(tr("UF"));
    buttonHemopump.setText(tr("Blood Pump"));
    buttonHeparin.setText(tr("Heparin"));
    buttonPressure.setText(tr("Pressure"));
    buttonDialysate.setText(tr("Dialysate"));
    buttonSubstitution.setText(tr("Substitution"));
    buttonIsoUF.setText(tr("ISO UF"));
    buttonOtherOper.setText(DE_TR_OTHERUSE);
}

WidgetSetupCure::WidgetSetupCure(QWidget *parent) :
    QObject(parent),
    sceneSetupCureTime(parent),
    sceneSetupUf(parent),
    sceneSetupIsoUF(parent),
    sceneSetupHemopump(parent),
    sceneSetupHeparin(parent),
    sceneSetupPressure(parent),
    sceneSetupDialysate(parent),
    sceneSetupSubspump(parent),
    sceneSetupOtherOper(parent),
    widget(parent),
    labelTitle(parent),
    buttonListShow(&widget),
    buttonGroupSetup(&widget),
    groupBox(&widget),
    buttonCureTime(&widget),
    buttonUf(&widget),
    buttonHemopump(&widget),
    buttonHeparin(&widget),
    buttonPressure(&widget),
    buttonDialysate(&widget),
    buttonSubstitution(&widget),
    buttonIsoUF(&widget),
    buttonOtherOper(&widget)
{
    setUi();
    CustomSetText();

    //connect(&buttonListShow, SIGNAL(clicked()), this, SLOT(buttonShowClicked()));
    connect(&buttonGroupSetup, SIGNAL(buttonClicked(int)), this, SLOT(SlotSceneActivate(int)));
    dataNodeAddObject(PACKET_NAME_WORKMODE, this);

    PageAppendPiece("Setup Cure", this);
    PageAppendPiece("Setup Pressure", this);
    PageAppendPiece("Setup Treatment time", this);
    PageAppendPiece("Setup UF", this);
    PageAppendPiece("Setup ISO UF", this);
    PageAppendPiece("Setup Heparin", this);
    PageAppendPiece("Setup Hemopump", this);
    PageAppendPiece("Setup Dialysate", this);
    PageAppendPiece("Setup Subspump", this);
    PageAppendPiece("SceneSetupOherOper", this);
}

void WidgetSetupCure::buttonShowClicked(int isHide)
{
    if (isHide == 0) {
        groupBox.show();
        widget.setGeometry(QRect(Widget_Setup_Menu_X,
                    Widget_Setup_Menu_Y,
                    Widget_Setup_Menu_Width,
                    Widget_Setup_Menu_High));
        return;
    } else if (isHide == 1) {
        groupBox.hide();
        widget.setGeometry(QRect(Widget_Setup_Menu_X,
                    Widget_Setup_Menu_Y,
                    Widget_Setup_Menu_Width,
                    30));
        return;
    }

    if (groupBox.isHidden()) {
        groupBox.show();
        widget.setGeometry(QRect(Widget_Setup_Menu_X,
                    Widget_Setup_Menu_Y,
                    Widget_Setup_Menu_Width,
                    Widget_Setup_Menu_High));
    } else {
        groupBox.hide();
        widget.setGeometry(QRect(Widget_Setup_Menu_X,
                    Widget_Setup_Menu_Y,
                    Widget_Setup_Menu_Width,
                    30));
    }
}

void WidgetSetupCure::setCusVisible(bool vis, char* para)
{
    char *pageName = NULL;
    para = para;

    labelTitle.setVisible(vis);
    widget.setVisible(vis);
    if (vis == true) {
        pageName = PageCurrent();
        if (strcmp(pageName, "Setup Treatment time") == 0) {
            buttonCureTime.setChecked(true);
            labelTitle.setText(tr("Setup Treatment time"));
        } else if (strcmp(pageName, "Setup UF") == 0) {
            buttonUf.setChecked(true);
            labelTitle.setText(tr("Setup UF"));
        } else if (strcmp(pageName, "Setup Hemopump") == 0) {
            buttonHemopump.setChecked(true);
            labelTitle.setText(tr("Setup Bloodpump"));
        } else if (strcmp(pageName, "Setup Dialysate") == 0) {
            buttonDialysate.setChecked(true);
            labelTitle.setText(tr("Setup Dialysate"));
        } else if (strcmp(pageName, "Setup Subspump") == 0) {
            buttonSubstitution.setChecked(true);
            labelTitle.setText(tr("Setup Subspump"));
        } else if (strcmp(pageName, "Setup ISO UF") == 0) {
            buttonIsoUF.setChecked(true);
            labelTitle.setText(tr("Setup ISO UF"));
        } else if (strcmp(pageName, "Setup Heparin") == 0) {
            buttonHeparin.setChecked(true);
            labelTitle.setText(tr("Setup Heparin"));
        }else if (strcmp(pageName, "Setup Pressure") == 0) {
             buttonPressure.setChecked(true);
            labelTitle.setText(tr("Setup Pressure"));
        }else if (strcmp(pageName, "SceneSetupOherOper") == 0) {
            buttonOtherOper.setChecked(true);
           labelTitle.setText(DE_TR_OTHERUSE);
       }
        else{labelTitle.setText(tr(pageName));}
        int id = buttonGroupSetup.checkedId();
        QAbstractButton * button = buttonGroupSetup.checkedButton();
        if(id == -1 ){
            if(id != -1)button->setChecked(false);
            buttonCureTime.setChecked(true);
            id=1;
        }
        SlotSceneActivate( id );
    }
}

void WidgetSetupCure::SlotSceneActivate(int id)
{
    switch (id) {
        case 1:
            PageShow("Setup Treatment time");
            break;
        case 2:
            PageShow("Setup UF");
            break;
        case 3:
            PageShow("Setup Hemopump");
            break;
        case 4:
            PageShow("Setup Heparin");
            break;
        case 5:
            PageShow("Setup Pressure");
            break;
        case 6:
            PageShow("Setup Dialysate");
            break;
        case 7:
            PageShow("Setup Subspump");
            break;
        case 8:
            PageShow("Setup ISO UF");
            break;
        case 9:
           PageShow("SceneSetupOherOper");
           break;
    }

}

void WidgetSetupCure::ActionHandle(const void* dataPtr,int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetSetupCure-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name == 0)) {
        return;
    }

    if (name==PACKET_NAME_WORKMODE) {//改变治疗模式
    PacketWorkMode *packetWorkMode = (PacketWorkMode *)dataPtr;

    if(packetWorkMode->treatmode.current == ISOUF_DNDP||packetWorkMode->treatmode.current == ISOUF_SNDP||\
       packetWorkMode->treatmode.current == ISOUF_SNSP)
    {
        buttonSubstitution.hide();
        buttonIsoUF.show();
    }
    else if(packetWorkMode->treatmode.current == HDF_pre_dilute||packetWorkMode->treatmode.current == HDF_post_dilute||\
            packetWorkMode->treatmode.current == HF_pre_dilute||packetWorkMode->treatmode.current == HF_post_dilute)
    {
        buttonSubstitution.show();
        buttonIsoUF.hide();
    }
    else
    {
        buttonSubstitution.hide();
        buttonIsoUF.hide();
    }
  }

/*
    if ((packetWorkMode->treatmode.current == HD_double) ||
        (packetWorkMode->treatmode.current == ISOUF_DNDP)) {
        buttonSubstitution.hide();
        buttonIsoUF.show();
    } else if (packetWorkMode->treatmode.current == HDF_pre_dilute) {
        buttonSubstitution.show();
        buttonIsoUF.hide();
        }
    }
    */
#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetSetupCure-ActionHandle-out");
#endif
}
