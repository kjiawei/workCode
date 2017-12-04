#include <QtGui>
#include "config_ui.h"
#include "widget_tempo.h"

int gValueScale;

WidgetTempo::WidgetTempo(QWidget *parent) :
    QWidget(parent),
    //progressTemp(this),
    labelTitle(this),
    labelRate(this)

{
    colorBackground = QColor(18, 22, 31);

    labelTitle.setGeometry(QRect(WIDGET_TEMPO_LABEL_X,
                WIDGET_TEMPO_LABEL_Y,
                WIDGET_TEMPO_LABEL_WIDTH+50,
                WIDGET_TEMPO_LABEL_HIGH));
    labelTitle.setFont(QFont(QString(CONFIG_FONT_STYLE), 18, CONFIG_FONT_NORMAL));
    labelTitle.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelRate.setGeometry(QRect(WIDGET_TEMPO_RATE_X,
                WIDGET_TEMPO_RATE_Y,
                WIDGET_TEMPO_RATE_WIDTH,
                WIDGET_TEMPO_RATE_HIGH));
    labelRate.setFont(QFont(QString(CONFIG_FONT_STYLE), 18, CONFIG_FONT_NORMAL));
    labelRate.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    valueNow = 0;
    valueTotal = 100;
    valueRate = 0;

    //init background
    bufferImage = QImage(500,510,QImage::Format_ARGB32);

    QPainter painter(&bufferImage);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(WIDGET_TEMPO_CENTRE_X,
            WIDGET_TEMPO_CENTRE_Y);

    painter.setPen((Qt::NoPen));
    painter.setBrush(colorBackground);
    painter.drawEllipse(-218, -218, 436, 436);

    QRadialGradient radialGradient(0, 0, 213, 0, 0);
    radialGradient.setColorAt(0, QColor(31, 38, 55));
    radialGradient.setColorAt(0.9, QColor(45, 52, 68));
    radialGradient.setColorAt(1, QColor(79, 98, 112));
    painter.setBrush(QBrush(radialGradient));
    painter.drawEllipse(-213, -213, 426, 426);

    #if 0
    progressTemp.setAutoFillBackground(true);
    progressTemp.setBackgroundRole(QPalette::Background);
    progressTemp.setFocusPolicy(Qt::StrongFocus);

    progressTemp.setMaximum(1000);
    progressTemp.setMinimum(0);
    progressTemp.setValue(68);
    progressTemp.setStyleSheet(
    "QProgressBar{border: 2px solid grey;border-radius: 5px;}"
    "QProgressBar::chunk {background-color: #8e1002;}  QProgressBar{text-align: center;}"//width: 10px;margin: 0.2px;
     );
    //progressTemp.show();
   #endif

    CustomSetText();
}

void WidgetTempo::setValueTotal(int value)
{
    valueTotal = value;
}

void WidgetTempo::setValue(int value)
{
    if(value>valueTotal)
        value=valueTotal;
    valueNow = value;
    QString str;

    if (valueTotal == 0) {
        valueScale = 100;
        radian = 0;
    } else {
        valueScale = valueNow*100/valueTotal;
        radian = valueScale*3.6*(-16);
    }
    labelRate.setText(str.sprintf("%d%%", valueScale));
    gValueScale = valueScale;

    update();
}

void WidgetTempo::paintEvent(QPaintEvent *event)
{
    event = event;
    QPainter painter(this);
    QString str,strval;

    painter.setRenderHint(QPainter::Antialiasing);

    //draw background
    painter.drawImage(QPoint(0,0),bufferImage);

    painter.translate(WIDGET_TEMPO_CENTRE_X,
            WIDGET_TEMPO_CENTRE_Y);

    QRadialGradient radialGradient2(0, 0, 213, 0, 0);
    radialGradient2.setColorAt(0, QColor(146, 84, 12));
    radialGradient2.setColorAt(1, QColor(216, 167, 67));
    painter.setBrush(radialGradient2);
    painter.drawPie(-213, -213, 426, 426, 90*16, radian);

    painter.setFont(QFont(QString(CONFIG_FONT_STYLE), 16, CONFIG_FONT_NORMAL));
    painter.setPen(QPen(QColor(219, 230, 239)));

    str.clear();
//    strval.clear();
//    strval.sprintf(" %d%% ", valueScale);
//    str.append(tr("Completed"));
//    str.append(strval);
    str.append(tr("Please wait till 100%."));
    painter.drawText(-185, -8, 370, 28,
            Qt::AlignHCenter | Qt::AlignCenter,
            //str.sprintf("Completed %d%%, and need %d minutes", valueScale, 20));
            str);

}
void WidgetTempo::CustomSetText()
{
    labelTitle.setText(tr("Rinse"));
}
