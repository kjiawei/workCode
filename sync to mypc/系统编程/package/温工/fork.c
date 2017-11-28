#include <stdio.h>	//printf
#include <unistd.h>	//fork,getpid
#include <sys/types.h>	//getpid

int main(void)
{

	pid_t pid;

	int x=0;

	/*
		将所有东西复制一遍，申请进程资源去跑子进程
	
		fork成功则	返回子进程的PID给父进程
					返回0给子进程
			失败则返回-1
	*/
	pid = fork();

	if(pid == -1)
	{
		perror("create child process failed\n");
		return -1;
	}

	if(pid == 0)	//子进程所跑的内容
	{
		printf("in child process\n");
		printf("in child x=%d\n", x);
		printf("pid_t=%d ,myid=%d, in child my parent ID=%d\n" ,pid,getpid(),  getppid());//getpid获取本进程的ID值，getppid获取父进程的ID值
	}

	if(pid > 0)	//父进程所跑的内容
	{
		x=1;
		printf("in parent process\n");
		printf("in parent x=%d\n", x);
		printf("child id =%d,in parent my ID = %d\n", pid, getpid());

		sleep(1);			//如果子进程还没退出，父进程却挂了，子进程的老爸就编程init进程（进程号为1）
	}


	return 0;
}
