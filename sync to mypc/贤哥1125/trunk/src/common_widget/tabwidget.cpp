#include "tabwidget.h"

TabWidget::TabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    setTabBar(new TabBar);
}

TabBar::TabBar(QWidget *parent) :
    QTabBar(parent)
{

#ifdef MULIT_LINE
    setUsesScrollButtons(false);
    this->setMaximumHeight(this->height() * 3);
    this->setMinimumHeight(this->height() * 3);
#endif

}

void TabBar::paintEvent(QPaintEvent *)
{
#ifdef MULIT_LINE

    QStylePainter p(this);

    bool resized=false;
    int moveIt = 0;
    int nrRows = 1;
    tabRectangle.clear();

    for (int i=0; i< count(); i++) {
        QStyleOptionTabV3 tab;
        initStyleOption(&tab, i);

        if (tab.rect.right() > width()*nrRows) {
            if (!resized) {
                setGeometry(0, 0, width(), tab.rect.height()*(nrRows+1));
                resized=true;
            }
            tab.rect.moveTo(moveIt, tab.rect.height()*nrRows);
            if (tab.rect.right() > width()) {
                nrRows++;
                moveIt = 0;
                setGeometry(0, 0, width(), tab.rect.height()*(nrRows+1));
                tab.rect.moveTo(moveIt,tab.rect.height()*nrRows);
            }
            moveIt+=tab.rect.width();
        }
        p.drawControl(QStyle::CE_TabBarTab, tab);

        tabRectangle.append(tab.rect);
    }
#else

    QStylePainter p(this);
    for (int index = 0; index < count(); index++)
    {
        QStyleOptionTabV3 tab;
        initStyleOption(&tab, index);

        QIcon tempIcon = tab.icon;
        QString tempText = tab.text;
        tab.icon = QIcon();
        tab.text = QString();

        p.drawControl(QStyle::CE_TabBarTab, tab);

        QPainter painter;
        painter.begin(this);
        QRect tabrect = tabRect(index);
        tabrect.adjust(0, 8, 0, -8);
        painter.drawText(tabrect, /*Qt::AlignBottom |*/ Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextWrapAnywhere, tempText);
        tempIcon.paint(&painter, 0, tabrect.top(), tab.iconSize.width(), tab.iconSize.height(), Qt::AlignTop | Qt::AlignHCenter);
        painter.end();
    }

#endif
}

void TabBar::resizeEvent(QResizeEvent *event)
{
    QTabBar::resizeEvent(event);
    update();
}

#ifdef MULIT_LINE
void TabBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        event->ignore();
        return;
    }

    for (int i=0; i<count(); i++) {
        if (tabRectangle[i].contains(event->pos())) {
            setCurrentIndex(i);
        }
    }
}

#endif
