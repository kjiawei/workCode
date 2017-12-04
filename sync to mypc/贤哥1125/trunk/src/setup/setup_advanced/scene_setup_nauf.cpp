/*****************************************************
** 作者： Huangjunxian
** 创始时间：2014-11-18
** 修改人：Huangjunxian
** 修改时间：2014-11-26
** 描述：
**    Na UF曲线缩略图选择按钮 《源文件》
*****************************************************/
#include "scene_setup_layout.h"
#include "scene_setup_nauf.h"
#include "curve_datahandle.h"
#include "common_tr_string.h"


#define SCENE_SETUP_BUTTON_CURVE_WIDTH          70
#define SCENE_SETUP_BUTTON_CURVE_HIGH           50
#define CURVE_X_NUM 165
#define CURVE_Y_NUM 130

inline void SceneSetupNaUF::setUi()
{
    labelNa.setGeometry(QRect(SCENE_SETUP_COL_LABEL+75, 80,
                SCENE_SETUP_COL_WIDTH_LABEL, SCENE_SETUP_WIDGET_HIGH));
    labelNa.setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelNa.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelNa.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelUF.setGeometry(QRect(SCENE_SETUP_COL_LABEL+75, 330,
                SCENE_SETUP_COL_WIDTH_LABEL, SCENE_SETUP_WIDGET_HIGH));
    labelUF.setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelUF.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelUF.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    int x=CURVE_X_NUM,y=CURVE_Y_NUM;
    for(int i=0;i<16;i++)
    {
        if(i%8==0&&i!=0)
        {
            x=CURVE_X_NUM;
            y+=90;
        }
        labelNaText[i]->setGeometry(x,y+45,80,30);
        labelNaText[i]->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        labelNaText[i]->setFont(QFont(QString(CONFIG_FONT_STYLE),
                    CFG_FONT_SIZE_FOURTH, CONFIG_FONT_NORMAL));
        labelNaText[i]->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

        labelUfText[i]->setGeometry(x,y+250+45,80,30);
        labelUfText[i]->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        labelUfText[i]->setFont(QFont(QString(CONFIG_FONT_STYLE),
                    CFG_FONT_SIZE_FOURTH, CONFIG_FONT_NORMAL));
        labelUfText[i]->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

        x += 85;
    }

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/resource/images/setup/rim.png")));
    //选择图形按钮时，添加的外边框#/
    widgetBackNa.setAutoFillBackground(true);
    widgetBackNa.setPalette(palette);
    widgetBackNa.setFixedSize(80, 60);
    widgetBackNa.setVisible(false);
    widgetBackUf.setAutoFillBackground(true);
    widgetBackUf.setPalette(palette);
    widgetBackUf.setFixedSize(80, 60);
    widgetBackUf.setVisible(false);

    buttonGroupNa.addButton(&buttonNaPre1,0);
    buttonGroupNa.addButton(&buttonNaPre2,1);
    buttonGroupNa.addButton(&buttonNaPre3,2);
    buttonGroupNa.addButton(&buttonNaPre4,3);
    buttonGroupNa.addButton(&buttonNaPre5,4);
    buttonGroupNa.addButton(&buttonNaPre6,5);
    buttonGroupNa.addButton(&buttonNaPre7,6);
    buttonGroupNa.addButton(&buttonNaPre8,7);
    buttonGroupNa.addButton(&buttonNaCus1,8);
    buttonGroupNa.addButton(&buttonNaCus2,9);
    buttonGroupNa.addButton(&buttonNaCus3,10);
    buttonGroupNa.addButton(&buttonNaCus4,11);
    buttonGroupNa.addButton(&buttonNaCus5,12);
    buttonGroupNa.addButton(&buttonNaCus6,13);
    buttonGroupNa.addButton(&buttonNaCus7,14);
    buttonGroupNa.addButton(&buttonNaCus8,15);

    buttonGroupUf.addButton(&buttonUfPre1,0);
    buttonGroupUf.addButton(&buttonUfPre2,1);
    buttonGroupUf.addButton(&buttonUfPre3,2);
    buttonGroupUf.addButton(&buttonUfPre4,3);
    buttonGroupUf.addButton(&buttonUfPre5,4);
    buttonGroupUf.addButton(&buttonUfPre6,5);
    buttonGroupUf.addButton(&buttonUfPre7,6);
    buttonGroupUf.addButton(&buttonUfPre8,7);
    buttonGroupUf.addButton(&buttonUfCus1,8);
    buttonGroupUf.addButton(&buttonUfCus2,9);
    buttonGroupUf.addButton(&buttonUfCus3,10);
    buttonGroupUf.addButton(&buttonUfCus4,11);
    buttonGroupUf.addButton(&buttonUfCus5,12);
    buttonGroupUf.addButton(&buttonUfCus6,13);
    buttonGroupUf.addButton(&buttonUfCus7,14);
    buttonGroupUf.addButton(&buttonUfCus8,15);

    //按键位置和大小#/
    buttonEdit.setGeometry(QRect(550, 600, 80, 40));
    buttonApp.setGeometry(QRect(650, 600, 80, 40));

    //按键样式和信号槽#/
    BUTTON_SIMPLE_STYLE(buttonEdit);
    BUTTON_SIMPLE_STYLE(buttonApp);

    chooseNa = -1;
    chooseUF = -1;
    CustomSetText();
}

