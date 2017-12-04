#include <QtGui>
#include "config_ui.h"
#include "page_info.h"
#include "widget_warn.h"
#include "data_set.h"

#include "global_var.h"
#include "lamp_control.h"

WidgetWarn::WidgetWarn(QWidget *parent) :
    QWidget(parent),
    widgetButtonWarn(this)
{
    widgetButtonWarn.setGeometry(QRect(WIDGETWARN_BUTTON_X,
                                          WIDGETWARN_BUTTON_Y,
                                          WIDGETWARN_BUTTON_WIDTH,
                                          WIDGETWARN_BUTTON_HIGH));

    widgetButtonWarn.setStyleSheet("QPushButton {border-style:none;background-color:transparent;}");
//    widgetButtonWarn.setTextFontPos(CFG_FONT_SIZE_FIRST_3, CONFIG_FONT_NORMAL,
//            CONFIG_COLOR_WHITE, QRect(38, 2, 90, 32));

    pixmapShadow = QPixmap(":/resource/images/button_shadow_warn.png");
    pixmapCommon = QPixmap(":/resource/images/warn/warn_common.png");
    pixmapMessage = QPixmap(":/resource/images/warn/warn_message.png");
    pixmapWarn = QPixmap(":/resource/images/warn/warn_warn.png");
    pixmapAlarm = QPixmap(":/resource/images/warn/warn_alarm.png");
    pixmapCurrent = pixmapCommon;

    connect(&widgetButtonWarn, SIGNAL(clicked()), this, SLOT(SignalPageSwitch()));

    timer->createTimer(this, 500);

    dataNodeAddObject(PACKET_NAME_BTNALARM, this);
    //dataNodeAddObject(PACKET_STATUS_NOTE, this);
//    dataNodeAddObject(PACKET_STATUS_ALARM, this);   //20151022 修改#/
    dataNodeAddObject(ALARMPRO, this);

    isFlashing = false;
    isFlashShow = false;
    flashType = 0;
}

void WidgetWarn::setWarnText(const QString& text)
{
    widgetButtonWarn.setText(text);
    update();
}


void WidgetWarn::paintEvent(QPaintEvent *event)
{
    event = event;
    QPainter painter(this);

    if (!pixmapShadow.isNull()) {
        painter.drawPixmap(WIDGET_WARN_SHADOW_X, WIDGET_WARN_SHADOW_Y, pixmapShadow);
    }

    if(pixmapCurrent.isNull())
        painter.drawPixmap(WIDGETWARN_BUTTON_X, WIDGETWARN_BUTTON_Y, pixmapCommon);

    if(isFlashShow) {
        isFlashing = !isFlashing;
        if(isFlashing)
            painter.drawPixmap(WIDGETWARN_BUTTON_X, WIDGETWARN_BUTTON_Y, pixmapCurrent);
        else
            painter.drawPixmap(WIDGETWARN_BUTTON_X, WIDGETWARN_BUTTON_Y, pixmapCommon);
    }

    if(flashType == 0)
    {
        painter.drawPixmap(WIDGETWARN_BUTTON_X, WIDGETWARN_BUTTON_Y, pixmapCommon);
    }
}

void WidgetWarn::ActionHandle(const void *dataPtr, int name)
{

//
#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetWarn-ActionHandle-in");
#endif

    if(name==PACKET_NAME_BTNALARM) {
        unsigned int type = *((unsigned int *)dataPtr);

        setAlarmColor(type);
        /*
        if(type>0)
        {
            pixmapCurrent = pixmapAlarm;
            flashType=1;
            isFlashShow = true;
        }
        else if(type==0)
        {
//          pixmapCurrent = pixmapAlarm;
            flashType=0;
            isFlashShow = false;
            this->update();
        }
        */
    }
}

void WidgetWarn::setAlarmColor(unsigned int type)
{
    switch(type) {

    case 0:
        isFlashShow = false;
        flashType = 0;
        this->update();
        break;

    case 1: //message
        pixmapCurrent = pixmapMessage;
        flashType=1;
        isFlashShow = true;
        break;

    case 2: //alarm
        pixmapCurrent = pixmapWarn;
         flashType=2;
        isFlashShow = true;
        break;

    case 3: //warn
        pixmapCurrent = pixmapAlarm;
         flashType=3;
        isFlashShow = true;
        break;
    case 4: //ownNote
        pixmapCurrent = pixmapMessage;
         flashType=4;
        isFlashShow = true;
        break;
    }
}

void WidgetWarn::timer_handler(const int timeOut)
{
    Q_UNUSED(timeOut);
#if DEBUG_TIMER_HANDLE
    Debug_insert("WidgetWarn-timer-in");
#endif
    if(isFlashShow)
    {
        this->update();
    }
    else if(Alarmcolorflag>0)
    {
        setAlarmColor(Alarmcolorflag);
    }
#if DEBUG_TIMER_HANDLE
    Debug_insert("WidgetWarn-timer-out");
#endif
}

void WidgetWarn::SignalPageSwitch()
{
    PageShow("Warn History");
}


