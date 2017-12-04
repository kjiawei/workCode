#ifndef SCENE_SETUP_DIALYSATE_H
#define SCENE_SETUP_DIALYSATE_H

#include <QtGui>
#include <QLabel>
#include "data_set.h"
#include "lang_switch.h"
#include "xml_config.h"
#include "page_info.h"
#include "data_custom.h"
#include "keysetready.h"
class SceneSetupDialysate : public QObject, public PieceVisible, public LangSetText,
    public ActionCustom, public KeySetReady
{
    Q_OBJECT
public:
    SceneSetupDialysate(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);
    void ActionHandle(const void* dataPtr=NULL, int name=0);

    void lineEdit_init(){}
    void lineEdit_close(const char *name){name = name;}
    void lineEdit_data_set(const char *name, const char *data){
            name = name;
            data = data;
            Confirm();
    }

private:
    inline void setUi();

private:
    QLabel labelConcentrationNa;
    QLineEdit lineEditConcentrationNa;
    QLabel labelConcentrationNaUnit;

    QLabel labelConcentrationHCO3;
    QLineEdit lineEditConcentrationHCO3;
    QLabel labelConcentrationHCO3Unit;

    QLabel labelVelocity;
    QLineEdit lineEditVelocity;
    QLabel labelVelocityUnit;

    QLabel labelTemp;
    QLineEdit lineEditTemp;
    QLabel labelTempUnit;

private:
    QPushButton buttonSave;
    QPushButton buttonCancel;
    QPushButton buttonCurve;

private:
    FormulaSet formulaSet;
private slots:
    void Confirm();
    void on_curve_clicked();
};

#endif
