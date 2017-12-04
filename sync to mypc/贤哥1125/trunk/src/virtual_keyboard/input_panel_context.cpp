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

#include <QtCore>
#include <Qt>
#include "input_panel_context.h"

//! [0]
//构造函数 创建两个输入面板，并建立其和输入上下文类的联系（信号和槽）
InputPanelContext::InputPanelContext()
{
    numberInputPanel = new NumberInputPanel;
    connect(numberInputPanel,
            SIGNAL(characterGenerated(int,Qt::KeyboardModifiers,QString,int)),
            SLOT(sendCharacter(int,Qt::KeyboardModifiers,QString,int)));

    connect(numberInputPanel,SIGNAL(wakeUpKeyboard(int)),this,SLOT(wakeUp(int)));
    connect(numberInputPanel,SIGNAL(hideKeyboard()),this,SLOT(hideKeyboard()));

    allInputPanel = new AllInputPanel;
    connect(allInputPanel,
            SIGNAL(characterGenerated(int,Qt::KeyboardModifiers,QString,int)),
            SLOT(sendCharacter(int,Qt::KeyboardModifiers,QString,int)));

    connect(allInputPanel,SIGNAL(wakeUpKeyboard(int)),this,SLOT(wakeUp(int)));
    connect(allInputPanel,SIGNAL(hideKeyboard()),this,SLOT(hideKeyboard()));
}

//! [0]
//析构函数
InputPanelContext::~InputPanelContext()
{
    delete numberInputPanel;
    delete allInputPanel;
}

void InputPanelContext::hideKeyboard()
{
    numberInputPanel->hide();
    allInputPanel->hide();
    isShowKeyboard = false;
}

//唤醒和使用哪一个输入键盘
void InputPanelContext::wakeUp(int whichInput)
{
    isShowKeyboard = true;
    inputMethod = whichInput;

   updatePosition();

    switch(inputMethod)
    {
        case NumInputMethod:
            allInputPanel->hide();
            numberInputPanel->show();
            numberInputPanel->raise();
            //numberInputPanel->activateWindow();
            break;

        case AllInputMethod:
            numberInputPanel->hide();
            allInputPanel->show();
            allInputPanel->raise();
            //allInputPanel->activateWindow();
            break;
    }

}

//! [1]


bool InputPanelContext::filterEvent(const QEvent* event)
{
    //发生请求打开软输入键盘的事件
    if (event->type() == QEvent::RequestSoftwareInputPanel)
    {
        if (isShowKeyboard == true)
        {

            updatePosition();

            switch(inputMethod)
            {
                case NumInputMethod:
                    allInputPanel->hide();
                    numberInputPanel->setText();
                    numberInputPanel->show();
                    numberInputPanel->raise();
                    //numberInputPanel->activateWindow();
                    break;

                case AllInputMethod:
                    numberInputPanel->hide();
                    allInputPanel->show();
                    allInputPanel->raise();
                    //allInputPanel->activateWindow();
                    break;
            }

        }
        return true;
    }//发生关闭软输入键盘时
    else if (event->type() == QEvent::CloseSoftwareInputPanel)
    {

        numberInputPanel->hide();
        allInputPanel->hide();

        return true;
    }

    return false;
}

//! [1]

QString InputPanelContext::identifierName()
{
    return "MyInputPanelContext";
}

void InputPanelContext::reset()
{
}

bool InputPanelContext::isComposing() const
{
    return false;
}

QString InputPanelContext::language()
{
    return "en_US";
}

//! [2]

void InputPanelContext::sendCharacter(int keyId,
                                        Qt::KeyboardModifiers Modifier,
                                        QString character,
                                        int involvedKeys)
{
    //建立一个QWidget指针，QPointer是一个模版类
    QPointer<QWidget> w;
    //获取输入窗口的指针
    //QWidget *widget = focusWidget();

    switch (inputMethod)
    {
        case NumInputMethod:
            w = numberInputPanel->getLineEdit();//指向输入键盘的编辑框
            break;

        case AllInputMethod:
            w = allInputPanel->getFocusWidget();
            break;
    }

    if (!w)
        return;

    //发送按下、释放事件，是为了让光标右移
    QKeyEvent keyPress(QEvent::KeyPress, keyId,Modifier ,character,false,involvedKeys);
    QApplication::sendEvent(w, &keyPress);

    if (!w)
        return;

    //QKeyEvent keyRelease(QEvent::KeyPress, keyId, Qt::NoModifier, QString());
    //QApplication::sendEvent(w, &keyRelease);
}

//! [2]

//! [3]

void InputPanelContext::updatePosition()
{
    QWidget *widget;

    switch(inputMethod)
    {
          case AllInputMethod:
            widget = allInputPanel->getFocusWidget();
            break;

          case NumInputMethod:
            widget = numberInputPanel->getFocusWidget();
            break;

          default:
            widget = 0;
    }

    if (!widget)
        return;

    QRect widgetRect = widget->rect();
    QPoint panelPos = QPoint(widgetRect.left(), widgetRect.bottom() + 2);
    panelPos = widget->mapToGlobal(panelPos);

    if ( inputMethod == AllInputMethod )
    {
        allInputPanel->move(panelPos);
    }

    if ( inputMethod == NumInputMethod)
    {
        QPoint offsetPoint;

        offsetPoint = numberInputPanel->getFocusWidget()->topLevelWidget()->pos();
        numberInputPanel->move(pos_point + offsetPoint);
    }
}

void InputPanelContext::setPosition(int x,int y)
{
    QPoint point(x,y);
    pos_point = point;
}

void InputPanelContext::setInputMethod(int inputMethod)
{
    InputPanelContext::inputMethod = inputMethod;
}

//watch：具体对应要输入的编辑框指针
//whichnput：指示使用纯数字或带字母键盘
//InputWidgetType：输入编辑框类型LineEdit或TextEdit
//handler：通过KeyetReady这个基类的指针指向使用软键盘的窗口页面

//nameDisplay： //设置在软键盘显示的名称
//isCheck：是否根据设置的参数进行最大最小的检查，默认false
//CheckWidgetType：输入的数字类型，int或float型

void InputPanelContext::appendWidget(QWidget *watch,
                                       int whichInput,
                                       inputWidgetType InputWidgetType,
                                       KeySetReady *handler,
                                       float checkMinValue,
                                       float checkMaxValue,
                                       float Step,                                       
                                       QString nameDisplay,//显示在小键盘，可转换语言
                                       QString name,        //代码中的标识，不作语言转换
                                       bool isCheck ,
                                       checkWidgetType CheckWidgetType)
{

//    if(whichInput==AllInputMethod)
          allInputPanel->appendWidget(watch,whichInput);
//    else
 //   {
        numberInputPanel->appendWidget(watch,
                                   whichInput,
                                   InputWidgetType,
                                   handler,
                                   checkMinValue,
                                   checkMaxValue,
                                   Step,
                                   nameDisplay,
                                   name,
                                   isCheck,
                                   CheckWidgetType);
//    }
}

void InputPanelContext::removeWidget(QWidget *watch)
{
    numberInputPanel->removeWidget(watch);
    allInputPanel->removeWidget(watch);
}


QPoint InputPanelContext::pos_point(0,0);
int InputPanelContext::inputMethod = NumInputMethod;
bool InputPanelContext::isShowKeyboard = false;

