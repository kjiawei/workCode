#include "myhead.h"
int i=0;
//定义一把锁
pthread_mutex_t  mutex;
void *fun1(void *arg)
{
	while(1)
	{
		//线程一上锁操作
		pthread_mutex_lock(&mutex); //独占性
		i++;
		printf("thread 1 i is:%d\n",i);
		sleep(1);
	
		pthread_mutex_unlock(&mutex);
		usleep(2000);  //防止线程一刚解完锁，又无耻地上锁
	}
	pthread_exit(NULL);
}
void *fun2(void *arg)
{
	
	while(1)
	{
		usleep(2000);
		//线程二上锁操作
		pthread_mutex_lock(&mutex); 
		i--;
		printf("thread 2 i is:%d\n",i);
		sleep(1);
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}
int main()
{
	pthread_t id1;
	pthread_t id2;
	
	//初始化锁
	pthread_mutex_init(&mutex,NULL);
	//创建两个线程
	pthread_create(&id1,NULL,fun1,NULL);
	pthread_create(&id2,NULL,fun2,NULL);
	
	//
	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	return 0;
}