SceneSetupNaUF::SceneSetupNaUF(QWidget *parent) :
    QObject(parent),
    labelNa(parent),
    widgetBackNa(parent),
    buttonGroupNa(parent),
    buttonNaPre1(parent),
    buttonNaPre2(parent),
    buttonNaPre3(parent),
    buttonNaPre4(parent),
    buttonNaPre5(parent),
    buttonNaPre6(parent),
    buttonNaPre7(parent),
    buttonNaPre8(parent),
    buttonNaCus1(parent),
    buttonNaCus2(parent),
    buttonNaCus3(parent),
    buttonNaCus4(parent),
    buttonNaCus5(parent),
    buttonNaCus6(parent),
    buttonNaCus7(parent),
    buttonNaCus8(parent),

    labelUF(parent),
    widgetBackUf(parent),
    buttonGroupUf(parent),
    buttonUfPre1(parent),
    buttonUfPre2(parent),
    buttonUfPre3(parent),
    buttonUfPre4(parent),
    buttonUfPre5(parent),
    buttonUfPre6(parent),
    buttonUfPre7(parent),
    buttonUfPre8(parent),
    buttonUfCus1(parent),
    buttonUfCus2(parent),
    buttonUfCus3(parent),
    buttonUfCus4(parent),
    buttonUfCus5(parent),
    buttonUfCus6(parent),
    buttonUfCus7(parent),
    buttonUfCus8(parent),

    buttonEdit(parent),
    buttonApp(parent)
{
    //根据钠曲线数据文件进行初始化#/
    curveDataObject.initNaCurve();
    //根据超滤曲线数据文件进行初始化#/
    curveDataObject.initUfCurve();
    int i=0;
    for(i=0;i<16;i++)
    {
        labelNaText[i]=new QLabel(parent);
        labelUfText[i]=new QLabel(parent);
    }

    setUi();
    connect(&buttonGroupNa, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(CurveNaActivate(QAbstractButton*)));
//    connect(&buttonGroupUf, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(CurveUfActivate(QAbstractButton*)));
    connect(&buttonEdit,SIGNAL(clicked()),this,SLOT(buttonEditClicked()));
    connect(&buttonApp,SIGNAL(clicked()),this,SLOT(buttonAppClicked()));
    PageAppendPiece("Setup NaProfile", this);
}

//选择纳曲线时#/
void SceneSetupNaUF::CurveNaActivate(QAbstractButton *button)
{
    widgetBackNa.move(button->x()-5, button->y()-5);
    chooseNa = buttonGroupNa.id(button);
    widgetBackNa.setVisible(true);
    //消除UF曲线的选择框#/
    if(chooseUF>=0)
    {
        chooseUF=-1;
        widgetBackUf.setVisible(false);
    }
}

//选择超滤曲线时#/
void SceneSetupNaUF::CurveUfActivate(QAbstractButton *button)
{
    widgetBackUf.move(button->x()-5, button->y()-5);
    chooseUF = buttonGroupUf.id(button);
    widgetBackUf.setVisible(true);
    if(chooseNa>=0)
    {
        chooseNa=-1;
        widgetBackNa.setVisible(false);
    }
}

