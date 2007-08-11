/***************************************************************************
 *   Copyright (C) 2006 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#ifndef NOC_FILTER_USER_H
#define NOC_FILTER_USER_H

#include "noc_filter_transaction.h"
#include "bassa_module_info.h"

typedef struct bassa_request
{
  char *name;
  char *email;
  char *ip;
  char *url;
  bassa_transaction *transaction;
  struct bassa_request *next;
  struct bassa_request *prev;
  int times_in_queue;
  unsigned long long size;
  unsigned long long downloaded_size;
  bassa_module_info *modinfo;
} bassa_request;

//typedef struct _bassa_user bassa_user;

bassa_request*
bassa_request_new(char *name, char *email, char *ip, 
		  char *url, long long size, bassa_module_info *modinfo);

void
bassa_request_prp_sync (bassa_request *request);

void 
bassa_request_delete(bassa_request *user);

void
bassa_request_pop_sync (bassa_request *request);

void
bassa_request_prp_init (bassa_request *request);
#endif //NOC_FILTER_USER_H
