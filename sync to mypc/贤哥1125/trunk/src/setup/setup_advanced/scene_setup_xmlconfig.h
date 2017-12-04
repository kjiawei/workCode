#ifndef SCENE_SETUP_XMLCONFIG_H
#define SCENE_SETUP_XMLCONFIG_H

#include <QtGui>
#include <QLabel>
#include "lang_switch.h"
#include "page_info.h"

class SceneSetupXmlconfig : public QObject, public PieceVisible, public LangSetText
{
    Q_OBJECT
public:
    SceneSetupXmlconfig(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

private:
    inline void setUi();

private:
    QLabel labelLoad;
    QPushButton buttonXmlPathLoad;
    QPushButton buttonXmlExeLoad;

    QLabel labelSave;
    QPushButton buttonXmlPathSave;
    QPushButton buttonXmlExeSave;

private slots:
    void buttonXmlPathLoad_clicked();
    void buttonXmlExeLoad_clicked();
    void buttonXmlPathSave_clicked();
    void buttonXmlExeSave_clicked();

private:
    QPushButton buttonSave;
    QPushButton buttonCancel;
};

#endif
