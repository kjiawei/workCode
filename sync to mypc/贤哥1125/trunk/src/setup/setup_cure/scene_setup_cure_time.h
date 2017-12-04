#ifndef SCENE_SETUP_CURE_TIME_H
#define SCENE_SETUP_CURE_TIME_H

#include <QtGui>
#include <QLabel>
#include "lang_switch.h"
#include "page_info.h"
#include "data_set.h"
#include "data_custom.h"

#include "keysetready.h"


class SceneSetupCureTime : public QObject, public PieceVisible, public LangSetText
        , public ActionCustom, public KeySetReady
{
    Q_OBJECT
public:
    SceneSetupCureTime(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

    void ActionHandle(const void* dataPtr=NULL, int name=0);

    void lineEdit_init();
    void lineEdit_data_set(const char *name, const char *data);
    void lineEdit_close(const char *name);
    void lineEdit_start(const char *name, float *max, float *min);
private:
    inline void setUi();

private slots:
    void save();

private:
    QLabel testlabel;//测试label

    QLabel labelTime;
    QLineEdit lineEditTime;

    QLabel labelmintext;
    QLabel labelmin;
    QLabel labelminunit;
    QLabel labelmaxtext;
    QLabel labelmax;
    QLabel labelmaxunit;

    QPushButton buttonSave;
    QPushButton buttonCancel;

private:
    Params params;
    QString timestr;
    bool isKeyInit;
    int Timer_state;
    float min_curetime;  //最小可设置治疗时间
    float max_curetime;   //最大可设置治疗时间
};

#endif
