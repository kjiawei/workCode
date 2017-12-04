#include <stdio.h>
#include "config_ui.h"
#include "widget_setup_other.h"
#include "global_var.h"

inline void WidgetSetupOther::setUi()
{
    labelTitle.setGeometry(QRect(34, 21, 300, 30));
    LABEL_TITLE_STYLE(labelTitle);

    widget.setGeometry(QRect(Widget_Setup_Other_Menu_X,
                Widget_Setup_Other_Menu_Y,
                Widget_Setup_Other_Menu_Width,
                Widget_Setup_Other_Menu_High));
    widget.setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(widget.backgroundRole(), QBrush(QPixmap(":/resource/images/setup/menu_back.bmp")));
    widget.setPalette(palette);

    groupBox.setStyleSheet(" QGroupBox {"
            "border: 0px solid gray;"
            "border-radius: 0px;"
            "margin-top: 0ex; /* leave space at the top for the title */"
            "}"
            );

    groupBox.setGeometry(QRect(0, 30, 147, 666));

    QVBoxLayout *verticalLayout = new QVBoxLayout(&groupBox);

    //QVBoxLayout *verticalLayout = new QVBoxLayout(&widget);
    verticalLayout->addWidget(&buttonSystemTime);
    verticalLayout->addWidget(&buttonSetupNetwork);
    verticalLayout->addWidget(&buttonSetupLang);
    verticalLayout->addWidget(&buttonSystemInfo);
    verticalLayout->addWidget(&buttonMaintain);
    verticalLayout->addWidget(&buttonStatusMap);
//    verticalLayout->addWidget(&buttonRFCard);
    verticalLayout->addWidget(&buttonUnit);
    verticalLayout->addWidget(&buttonautoWakeUp);
    verticalLayout->addWidget(&buttonEngineeringMode);
    verticalLayout->addStretch();

    buttonSystemTime.setFixedSize(124, 45);
    buttonSetupNetwork.setFixedSize(124, 45);
    buttonSetupLang.setFixedSize(124, 45);
    buttonSystemInfo.setFixedSize(124, 45);
    buttonMaintain.setFixedSize(124, 45);
    buttonStatusMap.setFixedSize(124, 45);
//    buttonRFCard.setFixedSize(124, 45);
    buttonUnit.setFixedSize(124, 45);
    buttonautoWakeUp.setFixedSize(124, 45);
    buttonEngineeringMode.setFixedSize(124, 45);

    buttonSystemTime.setTextFontPos(CFG_FONT_SIZE_SECOND, CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonSystemTime.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, 124, 45));

    buttonSetupNetwork.setTextFontPos(CFG_FONT_SIZE_SECOND, CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonSetupNetwork.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, 124, 45));

    buttonSetupLang.setTextFontPos(CFG_FONT_SIZE_SECOND, CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonSetupLang.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, 124, 45));

    buttonSystemInfo.setTextFontPos(CFG_FONT_SIZE_SECOND, CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonSystemInfo.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, 124, 45));

    buttonMaintain.setTextFontPos(CFG_FONT_SIZE_SECOND, CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonMaintain.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, 124, 45));

    buttonStatusMap.setTextFontPos(CFG_FONT_SIZE_SECOND, CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonStatusMap.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, 124, 45));
/*
    buttonRFCard.setTextFontPos(CFG_FONT_SIZE_SECOND, CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonRFCard.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, 124, 45));
*/

    buttonUnit.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonUnit.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonUnit.width(), buttonUnit.height()));

    buttonautoWakeUp.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonautoWakeUp.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonautoWakeUp.width(), buttonautoWakeUp.height()));

    buttonEngineeringMode.setTextFontPos(CFG_FONT_SIZE_SECOND, CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonEngineeringMode.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, 124, 45));



    buttonGroupSetup.addButton(&buttonSystemTime, 1);
    buttonGroupSetup.addButton(&buttonSetupNetwork, 2);
    buttonGroupSetup.addButton(&buttonSetupLang, 3);
    buttonGroupSetup.addButton(&buttonSystemInfo, 4);
    buttonGroupSetup.addButton(&buttonMaintain, 5);
    buttonGroupSetup.addButton(&buttonStatusMap, 6);
//    buttonGroupSetup.addButton(&buttonRFCard, 7);
    buttonGroupSetup.addButton(&buttonUnit, 8);
    buttonGroupSetup.addButton(&buttonautoWakeUp, 9);
    buttonGroupSetup.addButton(&buttonEngineeringMode, 10);

    CustomSetText();
}

WidgetSetupOther::WidgetSetupOther(QWidget *parent) :
    QObject(parent),
    sceneSetupLang(parent),
    sceneSetupNetwork(parent),//HJX网络模块禁止#/
    sceneSetupSysinfo(parent),
    sceneSetupSystemTime(parent),
    sceneSetupAutoWakeUp(parent),
 //   patientMessage(parent),

    widget(parent),
    labelTitle(parent),
    groupBox(&widget),
    buttonGroupSetup(&widget),
    buttonSystemTime(&widget),
    buttonSetupNetwork(&widget),
    buttonSetupLang(&widget),
    buttonSystemInfo(&widget),
    buttonMaintain(&widget),
