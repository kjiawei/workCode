#ifndef COMMON_CUSTOMWIDGET_H
#define COMMON_CUSTOMWIDGET_H

#include <QLineEdit>

/*-------------------------自定义编辑框类-------------------------------*/
//自定义编辑框控件
class MyLineEdit:public QLineEdit
{
    Q_OBJECT

public:
    MyLineEdit(QWidget *parent=0);

signals:
    void InFocus();

protected:
    void focusInEvent(QFocusEvent *ev);//焦点选中
};

#endif // COMMON_CUSTOMWIDGET_H
