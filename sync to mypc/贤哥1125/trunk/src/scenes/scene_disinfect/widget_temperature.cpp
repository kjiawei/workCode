#include <QtGui>
#include "config_ui.h"
#include "widget_temperature.h"

WIDGET_TEMPERATURE::WIDGET_TEMPERATURE(QWidget *parent,Qt::WindowFlags f) :
    QWidget(parent,f),
    progressTemp(this),
    labelTemp(this)
{
    colorBackground = QColor(18, 22, 31);

    labelTemp.setGeometry(QRect(10,
                351,
                60,
                32));
    labelTemp.setFont(QFont(QString(CONFIG_FONT_STYLE), 18, CONFIG_FONT_NORMAL));
    labelTemp.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    #if 1
    progressTemp.setGeometry(QRect(25, 0,10,346+5));
    progressTemp.setOrientation(Qt::Vertical);
    progressTemp.setTextVisible(false);

    progressTemp.setAutoFillBackground(true);
    progressTemp.setBackgroundRole(QPalette::Background);
    progressTemp.setFocusPolicy(Qt::StrongFocus);

    progressTemp.setMaximum(1000);
    progressTemp.setMinimum(0);
    //setTempVal(68);
    //progressTemp.setValue(68);
    progressTemp.setStyleSheet(
    "QProgressBar{border: 2px solid grey;border-radius: 5px;}"
    "QProgressBar::chunk {background-color: #8e1002;}  QProgressBar{text-align: center;}"//width: 10px;margin: 0.2px;
     );
    //progressTemp.show();
    #endif
    //*/
    dataNodeAddObject(PACKET_MASTERCTRL_HEAT, this);

    //connect(this, SIGNAL(tempSignal(int)), this, SLOT(setTempVal(int)));

    CustomSetText();

}

void WIDGET_TEMPERATURE::paintEvent(QPaintEvent *event)
{
    event = event;
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);

    QLinearGradient linearGradient(0, 0, 50, 336);
    linearGradient.setColorAt(0.0, Qt::red);
    linearGradient.setColorAt(0.2, Qt::yellow);
    linearGradient.setColorAt(1.0, Qt::darkGray);

    painter.setPen(QPen(QBrush(linearGradient), 30));
    painter.drawLine(30, 0, 30, 336);

    //progressTemp.setValue(temp);

}

void WIDGET_TEMPERATURE::CustomSetText()
{
//    labelTemp.setText("0C");
    labelTemp.setText("T");
}

void WIDGET_TEMPERATURE::setTempVal(int val)
{
    progressTemp.setValue(val);
}

void WIDGET_TEMPERATURE::ActionHandle(const void* dataPtr, int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("temperature-ActionHandle-in");
#endif
    //if(!this->isVisible())
        //return;
    if ((dataPtr == NULL) || (name == 0)) {
        return;
    }

    if (name==PACKET_MASTERCTRL_HEAT){
        pHeatTmp = *(PacketHeat *) dataPtr;

        progressTemp.setValue(pHeatTmp.N3_heat_temp);
/*
        QString str;
        str=QString::number(pHeatTmp.N3_heat_temp/10.0,'f',1);
        str.append("C");
        labelTemp.setText(str);
*/
        //temp=pHeatTmp.N3_heat_temp;
        //update();
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("temperature-ActionHandle-out");
#endif
}

