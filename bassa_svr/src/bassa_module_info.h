/***************************************************************************
 *   Copyright (C) 2007 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#ifndef BASSA_REGION_CONTEXT_H
#define BASSA_REGION_CONTEXT_H

#include "bassa_pop_context.h"
#include "bassa_prp_context.h"
#include "bassa_module_manager.h"

/**
 * Module data structure for bassa_request
 */
typedef struct
{
  bassa_prp_context *prpc;	//PRP Region context
  bassa_pop_context *popc;	//POP Region context
  bassa_module_table *modtab;	//Module table
} bassa_module_info;

/**
 * Create a new module_info data structure
 * @param bconf Bassa Configuration data structure
 * @param modtab Bassa module table 
 */
bassa_module_info*
bassa_module_info_new (bassa_conf *bconf, bassa_module_table *modtab);

/**
 * Delete module_info data structure and free the memory occupied
 * @param modinfo module_info structure to be deleted
 */
void
bassa_module_info_delete (bassa_module_info *modinfo);
#endif //BASSA_REGION_CONTEXT_H

