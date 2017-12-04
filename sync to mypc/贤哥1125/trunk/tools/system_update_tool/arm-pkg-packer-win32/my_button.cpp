#include "my_button.h"

CMyButton::CMyButton(QWidget * parent) : QPushButton(parent)
{
    connect(this, SIGNAL(clicked()), this, SLOT(Onclick()));
}

CMyButton::CMyButton(const QString & text, QWidget * parent):QPushButton(text,parent)
{
    connect(this, SIGNAL(clicked()), this, SLOT(Onclick()));
}

CMyButton::CMyButton(const QIcon & icon, const QString & text, QWidget * parent):QPushButton(icon,text,parent)
{
    connect(this, SIGNAL(clicked()), this, SLOT(Onclick()));
}

void CMyButton::Onclick()
{
    emit ClickedEvent(m_nId);
}

void CMyButton::setId(int i)
{
    m_nId = i;
}
