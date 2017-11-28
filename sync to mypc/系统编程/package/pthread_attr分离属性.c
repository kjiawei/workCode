#include "myhead.h"

void *fun(void *arg)
{
	printf("I am thread start!\n");
	pthread_exit("thread exit");
}
int main()
{
	void *str=NULL;
	pthread_t myid;
	pthread_attr_t myaddr;//属性结构体
	
	pthread_attr_init(&myaddr);//初始化
	pthread_attr_setdetachstate(&myaddr,PTHREAD_CREATE_DETACHED);//设置分离属性:自我回收
	
	pthread_create(&myid,&myaddr,fun,NULL);//创建线程,生效属性
   	pthread_join(myid,&str);//不能回收线程,设成分离后该代码不起作用
	printf("thread exit msg is:%s\n",(char *)str);//打印回收信息
	pthread_attr_destroy(&myaddr);//线程属性销毁
	return 0;
}