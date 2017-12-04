#include "wlan.h"
#include "wpa_ctrl.h"
#include "userdatarequest.h"
#include <stdio.h>
#include <dirent.h>
#include <QtGui>

enum {
    AUTH_NONE = 0,
    AUTH_IEEE8021X = 1,
    AUTH_WPA_PSK = 2,
    AUTH_WPA_EAP = 3,
    AUTH_WPA2_PSK = 4,
    AUTH_WPA2_EAP = 5
};


void CWlan::Scan()
{
    char reply[10];
    size_t reply_len = sizeof(reply);
    ctrlRequest("SCAN", reply, &reply_len);
    updateResults();
}

void CWlan::Connect()
{
    char reply[10];
    size_t reply_len = sizeof(reply);
    QTreeWidgetItem *sel;
    sel = scanResultsWidget->currentItem();

    if (sel == NULL)
    {
        return;
    }

    bssSelected(sel);
    addNetwork();
    ctrlRequest("REASSOCIATE", reply, &reply_len);
}

void CWlan::Disconnect()
{
    char reply[10];
    size_t reply_len = sizeof(reply);
    ctrlRequest("DISCONNECT", reply, &reply_len);
    //removeNetwork();
    //stopWpsRun(false);
}

void CWlan::Remove()
{
    char reply[10];
    size_t reply_len = sizeof(reply);
    if(ssid.isEmpty())
        return;
    ctrlRequest("DISCONNECT", reply, &reply_len);

    ctrlRequest("LIST_NETWORK", reply, &reply_len);

    if ( scanResultsWidget->currentItem() !=NULL )
    {
        QString str = scanResultsWidget->currentItem()->text(0);

        qDebug() << str;

        removeNetwork(str);
    }

    ctrlRequest("SAVE_CONFIG", reply, &reply_len);
}

void CWlan::retranslateUi(QWidget *wlan)
{
    wlan->setWindowTitle(QApplication::translate("wlan", "wlan", 0, QApplication::UnicodeUTF8));
    ConnectButton->setText(QApplication::translate("wlan", "Connect", 0, QApplication::UnicodeUTF8));
    DisconnectButton->setText(QApplication::translate("wlan", "Disconnect", 0, QApplication::UnicodeUTF8));
    ScanButton->setText(QApplication::translate("wlan", "Scan", 0, QApplication::UnicodeUTF8));
    removeButton->setText(tr("remove"));
    QTreeWidgetItem *___qtreewidgetitem = scanResultsWidget->headerItem();
    ___qtreewidgetitem->setText(4, QApplication::translate("wlan", "flags", 0, QApplication::UnicodeUTF8));
    ___qtreewidgetitem->setText(3, QApplication::translate("wlan", "signal", 0, QApplication::UnicodeUTF8));
    ___qtreewidgetitem->setText(2, QApplication::translate("wlan", "frequency", 0, QApplication::UnicodeUTF8));
    ___qtreewidgetitem->setText(1, QApplication::translate("wlan", "BSSID", 0, QApplication::UnicodeUTF8));
    ___qtreewidgetitem->setText(0, QApplication::translate("wlan", "SSID", 0, QApplication::UnicodeUTF8));
} // retranslateUi

void CWlan::setupUi(QWidget *wlan)
{
    if (wlan->objectName().isEmpty())
        wlan->setObjectName(QString::fromUtf8("wlan"));
    ConnectButton = new QPushButton(wlan);
    ConnectButton->setObjectName(QString::fromUtf8("ConnectButton"));
    ConnectButton->setGeometry(QRect(10, 270, 85, 27));
    DisconnectButton = new QPushButton(wlan);
    DisconnectButton->setObjectName(QString::fromUtf8("DisconnectButton"));
    DisconnectButton->setGeometry(QRect(250, 270, 85, 27));
    ScanButton = new QPushButton(wlan);
    ScanButton->setObjectName(QString::fromUtf8("ScanButton"));
    ScanButton->setGeometry(QRect(420, 270, 85, 27));
    scanResultsWidget = new QTreeWidget(wlan);
    scanResultsWidget->setObjectName(QString::fromUtf8("scanResultsWidget"));
    scanResultsWidget->setGeometry(QRect(0, 0, 550, 260));
    removeButton = new QPushButton(wlan);
    removeButton->setObjectName(QString::fromUtf8("removeButton"));
    removeButton->setGeometry(QRect(120, 270, 85, 27));

    retranslateUi(wlan);

    QMetaObject::connectSlotsByName(wlan);
} // setupUi

