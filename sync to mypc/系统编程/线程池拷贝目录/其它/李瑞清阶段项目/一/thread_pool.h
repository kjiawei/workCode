#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include "myhead.h"

typedef struct task     //任务链表节点
{
	void *(*handler)(void *);
	void *arg;
	struct task *next;	
}task_node,*task;

typedef struct thread_pool  //线程池
{
	pthread_mutex_t lock;
	pthread_cond_t cond;
	task task_list;
	pthread_t *tid;
	
	int effective_task_num;
	int active_thread_num;	
	bool pool_state;
	
}*thread_pool;

extern void solve(void *arg);
extern void *thread_fun(void *arg);
extern int pool_init(thread_pool mypool,int thread_num);
extern void add_task(thread_pool mypool,void *(*handler)(void *),void *arg);
extern void add_thread(thread_pool mypool,int addnum);
extern void del_thread(thread_pool mypool,int delnum);
extern void threadpool_destroy(thread_pool mypool);

#endif