//编辑按钮点击#/
void SceneSetupNaUF::buttonEditClicked()
{
//    if(chooseNa<0&&chooseUF<0)
    if(chooseNa<0)
    {
        MESSAGE_PROMPT_BOX(DE_TR_PROMPT,DE_TR_PLEASESELECT);
        return;
    }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupNaUF::buttonEditClicked-in");
#endif

    if(chooseNa>=0)
    {
        PageShow("Setup CurveNa");
        emit showNaCurve(chooseNa);
    }
/*
    if(chooseUF>=0)
    {
        PageShow("Setup CurveUF");
        emit showUfCurve(chooseUF);
    }
    */

#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupNaUF::buttonEditClicked-out");
#endif

//    emit setWidgetShow(false);//隐藏菜单栏#/
}

//应用按钮点击#/
void SceneSetupNaUF::buttonAppClicked()
{
//    if(chooseNa<0&&chooseUF<0)
    if(chooseNa<0)
    {
        MESSAGE_PROMPT_BOX(DE_TR_PROMPT,DE_TR_PLEASESELECT);
        return;
    }
    else
    {

        QString str;
        int i=0;

        if(residue_secCount<3600)//小于60分钟不给应用#/
        {
            str=DE_TR_CANNOTLESS;
            str += QString(" 60 ")+DE_TR_MINTUES;
            MESSAGE_PROMPT_BOX(DE_TR_PROMPT,str);
            return;
        }

        if(chooseNa==G_NA_SELECTCURVE_NUM&&G_NA_PROFILE_FLAG==true)  //该曲线正在应用,不能重复#/
        {
            str=DE_TR_THISCURVEISRUNING;
            MESSAGE_PROMPT_BOX(DE_TR_PROMPT,str);
            return;
        }

        if(curveDataObject.checkCurve(NACURVETYPE,chooseNa))   //读数据文件#/
            curveDataObject.readCurve(NACURVETYPE,chooseNa,(char *)naParaArray,sizeof(Curve_NaPara)*10);
        else
        {
            str=DE_TR_THISCURVEISUNDEFINED;
            MESSAGE_PROMPT_BOX(DE_TR_PROMPT,str);
            return;
        }


        //确定是否应用#/
        QMessageBox box(QMessageBox::Question,DE_TR_PROMPT,DE_TR_AREYOUSURE);
        QAbstractButton *bno =box.addButton(DE_TR_NO,QMessageBox::YesRole);
        box.addButton(DE_TR_YES,QMessageBox::YesRole);
        box.setWindowFlags(Qt::Widget);
        box.move(300,300);
        //消息框运行，等待点击按键退出#/
        box.exec();
        if (box.clickedButton() == bno)
            return;

        //曲线应用处理#/
        for(i=0;i<10;i++)
        {
            A_NA_PROFILE[i].flag=false;
            A_NA_PROFILE[i].navalue=naParaArray[i].navalue;
        }
        G_NA_PROFILE_FLAG=true;     //NA曲线开始标志#/
        G_NA_PROFILE_NUM=0;      //NA曲线从第几段开始#/
        G_NA_PROFILE_START=G_SYS_CURTIME;  //NA曲线开始时间#/
        G_NA_PROFILE_END=G_SYS_CURTIME+residue_secCount;    //NA曲线结束时间#/
        G_NA_SELECTCURVE_NUM=chooseNa;    //当前选择运行的曲线号#/

        MESSAGE_PROMPT_BOX(DE_TR_PROMPT,DE_TR_CURVERUNSUCCESS);
    }
}

