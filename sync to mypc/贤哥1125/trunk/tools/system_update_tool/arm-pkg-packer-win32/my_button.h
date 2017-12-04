
#ifndef MY_BUTTON_H
#define MY_BUTTON_H

#include <QPushButton>

class CMyButton : public QPushButton
{
    Q_OBJECT

public:
    CMyButton(QWidget * parent = 0);
    CMyButton(const QString & text, QWidget * parent = 0);
    CMyButton(const QIcon & icon, const QString & text, QWidget * parent = 0);
    void setId(int i);

private:
    int m_nId;

public slots:
    void Onclick();

signals:
    void ClickedEvent(int nId_);
};

#endif // MY_BUTTON_H
