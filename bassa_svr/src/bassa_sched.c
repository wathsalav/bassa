#include <unistd.h>

#include <bassa_sched.h>
#include <bassa_trigger.h>
#include <noc_filter_transaction.h>

bassa_sched*
bassa_sched_new (bassa_conf *conf)
{
  bassa_sched *bs = (bassa_sched*)malloc(sizeof(bassa_sched));
  bs->blist = bassa_list_new();
  bs->clist = bassa_list_new();
  bs->klist = bassa_list_new();
  clist = bs->clist;
  klist = bs->klist;
  blist = bs->blist;
  bs->htpool = bassa_task_pool_new((conf->dlcfg->max_children)*2);
  htpool = bs->htpool;
  bs->list_len = 0;
  bs->htproc_max = conf->dlcfg->max_children;
  bs->list_max = conf->dlcfg->max_children;
  bs->htproc_count = 0;
  htproc_count = &(bs->htproc_count);
  htproc_max = bs->htproc_max;
  bs->trig = bassa_trigger_new ("/tmp/bassa_sched_trigger"); 
  if (bs->trig == NULL)
  {
    perror("Error creating trigger");
    return NULL;
  }
  btrig = bs->trig;
  bs->sched_sleep = SLEEPING;
  bs->btimer = bassa_timer_new(bassa_sched_alarm);
  bs->dbd = bassa_db_init();
  return bs;
}


void
bassa_sched_start(void* param)
{
  bassa_sched *bs = (bassa_sched*)param;
  bs->htpool = bassa_task_pool_new(bs->htproc_count);
  bassa_timer_start(bs->btimer);
  int i = -1;
  while ((i=bassa_trigger_sleep(bs->trig))>=0)
  {
    switch(i)
    {
      case BASSA_BAD_SPAWN:
	printf ("Bad thread spawn timing\n");
	bassa_list *bbln = NULL;
	bassa_irequest *bbirq = NULL;
	if ((bbln = bassa_list_adrm_r(blist, BASSA_LIST_REM, NULL, 0)))
	{
	  bbirq = bbln->list_data;
	  bassa_db_update_cache(bs->dbd, bbirq);
	  bassa_irequest_free(bbirq);
	  bbirq = NULL;
	  bassa_list_delete(bbln);
	  bbln = NULL;
	}
	break;
      case BASSA_TRANSACTION_COMPLETE:
	printf ("Transaction Complete\n");
	//Go through completed list and put them to the db
	bassa_list *bln = NULL;
	bassa_irequest *birq = NULL;
	if ((bln = bassa_list_adrm_r(clist, BASSA_LIST_REM, NULL, 0)))
	{
	  birq = bln->list_data;
	  bassa_db_update_cache(bs->dbd, birq);
	  bassa_irequest_free(birq);
	  birq = NULL;
	  bassa_list_delete(bln);
	  bln = NULL;
	  printf ("PROC COUNT: %i\n", bs->htproc_count);
	  printf ("PROC MAX: %i\n", bs->htproc_max);
	}
	break;
      case BASSA_KILL_PROC:
	printf ("Killed Proc\n");
	//Go through completed list and put them to the db
	bassa_list *kbln = NULL;
	bassa_irequest *kbirq = NULL;
	bassa_list_dump(klist);
	if ((kbln = bassa_list_adrm_r(klist, BASSA_LIST_REM, NULL, 0)))
	{
	  kbirq = kbln->list_data;
	  if (kbirq)
	  {
	    bassa_db_update_cache(bs->dbd, kbirq);
	    bassa_irequest_free(kbirq);
	    kbirq = NULL;
	  }
	  bassa_list_delete(kbln);
	  kbln = NULL;
	}
	break;
      case BASSA_INIT_PROC:
	printf ("Initialize Process\n");
	bassa_irequest *sbirq = bassa_db_getpending(bs->dbd);
	if (sbirq != NULL)
	  bassa_nowait_spawn(htpool, bassa_sched_htproc, sbirq);
	break;
      case BASSA_ALARM_FIRED:
	printf ("ALARM FIRED\n");
	if (bs->sched_sleep == SLEEPING)
	{
	  printf ("WAKING UP\n");
	  bs->sched_sleep = AWAKE;
	  bassa_invert_status(bs->dbd, PROCESSING, PENDING);
	  bassa_trigger_wake(bs->trig, BASSA_INIT_PROC);
	  /*bassa_irequest *ibirq = bassa_db_getpending(bs->dbd);
	  if (ibirq != NULL)
	    bassa_nowait_spawn(htpool, bassa_sched_htproc, ibirq);*/ 
	}
	else
	{
	  printf ("SLEEPING\n");
	  bs->sched_sleep = SLEEPING;
	  //Kill the whole thread pool
	  bassa_kill_task_pool(htpool);
	}
	break;
    }
  } 
}


void*
bassa_sched_htproc(void *param)
{
  bassa_disable_cancel(NULL, NULL);
  //MUTEX LOCK
  bassa_mutex *bm = bassa_mutex_new();
  bassa_mutex_lock(bm);
  if (*htproc_count < htproc_max)
  {
    (*htproc_count)++;
    printf ("THREAD PROC COUNT: %i\n", *htproc_count);
    if (*htproc_count < htproc_max)
      bassa_trigger_wake(btrig, BASSA_INIT_PROC);
  }
  else
  {
    //save sate and clean the thread pool
    bassa_task_cleaner(htpool);
    bassa_sched_saveonexit(param);
    return NULL;
  }
  //MUTEX UNLOCK
  bassa_mutex_unlock(bm);
  bassa_mutex_free(bm);
  bassa_push_cleaner(bassa_task_cleaner, htpool); 
  bassa_push_cleaner(bassa_sched_saveonkill, param); 
  bassa_irequest *birq = (bassa_irequest*)param;
  bassa_transaction *btr = bassa_transaction_new(birq);
  bassa_enable_cancel(NULL, NULL);
  bassa_test_cancel();
  if (btr)
  {
    bassa_transaction_open(btr);
    bassa_transaction_download(btr);
    bassa_transaction_free(btr);
    btr = NULL;
  }
  bassa_disable_cancel(NULL, NULL);
  bassa_list_adrm_r(clist, BASSA_LIST_ADD, birq, 0);
  bassa_trigger_wake(btrig, BASSA_TRANSACTION_COMPLETE);
  bassa_pop_cleaner(0);
  bassa_pop_cleaner(0);
  (*htproc_count)--;
  bassa_trigger_wake(btrig, BASSA_INIT_PROC);
  bassa_enable_cancel(NULL, NULL);
  return NULL;
}

void
bassa_sched_saveonkill(void *arg)
{ 
  (*htproc_count)--;
  bassa_list_adrm_r(klist, BASSA_LIST_ADD, arg, 0);
  bassa_trigger_wake(btrig, BASSA_KILL_PROC);
}


void bassa_sched_saveonexit(void *arg)
{
  bassa_irequest *birq = (bassa_irequest*)arg;
  birq->bobj->status = PENDING;
  bassa_list_adrm_r(blist, BASSA_LIST_ADD, birq, 0);
  bassa_trigger_wake(btrig, BASSA_BAD_SPAWN);
}

void 
bassa_sched_free(bassa_sched *sched)
{
  if (sched)
  {
    bassa_db_shutdown(sched->dbd);
    free(sched->htpool);
    sched->htpool = NULL;
    htpool = NULL;
    free(sched);
    sched = NULL;
  }
}

void
bassa_sched_alarm(int sig)
{
  bassa_trigger_wake(btrig, BASSA_ALARM_FIRED);
}

