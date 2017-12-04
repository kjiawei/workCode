#include <QtGui/QApplication>
#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    dialog w;
    w.show();
    
    return a.exec();
}
