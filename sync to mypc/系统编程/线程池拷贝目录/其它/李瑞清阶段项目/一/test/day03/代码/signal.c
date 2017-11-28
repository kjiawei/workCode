#include "myhead.h"
// 定义收到SIGINT这个信号以后，进程需要执行的动作
void dongzuo(int sig)
{
	printf("我真的收到了SIGINT这个信号，你不相信我答应编号给你看 %d\n",sig);
}

int main()
{
	printf("hello pid is:%d\n",(int)getpid());
	// 调用signal这个函数，当我收到SIGINT这个信号的时候不退出(默认是要退出的)
	signal(SIGINT,SIG_IGN);
	while(1)
	{
		printf("hello world!\n");
		sleep(1);
	}
	pause();// 等待信号的到来
	return 0;
}