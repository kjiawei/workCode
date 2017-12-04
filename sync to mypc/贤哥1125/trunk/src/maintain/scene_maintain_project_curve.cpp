
#include "config_ui.h"
#include "scene_setup_layout.h"
#include "scene_maintain_project_curve.h"
#include "common_tr_string.h"
#include "xml_parse.h"
#include "common_func.h"
#include "scene_setup_curve_na.h"
#include "scene_setup_curve_uf.h"


#ifdef HW_ARM

#include <sys/stat.h>
#include <signal.h>
#include <wait.h>
#endif

inline void SceneProjectModeCurve::setUi()
{
    labelCurveNaTitle.setGeometry(MAINTAIN_CURVE_CURVEUF_X,MAINTAIN_CURVE_CURVEUF_Y,200,MAINTAIN_CURVE_CURVEUF_HIGH);
    labelCurveNaTitle.setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelCurveNaTitle.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelCurveNaTitle.setStyleSheet(CONFIG_COLOR_BLACK_CHARS);

    labelCurveUFTitle.setGeometry(MAINTAIN_CURVE_CURVEUF_X,MAINTAIN_CURVE_CURVEUF_Y+150,200,MAINTAIN_CURVE_CURVEUF_HIGH);
    labelCurveUFTitle.setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelCurveUFTitle.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelCurveUFTitle.setStyleSheet(CONFIG_COLOR_BLACK_CHARS);

    buttonCurveNa01.setGeometry(QRect(MAINTAIN_CURVE_CURVEUF_X,MAINTAIN_CURVE_CURVEUF_Y+50,80,39));
    buttonCurveNa02.setGeometry(QRect(MAINTAIN_CURVE_CURVEUF_X+100,MAINTAIN_CURVE_CURVEUF_Y+50,80,39));
    buttonCurveNa03.setGeometry(QRect(MAINTAIN_CURVE_CURVEUF_X+200,MAINTAIN_CURVE_CURVEUF_Y+50,80,39));
    buttonCurveNa04.setGeometry(QRect(MAINTAIN_CURVE_CURVEUF_X+300,MAINTAIN_CURVE_CURVEUF_Y+50,80,39));
    buttonCurveNa05.setGeometry(QRect(MAINTAIN_CURVE_CURVEUF_X+400,MAINTAIN_CURVE_CURVEUF_Y+50,80,39));
    buttonCurveNa06.setGeometry(QRect(MAINTAIN_CURVE_CURVEUF_X+500,MAINTAIN_CURVE_CURVEUF_Y+50,80,39));
    buttonCurveNa07.setGeometry(QRect(MAINTAIN_CURVE_CURVEUF_X+600,MAINTAIN_CURVE_CURVEUF_Y+50,80,39));


    buttonCurveUf01.setGeometry(QRect(MAINTAIN_CURVE_CURVEUF_X,MAINTAIN_CURVE_CURVEUF_Y+200,80,39));
    buttonCurveUf02.setGeometry(QRect(MAINTAIN_CURVE_CURVEUF_X+100,MAINTAIN_CURVE_CURVEUF_Y+200,80,39));
    buttonCurveUf03.setGeometry(QRect(MAINTAIN_CURVE_CURVEUF_X+200,MAINTAIN_CURVE_CURVEUF_Y+200,80,39));
    buttonCurveUf04.setGeometry(QRect(MAINTAIN_CURVE_CURVEUF_X+300,MAINTAIN_CURVE_CURVEUF_Y+200,80,39));
    buttonCurveUf05.setGeometry(QRect(MAINTAIN_CURVE_CURVEUF_X+400,MAINTAIN_CURVE_CURVEUF_Y+200,80,39));
    buttonCurveUf06.setGeometry(QRect(MAINTAIN_CURVE_CURVEUF_X+500,MAINTAIN_CURVE_CURVEUF_Y+200,80,39));
    buttonCurveUf07.setGeometry(QRect(MAINTAIN_CURVE_CURVEUF_X+600,MAINTAIN_CURVE_CURVEUF_Y+200,80,39));

    CustomSetText();
}

