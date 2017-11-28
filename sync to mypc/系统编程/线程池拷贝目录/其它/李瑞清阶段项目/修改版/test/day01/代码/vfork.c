#include "myhead.h"

int main()
{
	printf("father start!!!\n");
	pid_t myid=vfork();
	if(myid==0)
	{
		sleep(10);//没用，照样是子进程先执行
		printf("I am in child!!!\n");
		exit(0);
	}
	else if(myid>0)
	{
		printf("I am father!\n");
	}
	printf("father exit!!!\n");
	return 0;
}