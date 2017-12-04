#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "sound_adjust.h"

/*!
 * \brief SoundAdjustManual 
 * \param value 0-12
 * \return
 */
int SoundAdjustManual(int value)
{
	int fd;

	fd = open("/dev/fpga", O_RDWR);

	if (fd < 0)
	{
		printf("can't open device node!\n");
	}

    ioctl(fd,4,value);

	close(fd);

	return 0;
}

/*!
 * \brief SoundAdjustMode
 * \param mode: 0: silent mode 1:   2:   
 * \return
 */
int SoundAdjustMode(int mode)
{
	int fd;

	fd = open("/dev/fpga", O_RDWR);
	
    if (fd < 0)
	{
		printf("can't open device node!\n");
	}

    if(mode == 0)
        ioctl(fd,4,SILENT);
    else if(mode == 1)
        ioctl(fd,4,BEEP);					
    else if(mode == 2)
        ioctl(fd,4,ALARM);
    else
    {
	    close(fd);
        return -1;
    }

    close(fd);
	
    return 0;
}

int Sound_Mute(void)
{
    SoundAdjustMode(0);
    return 0;
}