void CWlan::initWlan()
{
    pwd = NULL;
    udr = NULL;
    ctrl_iface = NULL;
    ctrl_conn = NULL;
    monitor_conn = NULL;
    msgNotifier = NULL;
    ctrl_iface_dir = strdup("/var/run/wpa_supplicant");

    if (openCtrlConnection(ctrl_iface) < 0)
    {
        printf("Failed to open control connection to "
        "wpa_supplicant.\n");
    }
}

CWlan::CWlan(QWidget *parent):QWidget(parent)
{
    setupUi(this);

    pwd = NULL;
    udr = NULL;
    ctrl_iface = NULL;
    ctrl_conn = NULL;
    monitor_conn = NULL;
    msgNotifier = NULL;
    ctrl_iface_dir = strdup("/var/run/wpa_supplicant");

    if (openCtrlConnection(ctrl_iface) < 0) 
    {
        printf("Failed to open control connection to "
        "wpa_supplicant.\n");
    }

    connect(ScanButton, SIGNAL(clicked()), this, SLOT(Scan()));
    connect(ConnectButton, SIGNAL(clicked()), this, SLOT(Connect()));
    connect(DisconnectButton, SIGNAL(clicked()), this, SLOT(Disconnect()));
    connect(removeButton,SIGNAL(clicked()),this,SLOT(Remove()));

}

int CWlan::openCtrlConnection(const char *ifname)
{
	char *cfile;
	int flen;
	char buf[2048], *pos, *pos2;
	size_t len;

	if (ifname) {
		if (ifname != ctrl_iface) {
			free(ctrl_iface);
			ctrl_iface = strdup(ifname);
		}
	} else {
#ifdef CONFIG_CTRL_IFACE_UDP
		free(ctrl_iface);
		ctrl_iface = strdup("udp");
#endif /* CONFIG_CTRL_IFACE_UDP */
#ifdef CONFIG_CTRL_IFACE_UNIX
		struct dirent *dent;
		DIR *dir = opendir(ctrl_iface_dir);
		free(ctrl_iface);
		ctrl_iface = NULL;
		if (dir) {
			while ((dent = readdir(dir))) {
#ifdef _DIRENT_HAVE_D_TYPE
				/* Skip the file if it is not a socket.
				 * Also accept DT_UNKNOWN (0) in case
				 * the C library or underlying file
				 * system does not support d_type. */
				if (dent->d_type != DT_SOCK &&
				    dent->d_type != DT_UNKNOWN)
					continue;
#endif /* _DIRENT_HAVE_D_TYPE */

				if (strcmp(dent->d_name, ".") == 0 ||
				    strcmp(dent->d_name, "..") == 0)
					continue;
				printf("Selected interface '%s'\n",
				       dent->d_name);
				ctrl_iface = strdup(dent->d_name);
				break;
			}
			closedir(dir);
		}
#endif /* CONFIG_CTRL_IFACE_UNIX */
#ifdef CONFIG_CTRL_IFACE_NAMED_PIPE
		struct wpa_ctrl *ctrl;
		int ret;

		free(ctrl_iface);
		ctrl_iface = NULL;

		ctrl = wpa_ctrl_open(NULL);
		if (ctrl) {
            printf("wpa_ctrl_open is ok\n"); 
			len = sizeof(buf) - 1;
			ret = wpa_ctrl_request(ctrl, "INTERFACES", 10, buf,
					       &len, NULL);
			if (ret >= 0) {
				connectedToService = true;
				buf[len] = '\0';
				pos = strchr(buf, '\n');
				if (pos)
					*pos = '\0';
				ctrl_iface = strdup(buf);
			}
			wpa_ctrl_close(ctrl);
		}
#endif /* CONFIG_CTRL_IFACE_NAMED_PIPE */
	}

	if (ctrl_iface == NULL) {
#ifdef CONFIG_NATIVE_WINDOWS
		static bool first = true;
		if (first && !serviceRunning()) {
			first = false;
			if (QMessageBox::warning(
				    this, qAppName(),
				    "wpa_supplicant service is not running.\n"
				    "Do you want to start it?",
				    QMessageBox::Yes | QMessageBox::No) ==
			    QMessageBox::Yes)
				startService();
		}
#endif /* CONFIG_NATIVE_WINDOWS */
        printf("ctrl_iface == NULL\n");
		return -1;
	}

#ifdef CONFIG_CTRL_IFACE_UNIX
	flen = strlen(ctrl_iface_dir) + strlen(ctrl_iface) + 2;
	cfile = (char *) malloc(flen);
	if (cfile == NULL)
		return -1;
	snprintf(cfile, flen, "%s/%s", ctrl_iface_dir, ctrl_iface);
#else /* CONFIG_CTRL_IFACE_UNIX */
	flen = strlen(ctrl_iface) + 1;
	cfile = (char *) malloc(flen);
	if (cfile == NULL)
		return -1;
	snprintf(cfile, flen, "%s", ctrl_iface);
#endif /* CONFIG_CTRL_IFACE_UNIX */

	if (ctrl_conn) {
		wpa_ctrl_close(ctrl_conn);
		ctrl_conn = NULL;
	}

	if (monitor_conn) {
		delete msgNotifier;
		msgNotifier = NULL;
		wpa_ctrl_detach(monitor_conn);
		wpa_ctrl_close(monitor_conn);
		monitor_conn = NULL;
	}

	printf("Trying to connect to '%s'\n", cfile);
	ctrl_conn = wpa_ctrl_open(cfile);
	if (ctrl_conn == NULL) {
		free(cfile);
		return -1;
	}
	monitor_conn = wpa_ctrl_open(cfile);
	free(cfile);
	if (monitor_conn == NULL) {
		wpa_ctrl_close(ctrl_conn);
		return -1;
	}
	if (wpa_ctrl_attach(monitor_conn)) {
		printf("Failed to attach to wpa_supplicant\n");
		wpa_ctrl_close(monitor_conn);
		monitor_conn = NULL;
		wpa_ctrl_close(ctrl_conn);
		ctrl_conn = NULL;
		return -1;
	}

#if defined(CONFIG_CTRL_IFACE_UNIX) || defined(CONFIG_CTRL_IFACE_UDP)
    msgNotifier = new QSocketNotifier(wpa_ctrl_get_fd(monitor_conn),
                      QSocketNotifier::Read, this);
    connect(msgNotifier, SIGNAL(activated(int)),this, SLOT(receiveMsgs()));
#endif

	//adapterSelect->clear();
	//adapterSelect->addItem(ctrl_iface);
	//adapterSelect->setCurrentIndex(0);

	len = sizeof(buf) - 1;
	if (wpa_ctrl_request(ctrl_conn, "INTERFACES", 10, buf, &len, NULL) >=
	    0) {
		buf[len] = '\0';
		pos = buf;
		while (*pos) {
			pos2 = strchr(pos, '\n');
			if (pos2)
				*pos2 = '\0';
			if (strcmp(pos, ctrl_iface) != 0)
		    {
                printf("\n");
                //adapterSelect->addItem(pos);
            }
			if (pos2)
				pos = pos2 + 1;
			else
				break;
		}
	}

	len = sizeof(buf) - 1;
	if (wpa_ctrl_request(ctrl_conn, "GET_CAPABILITY eap", 18, buf, &len,
			     NULL) >= 0) {
		buf[len] = '\0';

		QString res(buf);
		QStringList types = res.split(QChar(' '));
        //bool wps = types.contains("WSC");
		//actionWPS->setEnabled(wps);
		//wpsTab->setEnabled(wps);
		//wpaguiTab->setTabEnabled(wpaguiTab->indexOf(wpsTab), wps);
	}

	return 0;
}

