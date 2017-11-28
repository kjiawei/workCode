#include "myhead.h"
void *fun1(void *arg)
{
	while(1)
	{
		printf("I am in thread 1!\n");
		sleep(1);
	}
	pthread_exit(NULL);
}
void *fun2(void *arg)
{
	while(1)
	{
		printf("I am in thread 2!\n");
		sleep(1);
	}
	pthread_exit(NULL);
}
void *fun3(void *arg)
{
	while(1)
	{
		printf("I am in thread 3!\n");
		sleep(1);
	}
	pthread_exit(NULL);
}
int main()
{
	pthread_t id1;
	pthread_t id2;
	pthread_t id3;
	//创建两个线程
	pthread_create(&id1,NULL,fun1,NULL);
	pthread_create(&id2,NULL,fun2,NULL);
	pthread_create(&id3,NULL,fun3,NULL);
	//回收
	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	pthread_join(id3,NULL);
	return 0;
}