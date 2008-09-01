/***************************************************************************
 *   Copyright (C) 2006 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
		
#include <bassa_trigger.h>
#include "noc_filter_timer.h"
#include "noc_filter_concur.h"

extern bassa_trigger *btrig;

bassa_timer*
bassa_timer_new(bassa_timer_handler handler) 
{
  bassa_timer *timer = (bassa_timer*)malloc(sizeof(bassa_timer));
  timer->handler = handler;
  signal (SIGALRM, timer->handler);
  return timer;
}

int 
bassa_timer_start(bassa_timer *timer)
{
  return alarm(timer->timeout_value);
  //return bassa_nowait_spawn (NULL, bassa_timer_thread, timer);
}

void 
bassa_timer_set_alarm(bassa_timer *timer, int hours, int mins, int seconds)
{
  time_t xt;
  time_t epoch_time = time(&xt);
  struct tm *xtm = localtime(&epoch_time);
#ifdef DEBUG
  printf ("TIME [H]: %i, %i\n", xtm->tm_hour, hours);
  printf ("TIME [M]: %i, %i\n", xtm->tm_min, mins);
  printf ("TIME [S]: %i, %i\n", xtm->tm_sec, seconds);
#endif //DEBUG
  int current_time = NOC_FILTER_TIMER_CVT_TIME2SECS(xtm->tm_hour, xtm->tm_min, xtm->tm_sec);
#ifdef DEBUG
  printf ("TIME: %i\n", current_time);
#endif //DEBUG
  int alarm_time = NOC_FILTER_TIMER_CVT_TIME2SECS(hours, mins, seconds);
#ifdef DEBUG
  printf ("TIME: %i\n", alarm_time);
#endif	//DEBUG
  int sec_diff = alarm_time - current_time;
  if(sec_diff < 0)
    sec_diff += NOC_FILTER_TIMER_CVT_HOURS2SECS(24);
  timer->timeout_value = sec_diff;
#ifdef DEBUG
  printf("Alarm will be activeted in: %i seconds\n", sec_diff);
#endif //DEBUG
}

void bassa_timer_sleep(time_t sec, long nsec)
{
  struct timespec req;
  req.tv_sec = sec;
  req.tv_nsec = nsec;
#ifdef DEBUG
  printf ("Nano sleep\n");
#endif //DEBUG
  nanosleep (&req, NULL);
}

