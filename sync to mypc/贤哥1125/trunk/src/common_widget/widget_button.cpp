#include <QPainter>
#include <QWidget>
#include "config_ui.h"
#include "global_var.h"
#include "common_func.h"
#include "widget_button.h"
#include "audioserver.h"

WidgetButton::WidgetButton(QWidget *parent, ButtonType buttonType) :
    QPushButton(parent)
{
    this->buttonType = buttonType;

    movie = NULL;
    //setStyleSheet("QPushButton{border:0pxgroovegray;}");
    setFocusPolicy(Qt::NoFocus);

    connect(this, SIGNAL(clicked()), this, SLOT(playSound()));
    connect(this, SIGNAL(clicked()), this, SLOT(changeIcon()));
}

void WidgetButton::setText(const QString& string)
{
    this->stringButton = string;

    update();
}

QString	WidgetButton::text () const
{
    return stringButton;
}

void WidgetButton::setTextFontPos(const int pointSize,
        const int weight,
        const int color,
        const QRect &rect)
{
    colorText.setRgb(color);
    fontText = QFont(QString(CONFIG_FONT_STYLE), pointSize, weight);
    if (rect.width() > 0) {
        rectText = rect;
    }
}

void WidgetButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);

    if (!stringButton.isEmpty()) {
        QPainter painter(this);
        painter.setFont(fontText);
        if (isChecked() == true) {
            painter.setPen(QPen(colorTextSwitch));
        } else {
            painter.setPen(QPen(colorText));
        }
        painter.drawText(rectText, Qt::AlignCenter|Qt::AlignVCenter, stringButton);
    }
}

bool WidgetButton::setStyleNormal(const char *xpm, const QRect& rect)
{
    resize(rect.size());
    icon.addFile(QString::fromUtf8(xpm), QSize(),
            QIcon::Normal, QIcon::Off);
    setIcon(icon);

    setGeometry(rect);
    if (!icon.isNull()) {
        setIconSize(rect.size());
    }
    rectText = QWidget::rect();
    setStyleSheet("QPushButton{border:0pxgroovegray;}");
    return true;
}

/*
 * Create our own style, named Sky. bounce back
 */
bool WidgetButton::setStyleSky(const char *xpm,
        const char *xpmActive,
        const QRect& rect)
{
    setCheckable(true);
    //将各状态下的按键图片添加到图标类#/
    icon.addPixmap(QString(xpm),
            QIcon::Normal, QIcon::Off);
    icon.addPixmap(QString(xpmActive),
            QIcon::Active, QIcon::On);
    setIcon(icon);

    setGeometry(rect);
    if (!icon.isNull()) {
        setIconSize(rect.size());
    }
    rectText = QWidget::rect();
    setStyleSheet("QPushButton{border:0pxgroovegray;}");
    setFocusPolicy(Qt::NoFocus);

    return true;
}

bool WidgetButton::setStyleSwitch(const char *xpm, const char *xpmActive,
        const int ColorTextSwitch, const QRect& rect)
{
    setCheckable(true);
    icon.addPixmap(QString(xpm),
            QIcon::Normal, QIcon::Off);
    icon.addPixmap(QString(xpmActive),
            QIcon::Active, QIcon::On);
    setIcon(icon);

    setGeometry(rect);
    if (!icon.isNull()) {
        setIconSize(rect.size());
    }
    rectText = QWidget::rect();
    colorTextSwitch.setRgb(ColorTextSwitch);
    setStyleSheet("QPushButton{border:0pxgroovegray;}");
    setFocusPolicy(Qt::NoFocus);

    return true;
}

bool WidgetButton::setStyleDisabled(const char *xpm, const char *xpmActive,
                                    const char *xpmDisabled,const int ColorTextSwitch,
                                    const QRect& rect)
{
    setCheckable(true);

    icon.addPixmap(QString(xpm),QIcon::Normal, QIcon::Off);
    icon.addPixmap(QString(xpmActive),QIcon::Active, QIcon::On);

    icon.addPixmap(QString(xpmDisabled),QIcon::Disabled, QIcon::Off);
    icon.addPixmap(QString(xpmDisabled),QIcon::Disabled, QIcon::On);
    setIcon(icon);

    setGeometry(rect);
    if (!icon.isNull()) {
        setIconSize(rect.size());
    }
    rectText = QWidget::rect();
    colorTextSwitch.setRgb(ColorTextSwitch);
    setStyleSheet("QPushButton{border:0pxgroovegray;}");
    setFocusPolicy(Qt::NoFocus);

    return true;
}

//设置是否键闪模式#/
void WidgetButton::setFlash(bool falsh)
{
    if (falsh)
    {
        if (movie != NULL)
        {
            movie->start();
        }
    }
    else
    {
        if (movie != NULL)
        {
            movie->stop();
        }

        setIcon(icon);
    }
}

//设置键闪动画的路径#/
void WidgetButton::setStyleMovie(QString imgPath)
{
    if (movie == NULL)
    {
        movie = new QMovie(imgPath, QByteArray(), this);
        connect(movie, SIGNAL(frameChanged(int)), this, SLOT(moviePlay(int)));
    }
}

//播放键闪动画#/
void WidgetButton::moviePlay(int)
{
    if (movie != NULL)
    {
        setIcon(movie->currentPixmap());
    }
}

//主要是键闪后，单击按键，退出键闪模式
void WidgetButton::changeIcon()
{
    setFlash(false);
}

int WidgetButton::playSound(void)
{

//    audio->beep();

    return 0;
}