void wpa_gui_msg_cb(char *msg, size_t)
{
	/* This should not happen anymore since two control connections are
	 * used. */
	printf("missed message: %s\n", msg);
}

int CWlan::ctrlRequest(const char *cmd, char *buf, size_t *buflen)
{
    int ret;

    if (ctrl_conn == NULL)
        return -3;
    
    ret = wpa_ctrl_request(ctrl_conn, cmd, strlen(cmd), buf, buflen,wpa_gui_msg_cb);
    
    if (ret == -2)
        printf("'%s' command timed out.\n", cmd);
    else if (ret < 0)
        printf("'%s' command failed.\n", cmd);
    
    return ret;
}


void CWlan::updateResults()
{
	char reply[2048];
	size_t reply_len;
	int index;
	char cmd[20];

    scanResultsWidget->clear();


	index = 0;
    //while (wpagui) {
    while(1){
        snprintf(cmd, sizeof(cmd), "BSS %d", index++);
		if (index > 1000)
			break;

		reply_len = sizeof(reply) - 1;
        //if (wpagui->ctrlRequest(cmd, reply, &reply_len) < 0)
        if (ctrlRequest(cmd, reply, &reply_len) < 0)
			break;
		reply[reply_len] = '\0';

		QString bss(reply);
		if (bss.isEmpty() || bss.startsWith("FAIL"))
			break;

		QString ssid, bssid, freq, signal, flags;

		QStringList lines = bss.split(QRegExp("\\n"));
		for (QStringList::Iterator it = lines.begin();
		     it != lines.end(); it++) {
			int pos = (*it).indexOf('=') + 1;
			if (pos < 1)
				continue;

			if ((*it).startsWith("bssid="))
				bssid = (*it).mid(pos);
			else if ((*it).startsWith("freq="))
				freq = (*it).mid(pos);
			else if ((*it).startsWith("qual="))
				signal = (*it).mid(pos);
			else if ((*it).startsWith("flags="))
				flags = (*it).mid(pos);
			else if ((*it).startsWith("ssid="))
				ssid = (*it).mid(pos);
		}

        QTreeWidgetItem *item = new QTreeWidgetItem(scanResultsWidget);
		if (item) {
			item->setText(0, ssid);
			item->setText(1, bssid);
			item->setText(2, freq);
			item->setText(3, signal);
			item->setText(4, flags);
		}

		if (bssid.isEmpty())
			break;
	}
}


