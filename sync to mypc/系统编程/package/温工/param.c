#include <stdio.h>
#include <pthread.h>

void *func(void *arg)
{
	//nice(0);
	unsigned long i;

	while(1)
	{
		i=100000000;
		while(i--);

		fprintf(stderr, "%s ", (char *)arg);
	}

}

int main(void)
{
	pthread_t tid1,tid2;

	pthread_attr_t attr1,attr2;

	
	/*初始化结构体*/
	pthread_attr_init(&attr1);
	pthread_attr_init(&attr2);

	/*继承调度设置，当前自行设置优先级，PTHREAD_EXPLICIT_SCHED:自行设置，PTHREAD_INHERIT_SCHED：系统默认设置*/
	pthread_attr_setinheritsched(&attr1, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setinheritsched(&attr2, PTHREAD_EXPLICIT_SCHED);

	
	/*设置调度机制，SCHED_RR：轮寻执行，SCHED_FIFO：强占执行， SCHED_OTHER：系统默认执行 */
	pthread_attr_setschedpolicy(&attr1, SCHED_RR);
	pthread_attr_setschedpolicy(&attr2, SCHED_RR);

	
	/*静态优先级结构体*/
	struct sched_param param1, param2;

	param1.sched_priority = 2;		//设置静态优先级
	param2.sched_priority = 3;

	int rtv;
	
	/*设置静态优先级进入线程属性*/
	rtv = pthread_attr_setschedparam(&attr1, &param1);
	if(rtv != 0)
		return;
	rtv = pthread_attr_setschedparam(&attr2, &param2);
	if(rtv != 0)
		return;

	pthread_create(&tid1, &attr1, func, (void *)"1");

	sleep(1);
	pthread_create(&tid2, &attr2, func, (void *)"2");


	pause();

	return 0;
}
