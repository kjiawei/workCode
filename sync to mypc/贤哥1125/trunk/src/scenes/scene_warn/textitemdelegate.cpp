#include "textitemdelegate.h"

#include <QTextOption>

void TextItemDelegate::setType(char type )
{
    this->type = type;
}

void TextItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    if(this->type == 1) {
        QString value = index.model()->data(index, Qt::DisplayRole).toString();
        QTextOption myOption(Qt::AlignLeft);
        painter->drawText(option.rect, value, myOption);

    } else if(this->type == 0) {

        QStyledItemDelegate::paint(painter, option, index);
    }
    //QStyleOptionViewItem  viewOption(option);
    //高亮显示与普通显示时的前景色一致（即选中行和为选中时候的文字颜色一样）#/
    //viewOption.palette.setColor(QPalette::HighlightedText, index.data(Qt::ForegroundRole).value<QColor>());
}
