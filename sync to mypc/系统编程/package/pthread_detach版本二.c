#include "myhead.h"

void *fun(void *arg)
{
	sleep(1);
	printf("*((pthread_t *)arg) is:%ld\n",*((pthread_t *)arg));
	printf("I am thread start!\n");
	pthread_exit("thread exit");
}
int main()
{
	void *str=NULL;
	pthread_t myid;
	// 创建一个线程，做点事情
	pthread_create(&myid,NULL,fun,(void *)(&myid));
	// 将该线程设置成可以分离
	pthread_detach(myid);
	printf("myid is:%ld\n",myid);
	sleep(2); //腾出足够的时间让子线程先运行
   	pthread_join(myid,&str);// 能否正常回收线程,不能回收，原因是线程被设置成分离的，该代码不起作用了
	printf("thread exit msg is:%s\n",(char *)str);//打印回收信息
	return 0;
}