#include "config_ui.h"
#include "scene_setup_layout.h"
#include "scene_setup_curve_test.h"

struct StandCurve standCurve;

inline void SceneSetupCurveTest::setUi()
{
    SCENE_SETUP_CREATE_TABLE(labelCurveSel, 1);
    SCENE_SETUP_CREATE_COMBOBOX(comboBoxCurveSel, 1);

    comboBoxCurveSel.insertItem(0, trUtf8("线性曲线"));
    comboBoxCurveSel.insertItem(1, trUtf8("抛物线曲线"));
    comboBoxCurveSel.insertItem(2, trUtf8("三次曲线"));
    comboBoxCurveSel.insertItem(3, trUtf8("指数曲线"));
    comboBoxCurveSel.insertItem(4, trUtf8("阶梯曲线"));
    comboBoxCurveSel.insertItem(5, trUtf8("方波曲线"));
    comboBoxCurveSel.insertItem(6, trUtf8("反方波曲线"));

    comboBoxCurveSel.setCurrentIndex(2);

    //三次曲线#/
   // widget_ThreeCurve.setGeometry(QRect(80, 83, 500, 400));
    SCENE_SETUP_CREATE_TABLE(three_curve_LabelTreatTime, 2);
    SCENE_SETUP_CREATE_LINEEDIT(three_curve_EditTreatTime, 2);
    SCENE_SETUP_CREATE_UNIT(three_curve_LabelTreatTimeUnit, 2);
    three_curve_LabelTreatTimeUnit.setText("h|min");

    SCENE_SETUP_CREATE_TABLE(three_curve_LabelWater, 3);
    SCENE_SETUP_CREATE_LINEEDIT(three_curve_EditWater, 3);
    SCENE_SETUP_CREATE_UNIT(three_curve_LabelWaterUnit, 3);
    three_curve_LabelWaterUnit.setText("kg");

    SCENE_SETUP_CREATE_TABLE(three_curve_LabelUFInit, 4);
    SCENE_SETUP_CREATE_LINEEDIT(three_curve_EditUFInit, 4);
    SCENE_SETUP_CREATE_UNIT(three_curve_LabelUFInitUnit, 4);
    three_curve_LabelUFInitUnit.setText("kg/h");

    SCENE_SETUP_CREATE_TABLE(three_curve_LabelUFMax, 5);
    SCENE_SETUP_CREATE_LINEEDIT(three_curve_EditUFMax, 5);
    SCENE_SETUP_CREATE_UNIT(three_curve_LabelUFMaxUnit, 5);
    three_curve_LabelUFMaxUnit.setText("kg/h");

    SCENE_SETUP_CREATE_TABLE(three_curve_LabelUFMaxTime, 6);
    SCENE_SETUP_CREATE_LINEEDIT(three_curve_EditUFMaxTime, 6);
    SCENE_SETUP_CREATE_UNIT(three_curve_LabelUFMaxTimeUnit, 6);
    three_curve_LabelUFMaxTimeUnit.setText("h|min");


    buttonModify.setGeometry(QRect(350, SCENE_SETUP_ROW_7, 80, 39));

    BUTTON_SIMPLE_STYLE(buttonModify);

    CustomSetText();

    curveBloodTemp.setGeometry(QRect(170, 420, 570, 300));
    curveBloodTemp.setUnit("kg/h");
    curveBloodTemp.setValueZoom(100);
    curveBloodTemp.setHorizontalScale(4);
    curveBloodTemp.setVerticalScale(14);
    curveBloodTemp.setMode(1);
    curveBloodTemp.setMinValue(0);
    curveBloodTemp.setMaxValue(150);
    curveBloodTemp.setTestTime(4 * 60 *60);
    curveBloodTemp.setUnitX(Hour);
}

SceneSetupCurveTest::SceneSetupCurveTest(QWidget *parent) :
    QObject(parent),
    curveBloodTemp(parent),
    labelCurveSel(parent),
    comboBoxCurveSel(parent),


    three_curve_LabelTreatTime(parent),
    three_curve_EditTreatTime(parent),
    three_curve_LabelTreatTimeUnit(parent),

    three_curve_LabelWater(parent),
    three_curve_EditWater(parent),
    three_curve_LabelWaterUnit(parent),

    three_curve_LabelUFInit(parent),
    three_curve_EditUFInit(parent),
    three_curve_LabelUFInitUnit(parent),

    three_curve_LabelUFMax(parent),
    three_curve_EditUFMax(parent),
    three_curve_LabelUFMaxUnit(parent),

    three_curve_LabelUFMaxTime(parent),
    three_curve_EditUFMaxTime(parent),
    three_curve_LabelUFMaxTimeUnit(parent),

    buttonModify(parent)

