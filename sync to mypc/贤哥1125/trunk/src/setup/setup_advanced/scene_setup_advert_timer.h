#ifndef SCENE_SETUP_ADVERTTIMER_H
#define SCENE_SETUP_ADVERTTIMER_H

#include <QtGui>
#include <QLabel>
#include "lang_switch.h"
#include "page_info.h"
#include "input_panel_context.h"


class SceneSetupAdvertTimer : public QObject, public PieceVisible, public LangSetText
{
    Q_OBJECT
public:
    SceneSetupAdvertTimer(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

private:
    inline void setUi();
    QTimer timer_advert;
    QTime  timeCount;

    int count;
    bool onetime;
    bool cycletime;
private:
    QLabel labelAbsolutTime;
    QLabel labelAbsolutTimeUnit;

    QTimeEdit   OnetimeEdit;
    QTimeEdit   CyctimeEdit;

    QLabel labelRelativeTime;
    QLabel labelRelativeTimeUnit;

    QLabel labelRemainTime;
    QLineEdit lineEditRemainTime;
    QLabel labelRemainTimeUnit;

    QLabel labelButtonOnetime;
    QPushButton buttonOnetime;
    QPushButton buttonCyctime;

    QLabel labelButtonSeleSound;
    QPushButton ButtonSeleSound;
    QPushButton ButtonSeleOption;

    QPushButton buttonSave;
    QPushButton buttonCancel;

private slots:
    void button_Onetimer_clicked();
    void button_Cyctimer_clicked();
    void timerDone();

protected:
    //void paintEvent(QPaintEvent *event);
};

#endif
