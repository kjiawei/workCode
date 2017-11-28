#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mt;

static void cleanuptest(void *arg)
{
	printf("in clean up\n");

	pthread_mutex_unlock(&mt);
	//return;
}

void *start_routine(void *arg)
{
	unsigned long i=1000000000;
	
	printf("the thread is %s\n", (char *)arg);

	pthread_cleanup_push(cleanuptest, (void *)1);		//注册cancel信号接收后的处理函数，防止死锁

	pthread_mutex_lock(&mt);
	
	printf("begin\n");

	while(i--);	//2-3s

	fprintf(stdout, "%s\n",(char *)arg);
	
	printf("end\n");

	pthread_mutex_unlock(&mt);

	pthread_cleanup_pop(0);							//清除栈内cancel处理函数，0则代表不执行

	printf("test\n");

	pthread_exit(NULL);
}


int main(void)
{

	pthread_t tid1, tid2;
	int rtv;

	pthread_mutex_init(&mt, NULL);		

	pthread_create(&tid1, NULL, start_routine, (void *)"A");
	usleep(100000);	//0.1s


	rtv = pthread_cancel(tid1);				//发送一个cancel信号给tid1的线程
	if(rtv == 0)
		printf("send success\n");


	pthread_create(&tid2, NULL, start_routine, (void *)"B");
	

	
	pthread_exit(NULL);
	return 0;
}
