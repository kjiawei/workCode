#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFONAME  "myfifo"

int main(void)
{
	int rtv;

	if(access(FIFONAME, F_OK))
	{

		rtv = mkfifo(FIFONAME, 0777);
		if(rtv == -1)
		{
			perror("create fifo file failed\n");
			return -1;
		}
	
	}

	int fd;

	fd = open(FIFONAME, O_RDONLY);
	if(fd == -1)
	{
		perror("open fifo file failed\n");
		return -1;
	}

	char buf[10];
	read(fd, buf, sizeof(buf));	
	printf("read buf=%s\n", buf);

	close(fd);


	return 0;
}
