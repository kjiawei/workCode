#include "myhead.h"
//线程取消属性
int i;

// 线程的功能函数
void *sub(void *arg)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);//线程可被取消
	//pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);//不能取消
	//pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);//适当时取消
	
	int *p=(int *)arg;*p=*p+1;
	//printf("thread id is:%d\n",(int)pthread_self());
	//printf("arg is:%d\n",*((int *)arg));
	
	printf("arg里面的数字加一以后是:%d\n",*p);
	while(1){printf("thread i is:%d\n",i);sleep(1);}
	printf("thread is cancel\n");
	pthread_exit("I have complete project"); //返回给join函数，知道退出情况
}
int main()
{
	pthread_t threadid;
	void *str=NULL;
	int num=48;
	printf("I am in main thread!\n");
	pthread_create(&threadid,NULL,sub,(void *)(&num));// 创建线程
	printf("create thread id is:%d\n",(int)threadid);
	sleep(4);
	pthread_cancel(threadid);//发送取消线程命令
	pthread_join(threadid,&str);// 回收线程
	//printf("线程退出时留下的遗言是:%s\n",(char *)str);
	return 0;
}