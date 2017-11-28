#include <stdio.h>
#include <signal.h>

void f(int sig)
{
	int i=5;

	while(i--)
	{
		printf("accept signal = %d\n", sig);
		sleep(1);
	}
	return;
}

int main(void)
{
	int i=10;
	int rtv;

	printf("my PID=%d\n", getpid());
	signal(SIGINT, f);		//声明当接收到SIGINT信号的时候去执行sig_hand_func

	sigset_t set;						//阻塞信号的集合变量

	sigemptyset(&set);					//清空集合变量里面的设置

	sigaddset(&set, SIGINT);			//添加SIGINT信号进这个集合变量
	sigaddset(&set, SIGHUP);			//添加SIGHUP信号进这个集合变量

	rtv = sigismember(&set, SIGINT);	//测试当前SIGINT这个信号有没有成功设置在这个集合变量里面
	if(rtv == 0)
	{
		printf("set SIGINT to sigset failed\n");
		return -1;
	}


	sigprocmask(SIG_BLOCK, &set, NULL);	//开始设置阻塞，阻塞集合变量里面所设置的信号（如果中间收到信号，则将信号晾一边，并没有丢弃信号）
	
	while(i--)
	{
		printf("hello\n");
		sleep(1);
	}

	sigprocmask(SIG_UNBLOCK, &set, NULL);//解开阻塞（解除集合变量里的信号阻塞（开始响应信号））
	

	pause();							//让程序暂停在这里，收到信号后开始往下走

	return 0;
}
