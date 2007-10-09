/***************************************************************************
 *   Copyright (C) 2007 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#ifndef BASSA_PRP_CONTEXT_H
#define BASSA_PRP_CONTEXT_H
/**
 * Bassa pre processing module context.
 * All pre processing modules should set the values provided in the 
 * data structure. Apart from modifing the data structure given below
 * modules are allowed to change physical attributes of the system like 
 * files and data bases.
 */

#include <stdlib.h>
#include "noc_filter_configure.h"

typedef struct 
{
  const bassa_conf const *bconf;	//Bassa main configuration
  unsigned long int queue_pos;		//Calculated queue position
  unsigned int restrict_download;	//If 0 download is not restricted, else it is restricted
  unsigned int routing_url;		//If 0 original URL will be used, else original URL will be rewritten
  char *url;				//URL that replaces the original URL.
  unsigned int content_exp;		//If 0 the content will not expire.
  unsigned long int content_exp_time; 	//The expiration time of the content
  unsigned int reset_proxy;		//If 0 use original proxy else, reset proxy
  char *proxy;				//Proxy
  char *orig_url;                       //Original URL
  unsigned int renamed;                 //If 0 the file is not renamed, else renamed
  char *new_name;		        //New file name
}bassa_prp_context;

/**
 * Creates a new bassa_prp_context
 */
bassa_prp_context*
bassa_prp_context_new (const bassa_conf const *bconf);

void
bassa_prp_context_delete (bassa_prp_context *bpc);

#endif //BASSA_PRP_CPNTEXT_H