void CWlan::bssSelected(QTreeWidgetItem *sel)
{
    //NetworkConfig *nc = new NetworkConfig();
    //if (nc == NULL)
    //    return;
    //nc->setWpaGui(wpagui);
    //nc->paramsFromScanResults(sel);
    //nc->show();
    //nc->exec();
    
    paramsFromScanResults(sel);
}



void CWlan::paramsFromScanResults(QTreeWidgetItem *sel)
{
    new_network = true;

    /* SSID BSSID frequency signal flags */
    
    //setWindowTitle(sel->text(0));
    
    //ssidEdit->setText(sel->text(0));
    ssid =  sel->text(0);

    QString flags = sel->text(4);
    int auth, encr = 0;
    if (flags.indexOf("[WPA2-EAP") >= 0)
        auth = AUTH_WPA2_EAP;
    else if (flags.indexOf("[WPA-EAP") >= 0)
        auth = AUTH_WPA_EAP;
    else if (flags.indexOf("[WPA2-PSK") >= 0)
        auth = AUTH_WPA2_PSK;
    else if (flags.indexOf("[WPA-PSK") >= 0)
        auth = AUTH_WPA_PSK;
    else
        auth = AUTH_NONE;

    if (flags.indexOf("-CCMP") >= 0)
        encr = 1;
    else if (flags.indexOf("-TKIP") >= 0)
        encr = 0;
    else if (flags.indexOf("WEP") >= 0)
        encr = 1;
    else
        encr = 0;

    //authSelect->setCurrentIndex(auth);
    this_auth = auth;
    authChanged(auth);
    //encrSelect->setCurrentIndex(encr);


    wepEnabled(auth == AUTH_NONE && encr == 1);

    getEapCapa();

    if (flags.indexOf("[WPS") >= 0)
    {
        //useWpsButton->setEnabled(true);
    }

    bssid = sel->text(1);
}



void CWlan::authChanged(int sel)
{
    //pskEdit->setEnabled(sel == AUTH_WPA_PSK || sel == AUTH_WPA2_PSK);
    if (sel == AUTH_WPA_PSK || sel == AUTH_WPA2_PSK)
        pskBool = true;
    else
        pskBool = false;

    //bool eap = sel == AUTH_IEEE8021X || sel == AUTH_WPA_EAP ||
    //     sel == AUTH_WPA2_EAP;
    //eapSelect->setEnabled(eap);
    //identityEdit->setEnabled(eap);
    //passwordEdit->setEnabled(eap);
    //cacertEdit->setEnabled(eap);
    //phase2Select->setEnabled(eap);
    //
    
    //if (eap)
       //eapChanged(eapSelect->currentIndex());
    
    //while (encrSelect->count())
    //{
        //encrSelect->removeItem(0);
    //}
    
    if (sel == AUTH_NONE || sel == AUTH_IEEE8021X)
    {
        //encrSelect->addItem("None");
        //encrSelect->addItem("WEP");
        //encrSelect->setCurrentIndex(sel == AUTH_NONE ? 0 : 1);
    }
    else
    {
        //encrSelect->addItem("TKIP");
        //encrSelect->addItem("CCMP");
        //encrSelect->setCurrentIndex((sel == AUTH_WPA2_PSK ||
            //sel == AUTH_WPA2_EAP) ? 1 : 0);
    }

    //wepEnabled(sel == AUTH_IEEE8021X);
}



void CWlan::wepEnabled(bool enabled)
{
    enabled = enabled;

    //wep0Edit->setEnabled(enabled);
    //wep1Edit->setEnabled(enabled);
    //wep2Edit->setEnabled(enabled);
    //wep3Edit->setEnabled(enabled);
    //wep0Radio->setEnabled(enabled);
    //wep1Radio->setEnabled(enabled);
    //wep2Radio->setEnabled(enabled);
    //wep3Radio->setEnabled(enabled);
}



