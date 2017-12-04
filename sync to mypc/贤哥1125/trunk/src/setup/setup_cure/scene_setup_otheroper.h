#ifndef SCENE_SETUP_OTHEROPER_H
#define SCENE_SETUP_OTHEROPER_H

#include <QtGui>
#include <QLabel>
#include "lang_switch.h"
#include "page_info.h"

class SceneSetupOherOper : public QObject, public PieceVisible, public LangSetText
{
    Q_OBJECT
public:
    SceneSetupOherOper(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

private slots:
    void buttonStandby01_clicked();
    void buttonStandby02_clicked();

private:
    inline void setUi();

private:
    QLabel labelInformation;

    QPushButton buttonStandby01;
    QPushButton buttonStandby02;

    bool buttonStandby01_flag;
    bool buttonStandby02_flag;

};

#endif
