#include <QtGui>
#include "config_ui.h"
#include "scene_ktv2.h"
#include "ktv2.h"
#include "widget_common.h"
#include "scene_setup_layout.h"

void SceneKTV2::setUi(void)
{
    labelTitle.setGeometry(QRect(34, 21, 100, 30));
    LABEL_TITLE_STYLE(labelTitle);

    Ktv = new QWidget();
    Curve = new QWidget();
    PerInfo =new QWidget();

    update_timer = new QTimer();
    update_timer->setInterval(1000*29);
    connect(update_timer, SIGNAL(timeout()),this, SLOT(updateKTV()));

/***********************************PerInfo Page***************************************/
    labelKtvHeight = new QLabel(PerInfo);
    lineEditKtvHeight = new QLineEdit(PerInfo);
    labelHeightUnit = new QLabel(PerInfo);
    SCENE_KTV_LABLE(*labelKtvHeight, SCENE_PATIENT_LABEL_X, 2, 200);
    SCENE_KTV_LINEEDIT(*lineEditKtvHeight, SCENE_PATIENT_LINEEDIT_X, 2);
    SCENE_KTV_UNIT(*labelHeightUnit,SCENE_PATIENT_UNIT_X, 2);

    labelKtvWeight = new QLabel(PerInfo);
    lineEditKtvWeight = new QLineEdit(PerInfo);
    labelWeightUnit = new QLabel(PerInfo);
    SCENE_KTV_LABLE(*labelKtvWeight, SCENE_PATIENT_LABEL_X, 3, 200);
    SCENE_KTV_LINEEDIT(*lineEditKtvWeight, SCENE_PATIENT_LINEEDIT_X, 3);
    SCENE_KTV_UNIT(*labelWeightUnit, SCENE_PATIENT_UNIT_X, 3);

    labelKtvAge = new QLabel(PerInfo);
    lineEditKtvAge = new QLineEdit(PerInfo);
    SCENE_KTV_LABLE(*labelKtvAge, SCENE_PATIENT_LABEL_X,4, 200);
    SCENE_KTV_LINEEDIT(*lineEditKtvAge, SCENE_PATIENT_LINEEDIT_X, 4);

    labelKtvGender = new QLabel(PerInfo);
    SCENE_KTV_LABLE(*labelKtvGender, SCENE_PATIENT_LABEL_X, 5, 200);

    radioButtonSelectMale = new QRadioButton(PerInfo);
    radioButtonSelectMale->setGeometry(QRect(225, 235, 90, 35));
    RADIOBUTTON_STYLE(*radioButtonSelectMale);
    radioButtonSelectMale->setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

    radioButtonSelectFemale = new QRadioButton(PerInfo);
    radioButtonSelectFemale->setGeometry(QRect(325, 235, 120, 35));
    RADIOBUTTON_STYLE(*radioButtonSelectFemale);
    radioButtonSelectFemale->setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));  

    radioButtonSex = new QButtonGroup(PerInfo);
    radioButtonSex->addButton(radioButtonSelectMale);
    radioButtonSex->addButton(radioButtonSelectFemale);

    radioButtonSelectMale->setChecked(true);

    btnPatientApply = new QPushButton(PerInfo);
    buttonCancel = new QPushButton(PerInfo);

    btnPatientApply->setGeometry(QRect(220, 300, 64, 39));
    buttonCancel->setGeometry(QRect(294, 300, 64, 39));

    BUTTON_SIMPLE_STYLE(*btnPatientApply);
    BUTTON_SIMPLE_STYLE(*buttonCancel);