void CWlan::getEapCapa()
{
    char reply[256];
    size_t reply_len;

    //if (wpagui == NULL)
        //return;

    reply_len = sizeof(reply) - 1;
    if (ctrlRequest("GET_CAPABILITY eap", reply, &reply_len) < 0)
        return;
    reply[reply_len] = '\0';

    QString res(reply);
    QStringList types = res.split(QChar(' '));
    //eapSelect->insertItems(-1, types);
}



void CWlan::addNetwork()
{
    bool ok;
    int psklen;
    QString text;
	char reply[10], cmd[256];
	size_t reply_len;
	int id;
	//int psklen = pskEdit->text().length();
	//int auth = authSelect->currentIndex();
    int auth = this_auth;

    if (pwd)
    {
        delete pwd;
    }

    pwd = new PASSWORD ();
	//if (auth == AUTH_WPA_PSK || auth == AUTH_WPA2_PSK) {
	//	if (psklen < 8 || psklen > 64) {
	//		QMessageBox::warning(this, "WPA Pre-Shared Key Error",
	//				     "WPA-PSK requires a passphrase "
	//				     "of 8 to 63 characters\n"
	//				     "or 64 hex digit PSK");
	//		pskEdit->setFocus();
	//		return;
	//	}
	//}

	//if (idstrEdit->isEnabled() && !idstrEdit->text().isEmpty()) {
	//	QRegExp rx("^(\\w|-)+$");
	//	if (rx.indexIn(idstrEdit->text()) < 0) {
	//		QMessageBox::warning(this, "Network ID Error",
	//				     "Network ID String contains "
	//				     "non-word characters.\n"
	//				     "It must be a simple string, "
	//				     "without spaces, containing\n"
	//				     "only characters in this range: "
	//				     "[A-Za-z0-9_-]\n");
	//		idstrEdit->setFocus();
	//		return;
	//	}
	//}

	//if (wpagui == NULL)
	//	return;

	memset(reply, 0, sizeof(reply));
	reply_len = sizeof(reply) - 1;

	if (new_network) {
		//wpagui->ctrlRequest("ADD_NETWORK", reply, &reply_len);
		ctrlRequest("ADD_NETWORK", reply, &reply_len);
		if (reply[0] == 'F') {
			QMessageBox::warning(this, "wpa_gui", "Failed to add "
				     "network to wpa_supplicant\n"
					     "configuration.");
			return;
		}
		id = atoi(reply);
	} else
		id = edit_network_id;

	setNetworkParam(id, "ssid", ssid.toAscii().constData(),
			true);

    const char *key_mgmt = NULL, *proto = NULL ;
    //const char *pairwise = NULL;

    switch (auth) {
	case AUTH_NONE:
		key_mgmt = "NONE";
		break;
	case AUTH_IEEE8021X:
		key_mgmt = "IEEE8021X";
		break;
	case AUTH_WPA_PSK:
		key_mgmt = "WPA-PSK";
		proto = "WPA";
		break;
	case AUTH_WPA_EAP:
		key_mgmt = "WPA-EAP";
		proto = "WPA";
		break;
	case AUTH_WPA2_PSK:
		key_mgmt = "WPA-PSK";
		proto = "WPA2";
		break;
	case AUTH_WPA2_EAP:
		key_mgmt = "WPA-EAP";
		proto = "WPA2";
		break;
	}

#if 0
	if (auth == AUTH_WPA_PSK || auth == AUTH_WPA_EAP ||
	    auth == AUTH_WPA2_PSK || auth == AUTH_WPA2_EAP) {
		int encr = encrSelect->currentIndex();
		if (encr == 0)
			pairwise = "TKIP";
		else
			pairwise = "CCMP";
	}
#endif

    if (proto)
        setNetworkParam(id, "proto", proto, false);
    if (key_mgmt)
        setNetworkParam(id, "key_mgmt", key_mgmt, false);

#if 0	
    if (pairwise) {
		setNetworkParam(id, "pairwise", pairwise, false);
		setNetworkParam(id, "group", "TKIP CCMP WEP104 WEP40", false);
	}
#endif

    //if (pskEdit->isEnabled() &&
    //    strcmp(pskEdit->text().toAscii().constData(),
    //	   WPA_GUI_KEY_DATA) != 0)
    //	setNetworkParam(id, "psk",
    //			pskEdit->text().toAscii().constData(),
    //			psklen != 64);

    //if (pskBool == true)
    //    setNetworkParam(id, "psk",
    //                    (char *)"22345678",
    //            true);


    while( (1) && (pskBool == true) )
    {

        //pwd->show();
        //pwd->raise();
        //pwd->activateWindow();

        if (pwd->exec())
        {
            ok = true;
            text = pwd->getText();
        }
        else
        {
            ok = false;
        }

        //text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
        //                                     tr("password:"), QLineEdit::Normal,
        //                                     QDir::home().dirName(), &ok);

        //text = "12345678";
        //ok = true;



        if (ok)
            psklen = text.length();
        else
        {
            psklen = 0;
            return;
        }

        if (psklen < 8 || psklen > 64)
        {
            continue;
        }
        else
        {
            setNetworkParam(id, "psk",
                        text.toAscii().constData(),
                        psklen != 64);
            break;
        }

    }

    setNetworkParam(id, "password", "NULL", false);

#if 0
	if (eapSelect->isEnabled()) {
		const char *eap =
			eapSelect->currentText().toAscii().constData();
		setNetworkParam(id, "eap", eap, false);
		if (strcmp(eap, "SIM") == 0 || strcmp(eap, "AKA") == 0)
			setNetworkParam(id, "pcsc", "", true);
		else
			setNetworkParam(id, "pcsc", "NULL", false);
	}
	if (phase2Select->isEnabled()) {
		QString eap = eapSelect->currentText();
		QString inner = phase2Select->currentText();
		char phase2[32];
		phase2[0] = '\0';
		if (eap.compare("PEAP") == 0) {
			if (inner.startsWith("EAP-"))
				snprintf(phase2, sizeof(phase2), "auth=%s",
					 inner.right(inner.size() - 4).
					 toAscii().constData());
		} else if (eap.compare("TTLS") == 0) {
			if (inner.startsWith("EAP-"))
				snprintf(phase2, sizeof(phase2), "autheap=%s",
					 inner.right(inner.size() - 4).
					 toAscii().constData());
			else
				snprintf(phase2, sizeof(phase2), "auth=%s",
					 inner.toAscii().constData());
		} else if (eap.compare("FAST") == 0) {
			const char *provisioning = NULL;
			if (inner.startsWith("EAP-")) {
				snprintf(phase2, sizeof(phase2), "auth=%s",
					 inner.right(inner.size() - 4).
					 toAscii().constData());
				provisioning = "fast_provisioning=2";
			} else if (inner.compare("GTC(auth) + MSCHAPv2(prov)")
				   == 0) {
				snprintf(phase2, sizeof(phase2),
					 "auth=GTC auth=MSCHAPV2");
				provisioning = "fast_provisioning=1";
			} else
				provisioning = "fast_provisioning=3";
			if (provisioning) {
				char blob[32];
				setNetworkParam(id, "phase1", provisioning,
						true);
				snprintf(blob, sizeof(blob),
					 "blob://fast-pac-%d", id);
				setNetworkParam(id, "pac_file", blob, true);
			}
		}
		if (phase2[0])
			setNetworkParam(id, "phase2", phase2, true);
		else
			setNetworkParam(id, "phase2", "NULL", false);
	} else
		setNetworkParam(id, "phase2", "NULL", false);
	if (identityEdit->isEnabled() && identityEdit->text().length() > 0)
		setNetworkParam(id, "identity",
				identityEdit->text().toAscii().constData(),
				true);
	else
		setNetworkParam(id, "identity", "NULL", false);


	if (passwordEdit->isEnabled() && passwordEdit->text().length() > 0 &&
	    strcmp(passwordEdit->text().toAscii().constData(),
		   WPA_GUI_KEY_DATA) != 0)
		setNetworkParam(id, "password",
				passwordEdit->text().toAscii().constData(),
				true);
	else if (passwordEdit->text().length() == 0)
		setNetworkParam(id, "password", "NULL", false);


	if (cacertEdit->isEnabled() && cacertEdit->text().length() > 0)
		setNetworkParam(id, "ca_cert",
				cacertEdit->text().toAscii().constData(),
				true);
	else
		setNetworkParam(id, "ca_cert", "NULL", false);
	writeWepKey(id, wep0Edit, 0);
	writeWepKey(id, wep1Edit, 1);
	writeWepKey(id, wep2Edit, 2);
	writeWepKey(id, wep3Edit, 3);

	if (wep0Radio->isEnabled() && wep0Radio->isChecked())
		setNetworkParam(id, "wep_tx_keyidx", "0", false);
	else if (wep1Radio->isEnabled() && wep1Radio->isChecked())
		setNetworkParam(id, "wep_tx_keyidx", "1", false);
	else if (wep2Radio->isEnabled() && wep2Radio->isChecked())
		setNetworkParam(id, "wep_tx_keyidx", "2", false);
	else if (wep3Radio->isEnabled() && wep3Radio->isChecked())
		setNetworkParam(id, "wep_tx_keyidx", "3", false);

	if (idstrEdit->isEnabled() && idstrEdit->text().length() > 0)
		setNetworkParam(id, "id_str",
				idstrEdit->text().toAscii().constData(),
				true);
	else
		setNetworkParam(id, "id_str", "NULL", false);

	if (prioritySpinBox->isEnabled()) {
		QString prio;
		prio = prio.setNum(prioritySpinBox->value());
		setNetworkParam(id, "priority", prio.toAscii().constData(),
				false);
	}
#endif
	snprintf(cmd, sizeof(cmd), "ENABLE_NETWORK %d", id);
	reply_len = sizeof(reply);
	ctrlRequest(cmd, reply, &reply_len);
	if (strncmp(reply, "OK", 2) != 0) {
		QMessageBox::warning(this, "wpa_gui", "Failed to enable "
				     "network in wpa_supplicant\n"
				     "configuration.");
		/* Network was added, so continue anyway */
	}
	//wpagui->triggerUpdate();
    ctrlRequest("SAVE_CONFIG", reply, &reply_len);

#if 0

	close();

#endif

}


