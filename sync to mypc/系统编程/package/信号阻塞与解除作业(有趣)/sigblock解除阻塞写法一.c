#include "myhead.h"
void dongzuo(int sig)
{
	printf("我正在等待SIGINT的到来！\n");
}
int main()
{
	int i=0;
	// 定义阻塞信号集
	sigset_t sigblock;
	sigset_t oldblock;
	// 初始化该集合
	// 清空集合
	sigemptyset(&sigblock);
	// 往集合中添加你想要阻塞的信号
	sigaddset(&sigblock,SIGINT);
	sigaddset(&sigblock,SIGQUIT);
	sigaddset(&sigblock,SIGKILL); // 这句代码是废话，SIGKILL不能被忽略，加进入了也没有用
	signal(SIGINT,dongzuo);
	//调用函数让集合生效
	sigprocmask(SIG_BLOCK,&sigblock,&oldblock);
	printf("I am in main!\n");
	for(; i<15; i++)
	{
		
		printf("NOW i is:%d\n",i);
		sleep(1);
	}
	// 解除对信号的阻塞
	sigprocmask(SIG_UNBLOCK,&sigblock,NULL);
	pause();
	return 0;
}