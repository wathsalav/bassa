#ifndef BASSA_SCHED_H
#define BASSA_SCHED_H
#include <stdlib.h>
#include <stdio.h>
#include <bassa_trigger.h>
#include <bassa_db.h>
#include <noc_filter_concur.h>
#include <noc_filter_util.h>
#include <noc_filter_configure.h>
#include <noc_filter_timer.h>

#define SLEEPING 1
#define AWAKE 0

bassa_trigger *btrig;
bassa_list *clist, *klist, *blist;
bassa_task_pool *htpool;
unsigned int *htproc_count, htproc_max;

typedef struct
{
  unsigned int hour;
  unsigned int min;
  unsigned int sec;
}bassa_time_object;

typedef struct
{
  unsigned int list_len;
  unsigned int list_max;
  unsigned int htproc_max;
  unsigned int htproc_count;
  bassa_task_pool *htpool;
  bassa_mutex *db_lock;
  //bassa_list *plist;
  //bassa_list *flist;
  bassa_list *clist;
  bassa_list *blist;
  bassa_list *klist;
  bassa_trigger *trig;
  int sched_sleep;
  bassa_timer *btimer;
  bassa_db *dbd;
  bassa_time_object start_bto;
  bassa_time_object stop_bto;
}bassa_sched;

bassa_sched*
bassa_sched_new (bassa_conf*);

void*
bassa_sched_monitor_db (void*);

void
bassa_sched_start(void*);

void
bassa_sched_blist_init(bassa_sched*);

void
bassa_sched_onalarm(bassa_sched*);

void*
bassa_sched_htproc(void*);

void
bassa_sched_alarm(int);

void
bassa_sched_free(bassa_sched*);

void
bassa_sched_saveonkill(void *arg);

void bassa_sched_saveonexit(void *arg);

#endif //BASSA_SCHED_H
