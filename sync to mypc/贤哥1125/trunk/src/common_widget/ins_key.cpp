#include "ins_key.h"

INS_KEY::INS_KEY(QObject * parent):QObject(parent)
{
    countWidget = -1;
}

void  INS_KEY::lineEditNotInsert(QLineEdit *widget)
{
    countWidget++;

    widgetList.append(widget);

    connect(widgetList.at(countWidget), SIGNAL(cursorPositionChanged(int,int)),
            &signalMapper, SLOT(map()));

    connect(widgetList.at(countWidget),SIGNAL(textChanged(QString)),
            &signalMapper, SLOT(map()));

    signalMapper.setMapping(widgetList.at(countWidget), widget);

    connect(&signalMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(lineEditSelectText(QWidget*)));
}

void INS_KEY::lineEditSelectText(QWidget *widget)
{
    QLineEdit *lineEdit = (QLineEdit *)widget;

    int pos;

    if (lineEdit->hasSelectedText())
    {
        return;
    }
    else
    {
        pos =  lineEdit->cursorPosition();
        lineEdit->setSelection(pos,1);
    }
}
