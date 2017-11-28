#include "myhead.h"

int i = 100;

//线程的功能函数
void *sub(void *arg){
	int *p=(int *)arg;
	//printf("arg is:%d\n",*((int *)arg));//测试传输数据
	 while(1){
		i--;
		printf("thread i is:%d\n",i);
		sleep(1);
	} 
	//线程的退出
	pthread_exit("I have complete project"); //返回给join函数，知道退出情况
} 
int main(void){
	void *str=NULL;//线程退出留言,接收exit函数的字符串
	printf("I am in main thread!\n");
	
	//线程创建 
	pthread_t thread_id;
	pthread_create(&thread_id,NULL,sub,NULL);
	
	//主线程对i加一
	 while(1){
		i++;
		printf("main i is:%d\n",i);
		sleep(1);
	} 
	
	//线程回收
	//pthread_join(thread_id,&str);
	//printf("thread exit is :%s\n",(char *)str);//打印退出信息
	return 0;
}