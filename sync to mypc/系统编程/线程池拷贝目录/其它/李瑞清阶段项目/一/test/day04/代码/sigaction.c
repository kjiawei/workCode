#include "myhead.h"
// 信号的响应函数
void fun(int sig,siginfo_t *val,void *arg)  
{
	printf("我现在捕捉到的信号是:%d\n",sig);
	printf("siginfo_t结构体里面我现在捕捉到的信号是:%d\n",val->si_signo);
	//printf("我现在捕捉到的额外数据是:%d\n",val->si_int);  //按照官方POSIX标准来写的
	printf("我嵌套的现在捕捉到的额外数据是:%d\n",val->_sifields._rt.si_sigval.sival_int);//按照目前我们使用的ubuntu里面定义的头文件写的
	
}                    
int main()
{
	struct sigaction newaction;
	bzero(&newaction,sizeof(newaction));
	newaction.sa_flags=SA_SIGINFO;
	newaction.sa_sigaction=fun;
	printf("我的进程id是:%d\n",getpid());
	// 使用sigaction()函数来捕捉信号
	sigaction(SIGINT,&newaction,NULL);
	pause();
	return 0;
}