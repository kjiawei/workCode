#ifndef WIDGET_CHECK_SELF_H
#define WIDGET_CHECK_SELF_H

#include <QtGui>
#include <QTimer>
#include <QProgressBar>
#include <QWidget>
#include "page_info.h"
#include "lang_switch.h"
#include "data_custom.h"
#include "widget_common.h"
#include "packet_ui.h"
#include "data_timer.h"

#define _WidgetCheckself_WIDTH                200
#define _WidgetCheckself_HIGH                 450

#define WINDOW_CHECK_TITLE_X        144
#define WINDOW_CHECK_TITLE_Y        90
#define WINDOW_CHECK_TITLE_WIDTH    200
#define WINDOW_CHECK_TITLE_HIGH     26

#define WIDGET_CHECK_ITEM_TITLE_X   0
#define WIDGET_CHECK_ITEM_TITLE_Y   0
#define WIDGET_CHECK_ITEM_TITLE_WIDTH   200
#define WIDGET_CHECK_ITEM_TITLE_HIGH    20

#define WINDOW_CHECK_ITEM_LEFT_X    145
#define WINDOW_CHECK_ITEM_RIGHT_X   610

#define WINDOW_CHECK_ITEM_Y         155
#define WINDOW_CHECK_ITEM_ADD_Y     110

#define SELFCK_RECT_CENTRE_X         300
#define SELFCK_RECT_CENTRE_Y         222
/*!
 * \brief The WindowCheckSelf class
 */
class WidgetCheckSelf : public QWidget, public LangSetText//public PieceVisible,
{
    Q_OBJECT

public:
    WidgetCheckSelf(QWidget *parent=0);
    void CustomSetText();
    QProgressBar  progressSelfcheck;
    QLabel labelTitle;

protected:
//    void paintEvent(QPaintEvent *event); //可以 Remove
    void keyPressEvent(QKeyEvent *event);

    /*
public:
    QMovie *movie;           //可以 Remove
    QPixmap pixmapFillRed;   //可以 Remove
    QPixmap pixmapFillGray;  //可以 Remove
    QPixmap pixmapFillGreen;  //可以 Remove
    QPixmap pixmapFillister;  //可以 Remove
    */

private:
    int timeCount;
    bool checkBloodResult;
    bool checkWaterResult;
    int bloodroadOk;
    int waterroadOk;
    bool isSelfcheck;
};

#endif