{
    setUi();
    PageAppendPiece("Setup CurveTest", this);
    connect(&comboBoxCurveSel, SIGNAL(currentIndexChanged(int)), this, SLOT(slotCurveSel(int)));
    connect(&buttonModify, SIGNAL(clicked()), this, SLOT(curveButtonModify()), Qt::QueuedConnection);

    ic->appendWidget(&three_curve_EditTreatTime,NumInputMethod, LineEdit, this);
    ic->appendWidget(&three_curve_EditWater,NumInputMethod, LineEdit, this);
    ic->appendWidget(&three_curve_EditUFInit,NumInputMethod, LineEdit, this);
    ic->appendWidget(&three_curve_EditUFMax,NumInputMethod, LineEdit, this);
    ic->appendWidget(&three_curve_EditUFMaxTime,NumInputMethod, LineEdit, this);

}

void SceneSetupCurveTest::slotCurveSel(int id)
{
    switch(id) {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    }
}


void SceneSetupCurveTest::generateCurve()
{
    int treatTimeHour = standCurve.treatTime * 100;
    int treatTimeMin = treatTimeHour/100 *60 + treatTimeHour%100;

    int count = treatTimeMin/15 + (((treatTimeMin%15) == 0) ? 0 : 1);
    int y = 0;

    curveBloodTemp.delAllTargetCurve();

    for(int i = 0; i < count; i++) {
        y = getThreeCurveParam(standCurve.treatTime, standCurve.threeCurve.waterValue,
            standCurve.threeCurve.InitValue, standCurve.threeCurve.maxValue, standCurve.threeCurve.maxTime, i * 15) * 100;

        curveBloodTemp.addTargetCurve(i * 15 *60, 15 * 60 , y);
        qDebug() << "y" << y;
    }
    curveBloodTemp.update();
}

void SceneSetupCurveTest::curveButtonModify()
{
    standCurve.treatTime = three_curve_EditTreatTime.text().toFloat();
    standCurve.threeCurve.waterValue = three_curve_EditWater.text().toFloat();
    standCurve.threeCurve.InitValue = three_curve_EditUFInit.text().toFloat();
    standCurve.threeCurve.maxValue = three_curve_EditUFMax.text().toFloat();
    standCurve.threeCurve.maxTime = three_curve_EditUFMaxTime.text().toFloat();
    generateCurve();
}

void SceneSetupCurveTest::CustomSetText()
{
    labelCurveSel.setText(trUtf8("超滤率曲线类型:  "));
    three_curve_LabelTreatTime.setText(trUtf8("治疗时间"));
    three_curve_LabelWater.setText(trUtf8("脱水量"));
    three_curve_LabelUFInit.setText(trUtf8("初始化超滤值"));
    three_curve_LabelUFMax.setText(trUtf8("超滤最大值"));
    three_curve_LabelUFMaxTime.setText(trUtf8("超滤最大值时刻"));

    three_curve_EditTreatTime.setText("3.45");
    three_curve_EditWater.setText("3.60");
    three_curve_EditUFInit.setText("1.04");
    three_curve_EditUFMax.setText("1.22");
    three_curve_EditUFMaxTime.setText("1.00");

    buttonModify.setText(trUtf8("修改"));
}

void SceneSetupCurveTest::setCusVisible(bool vis, char* para)
{
    para = para;

    labelCurveSel.setVisible(vis);
    comboBoxCurveSel.setVisible(vis);
    curveBloodTemp.setVisible(vis);

    three_curve_LabelTreatTime.setVisible(vis);
    three_curve_EditTreatTime.setVisible(vis);
    three_curve_LabelTreatTimeUnit.setVisible(vis);

    three_curve_LabelWater.setVisible(vis);
    three_curve_EditWater.setVisible(vis);
    three_curve_LabelWaterUnit.setVisible(vis);

    three_curve_LabelUFInit.setVisible(vis);
    three_curve_EditUFInit.setVisible(vis);
    three_curve_LabelUFInitUnit.setVisible(vis);

    three_curve_LabelUFMax.setVisible(vis);
    three_curve_EditUFMax.setVisible(vis);
    three_curve_LabelUFMaxUnit.setVisible(vis);

    three_curve_LabelUFMaxTime.setVisible(vis);
    three_curve_EditUFMaxTime.setVisible(vis);
    three_curve_LabelUFMaxTimeUnit.setVisible(vis);

    buttonModify.setVisible(vis);
}
