#ifndef SCENE_SETUP_SYSTEM_TIME_H
#define SCENE_SETUP_SYSTEM_TIME_H

#include <QtGui>
#include <QLabel>
#include "lang_switch.h"
#include "page_info.h"

class SceneSetupSystemTime : public QObject, public PieceVisible, public LangSetText
{
    Q_OBJECT
public:
    SceneSetupSystemTime(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

private slots:
    void SlotSystemTimeSet();

private:
    inline void setUi();

private:
    QLabel labelDate;
    QDateEdit dateEdit;

    QLabel labelTime;
    QTimeEdit timeEdit;

    QLabel labelZone;
    QComboBox comboBoxZone;

    QPushButton buttonSave;
    QPushButton buttonCancel;
};

#endif
