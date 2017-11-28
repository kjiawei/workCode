#include "myhead.h"
void dongzuo(int sig)
{
	printf("我正在等待SIGINT的到来！\n");
}
int main()
{
	// 定义阻塞信号集
	sigset_t sigblock;
	sigset_t oldblock;
	// 初始化该集合
	// 清空集合
	sigemptyset(&sigblock);
	// 往集合中添加你想要阻塞的信号
	sigaddset(&sigblock,SIGINT);
	sigaddset(&sigblock,SIGQUIT);
	sigaddset(&sigblock,SIGKILL);
	signal(SIGINT,SIG_DFL);
	//调用函数让集合生效
	sigprocmask(SIG_BLOCK,&sigblock,&oldblock);
	printf("I am in main!\n");
	for()
	{
		
		
	}
	pause();
	return 0;
}