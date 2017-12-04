#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "lamp_control.h"


#define WARN_LAMP_MAGIC      189

#define  RED_ON      _IO(WARN_LAMP_MAGIC,0)
#define  RED_OFF     _IO(WARN_LAMP_MAGIC,1)
#define YELLOW_ON     _IO(WARN_LAMP_MAGIC,2)
#define YELLOW_OFF    _IO(WARN_LAMP_MAGIC,3)
#define GREEN_ON      _IO(WARN_LAMP_MAGIC,4)
#define GREEN_OFF     _IO(WARN_LAMP_MAGIC,5)

int lamp_state=0;
int fd_lamp;

int Lampinit(void)
{
    fd_lamp = open("/dev/warn_lamp_mem", O_RDWR);
    if (fd_lamp < 0)
    {
        printf("can't open device node 'warn_lamp_mem'' !\n");
        return -1;
    }
    return 0;
 }

int LampControl(int mode)
{
    if(lamp_state==mode)
        return -1;

    lamp_state=mode;

    if(mode == 1)
    {
        ioctl(fd_lamp,RED_ON);
        ioctl(fd_lamp,YELLOW_OFF);
        ioctl(fd_lamp,GREEN_OFF);
    }
    else if(mode == 2)
    {
        ioctl(fd_lamp,RED_OFF);
        ioctl(fd_lamp,YELLOW_OFF);
        ioctl(fd_lamp,GREEN_OFF);
    }
    else if(mode == 3)
    {
        ioctl(fd_lamp,RED_OFF);
        ioctl(fd_lamp,YELLOW_ON);
        ioctl(fd_lamp,GREEN_OFF);
    }
    if(mode == 4)
    {
        ioctl(fd_lamp,RED_OFF);
        ioctl(fd_lamp,YELLOW_OFF);
        ioctl(fd_lamp,GREEN_OFF);
    }
    else if(mode == 5)
    {
        ioctl(fd_lamp,RED_OFF);
        ioctl(fd_lamp,YELLOW_OFF);
        ioctl(fd_lamp,GREEN_ON);
    }
    else if(mode == 6)
    {
        ioctl(fd_lamp,RED_OFF);
        ioctl(fd_lamp,YELLOW_OFF);
        ioctl(fd_lamp,GREEN_OFF);     

    }

//    else
//    {
//	    close(fd_lamp);
//        return -1;
//    }

//    close(fd_lamp);
	
    return 0;
}




