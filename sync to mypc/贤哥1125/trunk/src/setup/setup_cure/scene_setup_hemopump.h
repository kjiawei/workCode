#ifndef SCENE_SETUP_HEMOPUMP_H
#define SCENE_SETUP_HEMOPUMP_H

#include <QtGui>
#include "xml_config.h"
#include "config_xml.h"
#include "lang_switch.h"
#include "data_custom.h"
#include "page_info.h"

#include "keysetready.h"

class SceneSetupHemopump : public QObject, public PieceVisible, public ActionCustom,
    public LangSetText , public KeySetReady
{
    Q_OBJECT
public:
    SceneSetupHemopump(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);
    void lineEdit_init();
    void lineEdit_close(const char *name);
    void lineEdit_data_set(const char *name, const char *data);
    void lineEdit_start(const char *name, float *max, float *min);

private:
    void Init();
    inline void setUi();
    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void BPspeed_Handle(unsigned short int set_speed);      //速度相关处理#/
#if HJX_COMMUNICATION_SWITCH
signals:
    void signalSDO(int id);
#endif
public slots:
    void SlotApply();
    void SlotParameterSave();
    void SlotParameterCancel();

private:
    QLabel labelHemoSpeed;
    QLineEdit lineEditHemoSpeed;
    QLabel labelHemoSpeedUnit;

    QLabel labelSupplySpeed;
    QLineEdit lineEditSupplySpeed;
    QLabel labelSupplySpeedUnit;

    QLabel labelHemoTotal;
    QLabel labelHemoTotalValue;
    QLabel labelHemoTotalUnit;

    QLabel labelPumpSelect;

    QButtonGroup buttonGroup;
    QRadioButton radioButtonSelectYes;
    QRadioButton radioButtonSelectNo;

    QPushButton buttonSave;
    QPushButton buttonApply;
    QPushButton buttonCancel;

private:
    struct Params *params;
    struct PacketSdoBlood sdoBlood;
    struct PacketBloodPump packetBloodPump;
    struct PacketBloodPumpTube packetBloodPumpTube;
    bool isKeyInit;
    bool setspeed_clicked_flag;   //血泵设置速度按键按下标志#/
};

#endif
