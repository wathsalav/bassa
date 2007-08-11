#include <config.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <dlfcn.h>
#include <getopt.h>
#include <signal.h>

#include "noc_filter_sched.h"
#include "noc_filter_util.h"
#include "noc_filter_file.h"


bassa_sched *
bassa_sched_new (bassa_conf *conf)
{
  nfs = (bassa_sched*)malloc(sizeof(bassa_sched));
  nfs->timer = bassa_timer_new ((bassa_timer_handler)bassa_sigalarm_handler
				,(bassa_scheduler_status)bassa_cleaners_done, (void*)nfs);
  nfs->conf = conf;
  int hours=0;
  int minutes=0;
  int seconds=0;
  if (nfs->conf->dlcfg)
    hours = conf->dlcfg->hours;
  if (nfs->conf->dlcfg)
    minutes = nfs->conf->dlcfg->minutes;
  if (nfs->conf->dlcfg)
    seconds = nfs->conf->dlcfg->seconds;
  bassa_timer_set_alarm (nfs->timer, hours, minutes, seconds);
  nfs->gqueue = bassa_queue_new ();
  if (nfs->conf->dlcfg->max_children)
    nfs->bassa_max_downloaders = nfs->conf->dlcfg->max_children;
  nfs->bassa_downloader_ids = NULL;
  nfs->mtp = bassa_task_pool_new (1);
  nfs->stp = bassa_task_pool_new (nfs->bassa_max_downloaders);
  nfs->bassa_downloader_count = 0;
  nfs->trigger = 1;
  //nfs->sigignore_flag = 0;
  return nfs;
}


void
bassa_main_debug_user (bassa_request * ux)
{
  printf ("USER_OBJ_DEBUG_INFO\n");
  printf (">>>UX_NAME: %s\n", ux->name);
  printf (">>>UX_EMAIL: %s\n", ux->email);
  printf (">>>UX_IP: %s\n", ux->ip);
  printf (">>>UX_URL: %s\n", ux->url);
  printf (">>>UX_PATH: %s\n", ux->transaction->file_complete);
  printf (">>>UX_HTTP_PROXY: %s\n", ux->transaction->http_proxy);
  printf (">>>UX_SIZE: %lli\n", ux->transaction->size);
  printf ("\n\n");
}


void
bassa_sigalarm_handler (int signum)
{
  bassa_block_signal (SIGALRM);
#ifdef DEBUG
  printf ("TIMER_HANDLER\n");
#endif //DEBUG
  if (nfs->trigger == 1)
    {
#ifdef DEBUG
      printf ("Starting Downloader(s)...\n");
#endif //DEBUG
      //Set the deactivation time.
      int hours = nfs->conf->dlcfg->dhours;
      int mins = nfs->conf->dlcfg->dminutes;
      int secs = nfs->conf->dlcfg->dseconds;
      bassa_timer_set_alarm (nfs->timer, hours, mins, secs);
      nfs->trigger = 0;
      bassa_nowait_spawn (nfs->mtp, bassa_sched_downloader_loop, NULL);
    }
  else if (nfs->trigger == 0)
    {
#ifdef DEBUG
      printf ("Stoping Downloader(s)...\n");
#endif //DEBUG
      //Set the reactivation time.
      int hours = nfs->conf->dlcfg->hours;
      int mins = nfs->conf->dlcfg->minutes;
      int secs = nfs->conf->dlcfg->seconds;
      bassa_timer_set_alarm (nfs->timer, hours, mins, secs);
      nfs->trigger = 1;
      int ret = bassa_kill_task (nfs->mtp, nfs->mtp->tasklets[0]);
      if (ret)
        {
          printf ("ECRITICAL: Failed to stop downloader HUB: %lu\n", 
	  	   nfs->stp->tasklets[0]);
          exit (-1);
        }
    }
  bassa_timer_start (nfs->timer);
  bassa_unblock_signal (SIGALRM);
  return;
}

void *
bassa_sched_downloader_loop (void *param)
{
  bassa_block_signal (SIGALRM);
  int os, ot;
  bassa_push_cleaner(bassa_sched_downloader_cleanup_loop, NULL);
  while (1)
    {    
      bassa_enable_async_cancel(&os, &ot);
      if (nfs->bassa_downloader_count < nfs->bassa_max_downloaders)
        {
          bassa_disable_cancel (&os, &ot);
          bassa_mutex user_lock, count_lock;
          bassa_mutex_lock (&user_lock);
          bassa_request *user = bassa_queue_dequeue (nfs->gqueue);
          bassa_mutex_unlock (&user_lock);
          if (user)
            {
              bassa_mutex_lock (&count_lock);
              nfs->bassa_downloader_count++;
              bassa_mutex_unlock (&count_lock);
              bassa_nowait_spawn (nfs->stp, bassa_sched_outop_thread, (void*)user);
            }
          bassa_reset_cancel (&os, &ot);
          bassa_task_yield();
        }
      bassa_reset_cancel (&os, &ot);
    }
  bassa_pop_cleaner(0);
  bassa_unblock_signal (SIGALRM);
  return NULL;
}

void
bassa_sched_downloader_cleanup_loop (void *param)
{
  bassa_block_signal (SIGALRM);
#ifdef DEBUG
  printf ("BASSA_SCHED_DOWNLOADER_CLEANUP_LOOP\n");
#endif //DEBUG
  //Search the active pool slots and cancel them all, the rest should be
  //handled by their respective cleanup handlers.
  int i;
  for (i=0; i<nfs->stp->max_tasklets; i++)
    {
      if (nfs->stp->tasklets[i] != 0)
        {
          int ret = bassa_kill_task (nfs->stp, nfs->stp->tasklets[i]);
          if (ret)
            {
              printf ("ECRITICAL: Failed to stop downloader: %lu\n", 
			nfs->stp->tasklets[i]);
              exit (-1);
            }
        }
    }
#ifdef DEBUG
  printf ("DOWLOADER LOOP CLEANED\n");
#endif //DEBUG
  bassa_mutex fplock;
  bassa_mutex_lock (&fplock);
  bassa_mutex_unlock (&fplock);
  bassa_unblock_signal (SIGALRM);
}

