/***************************************************************************
 *   Copyright (C) 2007 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

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
#include "bassa_db.h"


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
  if (nfs->conf->dlcfg->max_children)
    nfs->bassa_max_downloaders = nfs->conf->dlcfg->max_children;
  nfs->bassa_downloader_ids = NULL;
  nfs->mtp = bassa_task_pool_new (1);
  nfs->stp = bassa_task_pool_new (nfs->bassa_max_downloaders);
  nfs->bassa_downloader_count = 0;
  nfs->cpu_lock = NULL;
  nfs->trigger = 0;
  nfs->saved_trigger = 0;
  return nfs;
}


void
bassa_sigalarm_handler (int signum)
{
  bassa_block_signal (1, SIGALRM);
#ifdef DEBUG
  printf ("TIMER_HANDLER %i\n", nfs->trigger);
#endif //DEBUG
  if (nfs->trigger == 0)
    {
#ifdef DEBUG
      printf ("Starting Downloader(s)...\n");
#endif //DEBUG
      //Set the deactivation time.
      int hours = nfs->conf->dlcfg->dhours;
      int mins = nfs->conf->dlcfg->dminutes;
      int secs = nfs->conf->dlcfg->dseconds;
      bassa_timer_set_alarm (nfs->timer, hours, mins, secs);
      nfs->cpu_lock = bassa_shared_semaphore (nfs->cpu_lock);
      nfs->trigger = 1;
      bassa_nowait_spawn (nfs->mtp, bassa_sched_downloader_loop, NULL);
    }
  else if (nfs->trigger == 1)
    {
#ifdef DEBUG
      printf ("Stoping Downloader(s)...\n");
#endif //DEBUG
      //Set the reactivation time.
      int hours = nfs->conf->dlcfg->hours;
      int mins = nfs->conf->dlcfg->minutes;
      int secs = nfs->conf->dlcfg->seconds;
      bassa_timer_set_alarm (nfs->timer, hours, mins, secs);
      nfs->trigger = 0;
      int ret = bassa_kill_task (nfs->mtp, nfs->mtp->tasklets[0]);
      if (ret)
        {
          printf ("ECRITICAL: Failed to stop downloader HUB: %lu\n", 
	  	   nfs->mtp->tasklets[0]);
          exit (-1);
        }
    }
  bassa_timer_start (nfs->timer);
  bassa_unblock_signal (1, SIGALRM);
  return;
}

void *
bassa_sched_downloader_loop (void *param)
{
  bassa_blockall_signals ();
  while (1)
    {
      //if (nfs->bassa_downloader_count < nfs->bassa_max_downloaders)
        //{
	  			//Replace when working with modules
	  			//bassa_module_info *modinfo = bassa_module_info_new (get_conf(), get_bmt());
	  			bassa_irequest *bir = bassa_db_getpending();
	  			bassa_transaction *ta = bassa_transaction_new (bir);
	  			if (bir)
	  				{
	  					nfs->bassa_downloader_count++;
	  					bassa_nowait_spawn (nfs->stp, bassa_sched_outop_thread, (void*)ta);
          		bassa_task_yield();
	  				}
        //}
    }
  bassa_unblockall_signals ();
  return NULL;
}

/**
void
bassa_sched_downloader_cleanup_loop (void *param)
{
  bassa_block_signal (1, SIGALRM);
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
          printf ("Killed: %lu\n", nfs->stp->tasklets[i]);
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
  bassa_sema_destroy (nfs->cpu_lock);
  bassa_unblock_signal (1, SIGALRM);
}

void
bassa_sched_outop_thread_clean (void *param)
{
  bassa_block_signal (1, SIGALRM);
  //bassa_block_signal (SIGUSR1);
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
  bassa_unblock_signal (1, SIGALRM);
#ifdef DEBUG
  printf ("BASSA_SCHED_OUTPUT_THREAD_CLEAN_DONE\n");
#endif //DEBUG
}
**/

