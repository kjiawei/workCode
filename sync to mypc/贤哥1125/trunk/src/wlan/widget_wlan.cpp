#include "widget_connect_wlan_icon.h"
#include "widget_wlan.h"

#include <stdio.h>

#ifdef Q_OS_LINUX

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <unistd.h>
#include <sys/stat.h>

#endif

#include <signal.h>
#include <string.h>

#ifdef Q_OS_LINUX

//#define WLAN_ADD_NUM SIGUSR1
//#define WLAN_REMOVE_NUM SIGUSR2

#define WLAN_ADD_NUM 35
#define WLAN_REMOVE_NUM 36

#endif

int APP_NetLink_Init()
{
#ifdef Q_OS_LINUX
    signal(WLAN_ADD_NUM, WlanAdd);
    signal(WLAN_REMOVE_NUM, WlanRemove);

#endif

    return 0;
}

int APP_NetLink_Qiut()
{

#ifdef Q_OS_LINUX
    APP_NetLink_GetIFFlags((char *)("wlan0"));

    signal(WLAN_ADD_NUM, WlanAdd);
    signal(WLAN_REMOVE_NUM, WlanRemove);

#endif

    return 0;
}

#ifdef Q_OS_LINUX

//测试无线网卡是否正常工作，并根据结果显示图标
int APP_NetLink_GetIFFlags( char *NetDev )
{
    int fd = -1;
    struct ifreq ifr;

    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, NetDev);
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        WidgetConnectWlanIcon::showWlanPicture = 0;
        close(fd);
        return -1;
    }
    else if( 0!=(ioctl(fd, SIOCGIFFLAGS, (char*)&ifr)) )//测试无线网卡是否工作正常
    {
        WidgetConnectWlanIcon::showWlanPicture = 0;
        close(fd);
        return -1;
    }

    WidgetConnectWlanIcon::showWlanPicture = 1;

    close(fd);
    return 0;
}

#endif


void WlanAdd(int)
{
    WidgetConnectWlanIcon::showWlanPicture = 1;
    printf("wlan is add\n");
}


void WlanRemove(int)
{
    WidgetConnectWlanIcon::showWlanPicture = 0;
    printf("wlan is remove\n");
}


