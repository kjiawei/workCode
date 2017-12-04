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

#ifndef INPUTPANELCONTEXT_H
#define INPUTPANELCONTEXT_H

#include <QtGui/QInputContext>

#include "number_input_panel.h"
#include "all_input_panel.h"

#include "keysetready.h"

//! [0]
//主要是对QInputContext类进行重写（实现这个基类中的一些函数），给应用程序的输入上下文提供接口
class InputPanelContext : public QInputContext
{
    Q_OBJECT

public:
    InputPanelContext();
    ~InputPanelContext();

    //输入事件过滤，即那些事件发生时才打开软键盘
    bool filterEvent(const QEvent* event);

    //返回输入方式的识别码
    QString identifierName();
    //返回输入编码方式
    QString language();
    //输入发送完毕时调用
    bool isComposing() const;

    void reset();
    static void setPosition(int x,int y);
    static void setShowMode(int mode);
    static void setInputMethod(int inputMethod);

    //将软键盘和要输入的控制框建立联系
    void appendWidget(QWidget *watch,
                      int whichInput = NumInputMethod,
                      enum inputWidgetType InputWidgetType= LineEdit,
                      KeySetReady *handler = NULL,
                      float checkMinValue = 0,
                      float checkMaxValue = 0,
                      float Step = 0,                      
                      QString nameDisplay = "Value:",
                      QString name = "Value",
                      bool isCheck = false,
                      enum checkWidgetType CheckWidgetType= INT);

    void removeWidget(QWidget *watch);
    NumberInputPanel *numberInputPanel;//纯数字键盘面板
private slots:
    void sendCharacter(int keyId,
                       Qt::KeyboardModifiers Modifier,
                       QString character,
                       int involvedKeys);

    void wakeUp(int whichInput);
    void hideKeyboard();

private:
    //更新软键盘位置
    void updatePosition();


private:
//    NumberInputPanel *numberInputPanel;//纯数字键盘面板
    AllInputPanel *allInputPanel;//带字母键盘面板
    static QPoint pos_point;
    static int inputMethod;  //指示是纯数字还是带字母键盘输入
    static bool isShowKeyboard;//是否显示键盘面板
};

//! [0]

#endif // INPUTPANELCONTEXT_H
