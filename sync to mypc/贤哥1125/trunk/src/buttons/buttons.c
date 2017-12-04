#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "buttons.h"
#include <signal.h>

int fd_button;
int btnMute;
int btnAlarmReset;

//按键中断发生后，读按键值#/

void key_handler (int irq)
{
        int key_num;

        irq=irq;
        read(fd_button, &key_num, sizeof(int));
        //printf(" %d key hit\n", key_num);
        buttons(key_num);  //把按键值作为QT信号发送#/
}


int ButtonsInit(void)
{
    int  oflags;

    fd_button = open("/dev/key_irq", O_RDONLY);
    if (fd_button < 0)
    {
        printf("can't open device node 'key_irq'' !\n");
        return -1;
    }
    printf(" open device node 'button_key_irq''succes !\n");
    signal(SIGIO, key_handler); //Linux 信号处理函数，有SIGIO时，调用key_handler函数#/
    fcntl(fd_button, F_SETOWN, getpid());//设置接收SIGIO和SIGURG信号的进程ID和进程组ID#/
    oflags = fcntl(fd_button, F_GETFL);//获取文件状态标志#/
    fcntl(fd_button, F_SETFL, oflags | FASYNC);//添加异步标志#/
    signal(SIGKILL, sigroutine_buttons);
    return 0;
}

void sigroutine_buttons(int dunno)
{
    dunno = dunno;
#ifdef OS_LINUX
    close(fd_button);
#endif
}



