#ifndef WIDGET_CONNECT_WLAN_ICON_H
#define WIDGET_CONNECT_WLAN_ICON_H

#include <QtGui>
#include <QWidget>
#include <QPainter>

class WidgetConnectWlanIcon : public QWidget
{
    Q_OBJECT
public:
    WidgetConnectWlanIcon(QWidget *parent=0);
    static int showWlanPicture;

public slots:
    void draw();

protected:
    void paintEvent(QPaintEvent *event);

private:

};



#endif // WIDGET_CONNECT_WLAN_ICON_H
