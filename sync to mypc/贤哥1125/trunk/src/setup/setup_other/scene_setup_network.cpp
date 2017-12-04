#include "config_ui.h"
#include "scene_setup_layout.h"
#include "scene_setup_network.h"
#include "global_var.h"
 #include <sys/wait.h>

#define Run(al0, al1, al2, al3, al4,al5,al6,al7) {char *arglist[9]; \
                                          arglist[0]=al0;  arglist[1]=al1; arglist[2]=al2; arglist[3]=al3; \
                                          arglist[4]=al4; arglist[5]=al5; arglist[6]=al6;arglist[7]=al7; \
                                          arglist[8]=NULL; \
                                          if(execute(arglist)) return 1;}
inline void SceneSetupNetwork::setUi()
{
#define NETWORK_ROW 10
    buttonSave.setGeometry(QRect(620, 430, 64, 39));
    buttonCancel.setGeometry(QRect(694, 430, 64, 39));

    buttonSetup.setGeometry(QRect(600, 600, 64, 39));
    networkLabel.setGeometry(QRect(220, 540-40, 200, 39));
    SCENE_SETUP_NETWORK_TABLE(networkLabel);
    SCENE_SETUP_NETWORK_TABLE(ipLabel);
    ipLabel.setGeometry(QRect(220, 540, 100, 39));
    SCENE_SETUP_CREATE_LINEEDIT(lineEditIp1st,NETWORK_ROW);
    lineEditIp1st.setGeometry(QRect(340, 540+8, 60, 30));
    SCENE_SETUP_CREATE_LINEEDIT(lineEditIp2nd,NETWORK_ROW);
    lineEditIp2nd.setGeometry(QRect(380+20, 540+8, 60, 30));
    SCENE_SETUP_CREATE_LINEEDIT(lineEditIp3rd,NETWORK_ROW);
    lineEditIp3rd.setGeometry(QRect(420+40, 540+8, 60, 30));
    SCENE_SETUP_CREATE_LINEEDIT(lineEditIp4th,NETWORK_ROW);
    lineEditIp4th.setGeometry(QRect(460+60, 540+8, 60, 30));


    maskLabel.setGeometry(QRect(220, 540+40, 100, 39));
    SCENE_SETUP_NETWORK_TABLE(maskLabel);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditMask1st,NETWORK_ROW+1);
    lineEditMask1st.setGeometry(QRect(340, 540+48, 60, 30));
    SCENE_SETUP_CREATE_LINEEDIT(lineEditMask2nd,NETWORK_ROW+1);
    lineEditMask2nd.setGeometry(QRect(380+20, 540+48, 60, 30));
    SCENE_SETUP_CREATE_LINEEDIT(lineEditMask3rd,NETWORK_ROW+1);
    lineEditMask3rd.setGeometry(QRect(420+40, 540+48, 60, 30));
    SCENE_SETUP_CREATE_LINEEDIT(lineEditMask4th,NETWORK_ROW+1);
    lineEditMask4th.setGeometry(QRect(460+60, 540+48, 60, 30));


    gatewayLabel.setGeometry(QRect(220, 540+80, 100, 39));
    SCENE_SETUP_NETWORK_TABLE(gatewayLabel);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditGateway1st,NETWORK_ROW+2);
    lineEditGateway1st.setGeometry(QRect(340, 540+88, 60, 30));
    SCENE_SETUP_CREATE_LINEEDIT(lineEditGateway2nd,NETWORK_ROW+2);
    lineEditGateway2nd.setGeometry(QRect(380+20, 540+88, 60, 30));
    SCENE_SETUP_CREATE_LINEEDIT(lineEditGateway3rd,NETWORK_ROW+2);
    lineEditGateway3rd.setGeometry(QRect(420+40, 540+88, 60, 30));
    SCENE_SETUP_CREATE_LINEEDIT(lineEditGateway4th,NETWORK_ROW+2);
    lineEditGateway4th.setGeometry(QRect(460+60, 540+88, 60, 30));


    dnsLabel.setGeometry(QRect(220, 540+120, 100, 39));
    SCENE_SETUP_NETWORK_TABLE(dnsLabel);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditDns1st,NETWORK_ROW+3);
    lineEditDns1st.setGeometry(QRect(340, 540+128, 60, 30));
    SCENE_SETUP_CREATE_LINEEDIT(lineEditDns2nd,NETWORK_ROW+3);
    lineEditDns2nd.setGeometry(QRect(380+20, 540+128, 60, 30));
    SCENE_SETUP_CREATE_LINEEDIT(lineEditDns3rd,NETWORK_ROW+3);
    lineEditDns3rd.setGeometry(QRect(420+40, 540+128, 60, 30));
    SCENE_SETUP_CREATE_LINEEDIT(lineEditDns4th,NETWORK_ROW+3);
    lineEditDns4th.setGeometry(QRect(460+60, 540+128, 60, 30));

