#ifndef WIDGET_CROSS_H
#define WIDGET_CROSS_H

#include <QLabel>
#include <QDateTime>
#include <QButtonGroup>
#include "config_ui.h"
#include "page_info.h"
#include "widget_button.h"
#include "widget_cross_extern.h"
#include "data_custom.h"

#define WIDGETCROSS_WIDTH                   193
#define WIDGETCROSS_HIGH                    237

#define WIDGETCROSS_BUTTON_DISINFECT_X      2
#define WIDGETCROSS_BUTTON_DISINFECT_Y      157
#define WIDGETCROSS_BUTTON_DISINFECT_WIDTH  78
#define WIDGETCROSS_BUTTON_DISINFECT_HIGH   81

#define WIDGETCROSS_BUTTON_INFO_X           0
#define WIDGETCROSS_BUTTON_INFO_Y           0
#define WIDGETCROSS_BUTTON_INFO_WIDTH       83
#define WIDGETCROSS_BUTTON_INFO_HIGH        86

#define WIDGETCROSS_BUTTON_EXT_X           76
#define WIDGETCROSS_BUTTON_EXT_Y           85
#define WIDGETCROSS_BUTTON_EXT_WIDTH       82
#define WIDGETCROSS_BUTTON_EXT_HIGH        76

#define WIDGETCROSS_BUTTON_HOME_X           15
#define WIDGETCROSS_BUTTON_HOME_Y           94
#define WIDGETCROSS_BUTTON_HOME_WIDTH       51
#define WIDGETCROSS_BUTTON_HOME_HIGH        52

#define WIDGETCROSS_CENTRE_X                41
#define WIDGETCROSS_CENTRE_Y                120

#define WIDGETCROSS_LABEL_TIME_X            100
#define WIDGETCROSS_LABEL_TIME_Y            196
#define WIDGETCROSS_LABEL_TIME_WIDTH        80
#define WIDGETCROSS_LABEL_TIME_HIGH         18

#define WIDGETCROSS_LABEL_DATE_X            78
#define WIDGETCROSS_LABEL_DATE_Y            220
#define WIDGETCROSS_LABEL_DATE_WIDTH        115
#define WIDGETCROSS_LABEL_DATE_HIGH         18

class WidgetCross : public QWidget, public PieceVisible,public ActionCustom
{
    Q_OBJECT
public:
    WidgetCross(QWidget *parent=0);
    void setCusVisible(bool vis=false, char* para=NULL);
signals:
    void clickEvent(int key);
private:
    void ActionHandle(const void* dataPtr=NULL, int name=0);

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void pushButtonInfoClicked(void); //日志信息按钮处理
    void pushButtonExtClicked(void);  //扩展按钮处理
    void pushButtonHomeClicked(void); //主按钮
    void pushButtonSetupClicked(void);//设置按钮
    void pushButtonDisinfectClicked(void);//消毒按钮
    void mouseClicked(int id);

private:
    QButtonGroup buttonGroupCross;
    WidgetButton pushButtonSetup;
    WidgetCrossExtern widgetSetupExtern; //设置的菜单选择框
    WidgetButton pushButtonDisinfect;
    WidgetButton pushButtonInfo;
    WidgetButton pushButtonExt;
    WidgetButton pushButtonHome;
    WorkState workstate;
};

#endif
