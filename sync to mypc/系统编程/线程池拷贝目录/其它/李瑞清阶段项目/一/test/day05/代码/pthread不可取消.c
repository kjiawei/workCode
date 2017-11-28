#include "myhead.h"
int i;
// 线程的功能函数
void *sub(void *arg)
{
	//pthread_setcanceltype(int type, int *oldtype);
	// 设置该线程不可以被取消
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
	int *p=(int *)arg;
	printf("thread id is:%d\n",(int)pthread_self());
	printf("arg is:%d\n",*((int *)arg));
	*p=*p+1;
	printf("arg里面的数字加一以后是:%d\n",*p);
	for(; i<10; i++)
	{
		printf("thread i is:%d\n",i);
		sleep(1);
	}
	
	printf("hehehfh\n");
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
	printf("create thread id is:%d\n",(int)threadid);
	while(1)
	{
		if(i==6)
		{
			pthread_cancel(threadid);
			break;
		}
		
	}
	
	// 回收线程
	pthread_join(threadid,&str);
	printf("线程退出时留下的遗言是:%s\n",(char *)str);
	return 0;
}