/***********************************Kt/V Page***************************************/
    labelDialyserModel = new QLabel(Ktv);

    labelDialyserModel->setGeometry(0,20,200,38);
    labelDialyserModel->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelDialyserModel->setFont(QFont(QString(CONFIG_FONT_STYLE),
                        CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelDialyserModel->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    disDialyserModel = new QLabel(Ktv);
    SCENE_KTV2_DIS_LABLE(*disDialyserModel, 220, 20, 160);

    labelBloodFlowRate = new QLabel(Ktv);
    labelBloodFlowRate->setGeometry(0,20+40,200,38);
    labelBloodFlowRate->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelBloodFlowRate->setFont(QFont(QString(CONFIG_FONT_STYLE),
                        CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelBloodFlowRate->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    disBloodFlowRate = new QLabel(Ktv);
    SCENE_KTV2_DIS_LABLE(*disBloodFlowRate, 220, 20+40, 100);

    labelBloodFlowRateUnit = new QLabel(Ktv);
    SCENE_KTV2_UNIT(*labelBloodFlowRateUnit,360,20+40);

    labelTime = new QLabel(Ktv);
    labelTime->setGeometry(0,20+120,200,38);
    labelTime->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelTime->setFont(QFont(QString(CONFIG_FONT_STYLE),
                        CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelTime->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    disTime = new QLabel(Ktv);
    SCENE_KTV2_DIS_LABLE(*disTime, 220, 20+120, 100);

    labelTimeUnit = new QLabel(Ktv);
    SCENE_KTV2_UNIT(*labelTimeUnit,360,20+120);

    labelUF = new QLabel(Ktv);
    labelUF->setGeometry(0,20+160,200,38);
    labelUF->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelUF->setFont(QFont(QString(CONFIG_FONT_STYLE),
                        CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelUF->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    disUF = new QLabel(Ktv);
    SCENE_KTV2_DIS_LABLE(*disUF, 220, 20+160, 100);

    labelUFUnit = new QLabel(Ktv);
    SCENE_KTV2_UNIT(*labelUFUnit,360,20+160);

    labelDialysateFlowRate = new QLabel(Ktv);
    labelDialysateFlowRate->setGeometry(0,20+200,200,38);
    labelDialysateFlowRate->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelDialysateFlowRate->setFont(QFont(QString(CONFIG_FONT_STYLE),
                        CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelDialysateFlowRate->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    disDialysateFlowRate = new QLabel(Ktv);
    SCENE_KTV2_DIS_LABLE(*disDialysateFlowRate, 220, 20+200, 100);

    labelDialysateFlowRateUnit = new QLabel(Ktv);
    SCENE_KTV2_UNIT(*labelDialysateFlowRateUnit,360,20+200);

    labelKtVTarget = new QLabel(Ktv);
    labelKtVTarget->setGeometry(0,20+280,200,38);
    labelKtVTarget->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelKtVTarget->setFont(QFont(QString(CONFIG_FONT_STYLE),
                        CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelKtVTarget->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    disKtVTarget = new QLabel(Ktv);
    SCENE_KTV2_DIS_LABLE(*disKtVTarget, 220, 20+280, 100);

    labelUpTo= new QLabel(Ktv);
    labelUpTo->setGeometry(0,20+320,200,38);
    labelUpTo->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelUpTo->setFont(QFont(QString(CONFIG_FONT_STYLE),
                        CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelUpTo->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    disUpTo = new QLabel(Ktv);
    SCENE_KTV2_DIS_LABLE(*disUpTo, 220, 20+320, 100);

    labelCurrentKtV = new QLabel(Ktv);
    labelCurrentKtV->setGeometry(0,20+360,200,38);
    labelCurrentKtV->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelCurrentKtV->setFont(QFont(QString(CONFIG_FONT_STYLE),
                        CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelCurrentKtV->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    disCurrentKtV = new QLabel(Ktv);
    SCENE_KTV2_DIS_LABLE(*disCurrentKtV, 220, 20+360, 100);

    btnPatientInfo = new QPushButton(Ktv);
    btnPatientInfo->setGeometry(QRect(0,20+80,200,39));
    BUTTON_SIMPLE_STYLE(*btnPatientInfo);

    /***********************************Curve Page************************************/
    labelCurve = new QLabel(Curve);
    labelCurve->setGeometry(0,10,530,180);
    labelCurve->setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                                   CFG_FONT_SIZE_TEXT, CONFIG_FONT_NORMAL));
    labelCurve->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    labelCurve->setWordWrap(true);
    labelCurve->setStyleSheet(CONFIG_COLOR_LABEL);

    KTVCurve = new WidgetCurve(Curve);
    KTVCurve->setMode(1);
    KTVCurve->setGeometry(15,100,500,400);
    KTVCurve->setValueZoom(10);
    KTVCurve->setXOffset(50);
    KTVCurve->setYOffset(30);
    KTVCurve->setUnitX(Hour);
    //KTVCurve->setUnit("ml");
    KTVCurve->setMaxValue(30);
    KTVCurve->setMinValue(0);
    KTVCurve->setTestTime(3600*6);
    KTVCurve->setVerticalScale(6);
    KTVCurve->setHorizontalScale(6);

   // KTVCurve->start();

    /**********************************************************************************/
    KtVTabWidget.addTab(Ktv, "Kt/V");
    //KtVTabWidget.addTab(Curve,"Curve");
    KtVTabWidget.addTab(PerInfo,tr("PerInfo"));
    KtVTabWidget.setGeometry(170,60,560,600);
    KtVTabWidget.setDocumentMode(true);
    KtVTabWidget.setStyleSheet("QTabBar::tab { height: 40px;width: 80px}");

    update_timer->start();
}

SceneKTV2::SceneKTV2(QWidget *parent) :
    QWidget(parent),
    labelTitle(parent),
    KtVTabWidget(parent)
{   
    setUi();

    CustomSetText();

    ic->appendWidget(lineEditKtvHeight,NumInputMethod,LineEdit);
    ic->appendWidget(lineEditKtvWeight,NumInputMethod,LineEdit);
    ic->appendWidget(lineEditKtvAge,NumInputMethod,LineEdit, this, 0, 150, 1,
                     labelKtvAge->text());

    PageAppendPiece("KtV2", this);
    setMouseTracking(true);

    connect(btnPatientInfo, SIGNAL(clicked()), this, SLOT(on_btnPatientInfo_clicked()));
    connect(btnPatientApply, SIGNAL(clicked()), this, SLOT(on_btnPatientApply_clicked()));

#if 0 //Testing KTV2 Thread
    KtV2_Start();
#endif
}

void SceneKTV2::on_btnPatientInfo_clicked()
{
    KtVTabWidget.setCurrentIndex(1);
}

void SceneKTV2::on_btnPatientApply_clicked()
{
    int years = lineEditKtvAge->text().toInt();
    int height = lineEditKtvHeight->text().toInt();
    int weight = lineEditKtvWeight->text().toInt();

    int sex;

    if (radioButtonSelectMale->isChecked())
        sex = MALE;
    else if (radioButtonSelectFemale->isChecked())
        sex = FEMALE;
    else
        sex = SEX_UNKNOWN;

    KtV2_Patient_Info_Save ( sex, years, height, weight);
    updateKTV();
    KtVTabWidget.setCurrentIndex(0);
}

void SceneKTV2::updateKTV()
{
    PacketSdoBlood sdoBlood;
    dataRead(PACKET_SDO_BLOOD,&sdoBlood);
    disBloodFlowRate->setNum( sdoBlood.N3_set_BPspeed );

    PacketUF UF;
    dataRead(PACKET_NAME_UF, &UF);
    disUF->setNum(UF.ufVolAim);

    disTime->setNum(total_secCount/60);

    PacketFluid Fluid;
    dataRead(PACKET_NAME_DIALYSATE, &Fluid);
    disDialysateFlowRate->setNum(Fluid.Wspeed);

    char f_str[10]={0};
    sprintf(f_str,"%f",KtV2_CurrentKtv());
    disCurrentKtV->setText(f_str);

    char ktv_up_to_str[10]={0};
    sprintf(ktv_up_to_str,"%f",KtV2_KtVUpTo(total_secCount/60));
    //printf("Kt/V Up to: %f\n", KtV2_KtVUpTo(total_secCount/60));
    disUpTo->setText(ktv_up_to_str);
}

void SceneKTV2::lineEdit_init()
{

}


void SceneKTV2::CustomSetText()
{
    labelHeightUnit->setText("cm");
    labelWeightUnit->setText("kg");
    labelKtvHeight->setText(tr("Height:"));
    labelKtvWeight->setText(tr("Dry Weight:"));
    labelKtvAge->setText(tr("Age:"));
    labelKtvGender->setText(tr("Gender:"));

    radioButtonSelectMale->setText(tr("Male"));
    radioButtonSelectFemale->setText(tr("Female"));
    btnPatientApply->setText(tr("Confirm"));
    buttonCancel->setText(tr("Cancel"));

    labelDialyserModel->setText(tr("Dialyzer Type"));
    disDialyserModel->setText("200ml/min");
    labelBloodFlowRate->setText(tr("Blood Flow"));
    labelBloodFlowRateUnit->setText(tr("[ml/min]"));
    btnPatientInfo->setText(tr("Patient Information"));
    labelTime->setText(tr("Treatment Time"));
    labelTimeUnit->setText(tr("[h:min]"));
    labelUF->setText(tr("UF Goal"));
    labelUFUnit->setText(tr("[ml]"));
    labelDialysateFlowRate->setText(tr("Dialysate Flowrate"));
    labelDialysateFlowRateUnit->setText(tr("[ml/min]"));
    labelKtVTarget->setText(tr("Kt/V Goal"));
    labelUpTo->setText(tr("Kt/V Upto"));
    labelCurrentKtV->setText(tr("Kt/V Actual"));

    labelCurve->setText(tr("  If the result after dialysis is >1.2,it measn dialysis adequacy."));
    KtVTabWidget.setTabText(KtVTabWidget.indexOf(PerInfo),tr("P.t Info"));
}

void SceneKTV2::setCusVisible(bool vis, char* para)
{
    para = para;
    labelTitle.setVisible(vis);
    KtVTabWidget.setVisible(vis);

}

void SceneKTV2::mouseMoveEvent (QMouseEvent * event)
{
    qDebug()<<"current point is "<<event->pos().x()<<","<<event->pos().y();
}
