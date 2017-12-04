/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MYINPUTPANEL_H
#define MYINPUTPANEL_H

#include <QtGui>
#include <QtCore>

#include "virtualKeyboard_public.h"
#include "ins_key.h"

#include "widget_button.h"

#include "keysetready.h"
#include "scene_log_run.h"
//#include "ui_virtual_keyboard.h"

#define KEY_WIDTH  45
#define KEY_HEIGHT 44
#define KEY_BOTTOM 460
#define KEY_LEFT  13
#define KEY_DISTANCE 12

#define OLD_DEFAULT_VALUE "old    "

#define CONFIG_KEYLABLE_STYLE(x)                       \
    x -> setStyleSheet("QLabel {"       \
                "color:#ffffff;"                \
                "border-radius: 4px;"           \
                "background: #192131;"          \
                "padding: 0 4px;"               \
                "}");


struct InputNumWidget
{
    QWidget *watch;
    int whichInput;
    enum inputWidgetType InputWidgetType;
    bool isCheck;
    float checkMinValue;
    float checkMaxValue;
    enum checkWidgetType CheckWidgetType;
    float Step;                     //键盘加减号对应的自加自减的值#/
    KeySetReady *handler;           //处理set数据#/
    QString nameDisplay;            //在软键盘显示的名称
    QString name;
};


//! [0]

class NumberInputPanel : public QWidget
{
    Q_OBJECT

public:
    NumberInputPanel();
    ~NumberInputPanel();
    QWidget * getFocusWidget();
    QWidget * getLineEdit();

    void appendWidget(QWidget *watch,
                      int whichInput = NumInputMethod,
                      enum inputWidgetType InputWidgetType= LineEdit,
                      KeySetReady *handler = NULL,
                      float checkMinValue = 0,
                      float checkMaxValue = 0,
                      float Step = 0 ,                      
                      QString nameDisplay = "Value:",
                      QString name = "Value",
                      bool isCheck = false,
                      enum checkWidgetType CheckWidgetType= INT);

    void removeWidget(QWidget *watch);

    void setText();


signals:
    void characterGenerated(int keyId,Qt::KeyboardModifiers Modifier,
                            QString ch,int involvedKeys);
    void wakeUpKeyboard(int whichInput);
    void hideKeyboard();
    void editFinished();
    void RunLog(logInfo log);
protected:
    bool event(QEvent *e);
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    void saveFocusWidget(QWidget */*oldFocus*/, QWidget *newFocus);
    void buttonClicked(int btn);
    void buttonPressed(int btn);
    void buttonReleased(int btn);
    void button_confirm_Clicked();
    void button_add_Clicked();
    void button_sub_Clicked();
    void button_del_Clicked();
    void closeKeyboard();

private:
    //Ui::Number_Form form;
    QWidget *lastFocusedWidget; //最后关联的一个输入编辑框#/
    QSignalMapper signalClick;
    QSignalMapper signalPressed;
    QSignalMapper signalReleased;
    QList<QPushButton *> allButtons;
    //test_dialog *dialog;
    QLabel * labelValueOld; //旧数值显示
    QLineEdit * labelValueNew; //新数值编辑框，之所以不用QLabel，是为了可以点击其中一位编辑#/
    QLabel *labelValueNewBk;
    QLabel *labelNewBk;

    QLabel * labelName;
    bool isNeg;
    WidgetButton * button_0;
    QPushButton * button_1;
    QPushButton * button_2;
    QPushButton * button_3;
    QPushButton * button_4;
    QPushButton * button_5;
    QPushButton * button_6;
    QPushButton * button_7;
    QPushButton * button_8;
    QPushButton * button_9;
    QPushButton * button_add;
    QPushButton * button_sub;
    QPushButton * button_del;
    QPushButton * button_period;
    QPushButton * button_confirm;
    QPushButton * button_cancel;


    QLabel * Label_max;
    QLabel * Label_min;

    QPixmap pix;
    QList <InputNumWidget> focusWidgetList;
    bool insertMode;
    InputNumWidget inputWidget; //关联输入编辑框的信息结构体#/
    INS_KEY ins;
    bool isOnRun;

public:
    int typeInput;
    int my_typeInput;    
};

//! [0]

#endif // MYINPUTPANEL_H
