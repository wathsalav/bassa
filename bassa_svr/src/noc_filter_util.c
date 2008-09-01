/***************************************************************************
 *   Copyright (C) 2007 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <noc_filter_concur.h>
#include "noc_filter_util.h"
#include "bassa_db.h"

bassa_list* bassa_list_adrm_r (bassa_list *list, int op, void *data, int pos)
{
  bassa_mutex *bm = bassa_mutex_new();
  bassa_mutex_lock(bm);
  switch (op)
  {
    case BASSA_LIST_ADD:
      return bassa_list_add (list, data);
    case BASSA_LIST_REM:
      return bassa_list_remove (list, pos);
  }
  bassa_mutex_unlock(bm);
  bassa_mutex_free(bm);
  bm = NULL;
  return NULL;
}


bassa_list* bassa_list_add (bassa_list *list, void* data)
{
#ifdef DEBUG
  printf ("ELEMENT ADDR ADDING %p\n", data);
  printf ("LIST ADDRESS %p\n", list);
#endif //DEBUG
  if (list)
  {
    bassa_list *xlist = create_list_node (data);
    bassa_list *tlist = list;
    while (tlist->next)
      tlist = tlist->next;
    tlist->next = xlist;
    xlist->prev = tlist;
    return list;
  }
  else
    return NULL;
}



bassa_list* bassa_list_remove (bassa_list *list, int pos)
{
  if (!list)
    return NULL;
  bassa_list *tlist = list->next;
  if (!tlist)
  {
    return NULL;
  }
  else
  {
    int i = 0;		
    while (tlist)
    {
      if (i == pos)
      {
	if (tlist->prev)
	  tlist->prev->next = tlist->next;
	if (tlist->next)
	  tlist->next->prev = tlist->prev;
	tlist->next = NULL;
	tlist->prev = NULL;
#ifdef DEBUG
	printf ("ELEMENT ADDR REMOVING %p\n", tlist);
	printf ("LIST ADDRESS %p\n", list);
#endif //DEBUG
	return tlist;
      }
      i++;
      tlist = tlist->next;
    }
    return NULL;
  }
}

bassa_list* bassa_list_add_r (bassa_list *list, void* data)
{
  bassa_mutex *bm = bassa_mutex_new();
  printf ("ELEMENT ADDR ADDING %p\n", data);
  printf ("LIST ADDRESS %p\n", list);
  if (list)
  {
    bassa_mutex_lock(bm);
    bassa_list *xlist = create_list_node (data);
    bassa_list *tlist = list;
    while (tlist->next)
      tlist = tlist->next;
    tlist->next = xlist;
    xlist->prev = tlist;
    bassa_mutex_unlock(bm);
    bassa_mutex_free(bm);
    bm = NULL;
    return list;
  }
  else
    return NULL;
}



bassa_list* bassa_list_remove_r (bassa_list *list, int pos)
{
  if (!list)
    return NULL;
  bassa_mutex *bm = bassa_mutex_new();
  bassa_mutex_lock(bm);
  bassa_list *tlist = list->next;
  if (!tlist)
  {
    //Unlock the mutex locked before
    bassa_mutex_unlock(bm);
    bassa_mutex_free(bm);
    bm = NULL;
    return NULL;
  }
  else
  {
    int i = 0;		
    while (tlist)
    {
      if (i == pos)
      {
	if (tlist->prev)
	  tlist->prev->next = tlist->next;
	if (tlist->next)
	  tlist->next->prev = tlist->prev;
	tlist->next = NULL;
	tlist->prev = NULL;
	printf ("ELEMENT ADDR REMOVING %p\n", tlist);
	printf ("LIST ADDRESS %p\n", list);
	//Unlock the previously locked mutex
	bassa_mutex_unlock(bm);
	bassa_mutex_free(bm);
	bm = NULL;
	return tlist;
      }
      i++;
      tlist = tlist->next;
    }
    //Unlock the previously locked mutex before leaving
    bassa_mutex_unlock(bm);
    bassa_mutex_free(bm);
    bm = NULL;
    return NULL;
  }
}




bassa_list* bassa_list_getnode (bassa_list *list, int pos)
{
  if (!list)
    return NULL;
  else
  {
    bassa_list *tlist = list;
    int i = 0;		
    while (tlist)
    {
      if (i == pos)
	return tlist;
      i++;
      tlist = tlist->next;
    }
    return NULL;
  }
}

bassa_list* create_list_node (void *data)
{
  bassa_list *list = (bassa_list*)malloc(sizeof(bassa_list));
  list->list_data = data;
  list->next = NULL;
  list->prev = NULL;
  return list;
}

void bassa_list_delete (bassa_list *list)
{
  bassa_list *del = list;
  if (del)
  {
    free (del);
    del = NULL;
  }		
}

void bassa_list_delete_all (bassa_list *list, void (*data_cleaner)(void*))
{
  if (!list)
    return;
  bassa_list *del = list->next;
  while (del)
  {
    bassa_list *next = del->next;
    if (data_cleaner)
    {
      data_cleaner(del->list_data);
    }
    bassa_list_delete(del);
    del = next;
  }
}

bassa_list* bassa_list_new ()
{
  return create_list_node (NULL);
}

void bassa_list_dump (bassa_list *list)
{
  if (!list)
    return;
  if (!(list->next))
    return;
  bassa_list *tlist = list->next;
  while(tlist)
  {
    printf ("Elemet: %p\n", tlist);
    tlist = tlist->next;
  }
}

