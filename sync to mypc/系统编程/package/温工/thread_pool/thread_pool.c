#include "thread_pool.h"

void handler(void *arg)				//解锁操作，防止死锁
{

	pthread_mutex_unlock((pthread_mutex_t *)arg);
}

void *routine(void *arg)
{
	thread_pool *pool = (thread_pool *)arg;		//获取线程池结构体
	struct task *p;

	while(1)
	{

		pthread_cleanup_push(handler, (void *)&pool->lock);		//注册cancel信号执行函数
		pthread_mutex_lock(&pool->lock);							//加个互斥锁，排除线程与线程之间的干扰


		while(pool->waiting_tasks == 0 && !pool->shutdown)			//判断当前任务队列有没有任务，并且线程池没有关闭
		{
			pthread_cond_wait(&pool->cond, &pool->lock);			//解锁，睡觉，等待唤醒后再加锁
		}


		if(pool->waiting_tasks == 0 && pool->shutdown == true)		//判断线程池是否退出
		{
			pthread_mutex_unlock(&pool->lock);
			pthread_exit(NULL);										//线程退出
		}


		p = pool->task_list->next;									//取出任务
		pool->task_list->next = p->next;							//将取出的任务从队列中删除
		pool->waiting_tasks--;										//将记录的等待运行的任务数量-1


		pthread_mutex_unlock(&pool->lock);							//解锁
		pthread_cleanup_pop(0);										//将cancel注册函数去除



		/*运行指定的任务，保证任务一定运行完*/
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);		//设置cancel信号响应为关闭
		(p->task)(p->arg);											//运行任务
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);		//设置cancel信号响应为打开

		free(p);													//释放取出来的任务节点
	}

	pthread_exit(NULL);												//这句话暂时没有意义
}

bool init_pool(thread_pool *pool, unsigned int threads_number)			//初始化线程池
{
	
	pthread_mutex_init(&pool->lock, NULL);							//初始化互斥锁
	pthread_cond_init(&pool->cond, NULL);							//初始化条件变量

	pool->shutdown = false;											//把开关设置为false（就是打开）
	pool->task_list = malloc(sizeof(struct task));					//申请头节点
	pool->tids = malloc(sizeof(pthread_t) * MAX_ACTIVE_THREADS);	//申请存放线程id的内存

	if(pool->task_list == NULL || pool->tids == NULL)				//判断申请成功吗
	{
		perror("allocate memory error");
		return false;
	}

	pool->task_list->next = NULL;									//让任务队列的下个节点指向NULL

	pool->waiting_tasks = 0;										//等待运行的任务设置为0
	pool->active_threads = threads_number;							//记录线程池创建的等待线程数

	int i;
	for(i=0; i<pool->active_threads; i++)							//创建线程
	{
		if(pthread_create(&((pool->tids)[i]), NULL,
					routine, (void *)pool) != 0)
		{
			perror("create threads error");
			return false;
		}
	}

	return true;
}

bool add_task(thread_pool *pool,
			void *(*task)(void *arg), void *arg)
{
	struct task *new_task = malloc(sizeof(struct task));
	if(new_task == NULL)
	{
		perror("allocate memory error");
		return false;
	}
	new_task->task = task;
	new_task->arg = arg;
	new_task->next = NULL;


	pthread_mutex_lock(&pool->lock);
	if(pool->waiting_tasks >= MAX_WAITING_TASKS)
	{
		pthread_mutex_unlock(&pool->lock);

		fprintf(stderr, "too many tasks.\n");
		free(new_task);

		return false;
	}
	
	struct task *tmp = pool->task_list;
	while(tmp->next != NULL)
		tmp = tmp->next;

	tmp->next = new_task;
	pool->waiting_tasks++;


	pthread_mutex_unlock(&pool->lock);
	pthread_cond_signal(&pool->cond);

	return true;
}

int add_thread(thread_pool *pool, unsigned additional_threads)
{
	if(additional_threads == 0)
		return 0;

	unsigned total_threads =
		     pool->active_threads + additional_threads;

	int i, actual_increment = 0;
	for(i = pool->active_threads;
	    i < total_threads && i < MAX_ACTIVE_THREADS;
	    i++)
	{
		if(pthread_create(&((pool->tids)[i]),
				NULL, routine, (void *)pool) != 0)
		{
			perror("add threads error");

			if(actual_increment == 0)
				return -1;

			break;
		}
		actual_increment++; 
	}

	pool->active_threads += actual_increment;
	return actual_increment;
}

int remove_thread(thread_pool *pool, unsigned int removing_threads)
{
	if(removing_threads == 0)
		return pool->active_threads;

	int remain_threads = pool->active_threads - removing_threads;
	remain_threads = remain_threads>0 ? remain_threads:1;

	int i;
	for(i=pool->active_threads-1; i>remain_threads-1; i--)
	{
		errno = pthread_cancel(pool->tids[i]);
		if(errno != 0)
			break;
	}

	if(i == pool->active_threads-1)
		return -1;
	else
	{
		pool->active_threads = i+1;
		return i+1;
	}
}

bool destroy_pool(thread_pool *pool)
{

	pool->shutdown = true;
	pthread_cond_broadcast(&pool->cond);

	int i;
	for(i=0; i<pool->active_threads; i++)
	{
		errno = pthread_join(pool->tids[i], NULL);
		if(errno != 0)
		{
			printf("join tids[%d] error: %s\n",
					i, strerror(errno));
		}
		else
			printf("[%u] is joined\n", (unsigned)pool->tids[i]);
		
	}

	free(pool->task_list);
	free(pool->tids);
	free(pool);

	return true;
}
