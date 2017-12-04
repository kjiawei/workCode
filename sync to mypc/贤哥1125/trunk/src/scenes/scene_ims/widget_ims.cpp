#include <stdio.h>
#include "config_ui.h"
#include "widget_ims.h"
#include "widget_common.h"
#include "common_tr_string.h"


inline void WidgetIms::setUi()
{
    labelTitle.setGeometry(QRect(34, 21, 200, 30));
    LABEL_TITLE_STYLE(labelTitle);
    labelTitle.setText(tr("IMS"));

    widget.setGeometry(QRect(Widget_Ims_Menu_X,
                Widget_Ims_Menu_Y,
                Widget_Ims_Menu_Width,
                Widget_Ims_Menu_High));
    widget.setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(widget.backgroundRole(), QBrush(QPixmap(":/resource/images/setup/menu_back.bmp")));
    widget.setPalette(palette);

    QVBoxLayout *verticalLayout = new QVBoxLayout(&widget);
//    verticalLayout->addWidget(&buttonLogTreat);
    verticalLayout->addWidget(&buttonLogWarn);
//    verticalLayout->addWidget(&buttonLogEmr);
    verticalLayout->addWidget(&buttonLogRun);
    verticalLayout->addWidget(&buttonRecordCurve);
    verticalLayout->addWidget(&buttonPatientMessage);
    verticalLayout->addStretch();

//    buttonLogTreat.setFixedSize(124, 45);
    buttonLogWarn.setFixedSize(124, 45);
//    buttonLogEmr.setFixedSize(124, 45);
    buttonLogRun.setFixedSize(124, 45);
    buttonRecordCurve.setFixedSize(124,45);
    buttonPatientMessage.setFixedSize(124,45);
/*
    buttonLogTreat.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonLogTreat.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, 124, 45));
*/
    buttonLogWarn.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonLogWarn.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, 124, 45));

    buttonLogRun.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonLogRun.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, 124, 45));

    /*
    buttonLogEmr.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonLogEmr.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, 124, 45));
*/

    buttonRecordCurve.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonRecordCurve.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, 124, 45));


    buttonPatientMessage.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonPatientMessage.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, 124, 45));

    CustomSetText();

//    buttonGroupIms.addButton(&buttonLogEmr, 1);
//    buttonGroupIms.addButton(&buttonLogTreat, 2);
    buttonGroupIms.addButton(&buttonLogWarn, 3);
    buttonGroupIms.addButton(&buttonLogRun, 4);
    buttonGroupIms.addButton(&buttonRecordCurve,5);
    buttonGroupIms.addButton(&buttonPatientMessage,6);
}

WidgetIms::WidgetIms(QWidget *parent) :
    QObject(parent),
    sceneLogTreat(parent),
    sceneLogWarn(parent),
    sceneLogEmr(parent),
    sceneLogRun(parent),
    sceneCurve(parent),
    patientMessage(parent),
    widget(parent),
    labelTitle(parent),
    buttonGroupIms(&widget),
//    buttonLogTreat(&widget),
    buttonLogWarn(&widget),
//    buttonLogEmr(&widget),
    buttonLogRun(&widget),
    buttonRecordCurve(&widget),
    buttonPatientMessage(&widget)
{
    setUi();

    connect(&buttonGroupIms, SIGNAL(buttonClicked(int)), this, SLOT(SlotSceneActivate(int)));
    PageAppendPiece("IMS", this);
    PageAppendPiece("LogTreat", this);
    PageAppendPiece("LogWarn", this);
    PageAppendPiece("LogEmr", this);
    PageAppendPiece("LogRun", this);
    PageAppendPiece("RecordCurve",this);
    PageAppendPiece("RFCard", this);
}

void WidgetIms::SlotSceneActivate(int id)
{
    setSubpageHide();
    switch (id) {
        case 1:
            PageShow("LogEmr");
            break;
        case 2:
            PageShow("LogTreat");
            break;
        case 3:
            PageShow("LogWarn");
            break;
        case 4:
            PageShow("LogRun");
            break;
        case 5:
            PageShow("RecordCurve");
            break;
        case 6:
            PageShow("RFCard");
            break;
    }
}

void WidgetIms::setSubpageHide()
{
    sceneLogEmr.setCusVisible(false);
    sceneLogTreat.setCusVisible(false);
    sceneLogWarn.setCusVisible(false);
    sceneLogRun.setCusVisible(false);
    sceneCurve.setCusVisible(false);
    patientMessage.setCusVisible(false);
}

void WidgetIms::setCusVisible(bool vis, char* para)
{
    para = para;
    if (vis == true) {
        widget.show();
        labelTitle.show();
    } else {
        widget.hide();
        labelTitle.hide();
        setSubpageHide();
    }
}

void WidgetIms::CustomSetText()
{
    labelTitle.setText(tr("IMS"));
//    buttonLogTreat.setText(tr("Log Treat"));
    buttonLogWarn.setText(tr("Log Warn"));
    buttonLogRun.setText(tr("Log Run"));
//    buttonLogEmr.setText(tr("Log EMR"));
    buttonRecordCurve.setText(tr("Record Profile"));
    buttonPatientMessage.setText(DE_TR_PATIENTMESSAGE);
}
