#include <bassa_prp_context.h>
#include <bassa_module_manager.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bassa_module_region module_region = PRP_REGION;
bassa_exec_point module_execpoint = PRP_QUEUE;

int module_exec (void *ctx);
void module_close (void *ctx);

extern char* bassa_file_get_name (char*);
extern char* mod_crypt_getsha1 (char*);
int 
module_exec (void *ctx)
{
  bassa_prp_context *prpc = (bassa_prp_context*)ctx;
  prpc->renamed = 1;
  char *fname = bassa_file_get_name (prpc->orig_url);
  char *sha1 = mod_crypt_getsha1 (prpc->orig_url);
  int len = strlen (fname) + strlen (sha1) + 2;
  char *hashed_fname = (char*)malloc(len);
  memset (hashed_fname, (int)'\0', len);
  strcpy (hashed_fname, sha1);
  strcat (hashed_fname, "_");
  strcat (hashed_fname, fname);
  prpc->new_name = hashed_fname;
  return 0;
}

void 
module_exit (void *ctx)
{
  //Nothing to be done here...
}

