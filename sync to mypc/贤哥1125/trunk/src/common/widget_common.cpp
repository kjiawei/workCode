#include <QApplication>
#include "config_ui.h"
#include "global_var.h"
#include "widget_common.h"

void MainQuit(int sig)
{
    sig = sig;
    /*
     *
    if (dataManage != NULL) {
        delete dataManage;
        dataManage = NULL;
    }
    if (strlen(pageManage.PageCurrent()) != 0) {
        pageManage.~PageManage();
    }
    timer->closeTimer();
    QApplication::closeAllWindows();
    _exit(0);

    */
}


void WindowBottom::mousePressEvent(QMouseEvent *event)
{
    //qDebug() << "\r\n mousePressEvent! \r\n";
    event->ignore();
    return;
    if (event->button() == Qt::LeftButton) {
        qDebug() << "left";
    } else {
        ;//QPushButton::mousePressEvent(event);
    }
}


