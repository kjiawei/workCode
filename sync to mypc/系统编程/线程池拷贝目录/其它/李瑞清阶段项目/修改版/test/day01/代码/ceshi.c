#include "myhead.h"

int main()
{
	pid_t mypid1,mypid2,mypid3,mypid4;
	int st1,st2,st3,st4;
	
	printf("father process start\n");
	//printf("%d\n",(int)getpid());
	mypid1 = fork();
	
	
	if(mypid1 == 0)
	{
		printf("111111111\n");
		//printf("%d\n",(int)getpid());
		exit(1);
	}
	else
	{
		sleep(1);
		waitpid(mypid1,&st1,WNOHANG);
		printf("child1 exit status is %d\n",WEXITSTATUS(st1));
	}
		
	
	mypid2 = fork();
	if(mypid2 == 0)
	{
		printf("222222222\n");
		exit(2);
	}
	else
	{
		sleep(1);
		waitpid(mypid2,&st2,WNOHANG);
		printf("child2 exit status is %d\n",WEXITSTATUS(st2));
	}
	
	mypid3 = fork();
	if(mypid3 == 0)
	{
		printf("33333333\n");
		exit(3);
	}
	else
	{
		sleep(1);
		waitpid(mypid3,&st3,WNOHANG);
		printf("child3 exit status is %d\n",WEXITSTATUS(st3));
	}
	
	mypid4 = fork();
	if(mypid4 == 0)
	{
		printf("444444444\n");
		//exit(4);
	}
	else
	{
		sleep(1);
		waitpid(mypid4,&st4,WNOHANG);
		printf("child4 exit status is %d\n",WEXITSTATUS(st4));
	}
	

	printf("hahaha\n");
	
	return 0;
}
