/***************************************************************************
 *   Copyright (C) 2007 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#ifndef NOC_FILTER_LIST_H
#define NOC_FILTER_LIST_H

typedef void (*cleanup_handler)(void*);

typedef struct list
{
  void *list_data;
  struct list *next;
  struct list *prev;
  cleanup_handler cleaner;
} bassa_list;

/**
 * Add a new data item to the list, if list is NULL then a list will be created.
 * Address of the list will be returned.
 */
bassa_list* bassa_list_add (bassa_list *list, void* data);
/**
 * Insert a new data item to a given position of a list. If list is NULL a new list will be created and address
 * will be returned. If pos exceeds bounds of list, a address of the unmodified list will be returned otherwise 
 * the address of list will bew returned.
 */
bassa_list* bassa_list_insert (bassa_list *list, void* data, int pos);

/**
 * Remove a data item from a list and return the address of removed list node. If pos exceeds the 
 * list bounds function will return NULL. if list is NULL function will return NULL.
 */
bassa_list* bassa_list_remove (bassa_list *list, int pos);

/**
 * Remove a list node reffered by ref from the list and return the address of removed list node.
 * If red or list is NULL the function will return NULL.  
 */
bassa_list* bassa_list_remove2 (bassa_list *list, void *ref);

/**
 * Get the node at position pos 
 */
bassa_list* bassa_list_getnode (bassa_list *list, int pos);

/**
 * Clean all the memory acquired by the list object.
 */
void bassa_list_delete (bassa_list *list);

/**
 * Create a list node.
 */
bassa_list* create_list_node (void *data);

/**
 * Create a list
 */
bassa_list* bassa_list_new ();

#endif //NOC_FILTER_LIST_H
