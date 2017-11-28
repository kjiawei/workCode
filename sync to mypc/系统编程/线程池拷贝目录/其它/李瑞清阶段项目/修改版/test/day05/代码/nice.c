#include "myhead.h"
void *fun1(void *arg)
{
	nice(5);// 数字小，优先级高
	while(1)
	{
		printf("I am in thread 1!\n");
		sleep(1);
	}
	pthread_exit(NULL);
}
void *fun2(void *arg)
{
	nice(20);
	while(1)
	{
		printf("I am in thread 2!\n");
		sleep(1);
	}
	pthread_exit(NULL);
}
int main()
{
	pthread_t id1;
	pthread_t id2;
	struct sched_param param1;
	struct sched_param param2;
	bzero(&param1,sizeof(struct sched_param));
	bzero(&param2,sizeof(struct sched_param));
	//初始化该结构体
	param1.sched_priority=0; //非实时线程  静态优先级为0
	param2.sched_priority=0; 
	//定义属性变量
	pthread_attr_t  myattr1;
	pthread_attr_t  myattr2;
	//初始化属性变量
	pthread_attr_init(&myattr1);
	pthread_attr_init(&myattr2);
	//调用设置静态优先级的函数
	pthread_attr_setschedparam(&myattr1,&param1);
	pthread_attr_setschedparam(&myattr2,&param2);
	//创建两个线程
	pthread_create(&id1,&myattr1,fun1,NULL);
	pthread_create(&id2,&myattr2,fun2,NULL);
	//回收
	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	//销毁属性
	pthread_attr_destroy(&myattr1);
	pthread_attr_destroy(&myattr2);
	return 0;
}