#ifdef OS_LINUX
    buttonInitWlan.setGeometry(QRect(420, 430, 100, 39));
    wlanScanDialog.setGeometry(QRect(200, 70, 550, 300));
#endif

    BUTTON_SIMPLE_STYLE(buttonSave);
    BUTTON_SIMPLE_STYLE(buttonCancel);
    BUTTON_SIMPLE_STYLE(buttonSetup);

#ifdef OS_LINUX
    BUTTON_SIMPLE_STYLE(buttonInitWlan);
#endif

#ifdef OS_LINUX
    connect(&buttonInitWlan,SIGNAL(clicked()),this,SLOT(initWlan()));
    connect(&buttonSetup,SIGNAL(clicked()),this,SLOT(validatePage()));
#endif

    CustomSetText();
    ic->appendWidget(&lineEditIp1st, NumInputMethod, LineEdit, this,1,255,1,tr("IP:"));
    ic->appendWidget(&lineEditIp2nd, NumInputMethod, LineEdit, this,1,255,1,tr("IP:"));
    ic->appendWidget(&lineEditIp3rd, NumInputMethod, LineEdit, this,1,255,1,tr("IP:"));
    ic->appendWidget(&lineEditIp4th, NumInputMethod, LineEdit, this,1,255,1,tr("IP:"));

    ic->appendWidget(&lineEditMask1st, NumInputMethod, LineEdit, this,1,255,1,tr("Mask:"));
    ic->appendWidget(&lineEditMask2nd, NumInputMethod, LineEdit, this,1,255,1,tr("Mask:"));
    ic->appendWidget(&lineEditMask3rd, NumInputMethod, LineEdit, this,1,255,1,tr("Mask:"));
    ic->appendWidget(&lineEditMask4th, NumInputMethod, LineEdit, this,1,255,1,tr("Mask:"));

    ic->appendWidget(&lineEditGateway1st, NumInputMethod, LineEdit, this,1,255,1,tr("Gateway:"));
    ic->appendWidget(&lineEditGateway2nd, NumInputMethod, LineEdit, this,1,255,1,tr("Gateway:"));
    ic->appendWidget(&lineEditGateway3rd, NumInputMethod, LineEdit, this,1,255,1,tr("Gateway:"));
    ic->appendWidget(&lineEditGateway4th, NumInputMethod, LineEdit, this,1,255,1,tr("Gateway:"));

    ic->appendWidget(&lineEditDns1st, NumInputMethod, LineEdit, this,1,255,1,tr("Dns:"));
    ic->appendWidget(&lineEditDns2nd, NumInputMethod, LineEdit, this,1,255,1,tr("Dns:"));
    ic->appendWidget(&lineEditDns3rd, NumInputMethod, LineEdit, this,1,255,1,tr("Dns:"));
    ic->appendWidget(&lineEditDns4th, NumInputMethod, LineEdit, this,1,255,1,tr("Dns:"));
}

SceneSetupNetwork::SceneSetupNetwork(QWidget *parent) :
    QObject(parent),
    buttonSave(parent),
    buttonCancel(parent),
    buttonSetup(parent),


    networkLabel(parent),
    ipLabel(parent),
    maskLabel(parent),
    gatewayLabel(parent),
    dnsLabel(parent),

    lineEditIp1st(parent),
    lineEditIp2nd(parent),
    lineEditIp3rd(parent),
    lineEditIp4th(parent),
    lineEditMask1st(parent),
    lineEditMask2nd(parent),
    lineEditMask3rd(parent),
    lineEditMask4th(parent),
    lineEditGateway1st(parent),
    lineEditGateway2nd(parent),
    lineEditGateway3rd(parent),
    lineEditGateway4th(parent),
    lineEditDns1st(parent),
    lineEditDns2nd(parent),
    lineEditDns3rd(parent),
    lineEditDns4th(parent)
#ifdef OS_LINUX
    ,buttonInitWlan(parent)
    ,wlanScanDialog(parent)

#endif
{
    setUi();
    PageAppendPiece("Setup Network", this);
}

