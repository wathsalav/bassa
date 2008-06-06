/***************************************************************************
 *   Copyright (C) 2007 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#include "bassa_prp_context.h"

bassa_prp_context*
bassa_prp_context_new (const bassa_conf const *bconf)
{
  bassa_prp_context *bpc = (bassa_prp_context*)
				malloc (sizeof(bassa_prp_context));
  bpc->bconf = bconf;
  bpc->queue_pos = 0;
  bpc->restrict_download = 0;
  bpc->routing_url = 0;
  bpc->url = NULL;
  bpc->content_exp = 0;
  bpc->content_exp_time = 0;
  bpc->reset_proxy = 0;
  bpc->proxy = NULL;
  bpc->renamed = 0;
  bpc->new_name = NULL;
  return bpc; 
}

void
bassa_prp_context_delete (bassa_prp_context *bpc)
{
  if (bpc)
    {
      //bpc->new_name is deleted by request
      free (bpc);
      bpc = NULL;
    }
}

