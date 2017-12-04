#ifndef SCENE_SETUP_OXIMETER_H
#define SCENE_SETUP_OXIMETER_H

#include <QtGui>
#include <QLabel>
#include "page_info.h"
#include "lang_switch.h"
#include "widget_curve.h"
#include "data_custom.h"
#include <QTimer>

class SceneSetupSpO2 : public QObject, public PieceVisible, public LangSetText
,public ActionCustom
{
    Q_OBJECT
public:
    SceneSetupSpO2(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

private:
    inline void setUi();
    void ActionHandle(const void* dataPtr=NULL, int name=0);
private slots:
    void curveUpdate();
private:
    QLabel labelSpO2;
    QLabel labelSpO2Value;
    QLabel labelSpO2Unit;

    WidgetCurve curveSpO2;
    PacketBloodState BloodState;

    QTimer oxtimer;
    int   secpass ;
    QTime timePoint;

};

#endif