SceneProjectModeCurve::SceneProjectModeCurve(QWidget *parent) :
    QObject(parent),
    labelCurveNaTitle(parent),
    labelCurveUFTitle(parent),
    buttonCurveNa01(parent),
    buttonCurveNa02(parent),
    buttonCurveNa03(parent),
    buttonCurveNa04(parent),
    buttonCurveNa05(parent),
    buttonCurveNa06(parent),
    buttonCurveNa07(parent),

    buttonCurveUf01(parent),
    buttonCurveUf02(parent),
    buttonCurveUf03(parent),
    buttonCurveUf04(parent),
    buttonCurveUf05(parent),
    buttonCurveUf06(parent),
    buttonCurveUf07(parent)
{
    setUi();
    PageAppendPiece("Maintain Curve", this);
    connect(&buttonCurveNa01, SIGNAL(clicked()), this, SLOT(on_buttonCurveNa01_clicked()));
    connect(&buttonCurveNa02, SIGNAL(clicked()), this, SLOT(on_buttonCurveNa02_clicked()));
    connect(&buttonCurveNa03, SIGNAL(clicked()), this, SLOT(on_buttonCurveNa03_clicked()));
    connect(&buttonCurveNa04, SIGNAL(clicked()), this, SLOT(on_buttonCurveNa04_clicked()));
    connect(&buttonCurveNa05, SIGNAL(clicked()), this, SLOT(on_buttonCurveNa05_clicked()));
    connect(&buttonCurveNa06, SIGNAL(clicked()), this, SLOT(on_buttonCurveNa06_clicked()));
    connect(&buttonCurveNa07, SIGNAL(clicked()), this, SLOT(on_buttonCurveNa07_clicked()));

    connect(&buttonCurveUf01, SIGNAL(clicked()), this, SLOT(on_buttonCurveUf01_clicked()));
    connect(&buttonCurveUf02, SIGNAL(clicked()), this, SLOT(on_buttonCurveUf02_clicked()));
    connect(&buttonCurveUf03, SIGNAL(clicked()), this, SLOT(on_buttonCurveUf03_clicked()));
    connect(&buttonCurveUf04, SIGNAL(clicked()), this, SLOT(on_buttonCurveUf04_clicked()));
    connect(&buttonCurveUf05, SIGNAL(clicked()), this, SLOT(on_buttonCurveUf05_clicked()));
    connect(&buttonCurveUf06, SIGNAL(clicked()), this, SLOT(on_buttonCurveUf06_clicked()));
    connect(&buttonCurveUf07, SIGNAL(clicked()), this, SLOT(on_buttonCurveUf07_clicked()));

}

void SceneProjectModeCurve::on_buttonCurveNa01_clicked()
{
    naEnableEdit=true;
    select_nacure_num=0;
    enginerDialog->hide();
    if(alarmmodule!=NULL)
        alarmmodule->Set_WorkState(DE_NORMAL_FLAG);
    PageShow("Setup CurveNa");
}

void SceneProjectModeCurve::on_buttonCurveNa02_clicked()
{
    enginerDialog->hide();
    if(alarmmodule!=NULL)
        alarmmodule->Set_WorkState(DE_NORMAL_FLAG);
    naEnableEdit=true;
    select_nacure_num=1;
    PageShow("Setup CurveNa");
}

void SceneProjectModeCurve::on_buttonCurveNa03_clicked()
{
    enginerDialog->hide();
    if(alarmmodule!=NULL)
        alarmmodule->Set_WorkState(DE_NORMAL_FLAG);
    naEnableEdit=true;
    select_nacure_num=2;
    PageShow("Setup CurveNa");
}

void SceneProjectModeCurve::on_buttonCurveNa04_clicked()
{
    enginerDialog->hide();
    if(alarmmodule!=NULL)
        alarmmodule->Set_WorkState(DE_NORMAL_FLAG);
    naEnableEdit=true;
    select_nacure_num=3;
    PageShow("Setup CurveNa");
}

void SceneProjectModeCurve::on_buttonCurveNa05_clicked()
{
    enginerDialog->hide();
    if(alarmmodule!=NULL)
        alarmmodule->Set_WorkState(DE_NORMAL_FLAG);
    naEnableEdit=true;
    select_nacure_num=4;
    PageShow("Setup CurveNa");
}

void SceneProjectModeCurve::on_buttonCurveNa06_clicked()
{
    enginerDialog->hide();
    if(alarmmodule!=NULL)
        alarmmodule->Set_WorkState(DE_NORMAL_FLAG);
    naEnableEdit=true;
    select_nacure_num=5;
    PageShow("Setup CurveNa");
}

void SceneProjectModeCurve::on_buttonCurveNa07_clicked()
{
    enginerDialog->hide();
    if(alarmmodule!=NULL)
        alarmmodule->Set_WorkState(DE_NORMAL_FLAG);
    naEnableEdit=true;
    select_nacure_num=6;
    PageShow("Setup CurveNa");
}

void SceneProjectModeCurve::on_buttonCurveUf01_clicked()
{
    enginerDialog->hide();
    if(alarmmodule!=NULL)
        alarmmodule->Set_WorkState(DE_NORMAL_FLAG);
    ufEnableEdit=true;
    select_ufcure_num=0;
    PageShow("Setup CurveUF");
}

