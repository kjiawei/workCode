#include <QtGui>
#include <string.h>
#include "config_ui.h"
#include "widget_check_self.h"
#include "global_var.h"
#include "scene_main.h"

WidgetCheckSelf::WidgetCheckSelf(QWidget *parent) :
    QWidget(parent),
    progressSelfcheck(this),
    labelTitle(this) //左上角文字#/
{
    progressSelfcheck.setGeometry(QRect(80,120,400,50));
    progressSelfcheck.setOrientation(Qt::Horizontal);
    progressSelfcheck.setTextVisible(true);


    labelTitle.setGeometry(QRect(WIDGET_CHECK_ITEM_TITLE_X+160,
                WIDGET_CHECK_ITEM_TITLE_Y+60,
                WIDGET_CHECK_ITEM_TITLE_WIDTH+50,
                WIDGET_CHECK_ITEM_TITLE_HIGH));
    labelTitle.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_FIRST_2, CONFIG_FONT_NORMAL));
    labelTitle.setStyleSheet(CONFIG_COLOR_YELLOW_CHARS);//CONFIG_COLOR_LABEL


    CustomSetText();
    bloodroadOk=false;
    waterroadOk=false;

    progressSelfcheck.setAutoFillBackground(true);
    progressSelfcheck.setBackgroundRole(QPalette::Background);
    progressSelfcheck.setFocusPolicy(Qt::StrongFocus);

    progressSelfcheck.setMaximum(60*40);//40分钟#/
    progressSelfcheck.setMinimum(0);
    progressSelfcheck.setValue(timeCount);
    progressSelfcheck.setObjectName("progressbar");

    QFont font;
    font.setPointSize(32);
    progressSelfcheck.setFont(font); // 设置进度条里面的字体#/
    progressSelfcheck.setStyleSheet(
    "QProgressBar{border: 2px solid grey;border-radius: 5px;}"
    "QProgressBar::chunk {background-color: #00ff00;}  QProgressBar{text-align: center;}"
     );
}

/*
void WidgetCheckSelf::paintEvent(QPaintEvent *event)
{
    event = event;

    QPainter painter(this);
    painter.translate(SELFCK_RECT_CENTRE_X, SELFCK_RECT_CENTRE_Y-50);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QColor(145, 145, 145), 0, Qt::SolidLine));
    painter.setBrush(QBrush(Qt::transparent,Qt::SolidPattern)); //设置画刷形式
    painter.drawRect(-222,-82+20,400,50);

    painter.setPen(QPen(Qt::yellow, 0, Qt::SolidLine));//QColor(255, 255,255)边框颜色
    painter.setBrush(QBrush(Qt::green,Qt::SolidPattern)); //设置画刷形式
    painter.drawRect(-222+2,-82+3+20,265,50-6);

    painter.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CONFIG_FONT_POINTSIZE_BIG, CONFIG_FONT_NORMAL));
    painter.setPen(QPen(Qt::blue));//QColor(112, 210, 255)
    painter.drawText(QRect(-50, -92+20, 120, 72), Qt::AlignCenter,
                     "%68");//QString::number(2, 10)

}*/


void WidgetCheckSelf::keyPressEvent(QKeyEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *eventKey = static_cast<QKeyEvent*>(event);
        switch (eventKey->key()) {
            case Qt::Key_F12:
#ifdef _MODULE_SCREEN_SHOT 
                //screen_shot(NULL);
#endif
                break;
            default:
                break;
        }
    }
}

void WidgetCheckSelf::CustomSetText()
{
    labelTitle.setText(tr("Self-testing"));
}



