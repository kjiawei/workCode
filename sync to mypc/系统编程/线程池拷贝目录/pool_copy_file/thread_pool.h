#ifndef _THREAD_POOL_H_
#def _THREAD_POOL_H_

#include "myhead.h"

#define MAX_TASK_NUM	1000
#define MAX_ACTIVE_THREADS	20

struct task
{
	void *(*handler)(void *arg);//代表任务的函数指针
	void *arg;//参数
	struct task *next;
};

typedef struct thread_pool
{
	pthread_t *id;//线程的id号
	pthread_mutex_t lock;//锁头
	pthread_cond_t cond;//条件变量
	unsigned task_num;//有效任务
	unsigned active_threads;//活跃线程
	bool pool_state;//线程池是否还在
	struct task *head;//任务链表头
}thread_pool;

void *closemutex(void *arg);//解锁
bool thread_pool_destroy(struct thread_pool *mypool);//线程池销毁
int del_thread(struct thread_pool *mypool,unsigned int delnum);//删除线程
int thread_add(struct thread_pool *mypool,unsigned int additional);//新增线程
void *thread_fun(void *arg,struct thread_pool *mypool);//线程处理函数
bool pool_init(struct thread_pool *mypool,unsigned int threadnum);//线程池初始化
int task_add(struct thread_pool *mypool,void *(*p)(void *arg),void *argument);//添加任务
