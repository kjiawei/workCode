#include "myhead.h"

// 线程的功能函数
void *sub(void *arg)
{
	int *p=(int *)arg;
	printf("arg is:%d\n",*((int *)arg));
	*p=*p+1;
	printf("arg里面的数字加一以后是:%d\n",*p);
	pthread_exit("I have complete project"); //返回给join函数，知道退出情况
}
int main()
{
	pthread_t threadid;
	void *str=NULL;
	int num=48;
	printf("I am in main thread!\n");
	// 创建一个线程
	pthread_create(&threadid,NULL,sub,(void *)(&num));
	// 回收线程
	pthread_join(threadid,&str);
	printf("线程退出时留下的遗言是:%s\n",(char *)str);
	return 0;
}