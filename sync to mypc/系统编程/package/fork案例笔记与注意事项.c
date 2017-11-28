#include "myhead.h"
//创建进程的,成功先执行一遍子进程外的代码（随机不一定老爸先执行）,然后再按照逻辑执行一遍代码

int main()
{
	int status;//回收子进程
	pid_t myid=fork();// 创建子进程
	
	// 判断是否在(0)子进程中
	if(myid==0)
	{printf("我是子进程,pid是:%d,父进程是:%d\n",(int)getpid(),(int)getppid());
		exit(0);//还可以返回其他数字;
		//_exit(0);不刷新IO缓冲区，直接退出而不打印其它东西出来
		}
	else if(myid>0)
	{sleep(2);//将父进程挂起2秒，那么子进程就有足够的时间去先执行
		printf("I am still in father process!\n");}
	//else{printf("create child failed!\n");}
	printf("I am father,prepare exit!\n");
	
	wait(&status);// 回收子进程(阻塞父到子退出后回收子),参数代表子退出时状态信息,不仅仅是数字
	//waitpid(ret,&status,0);
	//参数1:0  参数2:  参数3:WNOHANG
	if(WIFEXITED(status))printf("child exit normal!\n");// 子正常退出宏为真,失败-1,成功子id
	//退出并返回状态信息:WEXITSTATUS(status)
	//printf("第一个回收的是:%d\n",WEXITSTATUS(status));
	
	return 0;//可放在函数里执行到这句后返回到调用它的函数
}
