/***************************************************************************
 *   Copyright (C) 2006 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk  *
 ***************************************************************************/

#ifndef NOC_FILTER_TIMER
#define NOC_FILTER_TIMER

#include <config.h>

#define NOC_FILTER_TIMER_CVT_HOURS2SECS(h)	\
  h*60*60
		
#define NOC_FILTER_TIMER_CVT_MINS2SECS(m)	\
  m*60
		
#define NOC_FILTER_TIMER_CVT_TIME2SECS(h,m,s)	\
  (h*60*60)+(m*60)+s
		
typedef void (*bassa_timer_handler)(int);
typedef int (*bassa_scheduler_status)(void);
typedef struct
{
  bassa_timer_handler handler;
  int timeout_value;
  int do_repeate;
}bassa_timer;

bassa_timer *bassa_timer_new(bassa_timer_handler handler); 

int bassa_timer_start(bassa_timer *timer);

void bassa_timer_set_alarm(bassa_timer *timer, int hours, int mins, int seconds);

void bassa_timer_sleep(time_t sec, long nsec);

#endif //NOC_FILTER_TIMER

