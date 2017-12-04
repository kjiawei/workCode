#include <stdio.h>
#include "widget_common.h"
#include "widget_setup_ext.h"

void WidgetSetupExt::setUi()
{
    widget.setGeometry(QRect(Widget_Setup_Menu_X_KTV,
                Widget_Setup_Menu_Y_KTV,
                Widget_Setup_Menu_Width_KTV,
                Widget_Setup_Menu_High_KTV));
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
    verticalLayout->addWidget(&buttonHeartPressure);
    verticalLayout->addWidget(&buttonSpO2);
    verticalLayout->addWidget(&buttonBloodTemp);
    verticalLayout->addWidget(&buttonTimer);
    verticalLayout->addWidget(&buttonKtV);
    verticalLayout->addWidget(&buttonKtV2);
    verticalLayout->addWidget(&buttonBloodVolume);

    verticalLayout->addStretch();

    buttonListShow.setFixedSize(100, 30);
    buttonHeartPressure.setFixedSize(124, 45);
    buttonSpO2.setFixedSize(124, 45);
    buttonBloodTemp.setFixedSize(124, 45);
    buttonTimer.setFixedSize(124, 45);
    buttonKtV.setFixedSize(124, 45);
    buttonKtV2.setFixedSize(124, 45);
    buttonBloodVolume.setFixedSize(124, 45);

    buttonListShow.setStyleNormal(":/resource/images/setup/menu_button_normal.bmp");
    buttonListShow.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE,
            QRect(0, 0, buttonListShow.width(), buttonListShow.height()));

    buttonHeartPressure.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonHeartPressure.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonHeartPressure.width(), buttonHeartPressure.height()));

    buttonSpO2.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonSpO2.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonSpO2.width(), buttonSpO2.height()));

    buttonBloodTemp.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonBloodTemp.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonBloodTemp.width(), buttonBloodTemp.height()));


    buttonTimer.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonTimer.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonTimer.width(), buttonTimer.height()));

    buttonKtV.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonKtV.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonKtV.width(), buttonKtV.height()));

    buttonKtV2.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonKtV2.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonKtV2.width(), buttonKtV2.height()));

    buttonBloodVolume.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonBloodVolume.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonBloodVolume.width(), buttonBloodVolume.height()));

    buttonGroupSetup.addButton(&buttonHeartPressure, 1);
    buttonGroupSetup.addButton(&buttonSpO2, 2);
    buttonGroupSetup.addButton(&buttonBloodTemp, 3);
    buttonGroupSetup.addButton(&buttonTimer, 4);
    buttonGroupSetup.addButton(&buttonKtV, 7);
    buttonGroupSetup.addButton(&buttonKtV2, 5);
    buttonGroupSetup.addButton(&buttonBloodVolume, 6);

    labelTitle.setGeometry(QRect(34, 21, 300, 30));
    LABEL_TITLE_STYLE(labelTitle);
    //labelTitle.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    //labelTitle.setStyleSheet("color:#dbe6ef;font-size:20px;Bold");

    CustomSetText();
}

void WidgetSetupExt::buttonShowClicked(int isHide)
{
    if (isHide == 0) {
        groupBox.show();
        widget.setGeometry(QRect(Widget_Setup_Menu_X_KTV,
                    Widget_Setup_Menu_Y_KTV,
                    Widget_Setup_Menu_Width_KTV,
                    Widget_Setup_Menu_High_KTV));
        return;
    } else if (isHide == 1) {
        groupBox.hide();
        widget.setGeometry(QRect(Widget_Setup_Menu_X_KTV,
                    Widget_Setup_Menu_Y_KTV,
                    Widget_Setup_Menu_Width_KTV,
                    30));
        return;
    }

    if (groupBox.isHidden()) {
        groupBox.show();
        widget.setGeometry(QRect(Widget_Setup_Menu_X_KTV,
                    Widget_Setup_Menu_Y_KTV,
                    Widget_Setup_Menu_Width_KTV,
                    Widget_Setup_Menu_High_KTV));
    } else {
        groupBox.hide();
        widget.setGeometry(QRect(Widget_Setup_Menu_X_KTV,
                    Widget_Setup_Menu_Y_KTV,
                    Widget_Setup_Menu_Width_KTV,
                    30));
    }
}

