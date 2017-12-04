#ifndef SCENE_SETUP_Blood_Temp_H
#define SCENE_SETUP_Blood_Temp_H

#include <QtGui>
#include <QLabel>
#include "lang_switch.h"
#include "page_info.h"
#include "widget_curve.h"
#include "data_custom.h"
class SceneSetupBloodTemp : public QObject, public PieceVisible, public LangSetText,public ActionCustom
{
    Q_OBJECT
public:
    SceneSetupBloodTemp(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);
    void ActionHandle(const void* dataPtr=NULL, int name=0);

private:
    inline void setUi();

private:
    QLabel labelBloodTemp;
    QLabel labelBloodTempValue;
    QLabel labelBloodTempUnit;

    WidgetCurve curveBloodTemp;
};

#endif
