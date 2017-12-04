#include <QWidget>
#include <QSocketNotifier>
#include <QObject>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QtGui>
#include "password.h"

class UserDataRequest;

class CWlan:public QWidget
{
    Q_OBJECT

public:
    CWlan(QWidget *parent = 0);
    int ctrlRequest(const char *cmd, char *buf, size_t *buflen);
    void initWlan();

private:

    QPushButton *ConnectButton;
    QPushButton *DisconnectButton;
    QPushButton *ScanButton;
    QTreeWidget *scanResultsWidget;
    QPushButton *removeButton;

    char * ctrl_iface;
    struct wpa_ctrl * ctrl_conn;
    struct wpa_ctrl * monitor_conn;
    char * ctrl_iface_dir;
    QSocketNotifier * msgNotifier;
    int this_auth;
    QString bssid;
    QString ssid;
    int edit_network_id;
    bool new_network;
    bool pskBool;
    UserDataRequest *udr;
    PASSWORD *pwd;

    void setupUi(QWidget *wlan);
    void retranslateUi(QWidget *wlan);
    int openCtrlConnection(const char *ifname);

    void bssSelected(QTreeWidgetItem *sel);
    void paramsFromScanResults(QTreeWidgetItem *sel);
    void authChanged(int sel);
    void wepEnabled(bool enabled);
    void getEapCapa();
    void addNetwork();
    void removeNetwork(const QString &sel);
    int setNetworkParam(int id, const char *field,
				   const char *value, bool quote);
    void updateResults();
    //pa_gui_msg_cb(char *msg, size_t);

    void processMsg(char *msg);
    void processCtrlReq(const char *req);

public slots:
    void receiveMsgs();

private slots:
    void Scan();
    void Connect();
    void Disconnect();
    void Remove();
};
