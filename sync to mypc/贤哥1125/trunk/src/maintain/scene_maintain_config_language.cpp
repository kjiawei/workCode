#include "scene_maintain_config_language.h"
#include "data_custom.h"
#include <QDebug>
#include <QtXml>

SceneMaintainConfigLanguage::SceneMaintainConfigLanguage(QWidget *parent, Qt::WindowFlags f):
    QWidget(parent,f)
{
    pParentWidget = parent;
    numberNode = 0;
    pButton = NULL;
    setUI(parent);
    connect(&saveButton,SIGNAL(clicked()),this,SLOT(saveXml()));
    connect(&cancelButton,SIGNAL(clicked()),this,SLOT(cancelXml()));
}

void SceneMaintainConfigLanguage::setUI(QWidget *parent)
{
    saveButton.setText(tr("Confirm"));
    saveButton.setParent(parent);
    saveButton.setGeometry(400,0 + 200,100,50);
    cancelButton.setText(tr("cancel"));
    cancelButton.setParent(parent);
    cancelButton.setGeometry(400,50 + 200,100,50);

    PageAppendPiece("Maintain Language", this);
    loadXml(parent);

    int i = 0;
    while(i < numberNode)
    {
        pButton[i].setVisible(false);
        i++;
    }
}

void SceneMaintainConfigLanguage::CustomSetText()
{
    saveButton.setText(tr("Confirm"));
    cancelButton.setText(tr("cancel"));
}

void SceneMaintainConfigLanguage::setCusVisible(bool vis, char *para)
{
    para = para;

    int i = 0;
    while(i < numberNode)
    {
        pButton[i].setVisible(vis);
        i++;
    }

    saveButton.setVisible(vis);
    cancelButton.setVisible(vis);
}

void SceneMaintainConfigLanguage::loadXml(QWidget * parent)
{
    QDomDocument doc("test");
    QFile file("resource/languages/language.xml");

    if (!file.open(QIODevice::ReadOnly))
        return ;

    if (!doc.setContent(&file)){
        file.close();
        return ;
    }

    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();

    numberNode = docElem.childNodes().size();
    int i = 0;

    if (pButton == NULL)
    {
        pButton = new QCheckBox[numberNode];
    }

    while(!n.isNull()) {
        QDomElement e = n.toElement();

        pButton[i].setParent(parent);
        pButton[i].setText(e.attribute("language"));

        pButton[i].setCheckable(true);

        if (e.attribute("operate") == QString("true"))
        {
            pButton[i].setChecked(true);
            pButton[i].setGeometry(200,(i*50) + 200,200,50);
        }
        else
        {
            pButton[i].setChecked(false);
            pButton[i].setGeometry(200,(i*50) + 200,200,50);
        }

        n = n.nextSibling();
        i++;
    }

    file.close();
}

void SceneMaintainConfigLanguage::saveXml()
{
    QDomDocument doc("test");
    QFile fileRead("resource/languages/language.xml");

    if (!fileRead.open(QIODevice::ReadWrite))
        return ;

    if (!doc.setContent(&fileRead)){
        fileRead.close();
        return ;
    }

    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();

    int i = 0;

    while(!n.isNull()) {
        QDomElement e = n.toElement();

        if (pButton[i].isChecked())
        {
            e.setAttribute("operate","true");
        }
        else
        {
            e.setAttribute("operate","false");
        }

        n = n.nextSibling();
        i++;
    }
    fileRead.close();

    QFile fileWrite("resource/languages/language.xml");

    if(!fileWrite.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return ;  ///
    }

    QTextStream out(&fileWrite);
    doc.save(out,4);
    fileWrite.close();

    dataNodeUpdate(UPLANGUAGEXML,&sendSignal);
}

void SceneMaintainConfigLanguage::cancelXml()
{
    loadXml(pParentWidget);
}

SceneMaintainConfigLanguage::~SceneMaintainConfigLanguage()
{
    if (pButton != NULL)
    {
        delete [] pButton;
        pButton = NULL;
    }
}
