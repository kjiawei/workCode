#ifndef SCENE_SETUP_NETWORK_H
#define SCENE_SETUP_NETWORK_H

#include <QtGui>
#include <QLabel>
#include "lang_switch.h"
#include "page_info.h"

#ifdef OS_LINUX
#include "wlan.h"
#include "widget_common.h"
#include "keysetready.h"
#endif


#define NETWORK_CONFIG_FILE "/usr/local/sbin/network.sh"
//#define NETWORK_CONFIG_FILE "/home/sky/nfs/home/skyboard/networkTest.sh"

#define SCENE_SETUP_NETWORK_TABLE(x)           \
    do {                                        \
        (x).setAlignment( Qt::AlignVCenter | Qt::AlignRight); \
    (x).setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
        (x).setStyleSheet(CONFIG_COLOR_LABEL);                \
    } while(0)
#define SCENE_SETUP_NETWORK_FORM(x)           \
    do {                                        \
        (x).setAlignment( Qt::AlignVCenter | Qt::AlignLeft); \
    (x).setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
        (x).setStyleSheet(CONFIG_COLOR_LABEL);                \
    } while(0)
#define SCENE_SETUP_NETWORK_EDIT(x)           \
    do {                                        \
        (x).setAlignment( Qt::AlignCenter); \
   /* (x).setFont(QFont(QString(CONFIG_FONT_STYLE),    */     \
               /* CFG_FONT_SIZE_FOURTH, CONFIG_FONT_NORMAL)); */\
        (x).setStyleSheet(CONFIG_COLOR_BLUE_CHARS);                \
    } while(0)

class SceneSetupNetwork : public QObject, public PieceVisible, public LangSetText, public KeySetReady
{
    Q_OBJECT
public:
    SceneSetupNetwork(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);
    void ActionHandle(const void* dataPtr=NULL, const char* name=NULL){dataPtr=dataPtr;name=name;}
    int execute(char *arglist[]);


    void lineEdit_init(){}
    void lineEdit_data_set(const char *name, const char *data){name=name;data=data;}
    void lineEdit_close(const char *name){name=name;}
private:
    inline void setUi();

private:
    QPushButton buttonSave;
    QPushButton buttonCancel;
    QPushButton buttonSetup;
private:
    QLabel networkLabel;
    QLabel ipLabel;
    QLabel maskLabel;
    QLabel gatewayLabel;
    QLabel dnsLabel;
    QLineEdit ipLineEdit;
    QLineEdit maskLineEdit;
    QLineEdit gatewayLineEdit;
    QLineEdit dnsLineEdit;
    QLineEdit lineEditIp1st;
    QLineEdit lineEditIp2nd;
    QLineEdit lineEditIp3rd;
    QLineEdit lineEditIp4th;
    QLineEdit lineEditMask1st;
    QLineEdit lineEditMask2nd;
    QLineEdit lineEditMask3rd;
    QLineEdit lineEditMask4th;
    QLineEdit lineEditGateway1st;
    QLineEdit lineEditGateway2nd;
    QLineEdit lineEditGateway3rd;
    QLineEdit lineEditGateway4th;
    QLineEdit lineEditDns1st;
    QLineEdit lineEditDns2nd;
    QLineEdit lineEditDns3rd;
    QLineEdit lineEditDns4th;

#ifdef OS_LINUX
private:
    QPushButton buttonInitWlan;
    CWlan wlanScanDialog;


private slots:
    void initWlan();
    bool validatePage();
#endif
};

#endif
