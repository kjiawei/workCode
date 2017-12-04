
#ifndef SCENE_BLOODVOLUME_H
#define SCENE_BLOODVOLUME_H

#include <QtGui>
#include "page_info.h"
#include "lang_switch.h"
#include "widget_curve.h"

#include "input_panel_context.h"
#include "scene_setup_layout.h"
#include "keysetready.h"
#include  <QMap>
#include "bloodvolume_task.h"

struct BloodVloumeData{
    time_t time;
    float   bloodVolume;
};

class SceneBloodVolume : public QWidget, public PieceVisible, public LangSetText
        , public KeySetReady, public ActionCustom
{
    Q_OBJECT

public:
    SceneBloodVolume(QWidget *parent=0);

    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void setUi(void);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);
    void lineEdit_init();
    void lineEdit_data_set(const char* name = NULL, const char* data = NULL) {
        name = name;
        data = data;
    }

    void lineEdit_close(const char *name) {
        name = name;
    }

private:
    void mouseMoveEvent (QMouseEvent * event);

private slots:
    void on_buttonApply_clicked();

private:
    QLabel labelWeight;
    QLineEdit lineEditWeight;
    QLabel labelKg;

    QLabel labelBloodVolume;
    QLineEdit lineEditBloodVolume;

    QPushButton buttonApply;
    int bloodVolumeTime;
    int thirtyMinUf;
    float thirtyMinBloodV;
    QMap<int ,int> bloodVolume_map;
};

#endif

