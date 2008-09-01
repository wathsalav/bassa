/***************************************************************************
 *   Copyright (C) 2007 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

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
#include <semaphore.h>

#include "noc_filter_util.h"



#define bassa_block(count, lock_till)		\
  while(count == lock_till+1)

#ifdef POSIX_THREADS
#define bassa_push_cleaner(func,arg)		\
  pthread_cleanup_push (func, arg)

#define bassa_pop_cleaner(i)			\
  pthread_cleanup_pop (i)

#define bassa_task_id()				\
  pthread_self()

#define bassa_test_cancel()\
  pthread_testcancel();
#endif //POSIX_THREADS

#define NOC_FILTER_SET_POOL(thread_array, index)	\
  thread_array[index] = t;
	
#define NOC_FILTER_UNSET_POOL(thread_array, index)	\
  thread_array[index] = 0;
	
#define NOC_FILTER_CLEANUP_POOL(thread_array, array_length)		\
  int thread_array_counter = 0;						\
  for(;thread_array_counter<array_length; thread_array_counter++)	\
    thread_array[thread_array_counter] = 0;

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

typedef struct
{
#ifdef POSIX_THREADS
  sem_t semaphore;
#endif //POSIX_THREADS
}bassa_semaphore;


bassa_task_pool* bassa_task_pool_new (int max_tasklets);

bassa_mutex* bassa_mutex_new ();

void bassa_mutex_free (bassa_mutex *bm);

int bassa_block_signal (int num, ...);

int bassa_unblock_signal (int num, ...);

int bassa_blockall_signals ();

int bassa_unblockall_signals ();
	
int bassa_wait_spawn(bassa_task_pool *tt, void* (*routine)(void*), void *args);

int bassa_nowait_spawn(bassa_task_pool *tt, void* (*routine)(void*), void *args);
  
void bassa_task_cleaner(void *arg);

int bassa_enable_cancel(int* oldstate, int* oldtype);

int bassa_disable_cancel(int* oldstate, int* oldtype);  

int bassa_mutex_lock(bassa_mutex *bm);
	
int bassa_mutex_unlock(bassa_mutex *bm);

bassa_semaphore* 
bassa_named_semaphore ();

bassa_semaphore* 
bassa_shared_semaphore ();

int bassa_sema_wait(bassa_semaphore *bs);

int bassa_sema_trywait(bassa_semaphore *bs);

int bassa_sema_post(bassa_semaphore *bs);

int bassa_sema_destroy(bassa_semaphore *bs);

void bassa_task_exit(int *ret);
		
void bassa_task_yield();

int bassa_kill_task(bassa_task_pool *tp, unsigned long int t);
	
void  bassa_kill_task_pool(bassa_task_pool *tp);
#endif //NOC_FILTER_CONCUR_H
