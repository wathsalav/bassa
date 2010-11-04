#ifndef BASSA_TRIG_H
#define BASSA_TRIG_H
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/select.h>

#define BASSA_TRANSACTION_COMPLETE 0
#define BASSA_BAD_SPAWN 1
#define BASSA_KILL_PROC 2
#define BASSA_INIT_PROC 3
#define BASSA_ALARM_FIRED 4
#define BASSA_DB_UPDATED 5

typedef int type_t;
static int refcount = 0;

typedef struct
{
  char *fifo_name;
  int fifo_fd;
  type_t type;
}bassa_trigger;


bassa_trigger* bassa_trigger_new (char *tn);
void bassa_trigger_free (bassa_trigger *t);
int bassa_trigger_sleep (bassa_trigger *t);
int bassa_trigger_wake (bassa_trigger *t, type_t type);

#endif //BASSA_TRIG_H
