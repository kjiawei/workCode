/*****************************************************
** 作者： Huangjunxian
** 创始时间：2014-11-18
** 修改人：Huangjunxian
** 修改时间：2014-11-26
** 描述：
**    UF曲线缩略图选择按钮 《源文件》
*****************************************************/
#include "scene_setup_layout.h"
#include "scene_setup_sequential.h"
#include "curve_datahandle.h"
#include "common_tr_string.h"

#define SCENE_SETUP_BUTTON_CURVE_WIDTH          70
#define SCENE_SETUP_BUTTON_CURVE_HIGH           50
#define CURVE_X_NUM 165
#define CURVE_Y_NUM 130

inline void SceneSetupSequential::setUi()
{
    labelUF.setGeometry(QRect(SCENE_SETUP_COL_LABEL+75, 80,
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
        labelUfText[i]->setGeometry(x,y+45,80,30);
        labelUfText[i]->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        labelUfText[i]->setFont(QFont(QString(CONFIG_FONT_STYLE),
                    CFG_FONT_SIZE_FOURTH, CONFIG_FONT_NORMAL));
        labelUfText[i]->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

        x += 85;
    }

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/resource/images/setup/rim.png")));
    //选择图形按钮时，添加的外边框
    widgetBackUf.setAutoFillBackground(true);
    widgetBackUf.setPalette(palette);
    widgetBackUf.setFixedSize(80, 60);
    widgetBackUf.setVisible(false);

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

    chooseUF = -1;
    CustomSetText();
}

SceneSetupSequential::SceneSetupSequential(QWidget *parent) :
    QObject(parent),
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

    for(int i=0;i<16;i++)
    {
        labelUfText[i]=new QLabel(parent);
    }

    setUi();
    connect(&buttonGroupUf, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(CurveUfActivate(QAbstractButton*)));
    connect(&buttonEdit,SIGNAL(clicked()),this,SLOT(buttonEditClicked()));
    connect(&buttonApp,SIGNAL(clicked()),this,SLOT(buttonAppClicked()));
    PageAppendPiece("Setup Sequential", this);
}

//选择超滤曲线时#/
void SceneSetupSequential::CurveUfActivate(QAbstractButton *button)
{
    widgetBackUf.move(button->x()-5, button->y()-5);
    chooseUF = buttonGroupUf.id(button);
    widgetBackUf.setVisible(true);
}

//编辑按钮点击#/
void SceneSetupSequential::buttonEditClicked()
{
    if(chooseUF<0)
    {
        MESSAGE_PROMPT_BOX(DE_TR_PROMPT,DE_TR_PLEASESELECT);
        return;
    }

    if(chooseUF>=0)
    {
        PageShow("Setup CurveUF");
        emit showUfCurve(chooseUF);
    }

//    emit setWidgetShow(false);//隐藏菜单栏#/
}

