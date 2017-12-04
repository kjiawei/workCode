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
    PerInfo =new QWidget();
    DialyzerP = new QWidget();
    //Curve = new QWidget();

    update_timer = new QTimer();
    update_timer->setInterval(1000*29);
    connect(update_timer, SIGNAL(timeout()),this, SLOT(updateKTV()));

    /***********************************Kt/V Page***************************************/
    labelCaliText = new QLabel(Ktv);
    labelCaliText->setGeometry(50,10,530,180);
    labelCaliText->setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                                 CFG_FONT_SIZE_TEXT, CONFIG_FONT_NORMAL));
    labelCaliText->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    labelCaliText->setWordWrap(true);
    labelCaliText->setStyleSheet(CONFIG_COLOR_LABEL);

    labelAttentionText = new QLabel(Ktv);
    labelAttentionText->setGeometry(50,440,530,150);
    labelAttentionText->setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                                      CFG_FONT_SIZE_TEXT, CONFIG_FONT_NORMAL));
    labelAttentionText->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    labelAttentionText->setWordWrap(true);
    labelAttentionText->setStyleSheet(CONFIG_COLOR_LABEL);

    labelDialyserModel = new QLabel(Ktv);
    labelDialyserModel->setGeometry(0,180,200,38);
    labelDialyserModel->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelDialyserModel->setFont(QFont(QString(CONFIG_FONT_STYLE),
                                      CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelDialyserModel->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    disDialyserModel = new QLabel(Ktv);
    SCENE_KTV2_DIS_LABLE(*disDialyserModel, 220, 180, 160);

    startCaliButton = new QPushButton(Ktv);
    startCaliButton->setGeometry(50,280,100,38);
    caliProgressBar = new QProgressBar(Ktv);//校正进度条
    caliProgressBar->setGeometry(170,280,300,38);
    caliProgressBar->setMinimum(0);
    caliProgressBar->setMaximum(100);
    caliProgressBar->setValue(80);

    labelUF = new QLabel(Ktv);
    labelUF->setGeometry(0,330,200,38);
    labelUF->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelUF->setFont(QFont(QString(CONFIG_FONT_STYLE),
                           CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelUF->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    disUF = new QLabel(Ktv);
    SCENE_KTV2_DIS_LABLE(*disUF, 60, 330, 100);
    labelUFUnit = new QLabel(Ktv);
    SCENE_KTV2_UNIT(*labelUFUnit,180,330);

    labelDialysateFlowRate = new QLabel(Ktv);
    labelDialysateFlowRate->setGeometry(280,330,200,38);
    labelDialysateFlowRate->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelDialysateFlowRate->setFont(QFont(QString(CONFIG_FONT_STYLE),
                                          CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelDialysateFlowRate->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    disDialysateFlowRate = new QLabel(Ktv);
    SCENE_KTV2_DIS_LABLE(*disDialysateFlowRate, 340, 330, 100);
    labelDialysateFlowRateUnit = new QLabel(Ktv);
    SCENE_KTV2_UNIT(*labelDialysateFlowRateUnit,440,330);

    labelBloodFlowRate = new QLabel(Ktv);
    labelBloodFlowRate->setGeometry(0,360,200,38);
    labelBloodFlowRate->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelBloodFlowRate->setFont(QFont(QString(CONFIG_FONT_STYLE),
                                      CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelBloodFlowRate->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    disBloodFlowRate = new QLabel(Ktv);
    SCENE_KTV2_DIS_LABLE(*disBloodFlowRate, 60, 360, 100);
    labelBloodFlowRateUnit = new QLabel(Ktv);
    SCENE_KTV2_UNIT(*labelBloodFlowRateUnit,180,360);

    labelTime = new QLabel(Ktv);
    labelTime->setGeometry(280,360,200,38);
    labelTime->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelTime->setFont(QFont(QString(CONFIG_FONT_STYLE),
                             CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelTime->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    disTime = new QLabel(Ktv);
    SCENE_KTV2_DIS_LABLE(*disTime, 340, 360, 100);
    labelTimeUnit = new QLabel(Ktv);
    SCENE_KTV2_UNIT(*labelTimeUnit,440,360);
    //btnPatientInfo = new QPushButton(Ktv);
    //btnPatientInfo->setGeometry(QRect(0,20+80,200,39));
    //BUTTON_SIMPLE_STYLE(*btnPatientInfo);
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

    labelKtVTarget = new QLabel(PerInfo);
    labelKtVTarget->setGeometry(100,80+280,160,38);
    labelKtVTarget->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelKtVTarget->setFont(QFont(QString(CONFIG_FONT_STYLE),
                                  CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelKtVTarget->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    disKtVTarget = new QLabel(PerInfo);
    SCENE_KTV2_DIS_LABLE(*disKtVTarget, 280, 80+280, 200);

    labelUpTo= new QLabel(PerInfo);
    labelUpTo->setGeometry(100,80+320,160,38);
    labelUpTo->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelUpTo->setFont(QFont(QString(CONFIG_FONT_STYLE),
                             CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelUpTo->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    disUpTo = new QLabel(PerInfo);
    SCENE_KTV2_DIS_LABLE(*disUpTo, 280, 80+320, 200);

    labelCurrentKtV = new QLabel(PerInfo);
    labelCurrentKtV->setGeometry(100,80+360,160,38);
    labelCurrentKtV->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelCurrentKtV->setFont(QFont(QString(CONFIG_FONT_STYLE),
                                   CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelCurrentKtV->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    disCurrentKtV = new QLabel(PerInfo);
    SCENE_KTV2_DIS_LABLE(*disCurrentKtV, 280, 80+360, 200);
    /***************************Dialyzer Parameters Page*******************************/
    ureaExcRatio = new QLabel(DialyzerP);//尿素清除量
    ureaExcRatio->setGeometry(0,120,200,38);//KTV标签左上角即为0点
    ureaExcRatio->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    ureaExcRatio->setFont(QFont(QString(CONFIG_FONT_STYLE),
                                           CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    ureaExcRatio->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    //labelUreaExcretionRatio->setGeometry(0,160,200,38);

    labelUreaExcretionRatioUnit = new QLabel(DialyzerP);//单位
    labelUreaExcretionRatioUnit->setGeometry(100,80,200,38);//KTV标签左上角即为0点
    labelUreaExcretionRatioUnit->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelUreaExcretionRatioUnit->setFont(QFont(QString(CONFIG_FONT_STYLE),
                                           CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelUreaExcretionRatioUnit->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    //SCENE_KTV_LABLE(*labelUreaExcretionRatioUnit, SCENE_PATIENT_LABEL_X - 40, 3, 200);

    lineEditUreaExcretionRatio = new QLineEdit(DialyzerP);//编辑框
    lineEditUreaExcretionRatio->setGeometry(240,80,100,38);//KTV标签左上角即为0点
    lineEditUreaExcretionRatio->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    lineEditUreaExcretionRatio->setFont(QFont(QString(CONFIG_FONT_STYLE),
                                           CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    lineEditUreaExcretionRatio->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    //SCENE_KTV_LINEEDIT(*lineEditUreaExcretionRatio, 250, 3);

    labelDefaultUreaExcretionRatio = new QLabel(DialyzerP);//默认值
    labelDefaultUreaExcretionRatio->setGeometry(360,80,200,38);//KTV标签左上角即为0点
    labelDefaultUreaExcretionRatio->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelDefaultUreaExcretionRatio->setFont(QFont(QString(CONFIG_FONT_STYLE),
                                           CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelDefaultUreaExcretionRatio->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    //SCENE_KTV_LABLE(*labelDefaultUreaExcretionRatio, 350, 3, 200);

    labelUreaExcretionRatioUnit2 = new QLabel(DialyzerP);//单位
    labelUreaExcretionRatioUnit2->setGeometry(100,200,200,38);//KTV标签左上角即为0点
    labelUreaExcretionRatioUnit2->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelUreaExcretionRatioUnit2->setFont(QFont(QString(CONFIG_FONT_STYLE),
                                           CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelUreaExcretionRatioUnit2->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    //SCENE_KTV_LABLE(*labelUreaExcretionRatioUnit2, SCENE_PATIENT_LABEL_X - 40, 4, 200);

    lineEditUreaExcretionRatio2 = new QLineEdit(DialyzerP);
    lineEditUreaExcretionRatio2->setGeometry(240,200,100,38);//KTV标签左上角即为0点
    lineEditUreaExcretionRatio2->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    lineEditUreaExcretionRatio2->setFont(QFont(QString(CONFIG_FONT_STYLE),
                                           CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    lineEditUreaExcretionRatio2->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    //SCENE_KTV_LINEEDIT(*lineEditUreaExcretionRatio2, 250, 4);//SCENE_PATIENT_LINEEDIT_X

    labelDefaultUreaExcretionRatio2 = new QLabel(DialyzerP);
    labelDefaultUreaExcretionRatio2->setGeometry(360,200,200,38);//KTV标签左上角即为0点
    labelDefaultUreaExcretionRatio2->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelDefaultUreaExcretionRatio2->setFont(QFont(QString(CONFIG_FONT_STYLE),
                                           CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelDefaultUreaExcretionRatio2->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    //SCENE_KTV_LABLE(*labelDefaultUreaExcretionRatio2, 350, 4, 200);

    labelKTV = new QLabel(DialyzerP);
    labelKTV->setGeometry(0,320,200,38);//260->325
    labelKTV->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelKTV->setFont(QFont(QString(CONFIG_FONT_STYLE),
                            CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelKTV->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelKTVValue = new QLabel(DialyzerP);
    labelKTVValue->setGeometry(120,320,200,38);
    labelKTVValue->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelKTVValue->setFont(QFont(QString(CONFIG_FONT_STYLE),
                                 CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelKTVValue->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelTreatTime = new QLabel(DialyzerP);
    labelTreatTime->setGeometry(240,320,200,38);
    labelTreatTime->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelTreatTime->setFont(QFont(QString(CONFIG_FONT_STYLE),
                                  CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelTreatTime->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    disTreatTime = new QLabel(DialyzerP);
    disTreatTime->setGeometry(400,320,200,38);
    disTreatTime->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    disTreatTime->setFont(QFont(QString(CONFIG_FONT_STYLE),
                                      CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    disTreatTime->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    //SCENE_KTV_LINEEDIT(*disTreatTime, 420, 460);

    labelTreatTimeUnit = new QLabel(DialyzerP);
    labelTreatTimeUnit->setGeometry(500,320,200,38);
    labelTreatTimeUnit->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelTreatTimeUnit->setFont(QFont(QString(CONFIG_FONT_STYLE),
                                      CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelTreatTimeUnit->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelAdaptiveProportion = new QLabel(DialyzerP);//调整比例
    labelAdaptiveProportion->setGeometry(0,440,200,38);//KTV标签左上角即为0点
    labelAdaptiveProportion->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelAdaptiveProportion->setFont(QFont(QString(CONFIG_FONT_STYLE),
                                           CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelAdaptiveProportion->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    lineEditAdaptiveProportion = new QLineEdit(DialyzerP);
    lineEditAdaptiveProportion->setGeometry(140,440,100,38);//KTV标签左上角即为0点
    //SCENE_KTV_LINEEDIT(*lineEditAdaptiveProportion, SCENE_PATIENT_LABEL_X + 120, 7);

    labelDefaultAdaptiveProportion = new QLabel(DialyzerP);
    labelDefaultAdaptiveProportion->setGeometry(320,440,200,38);//KTV标签左上角即为0点
    labelDefaultAdaptiveProportion->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelDefaultAdaptiveProportion->setFont(QFont(QString(CONFIG_FONT_STYLE),
                                                  CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelDefaultAdaptiveProportion->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelDefaultValueAdaptiveProportion = new QLabel(DialyzerP);
    labelDefaultValueAdaptiveProportion->setGeometry(500,440,200,38);//KTV标签左上角即为0点
    labelDefaultValueAdaptiveProportion->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelDefaultValueAdaptiveProportion->setFont(QFont(QString(CONFIG_FONT_STYLE),
                                                       CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelDefaultValueAdaptiveProportion->setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    /***********************************Curve Page************************************/
//    labelCurve = new QLabel(Curve);
//    labelCurve->setGeometry(0,10,530,180);
//    labelCurve->setFont(QFont(QString(CONFIG_FONT_STYLE),         \
//                              CFG_FONT_SIZE_TEXT, CONFIG_FONT_NORMAL));
//    labelCurve->setAlignment(Qt::AlignTop | Qt::AlignLeft);
//    labelCurve->setWordWrap(true);
//    labelCurve->setStyleSheet(CONFIG_COLOR_LABEL);

//    KTVCurve = new WidgetCurve(Curve);
//    KTVCurve->setMode(1);
//    KTVCurve->setGeometry(15,100,500,400);
//    KTVCurve->setValueZoom(10);
//    KTVCurve->setXOffset(50);
//    KTVCurve->setYOffset(30);
//    KTVCurve->setUnitX(Hour);
//    KTVCurve->setMaxValue(30);
//    KTVCurve->setMinValue(0);
//    KTVCurve->setTestTime(3600*6);
//    KTVCurve->setVerticalScale(6);
//    KTVCurve->setHorizontalScale(6);
    //KTVCurve->start();
    /******************** KtVTab 添加页签Setting ****************************************/
    KtVTabWidget.setGeometry(170,60,560,600);
    KtVTabWidget.setDocumentMode(true);
    KtVTabWidget.setStyleSheet("QTabBar::tab { height: 40px;width: 80px}");
    KtVTabWidget.addTab(Ktv, "Kt/V");
    KtVTabWidget.addTab(PerInfo,tr("PerInfo"));
    KtVTabWidget.addTab(DialyzerP,tr("D.P."));
    //KtVTabWidget.addTab(Curve,"Curve");

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
    ic->appendWidget(ureaExc,NumInputMethod,LineEdit);
    ic->appendWidget(ureaExc2,NumInputMethod,LineEdit);
    ic->appendWidget(adpPro,NumInputMethod,LineEdit);
    ic->appendWidget(lineEditKtvHeight,NumInputMethod,LineEdit);

    PageAppendPiece("KtV2", this);
    setMouseTracking(true);
    //connect(btnPatientInfo, SIGNAL(clicked()), this, SLOT(on_btnPatientInfo_clicked()));
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
    disTreatTime->setNum(total_secCount/60);

    PacketFluid Fluid;
    dataRead(PACKET_NAME_DIALYSATE, &Fluid);
    disDialysateFlowRate->setNum(Fluid.Wspeed);

    //disKtVTarget->setText("500");

    //this->CurrentBloodFlowRate = sdoBlood.N3_set_BPspeed;
    //currentKTV=QString::number(this->CurrentBloodFlowRate);
    char f_str[10]={0};
    sprintf(f_str,"%f",KtV2_CurrentKtv());
    disCurrentKtV->setText(f_str);//actual

    char ktv_up_to_str[10]={0};
    sprintf(ktv_up_to_str,"%f",KtV2_KtVUpTo(total_secCount/60));
    disUpTo->setText(ktv_up_to_str);
}

void SceneKTV2::lineEdit_init()
{

}


void SceneKTV2::CustomSetText()
{
    //KTV
    labelDialyserModel->setText(tr("Dialyzer Type"));
    disDialyserModel->setText("200ml/min");
    labelBloodFlowRate->setText(tr("Blood Flow"));
    labelBloodFlowRateUnit->setText(tr("[ml/min]"));
    //btnPatientInfo->setText(tr("Patient Information"));
    labelTime->setText(tr("Cure time"));
    labelTimeUnit->setText(tr("[h:min]"));
    labelUF->setText(tr("UF Goal"));
    labelUFUnit->setText(tr("[ml]"));
    labelDialysateFlowRate->setText(tr("Dialysate Flowrate"));
    labelDialysateFlowRateUnit->setText(tr("[ml/min]"));
    labelCaliText->setText(tr("      Calibration should be done when Kt/V tests inaccurately.In calibration,machine should be in prime status.Precondition on successful calibration: Pre-dialysis conductivity and post-dialysis conductivityshould be in the range of 13.0-15.0 mS/cm,the error between them should be less than1.0 mS/cm."));
    labelAttentionText->setText(tr("      Attention: It is not easy to change the flow rate of the blood pump as the reference value, and the change of the flow rate of KT/V is not a reference standard"));
    startCaliButton->setText("Start Calibration");

    //Patient Info
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
    labelKtVTarget->setText(tr("Kt/V Goal"));
    labelUpTo->setText(tr("Kt/V Upto"));
    labelCurrentKtV->setText(tr("Kt/V Actual"));

    //DialysisP
    ureaExcRatio->setText(tr("CCA"));
    ureaExcRatioUnit->setText("200ml/min");
    ureaExcRatioUnit2->setText("300ml/min");

    defaultUreaExc->setText(tr("Default:180ml/min"));
    defaultUreaExc2->setText(tr("Default:230ml/min"));
    labelKTV->setText(tr("KTV"));
    ktvValue->setText(tr("1.8"));
    treatTime->setText(tr("Cure time"));
    treatTimeUnit->setText(tr("min"));

    adpPro->setText(tr("AdpPro"));
    adpProDef->setText(tr("Default"));
    adpProDefV->setText(tr("1.0"));

    //Curve
    //labelCurve->setText(tr("  If the result after dialysis is >1.2,it measn dialysis adequacy."));

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
