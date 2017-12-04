#ifndef WIDGET_BUTTON_H
#define WIDGET_BUTTON_H

#include <QPushButton>
#include <QMovie>

enum ButtonType
{
    BUTTON_TYPE_NORMAL,
    BUTTON_TYPE_SWITCH_TWO,
    BUTTON_TYPE_SWITCH_THREE,

    /* ... */
    BUTTON_TYPE_NULL
};

class WidgetButton : public QPushButton 
{
    Q_OBJECT
public:
    WidgetButton(QWidget *parent=0, ButtonType nuttonType=BUTTON_TYPE_NORMAL);

    void setText(const QString &);
    QString	text () const;
    void setTextFontPos(const int pointSize,  const int weight,
            const int color=0xff000000, const QRect &rect=QRect(0,0,0,0));

    bool setStyleNormal(const char *xpm=NULL,
                const QRect& rect=QRect(0, 0, 60, 60));

    // Create our own style, named Sky.
    bool setStyleSky(const char *xpm=NULL, const char *xpmActive=NULL,
                const QRect& rect=QRect(0, 0, 60, 60));
    bool setStyleSwitch(const char *xpm=NULL, const char *xpmActive=NULL,
                const int colorTextSwitch=0xff000000,
                const QRect& rect=QRect(0, 0, 60, 60));

    //create QIcon disabled style
    bool setStyleDisabled(const char *xpm, const char *xpmActive,
                                        const char *xpmDisabled, const int ColorTextSwitch,
                                        const QRect& rect);

    //设置按钮闪动#/
    void setFlash(bool falsh);
    //设置按钮闪动时的图片路径#/
    void setStyleMovie(QString imgPath);

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    int playSound(void);
    //播放图片#/
    void moviePlay(int);
    void changeIcon();

private:
    QIcon icon;
    QMovie *movie;
    QRect rectText;  //按钮字体位置尺寸#/
    QFont fontText;  //按钮字体#/
    QColor colorText;  //非选中时字体颜色#/
    QColor colorTextSwitch;  //选中时字体颜色
    QString stringButton; //按键内容

    QColor colorBack;
    ButtonType buttonType;
};

#endif
