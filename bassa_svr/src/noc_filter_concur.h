#ifndef NOC_FILTER_CONCUR_H
#define NOC_FILTER_CONCUR_H
#include <config.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/file.h>
#include <sched.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#include "noc_filter_util.h"

typedef struct 
{
#ifdef POSIX_THREADS
  pthread_t *tasklets;
#endif //POSIX_THREADS
  unsigned int max_tasklets;
  int task_model;
  char *task_model_desc;
} bassa_task_pool;

typedef struct
{
#ifdef POSIX_THREADS
  pthread_mutex_t lock;
#endif //POSIX_THREADS
} bassa_mutex;

bassa_task_pool* bassa_task_pool_new (int max_tasklets);

bassa_mutex* bassa_mutex_new ();

int bassa_block_signal (int);

int bassa_unblock_signal (int);

#ifdef POSIX_THREADS
#define bassa_add_cleanup(f, a)			\
  pthread_cleanup_push(f, a);
#endif //POSIX_THREADS
	
#ifdef POSIX_TREADS
#define bassa_remove_cleanup()			\
  pthread_cleanup_pop(0);
#endif //POSIX_THREADS
	
int bassa_wait_spawn(bassa_task_pool *tt, void* (*routine)(void*), void *args);

int bassa_nowait_spawn(bassa_task_pool *tt, void* (*routine)(void*), void *args);
  
int bassa_enable_async_cancel(int* oldstate, int* oldtype);

int bassa_disable_cancel(int* oldstate, int* oldtype);
  
int bassa_reset_cancel(int* oldstate, int* oldtype);

int bassa_mutex_lock(bassa_mutex *bm);
	
int bassa_mutex_unlock(bassa_mutex *bm);

void bassa_exit(int *ret);
		
void bassa_task_yield();
	
#define bassa_block(count, lock_till)		\
  while(count == lock_till+1)

int bassa_kill_task(bassa_task_pool *tp, int t);

#define bassa_push_cleaner(func,arg)		\
  pthread_cleanup_push (func, arg)

#define bassa_pop_cleaner(i)			\
  pthread_cleanup_pop (i)
	

#define NOC_FILTER_SET_POOL(thread_array, index)	\
  thread_array[index] = t;
	
#define NOC_FILTER_UNSET_POOL(thread_array, index)	\
  thread_array[index] = 0;
	
#define NOC_FILTER_CLEANUP_POOL(thread_array, array_length)		\
  int thread_array_counter = 0;						\
  for(;thread_array_counter<array_length; thread_array_counter++)	\
    thread_array[thread_array_counter] = 0;

#endif //NOC_FILTER_CONCUR_H
