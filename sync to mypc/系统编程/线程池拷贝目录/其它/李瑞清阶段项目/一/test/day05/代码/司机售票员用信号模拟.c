	用信号模拟司机售票员：创建子进程代表售票员，父进程代表司机，
		1：售票员捕捉SIGINT(代表开车)，发SIGUSR1 给司机，司机捕捉到SIGUSR1 信

号后打印“move to next station”
		2: 售票员捕捉SIGQUIT(代表靠站)，发SIGUSR2 给司机，司机捕捉到SIGUSR2 信

号后打印“stop the bus”
		3: 司机捕获到SIGTSTP（车到总站），发SIGUSR1给售票员，
		   售票员捕捉到SIGUSR1信号后打印“all get off the bus”，全部进程结束	
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

pid_t pid ;
int flag = 1;  //开关  控制父进程中的死循环
void sig1(int sig)
{
	printf("move to next station  \n");
}

void sig2(int sig)
{
	printf("stop the bus  \n");
}

void stop(int sig)
{
	printf("all get off the bus  \n");
	exit (0);
}

void quit(int sig)
{
	if(sig==SIGINT)	
		kill(getppid(),SIGUSR1);
	else if(sig==SIGQUIT)
		kill(getppid(),SIGUSR2);
//	printf("catch %d sig in child\n",sig);
}

void end(int sig)
{
	kill(pid,SIGUSR1);
	flag = 0;
}

int main(void)
{
	pid = fork();
	
	if(pid < 0)
	{
		printf("failed! \n");
	}
	else if(pid == 0)// 售票员
	{
		signal(SIGTSTP,SIG_IGN);
		signal(SIGINT,quit);
		signal(SIGQUIT,quit);
		signal(SIGUSR1,	stop);
		while(1){			
			while(-1 != pause());
		}
		
	}
	else//司机 
	{
		signal(SIGTSTP,end);
		signal(SIGQUIT,SIG_IGN);
		signal(SIGINT,SIG_IGN);
		signal(SIGUSR1,sig1);
		signal(SIGUSR2,sig2);
		while(flag){
			while(-1 != pause());
		}	
		wait(NULL);
		return 0;
	}
}