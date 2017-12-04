#include "edittablewidgetitemdelege.h"
#include <QDebug>
#include "global_var.h"

EditTableWidgetItemDelege::EditTableWidgetItemDelege(QObject *parent)
    : QItemDelegate(parent)
{
    connect(this, SIGNAL(sendKeyListener(QLineEdit*, int, int)), this, SLOT(addKeyListener(QLineEdit*, int, int)));
    setParentName("EditTableWidgetItemDelege");
}

void EditTableWidgetItemDelege::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
   QString value = index.model()->data(index, Qt::DisplayRole).toString();


   QTextOption myOption(Qt::AlignCenter);
   painter->drawText(option.rect, value, myOption);
   //QItemDelegate::paint(painter, option, index);
}

QWidget *EditTableWidgetItemDelege::createEditor(QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    if(index.column() < 6 && index.column() > 0) {
        QLineEdit *edit = new QLineEdit(parent);
        emit sendKeyListener(edit, index.row(), index.column());
        return edit;
    } else
        return QItemDelegate::createEditor(parent, option, index);
}

void EditTableWidgetItemDelege::addKeyListener(QLineEdit *edit, int row, int column)
{
    this->currentEdit = edit;
    this->row = row;
    this->column = column;
#ifdef HW_ARM
    ic->appendWidget(edit, NumInputMethod, LineEdit, this);
#endif
}

void EditTableWidgetItemDelege::lineEdit_data_set(const char *name, const char *data)
{
    name = name;
    emit tableWidgetEditListener(row, column, data);
}

void EditTableWidgetItemDelege::lineEdit_close(const char* name)
{
    Q_UNUSED(name);
//    printf("\r trying ! \n ");
    emit commitData(currentEdit);
    emit closeEditor(currentEdit);
    delete this->currentEdit;

}

void EditTableWidgetItemDelege::commitAndCloseEditor()
{

    QLineEdit *editor =qobject_cast<QLineEdit *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
    //delete this->currentEdit;
    //currentEdit=NULL;
}