#if 0
void NetworkConfig::writeWepKey(int network_id, QLineEdit *edit, int id)
{
	char buf[10];
	bool hex;
	const char *txt, *pos;
	size_t len;

	if (!edit->isEnabled() || edit->text().isEmpty())
		return;

	/*
	 * Assume hex key if only hex characters are present and length matches
	 * with 40, 104, or 128-bit key
	 */
	txt = edit->text().toAscii().constData();
	if (strcmp(txt, WPA_GUI_KEY_DATA) == 0)
		return;
	len = strlen(txt);
	if (len == 0)
		return;
	pos = txt;
	hex = true;
	while (*pos) {
		if (!((*pos >= '0' && *pos <= '9') ||
		      (*pos >= 'a' && *pos <= 'f') ||
		      (*pos >= 'A' && *pos <= 'F'))) {
			hex = false;
			break;
		}
		pos++;
	}
	if (hex && len != 10 && len != 26 && len != 32)
		hex = false;
	snprintf(buf, sizeof(buf), "wep_key%d", id);
	setNetworkParam(network_id, buf, txt, !hex);
}
#endif



int CWlan::setNetworkParam(int id, const char *field,
				   const char *value, bool quote)
{
	char reply[10], cmd[256];
	size_t reply_len;
	snprintf(cmd, sizeof(cmd), "SET_NETWORK %d %s %s%s%s",
		 id, field, quote ? "\"" : "", value, quote ? "\"" : "");
	reply_len = sizeof(reply);
	//wpagui->ctrlRequest(cmd, reply, &reply_len);
	ctrlRequest(cmd, reply, &reply_len);
	return strncmp(reply, "OK", 2) == 0 ? 0 : -1;
}   

    
void CWlan::receiveMsgs()
{
    char buf[256];
    size_t len;

    while (monitor_conn && wpa_ctrl_pending(monitor_conn) > 0) {
        len = sizeof(buf) - 1;
        if (wpa_ctrl_recv(monitor_conn, buf, &len) == 0) {
            buf[len] = '\0';
            processMsg(buf);
        }
    }
}

