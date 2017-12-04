#ifndef SCENE_SETUP_EC_H
#define SCENE_SETUP_EC_H

#include <QtGui>
#include "page_info.h"
#include "lang_switch.h"
#include "widget_curve.h"
#include "widget_data_curve.h"

#define SCENE_EC_CURVE_X 10
#define SCENE_EC_CURVE_Y 330

class SceneSetupEC : public QObject, public PieceVisible, public LangSetText
{
    Q_OBJECT

public:
    SceneSetupEC(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);
    void curve_Handler();  //曲线刷新处理#/

private:
    QLabel labelNaC;
    QLabel labelNaCValue;
    QLabel labelNaCUnit;

    QLabel labelEC;
    QLabel labelECValue;
    QLabel labelECUnit;
};

#endif