void
bassa_sched_outop_thread_clean (void *param)
{
  bassa_block_signal (SIGALRM);
#ifdef DEBUG
  printf ("BASSA_SCHED_OUTPUT_THREAD_CLEAN\n");
#endif //DEBUG
  bassa_mutex clbmut;
  bassa_mutex_lock (&clbmut);
  if (param)
    {
      bassa_request *user = (bassa_request*)param;
      bassa_queue_reverse_enqueue (user, nfs->gqueue);
      nfs->bassa_downloader_count--;
    }
  bassa_mutex_unlock (&clbmut);
  bassa_unblock_signal (SIGALRM);
}

void *
bassa_sched_outop_thread (void *un)
{
  bassa_block_signal (SIGALRM);
  int os, ot;
  bassa_mutex cbmut, pbmut;
  //What if we cancel the thread while we are downloading, 
  //we will have to enqueue user again. So push the appropriate
  //cleanup handler which will enqueue the user back to the queue.
  bassa_push_cleaner (bassa_sched_outop_thread_clean, un);
  bassa_enable_async_cancel (&os, &ot);
  bassa_request *user = (bassa_request*)un;
  int status;
#ifdef DEBUG
  printf ("BASSA_SCHED_OUTPUT_THREAD\n");
#endif //DEBUG
  //Execution point POP_STORE@POP_REGION
  bassa_exec_path (user->modinfo->popc, user->modinfo->modtab,
		   POP_STORE, POP_REGION);
  //Set snetwork options for this transaction
  bassa_transcation_set_network_opts (user->transaction, nfs->conf->dlcfg->connect_timeout,
				      nfs->conf->dlcfg->max_tries);
  status = bassa_transaction_download (user->transaction);
 
  bassa_disable_cancel (&os, &ot);
  
  if (!status)
    {
      if (IS_HTTP_PROTO(user->transaction->http_bf) || 
	  IS_HTTPS_PROTO(user->transaction->http_bf))
	{
	  if (IS_OK(user->transaction->http_bf) || 
	      IS_NO_CONTENT(user->transaction->http_bf) ||
	      IS_PARTIAL_CONTENT(user->transaction->http_bf))
	    status = 0;
	  else if (IS_BAD_REQUEST(user->transaction->http_bf) || 
		   IS_UNAUTHORIZED(user->transaction->http_bf) || 
		   IS_FORBIDDEN(user->transaction->http_bf) || 
		   IS_NOT_FOUND(user->transaction->http_bf) ||
		   IS_REQUEST_TIMEOUT(user->transaction->http_bf))
	    {
	      unlink (user->transaction->file_complete);
	      status = 1;
	    }
	  else if (IS_INTERNAL_SERVER_ERROR(user->transaction->http_bf) ||
		   IS_VERSION_NOT_SUPPORTED(user->transaction->http_bf) ||
		   IS_SERVICE_UNAVAILABLE(user->transaction->http_bf))
	    {
	      unlink (user->transaction->file_complete);
	      status = 1;
	    }
	  else
	    {
	      unlink (user->transaction->file_complete);
	      status = 1;
	    }
	}
    }
  else
    {
      unlink (user->transaction->file_complete);
      status = 1;
    }
  user->size = bassa_file_get_size (user->transaction->file_complete);
  user->transaction->bassa_status = status;
  //Synchronize pop context and transaction data
  bassa_request_pop_sync (user);
  //Execution point POP_NOTIFY@POP_REGION
  bassa_exec_path (user->modinfo->popc, user->modinfo->modtab,
			POP_REGION, POP_NOTIFY);
  bassa_request_delete (un);
  bassa_mutex_lock (&cbmut);
  bassa_mutex_unlock (&cbmut);
  bassa_task_yield ();
	
  //NOC_FILTER_SET_POOL should done after NOC_FILTER_TASK_UNSTOPABLE to avaoid lost updates
  pthread_t xtid = pthread_self ();
  int i = 0;
  for (i=0; i<nfs->stp->max_tasklets; i++)
    {
      pthread_t td = (pthread_t)(nfs->stp->tasklets[i]);
      if (xtid == td)
	{
	  bassa_mutex_lock (&pbmut);
	  nfs->stp->tasklets[i] = 0;
          nfs->bassa_downloader_count--;
#ifdef DEBUG
  printf ("DECREMENTED nfs->bassa_downloader_count: %i\n", nfs->bassa_downloader_count);
#endif //DEBUG
	  bassa_mutex_unlock (&pbmut);
	  break;
	}
    }
  //We no longer need cleanup handler, so pop it from the stack. But we should pop it
  //after making task unstopable. This is because the time gap between poping cleanup handler and 
  //making task unstopable will cause a lost update (since cancellation could occur during this gap).
  bassa_reset_cancel (&os, &ot);
  bassa_reset_cancel (&os, &ot);	
  bassa_pop_cleaner (0);
  bassa_unblock_signal (SIGALRM);
  return NULL;
}

int bassa_sched_start (bassa_sched *nfs)
{
  return bassa_timer_start (nfs->timer);
}

int bassa_cleaners_done (bassa_sched *nfs)
{
  int i=0;
  int result = 1;
  for (; i < nfs->bassa_downloader_count; i++)
    {
      result &= nfs->bassa_cleaner_status[i];
    }
  return result;
}