void SceneProjectModeCurve::on_buttonCurveUf02_clicked()
{
    enginerDialog->hide();
    if(alarmmodule!=NULL)
        alarmmodule->Set_WorkState(DE_NORMAL_FLAG);
    ufEnableEdit=true;
    select_ufcure_num=1;
    PageShow("Setup CurveUF");
}
void SceneProjectModeCurve::on_buttonCurveUf03_clicked()
{
    enginerDialog->hide();
    if(alarmmodule!=NULL)
        alarmmodule->Set_WorkState(DE_NORMAL_FLAG);
    ufEnableEdit=true;
    select_ufcure_num=2;
    PageShow("Setup CurveUF");
}
void SceneProjectModeCurve::on_buttonCurveUf04_clicked()
{
    enginerDialog->hide();
    if(alarmmodule!=NULL)
        alarmmodule->Set_WorkState(DE_NORMAL_FLAG);
    ufEnableEdit=true;
    select_ufcure_num=3;
    PageShow("Setup CurveUF");
}
void SceneProjectModeCurve::on_buttonCurveUf05_clicked()
{
    enginerDialog->hide();
    if(alarmmodule!=NULL)
        alarmmodule->Set_WorkState(DE_NORMAL_FLAG);
    ufEnableEdit=true;
    select_ufcure_num=4;
    PageShow("Setup CurveUF");
}
void SceneProjectModeCurve::on_buttonCurveUf06_clicked()
{
    enginerDialog->hide();
    if(alarmmodule!=NULL)
        alarmmodule->Set_WorkState(DE_NORMAL_FLAG);
    ufEnableEdit=true;
    select_ufcure_num=5;
    PageShow("Setup CurveUF");
}
void SceneProjectModeCurve::on_buttonCurveUf07_clicked()
{
    enginerDialog->hide();
    if(alarmmodule!=NULL)
        alarmmodule->Set_WorkState(DE_NORMAL_FLAG);
    ufEnableEdit=true;
    select_ufcure_num=6;
    PageShow("Setup CurveUF");
}


void SceneProjectModeCurve::CustomSetText()
{

    labelCurveNaTitle.setText(DE_TR_NACUREPREDEFINEDMODIFY);
    labelCurveUFTitle.setText(DE_TR_UFCUREPREDEFINEDMODIFY);
    buttonCurveNa01.setText(DE_TR_NAPRECURE+"1");
    buttonCurveNa02.setText(DE_TR_NAPRECURE+"2");
    buttonCurveNa03.setText(DE_TR_NAPRECURE+"3");
    buttonCurveNa04.setText(DE_TR_NAPRECURE+"4");
    buttonCurveNa05.setText(DE_TR_NAPRECURE+"5");
    buttonCurveNa06.setText(DE_TR_NAPRECURE+"6");
    buttonCurveNa07.setText(DE_TR_NAPRECURE+"7");

    buttonCurveUf01.setText(DE_TR_UFPRECURE+"1");
    buttonCurveUf02.setText(DE_TR_UFPRECURE+"2");
    buttonCurveUf03.setText(DE_TR_UFPRECURE+"3");
    buttonCurveUf04.setText(DE_TR_UFPRECURE+"4");
    buttonCurveUf05.setText(DE_TR_UFPRECURE+"5");
    buttonCurveUf06.setText(DE_TR_UFPRECURE+"6");
    buttonCurveUf07.setText(DE_TR_UFPRECURE+"7");
}

void SceneProjectModeCurve::setCusVisible(bool vis, char* para)
{
    para=para;

    labelCurveNaTitle.setVisible(vis);
    labelCurveUFTitle.setVisible(vis);
    buttonCurveNa01.setVisible(vis);
    buttonCurveNa02.setVisible(vis);
    buttonCurveNa03.setVisible(vis);
    buttonCurveNa04.setVisible(vis);
    buttonCurveNa05.setVisible(vis);
    buttonCurveNa06.setVisible(vis);
    buttonCurveNa07.setVisible(vis);

    buttonCurveUf01.setVisible(vis);
    buttonCurveUf02.setVisible(vis);
    buttonCurveUf03.setVisible(vis);
    buttonCurveUf04.setVisible(vis);
    buttonCurveUf05.setVisible(vis);
    buttonCurveUf06.setVisible(vis);
    buttonCurveUf07.setVisible(vis);

}

void SceneProjectModeCurve::lineEdit_init()
{

}

void SceneProjectModeCurve::lineEdit_data_set(const char *name, const char *data)
{
    name = name;
    data = data;
}

void SceneProjectModeCurve::ActionHandle(const void *dataPtr, int name)
{
    dataPtr = dataPtr;
    name = name;
}

