/***************************************************************************
 *   Copyright (C) 2007 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#include <bassa_db.h>
#include <bassa_module_manager.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bassa_module_region module_region = PRP_REGION;
bassa_exec_point module_execpoint = PRP_QUEUE;

int module_exec (void *ctx);
void module_close (void *ctx);

extern char* mod_crypt_getsha1 (char*);
int 
module_exec (void *ctx)
{
  bassa_object *bobj = (bassa_object*)ctx;
  if (!bobj)
    return -1;
  if (!bobj->origin_url)
    return -1;
  char *sha1 = mod_crypt_getsha1 (bobj->origin_url);
  int len = strlen(conf->repocfg->repo_path) + strlen (sha1) + 2;
  char *hashed_fname = (char*)malloc(len);
  memset (hashed_fname, (int)'\0', len);
  strcpy (hashed_fname, conf->repocfg->repo_path);
  strcat (hashed_fname, "/");
  strcat (hashed_fname, sha1);
  if (bobj->object_path)
  {
    free(bobj->object_path);
    bobj->object_path = NULL;
  }
  bobj->object_path = hashed_fname;
  len = strlen(conf->repocfg->url) + strlen (sha1) + 2;
  char *hashed_url = (char*)malloc(len);
  memset (hashed_url, (int)'\0', len);
  strcpy (hashed_url, conf->repocfg->url);
  strcat (hashed_url, "/");
  strcat (hashed_url, sha1);
  if (bobj->object_url)
  {
    free(bobj->object_url);
    bobj->object_url = NULL;
  }
  bobj->object_url = hashed_url;
  return 0;
}

void 
module_exit (void *ctx)
{
  //Nothing to be done here...
}

