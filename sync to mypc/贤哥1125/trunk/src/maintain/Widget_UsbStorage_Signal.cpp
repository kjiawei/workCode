#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>
#include <errno.h>

#include "Widget_UsbStorage_Signal.h"
#include "scene_maintain.h"
#include "scene_maintain_project_usbstroage_button.h"
#include "scene_maintain_project_update.h"

#define USBSTORAGE_ADD_NUM SIGUSR1
#define USBSTORAGE_REMOVE_NUM SIGUSR2

#define USBUPDATESIGNAL SIGRTMIN

extern WidgetUSBStorageButton* widgetUSBStorageButton;

int APP_UsbStorage_Signal_Init()
{
#ifdef Q_OS_LINUX ///操作系统标示定义，在/usr/include/QtCore/qglobal.h中定义，由编译系统自动识别
    signal(USBSTORAGE_ADD_NUM, usbStorageAdd);  //添加U盘 自定义的信号
    signal(USBSTORAGE_REMOVE_NUM, usbStorageRemove);//拔出U盘

    struct sigaction sa;
    sa.sa_sigaction = usbUpdateProgress;
    sa.sa_flags |= SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sigaction(USBUPDATESIGNAL,&sa,NULL);
#endif
    return 0;
}

int APP_UsbStorage_Signal_Quit()
{
#ifdef Q_OS_LINUX
    //signal(USBSTORAGE_ADD_NUM, usbStorageAdd);
    //signal(USBSTORAGE_REMOVE_NUM, usbStorageRemove);

    struct stat st;
    stat("/mnt/usbStorage",&st);
    //printf("st_size :%d",st.st_size);
    if(st.st_size<=0 || st.st_size == 1024){
        SceneMaintain::isUsbStroageExist = false;
    }else{
        SceneMaintain::isUsbStroageExist = true;
    }
#endif
    return 0;
}

#ifdef Q_OS_LINUX
void usbUpdateProgress(int signum,siginfo_t *info,void*myact)
{
    Q_UNUSED(signum);
    Q_UNUSED(myact);
    int msg=info->si_value.sival_int;
    qDebug() << "usbUpdateProgress";
    SceneProjectModeUpdate::updateData = msg;
}

void usbStorageAdd(int)
{
    SceneMaintain::isUsbStroageExist = true;
    qDebug() << "usbStorageAdd";
    widgetUSBStorageButton->draw();  //重绘USB的图标
}

void usbStorageRemove(int)
{
    SceneMaintain::isUsbStroageExist = false;
    qDebug() << "usbStorageRemove";
    widgetUSBStorageButton->draw();
}

#endif


