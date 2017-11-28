#include "myhead.h"

int i;

void *func1(void *arg)
{
	printf("welcome league of heroes\n");
	if(*((int *)arg) == 0)
		pthread_exit("i quit with 0");
	else
		pthread_exit("i quit with 1");	
}

void *func2(void *arg)
{
	printf("welcome\n");
	pthread_exit("i have complete task");
	
}

int main()
{
	int ptr;
	void *str = NULL;
	
	printf("请输入子线程1的退出状态0或1\n");
	scanf("%d",&ptr);
	pthread_t ptid1,ptid2;
	pthread_create(&ptid1,NULL,func1,&ptr);
	pthread_join(ptid1,&str);
	printf("线程1退出时留下的遗言是:%s\n",(char *)str);
	
	pthread_create(&ptid2,NULL,func2,NULL);	
	pthread_cancel(ptid2);
	
	return 0;
}