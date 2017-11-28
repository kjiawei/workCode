#include "myhead.h"

int main()
{
	int fd[2];
	int ret;
	char buf[50];
	bzero(buf,50);
	// 创建无名管道
	ret=pipe(fd);
	if(ret==-1)
	{
		perror("create no name pipe!\n");
		return -1;
	}
	pid_t myid=fork();
	if(myid==0)
	{
		write(fd[1],"father I love you!",50);
		exit(0);
	}
	else if(myid>0)
	{
		read(fd[0],buf,50);
		printf("message from my son is:%s\n",buf);
	}
	return 0;
}