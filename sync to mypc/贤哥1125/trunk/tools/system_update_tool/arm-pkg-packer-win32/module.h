#ifndef MODULE_H
#define MODULE_H

#include <QCheckBox>
#include <QLineEdit>
#include "my_button.h"

struct TMP_MODULE
{
    QCheckBox * check;
    QLineEdit * edit;
    CMyButton * button;
    char tag[16];
};

#endif // MODULE_H
