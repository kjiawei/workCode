#include "myhead.h"

void func(int sig,siginfo_t *si,void *p)
{
	printf("sig is:%d\n",sig);
	printf("%s\n",(char *)si->si_ptr);
	//printf("%d\n",si->si_int);
	
}

int main()
{
	pid_t mypid;
	union sigval value;
	struct sigaction act;
	bzero(&act,sizeof(act));
	char ptr[20]="hello,world";
	
	value.sival_ptr = ptr;
	//value.sival_int = 123;
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = func;
	
	mypid = fork();

	if(mypid > 0)
	{
		sleep(1);
		sigqueue(mypid,SIGINT,value);	
		
	}
	else
	{
		printf("%d\n",getpid());
		sigaction(SIGINT,&act,NULL);
		pause();
		exit(1);				
	}
	
	waitpid(mypid,NULL,0);
	return 0;
}
