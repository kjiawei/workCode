#include "scene_maintain_project_usbstroage_button.h"
#include "scene_maintain.h"

WidgetUSBStorageButton::WidgetUSBStorageButton(QWidget *parent) :
    QPushButton(parent)
{

}

void WidgetUSBStorageButton::draw()
{
    this->update();
}

void WidgetUSBStorageButton::paintEvent(QPaintEvent *event)
{
    event = event;
    QPainter painter(this);
    if (SceneMaintain::isUsbStroageExist) {
        QPixmap picture(":/resource/images/usbStorage.png");
        painter.drawPixmap(0, 0, picture);
    } else {
        painter.drawText(974, 20, "");
    }
}

