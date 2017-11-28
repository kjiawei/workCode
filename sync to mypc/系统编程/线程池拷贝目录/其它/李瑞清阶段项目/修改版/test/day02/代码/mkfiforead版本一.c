#include "myhead.h"

int main()
{
	char buf[6];
	// 创建有名管道
	/* int ret=mkfifo("/home/gec/mypipe",0777);
	if(ret==-1)
	{
		perror("create fifo failed!\n");
		return -1;
	} */
	// 通过上一步生成的管道文件来实现不同进程之间的通信
	int fd=open("/home/gec/mypipe",O_RDONLY);
	if(fd==-1)
	{
		perror("open fifo failed!\n");
		return -1;
	}
	read(fd,buf,6);
	printf("有个进程给我发送的信息是：%s\n",buf);
	close(fd);
	system("rm /home/gec/mypipe");
	return 0;
}