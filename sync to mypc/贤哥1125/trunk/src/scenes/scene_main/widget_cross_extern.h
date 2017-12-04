#ifndef WIDGET_CROSS_EXTERN_H
#define WIDGET_CROSS_EXTERN_H

#include <QLabel>
#include "config_ui.h"
#include "lang_switch.h"
#include "widget_button.h"

#define WIDGET_CROSS_EXTEND_X           582
#define WIDGET_CROSS_EXTEND_Y           506
#define WIDGET_CROSS_EXTEND_WIDTH       170
#define WIDGET_CROSS_EXTEND_HIGH        172

#define WIDGET_CROSS_EXTEND_BUTTON_X        10
#define WIDGET_CROSS_EXTEND_WORKMODE_Y      8
#define WIDGET_CROSS_EXTEND_CUREINFO_Y      48
#define WIDGET_CROSS_EXTEND_DIALYSYSLIQUID_Y  88
#define WIDGET_CROSS_EXTEND_OTHER_Y         128

#define WIDGET_CROSS_EXTEND_BUTTON_WIDTH    140
#define WIDGET_CROSS_EXTEND_BUTTON_HIGH     38

class WidgetCrossExtern : public QWidget, public LangSetText
{
    Q_OBJECT
public:
    WidgetCrossExtern(QWidget *parent=0);
    void CustomSetText();

private slots:
    void pushButtonWorkModeClicked(void);
    void pushButtonCureInforClicked(void);
    void pushButtonAdvancedClicked(void);
    void pushButtonOtherClicked(void);

protected:
    void paintEvent(QPaintEvent *event);

private:
    WidgetButton pushButtonWorkMode;
    WidgetButton pushButtonCureInfor;
    WidgetButton pushButtonAdvanced;
    WidgetButton pushButtonOther;

    QPixmap pixmapBackGround;
};

#endif