void SceneSetupNaUF::setCusVisible(bool vis, char* para)
{
    para = para;

    int x=CURVE_X_NUM,y=CURVE_Y_NUM,i;
    WidgetButton *pbutton=NULL;
    QByteArray ba;

    if(vis==true)
    {
        //根据曲线数据文件是否存在 生成曲线图形按钮#/
        for(i=0;i<16;i++)
        {
            if(i%8==0&&i!=0)
            {
                x=CURVE_X_NUM;
                y+=90;
            }

            pbutton=(WidgetButton *)buttonGroupNa.button(i);//钠曲线按钮#/
            ba=curveDataObject.checkPngFile(NACURVETYPE,i);
            if(ba.isEmpty())
            {
                pbutton->setStyleNormal(":/resource/images/setup/curve_na_custom.bmp",
                         QRect(x, y, SCENE_SETUP_BUTTON_CURVE_WIDTH, SCENE_SETUP_BUTTON_CURVE_HIGH));
            }
            else
            {
                pbutton->setStyleNormal(ba.data(),
                         QRect(x, y, SCENE_SETUP_BUTTON_CURVE_WIDTH, SCENE_SETUP_BUTTON_CURVE_HIGH));
            }

            /*
            pbutton=(WidgetButton *)buttonGroupUf.button(i);//超滤曲线按钮#/
            ba=curveDataObject.checkPngFile(UFCURVETYPE,i);
            if(ba.isEmpty())
            {
                pbutton->setStyleNormal(":/resource/images/setup/curve_na_custom.bmp",
                         QRect(x, y+250, SCENE_SETUP_BUTTON_CURVE_WIDTH, SCENE_SETUP_BUTTON_CURVE_HIGH));
            }
            else
            {
                pbutton->setStyleNormal(ba.data(),
                         QRect(x, y+250, SCENE_SETUP_BUTTON_CURVE_WIDTH, SCENE_SETUP_BUTTON_CURVE_HIGH));
            }
            */

            x += 85;
        }//for(i=0;i<16;i++)
    }//if(vis==true)

    for(i=0;i<16;i++)
    {
        if(i<8)
        {
            labelNaText[i]->setText(DE_TR_DEFINE+QString().setNum(i));
            labelUfText[i]->setText(DE_TR_DEFINE+QString().setNum(i));
        }
        else
        {
            labelNaText[i]->setText(DE_TR_CUSTOM+QString().setNum(i));
            labelUfText[i]->setText(DE_TR_CUSTOM+QString().setNum(i));
        }

        labelNaText[i]->setVisible(vis);
        labelUfText[i]->setVisible(false);
    }

    if (chooseNa >=0) {
        widgetBackNa.setVisible(vis);
    }
    labelNa.setVisible(vis);
    buttonNaPre1.setVisible(vis);
    buttonNaPre2.setVisible(vis);
    buttonNaPre3.setVisible(vis);
    buttonNaPre4.setVisible(vis);
    buttonNaPre5.setVisible(vis);
    buttonNaPre6.setVisible(vis);
    buttonNaPre7.setVisible(vis);
    buttonNaPre8.setVisible(vis);
    buttonNaCus1.setVisible(vis);
    buttonNaCus2.setVisible(vis);
    buttonNaCus3.setVisible(vis);
    buttonNaCus4.setVisible(vis);
    buttonNaCus5.setVisible(vis);
    buttonNaCus6.setVisible(vis);
    buttonNaCus7.setVisible(vis);
    buttonNaCus8.setVisible(vis);

    if (chooseUF >=0) {
    widgetBackUf.setVisible(false);
    }
    labelUF.setVisible(false);
    buttonUfPre1.setVisible(false);
    buttonUfPre2.setVisible(false);
    buttonUfPre3.setVisible(false);
    buttonUfPre4.setVisible(false);
    buttonUfPre5.setVisible(false);
    buttonUfPre6.setVisible(false);
    buttonUfPre7.setVisible(false);
    buttonUfPre8.setVisible(false);
    buttonUfCus1.setVisible(false);
    buttonUfCus2.setVisible(false);
    buttonUfCus3.setVisible(false);
    buttonUfCus4.setVisible(false);
    buttonUfCus5.setVisible(false);
    buttonUfCus6.setVisible(false);
    buttonUfCus7.setVisible(false);
    buttonUfCus8.setVisible(false);

    buttonEdit.setVisible(vis);
    buttonApp.setVisible(vis);
}

void SceneSetupNaUF::CustomSetText()
{
#if EEDA_VERSION_OPEN
    labelNa.setText(tr("Na+ Profile"));
    labelUF.setText(tr("UF Profile"));
    buttonEdit.setText(DE_TR_EDIT);
    buttonApp.setText(DE_TR_APPLY);
#else

    labelNa.setText(DE_TR_NACURVE);
    labelUF.setText(DE_TR_UFCURVE);
    buttonEdit.setText(DE_TR_EDIT);
    buttonApp.setText(DE_TR_APPLY);
#endif

}