//应用按钮点击#/
void SceneSetupSequential::buttonAppClicked()
{
    if(chooseUF<0)
    {
        MESSAGE_PROMPT_BOX(DE_TR_PROMPT,DE_TR_PLEASESELECT);
        return;
    }
    else
    {
        QString str;
        int i=0;

        if(total_secCount<1800)//小于30分钟不给应用#/
        {
            str=DE_TR_CANNOTLESS;
            str += QString(" 30 ")+DE_TR_MINTUES;
            MESSAGE_PROMPT_BOX(DE_TR_PROMPT,str);
            return;
        }

        if(chooseUF==G_UF_SELECTCURVE_NUM&&G_UF_PROFILE_FLAG==true)  //该曲线正在应用,不能重复#/
        {
            str=DE_TR_THISCURVEISRUNING;
            MESSAGE_PROMPT_BOX(DE_TR_PROMPT,str);
            return;
        }

        if(curveDataObject.checkCurve(UFCURVETYPE,chooseUF))
            curveDataObject.readCurve(UFCURVETYPE,chooseUF,(char *)ufParaArray,sizeof(Curve_UfPara)*10);
        else
        {
            str=DE_TR_THISCURVEISUNDEFINED;
            MESSAGE_PROMPT_BOX(DE_TR_PROMPT,str);
            return;
        }

        QMessageBox box(QMessageBox::Question,DE_TR_PROMPT,DE_TR_AREYOUSURE);
        QAbstractButton *bno =box.addButton(DE_TR_NO,QMessageBox::YesRole);
        box.addButton(DE_TR_YES,QMessageBox::YesRole);
        box.setWindowFlags(Qt::Widget);
        box.move(300,300);
        //消息框运行，等待点击按键退出#/
        box.exec();
        if (box.clickedButton() == bno)
            return;

        float perufvol=0;
        perufvol=(G_UF_CURETARGET-G_UF_CUREFINISH)/10.0; //每段平均超滤量#/
        //曲线应用处理#/
        for(i=0;i<10;i++)
        {
            A_UF_PROFILE[i].flag=false;
            A_UF_PROFILE[i].seqmode=ufParaArray[i].seqmode;
            A_UF_PROFILE[i].ufrate=ufParaArray[i].ufrate;
            A_UF_PROFILE[i].ufvol=(ufParaArray[i].ufrate/100.0)*perufvol; //该时段的超滤目标量#/
        }

        G_UF_PROFILE_FLAG=true;  //UF曲线开始标志#/
        G_UF_PROFILE_STARTVOL=G_UF_CUREFINISH; //UF曲线时段初始超滤量#/
        G_UF_PROFILE_NUM=0;   //UF曲线从第几段开始#/
        G_UF_PROFILE_START=G_SYS_CURTIME;  //UF曲线开始时间#/
        G_UF_PROFILE_END=G_SYS_CURTIME+residue_secCount;   //UF曲线结束时间#/

        G_UF_SELECTCURVE_NUM=chooseUF;    //当前选择运行的曲线号#/

        MESSAGE_PROMPT_BOX(DE_TR_PROMPT,DE_TR_CURVERUNSUCCESS);
    }
}


void SceneSetupSequential::setCusVisible(bool vis, char* para)
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
            pbutton=(WidgetButton *)buttonGroupUf.button(i);//超滤曲线按钮#/
            ba=curveDataObject.checkPngFile(UFCURVETYPE,i);
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
            x += 85;
        }//for(i=0;i<16;i++)
    }//if(vis==true)

    for(i=0;i<16;i++)
    {
        if(i<8)
        {
            labelUfText[i]->setText(DE_TR_DEFINE+QString::number(i));
        }
        else
        {
            labelUfText[i]->setText(DE_TR_CUSTOM+QString::number(i));
        }

        labelUfText[i]->setVisible(vis);
    }

    if (chooseUF >=0) {
    widgetBackUf.setVisible(vis);
    }
    labelUF.setVisible(vis);
    buttonUfPre1.setVisible(vis);
    buttonUfPre2.setVisible(vis);
    buttonUfPre3.setVisible(vis);
    buttonUfPre4.setVisible(vis);
    buttonUfPre5.setVisible(vis);
    buttonUfPre6.setVisible(vis);
    buttonUfPre7.setVisible(vis);
    buttonUfPre8.setVisible(vis);
    buttonUfCus1.setVisible(vis);
    buttonUfCus2.setVisible(vis);
    buttonUfCus3.setVisible(vis);
    buttonUfCus4.setVisible(vis);
    buttonUfCus5.setVisible(vis);
    buttonUfCus6.setVisible(vis);
    buttonUfCus7.setVisible(vis);
    buttonUfCus8.setVisible(vis);

    buttonEdit.setVisible(vis);
    buttonApp.setVisible(vis);
}

void SceneSetupSequential::CustomSetText()
{
    labelUF.setText(DE_TR_UFCURVE);
    buttonEdit.setText(DE_TR_EDIT);
    buttonApp.setText(DE_TR_APPLY);
}