WidgetSetupExt::WidgetSetupExt(QWidget *parent) :
    QObject(parent),
    sceneSetupBPM(parent),
    sceneSetupSpO2(parent),
    sceneSetupBloodTemp(parent),
    sceneSetupAdvertTime(parent),
    sceneKTV(parent),
    sceneKTV2(parent),
    sceneBloodVolume(parent),
    widget(parent),
    labelTitle(parent),
    buttonListShow(&widget),
    buttonGroupSetup(&widget),
    groupBox(&widget),
    buttonHeartPressure(&widget),
    buttonSpO2(&widget),
    buttonTimer(&widget),
    buttonKtV(&widget),
    buttonKtV2(&widget),
    buttonBloodVolume(&widget)
{
    setUi();
    connect(&buttonListShow, SIGNAL(clicked()), this, SLOT(buttonShowClicked()));
    connect(&buttonGroupSetup, SIGNAL(buttonClicked(int)), this, SLOT(SetupMenuActivate(int)));
    PageAppendPiece("SetupExtersion", this);
    PageAppendPiece("Setup SpO2", this);
    PageAppendPiece("Setup BloodTemp", this);
    PageAppendPiece("Setup BPM", this);
    PageAppendPiece("Setup Timer", this);
    PageAppendPiece("KtV", this);
    PageAppendPiece("KtV2", this);
    PageAppendPiece("BloodVolume", this);
}

void WidgetSetupExt::SetupMenuActivate(int id)
{
    switch (id)
    {
        case 1:
            PageShow("Setup BPM");
            break;
        case 2:
            PageShow("Setup SpO2");
            break;
        case 3:
            PageShow("Setup BloodTemp");
            break;
        case 4:
            PageShow("Setup Timer");
            break;
        case 7:
            PageShow("KtV");
            break;
        case 5:
            PageShow("KtV2");
            break;
        case 6:
            PageShow("BloodVolume");
            break;
    }
}

void WidgetSetupExt::setCusVisible(bool vis, char* para)
{
    char *pageName = NULL;
    para = para;

    labelTitle.setVisible(vis);
    widget.setVisible(vis);
    buttonKtV.setVisible(vis);//false
    if (vis == true) {
        pageName = PageCurrent();
        labelTitle.setText(tr(pageName));

        if (strcmp(pageName, "Setup BPM") == 0) {
            buttonHeartPressure.setChecked(true);
        }else if(strcmp(pageName, "Setup SpO2") == 0) {
            buttonSpO2.setChecked(true);
        }else if(strcmp(pageName, "Setup BloodTemp") == 0) {
            buttonBloodTemp.setChecked(true);
        }else if(strcmp(pageName, "Setup Timer") == 0) {
            buttonTimer.setChecked(true);
        }else if(strcmp(pageName, "KtV") == 0) {
            buttonKtV.setChecked(true);
        }else if(strcmp(pageName, "KtV2") == 0) {
            buttonKtV2.setChecked(true);
        }else if(strcmp(pageName, "BloodVolume") == 0) {
            buttonBloodVolume.setChecked(true);
        }
        int id = buttonGroupSetup.checkedId();
        if(id == -1)id=1;
        SetupMenuActivate( id );
    }
}

void WidgetSetupExt::CustomSetText()
{
    buttonListShow.setText(tr(" "));
    buttonHeartPressure.setText(tr("BPM"));
    buttonSpO2.setText(tr("SpO2"));
    buttonBloodTemp.setText(tr("BTM"));
    buttonTimer.setText(tr("Timer"));
    buttonKtV.setText(tr("Kt/V"));
    buttonKtV2.setText(tr("Kt/V2"));
    buttonBloodVolume.setText(tr("BVM"));
}
