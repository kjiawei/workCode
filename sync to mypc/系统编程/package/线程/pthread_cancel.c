#include "myhead.h"

void *handler(void *arg){//线程处理函数
	printf("123\n");
}

void *fun1(void *str){
	pthread_setcancelstate(PTHREAD_CREATE_DISABLE,NULL);//设置不可取消  DETACHED
	int i = 10;
	while(i){i--;printf("i=%d\n",i);sleep(1);}
	pthread_setcancelstate(PTHREAD_CREATE_ENABLE,NULL);//设置可取消  DETACHED
	pthread_setcanceltype(PTHREAD_CREATE_ASYNCHRONOUS,NULL);//立即取消  DEFERRED(延时:运行到取消点printf-posix定义)
	printf("really cancel!!\n");printf("really cancel!!!\n");
	pthread_exit(NULL);
	//pthread_cleanup_push(handler);//互斥锁时使用
}

int main(void){
	pthread_t pid;//创建线程
	pthread_create(&pid,NULL,fun1,NULL);
	sleep(2);//腾出时间给线程运行一会
	pthread_cancel(pid);//取消线程,给线程发送取消命令
	printf("准备退出\n");
	pthread_join(pid,NULL);//回收线程
	//pthread_cleanup_pop(0);
	return 0;
}