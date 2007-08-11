#include <stdio.h>
#include <stdlib.h>

#include "noc_filter_concur.h"

bassa_task_pool* bassa_task_pool_new (int max_tasklets)
{
  bassa_task_pool *tp = (bassa_task_pool*)
			  malloc(sizeof(bassa_task_pool));
  tp->max_tasklets = max_tasklets;
#ifdef POSIX_THREADS
  tp->tasklets = (pthread_t*) malloc(tp->max_tasklets*sizeof(pthread_t));
  memset (tp->tasklets, 0, tp->max_tasklets*sizeof(pthread_t));
#endif //POSIX_THREADS
  return tp;
}

bassa_mutex* bassa_mutex_new ()
{
  bassa_mutex *bm = (bassa_mutex*)
			malloc(sizeof(bassa_mutex));
  return bm;
}

int bassa_block_signal (int signal)
{
  sigset_t newmask;
  sigemptyset (&newmask);
  sigaddset (&newmask, signal);
  if (pthread_sigmask(SIG_BLOCK, &newmask, NULL) < 0)
    return 1;
  return 0;
}

int bassa_unblock_signal (int signal)
{
  sigset_t newmask, pendmask;
  if (sigpending (&pendmask) < 0)
    return 1;
  if (sigismember (&pendmask, signal) < 0)
    return 2;
  sigemptyset (&newmask);
  sigaddset (&newmask, signal);
  if (pthread_sigmask (SIG_UNBLOCK, &newmask, NULL) < 0)
    return 3;
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
  
int 
bassa_enable_async_cancel(int* oldstate, int* oldtype)
{
  int ret = 0;
  #ifdef POSIX_THREADS
  ret = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, oldstate);
  ret |= pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, oldtype);
  #endif //POSIX_THREADS
  return ret;
}

int bassa_disable_cancel(int* oldstate, int* oldtype)
{
  int ret = 0;
  #ifdef POSIX_THREADS
  ret = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, oldstate);
  ret |= pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, oldtype);
  #endif //POSIX_THREADS
  return ret;
}
  
int 
bassa_reset_cancel(int* oldstate, int* oldtype)
{
  int ret = 0;
  #ifdef POSIX_THREADS
  ret = pthread_setcancelstate(*oldstate, oldstate);
  ret |= pthread_setcanceltype(*oldtype, oldtype);
  #endif //POSIX_THREADS
  return ret;
}

int bassa_mutex_lock(bassa_mutex *bm)
{
  #ifdef POSIX_THREADS
  pthread_mutex_t mut;
  bm->lock = mut;
  pthread_mutex_init(&(bm->lock), NULL);
  return pthread_mutex_lock(&(bm->lock));
  #endif //POSIX_THREADS
}
	
int bassa_mutex_unlock(bassa_mutex *bm)
{
  #ifdef POSIX_THREADS
  return pthread_mutex_unlock(&(bm->lock));
  #endif //POSIX_THREADS
}

void bassa_exit(int *ret)
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

int bassa_kill_task(bassa_task_pool *tp, int t)
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
              ret = pthread_cancel(t);
              if (ret == ESRCH)
                printf ("No such thread\n");
              if (ret)
                printf ("Did not cancel thread\n");
              break;
            }
        }
    }
  return ret;
}
