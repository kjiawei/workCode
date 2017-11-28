#include "myhead.h"

int main()
{
	//先创建子进程
	pid_t myid=vfork();
	if(myid==0)
	{
		//调用exec函数族中的一个函数
		execlp("hello","hello",NULL);
		exit(1);
	}	
	else if(myid>0)
	{
		while(1)
		{
			printf("nihao china!\n");
			sleep(1);
		}
	}
	return 0;
}