#include "myhead.h"

int i,j;

void *f1(void *arg)//线程1
{
	int n = *((int *)arg);
	printf("设置的秒数为：n = %d\n", n);
	
	for(i = 1; i <= n; i++)
	{
		sleep(1);
		printf("I'm thread1 help!!!  %d...\n", i);
		
	
	}
	
	pthread_exit("thread1 gone!");
}

void *f2()//线程2
{
	usleep(100);
	for(j = 1; j <= 10; j++)
	{
		printf("(%d) I'm thread2, my ID is %d\n", j, (int)pthread_self());
		
		sleep(1);
	}
	printf("thread2 is gone!\n");
	
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
}

int main()
{
	pthread_t tid1, tid2;
	
	printf("你想线程1运行几秒后退出： \n");
	scanf("%d", &i); //5
	
	pthread_create(&tid1, NULL, f1, (void *)&i);
	pthread_create(&tid2, NULL, f2, NULL);
	
	void *buf1;
	if(i < 8)
	{
		pthread_join(tid1, &buf1);
		printf("%s\n", (char *)buf1);
		
		while(1)
		{	
			if(j == 8)
			{	
				pthread_cancel(tid2);
				break;
			}
		}
	
		pthread_join(tid2, NULL);
			
	}
	
	else
	{
		while(1)
		{	
			if(j == 8)
			{	
				pthread_cancel(tid2);
				break;
			}
		}
		pthread_join(tid1, &buf1);
		printf("%s\n", (char *)buf1);
		pthread_join(tid2, NULL);
		
	}
	
	//pthread_join(tid1, &buf1);
	//printf("%s\n", (char *)buf1);
	
	/*
	while(1)
	{	
		if(j == 8)
		{	
			pthread_cancel(tid2);
			break;
		}
	}
	*/
	
	//void *buf1;
	//pthread_join(tid1, &buf1);
	//printf("%s\n", (char *)buf1);
	
	//pthread_join(tid2, NULL);
	
	
	return 0;
}