//    buttonRFCard(&widget) ,
    buttonUnit(&widget) ,
    buttonautoWakeUp(&widget),
    buttonEngineeringMode(&widget)
{
    setUi();

    connect(&buttonGroupSetup, SIGNAL(buttonClicked(int)), this, SLOT(SlotSceneActivate(int)));
    PageAppendPiece("SetupOther", this);
    PageAppendPiece("Setup Lang", this);
    PageAppendPiece("Setup Network", this);
    PageAppendPiece("Setup System Info", this);
    PageAppendPiece("Setup System Time", this);
//    PageAppendPiece("RFCard", this);
    PageAppendPiece("Setup autoWakeUp",this);
    PageAppendPiece("Setup Unit", this);

}

void ShowPasswdDialog(void)
{

    if(PasswdDialog == NULL) {
        PasswdDialog = new Window_Passwd(0, QObject::tr("Please Input Passwd"), ":/resource/test.png");

    }
    PasswdDialog->showNote(true);
    //delete PasswdDialog;
    //PasswdDialog = NULL;
}

void WidgetSetupOther::SlotSceneActivate(int id)
{
    switch (id) {
        case 1:
            PageShow("Setup System Time");
            break;
        case 2:
            PageShow("Setup Network");  //HJX网络模块禁止#/
            break;
        case 3:
            PageShow("Setup Lang");
            break;
        case 4:
            PageShow("Setup System Info");
            break;
        case 5:
            PageShow("Maintain");
            break;
        case 6:
            if(alarmmodule!=NULL)
                alarmmodule->Set_WorkState(DE_HIDDED_FLAG);
            PageShow("Status Graph");
            break;
        case 7:
            PageShow("RFCard");
            break;
        case 8:
            PageShow("Setup Unit");
            break;
        case 9:
            PageShow("Setup autoWakeUp");
            break;
        case 10:
            ShowPasswdDialog();  //进入工程模式设置界面,密码框#/
            //enginerDialog->show();
            break;
        default:
            break;
    }
    if(id!=10)
    {
        if(PasswdDialog!=NULL)
             PasswdDialog->setShown(false);
    }
}

void WidgetSetupOther::setSubpageHide()
{
    sceneSetupLang.setCusVisible(false);
    sceneSetupNetwork.setCusVisible(false); //HJX网络模块禁止#/
    sceneSetupSysinfo.setCusVisible(false);
    sceneSetupSystemTime.setCusVisible(false);
//    patientMessage.setCusVisible(false);
}

void WidgetSetupOther::setCusVisible(bool vis, char* para)
{
    char *pageName = NULL;
    para = para;
    widget.setVisible(vis);
    labelTitle.setVisible(vis);

    if(vis==false)
    {
        if(PasswdDialog!=NULL)
             PasswdDialog->setShown(false);
    }

    if(vis){//显示默认页面或上一次页面
        pageName = PageCurrent();
        if (strcmp(pageName, "Setup System Time") == 0) {
            labelTitle.setText(tr("Setup System Time"));
        }else if (strcmp(pageName, "Setup Network") == 0) {
            labelTitle.setText(tr("Setup Network"));
        }else if (strcmp(pageName, "Setup Lang") == 0) {
            labelTitle.setText(tr("Setup Lang"));
        }else if (strcmp(pageName, "Setup System Info") == 0) {
            labelTitle.setText(tr("Setup System Info"));
        }else if (strcmp(pageName, "Maintain") == 0) {
            labelTitle.setText(tr("Maintain"));
        }else if (strcmp(pageName, "Status Graph") == 0) {
            labelTitle.setText(tr("Status Graph"));
        }else if (strcmp(pageName, "RFCard") == 0) {
 //           labelTitle.setText(tr("patientMessage"));
        }else if(strcmp(pageName, "Setup Unit") == 0) {
            labelTitle.setText(tr("Setup Unit"));
        }else if(strcmp(pageName, "Setup autoWakeUp") == 0) {
            labelTitle.setText(tr("Setup Auto-wake-up"));
        }else {
            labelTitle.setText(tr("Setup Password"));
        }
        int id = buttonGroupSetup.checkedId();
        QAbstractButton * button = buttonGroupSetup.checkedButton();
        if(id == -1||id == 6 || id ==5 ){//显示默认界面
            if(id != -1)button->setChecked(false);
            buttonSystemTime.setChecked(true);
            id=1;
        }
    SlotSceneActivate( id );
    }
}

void WidgetSetupOther::CustomSetText()
{
    labelTitle.setText(tr("Other"));
    buttonSystemTime.setText(tr("System Time"));
    buttonSetupNetwork.setText(tr("Network"));
    buttonSetupLang.setText(tr("Language"));
    buttonSystemInfo.setText(tr("System Info"));
    buttonMaintain.setText(tr("Maintain"));
    buttonStatusMap.setText(tr("Status Map"));
//    buttonRFCard.setText((tr("patientMessage")));
    buttonEngineeringMode.setText((tr("Engineer")));
    buttonUnit.setText(tr("Unit"));
    buttonautoWakeUp.setText(tr("Auto-wake-up"));
}
