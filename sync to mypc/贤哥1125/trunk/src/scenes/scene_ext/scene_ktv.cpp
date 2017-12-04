#include <QtGui>
#include "config_ui.h"
#include "scene_ktv.h"
#include "widget_common.h"
#include "scene_setup_layout.h"

void SceneKTV::setUi(void)
{
    labelTitle.setGeometry(QRect(34, 21, 100, 30));
    LABEL_TITLE_STYLE(labelTitle);

    Ktv = new QWidget();
    URR = new QWidget();
    SRI2pool = new QWidget();
    nPCR = new QWidget();
    TACurea = new QWidget();
    Curve = new QWidget();
    PerInfo =new QWidget();
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

/***********************************Kt/V Page***************************************/
    labelCaliText = new QLabel(Ktv);
    labelCaliText->setGeometry(0,10,530,150);
    labelCaliText->setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                                   CFG_FONT_SIZE_TEXT, CONFIG_FONT_NORMAL));
    labelCaliText->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    labelCaliText->setWordWrap(true);
    labelCaliText->setStyleSheet(CONFIG_COLOR_LABEL);

    labelKtVFunction = new QLabel(Ktv);
    SCENE_KTV_LABLE(*labelKtVFunction, SCENE_DIALYSIS_LABEL_X, 4, 200);

    radioButtonStartKTV = new QRadioButton(Ktv);
    radioButtonStartKTV->setGeometry(QRect(215, 190, 90, 35));
    RADIOBUTTON_STYLE(*radioButtonStartKTV);
    radioButtonStartKTV->setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

    radioButtonBAN = new QRadioButton(Ktv);
    radioButtonBAN->setGeometry(QRect(285, 190, 90, 35));
    RADIOBUTTON_STYLE(*radioButtonBAN);
    radioButtonBAN->setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

    startCaliButton = new QPushButton(Ktv);
    startCaliButton->setGeometry(50,280,100,38);

    CaliProgressBar = new QProgressBar(Ktv);
    CaliProgressBar->setGeometry(170,280,300,38);
    CaliProgressBar->setMinimum(0);
    CaliProgressBar->setMaximum(100);
    CaliProgressBar->setValue(20);

    labelPreDialysis = new QLabel(Ktv);
    lineEditPreDialysis = new QLineEdit(Ktv);
    labelPreDialysisUnit = new QLabel(Ktv);
    SCENE_KTV_LABLE(*labelPreDialysis, SCENE_DIALYSIS_LABEL_X, 8, 300);
    SCENE_KTV_LINEEDIT(*lineEditPreDialysis, SCENE_DISLYSIS_LINEEDIT_X, 8);
    lineEditPreDialysis->setReadOnly(true);
    SCENE_KTV_UNIT(*labelPreDialysisUnit,SCENE_DISLYSIS_UNIT_X, 8);

    labelPostDialysis = new QLabel(Ktv);
    lineEditPostDialysis = new QLineEdit(Ktv);
    labelPostDialysisUnit = new QLabel(Ktv);
    SCENE_KTV_LABLE(*labelPostDialysis, SCENE_DIALYSIS_LABEL_X, 9, 300);
    SCENE_KTV_LINEEDIT(*lineEditPostDialysis, SCENE_DISLYSIS_LINEEDIT_X, 9);
    lineEditPostDialysis->setReadOnly(true);
    SCENE_KTV_UNIT(*labelPostDialysisUnit,SCENE_DISLYSIS_UNIT_X, 9);
    /***********************************URR Page***************************************/
    labelURRText = new QLabel(URR);
    labelURRText->setGeometry(0,10,530,150);
    labelURRText->setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                                   CFG_FONT_SIZE_TEXT, CONFIG_FONT_NORMAL));
    labelURRText->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    labelURRText->setWordWrap(true);
    labelURRText->setStyleSheet(CONFIG_COLOR_LABEL);

    labelFirstPreDia = new QLabel(URR);
    lineEditFirstPreDia = new QLineEdit(URR);
    labelFirstPreDiaUnit = new QLabel(URR);
    SCENE_KTV_LABLE(*labelFirstPreDia, SCENE_DIALYSIS_LABEL_X, 4, 300);
    SCENE_KTV_LINEEDIT(*lineEditFirstPreDia, SCENE_DISLYSIS_LINEEDIT_X, 4);
    lineEditFirstPreDia->setReadOnly(true);
    SCENE_KTV_UNIT(*labelFirstPreDiaUnit,SCENE_DISLYSIS_UNIT_X, 4);

    labelFirstPostDia = new QLabel(URR);
    lineEditFirstPostDia = new QLineEdit(URR);
    labelFirstPostDiaUnit = new QLabel(URR);
    SCENE_KTV_LABLE(*labelFirstPostDia, SCENE_DIALYSIS_LABEL_X, 5, 300);
    SCENE_KTV_LINEEDIT(*lineEditFirstPostDia, SCENE_DISLYSIS_LINEEDIT_X, 5);
    lineEditFirstPostDia->setReadOnly(true);
    SCENE_KTV_UNIT(*labelFirstPostDiaUnit,SCENE_DISLYSIS_UNIT_X, 5);

    labelReduceRate = new QLabel(URR);
    SCENE_KTV_LABLE(*labelReduceRate, SCENE_DIALYSIS_LABEL_X, 6, 300);

    labelReduceRateValue = new QLabel(URR);
    SCENE_KTV_UNIT(*labelReduceRateValue, SCENE_DIALYSIS_LABEL_X + 310, 6);
    labelReduceRateValue->setFont(QFont(QString(CONFIG_FONT_STYLE),  \
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_WEIGHT));
    /***********************************SRI2pool Page***************************************/
    labelSRIText = new QLabel(SRI2pool);
    labelSRIText->setGeometry(0,10,530,150);
    labelSRIText->setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                                   CFG_FONT_SIZE_TEXT, CONFIG_FONT_NORMAL));
    labelSRIText->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    labelSRIText->setWordWrap(true);
    labelSRIText->setStyleSheet(CONFIG_COLOR_LABEL);

    labelFirstPreDiaBUN = new QLabel(SRI2pool);
    EditFirstPreDiaBUN = new QLineEdit(SRI2pool);
    labelFirstPreDiaBUNUnit = new QLabel(SRI2pool);
    SCENE_KTV_LABLE(*labelFirstPreDiaBUN, SCENE_DIALYSIS_LABEL_X, 4, 300);
    SCENE_KTV_LINEEDIT(*EditFirstPreDiaBUN, SCENE_DISLYSIS_LINEEDIT_X, 4);
    EditFirstPreDiaBUN->setReadOnly(true);
    SCENE_KTV_UNIT(*labelFirstPreDiaBUNUnit,SCENE_DISLYSIS_UNIT_X, 4);

    labelFirstPostDiaBUN = new QLabel(SRI2pool);
    EditFirstPostDiaBUN = new QLineEdit(SRI2pool);
    labelFirstPostDiaBUNUnit = new QLabel(SRI2pool);
    SCENE_KTV_LABLE(*labelFirstPostDiaBUN, SCENE_DIALYSIS_LABEL_X, 5, 300);
    SCENE_KTV_LINEEDIT(*EditFirstPostDiaBUN, SCENE_DISLYSIS_LINEEDIT_X, 5);
    EditFirstPostDiaBUN->setReadOnly(true);
    SCENE_KTV_UNIT(*labelFirstPostDiaBUNUnit,SCENE_DISLYSIS_UNIT_X, 5);

    labelUFVolume = new QLabel(SRI2pool);
    EditUFVolume = new QLineEdit(SRI2pool);
    labelUFVolumeUnit = new QLabel(SRI2pool);
    SCENE_KTV_LABLE(*labelUFVolume, SCENE_DIALYSIS_LABEL_X, 6, 300);
    SCENE_KTV_LINEEDIT(*EditUFVolume, SCENE_DISLYSIS_LINEEDIT_X, 6);
    EditUFVolume->setReadOnly(true);
    SCENE_KTV_UNIT(*labelUFVolumeUnit,SCENE_DISLYSIS_UNIT_X, 6);

    labelSRI2pool = new QLabel(SRI2pool);
    SCENE_KTV_LABLE(*labelSRI2pool, SCENE_DIALYSIS_LABEL_X, 7, 450);

    labelSRI2poolValue = new QLabel(SRI2pool);
    SCENE_KTV_UNIT(*labelSRI2poolValue, SCENE_DIALYSIS_LABEL_X + 460, 7);
    labelSRI2poolValue->setFont(QFont(QString(CONFIG_FONT_STYLE),  \
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_WEIGHT));
    /***********************************nPCR Page***************************************/
    labelnPCRText = new QLabel(nPCR);
    labelnPCRText->setGeometry(0,10,530,180);PageAppendPiece("Kt/V", this);
    labelnPCRText->setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                                   CFG_FONT_SIZE_TEXT, CONFIG_FONT_NORMAL));
    labelnPCRText->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    labelnPCRText->setWordWrap(true);
    labelnPCRText->setStyleSheet(CONFIG_COLOR_LABEL);

    labelUrineVolume = new QLabel(nPCR);
    EditUrineVolume = new QLineEdit(nPCR);
    labelUrineVolumeUnit = new QLabel(nPCR);
    SCENE_KTV_LABLE(*labelUrineVolume, SCENE_DIALYSIS_LABEL_X, 4, 300);
    SCENE_KTV_LINEEDIT(*EditUrineVolume, SCENE_DISLYSIS_LINEEDIT_X, 4);
    EditUrineVolume->setReadOnly(true);
    SCENE_KTV_UNIT(*labelUrineVolumeUnit,SCENE_DISLYSIS_UNIT_X, 4);

    labelIntervalTime = new QLabel(nPCR);
    EditIntervalTime = new QLineEdit(nPCR);
    labelIntervalTimeUnit = new QLabel(nPCR);
    SCENE_KTV_LABLE(*labelIntervalTime, SCENE_DIALYSIS_LABEL_X, 5, 300);
    SCENE_KTV_LINEEDIT(*EditIntervalTime, SCENE_DISLYSIS_LINEEDIT_X, 5);
    EditIntervalTime->setReadOnly(true);
    SCENE_KTV_UNIT(*labelIntervalTimeUnit,SCENE_DISLYSIS_UNIT_X, 5);

    labelUrineBUN = new QLabel(nPCR);
    EditUrineBUN = new QLineEdit(nPCR);
    labelUrineBUNUnit = new QLabel(nPCR);
    SCENE_KTV_LABLE(*labelUrineBUN, SCENE_DIALYSIS_LABEL_X, 6, 300);
    SCENE_KTV_LINEEDIT(*EditUrineBUN, SCENE_DISLYSIS_LINEEDIT_X, 6);
    EditUrineBUN->setReadOnly(true);
    SCENE_KTV_UNIT(*labelUrineBUNUnit,SCENE_DISLYSIS_UNIT_X, 6);

    labelFirstPostDiaBUNII = new QLabel(nPCR);
    EditFirstPostDiaBUNII = new QLineEdit(nPCR);
    labelFirstPostDiaBUNIIUnit = new QLabel(nPCR);
    SCENE_KTV_LABLE(*labelFirstPostDiaBUNII, SCENE_DIALYSIS_LABEL_X, 7, 300);
    SCENE_KTV_LINEEDIT(*EditFirstPostDiaBUNII, SCENE_DISLYSIS_LINEEDIT_X, 7);
    EditFirstPostDiaBUNII->setReadOnly(true);
    SCENE_KTV_UNIT(*labelFirstPostDiaBUNIIUnit,SCENE_DISLYSIS_UNIT_X, 7);

    labelSecPostDiaBUN = new QLabel(nPCR);
    EditSecPostDiaBUN = new QLineEdit(nPCR);
    labelSecPostDiaBUNUnit = new QLabel(nPCR);
    SCENE_KTV_LABLE(*labelSecPostDiaBUN, SCENE_DIALYSIS_LABEL_X, 8, 300);
    SCENE_KTV_LINEEDIT(*EditSecPostDiaBUN, SCENE_DISLYSIS_LINEEDIT_X, 8);
    EditSecPostDiaBUN->setReadOnly(true);
    SCENE_KTV_UNIT(*labelSecPostDiaBUNUnit,SCENE_DISLYSIS_UNIT_X, 8);

    labelnPCR = new QLabel(nPCR);
    SCENE_KTV_LABLE(*labelnPCR, SCENE_DIALYSIS_LABEL_X, 9, 400);

    labelnPCRValue = new QLabel(nPCR);
    labelnPCRValue->setGeometry(QRect(SCENE_DIALYSIS_LABEL_X + 405,       \
                                      SCENE_KTV_LABEL_Y_9,                \
                                      50,         \
                                      SCENE_SETUP_WIDGET_HIGH));
    labelnPCRValue->setAlignment(Qt::AlignCenter);
    labelnPCRValue->setFont(QFont(QString(CONFIG_FONT_STYLE),     \
                                  CFG_FONT_SIZE_THIRD, CONFIG_FONT_WEIGHT));
    labelnPCRValue->setStyleSheet(CONFIG_COLOR_LABEL);

    labelnPCRUnit = new QLabel(nPCR);
    SCENE_KTV_UNIT(*labelnPCRUnit, SCENE_DIALYSIS_LABEL_X+460, 9);
    /***********************************TACurea Page***************************************/
    labelnTACureaText = new QLabel(TACurea);
    labelnTACureaText->setGeometry(0,10,530,180);
    labelnTACureaText->setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                                   CFG_FONT_SIZE_TEXT, CONFIG_FONT_NORMAL));
    labelnTACureaText->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    labelnTACureaText->setWordWrap(true);
    labelnTACureaText->setStyleSheet(CONFIG_COLOR_LABEL);

    labelTACFirstPreDiaBUN = new QLabel(TACurea);
    EditTACFirstPreDiaBUN = new QLineEdit(TACurea);
    labelTACFirstPreDiaBUNUnit = new QLabel(TACurea);
    SCENE_KTV_LABLE(*labelTACFirstPreDiaBUN, SCENE_DIALYSIS_LABEL_X, 4, 300);
    SCENE_KTV_LINEEDIT(*EditTACFirstPreDiaBUN, SCENE_DISLYSIS_LINEEDIT_X, 4);
    EditTACFirstPreDiaBUN->setReadOnly(true);
    SCENE_KTV_UNIT(*labelTACFirstPreDiaBUNUnit,SCENE_DISLYSIS_UNIT_X, 4);

    labelTACFirstPostDiaBUN = new QLabel(TACurea);
    EditTACFirstPostDiaBUN = new QLineEdit(TACurea);
    labelTACFirstPostDiaBUNUnit = new QLabel(TACurea);
    SCENE_KTV_LABLE(*labelTACFirstPostDiaBUN, SCENE_DIALYSIS_LABEL_X, 5, 300);
    SCENE_KTV_LINEEDIT(*EditTACFirstPostDiaBUN, SCENE_DISLYSIS_LINEEDIT_X, 5);
    EditTACFirstPostDiaBUN->setReadOnly(true);
    SCENE_KTV_UNIT(*labelTACFirstPostDiaBUNUnit,SCENE_DISLYSIS_UNIT_X, 5);

    labelTACSecPreDiaBUN = new QLabel(TACurea);
    EditTACSecPreDiaBUN = new QLineEdit(TACurea);
    labelTACSecPreDiaBUNUnit = new QLabel(TACurea);
    SCENE_KTV_LABLE(*labelTACSecPreDiaBUN, SCENE_DIALYSIS_LABEL_X, 6, 300);
    SCENE_KTV_LINEEDIT(*EditTACSecPreDiaBUN, SCENE_DISLYSIS_LINEEDIT_X, 6);
    EditTACSecPreDiaBUN->setReadOnly(true);
    SCENE_KTV_UNIT(*labelTACSecPreDiaBUNUnit,SCENE_DISLYSIS_UNIT_X, 6);

    labelTACDiaTime = new QLabel(TACurea);
    EditTACDiaTime = new QLineEdit(TACurea);
    labelTACDiaTimeUnit = new QLabel(TACurea);
    SCENE_KTV_LABLE(*labelTACDiaTime, SCENE_DIALYSIS_LABEL_X, 7, 300);
    SCENE_KTV_LINEEDIT(*EditTACDiaTime, SCENE_DISLYSIS_LINEEDIT_X, 7);
    EditTACDiaTime->setReadOnly(true);
    SCENE_KTV_UNIT(*labelTACDiaTimeUnit,SCENE_DISLYSIS_UNIT_X, 7);

    labelTACDiaInterTime = new QLabel(TACurea);
    EditTACDiaInterTime = new QLineEdit(TACurea);
    labelTACDiaInterTimeUnit = new QLabel(TACurea);
    SCENE_KTV_LABLE(*labelTACDiaInterTime, SCENE_DIALYSIS_LABEL_X, 8, 300);
    SCENE_KTV_LINEEDIT(*EditTACDiaInterTime, SCENE_DISLYSIS_LINEEDIT_X, 8);
    EditTACDiaInterTime->setReadOnly(true);
    SCENE_KTV_UNIT(*labelTACDiaInterTimeUnit,SCENE_DISLYSIS_UNIT_X, 8);

    labelnTAC = new QLabel(TACurea);
    SCENE_KTV_LABLE(*labelnTAC, SCENE_DIALYSIS_LABEL_X, 9, 400);

    labelnTACValue = new QLabel(TACurea);
    labelnTACValue->setGeometry(QRect(SCENE_DIALYSIS_LABEL_X + 405,       \
                                      SCENE_KTV_LABEL_Y_9,                \
                                      50,         \
                                      SCENE_SETUP_WIDGET_HIGH));
    labelnTACValue->setAlignment(Qt::AlignCenter);
    labelnTACValue->setFont(QFont(QString(CONFIG_FONT_STYLE),     \
                                  CFG_FONT_SIZE_THIRD, CONFIG_FONT_WEIGHT));
    labelnTACValue->setStyleSheet(CONFIG_COLOR_LABEL);

    labelnTACUnit = new QLabel(TACurea);
    SCENE_KTV_UNIT(*labelnTACUnit, SCENE_DIALYSIS_LABEL_X+460, 9);
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
    KtVTabWidget.addTab(URR, "URR");
    KtVTabWidget.addTab(SRI2pool, "SRI2pool");
    KtVTabWidget.addTab(nPCR, "nPCR");
    KtVTabWidget.addTab(TACurea, "TACurea");
    KtVTabWidget.addTab(Curve,"Profile");
    KtVTabWidget.addTab(PerInfo,"P.t Info");

    KtVTabWidget.setGeometry(170,60,560,600);
    KtVTabWidget.setDocumentMode(true);
    KtVTabWidget.setStyleSheet("QTabBar::tab { height: 40px;width: 80px}");
}

