#include "thread_pool.h"

void solve(void *arg)
{
	pthread_mutex_unlock((pthread_mutex_t *)arg);
}

void *thread_fun(void *arg)//线程功能函数
{
	thread_pool pool = (thread_pool)arg;
	task p;
	while(1)
	{	//防止出现死锁
		pthread_cleanup_push(solve,&pool->lock);
		pthread_mutex_lock(&pool->lock);
		//判断线程池是否有任务且线程池是否关闭
		while(pool->effective_task_num==0 && pool->pool_state == true)
		{
			pthread_cond_wait(&pool->cond,&pool->lock);
		}
		
		if(pool->effective_task_num==0 && pool->pool_state == false)
		{
			pthread_mutex_unlock(&pool->lock);
			pthread_exit(NULL);
		}
		//取出任务链表中的任务
		p = pool->task_list->next;
		pool->task_list->next = p->next;
		p->next = NULL;
		pool->effective_task_num--;
		//解锁并弹栈
		pthread_mutex_unlock(&pool->lock);
		pthread_cleanup_pop(0);
		//执行任务且执行期间设置线程不能取消
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
		(p->handler)(p->arg);
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
		//释放取出来的任务节点
		free(p);
	}
	
	pthread_exit(NULL);	
}

int pool_init(thread_pool mypool,int thread_num)//初始化线程池
{
	int i;
	
	pthread_mutex_init(&mypool->lock,NULL);
	pthread_cond_init(&mypool->cond,NULL);
	
	mypool->task_list = malloc(sizeof(task_node));
	mypool->tid = malloc(20*sizeof(pthread_t));
	
	mypool->task_list->next = NULL;
	
	mypool->effective_task_num = 0;
	mypool->active_thread_num = thread_num;
	mypool->pool_state = true;
	for(i = 0;i < thread_num;i++)
	{
		if(pthread_create(&(mypool->tid[i]),NULL,thread_fun,mypool)!=0)
		{
			perror("create thread failed\n");
			exit(1);
		}
		//printf("%lu\n",mypool->tid[i]);
	}
	
	return 0;
}

void add_task(thread_pool mypool,void *(*handler)(void *),void *arg)//添加任务
{	
	task new_task = malloc(sizeof(task_node));
	new_task->handler = handler;
	new_task->arg = arg;
	new_task->next = NULL;
	
	pthread_mutex_lock(&mypool->lock);
	
	task tmp = mypool->task_list;
	while(tmp->next != NULL)
		tmp = tmp->next;
	
	tmp->next = new_task;
	
	mypool->effective_task_num++;
	pthread_mutex_unlock(&mypool->lock);
	
	pthread_cond_broadcast(&mypool->cond);	
}

void add_thread(thread_pool mypool,int addnum)
{
	int i;
	for(i = mypool->active_thread_num;i < mypool->active_thread_num + addnum;i++)
	{
		if(pthread_create(&((mypool->tid)[i]),NULL,thread_fun,mypool)!=0)
		{
			perror("create thread failed\n");
			exit(1);
		}
	}
	
	mypool->active_thread_num += addnum;	
}

void del_thread(thread_pool mypool,int delnum)
{
	int i;	
	for(i = 0;i < delnum;i++)
	{
		if(pthread_cancel(mypool->tid[i]) != 0)
			break;
	}
}

void threadpool_destroy(thread_pool mypool)
{
	int i;
	mypool->pool_state = false;
	pthread_cond_broadcast(&mypool->cond);
	
	for(i = 0;i < mypool->active_thread_num;i++)
	{	
		if(pthread_join(mypool->tid[i],NULL) != 0)
		{
			perror("join thread failed\n");
			exit(1);
		}
			
	}
	pthread_mutex_destroy(&mypool->lock);
	pthread_cond_destroy(&mypool->cond);
	free(mypool->task_list);
	free(mypool->tid);
	free(mypool);
}




