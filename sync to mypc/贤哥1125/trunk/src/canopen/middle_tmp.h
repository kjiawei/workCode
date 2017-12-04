#ifndef MIDDLE_TMP_H
#define MIDDLE_TMP_H

#include <QObject>
#include "interactive.h"

class MessageSignal : public QObject
{
    Q_OBJECT
public:
    MessageSignal(){};
    void MessageSignalApp(const Message *message=NULL) {
        emit DataComeSignal(*message);
    }
signals:
    void DataComeSignal(const Message & message);
};
#endif
