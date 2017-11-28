#include "myhead.h"
// 父进程创建4个子进程，每个子进程完成不同的任务，然后父进程一个个回收
/*
   
*/
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
	else if(myid>0)// myid>0实际子进程的id
	{
		//sleep(2);
		printf("myid is:%d\n",(int)myid);
		waitpid(myid,&status1,0);
		if(WIFEXITED(status1))
			printf("第一个回收的是:%d\n",WEXITSTATUS(status1));
		
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
			//sleep(2);
			printf("myid is:%d\n",(int)myid);
			waitpid(myid,&status2,0);
			if(WIFEXITED(status2))
				printf("第二个回收的是:%d\n",WEXITSTATUS(status2));
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
				//sleep(2);
				printf("myid is:%d\n",(int)myid);
				waitpid(myid,&status3,0);
				if(WIFEXITED(status3))
				printf("第三个回收的是:%d\n",WEXITSTATUS(status3));
				//接着创建第四个子进程
				pid_t myid=fork();
				if(myid==0)
				{
					printf("我是第四个孩子，我要的做的事情打台球！\n");
					id4=getpid();
					printf("id4 is:%d\n",(int)id4);
					exit(4);
				}
				else if(myid>0)
				{
					//sleep(2);
					printf("myid is:%d\n",(int)myid);
					waitpid(myid,&status4,0);
					if(WIFEXITED(status4))
						printf("第三个回收的是:%d\n",WEXITSTATUS(status4));
				}
				
			}
		}
	}
	return 0;
}


