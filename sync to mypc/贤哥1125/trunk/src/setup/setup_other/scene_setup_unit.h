#ifndef SCENE_SETUP_UNIT_H
#define SCENE_SETUP_UNIT_H

#include <QtGui>
#include <QLabel>
#include "lang_switch.h"
#include "page_info.h"

class SceneSetupUnit : public QObject, public PieceVisible, public LangSetText
{
    Q_OBJECT
public:
    SceneSetupUnit(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

private:
    inline void setUi();

private:
    QLabel labelLen;
    QComboBox comboLen;

    QLabel labelPressure;
    QComboBox comboPressure;

private:
    QPushButton buttonSave;
    QPushButton buttonCancel;
};

#endif
