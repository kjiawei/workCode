#include <stdio.h>
#include "widget_common.h"
#include "widget_setup_advanced.h"
#include "common_tr_string.h"

void WidgetSetupAdvanced::setUi()
{
    //左侧菜单栏显示框尺寸#/
    widget.setGeometry(QRect(Widget_Setup_Menu_X,
                Widget_Setup_Menu_Y,
                Widget_Setup_Menu_Width,
                Widget_Setup_Menu_High));
    widget.setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(widget.backgroundRole(),
            QBrush(QPixmap(":/resource/images/setup/menu_back.bmp")));
    widget.setPalette(palette);

    //菜单栏隐藏按钮位置尺寸，位于菜单栏顶部#/
    buttonListShow.setGeometry(QRect(0, 0, 124, 30));

    groupBox.setStyleSheet(" QGroupBox {"
                           "border: 0px solid gray;"
                           "border-radius: 0px;"
                           "margin-top: 0ex; /* leave space at the top for the title */"
                           "}"
                      );

    groupBox.setGeometry(QRect(0, 30, 147, 666));

    QVBoxLayout *verticalLayout = new QVBoxLayout(&groupBox);

    verticalLayout->addWidget(&buttonNaUf);
    verticalLayout->addWidget(&buttonSequent);
    verticalLayout->addWidget(&buttonConduction);
//    verticalLayout->addWidget(&buttonXmlconfig);
    verticalLayout->addWidget(&buttonPreflush);
   //verticalLayout->addWidget(&buttonTimer);
    verticalLayout->addWidget(&buttonDialysateFlow);
    verticalLayout->addWidget(&buttonHeparinCurve);
    verticalLayout->addWidget(&buttonCondCurve);
    verticalLayout->addWidget(&buttonTempCurve);
    verticalLayout->addWidget(&buttonHco3Curve);

    verticalLayout->addStretch();

    buttonListShow.setFixedSize(100, 30);
    buttonDialysateFlow.setFixedSize(124, 45);
    buttonNaUf.setFixedSize(124, 45);
    buttonSequent.setFixedSize(124, 45);
    buttonConduction.setFixedSize(124, 45);
//    buttonXmlconfig.setFixedSize(124, 45);

    buttonHeparinCurve.setFixedSize(124, 45);
    buttonPreflush.setFixedSize(124, 45);
    //buttonTimer.setFixedSize(124, 45);
    buttonCondCurve.setFixedSize(124,45);
    buttonTempCurve.setFixedSize(124,45);
    buttonHco3Curve.setFixedSize(124,45);


    buttonListShow.setStyleNormal(":/resource/images/setup/menu_button_normal.bmp");
    buttonListShow.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE,
            QRect(0, 0, buttonListShow.width(), buttonListShow.height()));

    buttonDialysateFlow.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonDialysateFlow.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonDialysateFlow.width(),
                buttonDialysateFlow.height()));

    buttonNaUf.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonNaUf.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonNaUf.width(),
                buttonNaUf.height()));

    buttonSequent.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonSequent.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonSequent.width(),
                buttonSequent.height()));

    buttonConduction.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonConduction.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonConduction.width(), buttonConduction.height()));


