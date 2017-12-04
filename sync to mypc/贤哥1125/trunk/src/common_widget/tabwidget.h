#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>
#include <QTabBar>
#include <QtGui>

class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = 0);
};

class TabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit TabBar(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *event);

#ifdef MULIT_LINE
    void mousePressEvent(QMouseEvent *event);

private:
    QList<QRect> tabRectangle;
#else
    QSize tabSizeHint(int) const
    {
        return QSize(80,80);
    }
#endif

};

#endif // MYTABWIDGET_H
