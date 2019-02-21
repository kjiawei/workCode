#include <stdio.h>
#include <pthread.h>


int flag;
void *fun1(void *arg)
{
	flag =1;
	while(flag)
	{
		printf("%s\n",(char *)arg);
		sleep(1);
	}
	pthread_exit(NULL);
}


void *fun2(void *arg)
{

	while(1)
	{
		printf("%s\n",(char *)arg);
		sleep(1);
	}
}


int main()
{

	pthread_t pid1,pid2;
	int ret = pthread_create(&pid1,NULL,fun1,"thread 1");
	if(ret != 0)
	{
		perror("pthread:");
		return -1;
	}
	ret = pthread_create(&pid2,NULL,fun2,"thread 2");
	if(ret != 0)
	{
		perror("pthread:");
		return -1;
	}

	int x,cnt=0;
	while(cnt < 2)
	{
		scanf("%d",&x);
		if(x ==1)
		{
			flag = 0;	 
			cnt++;
		}

		else if(x ==2)
		{
			pthread_cancel(pid2);
			cnt++;
		}
	}
	

	pthread_join(pid1,NULL);
	pthread_join(pid2,NULL);

	return 0;
}