void *
bassa_sched_outop_thread (void *un)
{
  bassa_block_signal (1, SIGALRM);
  bassa_transaction *transaction = (bassa_transaction*)un;
  int status;
#ifdef DEBUG
  printf ("BASSA_SCHED_OUTPUT_THREAD\n");
#endif //DEBUG
  //Replace when working with modules
  //Execution point POP_STORE@POP_REGION
  //bassa_exec_path (bobj->modinfo->popc, bobj->modinfo->modtab,
	//	   POP_STORE, POP_REGION);
  //Set snetwork options for this transaction
  bassa_transaction_open (transaction);
  bassa_transcation_set_network_opts (transaction, nfs->conf->dlcfg->connect_timeout,
				      nfs->conf->dlcfg->max_tries);
  status = bassa_transaction_download (transaction);
  
  if (!status)
    {
      if (IS_HTTP_PROTO(transaction->http_bf) || 
	  		IS_HTTPS_PROTO(transaction->http_bf))
				{
	  			if (IS_OK(transaction->http_bf) || 
	      			IS_NO_CONTENT(transaction->http_bf) ||
	      			IS_PARTIAL_CONTENT(transaction->http_bf))
	    			{
	    				status = 0;
	    				transaction->birq->bobj->status = COMPLETED;
	    			}
	  			else if (IS_BAD_REQUEST(transaction->http_bf) || 
		   						IS_UNAUTHORIZED(transaction->http_bf) || 
		   						IS_FORBIDDEN(transaction->http_bf) || 
		   						IS_NOT_FOUND(transaction->http_bf) ||
		   						IS_REQUEST_TIMEOUT(transaction->http_bf))
	    			{
	      			unlink (transaction->birq->bobj->object_path);
	      			status = 1;
	      			transaction->birq->bobj->status = FAILED;
	    			}
	  			else if (IS_INTERNAL_SERVER_ERROR(transaction->http_bf) ||
		   						IS_VERSION_NOT_SUPPORTED(transaction->http_bf) ||
		   						IS_SERVICE_UNAVAILABLE(transaction->http_bf))
	    			{
	      			unlink (transaction->birq->bobj->object_path);
	      			status = 1;
	      			transaction->birq->bobj->status = FAILED;
	    			}
	  			else
	    			{
	      			unlink (transaction->birq->bobj->object_path);
	      			status = 1;
	      			transaction->birq->bobj->status = FAILED;
	    			}
				}
    }
  else
    {
      unlink (transaction->birq->bobj->object_path);
      status = 1;
      transaction->birq->bobj->status = FAILED;
    }
  transaction->birq->bobj->content_length = 
  					bassa_file_get_size (transaction->birq->bobj->object_path);
  transaction->bassa_status = status;
  //Replace when working with modules
  //Synchronize pop context and transaction data
  //bassa_request_pop_sync (bobj);
  //Execution point POP_NOTIFY@POP_REGION
  //bassa_exec_path (bobj->modinfo->popc, bobj->modinfo->modtab,
	//		POP_REGION, POP_NOTIFY);
  if(status==0)
		bassa_db_update_cache(transaction->birq);
  else
		bassa_db_update_cache(transaction->birq);
  //NOC_FILTER_SET_POOL should done after NOC_FILTER_TASK_UNSTOPABLE to avaoid lost updates
  pthread_t xtid = pthread_self ();
  int i = 0;
  for (i=0; i<nfs->stp->max_tasklets; i++)
    {
      pthread_t td = (pthread_t)(nfs->stp->tasklets[i]);
      if (xtid == td)
				{
					bassa_mutex *pbmut = bassa_mutex_new ();
	  			bassa_mutex_lock (pbmut);
	  			nfs->stp->tasklets[i] = 0;
          nfs->bassa_downloader_count--;
#ifdef DEBUG
  printf ("DECREMENTED nfs->bassa_downloader_count: %i\n", nfs->bassa_downloader_count);
#endif //DEBUG
	  			bassa_mutex_unlock (pbmut);
	 	 			break;
				}
    }
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

void bassa_sigusr1_handler (int signum)
{
  printf (">%lu\n", bassa_task_id());
  return;
}


void bassa_sigusr2_handler (int signum)
{
  printf ("Fuck me plenty, SIGUSR2 received\n");
}

