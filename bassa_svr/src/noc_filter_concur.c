/***************************************************************************
 *   Copyright (C) 2007 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "noc_filter_concur.h"

bassa_task_pool* bassa_task_pool_new (int max_tasklets)
{
  bassa_task_pool *tp = (bassa_task_pool*)
    malloc(sizeof(bassa_task_pool));
  tp->max_tasklets = max_tasklets;
#ifdef POSIX_THREADS
  tp->tasklets = (pthread_t*) malloc(max_tasklets*sizeof(pthread_t));
  memset (tp->tasklets, 0, max_tasklets*sizeof(pthread_t));
#endif //POSIX_THREADS
  return tp;
}

bassa_mutex* bassa_mutex_new ()
{
  bassa_mutex *bm = (bassa_mutex*)malloc(sizeof(bassa_mutex));
#ifdef POSIX_THREADS  
  pthread_mutex_t _lock = PTHREAD_MUTEX_INITIALIZER;
  bm->lock = _lock;
#endif //POSIX_THREADS
  return bm;
}

void bassa_mutex_free (bassa_mutex *bm)
{
  if (bm)
  {
    free (bm);
    bm = NULL;
  }
}

int bassa_block_signal (int num, ...)
{
  va_list argptr;
  va_start(argptr, num);
  sigset_t newmask;
  sigemptyset (&newmask);
  for (;num;num--)
  {
    int sig = va_arg(argptr, int);
    sigaddset (&newmask, sig);
  }
  if (pthread_sigmask(SIG_BLOCK, &newmask, NULL) < 0)
    return 1;
  return 0;
}

int bassa_unblock_signal (int num, ...)
{
  va_list argptr;
  va_start(argptr, num);
  sigset_t newmask, pendmask;
  sigemptyset (&newmask);
  for (;num;num--)
  {
    int sig = va_arg(argptr, int);
    if (sigpending (&pendmask) < 0)
      return 1;
    if (sigismember (&pendmask, sig) < 0)
      return 2;
    sigaddset (&newmask, sig);
  }
  if (pthread_sigmask (SIG_UNBLOCK, &newmask, NULL) < 0)
    return 3;
  return 0;
}


int bassa_blockall_signals ()
{
  sigset_t newmask;
  sigfillset (&newmask);
  if (pthread_sigmask(SIG_BLOCK, &newmask, NULL) < 0)
    return 1;
  return 0;
}

int bassa_unblockall_signals ()
{
  sigset_t newmask;
  sigfillset (&newmask);
  if (pthread_sigmask(SIG_UNBLOCK, &newmask, NULL) < 0)
    return 1;
  return 0;
}


  int 
bassa_wait_spawn(bassa_task_pool *tp, void* (*routine)(void*), void *args)
{
  int ret = 0;
#ifdef POSIX_THREADS
  pthread_t* pth = (pthread_t*)malloc(sizeof(pthread_t));
  ret = pthread_create (pth, NULL, routine, args);
  if (tp)
  {
    int i;
    for (i=0; i<tp->max_tasklets; i++)
    {
      if (tp->tasklets[i] == 0)
      {
	tp->tasklets[i] = *pth;
	break;
      }
    }
  }
  pthread_join (*pth, NULL);
#endif //POSIX_THREADS
  return ret;
}

  int 
bassa_nowait_spawn(bassa_task_pool *tp, void* (*routine)(void*), void *args)
{
  int ret = 0;
#ifdef POSIX_THREADS
  pthread_t* pth = (pthread_t*)malloc(sizeof(pthread_t));
  ret = pthread_create (pth, NULL, routine, args);
  if (tp)
  {
    int i;
    for (i=0; i<tp->max_tasklets; i++)
    {
      if (tp->tasklets[i] == 0)
      {
	tp->tasklets[i] = *pth;
	break;
      }
    }
  }
  pthread_detach (*pth);
#endif //POSIX_THREADS
  return ret;
}

  void
bassa_task_cleaner(void *arg)
{
  bassa_task_pool *tp = (bassa_task_pool*)arg;
#ifdef DEBUG
  printf ("CLEANER INVOKED\n");
#endif //DEBUG
  if (tp)
  {
    int i;
    int id = bassa_task_id();
    for (i=0; i<tp->max_tasklets; i++)
    {
      if (tp->tasklets[i] == id)
      {
	tp->tasklets[i] = 0;
	break;
      }
    }
  }
}


int 
bassa_enable_cancel(int* oldstate, int* oldtype)
{
  int ret = 0;
#ifdef POSIX_THREADS
  ret = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, oldstate);
  if (ret != 0)
    return ret;
#endif //POSIX_THREADS
  return 0;
}

int bassa_disable_cancel(int* oldstate, int* oldtype)
{
  int ret = 0;
#ifdef POSIX_THREADS
  ret = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, oldstate);
  if (ret != 0)
    return ret;
#endif //POSIX_THREADS
  return 0;
}


int bassa_mutex_lock(bassa_mutex *bm)
{
#ifdef POSIX_THREADS
  //pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
  //bm->lock = mut;
  //pthread_mutex_init(&(bm->lock), NULL);
  return pthread_mutex_lock(&(bm->lock));
#endif //POSIX_THREADS
}

int bassa_mutex_unlock(bassa_mutex *bm)
{
#ifdef POSIX_THREADS
  return pthread_mutex_unlock(&(bm->lock));
#endif //POSIX_THREADS
}

  bassa_semaphore*
bassa_shared_semaphore(bassa_semaphore *bs)
{
  if (bs == NULL)
    bs = (bassa_semaphore*)malloc(sizeof(bassa_semaphore));
  sem_init (&(bs->semaphore), 0, 0);
  return bs;
}

int bassa_sema_wait(bassa_semaphore *bs)
{
#ifdef POSIX_THREADS
  return sem_wait(&(bs->semaphore));
#endif //POSIX_THREADS
}

int bassa_sema_trywait(bassa_semaphore *bs)
{
#ifdef POSIX_THREADS
  return sem_trywait(&(bs->semaphore));
#endif //POSIX_THREADS
}

int bassa_sema_post(bassa_semaphore *bs)
{
#ifdef POSIX_THREADS
  return sem_post(&(bs->semaphore));
#endif //POSIX_THREADS
}

int bassa_sema_destroy(bassa_semaphore *bs)
{
#ifdef POSIX_THREADS
  int ret = sem_destroy(&(bs->semaphore));
#endif //POSIX_THREADS
  return ret;
}

void bassa_task_exit(int *ret)
{
#ifdef POSIX_THREADS
  pthread_exit((void*)ret);
#endif //POSIX_THREADS
}

void bassa_task_yield()
{
#ifdef POSIX_THREADS
  sched_yield();
#endif //POSIX_THREADS
}


void  bassa_kill_task_pool(bassa_task_pool *tp)
{
  int i;
  if (tp)
  {
    for (i=0; i<tp->max_tasklets; i++)
    {
      if (tp->tasklets[i] != 0)
      {
	printf ("KILLING TASK: %lu\n", tp->tasklets[i]);
	bassa_kill_task(tp, tp->tasklets[i]);
      }
    }
  }
}

int bassa_kill_task(bassa_task_pool *tp, unsigned long t)
{
  int i;
  int ret = 0;
  if (tp)
  {
    for (i=0; i<tp->max_tasklets; i++)
    {
      if (tp->tasklets[i] == t)
      {
	tp->tasklets[i] = 0;
	ret = pthread_cancel((pthread_t)t);
	if (ret == ESRCH)
	{
#ifdef DEBUG
	  fprintf (stderr, "No such thread\n");
#endif //DEBUG
	}
	if (ret)
	{
#ifdef DEBUG
	  printf ("Did not cancel thread\n");
#endif //DEBUG
	}
	break;
      }
    }
  }
  return ret;
}


