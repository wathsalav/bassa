/***************************************************************************
 *   Copyright (C) 2006 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#include <stdlib.h>
#include <pthread.h>

#include "noc_filter_queue.h"
#include "noc_filter_user.h"
#include "noc_filter_concur.h"

bassa_queue *bassa_queue_new()
{
  bassa_queue *queue = (bassa_queue*)malloc(sizeof(bassa_queue));
  queue->element_count = 0;
  return queue;
}

void bassa_queue_enqueue(bassa_request *user, bassa_queue *queue)
{
  if(!(queue->element_count))
    {
      queue->head = user;
      queue->tail = user;
      queue->element_count++;
      user->times_in_queue++;
    }
  else
    {
      queue->head->prev = user;
      user->next = queue->head;
      queue->head = user;
      queue->element_count++;
      user->times_in_queue++;
    }
  queue->head->prev = NULL;
  queue->tail->next = NULL;
  
#ifdef DEBUG
  printf("FILTER_QEUE_ALLOC_INFO\n");
  bassa_request *ux = queue->head;
  int x;
  for(x=0; x<queue->element_count; x++)
    { 
      printf(">>>UX_ADDR: 0X%p\n", ux);
      printf(">>>UX->NEXT_ADDR: 0X%p\n", ux->next);
      printf(">>>UX->PREV_ADDR: 0X%p\n", ux->prev);
      ux = ux->next;
    }
  printf("\n");
#endif
}

bassa_request *bassa_queue_dequeue(bassa_queue *queue)
{
  bassa_request *tmp_user;
  if(!queue)
    return NULL;
  if(!(queue->element_count))
    tmp_user = NULL;
  else
    {
      tmp_user = queue->tail;
      queue->tail = queue->tail->prev;
      queue->element_count--;
      tmp_user->prev = NULL;
      tmp_user->next = NULL;
    }
  return tmp_user;
}

bassa_request *bassa_queue_try_dequeue(bassa_queue *queue)
{
  bassa_request *user;
  if(!(queue->element_count))
    user = NULL;
  else
    user = queue->tail;
  return user;
}

void bassa_queue_delete(bassa_queue *queue)
{
  if(!queue->element_count)
    free(queue);
  else
    {
      bassa_request *ux = queue->head;
      bassa_request *tmp;
      int x;
      for(x=0; x<queue->element_count; x++)
	{ 
	  tmp = ux;
	  ux = ux->next;
	  bassa_request_delete(tmp);
	}
      free(queue);
    }
}

void bassa_queue_reverse_enqueue(bassa_request *user, bassa_queue *queue)
{
  if(!(queue->element_count))
    {
      queue->head = user;
      queue->tail = user;
      queue->element_count++;
      user->times_in_queue++;
    }
  else
    {
      queue->tail->next = user;
      user->prev = queue->tail;
      queue->tail = user;
      queue->element_count++;
      user->times_in_queue++;
    }
  queue->head->prev = NULL;
  queue->tail->next = NULL;
  
#ifdef DEBUG
  printf("FILTER_QEUE_ALLOC_INFO ===> %i\n", queue->element_count);
  bassa_request *ux = queue->head;
  while (ux)
    { 
      printf(">>>UX_ADDR: %p\n", ux);
      printf(">>>UX->NEXT_ADDR: %p\n", ux->next);
      printf(">>>UX->PREV_ADDR: %p\n", ux->prev);
      ux = ux->next;
    }
  printf("\n");
#endif
}
