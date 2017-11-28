#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <errno.h>
#include <pthread.h>

#define MAX_WAITING_TASKS	1000
#define MAX_ACTIVE_THREADS	20

struct task
{
	void *(*task)(const char *pathname, const char *test);			//2
	const char *pathname;											//2
	const char *test;												//2

	struct task *next;
};

typedef struct thread_pool
{
	pthread_mutex_t lock;
	pthread_cond_t  cond;
	struct task *task_list;

	pthread_t *tids;

	unsigned waiting_tasks;
	unsigned active_threads;

	bool shutdown;
}thread_pool;


bool
init_pool(thread_pool *pool,
          unsigned int threads_number);

bool
add_task(thread_pool *pool,
         void *(*task)(const char *, const char *),
         const char *pathname, const char *test);				//3

int 
add_thread(thread_pool *pool,
           unsigned int additional_threads_number);

int 
remove_thread(thread_pool *pool,
              unsigned int removing_threads_number);

bool destroy_pool(thread_pool *pool);
void *routine(void *arg);

#endif