int str_match(const char *a, const char *b)
{
    return strncmp(a, b, strlen(b)) == 0;
}

void CWlan::processMsg(char *msg)
{
    char *pos = msg;
    //char *pos2;
    //int priority = 2;

    if (*pos == '<') {
        /* skip priority */
        pos++;
        //priority = atoi(pos);
        pos = strchr(pos, '>');
        if (pos)
            pos++;
        else
            pos = msg;
    }

    //WpaMsg wm(pos, priority);
    //if (eh)
   //     eh->addEvent(wm);
   // msgs.append(wm);
   // while (msgs.count() > 100)
    //    msgs.pop_front();

    /* Update last message with truncated version of the event */
   // if (strncmp(pos, "CTRL-", 5) == 0) {
   //     pos2 = strchr(pos, str_match(pos, WPA_CTRL_REQ) ? ':' : ' ');
  //      if (pos2)
   //         pos2++;
   //     else
    //        pos2 = pos;
   // } else
   //     pos2 = pos;
    //QString lastmsg = pos2;
    //lastmsg.truncate(40);
    //textLastMessage->setText(lastmsg);

    //pingsToStatusUpdate = 0;
    //networkMayHaveChanged = true;

    //printf("hehe\n");

    if (str_match(pos, WPA_CTRL_REQ))
    {
        //printf("222222222\n");
        //processCtrlReq(pos + strlen(WPA_CTRL_REQ));
    }
#if 0
    else if (str_match(pos, WPA_EVENT_SCAN_RESULTS) && scanres)
        scanres->updateResults();
#endif
    else if (str_match(pos, WPA_EVENT_DISCONNECTED))
    {
        //showTrayMessage(QSystemTrayIcon::Information, 3,
        //        "Disconnected from network.");

        //printf("my_disconnected\n");
        //Disconnect();
        //Connect();
    }
#if 0
    else if (str_match(pos, WPA_EVENT_CONNECTED)) {
        showTrayMessage(QSystemTrayIcon::Information, 3,
                "Connection to network established.");
        QTimer::singleShot(5 * 1000, this, SLOT(showTrayStatus()));
        stopWpsRun(true);
    } else if (str_match(pos, WPS_EVENT_AP_AVAILABLE_PBC)) {
        showTrayMessage(QSystemTrayIcon::Information, 3,
                "Wi-Fi Protected Setup (WPS) AP\n"
                "in active PBC mode found.");
        wpsStatusText->setText("WPS AP in active PBC mode found");
        if (textStatus->text() == "INACTIVE" ||
            textStatus->text() == "DISCONNECTED")
            wpaguiTab->setCurrentWidget(wpsTab);
        wpsInstructions->setText("Press the PBC button on the screen "
                     "to start registration");
    } else if (str_match(pos, WPS_EVENT_AP_AVAILABLE_PIN)) {
        showTrayMessage(QSystemTrayIcon::Information, 3,
                "Wi-Fi Protected Setup (WPS) AP\n"
                " in active PIN mode found.");
        wpsStatusText->setText("WPS AP with recently selected "
                       "registrar");
        if (textStatus->text() == "INACTIVE" ||
            textStatus->text() == "DISCONNECTED")
            wpaguiTab->setCurrentWidget(wpsTab);
    } else if (str_match(pos, WPS_EVENT_AP_AVAILABLE)) {
        showTrayMessage(QSystemTrayIcon::Information, 3,
                "Wi-Fi Protected Setup (WPS)\n"
                "AP detected.");
        wpsStatusText->setText("WPS AP detected");
    } else if (str_match(pos, WPS_EVENT_OVERLAP)) {
        showTrayMessage(QSystemTrayIcon::Information, 3,
                "Wi-Fi Protected Setup (WPS)\n"
                "PBC mode overlap detected.");
        wpsStatusText->setText("PBC mode overlap detected");
        wpsInstructions->setText("More than one AP is currently in "
                     "active WPS PBC mode. Wait couple of "
                     "minutes and try again");
        wpaguiTab->setCurrentWidget(wpsTab);
    } else if (str_match(pos, WPS_EVENT_CRED_RECEIVED)) {
        wpsStatusText->setText("Network configuration received");
        wpaguiTab->setCurrentWidget(wpsTab);
    } else if (str_match(pos, WPA_EVENT_EAP_METHOD)) {
        if (strstr(pos, "(WSC)"))
            wpsStatusText->setText("Registration started");
    } else if (str_match(pos, WPS_EVENT_M2D)) {
        wpsStatusText->setText("Registrar does not yet know PIN");
    } else if (str_match(pos, WPS_EVENT_FAIL)) {
        wpsStatusText->setText("Registration failed");
    } else if (str_match(pos, WPS_EVENT_SUCCESS)) {
        wpsStatusText->setText("Registration succeeded");
    }
#endif
}


void CWlan::processCtrlReq(const char *req)
{
    if (udr) {
        udr->close();
        delete udr;
    }
    udr = new UserDataRequest();
    if (udr == NULL)
        return;
    if (udr->setParams(this, req) < 0) {
        delete udr;
        udr = NULL;
        return;
    }
    udr->show();
    udr->exec();
}

void CWlan::removeNetwork(const QString &sel)
{
    QString cmd(sel);
    char reply[10];
    size_t reply_len = sizeof(reply);

#if 0
    if (cmd.startsWith("Select any"))
        return;

    if (!cmd.startsWith("all")) {
        int pos = cmd.indexOf(':');
        if (pos < 0) {
            printf("Invalid removeNetwork '%s'\n",
                   cmd.toAscii().constData());
            return;
        }
        cmd.truncate(pos);
    }
#endif

    cmd.prepend("REMOVE_NETWORK ");

    qDebug() << cmd;

    ctrlRequest(cmd.toAscii().constData(), reply, &reply_len);
}
