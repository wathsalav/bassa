#ifndef BASSA_POP_CONTEXT_H
#define BASSA_POP_CONTEXT_H

#include "noc_filter_configure.h"
/**
 * Bassa post processing module context.
 * All pre processing modules should set the values provided in the 
 * data structure. Apart from modifing the data structure given below
 * modules are allowed to change physical attributes of the system like 
 * files and data bases.
 */

typedef struct 
{
  const bassa_conf const *bconf;	//Bassa main configuration
  char *mime_type;			//MIME type of the file
  unsigned int enable_comperssion;	//If 1 encryption is enabled, else dissabled
  int (*compress)(void*);		//Compression function
  unsigned int enable_encryption;	//If 1 encryption is enabled, else dissabled
  unsigned char *enc_key;		//Encryption key
  int (*encrypt)(void*);		//Encryption function
  unsigned int server_response;         //Status code sent by the remote server
  unsigned int status;                  //Bassa status
  char *file_name;                      //File name of the download
  char *url;                            //Where file was stored
  unsigned long long size;              //Size of the file
}bassa_pop_context;

bassa_pop_context*
bassa_pop_context_new (const bassa_conf* const bconf);

void
bassa_pop_context_delete (bassa_pop_context *bpc);
#endif //BASSA_POP_CONTEXT_H
