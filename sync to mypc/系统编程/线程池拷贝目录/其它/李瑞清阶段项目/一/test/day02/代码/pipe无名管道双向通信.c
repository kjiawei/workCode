#include "myhead.h"
// 无名管道实现父子进程间的双向聊天
int main()
{
	int fd1[2];
	int fd2[2];
	int ret;
	char sbuf[50];
	char rbuf[50];
	char sbuf1[50];
	char rbuf1[50];
	bzero(sbuf,50);
	bzero(rbuf,50);
	bzero(sbuf1,50);
	bzero(rbuf1,50);
	// 创建两个无名管道
	ret=pipe(fd1);
	if(ret==-1)
	{
		perror("create no name pipe!\n");
		return -1;
	}
	ret=pipe(fd2);
	if(ret==-1)
	{
		perror("create no name pipe!\n");
		return -1;
	}
	pid_t myid=fork();
	if(myid==0)
	{
		while(1)
		{
			printf("please input the message you want to send to your father!\n");
			fgets(sbuf,50,stdin);
			write(fd1[1],sbuf,50);
			read(fd2[0],rbuf1,50);
			printf("the message I recv from father is:%s\n",rbuf1);
		}
		
		exit(0);
	}
	else if(myid>0)
	{
		while(1)
		{
			read(fd1[0],rbuf,50);
			printf("the message I recv from son is:%s\n",rbuf);
			printf("please input the message you want to send to your son!\n");
			fgets(sbuf1,50,stdin);
			write(fd2[1],sbuf1,50);
		}
	}
	// 收尾
	close(fd1[0]);
	close(fd1[1]);
	printf("111111\n"); // 由于是死循环，我们要退出程序目前就只能用ctrl+c强行退出，但是后面的收尾工作没有执行，怎么办？？
	close(fd2[0]);
	close(fd2[1]);
	wait(NULL);
	return 0;
}