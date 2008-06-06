/***************************************************************************
 *   Copyright (C) 2007 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#ifndef BASSA_MODULE_MANAGER_H
#define BASSA_MODULE_MANAGER_H

#include <dlfcn.h>
#include "noc_filter_util.h"

//Variables accessible for modules
extern char *sys_addr;
extern int sys_port;

#define CORE_BINDING RTLD_NOW | RTLD_GLOBAL
#define TEST_BINDING RTLD_LAZY | RTLD_LOCAL
#define LATE_BINDING RTLD_LAZY | RTLD_GLOBAL
#define EARLY_BINDING RTLD_NOW

#define N_REGIONS 4
typedef enum
{
  PRP_REGION,	//Preprocessing region 
  POP_REGION,	//Post processing region
  COR_REGION,	//Core module region
  ACT_REGION	//Active module region
} bassa_module_region;

#define N_PRP_EXECPOINTS 1
#define N_POP_EXECPOINTS 2
#define N_ACT_EXECPOINTS 1
#define N_COR_EXECPOINTS 1
typedef enum
{
  PRP_QUEUE,	//Exec point before enqueueing 
  POP_STORE,	//Exec point at storage
  POP_NOTIFY, 	//Exec point at notifier
  COR_X,	//One and only one exec point of COR
  ACT_X		//One and onlly one exec point of ACT	
} bassa_exec_point;

typedef int (*mod_exec)(void*, char*);
typedef void (*mod_exit)(void*); 
typedef void (*mod_init)(char*);

typedef struct
{
  void *module_handle;
  char *module_path;
  char *module_name;
  char *module_conf;
  bassa_module_region region;
  bassa_exec_point exec_point;
  mod_exec mexec;
  mod_exit mexit;
  mod_init minit;
}bassa_module_context;


typedef struct
{
  bassa_exec_point exec_point;	//Execution point
  bassa_list *ml;		//Module table
}bassa_exec_table;

typedef struct
{
  unsigned int execpoint_num; 	//Number of execution points
  bassa_module_region region;	//Region
  bassa_exec_table *et;		//execution table
}bassa_region_table;

typedef struct
{
  unsigned int region_num;	//Number of regions
  bassa_region_table *rt;	//Region table
}bassa_module_table;

bassa_module_table*
bassa_module_table_new ();

int 
bassa_register_module (bassa_module_table *mod_tbl,
		       char *mod_name, char *mod_path, char *mod_conf);

void
bassa_close_module (bassa_module_context *modctx);

void 
bassa_print_modtab (bassa_module_table *mod_tbl);

void
bassa_exec_path (void *regcontx, bassa_module_table *modtbl, 
		 bassa_module_region reg, bassa_exec_point ep);

void
bassa_modtab_delete (bassa_module_table *mod_tbl);

void
bassa_modctx_delete (bassa_module_context *modctx);

#endif //BASSA_MODULE_MANAGER_H

