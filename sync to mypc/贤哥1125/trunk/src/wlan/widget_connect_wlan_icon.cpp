#include "widget_connect_wlan_icon.h"

WidgetConnectWlanIcon::WidgetConnectWlanIcon(QWidget *parent) :
    QWidget(parent)
{

}

void WidgetConnectWlanIcon::draw()
{
    update();
}

void WidgetConnectWlanIcon::paintEvent(QPaintEvent *event)
{
    event = event;
    QPainter painter(this);

    if (showWlanPicture)
    {
        QPixmap picture(":/resource/images/radio_button_check.png");
        painter.drawPixmap(0,0,31,31,picture);
    }
    else
    {   QPixmap picture(":/resource/images/radio_button_uncheck.png");
        painter.drawPixmap(0,0,31,31,picture);
    }
}

int WidgetConnectWlanIcon::showWlanPicture = 0;
