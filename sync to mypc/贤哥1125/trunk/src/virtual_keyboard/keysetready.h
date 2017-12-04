#ifndef KEYSETREADY_H
#define KEYSETREADY_H

#include <QDebug>

class KeySetReady
{
public:
    KeySetReady();

    virtual void lineEdit_init() = 0;
    virtual void lineEdit_close(const char* name) = 0;

    virtual void lineEdit_data_set(const char* name , const char* data) = 0;

    void setParentName(QString parentName) {
        this->className = parentName;
    }

    void lineEdit_start(const char* name, float* max, float* min) {
        name = name;
        max = max;
        min = min;
    }

    //设置
    void setRange(float max, float min)
    {
        max = max;
        min = min;
    }

    QString getParentName(){
        return className;
    }

private:
    QString className;
};

#endif // KEYSETREADY_H
