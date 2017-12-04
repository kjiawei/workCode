#ifndef WIDGET_WLAN_H
#define WIDGET_WLAN_H

#include <QtGui>
#include <signal.h>

extern "C" {

    int APP_NetLink_Init();
    int APP_NetLink_Qiut();

#ifdef Q_OS_LINUX
    int APP_NetLink_GetIFFlags( char *NetDev );
    void usbUpdateProgress(int signum,siginfo_t *info,void*myact);
#endif
    void usbStorageAdd(int);
    void usbStorageRemove(int);
    void WlanAdd(int);
    void WlanRemove(int);
}

#endif
