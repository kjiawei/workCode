#include "config_ui.h"
#include "common_func.h"
#include "scene_setup_layout.h"
#include "scene_setup_otheroper.h"
#include "common_tr_string.h"

inline void SceneSetupOherOper::setUi()
{

    labelInformation.setGeometry(380,200,600,35);
    labelInformation.setAlignment( Qt::AlignLeft);
    labelInformation.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelInformation.setStyleSheet(CONFIG_COLOR_RED_CHARS);

    buttonStandby01.setGeometry(400,360,120,50);
    buttonStandby02.setGeometry(400,460,120,50);

    CustomSetText();
}

SceneSetupOherOper::SceneSetupOherOper(QWidget *parent) :
    QObject(parent),
    labelInformation(parent),
    buttonStandby01(parent),
    buttonStandby02(parent)
{
    setUi();
    connect(&buttonStandby01, SIGNAL(clicked()), this, SLOT(buttonStandby01_clicked()));
    connect(&buttonStandby02, SIGNAL(clicked()), this, SLOT(buttonStandby02_clicked()));
    PageAppendPiece("SceneSetupOherOper", this);

    buttonStandby01_flag=false;
    buttonStandby02_flag=false;
}

void SceneSetupOherOper::CustomSetText()
{
    labelInformation.setText(DE_TR_NOSTANDBYMODE);
    buttonStandby01.setText(DE_TR_STANDBYMODE01);
    buttonStandby02.setText(DE_TR_STANDBYMODE02);
}

void SceneSetupOherOper::setCusVisible(bool vis, char* para)
{
    para = para;
    labelInformation.setVisible(vis);
    buttonStandby01.setVisible(vis);
    buttonStandby02.setVisible(vis);
}

void SceneSetupOherOper::buttonStandby01_clicked()
{
    QString str1;
    if(buttonStandby01_flag==false)
    {
        buttonStandby01_flag=true;
   //     buttonStandby01.setChecked(true);
        buttonStandby02.setDisabled(true);
        str1=DE_TR_STANDBYMODE01;
        str1.append(DE_TR_KEYRUN);
    }
    else if(buttonStandby01_flag==true)
    {
        buttonStandby01_flag=false;
    //    buttonStandby01.setChecked(false);
        buttonStandby02.setDisabled(false);
        str1=DE_TR_NOSTANDBYMODE;
    }
    labelInformation.setText(str1);
}

void SceneSetupOherOper::buttonStandby02_clicked()
{
    QString str1;
    if(buttonStandby02_flag==false)
    {
        buttonStandby02_flag=true;
   //     buttonStandby02.setChecked(true);
        buttonStandby01.setDisabled(true);
        str1=DE_TR_STANDBYMODE02;
        str1.append(DE_TR_KEYRUN);
    }
    else if(buttonStandby02_flag==true)
    {
        buttonStandby02_flag=false;
   //     buttonStandby02.setChecked(false);
        buttonStandby01.setDisabled(false);
        str1=DE_TR_NOSTANDBYMODE;
    }
    labelInformation.setText(str1);

}
