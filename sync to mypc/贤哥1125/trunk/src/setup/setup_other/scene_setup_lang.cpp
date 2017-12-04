#include "config_ui.h"
#include "xml_parse.h"
#include "scene_setup_layout.h"
#include "scene_setup_lang.h"

inline void SceneSetupLang::setUi()
{
    buttonSave.setGeometry(QRect(620, 500, 64, 39));
    buttonCancel.setGeometry(QRect(694, 500, 64, 39));

    BUTTON_SIMPLE_STYLE(buttonSave);
    BUTTON_SIMPLE_STYLE(buttonCancel);

    loadLanguageXml();
    CustomSetText();

    connect(&buttonSave,SIGNAL(clicked()),this,SLOT(switchLanguage()));
    connect(&buttonCancel,SIGNAL(clicked()),this,SLOT(cancel()));
    connect(&groupLanguageType,SIGNAL(buttonClicked(int)),this,SLOT(switchLanguage()));
}

SceneSetupLang::SceneSetupLang(QWidget *parent) :
    QObject(parent),    
    buttonSave(parent),
    buttonCancel(parent),
    groupLanguageType(parent)
{
    pParent = parent;
    languageString = "";
    dataNodeAddObject(UPLANGUAGEXML, this);
    setUi();
    PageAppendPiece("Setup Lang", this);

}

void SceneSetupLang::CustomXmlConfig(const char *file, const char *cmd)
{
#if DEBUG_XML_CONFIG
    Debug_insert("SceneSetupLang-CustomXmlConfig-in");
#endif
    QMap<QString, QString> mapV;
    QDomDocument doc;

    xml_DocReadOpen(&doc, file);
    if (strcmp(cmd, DEFAULT_XML_LOAD) == 0) {
        if ((xml_NodeReadValue(&doc, (char *)"/Config/Language", &mapV) ==0) &&
                (mapV.isEmpty() == false)) {

            languageString = mapV["current"];
            if (languageString=="Chinese") {
                language=Chinese;
            }else if(languageString=="English"){
                language=English;
            }
            loadDefaultLanguage(mapV["current"],mapV["file"]);
        }
    }
    mapV.clear();
#if DEBUG_XML_CONFIG
    Debug_insert("SceneSetupLang-CustomXmlConfig-out");
#endif
}

void SceneSetupLang::loadLanguageXml()
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

    int j = 0;

    if (!listButton.isEmpty())
    {
        QList <QRadioButton *>::const_iterator it;

        for (it= listButton.constBegin(); it != listButton.constEnd(); it++) {
            delete (*it);
        }

        listButton.clear();
    }

    languageFileList.clear();

    while(!n.isNull()) {
        QDomElement e = n.toElement();

        if (e.attribute("operate") == QString("true"))
        {
            pButton = new QRadioButton(pParent);

            pButton->setStyleSheet("QRadioButton {color:#dce6f0;}"     \
                        "QRadioButton::indicator {width: 29px;height: 31px;} "  \
                        "QRadioButton::indicator::checked {image: url(:/resource/images/radio_button_check.png);} "  \
                        "QRadioButton::indicator::unchecked {image: url(:/resource/images/radio_button_uncheck.png);}");

            pButton->setText(e.attribute("language"));
            pButton->setCheckable(true);
            pButton->setGeometry(200,(j*50) + 200,200,50);
            pButton->setParent(pParent);
            listButton.append(pButton);
            groupLanguageType.addButton(pButton);
            languageFileList.insert(e.attribute("language"),e.attribute("file"));
            j++;
        }

        n = n.nextSibling();
    }

    file.close();
}

void SceneSetupLang::loadDefaultLanguage(QString language, QString file)
{
    if (!listButton.isEmpty())
    {
        QList <QRadioButton *>::const_iterator it;

        for (it= listButton.begin(); it != listButton.end(); it++) {
            pButton = (*it);

            if (pButton->text() == language)
            {
                pButton->setChecked(true);
                break;
            }
        }
    }

    if (!qtTranslator.isEmpty())
    {
        qApp->removeTranslator(&qtTranslator);
    }

    qtTranslator.load(file);
    qApp->installTranslator(&qtTranslator);
    enginerDialog->ui_translate();//deEE.retranslateUi();
    ReUpdateLangText();
}

#include "lang_switch.h"
void SceneSetupLang::switchLanguage()
{

    if (!qtTranslator.isEmpty())
    {
        qApp->removeTranslator(&qtTranslator);
    }

    QList <QRadioButton *>::const_iterator it;

    for (it= listButton.begin(); it != listButton.end(); it++) {
        pButton = (*it);

        if (pButton->isChecked())
        {
            languageString = pButton->text();
            if (languageString=="Chinese") {
                language=Chinese;

            }else if(languageString=="English"){
                language=English;

            }

            break;
        }
    }

    qtTranslator.load(languageFileList.value(pButton->text()));
    qApp->installTranslator(&qtTranslator);
    enginerDialog->ui_translate();
    QMap<QString, QString> mapV;

    mapV.insert("current", pButton->text());
    mapV.insert("file", languageFileList.value(pButton->text()));

    QDomDocument doc;
    xml_DocReadOpen(&doc, "conf/config.xml");
    xml_NodeWriteValue(&doc, "/Config/Language", &mapV);
    waiteToFile(&doc, "conf/config.xml");

    mapV.clear();

    ReUpdateLangText();
}

void SceneSetupLang::cancel()
{
    if (!listButton.isEmpty())
    {
        QList <QRadioButton *>::const_iterator it;

        for (it= listButton.begin(); it != listButton.end(); it++) {
            pButton = (*it);

            if (pButton->text() == languageString)
            {
                pButton->setChecked(true);
                break;
            }
        }
    }
}

void SceneSetupLang::CustomSetText()
{
    buttonSave.setText(tr("Confirm"));
    buttonCancel.setText(tr("Cancel"));
}

void SceneSetupLang::setCusVisible(bool vis, char* para)
{
    para = para;

    buttonSave.setVisible(vis);
    buttonCancel.setVisible(vis);

    if (!listButton.isEmpty())
    {
        QList <QRadioButton *>::const_iterator it;

        for (it= listButton.begin(); it != listButton.end(); it++) {
            pButton = (*it);
            pButton->setVisible(vis);
        }
    }
}

SceneSetupLang::~SceneSetupLang()
{
    if (!listButton.isEmpty())
    {
        QList <QRadioButton *>::const_iterator it;

        for (it= listButton.constBegin(); it != listButton.constEnd(); it++) {
            delete (*it);
        }

        listButton.clear();
    }

    languageFileList.clear();
}

void SceneSetupLang::ActionHandle(const void *dataPtr, int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupLang-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name == 0)) {
        return;
    }

    if (name==UPLANGUAGEXML) {
        loadLanguageXml();
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupLang-ActionHandle-out");
#endif
}
