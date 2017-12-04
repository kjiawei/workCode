#ifndef WIDGET_TEMPERATURE_H
#define WIDGET_TEMPERATURE_H

#include <QtGui>
#include "lang_switch.h"
#include <QProgressBar>
#include "data_custom.h"

class WIDGET_TEMPERATURE : public QWidget , public LangSetText
,public ActionCustom
{
    //Q_OBJECT
public:
    WIDGET_TEMPERATURE(QWidget * parent = 0, Qt::WindowFlags f = 0);
    void CustomSetText();

    //void setTempVal(int val);

protected:
    void paintEvent(QPaintEvent *event);
private:
    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void setTempVal(int val);
private:
    QProgressBar progressTemp;
    QColor colorBackground;
    QLabel labelTemp;
    int temp;
    PacketHeat pHeatTmp;
};

#endif