SceneKTV::SceneKTV(QWidget *parent) :
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

    PageAppendPiece("KtV", this);
    setMouseTracking(true);
}

void SceneKTV::lineEdit_init()
{

}


void SceneKTV::CustomSetText()
{

    labelHeightUnit->setText("cm");
    labelWeightUnit->setText("kg");
    labelPreDialysisUnit->setText("mS/cm");
    labelPostDialysisUnit->setText("mS/cm");
    labelPreDialysis->setText(tr("Pre-dislysis:"));
    labelPostDialysis->setText(tr("Post-dislysis:"));
    labelKtvHeight->setText(tr("Height:"));
    labelKtvWeight->setText(tr("Dry weight:"));
    labelKtvAge->setText(tr("Age:"));
    labelKtvGender->setText(tr("Gender:"));
    labelCaliText->setText(tr("  Calibration should be done when Kt/V tests inaccurately.\
In calibration,machine should be in prime status.\
Precondition on successful calibration: Pre-dialysis \
conductivity and post-dialysis conductivity should be in the range of \
13.0-15.0 mS/cm,the error between them should be less than 1.0 mS/cm."));

    radioButtonSelectMale->setText(tr("Male"));
    radioButtonSelectFemale->setText(tr("Female"));
    radioButtonStartKTV->setText(tr("OK"));
    radioButtonBAN->setText(tr("Ban"));
    startCaliButton->setText("Start Calibration");
    labelKtVFunction->setText(tr("Kt/V function"));
    labelURRText->setText(tr("  Dialysis urea reduction rate just reflects reduction percentage of solute\
instead of actual solute-removing index. >70% indicates dialysis adequacy.\
Only for reference! Please refer to actual clinical for analysis."));
    labelFirstPreDia->setText(tr("First pre-dialysis BUN:"));
    labelFirstPostDia->setText(tr("First post-dialysis BUN:"));
    labelFirstPreDiaUnit->setText(tr("mmol/l"));
    labelFirstPostDiaUnit->setText(tr("mmol/l"));
    labelReduceRate->setText(tr("Dialysis urea reduction rate(URR):"));
    labelReduceRateValue->setText(tr("0 %"));
    labelSRIText->setText(tr("  Pool II solute clearance index, a parameter calculated by urea kinetic model, \
indicates solute removing index in dialysis. SRI>70% indicates dialysis adequacy. \
Only for reference! Please refer to actual clinical for analysis"));
    labelFirstPreDiaBUN->setText(tr("First pre-dialysis BUN:"));
    labelFirstPostDiaBUN->setText(tr("First post-dialysis BUN:"));
    labelUFVolume->setText(tr("Acc UF Volume:"));
    labelSRI2pool->setText(tr("Pool II solute clearance index(SRI2pool):"));
    labelFirstPreDiaBUNUnit->setText(tr("mmol/l"));
    labelFirstPostDiaBUNUnit->setText(tr("mmol/l"));
    labelUFVolumeUnit->setText(tr("ml"));
    labelSRI2poolValue->setText(tr("0 %"));
    labelnPCRText->setText(tr("  Normal protein catabolism rate.The protein intakes for dialysis patients \
should be more than 1.1(g/kg.d),then nPCR will be more than 1.1(g/kg.d) too.\
When nPCR is less than 0.8(g/kg.d),it means malnutrition and inadequate dialysis\
rate will be high. Only for reference! Please refer to actual clinical for analysis"));

    labelUrineVolume->setText(tr("Urine volume in dialysis interphase:"));
    labelIntervalTime->setText(tr("Interval time of dialysis:"));
    labelUrineBUN->setText(tr("Urine BUN in dialysis interphase:"));
    labelFirstPostDiaBUNII->setText(tr("First post-dialysis BUN:"));
    labelSecPostDiaBUN->setText(tr("Second pre-dialysis BUN:"));
    labelnPCR->setText(tr("Normal protein catabolism rate(nPCR):"));
    labelUrineVolumeUnit->setText(tr("ml"));
    labelIntervalTimeUnit->setText(tr("min"));
    labelUrineBUNUnit->setText(tr("mmol/l"));
    labelFirstPostDiaBUNIIUnit->setText(tr("mmol/l"));
    labelSecPostDiaBUNUnit->setText(tr("mmol/l"));
    labelnPCRValue->setText(tr("1000"));
    labelnPCRUnit->setText(tr("g/kg.d"));
    labelnTACureaText->setText(tr("  Time Average Concentrations of Urea reflects synthesis situation of \
      dialysis urea remov index and patient's protein metabolism.TACurea>50(mg/dl) \
      means reference index for dialysis adequacy.It indicates cardiovascular,\
      nervous system of alimentary canal and other long-term complications increase.\
      Only for reference! Please refer to actual clinical for analysis."));
    labelTACFirstPreDiaBUN->setText(tr("First pre-dialysis BUN:"));
    labelTACFirstPostDiaBUN->setText(tr("First post-dialysis BUN:"));
    labelTACSecPreDiaBUN->setText(tr("Second pre-dialysis BUN:"));
    labelTACDiaTime->setText(tr("Dialysis time:"));
    labelTACDiaInterTime->setText(tr("Dialysis interval time:"));
    labelnTAC->setText(tr("Time Average Concentrations of Urea(TACurea):"));
    labelTACFirstPreDiaBUNUnit->setText(tr("mmol/l"));
    labelTACFirstPostDiaBUNUnit->setText(tr("mmol/l"));
    labelTACSecPreDiaBUNUnit->setText(tr("mmol/l"));
    labelTACDiaTimeUnit->setText(tr("min"));
    labelTACDiaInterTimeUnit->setText(tr("min"));
    labelnTACUnit->setText(tr("mg/dl"));
    labelnTACValue->setText(tr("0"));
    labelCurve->setText(tr("  If the result after dialysis is >1.2,it measn dialysis adequacy."));
}

void SceneKTV::setCusVisible(bool vis, char* para)
{
    para = para;
    labelTitle.setVisible(vis);

    KtVTabWidget.setVisible(vis);

}

void SceneKTV::mouseMoveEvent (QMouseEvent * event)
{
    qDebug()<<"current point is "<<event->pos().x()<<","<<event->pos().y();
}
