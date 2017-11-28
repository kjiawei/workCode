#include "myhead.h"
/*创建一个线程，传递一个整数给该线程，该线程将整数的值加一之后
退出线程，退出线程的线程必须告诉主线程"I have complete project"
 ,主线程回收子线程的时候打印出这句话*/
//int i = 100;
void *add_exit(void *arg){
	int *p = (int *)arg;
	printf("arg is:%d\n",*((int *)arg));
	//加一后退出
	*p = *p + 1;//*((int *)arg)+=1;
	printf("arg add 1 is:%d\n",*p);
	pthread_exit("I have complete project");//存放线程退出时的信息（程序员写的）
}

int main(void){
	int num = 48;
	void *str = NULL;
	pthread_t pthread_id;
	pthread_create(&pthread_id,NULL,add_exit,(void *)(&num));
	//回收线程
	pthread_join(pthread_id,&str);//存放线程退出时返回的信息
	printf("thread exit is :%s\n",(char *)str);
	return 0;
}