/*
    buttonXmlconfig.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonXmlconfig.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonXmlconfig.width(), buttonXmlconfig.height()));
*/

    buttonHeparinCurve.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonHeparinCurve.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonHeparinCurve.width(), buttonHeparinCurve.height()));

    buttonCondCurve.setTextFontPos(CFG_FONT_SIZE_SECOND,
                                      CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonCondCurve.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
                                      ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
                                      QRect(0, 0, buttonCondCurve.width(), buttonCondCurve.height()));

    buttonTempCurve.setTextFontPos(CFG_FONT_SIZE_SECOND,
                                      CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonTempCurve.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
                                      ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
                                      QRect(0, 0, buttonTempCurve.width(), buttonTempCurve.height()));

    buttonHco3Curve.setTextFontPos(CFG_FONT_SIZE_SECOND,
                                      CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonHco3Curve.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
                                      ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
                                      QRect(0, 0, buttonHco3Curve.width(), buttonHco3Curve.height()));

    buttonPreflush.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonPreflush.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonPreflush.width(), buttonPreflush.height()));

    /*buttonTimer.setTextFontPos(CFG_FONT_SIZE_SECOND,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_WHITE);
    buttonTimer.setStyleSwitch(":/resource/images/setup/menu_button_normal.bmp",
            ":/resource/images/setup/menu_button_active.png", CONFIG_COLOR_BLUE,
            QRect(0, 0, buttonTimer.width(), buttonTimer.height()));*/



    buttonGroupSetup.addButton(&buttonNaUf, 2);
    buttonGroupSetup.addButton(&buttonSequent, 3);
    buttonGroupSetup.addButton(&buttonConduction, 4);
    buttonGroupSetup.addButton(&buttonPreflush, 5);
//    buttonGroupSetup.addButton(&buttonXmlconfig, 6);
    buttonGroupSetup.addButton(&buttonDialysateFlow, 7);
    buttonGroupSetup.addButton(&buttonHeparinCurve,8);
    buttonGroupSetup.addButton(&buttonCondCurve,9);
    buttonGroupSetup.addButton(&buttonTempCurve,10);
    buttonGroupSetup.addButton(&buttonHco3Curve,11);

    labelTitle.setGeometry(QRect(34, 21, 300, 30));
    LABEL_TITLE_STYLE(labelTitle);
    //labelTitle.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    //labelTitle.setStyleSheet("color:#dbe6ef;font-size:20px;Bold");

    CustomSetText();
}


//点击菜单栏隐藏按钮#/
void WidgetSetupAdvanced::buttonShowClicked(int isHide)
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
                    30)); ///减小高度#/
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

WidgetSetupAdvanced::WidgetSetupAdvanced(QWidget *parent) :
    QObject(parent),
    sceneSetupEC(parent),
    sceneSetupNaUF(parent),
    sceneSetupUnit(parent),
    sceneSetupCurveNa(parent),
    sceneSetupCurveUF(parent),
    //sceneSetupSpO2(parent),
   // sceneSetupBloodTemp(parent),
    sceneSetupSequential(parent),
    sceneSetupDiaFlowCurve(parent),
    //sceneSetupBPM(parent),
 //   sceneSetupXmlconfig(parent),
    sceneSetupHeparinCur(parent),
    sceneSetupCondCurve(parent),
    sceneSetupTempCurve(parent),
    sceneSetupHco3Curve(parent),
    sceneSetupCurveTest(parent),
    sceneSetupPreflush(parent),
    //sceneSetupAdvertTime(parent),
    widget(parent),
    labelTitle(parent),
    buttonListShow(&widget),
    buttonGroupSetup(&widget),
    groupBox(&widget),
    buttonNaUf(&widget),
    buttonSequent(&widget),
    buttonConduction(&widget),
//    buttonXmlconfig(&widget),
    buttonPreflush(&widget),
    buttonDialysateFlow(&widget),
    buttonHeparinCurve(&widget),
    buttonCondCurve(&widget),
    buttonTempCurve(&widget),
    buttonHco3Curve(&widget)
   // buttonTimer(&widget),

