#include "config_ui.h"
#include "xml_config.h"
#include "scene_setup_layout.h"
#include "scene_setup_xmlconfig.h"
#include "scene_maintain.h"
#include "Widget_UsbStorage_Signal.h"
#include "common_tr_string.h"

inline void SceneSetupXmlconfig::setUi()
{
    SCENE_SETUP_CREATE_TABLE(labelLoad, 1);
    SCENE_SETUP_WIDGET_XYW(labelLoad, 150, 1, 100);
    SCENE_SETUP_WIDGET_XYW(buttonXmlPathLoad, 260, 1, 370);
    SCENE_SETUP_WIDGET_XYW(buttonXmlExeLoad, 650, 1, 100);

    SCENE_SETUP_CREATE_TABLE(labelSave, 2);
    SCENE_SETUP_WIDGET_XYW(labelSave, 150, 2, 100);
    SCENE_SETUP_WIDGET_XYW(buttonXmlPathSave, 260, 2, 370);
    SCENE_SETUP_WIDGET_XYW(buttonXmlExeSave, 650, 2, 100);

    buttonSave.setGeometry(QRect(620, 500, 64, 39));
    buttonCancel.setGeometry(QRect(694, 500, 64, 39));

    BUTTON_SIMPLE_STYLE(buttonXmlPathLoad);
    BUTTON_SIMPLE_STYLE(buttonXmlExeLoad);
    BUTTON_SIMPLE_STYLE(buttonXmlPathSave);
    BUTTON_SIMPLE_STYLE(buttonXmlExeSave);
    BUTTON_SIMPLE_STYLE(buttonSave);
    BUTTON_SIMPLE_STYLE(buttonCancel);

    connect(&buttonXmlPathLoad, SIGNAL(clicked()), this, SLOT(buttonXmlPathLoad_clicked()));
    connect(&buttonXmlExeLoad, SIGNAL(clicked()), this, SLOT(buttonXmlExeLoad_clicked()));
    connect(&buttonXmlPathSave, SIGNAL(clicked()), this, SLOT(buttonXmlPathSave_clicked()));
    connect(&buttonXmlExeSave, SIGNAL(clicked()), this, SLOT(buttonXmlExeSave_clicked()));

    CustomSetText();
    buttonXmlPathLoad.setText(DEFAULT_XML_FILE);
}

SceneSetupXmlconfig::SceneSetupXmlconfig(QWidget *parent) :
    QObject(parent),
    labelLoad(parent),
    buttonXmlPathLoad(parent),
    buttonXmlExeLoad(parent),
    labelSave(parent),
    buttonXmlPathSave(parent),
    buttonXmlExeSave(parent),
    buttonSave(parent),
    buttonCancel(parent)
{
    setUi();
    PageAppendPiece("Setup XmlConfig", this);
}

void SceneSetupXmlconfig::buttonXmlPathLoad_clicked()
{
    QString strPath = QFileDialog::getOpenFileName(NULL, tr("Open File"), "./",
            tr("Config files (*.xml)"));
    buttonXmlPathLoad.setText(QDir::current().relativeFilePath(strPath));
}

void SceneSetupXmlconfig::buttonXmlExeLoad_clicked()
{
    QString strPath = buttonXmlPathLoad.text();
    if (strPath.isEmpty()) {
        return;
    }
    ReUpdateXmlConfig(strPath.toLatin1().data());

    MESSAGE_PROMPT_BOX(DE_TR_PROMPT,tr("update ok!"));
}

void SceneSetupXmlconfig::buttonXmlPathSave_clicked()
{
//    APP_UsbStorage_Signal_Quit();
    if(SceneMaintain::isUsbStroageExist == false)
    {
        QString fileName = QFileDialog::getSaveFileName(NULL,
                    tr("Save File"), "./conf/config_bk.xml", tr("Config Files (*.xml)"));
        if ((fileName.isEmpty() != true) &&
                (QFileInfo(fileName).suffix() != "xml")) {
            fileName.append(".xml");
         }

        buttonXmlPathSave.setText(QDir::current().relativeFilePath(fileName));
    }

    else
    {
        QString fileName = QFileDialog::getSaveFileName(NULL,
                    tr("Save File"), "/mnt/usbStorage/config_bk.xml", tr("Config Files (*.xml)"));
        if ((fileName.isEmpty() != true) &&
                (QFileInfo(fileName).suffix() != "xml")) {
            fileName.append(".xml");
        }
        buttonXmlPathSave.setText(QDir::current().relativeFilePath(fileName));
        //us_sleep(1000000);
        //system("umount /mnt/usbStorage");

     }

}

void SceneSetupXmlconfig::buttonXmlExeSave_clicked()
{
    int rev;
    QString strPath = buttonXmlPathSave.text();
    if (strPath.isEmpty()) {
         MESSAGE_PROMPT_BOX(DE_TR_PROMPT,tr("save fail!"));
        return;
    }
//    APP_UsbStorage_Signal_Quit();
    if(SceneMaintain::isUsbStroageExist == false)
    {
        rev=system("cp /usr/local/app/conf/config.xml /usr/local/app/conf/bk_config.xml");
     }
    else {
        rev=system("cp /usr/local/app/conf/config.xml /mnt/usbStorage/config_bk.xml");
    }
    us_sleep(1000000);
    rev=system("sync");
    qDebug()<<"save ok!";
    MESSAGE_PROMPT_BOX(DE_TR_PROMPT,tr("save ok!"));
    //ReUpdateXmlConfig(strPath.toLatin1().data(), DEFAULT_XML_SAVE);
}

void SceneSetupXmlconfig::CustomSetText()
{
    labelLoad.setText(tr("Load File :"));
    labelSave.setText(tr("Save File:"));
    buttonXmlExeLoad.setText(tr("Load"));
    buttonXmlExeSave.setText(tr("Confirm"));
    buttonSave.setText(tr("Confirm"));
    buttonCancel.setText(tr("Cancel"));
}

void SceneSetupXmlconfig::setCusVisible(bool vis, char* para)
{
    para = para;
    labelLoad.setVisible(vis);
    buttonXmlPathLoad.setVisible(vis);
    buttonXmlExeLoad.setVisible(vis);
    labelSave.setVisible(vis);
    buttonXmlPathSave.setVisible(vis);
    buttonXmlExeSave.setVisible(vis);
    buttonSave.setVisible(false);
    buttonCancel.setVisible(false);
}