#ifdef OS_LINUX
void  SceneSetupNetwork::initWlan()
{
    //run wpa_supplicant
    QProcess processWpa;
    QString programWpa = "wpa_supplicant";
    QStringList argumentsWpa;
    argumentsWpa << "-i" << "wlan0"
              << "-D" << "wext"
              << "-c" << "/usr/local/etc/wpa_supplicant.conf"
              << "-B" ;
    processWpa.start(programWpa,argumentsWpa);

    //wpa_supplicant is finish??
    if (processWpa.waitForFinished())
    {
        qDebug() << "finish the wpa tool";
    }
    else
    {
        qDebug() << "can not finish the wpa tool";
        return ;
    }

    //run udhcpc
    QProcess processDHCP;
    QString programDHCP = "udhcpc";
    QStringList argumentsDHCP;
    argumentsDHCP << "-b"
                  << "-i" << "wlan0"
                  << "-p" << "/var/run/udhcpc.pid";
    processDHCP.start(programDHCP,argumentsDHCP);

    //udhcpc is finish??
    if (processDHCP.waitForFinished())
    {
        qDebug() << "finish the udhcpc tool";
    }
    else
    {
        qDebug() << "can not finish the udhcpc tool";
        return ;
    }

    //run wpa_gui
    wlanScanDialog.initWlan();
}
int SceneSetupNetwork::execute(char *arglist[])
{
      int pid, exitstatus=0,temp=0;

      qDebug()<<"arglist"<<arglist[0]<<arglist[1]<<arglist[2]<<arglist[3]<<arglist[4]<<arglist[5]<<arglist[6]<<arglist[7];
      pid = fork();
      switch (pid) {
        case -1:
          perror("fork failed");
          return 1;
        case 0:
          execvp(arglist[0], &arglist[1]);
          qDebug("execvp failed");
          return 1;
        default:
          while(temp != pid)temp=wait(&exitstatus);
        }
      return 0;
}
bool SceneSetupNetwork::validatePage()
{
                printf("use setting\n");
                QString ipString = lineEditIp1st.text()+'.'+lineEditIp2nd.text()+'.'\
                        +lineEditIp3rd.text()+'.'+lineEditIp4th.text();
                QString maskString = lineEditMask1st.text()+'.'+lineEditMask2nd.text()+'.'\
                        +lineEditMask3rd.text()+'.'+lineEditMask4th.text();
                QString gatewayString = lineEditGateway1st.text()+'.'+lineEditGateway2nd.text()+'.'\
                        +lineEditGateway3rd.text()+'.'+lineEditGateway4th.text();
                QString dnsString = lineEditDns1st.text()+'.'+lineEditDns2nd.text()+'.'\
                        +lineEditDns3rd.text()+'.'+lineEditDns4th.text();

                char ipBuf[20] = {0};
                char maskBuf[20] = {0};
                char gatewayBuf[20] = {0};
                char dnsBuf[20] = {0};

                /*************************************************************************/
                QFile f(NETWORK_CONFIG_FILE);

                if(!f.open(QIODevice::ReadOnly ))qDebug("Open network.sh failed.") ;
                else{
                    QString str(f.readAll());
                    if(str.contains("ifconfig eth0",Qt::CaseInsensitive)){
                        str.replace(QRegExp("ifconfig.* up"),\
                                    QString("ifconfig eth0 ")+ipString+QString(" broadcast ")+gatewayString\
                                    +QString(" netmask ")+maskString+QString(" up"));
                    }
                    f.close();
                    if(!f.open(QIODevice::WriteOnly | QIODevice::Text  )) qDebug("Open network.sh failed.") ;
                    else{
                        f.write(str.toUtf8());
                        f.close();
                    }
                   }
                /*************************************************************************/
                strncpy(ipBuf, ipString.toStdString().c_str(), sizeof(ipBuf));
                strncpy(maskBuf, maskString.toStdString().c_str(), sizeof(maskBuf));
                strncpy(gatewayBuf, gatewayString.toStdString().c_str(), sizeof(gatewayBuf));
                strncpy(dnsBuf, dnsString.toStdString().c_str(), sizeof(dnsBuf));

#ifdef HW_ARM
               Run((char *)"/sbin/ifconfig",(char *)"ifconfig",(char *)"eth0", ipBuf,\
                   (char *)"broadcast",gatewayBuf,(char *) "netmask", maskBuf);
#endif
                //Run((char *)"/sbin/route add default gw",(char *)"route add default gw", gatewayBuf, NULL, NULL, NULL);
                //Run((char *)"/bin/echo", (char *)"echo",(char *)"nameserver", dnsBuf, (char *)">> /etc/resolv.conf", NULL);
       // }
        //wizard()->next();
        return true;
}
#endif

