#ifndef SCENE_SETUP_Curve_Test_H
#define SCENE_SETUP_Curve_Test_H

#include <QtGui>
#include <QLabel>
#include "lang_switch.h"
#include "page_info.h"
#include "widget_curve.h"
#include "keysetready.h"
extern "C" {
    float getThreeCurveParam(int min);
    float getPaoWuXianCurveParam(int min);
}


struct ThreeCurve{
    float waterValue;
    float InitValue;
    float maxValue;
    float maxTime;
};

struct LineCurve {
    float initValue;
};

struct StandCurve {
    char curveType;     //治疗类型
    float treatTime;    //治疗时间
    struct ThreeCurve threeCurve;
    struct LineCurve lineCurve;
};

class SceneSetupCurveTest : public QObject, public PieceVisible, public LangSetText, public KeySetReady
{
    Q_OBJECT
public:
    SceneSetupCurveTest(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

    void lineEdit_init(){}
    void lineEdit_close(const char *name) {
        name = name;
    }
    void lineEdit_data_set(const char *name, const char *data) {
        name = name;
        data = data;
        curveButtonModify();
    }
public slots:
    void slotCurveSel(int id);
    void curveButtonModify();
private:
    inline void setUi();
    void generateCurve();
private:
    QList<QWidget*> listW;
    WidgetCurve curveBloodTemp;

    QLabel labelCurveSel;
    QComboBox comboBoxCurveSel;

    QLabel three_curve_LabelTreatTime;
    QLineEdit three_curve_EditTreatTime;
    QLabel three_curve_LabelTreatTimeUnit;

    QLabel three_curve_LabelWater;
    QLineEdit three_curve_EditWater;
    QLabel three_curve_LabelWaterUnit;

    QLabel three_curve_LabelUFInit;
    QLineEdit three_curve_EditUFInit;
    QLabel three_curve_LabelUFInitUnit;

    QLabel three_curve_LabelUFMax;
    QLineEdit three_curve_EditUFMax;
    QLabel three_curve_LabelUFMaxUnit;

    QLabel three_curve_LabelUFMaxTime;
    QLineEdit three_curve_EditUFMaxTime;
    QLabel three_curve_LabelUFMaxTimeUnit;

    QPushButton buttonModify;
};

#endif
