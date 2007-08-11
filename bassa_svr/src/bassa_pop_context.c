#include <stdlib.h>
#include <string.h>
#include "bassa_pop_context.h"

bassa_pop_context*
bassa_pop_context_new (const bassa_conf const *bconf)
{
  bassa_pop_context *bpc = (bassa_pop_context*)
				malloc(sizeof(bassa_pop_context));
  bpc->bconf = bconf;
  bpc->url = NULL;
  bpc->file_name = NULL;
  bpc->server_response = 0;
  bpc->status = 0;
  bpc->size = 0;
  return bpc;
}


void
bassa_pop_context_delete (bassa_pop_context *bpc)
{
  if (bpc)
    free (bpc);
  bpc = NULL;
}
