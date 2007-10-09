/***************************************************************************
 *   Copyright (C) 2006 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "noc_filter_user.h"
#include "noc_filter_transaction.h"
#include "noc_filter_file.h"

extern char *sys_docroot;

bassa_request *
bassa_request_new (char *name, char *email, char *ip, 
		   char *url, long long size, bassa_module_info *modinfo)
{
#ifdef DEBUG
  printf ("USER_NEW\n");
#endif //DEBUG
  bassa_request *user = (bassa_request *) malloc (sizeof (bassa_request));
  user->name = name;
  user->email = email;
  user->ip = ip;
  user->url = url;
  user->times_in_queue = 0;
  user->size = size;
  char *local_path = NULL;
  char *file_name = bassa_file_get_name (url);
  char *path = sys_docroot;
  if (file_name)
    {
      local_path = (char *) malloc (strlen (file_name) + strlen (path) + 2);
      strcpy (local_path, path);
      strcat (local_path, "/");
      strcat (local_path, file_name);
      free (file_name);
    }
  user->transaction = bassa_transaction_new (url, local_path, size/*, http_proxy*/);
  user->modinfo = modinfo; 
  return user;
}

void
bassa_request_prp_sync (bassa_request *request)
{
  char *proxy = NULL;
  char *file_name = NULL;
  char *local_path = NULL;
  char *path;
  if (request == NULL)
    return;
  if (request->modinfo == NULL)
    return;
  if (request->modinfo->prpc == NULL)
    return;
  if (request->modinfo->prpc->routing_url != 0)
    request->url = request->modinfo->prpc->url;
  if (request->modinfo->prpc->reset_proxy != 0)
    proxy = request->modinfo->prpc->proxy;
  else
    proxy = request->transaction->http_proxy;
  if (request->modinfo->prpc->renamed == 0)
    file_name = bassa_file_get_name (request->url);
  else
    file_name = request->modinfo->prpc->new_name;
  path = sys_docroot;
  if (file_name)
    {
      local_path = (char *) malloc (strlen (file_name) + strlen (path) + 2);
      strcpy (local_path, path);
      strcat (local_path, "/");
      strcat (local_path, file_name);
      free (file_name);
      file_name = NULL;
    }
  bassa_transaction_prp_reinit (request->transaction, request->url, local_path, request->size,
				proxy);
}

void
bassa_request_pop_sync (bassa_request *request)
{
  if (request == NULL)
    return;
  if (request->modinfo == NULL)
    return;
  if (request->modinfo->popc == NULL)
    return;
  if (request->transaction == NULL)
    return;
  request->modinfo->popc->file_name = request->transaction->file_complete;
  request->modinfo->popc->url = request->transaction->url;
  request->modinfo->popc->status = request->transaction->bassa_status;
  request->modinfo->popc->server_response = GET_STATUS(request->transaction->http_bf);
  request->modinfo->popc->size = request->size;
}

void
bassa_request_prp_init (bassa_request *request)
{
  if (request == NULL)
    return;
  if (request->modinfo == NULL)
    return;
  if (request->modinfo->prpc == NULL)
    return;
  request->modinfo->prpc->orig_url = request->url;
  return;
}

void
bassa_request_delete (bassa_request * user)
{
#ifdef DEBUG
  printf ("DELETING REQUEST\n");
#endif //DEBUG
  bassa_transaction_delete (user->transaction);
  bassa_module_info_delete (user->modinfo);
  free (user);
}
