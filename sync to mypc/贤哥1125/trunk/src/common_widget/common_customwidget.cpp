#include "common_customwidget.h"


/*-------------------------自定义编辑框类-------------------------------*/

MyLineEdit::MyLineEdit(QWidget *parent):QLineEdit(parent){}

void MyLineEdit::focusInEvent(QFocusEvent *ev)
{
    emit InFocus();
    QLineEdit::focusInEvent(ev);
}
