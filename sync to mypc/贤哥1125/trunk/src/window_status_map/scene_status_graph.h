#ifndef SCENE_STATUS_GRAPH_H
#define SCENE_STATUS_GRAPH_H

#include <QtGui>
#include "page_info.h"
#include "lang_switch.h"
#include "data_custom.h"


class SceneStatusGraph : public QWidget, public LangSetText,
    public PieceVisible, public ActionCustom
{
    Q_OBJECT
public:
    SceneStatusGraph(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

private:
    inline void setUi();
    void ActionHandle(const void* dataPtr=NULL, int name=0);

protected:
    void paintEvent(QPaintEvent *event);
    void StatusDraw();

private:
    QLabel labelP2Power;
    QLabel labelT204Temp;
    QLabel labelC702Cond;

    QLabel labelP3Power;
    QLabel labelT201Temp;
    QLabel labelC701Cond;
    //QLabel labelC701CondTarget;

    QLabel labelT203Temp;
    QLabel labelC709Cond;
    QLabel labelC709Target;

    QLabel labelT205Temp;
    QLabel labelC704Cond;

    QLabel labelCP1WP;

    QLabel labelBloodSpeed;

    QLabel labelP1Power;
    QLabel labelCP2Pre;
    QLabel labelJP1Rate;
    QLabel labelJP1Now;

    //QLabel labelHeatPower;
    QLabel labelT202Power;
    QLabel labelT202Temp;
    QLabel labelT202Target;

    QLabel labelJP3Stand;
    QLabel labelJP3Now;
    QLabel labelJP2Stand;

    QLabel labelJP2Now;

    QLabel labelDB501Leak;

    QLabel labelAP;
    QLabel labelVP;
    QLabel labelTMP;

    QLabel labelP2Speed;
    QLabel labelD6Speed;

private:
    QPoint point[32];
    PacketValve valve;

    PacketOutlet masterCtrlOutlet;
};

#endif