{
    setUi();
    //菜单栏隐藏按钮#/
    //connect(&buttonListShow, SIGNAL(clicked()), this, SLOT(buttonShowClicked()));
    //菜单按钮点击信号#/
    connect(&buttonGroupSetup, SIGNAL(buttonClicked(int)), this, SLOT(SetupMenuActivate(int)));
    //菜单栏隐藏信号#/
    connect(&sceneSetupNaUF,SIGNAL(setWidgetShow(bool)),&widget,SLOT(setVisible(bool)));
    //传递选择的曲线号#/
    connect(&sceneSetupNaUF,SIGNAL(showNaCurve(int)),&sceneSetupCurveNa,SLOT(selectedCurve(int)));
    connect(&sceneSetupNaUF,SIGNAL(showUfCurve(int)),&sceneSetupCurveUF,SLOT(selectedCurve(int)));
    //菜单栏隐藏信号#/
    connect(&sceneSetupSequential,SIGNAL(setWidgetShow(bool)),&widget,SLOT(setVisible(bool)));
    //传递选择的曲线号#/
    connect(&sceneSetupSequential,SIGNAL(showUfCurve(int)),&sceneSetupCurveUF,SLOT(selectedCurve(int)));

    //以下目的是：刷新显示菜单指向的子页面之前，先刷新显示本页面#/
    //这样可以避免从子页面直接跳转到主页面时，菜单栏不隐藏#/
    //也让从其他页面返回本页面时，能保持之前点击的按钮#/
    PageAppendPiece("SetupAdvanced", this);
    PageAppendPiece("Setup Cond", this);
    PageAppendPiece("Setup NaProfile", this);

    //some things will happy error
//    PageAppendPiece("Setup CurveNa", this);
    PageAppendPiece("Setup CurveUF", this);

    PageAppendPiece("Setup Sequential", this);
    PageAppendPiece("Setup Bicarbonate", this);
    //PageAppendPiece("Setup BPM", this);
    PageAppendPiece("Setup XmlConfig", this);
    PageAppendPiece("Setup CurveTest", this);
    PageAppendPiece("Setup Preflush",this);
    PageAppendPiece("Setup DiaFlowCurve",this);
    PageAppendPiece("Setup HeparinCurve",this);
    PageAppendPiece("Setup CondCurve",this);
    PageAppendPiece("Setup TempCurve",this);
    PageAppendPiece("Setup Hco3Curve",this);
    //PageAppendPiece("Setup Timer", this);

}

void WidgetSetupAdvanced::SetupMenuActivate(int id)
{
    switch (id) {
        case 2:
            PageShow("Setup NaProfile");
            break;
        case 3:
            PageShow("Setup Sequential");
            break;
        case 4:
            PageShow("Setup Cond");
            break;
        case 5:
            PageShow("Setup Preflush");
            break;
        case 6:
            PageShow("Setup XmlConfig");
        break;
    case 7:
            widget.setVisible(false);
            PageShow("Setup DiaFlowCurve");
        break;
    case 8:
            widget.setVisible(false);
            PageShow("Setup HeparinCurve");
        break;
    case 9:
            widget.setVisible(false);
            PageShow("Setup CondCurve");
        break;
   case 10:
            widget.setVisible(false);
            PageShow("Setup TempCurve");
        break;
   case 11:
            widget.setVisible(false);
            PageShow("Setup Hco3Curve");
        break;
    default: break;

    }
}

