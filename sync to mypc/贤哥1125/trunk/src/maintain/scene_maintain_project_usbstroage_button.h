#ifndef WIDGET_USBCONNECT_BUTTON_H
#define WIDGET_USBCONNECT_BUTTON_H

#include <QtGui>
#include <QWidget>
#include <QPainter>

class WidgetUSBStorageButton : public QPushButton
{
    Q_OBJECT
public:
    WidgetUSBStorageButton(QWidget *parent=0);

public slots:
    void draw();

protected:
    void paintEvent(QPaintEvent *event);

private:

};



#endif // WIDGET_USBCONNECT_BUTTON_H
