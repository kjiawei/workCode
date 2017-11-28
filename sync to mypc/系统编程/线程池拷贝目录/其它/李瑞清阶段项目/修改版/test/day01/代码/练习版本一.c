#include "myhead.h"
// 父进程创建4个子进程，每个子进程完成不同的任务，然后父进程一个个回收
int main()
{
	int status1,status2,status3,status4;
	int i;
	pid_t id1,id2,id3,id4;
	printf("father start!!!\n");
	// 准备创建第一个子进程
	pid_t myid=fork();
	if(myid==0)
	{
		printf("我是第一个孩子，我要的做的事情踢足球！\n");
		id1=getpid();
		printf("id1 is:%d\n",(int)id1);
		exit(1);
	}
	else if(myid>0)
	{
		
		//接着创建第二个子进程
		pid_t myid=fork();
		if(myid==0)
		{
			printf("我是第二个孩子，我要的做的事情打篮球！\n");
			id2=getpid();
			printf("id2 is:%d\n",(int)id2);
			exit(2);
		}
		else if(myid>0)
		{
			//接着创建第三个子进程
			pid_t myid=fork();
			if(myid==0)
			{
				printf("我是第三个孩子，我要的做的事情打网球！\n");
				id3=getpid();
				printf("id3 is:%d\n",(int)id3);
				exit(3);
			}
			else if(myid>0)
			{
				//接着创建第四个子进程
				pid_t myid=fork();
				if(myid==0)
				{
					printf("我是第四个孩子，我要的做的事情打台球！\n");
					id4=getpid();
					printf("id4 is:%d\n",(int)id4);
					exit(4);
				}
				
			}
		}
	}
	sleep(2);
	for(i=0; i<4; i++)
	{
		waitpid(0,&status1,WNOHANG);  
		// 使用wait()更加简单
		if(WIFEXITED(status1))
			printf("第一个回收的是:%d\n",WEXITSTATUS(status1));
	}
	
	return 0;
}


