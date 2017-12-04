#ifndef SCENE_SETUP_PREFLUSH_H
#define SCENE_SETUP_PREFLUSH_H

#include <QtGui>
#include "lang_switch.h"
#include "page_info.h"
#include "keysetready.h"

#include "data_custom.h"
#include "global_var.h"

class SceneSetupPreflush  : public QObject, public PieceVisible, public LangSetText,
        public KeySetReady, public ActionCustom

{
    Q_OBJECT

public:
    SceneSetupPreflush(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);
    void ActionHandle(const void* dataPtr=NULL, int name=0);

    void lineEdit_init();
    void lineEdit_close(const char *name) {
        name = name;
    }

    void lineEdit_data_set(const char *name, const char *data) {
        name = name;
        data = data;
        save();
    }

private:
    //void ActionHandle(const void* dataPtr=NULL, int name=0);
    inline void setUi();
    //void WriteXmlPreflushConfig(const char *file=DEFAULT_XML_FILE, const char *cmd=DEFAULT_XML_LOAD);

private:
    //手动预冲血泵流速#/
    QLabel labelManualPreflushBloodSpeed;
    QLineEdit lineEditManualPreflushBloodSpeed;
    QLabel labelManualPreflushBloodSpeedUnit;

    //预冲血泵流速#/
    QLabel labelPreflushBloodSpeed;
    QLineEdit lineEditPreflushBloodSpeed;
    QLabel labelPreflushBloodSpeedUnit;

    //手动预冲超滤速度#/
    QLabel lineManualPreflushUfRate;
    QLineEdit lineEditManualPreflushUfRate;
    QLabel lineManualPreflushUfRateUnit;

    //预冲时间#/
    QLabel labelPreflushTime;
    QLineEdit lineEditPreflushTime;
    QLabel labelPreflushTimeUnit;

    //预冲超滤率#/
    QLabel labelPreflushUfRate;
    QLineEdit lineEditPreflushUfRate;
    QLabel labelPreflushUfRateUnit;

    //预冲超滤量#/
    QLabel labelPreflushUfVolume;
    QLineEdit lineEditPreflushUfVolume;
    QLabel labelPreflushUfVolumeUnit;    

    //引血血流量#/
    QLabel labelReturnBloodVolume;
    QLineEdit lineEditReturnBloodVolume;
    QLabel labelReturnBloodVolumeUnit;

    //已预冲量#/
    QLabel labelPreFlushVolume;
    QLineEdit lineEditPreFlushVolume;
    QLabel labelPreFlushVolumeUnit;

    QButtonGroup groupPreprime;
    QRadioButton radioAutoPreflush;//自动预冲#/
    QRadioButton radioManualOperation; //手动预冲#/
    QRadioButton radioOnlinePreflush;   //在线自动预冲#/

    QButtonGroup Blood_lead;
    QRadioButton lead_yes;
    QRadioButton lead_no;

    QButtonGroup Blood_return;
    QRadioButton return_yes;
    QRadioButton return_no;

    //保存按钮#/
    QPushButton buttonSave;
    //取消按钮#/
    QPushButton buttonCancel;

private:
    struct Params *params;
    bool isKeyInit;
    PacketPreflush Preflush;
    Engineer *engineerset;
    enum PreflushModel choosePreflushModel;
private slots:
    void save();
    void radioChange(void);
    void Group_lead(int id);
    void Group_return(int id);
};

#endif // SCENE_SETUP_PREFLUSH_H
