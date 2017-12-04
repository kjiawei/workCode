#ifndef WIDGET_COMMON_H
#define WIDGET_COMMON_H

#include "config_ui.h"
#include "global_var.h"
#include <QInputContext>

#define LINEEDIT_STYLE(x)                       \
    (x).setStyleSheet(                            \
            "QLineEdit {"                       \
            "color:#cdcdcd;"                    \
            "border: 1px groove #0f131b;"       \
            "border-radius: 9px;"               \
            "padding: 0 8px;"                   \
            "background: #123f75;"              \
            "}");                               \
    (x).setFont(QFont(QString(CONFIG_FONT_STYLE), \
            CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
    (x).setAlignment(Qt::AlignCenter)

#define LINEEDIT_SHOWSTYLE(x)                       \
    (x).setStyleSheet(                            \
            "QLineEdit {"                       \
            "color:#000000;"                    \
            "border: 1px groove #0f131b;"       \
            "border-radius: 9px;"               \
            "padding: 0 8px;"                   \
            "background: #acacac;"              \
            "}");                                \
    (x).setFont(QFont(QString(CONFIG_FONT_STYLE), \
            CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
    (x).setAlignment(Qt::AlignCenter)

#define LINENOEDIT_STYLE(x)                       \
    (x).setStyleSheet(                            \
            "QLineEdit {"                       \
            "color:#cdcdcd;"                    \
            "border-style:none;"                \
            "padding: 0 8px;"                   \
            "background-color:transparent;"              \
            "}");                               \
    (x).setFont(QFont(QString(CONFIG_FONT_STYLE), \
            CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
    (x).setAlignment(Qt::AlignCenter)


#define COMBOBOX_STYLE(x)                       \
    (x).setStyleSheet("QComboBox {"               \
                "color:#cdcdcd;"                \
                "border-radius: 9px;"           \
                "background: #123f75;"          \
                "padding: 0 8px;"               \
                "}")

#define RADIOBUTTON_STYLE(x)                    \
    (x).setStyleSheet("QRadioButton {color:#dce6f0;}"     \
            "QRadioButton::indicator {width: 29px;height: 31px;} "  \
            "QRadioButton::indicator::checked {image: url(:/resource/images/radio_button_check.png);} "  \
            "QRadioButton::indicator::unchecked {image: url(:/resource/images/radio_button_uncheck.png);}")

#define LABEL_TITLE_STYLE(x)                    \
    (x).setAlignment(Qt::AlignVCenter | Qt::AlignLeft);       \
    (x).setFont(QFont(QString(CONFIG_FONT_STYLE), \
                CFG_FONT_SIZE_TITLE, CONFIG_FONT_NORMAL));  \
    (x).setStyleSheet(CONFIG_COLOR_WHITE_CHARS)

#define LABEL_REMIND_TITLE(x)                    \
    (x).setAlignment(Qt::AlignVCenter | Qt::AlignRight);      \
    (x).setFont(QFont(QString(CONFIG_FONT_STYLE),             \
                CFG_FONT_SIZE_FOURTH, CONFIG_FONT_NORMAL)); \
    (x).setStyleSheet(CONFIG_COLOR_WHITE_CHARS)

#define LABEL_REMIND_VALUE(x)                    \
    (x).setAlignment(Qt::AlignVCenter | Qt::AlignCenter);       \
    (x).setFont(QFont(QString(CONFIG_FONT_STYLE),             \
                CFG_FONT_SIZE_FOURTH, CONFIG_FONT_NORMAL)); \
    (x).setStyleSheet(CONFIG_COLOR_WHITE_CHARS)

#define LABEL_REMIND_UNIT(x)                    \
    (x).setAlignment(Qt::AlignVCenter | Qt::AlignLeft);       \
    (x).setFont(QFont(QString(CONFIG_FONT_STYLE),             \
                CFG_FONT_SIZE_FOURTH, CONFIG_FONT_NORMAL)); \
    (x).setStyleSheet(CONFIG_COLOR_WHITE_CHARS)

/*
#define BUTTON_SIMPLE_STYLE(x)                    \
    (x).setFont(QFont(QString(CONFIG_FONT_STYLE), 15));   \
    connect(&x, SIGNAL(clicked()), audio, SLOT(beep()),)
    //x.setStyleSheet(CONFIG_COLOR_BLUE_CHARS)

*/

#define BUTTON_SIMPLE_STYLE(x)                    \
    (x).setFont(QFont(QString(CONFIG_FONT_STYLE), 15))

/*
#define MESSAGE_PROMPT_BOX(title,text)    \
       QMessageBox::information(0,title,text,QMessageBox::Ok)
*/

#define MESSAGE_PROMPT_BOX(title,text)     \
    QMessageBox informat(QMessageBox::Information,title,text,QMessageBox::Yes);\
    informat.setWindowFlags(Qt::Widget); \
    informat.setWindowFlags(Qt::FramelessWindowHint);\
    informat.move(300,300);  \
    informat.exec();

//工程设置界面专用提示框#/
#define MESSAGE_ENGINEER_PROMPT_BOX(title,text)     \
    QMessageBox informat(QMessageBox::Information,title,text,QMessageBox::Yes);\
    informat.setWindowFlags(Qt::Widget); \
    informat.setStyleSheet("background-color:yellow");\
    informat.move(450,300);  \
    informat.exec();

void MainQuit(int sig=0);

class WindowBottom : public QWidget
{
public:
    WindowBottom(QWidget *parent=0):QWidget(parent){}
    void closeEvent(QCloseEvent *event) {
        event = event;
        MainQuit();
    }
protected:
    void mousePressEvent(QMouseEvent *event);

};


#endif
