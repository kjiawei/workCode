#ifndef SCENE_SETUP_LANG_H
#define SCENE_SETUP_LANG_H

#include <QtGui>
#include <QLabel>
#include "lang_switch.h"
#include "xml_config.h"
#include "config_xml.h"
#include "page_info.h"
#include "data_custom.h"

class SceneSetupLang : public QObject, public PieceVisible, public LangSetText,
    public XmlConfig,public ActionCustom
{
    Q_OBJECT
public:
    SceneSetupLang(QWidget *parent=0);
    ~SceneSetupLang();
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);
    void CustomXmlConfig(const char *file=DEFAULT_XML_FILE, const char *cmd=DEFAULT_XML_LOAD);

private:
    inline void setUi();
    void loadDefaultLanguage(QString language, QString file);
    void loadLanguageXml();
    void ActionHandle(const void* dataPtr=NULL, int name=0);

private slots:
    void switchLanguage();
    void cancel();

private:
    QPushButton buttonSave;
    QPushButton buttonCancel;
    QRadioButton * pButton;
    QList <QRadioButton *> listButton;
    QButtonGroup groupLanguageType;
    //first arg is language name , second arg is file path
    QHash <QString,QString> languageFileList;
    QTranslator qtTranslator;
    QString languageString;
    QWidget *pParent;
};

#endif
