#ifndef WIDGET_TEMPO_H
#define WIDGET_TEMPO_H

#define WIDGET_TEMPO_X              321
#define WIDGET_TEMPO_Y              85
#define WIDGET_TEMPO_WIDTH          478
#define WIDGET_TEMPO_HIGH           504

#define WIDGET_TEMPO_LABEL_X        0
#define WIDGET_TEMPO_LABEL_Y        0
#define WIDGET_TEMPO_LABEL_WIDTH    120
#define WIDGET_TEMPO_LABEL_HIGH     22

#define WIDGET_TEMPO_RATE_X         290
#define WIDGET_TEMPO_RATE_Y         31
#define WIDGET_TEMPO_RATE_WIDTH     60
#define WIDGET_TEMPO_RATE_HIGH      32

#define WIDGET_TEMPO_CENTRE_X       239
#define WIDGET_TEMPO_CENTRE_Y       285

#include <QLabel>
#include "lang_switch.h"

class WidgetTempo : public QWidget, public LangSetText
{
    Q_OBJECT

public:
    WidgetTempo(QWidget *parent = 0);
    void CustomSetText();

    void setValue(int value = 0);
    void setValueTotal(int value = 0);
    QLabel labelTitle;

protected:
    void paintEvent(QPaintEvent *event);

private:
    QColor colorBackground;

    QLabel labelRate;

    QImage bufferImage;

    int valueNow;
    int valueTotal;
    int valueRate;
    int valueScale;
    float radian;
};

#endif
