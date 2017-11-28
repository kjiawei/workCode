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
	// 定义属性结构体变量
	pthread_attr_t myaddr;
	// 初始化该变量
	pthread_attr_init(&myaddr);
	// 设置分离属性   线程可以分离
	pthread_attr_setdetachstate(&myaddr,PTHREAD_CREATE_DETACHED);
	// 创建一个线程，做点事情，让设置的属性生效
	pthread_create(&myid,&myaddr,fun,NULL);
   	pthread_join(myid,&str);// 能否正常回收线程,不能回收，原因是线程被设置成分离的，该代码不起作用了
	//打印回收信息
	printf("thread exit msg is:%s\n",(char *)str);
	//线程属性的销毁
	pthread_attr_destroy(&myaddr);
	return 0;
}