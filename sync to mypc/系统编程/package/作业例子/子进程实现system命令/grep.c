#include "myhead.h"

int main(void)
{
	pid_t myid=fork();
	if(myid>0)
	{
		FILE *myfile;
		printf("I am father ,my id is %d\n",getpid());
		int fd1= open("child.txt",O_CREAT);
	}
	else if(myid==0)
	{
		printf("I am child,my id is %d\n",getpid());
		execl("./child","./child",NULL);
	}
	else{printf("failure!\n");}//创建进程失败！
	return 0;
}
