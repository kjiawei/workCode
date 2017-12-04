#include "config_ui.h"
#include "scene_setup_layout.h"
#include "password.h"

PASSWORD::PASSWORD(QWidget *parent, Qt::WindowFlags f) :
    QDialog(parent,f),
    pwdLabel(this),
    pwdLineEdit(this),
    okButton(this),
    cancelButton(this)
{
    setUi();
}

inline void PASSWORD::setUi()
{
    pwdLabel.setGeometry(10,10,100,50);
    pwdLineEdit.setGeometry(120,10,100,50);
    okButton.setGeometry(10,80,100,50);
    cancelButton.setGeometry(120,80,100,50);

    BUTTON_SIMPLE_STYLE(okButton);
    BUTTON_SIMPLE_STYLE(cancelButton);
#if 0
    listWidgetAdd(&pwdLabel);
    listWidgetAdd(&pwdLineEdit);
    listWidgetAdd(&okButton);
    listWidgetAdd(&cancelButton);
#endif
    ic->appendWidget(&pwdLineEdit,AllInputMethod,LineEdit);

    CustomSetText();

    connect(&okButton,SIGNAL(clicked()),this,SLOT(accept()));
    connect(&cancelButton,SIGNAL(clicked()),this,SLOT(reject()));
}

void PASSWORD::CustomSetText()
{
    okButton.setText(tr("OK"));
    cancelButton.setText(tr("Cancel"));
    pwdLabel.setText(tr("password"));
}

QString PASSWORD::getText()
{
    return pwdLineEdit.text();
}

