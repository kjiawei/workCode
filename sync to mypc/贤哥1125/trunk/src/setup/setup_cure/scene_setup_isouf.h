#ifndef SCENE_SETUP_ISODF_H
#define SCENE_SETUP_ISODF_H

#include <QtGui>
#include <QLabel>
#include "lang_switch.h"
#include "page_info.h"
#include "packet.h"
#include "keysetready.h"
#include "data_custom.h"

class SceneSetupIsoUF : public QObject, public PieceVisible,public ActionCustom,public LangSetText ,
        KeySetReady
{
    Q_OBJECT
public:
    SceneSetupIsoUF(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

    void lineEdit_init();
    void lineEdit_data_set(const char *name, const char *data);
    void lineEdit_start(const char *name, float *max, float *min);
    void lineEdit_close(const char *name){name=name;}
private:
    inline void setUi();
    void ActionHandle(const void* dataPtr=NULL, int name=0);

#if HJX_COMMUNICATION_SWITCH
signals:
    void signalSDO(int id);
#endif
private slots:
    void Save(void);

private:
    QLabel labelIsoufTimeHour;
    QLineEdit lineEditIsoufTimeHour;
    QLabel labelIsoufTimeHourUnit;

    //QLabel labelIsoufTimeMin;
    QLineEdit lineEditIsoufTimeMin;
    QLabel labelIsoufTimeMinUnit;


    QLabel labelIsoufTotal;
    QLineEdit lineEditIsoufTotal;
    QLabel labelIsoufTotalUnit;

    QLabel labelActualAmountIsoUf;
    QLabel labelActualAmountIsoUfValue;
    QLabel labelActualAmountIsoUfunit;

    QPushButton buttonSave;
    QPushButton buttonCancel;
private:
    PacketISO packetISO;
    struct Params *params;
};

#endif
