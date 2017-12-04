#ifndef WINDOW_BOOT_H
#define WINDOW_BOOT_H

#include <QLabel>
#include <QTimer>
#include <QWidget>
#include "lang_switch.h"
#include "widget_button.h"
#include "widget_common.h"
#include "page_info.h"
#include "data_timer.h"

#include "packet.h"

#define BOOT_RING_CENTRE_X          514
#define BOOT_RING_CENTRE_Y          222

#define BOOT_SCREEN_LABEL_X         362
#define BOOT_SCREEN_LABEL_Y         346
#define BOOT_SCREEN_LABEL_WIDTH     300
#define BOOT_SCREEN_LABEL_HIGH      25

#define BOOT_SCREEN_BUTTON_X        456
#define BOOT_SCREEN_BUTTON_Y        436
#define BOOT_SCREEN_BUTTON_WIDTH    113
#define BOOT_SCREEN_BUTTON_HIGH     49

class WindowBoot : public QWidget, public LangSetText, public PieceVisible
        , public timer_handle
{
    Q_OBJECT

public:
    WindowBoot(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);
    void timer_handler(const int timeOut = 0);
    void Selfcheck_Pass(void);
    void startRecive(void);

private slots:
    void PassCheck(void);
    void DebugMode_Enter(void);
    void ConfirmDebugMode(int sel);
    void ConfirmSelect(int sel);

protected:
    void paintEvent(QPaintEvent *event);
private:
   void startMsg(void);
private:
    int timeCount;
    int timeNow;

    QPixmap pixmapShaft;
    QLabel label;
    WidgetButton widgetButton;
    QPushButton ButtonDebugEnter;
    bool startDelay;
    int startDelayCnt;
};

#endif