void WidgetSetupAdvanced::setCusVisible(bool vis, char* para)
{
    char *pageName = NULL;
    para = para;

#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetSetupAdvanced::setCusVisible-in");
#endif

    buttonConduction.setVisible(false);

    labelTitle.setVisible(vis);
    widget.setVisible(vis);

    //从主页面回来时，根据当前页面名称还原显示#/
    if (vis == true) {
        pageName = PageCurrent();   //?????segmentation#/
        labelTitle.setText(tr(pageName));//设置标题栏字符串#/

        if (strcmp(pageName, "Setup DiaFlowCurve") == 0) {
            buttonDialysateFlow.setChecked(true);
            labelTitle.setText(DE_TR_DIAFLOWPROFILE);
        }else if (strcmp(pageName, "Setup NaProfile") == 0) {
            buttonNaUf.setChecked(true);
            labelTitle.setText(tr("Setup NaProfile"));
        }else if (strcmp(pageName, "Setup Sequential") == 0) {
            buttonSequent.setChecked(true);
            labelTitle.setText(tr("Setup Sequential"));
        }else if (strcmp(pageName, "Setup Cond") == 0) {
            buttonConduction.setChecked(true);
            labelTitle.setText(tr("Setup Cond"));
        }else if(strcmp(pageName, "Setup XmlConfig") == 0) {
//            buttonXmlconfig.setChecked(true);
            labelTitle.setText(tr("Setup XmlConfig"));
        }else if(strcmp(pageName, "Setup HeparinCurve") == 0) {
            buttonHeparinCurve.setChecked(true);
            labelTitle.setText(DE_TR_HEPARINPROFILEE);
        }else if(strcmp(pageName, "Setup Preflush") == 0) {
            buttonPreflush.setChecked(true);
            labelTitle.setText(tr("Setup Preflush"));
        }else if(strcmp(pageName,"Setup CondCurve")== 0 ) {
            buttonCondCurve.setChecked(true);
            labelTitle.setText(DE_TR_CONDPROFILE);
        }else if(strcmp(pageName,"Setup TempCurve")== 0 ) {
            buttonTempCurve.setChecked(true);
            labelTitle.setText(DE_TR_TEMPPROFILE);
        }else if(strcmp(pageName,"Setup Hco3Curve")== 0 ) {
            buttonHco3Curve.setChecked(true);
            labelTitle.setText(DE_TR_HCO3PROFILE);
        }

        int id = buttonGroupSetup.checkedId();
        if(id == -1 ){
            //buttonBicarbonate.setChecked(true);
            id=2;
        }

        if((strcmp(pageName, "Setup CurveNa") == 0)||(strcmp(pageName, "Setup CurveUF")==0)\
                )
            widget.setVisible(false);
        else
            SetupMenuActivate(id);


        if(engineer.factoryset.naprofileflag==true)
        {
            buttonNaUf.setVisible(vis);
        }
        else
        {
             buttonNaUf.setVisible(false);
        }

        if(engineer.factoryset.ufprofileflag==true)
        {
            buttonSequent.setVisible(vis);
        }
        else
        {
            buttonSequent.setVisible(false);
        }

        if(engineer.factoryset.diaprofileflag==true)
        {
            buttonDialysateFlow.setVisible(vis);
        }
        else
        {
            buttonDialysateFlow.setVisible(false);
        }

        if(engineer.factoryset.tempprofileflag==true)
        {
           buttonTempCurve.setVisible(vis);
        }
        else
        {
            buttonTempCurve.setVisible(false);
        }

        if(engineer.factoryset.condprofileflag==true)
        {
            buttonCondCurve.setVisible(vis);
        }
        else
        {
            buttonCondCurve.setVisible(false);
        }

        if(engineer.factoryset.hepprofileflag==true)
        {
            buttonHeparinCurve.setVisible(vis);
        }
        else
        {
            buttonHeparinCurve.setVisible(false);
        }

        if(engineer.factoryset.hco3profileflag==true)
        {
            buttonHco3Curve.setVisible(vis);
        }
        else
        {
            buttonHco3Curve.setVisible(false);
        }

    }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetSetupAdvanced::setCusVisible-out");
#endif
}

//设置菜单按键名称#/
void WidgetSetupAdvanced::CustomSetText()
{
    buttonListShow.setText(tr(" "));
    buttonDialysateFlow.setText(DE_TR_DIAFLOWPROFILE);
    buttonNaUf.setText(tr("Na Profile"));
    buttonSequent.setText(tr("Sequential"));
    buttonConduction.setText(tr("Cond"));

    buttonPreflush.setText(tr("Preflush"));
    //buttonTimer.setText(tr("Timer"));
//    buttonXmlconfig.setText(tr("XMLConfig"));
    buttonHeparinCurve.setText(DE_TR_HEPARINPROFILEE);
    buttonCondCurve.setText(DE_TR_CONDPROFILE);
    buttonTempCurve.setText(DE_TR_TEMPPROFILE);
    buttonHco3Curve.setText(DE_TR_HCO3PROFILE);
}
