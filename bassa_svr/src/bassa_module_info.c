#include "bassa_module_info.h"

bassa_module_info*
bassa_module_info_new (bassa_conf *bconf, bassa_module_table *modtab)
{
  bassa_module_info *modinfo = (bassa_module_info*)malloc 
				(sizeof(bassa_module_info));
  modinfo->prpc = bassa_prp_context_new (bconf);
  modinfo->popc = bassa_pop_context_new (bconf);
  modinfo->modtab = modtab;
  return modinfo;
}

/*
 * Delete all except module table, there is only one module table
 * for the whole system.
 */
void
bassa_module_info_delete (bassa_module_info *modinfo)
{
  
  if (modinfo)
    {
      bassa_pop_context_delete (modinfo->popc);
      bassa_prp_context_delete (modinfo->prpc);
      free (modinfo);
    }
  modinfo = NULL;
}

