#include "myhead.h"
/*猜测的答案
I am in father process!
I am in child process!
I am still in father process!
I am father,prepare exit!
*/

/*实际正确答案
I am in father process!
I am in child process!
I am father,prepare exit!
I am still in father process!
I am father,prepare exit!
*/
int main()
{
	pid_t myid;
	printf("I am in father process grandpa ID is:%d!\n",(int)getppid());
	printf("I am in father process ID is:%d!\n",(int)getpid());
	// 创建子进程（生个仔）
	myid=fork();
	// 判断是否在子进程中
	if(myid==0)
	{
		printf("I am in child process! my id is:%d\n",(int)getpid());
		printf("my father is:%d\n",(int)getppid());
	}
	// 处于父进程
	else if(myid>0)
	{
		sleep(2);//将父进程挂起2秒，那么子进程就有足够的时间去先执行
		printf("I am still in father process!\n");
	}
	else
	{
		printf("create child failed!\n");
	}
	printf("I am father,prepare exit!\n");
	return 0;
}
/*
属于父亲的代码：
   printf("I am in father process!\n");
   printf("I am still in father process!\n");
   printf("I am father,prepare exit!\n");
属于儿子的代码：
   printf("I am in child process!\n");
   printf("I am in father process!\n"); 不能复制父亲的，因为在生我之前已经有了
   printf("I am still in father process!\n");不能复制父亲的，老爸独有
   printf("I am father,prepare exit!\n");
*/




