/***************************************************************************
 *   Copyright (C) 2007 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#ifndef NOC_FILTER_LIST_H
#define NOC_FILTER_LIST_H

#define BASSA_LIST_ADD 0
#define BASSA_LIST_REM 1

typedef void (*cleanup_handler)(void*);

typedef struct list
{
  void *list_data;
  struct list *next;
  struct list *prev;
  cleanup_handler cleaner;
} bassa_list;

/**
 * A thread safe function to add and remove elements from list in a multi threaded
 * environment.
 */
bassa_list* bassa_list_adrm_r (bassa_list *list, int op, void *data, int pos);


/**
 * Thread safe function to add a new data item to the list, 
 * if list is NULL then a list will be created.
 * Address of the list will be returned.
 */
bassa_list* bassa_list_add_r (bassa_list *list, void* data);

/**
 * Thread safe function to remove a data item from a list and 
 * return the address of removed list node. If pos exceeds the 
 * list bounds function will return NULL. 
 * If list is NULL function will return NULL.
 */
bassa_list* bassa_list_remove_r (bassa_list *list, int pos);

/**
 * Add a new data item to the list, if list is NULL then a list will be created.
 * Address of the list will be returned. (Not thread safe)
 */
bassa_list* bassa_list_add (bassa_list *list, void* data);

/**
 * Remove a data item from a list and return the address of removed list node. If pos exceeds the 
 * list bounds function will return NULL. if list is NULL function will return NULL.
 * (Not thread safe)
 */
bassa_list* bassa_list_remove (bassa_list *list, int pos);


/**
 * Get the node at position pos 
 */
bassa_list* bassa_list_getnode (bassa_list *list, int pos);

/**
 * Clean all the memory acquired by the list object.
 */
void bassa_list_delete (bassa_list *list);

/**
 * Delete whole list including data if data_cleaner is not NULL
 */
void bassa_list_delete_all (bassa_list *list, void (*data_cleaner)(void*));

/**
 * Create a list node.
 */
bassa_list* create_list_node (void *data);

/**
 * Create a list
 */
bassa_list* bassa_list_new ();

/**
 * Dump the list
 */
void bassa_list_dump (bassa_list *list);

/**
 * Do Base 64 encoding
 */
char* bassa_base64_encode(unsigned char *content, int length);

char* bassa_base64_decode(unsigned char *content, int length);

#endif //NOC_FILTER_LIST_H