void SceneSetupNetwork::CustomSetText()
{
    buttonSave.setText(tr("Confirm"));
    buttonCancel.setText(tr("Cancel"));

    buttonSetup.setText(tr("Set"));
    networkLabel.setText(tr("Parameter of Network:"));
    ipLabel.setText(tr("Ip:"));
    lineEditIp1st.setText(tr("192"));
    lineEditIp2nd.setText(tr("168"));
    lineEditIp3rd.setText(tr("1"));
    lineEditIp4th.setText(tr("10"));
    lineEditMask1st.setText(tr("255"));
    lineEditMask2nd.setText(tr("255"));
    lineEditMask3rd.setText(tr("255"));
    lineEditMask4th.setText(tr("0"));
    lineEditGateway1st.setText(tr("192"));
    lineEditGateway2nd.setText(tr("168"));
    lineEditGateway3rd.setText(tr("1"));
    lineEditGateway4th.setText(tr("255"));
    lineEditDns1st.setText(tr("192"));
    lineEditDns2nd.setText(tr("168"));
    lineEditDns3rd.setText(tr("0"));
    lineEditDns4th.setText(tr("1"));
    maskLabel.setText(tr("Mask:"));
    gatewayLabel.setText(tr("Gateway:"));
    dnsLabel.setText(tr("DNS:"));

#ifdef OS_LINUX
    buttonInitWlan.setText(tr("Init wlan"));
#endif
}

void SceneSetupNetwork::setCusVisible(bool vis, char* para)
{
    para = para;
    if(vis==true){
        QFile f(NETWORK_CONFIG_FILE);
        if(!f.open(QIODevice::ReadOnly ))qDebug("Open network.sh failed.") ;
        else{
            QString str(f.readAll());
            str=str.mid(str.indexOf("eth0")+5);
            lineEditIp1st.setText(str.mid(0,str.indexOf(".")));
            str=str.mid(str.indexOf(".")+1);
            lineEditIp2nd.setText(str.mid(0,str.indexOf(".")));
            str=str.mid(str.indexOf(".")+1);
            lineEditIp3rd.setText(str.mid(0,str.indexOf(".")));
            str=str.mid(str.indexOf(".")+1);
            lineEditIp4th.setText(str.mid(0,str.indexOf(" ")));
            str=str.mid(str.indexOf("broadcast")+10);
            lineEditGateway1st.setText(str.mid(0,str.indexOf(".")));
            str=str.mid(str.indexOf(".")+1);
            lineEditGateway2nd.setText(str.mid(0,str.indexOf(".")));
            str=str.mid(str.indexOf(".")+1);
            lineEditGateway3rd.setText(str.mid(0,str.indexOf(".")));
            str=str.mid(str.indexOf(".")+1);
            lineEditGateway4th.setText(str.mid(0,str.indexOf(" ")));
            str=str.mid(str.indexOf("netmask")+8);
            lineEditMask1st.setText(str.mid(0,str.indexOf(".")));
            str=str.mid(str.indexOf(".")+1);
            lineEditMask2nd.setText(str.mid(0,str.indexOf(".")));
            str=str.mid(str.indexOf(".")+1);
            lineEditMask3rd.setText(str.mid(0,str.indexOf(".")));
            str=str.mid(str.indexOf(".")+1);
            lineEditMask4th.setText(str.mid(0,str.indexOf(" ")));
            }
            f.close();
    }
    buttonSave.setVisible(vis);
    buttonCancel.setVisible(vis);

    buttonSetup.setVisible(vis);
    networkLabel.setVisible(vis);
    ipLabel.setVisible(vis);
    lineEditIp1st.setVisible(vis);
    lineEditIp2nd.setVisible(vis);
    lineEditIp3rd.setVisible(vis);
    lineEditIp4th.setVisible(vis);
    lineEditMask1st.setVisible(vis);
    lineEditMask2nd.setVisible(vis);
    lineEditMask3rd.setVisible(vis);
    lineEditMask4th.setVisible(vis);
    lineEditGateway1st.setVisible(vis);
    lineEditGateway2nd.setVisible(vis);
    lineEditGateway3rd.setVisible(vis);
    lineEditGateway4th.setVisible(vis);
    lineEditDns1st.setVisible(false);
    lineEditDns2nd.setVisible(false);
    lineEditDns3rd.setVisible(false);
    lineEditDns4th.setVisible(false);
    maskLabel.setVisible(vis);
    gatewayLabel.setVisible(vis);
    dnsLabel.setVisible(false);
#ifdef OS_LINUX
    buttonInitWlan.setVisible(vis);
    wlanScanDialog.setVisible(vis);
#endif
}
