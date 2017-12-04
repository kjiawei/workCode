#ifndef INS_KEY_H
#define INS_KEY_H

#include <QtGui>

class INS_KEY : public QObject
{
    Q_OBJECT

public :
    INS_KEY(QObject * parent = 0);
    void lineEditNotInsert(QLineEdit *widget);

private slots:
    void lineEditSelectText(QWidget *widget);

private:
    QVector <QLineEdit *> widgetList;
    QSignalMapper signalMapper;
    int countWidget;
};

#endif
