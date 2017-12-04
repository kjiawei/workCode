#ifndef PASSWORD_H
#define PASSWORD_H

#include <QtGui>
#include "lang_switch.h"
#include "page_info.h"

class PASSWORD : public QDialog, public LangSetText//, public SceneSetup 
{
    Q_OBJECT

public:
    PASSWORD(QWidget * parent = 0, Qt::WindowFlags f = 0);
    void CustomSetText();
    QString getText();

private:
    inline void setUi();

private:
    QLabel pwdLabel;
    QLineEdit pwdLineEdit;
    QPushButton okButton;
    QPushButton cancelButton;
};


#endif // PASSWORD_H
