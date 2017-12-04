#ifndef SCENE_SETUP_ULTRAFILTRATION_H
#define SCENE_SETUP_ULTRAFILTRATION_H

#include <QtGui>
#include "lang_switch.h"
#include "page_info.h"
#include "widget_curve.h"
#include "data_custom.h"

#include "keysetready.h"

#define SCENE_SETUP_UF_LABEL_X                       10
#define SCENE_SETUP_UF_LABEL_WIDTH                   250
#define SCENE_SETUP_UF_LABEL_HIGH                    22

#define SCENE_SETUP_UF_LABEL_ABOVE_UF_TIME_Y         85
#define SCENE_SETUP_UF_LABEL_UF_TARGET_Y             135
#define SCENE_SETUP_UF_LABEL_TIME_Y                  185
#define SCENE_SETUP_UF_LABEL_ACTUAL_AMOUNT_Y         235

#define SCENE_SETUP_UF_LINEEDIT_X                    270
#define SCENE_SETUP_UF_LINEEDIT_WIDTH                116
#define SCENE_SETUP_UF_LINEEDIT_HIGH                 38


class SceneSetupUf : public QObject, public ActionCustom, public PieceVisible, public LangSetText ,
        KeySetReady
{
    Q_OBJECT
public:
    SceneSetupUf(QWidget *parent=0);
    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

    void lineEdit_init();
    void lineEdit_data_set(const char *name, const char *data);
    void lineEdit_start(const char *name, float *max, float *min);
    void lineEdit_close(const char *name);
    void LableUpdate(const char *name);
private:
    inline void setUi();

public slots:
    void SlotParameterSave();
    void SlotParameterCancel();
    void on_buttonCurve_clicked();

private:
    //Ultrafiltration target
    QLabel labelUfTarget;
    QLineEdit lineEditUfAim;
    QLabel labelUfTargetUnit;

    //Ultrafiltration rate
    QLabel labelUfRate;
    QLineEdit lineEditUfRate;
    QLabel labelUfRateUnit;

    QLabel labelNowUfRateText;
    QLabel labelNowUfRateValue;
    QLabel labelNowUfRateUnit;

    //The actual amount of ultrafiltration
    QLabel labelActualAmountUf;
    QLabel labelActualAmountUfValue;
    QLabel labelActualAmountUfunit;

    QPushButton buttonCurve;
    QPushButton buttonSave;
    QPushButton buttonCancel;


private:
    struct PacketUF packetUFinit;
    struct PacketUF packetUF;
    bool isKeyInit;
    struct Params *params;
};

#endif
