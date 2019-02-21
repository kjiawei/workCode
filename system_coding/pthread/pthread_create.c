#include <stdio.h>
#include <pthread.h>


//线程执行函数
void *thread_start(void *arg)
{
	while(1)
	{
		printf("hi man!\n");
		sleep(1);
	}
}

int main(void)
{
	//线程识别变量
	pthread_t pthread_id;
	int ret;
	
	//线程创建
	ret = pthread_create(&pthread_id,NULL,&thread_start,NULL);
	if(ret !=0)
	{
		printf("pthread_create failed!\n");
		return;
	}
	while(1)
	{
		printf("hellowrold!\n");
		sleep(1);
	}
	
	return 0;
}