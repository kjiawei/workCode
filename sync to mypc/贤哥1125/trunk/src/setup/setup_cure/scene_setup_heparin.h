#ifndef SCENE_SETUP_HEPARIN_H
#define SCENE_SETUP_HEPARIN_H

#include <QtGui>
#include "packet.h"
#include "xml_config.h"
#include "config_xml.h"
#include "lang_switch.h"
#include "page_info.h"
#include "data_custom.h"
#include "widget_curve.h"
#include "widget_button.h"

#include "keysetready.h"

#define SCENE_HEPARIN_CURVE_X 15
#define SCENE_HEPARIN_CURVE_Y 460

class SceneSetupHeparin : public QObject, public ActionCustom, public PieceVisible,
    public LangSetText, public KeySetReady
{
    Q_OBJECT
public:
    SceneSetupHeparin(QWidget *parent=0);
    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

    void lineEdit_init();
    void lineEdit_close(const char *name) {
        name = name;
    }

    void lineEdit_data_set(const char *name, const char *data) {
        name = name;
        data = data;
        SlotParameterSave();
    }

public slots:
    void SlotParameterSave();
    void StopHeparinPump();
    void SlotApplyClicked();
    void SlotAddSingleClicked();
    void ApplyStatusHeparin(int id);

private:
    void Init();
    inline void setUi();
    void StateSwitch(const unsigned int dst=0);

private:
    QLabel labelSyringeStyle;
    QLabel labelSyringeStyleValue;
    QLabel labelSyringeStyleunit;

    //
    QLabel labelShotTotal;  //累计量#/
    QLabel labelShotTotalValue;
    QLabel labelShotTotalUnit;

    //
    QLabel labelFirstDose;  //首剂量#/
    QLineEdit lineEditFirstDose;
    QLabel labelFirstDoseUnit;

    //
    QLabel labelShotSpeed;  //注射速度#/
    QLineEdit lineEditShotSpeed;
    QLabel labelShotSpeedUnit;

    //
    QLabel labelTimePreStop;  //预先停止时间#/
    QLineEdit lineEditTimePreStop;
    QLabel labelTimePreStopUnit;

    QCheckBox radioButtonSelectYes;
    QButtonGroup groupHeparin;
    
    //
    QLabel labelAddSingle;   //追加量#/
    QLineEdit lineEditAddSingle;
    QLabel labelAddSingleUnit;
    WidgetButton buttonAddSingle;

    WidgetCurve curveHeparin;

    QPushButton buttonSave;
    QPushButton buttonApply;
    QPushButton buttonCancel;

private:
    void setInputValue(int value);

private:
    PacketHeparinPump heparinData;
    PacketPDOHeparinPump heparinPDO;
    struct Params *params;

    bool isKeyInit;
    bool AddSingle_flag;
};

#endif
