/***************************************************************************
 *   Copyright (C) 2006 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk  *
 ***************************************************************************/

#ifndef NOC_FILTER_QUEUE
#define NOC_FILTER_QUEUE

#include <config.h>
#include "noc_filter_user.h"

typedef struct
{
  bassa_request *head;
  bassa_request *tail;
  int element_count;
}bassa_queue;

bassa_queue *bassa_queue_new();

void bassa_queue_enqueue(bassa_request *user, bassa_queue *queue);

bassa_request *bassa_queue_dequeue(bassa_queue *queue);

bassa_request *bassa_queue_try_dequeue(bassa_queue *queue);

void bassa_queue_delete(bassa_queue *queue);

void bassa_queue_reverse_enqueue(bassa_request *user, bassa_queue *queue);

#endif //NOC_FILTER_QUEUE
