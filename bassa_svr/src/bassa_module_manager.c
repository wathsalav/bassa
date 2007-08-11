#include <stdlib.h>
#include <stdio.h>

#include "bassa_module_manager.h"
#include "noc_filter_util.h"

bassa_module_table*
bassa_module_table_new ()
{
  bassa_module_table *modtbl = (bassa_module_table*) 
				malloc (sizeof(bassa_module_table));
  modtbl->rt = (bassa_region_table*)
		malloc (sizeof(bassa_region_table) * N_REGIONS);
  modtbl->region_num = N_REGIONS;
  bassa_module_region region_val;
  int region_count=0;
  for (region_val=PRP_REGION; region_val<=ACT_REGION; region_val++, region_count++)
    {
      modtbl->rt[region_count].region = region_val;
      switch (region_val)
        {
          case PRP_REGION:
            modtbl->rt[PRP_REGION].et = (bassa_exec_table*)
			      malloc (sizeof(bassa_exec_table)*N_PRP_EXECPOINTS);
            modtbl->rt[PRP_REGION].execpoint_num = N_PRP_EXECPOINTS;
            modtbl->rt[PRP_REGION].et[0].exec_point = PRP_QUEUE;
            modtbl->rt[PRP_REGION].et[0].ml = NULL;
	    break;
          case POP_REGION:
            modtbl->rt[POP_REGION].et = (bassa_exec_table*)
			      malloc (sizeof(bassa_exec_table)*N_POP_EXECPOINTS);
            modtbl->rt[POP_REGION].execpoint_num = N_POP_EXECPOINTS;
            modtbl->rt[POP_REGION].et[0].exec_point = POP_STORE;
            modtbl->rt[POP_REGION].et[0].ml = NULL;
            modtbl->rt[POP_REGION].et[1].exec_point = POP_NOTIFY;
            modtbl->rt[POP_REGION].et[1].ml = NULL;
	    break;
          case ACT_REGION:
            modtbl->rt[ACT_REGION].et = (bassa_exec_table*)
			      malloc (sizeof(bassa_exec_table)*N_ACT_EXECPOINTS);
            modtbl->rt[ACT_REGION].execpoint_num = N_ACT_EXECPOINTS;
            modtbl->rt[ACT_REGION].et[0].exec_point = ACT_X;
            modtbl->rt[ACT_REGION].et[0].ml = NULL;
            break;
          case COR_REGION:
            modtbl->rt[COR_REGION].et = (bassa_exec_table*)
			      malloc (sizeof(bassa_exec_table)*N_COR_EXECPOINTS);
            modtbl->rt[COR_REGION].execpoint_num = N_COR_EXECPOINTS;
            modtbl->rt[COR_REGION].et[0].exec_point = COR_X;
            modtbl->rt[COR_REGION].et[0].ml = NULL;
        }
    } 
  return modtbl;
}

void 
bassa_print_modtab (bassa_module_table *mod_tbl)
{
  bassa_module_region region_val;
  int region_count=0;
  int max_ep = 0;
  for (region_val=PRP_REGION; region_val<=ACT_REGION; region_val++, region_count++)
    {
      switch (region_val)
        {
          case PRP_REGION:
            printf ("*************PRP REGION*************\n");
	    max_ep = N_PRP_EXECPOINTS;
            break;
          case POP_REGION:
            printf ("*************POP REGION*************\n");
            max_ep = N_POP_EXECPOINTS;
            break;
          case ACT_REGION:
            printf ("*************ACT REGION*************\n");
            max_ep = N_ACT_EXECPOINTS;
            break;
          case COR_REGION:
            printf ("*************COR REGION*************\n");
            max_ep = N_COR_EXECPOINTS;
        }
      int ep_count;
      for (ep_count = 0; ep_count<max_ep; ep_count++)
        {
          bassa_list *bl = mod_tbl->rt[region_count].et[ep_count].ml;
          while (bl)
            {
              printf ("MODULE LIST #%i: %p\n", ep_count, bl);
              bl = bl->next;
            }
        }
      printf ("************************************\n");
    }
}

