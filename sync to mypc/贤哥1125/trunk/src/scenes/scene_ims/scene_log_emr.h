#ifndef SCENE_LOG_EMR_H
#define SCENE_LOG_EMR_H

#include <QtGui>
#include <QLabel>
#include "lang_switch.h"
#include "page_info.h"

class SceneLogEmr : public QObject, public PieceVisible, public LangSetText
{
    Q_OBJECT
public:
    SceneLogEmr(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

private:
    inline void setUi();

private:
    QPushButton buttonSave;
    QPushButton buttonCancel;
};

#endif
