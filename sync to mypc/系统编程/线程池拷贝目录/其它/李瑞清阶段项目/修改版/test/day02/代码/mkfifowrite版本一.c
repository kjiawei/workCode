#include "myhead.h"

int main()
{
	// 创建有名管道
	int ret=mkfifo("/home/gec/mypipe",0777);
	if(ret==-1)
	{
		perror("create fifo failed!\n");
		return -1;
	}
	// 通过上一步生成的管道文件来实现不同进程之间的通信
	int fd=open("/home/gec/mypipe",O_WRONLY);
	if(fd==-1)
	{
		perror("open fifo failed!\n");
		return -1;
	}
	write(fd,"hello",5);
	close(fd);
	return 0;
}