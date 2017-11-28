#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include<unistd.h>

int main(void)
{

	pid_t myid=fork();
	if(myid>0)
	{
		FILE *myfile;
		printf("I am father ,my id is %d\n",getpid());
		int fd1= open("grep.txt",O_CREAT);
	}
	else if(myid==0)
	{
		printf("I am child,my id is %d\n",getpid());
		execl("./child","./child",NULL);
	}
		else
		{
		//创建进程失败！
		printf("failure!\n");
		
	          }
	          return 0;
}