int 
bassa_register_module (bassa_module_table *mod_tbl, 
		       char *mod_name, char *mod_path, char *modconf)
{
  bassa_module_context *mc = (bassa_module_context*)
				malloc(sizeof(bassa_module_context));
  bassa_module_region *region = NULL;
  bassa_exec_point *exec_point = NULL;
  mod_exec mexec = NULL;
  mod_exit mexit = NULL;
  mod_init minit = NULL;
  bassa_module_region r;
  bassa_exec_point ep;
  mc->module_path = mod_path;
  mc->module_name = mod_name;
  mc->module_conf = modconf;
  //Open module to see whether it is a bassa module and to know it's region/execution point
  void *mod_hndl = dlopen (mc->module_path, TEST_BINDING);
  if (!mod_hndl)
    {
      free (mc);
      return -1;
    }
  else
    {
      region = (bassa_module_region*)dlsym(mod_hndl, "module_region");
      exec_point = (bassa_exec_point*)dlsym(mod_hndl, "module_execpoint");
      if (*region != COR_REGION && *exec_point != COR_X)
	{ 
	  mexec = (mod_exec)dlsym(mod_hndl, "module_exec");
	  mexit = (mod_exit)dlsym(mod_hndl, "module_exit");
	  minit = NULL;
	}
      else
	{
	  minit = (mod_init)dlsym(mod_hndl, "module_init");
	  mexec = NULL;
	  mexit = NULL;
	}
      r = *region;
      ep = *exec_point;
      if (*region != COR_REGION && *exec_point != COR_X)
	{
	  if (!exec_point || !region || !mexec || !mexit) //It isn't a Bassa object
	    {
	      dlclose (mod_hndl);
	      return -2;
	    }
	}
      else
	{
	  if (!exec_point || !region || !minit) //It isn't a Bassa object
	    {
	      dlclose (mod_hndl);
	      return -2;
	    }
	}
      dlclose (mod_hndl);
    }
  int flag = 0;
  if (r == COR_REGION)
    flag = CORE_BINDING;
  else
    flag = LATE_BINDING;
  mod_hndl = dlopen (mc->module_path, flag);
  if (!mod_hndl)
    {
      free (mc);
      return -1;
    }
  mc->module_handle = mod_hndl;
  if (r != COR_REGION)
    {
      mc->mexec = (mod_exec)dlsym(mod_hndl, "module_exec");
      mc->mexit = (mod_exit)dlsym(mod_hndl, "module_exit");
      mc->minit = NULL;
    }
  else
    {
      mc->minit = (mod_init)dlsym(mod_hndl, "module_init");
      mc->mexec = NULL;
      mc->mexit = NULL;
    }

  unsigned int nrs = mod_tbl->region_num;
  unsigned int r_count;
  for (r_count=0; r_count<nrs; r_count++)
    {
      if (mod_tbl->rt[r_count].region == r)
        break;
    }
  unsigned int neps = mod_tbl->rt[r_count].execpoint_num;
  unsigned int ep_count;
  for (ep_count=0; ep_count<neps; ep_count++)
    {
      if (mod_tbl->rt[r_count].et[ep_count].exec_point == ep)
        break;
    }
  mod_tbl->rt[r_count].et[ep_count].ml = 
  		bassa_list_add (mod_tbl->rt[r_count].et[ep_count].ml, (void*)mc);
  return 0;
}

void
bassa_exec_path (void *regcontx, bassa_module_table *modtbl, 
		 bassa_module_region reg, bassa_exec_point ep)
{
  unsigned int nrs = modtbl->region_num;
  unsigned int r_count;
  for (r_count=0; r_count<nrs; r_count++)
    {
      if (modtbl->rt[r_count].region == reg)
        break;
    }
  unsigned int neps = modtbl->rt[r_count].execpoint_num;
  unsigned int ep_count;
  for (ep_count=0; ep_count<neps; ep_count++)
    {
      if (modtbl->rt[r_count].et[ep_count].exec_point == ep)
        break;
    }
  bassa_list *bl = modtbl->rt[r_count].et[ep_count].ml;
  while (bl)
    {
      bassa_module_context *bmc = (bassa_module_context*)(bl->list_data);
      if (bmc->mexec != NULL)
	bmc->mexec(regcontx, bmc->module_conf);
      else if (bmc->minit != NULL)
	bmc->minit(bmc->module_conf);
      bl = bl->next;
    }
}

void
bassa_modtab_delete (bassa_module_table *mod_tbl)
{
  bassa_module_region region_val;
  int region_count=0;
  int max_ep = 0;
  for (region_val=PRP_REGION; region_val<=ACT_REGION; region_val++, region_count++)
    {
      switch (region_val)
        {
          case PRP_REGION:
            max_ep = N_PRP_EXECPOINTS;
            break;
          case POP_REGION:
            max_ep = N_POP_EXECPOINTS;
            break;
          case ACT_REGION:
            max_ep = N_ACT_EXECPOINTS;
            break;
          case COR_REGION:
            max_ep = N_COR_EXECPOINTS;
        }
      int ep_count;
      for (ep_count = 0; ep_count<max_ep; ep_count++)
        {
          bassa_list *bl = mod_tbl->rt[region_count].et[ep_count].ml;
          bassa_list *dbrowse = bl;
          while (dbrowse)
            {
              bassa_modctx_delete ((bassa_module_context*)(dbrowse->list_data));
	      dbrowse = dbrowse->next;
            }
          bassa_list_delete (bl);
          bl = NULL;
        }
    }
}

void
bassa_close_module (bassa_module_context *modctx)
{
  if (modctx)
    dlclose (modctx->module_handle);
}

void
bassa_modctx_delete (bassa_module_context *modctx)
{
  if (modctx)
    free (modctx);
 modctx = NULL;
}

