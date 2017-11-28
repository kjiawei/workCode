#include "myhead.h"

int i;
void *sub(void *arg)// 线程的功能函数
{
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);//线程不可取消
	int *p=(int *)arg;*p=*p+1;
	//printf("thread id is:%d\n",(int)pthread_self());//进程getpid
	//printf("arg is:%d\n",*((int *)arg));
	printf("arg里面的数字加一以后是:%d\n",*p);
	for(; i<10; i++){printf("thread i is:%d\n",i);sleep(1);}
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
	while(1){if(i==6){pthread_cancel(threadid);break;}}
	pthread_join(threadid,&str);// 回收线程
	printf("线程退出时留下的遗言是:%s\n",(char *)str);
	return 0;
}