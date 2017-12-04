#ifndef TEXTITEMDELEGATE_H
#define TEXTITEMDELEGATE_H

#include <QtGui>

class TextItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    TextItemDelegate(QObject *parent = 0): QStyledItemDelegate(parent)
    {
        this->type = 0;
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    char type;
    void setType(char type );
};

#endif // TEXTITEMDELEGATE_H
