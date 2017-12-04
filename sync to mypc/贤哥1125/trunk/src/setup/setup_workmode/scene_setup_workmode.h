#ifndef SCENE_SETUP_WORKMODE_H
#define SCENE_SETUP_WORKMODE_H
#include <QDialog>
#include <QLabel>
#include <QButtonGroup>
#include <QRadioButton>
#include <QGroupBox>
#include "packet.h"
#include "xml_config.h"
#include "lang_switch.h"
#include "data_custom.h"
#include "page_info.h"
#include "widget_button.h"

#define Widget_Setup_Menu_X             12
#define Widget_Setup_Menu_Y             57
#define Widget_Setup_Menu_Width         147
#define Widget_Setup_Menu_High          696

class SceneSetupWorkmode : public QObject, public PieceVisible, public LangSetText,
    public XmlConfig, public ActionCustom//,public QDialog
{
    Q_OBJECT
public:
    SceneSetupWorkmode(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);
    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void CustomXmlConfig(const char *file=DEFAULT_XML_FILE, const char *cmd=DEFAULT_XML_LOAD);

private:
    void setUi();
    void showCurrentMode();

private slots:
    void statusDilute(int id);
    void buttonSaveClicked();
    void buttonCancelClicked();
    void NoteSel(int sel);
//protected:
    //void showEvent (QShowEvent * event);
private:
    QLabel labelTitle;
    QLabel labelCureMode;
    QLabel labelFeedMode;
    QFrame frameLineFirst;
    QFrame frameLineTwo;

    QRadioButton radioButtonHD;
    QRadioButton radioButtonHDF;
    QRadioButton radioButtonHF;
    QRadioButton radioButtonIsouf;
    QRadioButton radioButtonPE;

    QWidget stageTwo;
    QRadioButton radioButtonNeedleSingle;
    QRadioButton radioButtonNeedleSingleDP;//单针双泵#/
    QRadioButton radioButtonNeedleDouble;//双针双泵#/
    QRadioButton radioButtonDiluteFront;
    QRadioButton radioButtonDiluteBehind;
    QRadioButton radioButton_HFDiluteFront;
    QRadioButton radioButton_HFDiluteBehind;

    QRadioButton radioButtonABf;
    QRadioButton radioButtonAfBs;
    QRadioButton radioButtonAcetate;
    QRadioButton radioButtonCentreInflow;

    QButtonGroup groupCureMode;
    QButtonGroup groupDilute;
    QButtonGroup groupHF;
    QButtonGroup groupFeedMode;
    QButtonGroup groupOther;

    QLabel labelScale;
    QLabel labelScaleValue;

    QLabel labelNaAc;
    QLabel labelNaAcvalue;
    QLabel labelNaAcUnit;

    QLabel labelNa;
    QLabel labelNavalue;
    QLabel labelNaUnit;

    QLabel labelCl;
    QLabel labelClvalue;
    QLabel labelClUnit;

    QLabel labelK;
    QLabel labelKvalue;
    QLabel labelKUnit;

    QLabel labelHCO3;
    QLabel labelHCO3value;
    QLabel labelHCO3Unit;

    QLabel labelMg;
    QLabel labelMgvalue;
    QLabel labelMgUnit;

    QLabel labelAcetate;
    QLabel labelAcetatevalue;
    QLabel labelAcetateUnit;

    QLabel labelCa;
    QLabel labelCavalue;
    QLabel labelCaUnit;

    QLabel labelBNacl;
    QLabel labelBNaclvalue;
    QLabel labelBNaclUnit;
private:
    QPushButton buttonSave;
    QPushButton buttonCancel;

private:
    DataCustom *workMode;
    PacketWorkMode workModeData;
    bool iSshow;
    int modeltmp,fluidmodetmp;
    bool flagPageFrom;//flase表示来自其他页面，true来自本页面
};

#endif
