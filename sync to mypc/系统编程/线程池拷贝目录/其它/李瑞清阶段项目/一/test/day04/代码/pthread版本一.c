#include "myhead.h"
int i=100;
// 线程的功能函数
void *sub(void *arg)
{
	while(1)
	{
		i--;
		printf("thread i is:%d\n",i);
		sleep(1);
	}
}
int main()
{
	pthread_t threadid;
	printf("I am in main thread!\n");
	// 创建一个线程
	pthread_create(&threadid,NULL,sub,NULL);
	// 主线程中对i加一
	while(1)
	{
		i++;
		printf("main i is:%d\n",i);
		sleep(1);
	}
	return 0;
	
}