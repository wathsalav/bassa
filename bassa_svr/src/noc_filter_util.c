#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "noc_filter_util.h"

bassa_list* bassa_list_add (bassa_list *list, void* data)
{
  bassa_list *xlist = create_list_node (data);
  if (!list)
    {
      xlist->next = NULL;
      xlist->prev = NULL;
      xlist->list_data = data;
      return xlist;
    }
  else
    {
      bassa_list *tlist = list;
      while (tlist->next)
	tlist = tlist->next;
      tlist->next = xlist;
      xlist->prev = tlist;
      return list;
    }
}

bassa_list* bassa_list_insert (bassa_list *list, void* data, int pos)
{
  if (!list)
    {
      return create_list_node (data);
    }
  else
    {
      bassa_list *xlist = create_list_node (data);
      bassa_list *tlist = list;
      int i = 0;		
      while (tlist)
	{
	  i++;
	  if (i == pos)
	    {
	      xlist->next = tlist;
	      xlist->prev = tlist->prev;
	      if (xlist->prev)
		xlist->prev->next = xlist;
	      if (xlist->next)
		xlist->next->prev = xlist;
	      break;
	    }
	  tlist = tlist->next;
	}
      if (pos == 1)
	return xlist;
      else
	return list;
    }
}


bassa_list* bassa_list_remove (bassa_list *list, int pos)
{
  if (!list)
    {
      return NULL;
    }
  else
    {
      bassa_list *tlist = list;
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
	      return tlist;
	    }
	  i++;
          tlist = tlist->next;
	}
      return NULL;
    }
}


bassa_list* bassa_list_remove2 (bassa_list *list, void *ref)
{
  if (!list)
    {
      return NULL;
    }
  else
    {
      bassa_list *tlist = list;		
      while (tlist)
	{
	  if (tlist->list_data == ref)
	    {
              if (tlist->prev)
	        tlist->prev->next = tlist->next;
              if (tlist->next)
	        tlist->next->prev = tlist->prev;
	      tlist->next = NULL;
	      tlist->prev = NULL;
	      return tlist;
	    }
          tlist = tlist->next;
	}
      return NULL;
    }
}


bassa_list* bassa_list_getnode (bassa_list *list, int pos)
{
  if (!list)
    {
      return NULL;
    }
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
  bassa_list *tlist = list;
  bassa_list *del;
  while (tlist)
    {
      del = tlist;
      tlist = tlist->next;
      free (del);
      del = NULL;
    }		
}

bassa_list* bassa_list_new ()
{
  return create_list_node (NULL);
}
