#ifndef SCENE_SETUP_SYSINFO_H
#define SCENE_SETUP_SYSINFO_H

#include <QtGui>
#include <QLabel>
#include "lang_switch.h"
#include "page_info.h"

class SceneSetupSysinfo : public QObject, public PieceVisible, public LangSetText
{
    Q_OBJECT
public:
    SceneSetupSysinfo(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

private:
    inline void setUi();

private:
    QLabel labelVer;
    QLabel labelVerValue;

    QLabel labelCompileTime;
    QLabel labelCompileTimeValue;

    QPushButton buttonSave;
    QPushButton buttonCancel;
};

#endif
