#ifndef SCENE_SETUP_SUBSPUMP_H
#define SCENE_SETUP_SUBSPUMP_H

#include <QtGui>
#include <QLabel>
#include "lang_switch.h"
#include "page_info.h"
#include "widget_button.h"
#include "data_custom.h"
#include "keysetready.h"
class SceneSetupSubspump : public QObject, public PieceVisible,
        public LangSetText,public ActionCustom ,public KeySetReady
{
    Q_OBJECT
public:
    SceneSetupSubspump(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);
    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void lineEdit_init();
    void lineEdit_start(const char *name, float *max, float *min);
    void lineEdit_close(const char *name);
    void lineEdit_data_set(const char *name, const char *data);


private:
    inline void setUi();
#if HJX_COMMUNICATION_SWITCH
signals:
    void signalSDO(int id);
#endif
private slots:
    void applyAddQuick();
    void SlotApply();
    void Save();
    void slotVolumeSel(int id);

private:
    QLabel labelDTtitle;  //稀释类型题目#/
    QLabel labelDTtext;   //稀释类型内容#/

    QLabel labelUFBPtitle;//超滤/血液流速比#/
    QLabel labelUFBPtext;

    QLabel labelBPspeedtitle;//血泵1流速#/
    QLabel labelBPspeedtext;
    QLabel labelBPspeedUnit;

    QLabel labeldialyspeedtitle;//透析夜流速#/
    QLabel labeldialyspeedtext;
    QLabel labeldialyspeedUnit;

    QLabel labelSubsSpeed;  //置换泵速度#/
    QLineEdit lineEditSubsSpeed;
    QLabel labelSubsSpeedUnit;

    QLabel labelSubsTotal;  //目标量#/
    QLineEdit lineEditSubsTotal;
    QLabel labelSubsTotalUnit;

    QLabel labelSubsValue;   //已置换量#/
    QLabel labelSubsValueHas;
    QLabel labelSubsValueUnit;

    QLabel labelAddValue;   //已补液量#/
    QLabel labelAddValueHas;
    QLabel labelAddValueUnit;

    QLabel labelAddQuick;   //单次在线补液#/
    QLineEdit lineEditAddQuick;
    QLabel labelAddQuickUnit;
    WidgetButton buttonAddQuick;

    QComboBox comboBoxSubBpSpeed;

    bool statusAddQuick;//补液状态
    bool statusBtRun;
    QPalette pal;

    QPushButton buttonSave;
    QPushButton buttonCancel;
    QPushButton buttonRun;
    PacketSubBloodPump SubBPump;
    int subsValueHas; //已置换量
    int addValueHas; //总共已补液量
    int addValueNow; //当次补液量
    int addValueTmp; //补液临时累计量
    bool isKeyInit;
};

#endif
