/***************************************************************************
 *   Copyright (C) 2007 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/
#include <config.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <dlfcn.h>
#include <getopt.h>
#include <signal.h>

#include "noc_filter_file.h"
#include "noc_filter_queue.h"
#include "noc_filter_timer.h"
#include "noc_filter_transaction.h"
#include "noc_filter_user.h"
#include "noc_filter_server.h"
#include "noc_filter_concur.h"
#include "noc_filter_messages.h"
#include "noc_filter_repo.h"
#include "noc_filter_configure.h"
#include "noc_filter_sched.h"
#include "bassa_module_manager.h"

//prefix of the http protocol
#define HTTP_PROTO_PREFIX "http://"

//Location of the default configuration file
#define BASSA_DEFAULT_CONF "/etc/bassa/bassa.conf"

//Bassa usage message
#define BASSA_USAGE "Usage: bassa -c CONFIG_FILE\n Try `bassa --help' for more information\n"

//Bassa help message
#define BASSA_HELP "Usage: bassa -c CONFIG_FILE\nBassa is an offline downloader (policy based)\n\n -c, --config-file\tpass a configuration file,\n\t\t\tdefault is %s\n"

/**
 * Data for client handler thread
 */
typedef struct 
{
  bassa_client *client; //Pointer to client data structure
  bassa_sched *sched;	//Scheduler data structure
} bassa_inputd;

/**
 * Input data for client accept function 
 */
typedef struct
{
  bassa_sched *sched;  //Pointer to scheduler
  bassa_server *server; //Pointer to server
} bassa_accept_info;

char *sys_addr;
char *sys_proxy;
char *sys_docroot;
int sys_port;

//This structure holds all the variables that are passed as command line arguments.
struct bassa_optstruct 
{
  char *config_file;
};

extern char* optarg;
extern int optind;

/**
 * This function parse all options passed in to the prgram and initialize a opt_struct.
 * @param opts The opt_struct that will be initialized by the funciton.
 * @param argc The number of arguments.
 * @param argv The original arguments list passed to bassa.
 * @return noc_main_opt_struct.
 */
struct bassa_optstruct*
bassa_getopts (int argc, char* argv[]);

/**
 * This function prints informative things as bassa starts
 * @param itype The type of the information required.
 */
void bassa_uinfo (int itype);

/**
 * This function runs on a thread to collect data.
 * @param structure of client and scheduler object.
 */
void *bassa_inop_thread (void *in);

/**
 * Go to an infinte loop and acept all connections.
 */
void *bassa_accept (void *info);

/**
 * Register all global vairable and set environment
 */
void bassa_register_globals (bassa_conf *conf);

/**
 * Handles SIGPIPE 
 */
void bassa_sigpipe_handler (int signum);

int THREAD_COUNT;	 //Number of threads running
int MAX_THREADS = 5;	 //Default maximum number of threads
bassa_conf *conf;	 //Configuration data structure
bassa_module_table *bmt; //Bassa module table


int
main (int argc, char *argv[])
{
  //bassa_block_signal (1,SIGALRM);
  signal (SIGPIPE, bassa_sigpipe_handler);
  struct bassa_optstruct *opts = bassa_getopts (argc, argv);
  conf = bassa_parse_configuration (opts->config_file);
  //Set global values
  bassa_register_globals(conf);
  if (!conf)
    {
      printf ("Invalid configuration file: %s\n", opts->config_file);
      exit (-1);
    }

  
  bmt = bassa_module_table_new ();
  bassa_list *modconflist = conf->cfgcol->modconf_list;
  
  while (modconflist)
    {
      bassa_module_conf *bmc = modconflist->list_data;
      bassa_register_module (bmt,bmc->name, 
			     bmc->path, bmc->modconf);
      modconflist = modconflist->next;
    }
  bassa_print_modtab (bmt);
  //Start core region modules
  bassa_exec_path (NULL, bmt, 
		   COR_REGION, COR_X);
  bassa_sched *scheduler = bassa_sched_new (conf);
  bassa_server *server = NULL;
  if (conf->svrcfg->server_address)
    server = bassa_server_new (conf->svrcfg->server_address, 
			       conf->svrcfg->server_port, conf->svrcfg->server_backlog);
  else
    {
      printf ("Bind Address Not Found:  Please Check %s\n", opts->config_file);
    }
	
  if (conf->svrcfg->max_children)
    MAX_THREADS = conf->svrcfg->max_children;

  bassa_server_bind (server);
  bassa_server_listen (server);
  
  bassa_accept_info acinfo;

  acinfo.sched = scheduler;
  acinfo.server = server;

  int jj = bassa_sched_start (scheduler);
#ifdef DEBUG
  printf ("ALARM RETURNED: %i\n", jj);
#endif

  //Start active region modules
  bassa_exec_path (NULL, bmt, 
		   ACT_REGION, ACT_X);

  bassa_wait_spawn (NULL, bassa_accept, (void *) &acinfo);

  //Shutdown
  bassa_server_shutdown (server);
  bassa_server_delete (server);
  bassa_modtab_delete (bmt);
  //bassa_unblock_signal (1,SIGALRM);
  exit (0);
}

void *bassa_accept (void *info)
{
  bassa_block_signal (1, SIGALRM);
  bassa_client *client;
  bassa_accept_info *acinfo = (bassa_accept_info*)info;
  bassa_server *server = acinfo->server;
  bassa_sched *scheduler = acinfo->sched;
  THREAD_COUNT = 0;
  while ((client = bassa_server_accept (server)))
    {
      bassa_client_settimeout (client, conf->svrcfg->child_timeout);
      bassa_block (THREAD_COUNT, MAX_THREADS);
      bassa_mutex bm;
      bassa_mutex_lock (&bm);
      THREAD_COUNT++;
      bassa_mutex_unlock (&bm);
      bassa_inputd inop;
      inop.sched = scheduler;
      inop.client = client;
      bassa_nowait_spawn (NULL, bassa_inop_thread, (void *) &inop);
      bassa_task_yield ();
    }
  bassa_unblock_signal (1, SIGALRM);
  return NULL;
}

void *bassa_inop_thread (void *in)
{
  bassa_inputd *inop = (bassa_inputd*)in;
  bassa_block_signal (1, SIGALRM);
  //bassa_block_signal (SIGUSR1);
  bassa_client *xclient = (bassa_client *) inop->client;
  char *rec = NULL;
  char *xml = NULL;
  char *ip;
  int xml_len = 0;
  int cr_flag = 1;
  int lf_flag = 1;
  while (cr_flag | lf_flag)
    {
      int count;
      rec = bassa_client_read ((void *) xclient, 10);
      if (rec == NULL)
	break;
      for (count = 0; count < strlen (rec); count++)
	{
	  if ((int) rec[count] == LF)
	    lf_flag = 0;
	  else if ((int) rec[count] == CR)
	    cr_flag = 0;
	  else
	    {
	      cr_flag = 1;
	      lf_flag = 1;
	    }
	}
      if (!xml)
	{
	  xml = (char *) malloc (strlen (rec) + 1);
	  strcpy (xml, rec);
	}
      else
	{
	  xml_len = strlen (rec) + strlen (xml) + 1;
	  xml = (char *) realloc (xml, xml_len);
	  strcat (xml, rec);
	}
      free (rec);
    }
#ifdef DEBUG
  printf ("READ_DONE\n");
  if (xml)
    printf ("%s\n", xml);	
		
#endif //DEBUG
  bassa_messages *bm = bassa_messages_decode (xml);
#ifdef DEBUG
  if (bm)
    {
      if (bm->is_od_message)
	{
	  printf (">>>DLX_UNAME: %s\n", bm->od_message->name);
	  printf (">>>DLX_UEMAIL: %s\n", bm->od_message->email);
	  printf (">>>DLX_UORG_UNIT: %s\n", bm->od_message->org_unit);
	  printf (">>>DLX_UIP: %s\n", bm->od_message->ip);
	  printf (">>>DLX_URL: %s\n", bm->od_message->url);
	  printf (">>>DLX_SIZE: %s\n", bm->od_message->size_str);
	}
      if (bm->is_search_message)
	{
	  printf (">>>SLX_UFILE: %s\n", bm->search_message->file_name);
	}
    }
#endif
  free (xml);
  /*  If the message is not null, check the type os message. If it is a 
   *  download_query, enqueue it. If it is a search request search it in 
   *  repository, generate appropriate XML and send reply to the destination.
   */
  if (bm)
    {
      if (bm->is_od_message)
	{
	  //If we allready have the file ignore request!
	  char *rpath = sys_docroot;
	  bassa_repo *lrepo = bassa_repo_new (rpath);

	  //This is nedded since inet_ntoa() returns a statically 
	  //allocated buffer, subsequent calls will overide it
	  char *tmp_ip = inet_ntoa(xclient->peer_addr.sin_addr);
	  ip = (char*)malloc(strlen(tmp_ip) + 1);
	  strcpy(ip, tmp_ip);

	  //No need to delete tmo_ip since it is statically allocated.
	  long long file_size = 0;
	  if (bm->od_message->size_str)
	    file_size = atol (bm->od_message->size_str);
          bassa_module_info *modinfo = bassa_module_info_new (conf, bmt);
	  bassa_request *user = 
	    bassa_request_new (bm->od_message->name, bm->od_message->email, 
			       ip, bm->od_message->url,
			       file_size, modinfo);
	  if (ip && bm->od_message->url)
	    {
	      bassa_mutex bmut1;
	      bassa_mutex_lock (&bmut1);
#ifdef DEBUG
	      printf ("file_part: %s\n", user->transaction->file_part);
	      printf ("file_complete: %s\n", user->transaction->file_complete);
#endif //DEBUG
	      //Init prp context
	      bassa_request_prp_init (user);
              //Execute module list here.
              bassa_exec_path (modinfo->prpc, modinfo->modtab,
				PRP_REGION, PRP_QUEUE);
	      //Sync PRP data with request
	      bassa_request_prp_sync (user);
	      char *ack = NULL;
	      char *bmsg = NULL;
	      char *xmsg = NULL;
	      if(!(bassa_repo_search (lrepo, bassa_file_get_name (user->transaction->file_complete))
		 || bassa_repo_search (lrepo, bassa_file_get_name(user->transaction->file_part))))
		{
		  bassa_transaction_open (user->transaction);
		  bassa_queue_enqueue (user, inop->sched->gqueue);
		  ack = bassa_ackmessage_encode (OK_STAT, OK_CODE, NULL);
		  bmsg = bassa_message_encode (ack);
		  xmsg = bassa_xml_document_encode (bmsg);
		  if (bassa_client_write (xclient, xmsg) < 1)
		    perror ("ERROR_WRITING");
		  if (bassa_client_write (xclient, "\r\n") < 1)
		  perror ("ERROR_WRITING");
		}
	      else
		{
#ifdef DEBUG
		  printf ("FILE EXIST\n");
#endif //DEBUG
		  ack = bassa_ackmessage_encode (FEXIST_STAT, FEXIST_CODE, NULL);
		  bmsg = bassa_message_encode (ack);
		  xmsg = bassa_xml_document_encode (bmsg);
		  if (bassa_client_write (xclient, xmsg) < 1)
		    perror ("ERROR_WRITING");
		  if (bassa_client_write (xclient, "\r\n") < 1)
		    perror ("ERROR_WRITING");
		  bassa_request_delete (user);
		}
	      free (ack);
	      ack = NULL;
	      free (bmsg);
	      bmsg = NULL;
	      free (xmsg);
	      xmsg = NULL;
	      bassa_mutex_unlock (&bmut1);
	    }
	  else
	    bassa_request_delete(user);
	  bassa_repo_delete (lrepo);
	}
      /*else if (bm->is_search_message)
	{
	  char *repo_path = sys_docroot;
	  if (!bm->search_message->file_name)
	    return NULL;
	  if (repo_path)
	    {
	      bassa_repo *rp = bassa_repo_new (repo_path);
	      if (bassa_repo_search (rp, bm->search_message->file_name))
		{
		  char *url_prefix = getenv ("url_prefix");
		  char *url_found = (char *) malloc (strlen (bm->search_message->file_name) + 
						     strlen (url_prefix) + 1);
		  strcpy (url_found, url_prefix);
		  strcat (url_found, bm->search_message->file_name);
		  char *search_msg = bassa_search_message_encode (url_found);
		  char *bassa_msg = bassa_message_encode (search_msg);
		  char *xml_doc = bassa_xml_document_encode (bassa_msg);
		  if (bassa_client_write (xclient, xml_doc) < 1)
		    perror ("ERROR_WRITING");
		  if (bassa_client_write (xclient, "\r\n") < 1)
		    perror ("ERROR_WRITING");
		  free (url_found);
		  free (search_msg);
		  free (bassa_msg);
		  free (xml_doc);
		}
	      else
		printf ("No such file.\n");
	      bassa_repo_delete (rp);
	    }
	}*/
    }
  bassa_client_close (xclient);
  bassa_client_delete (xclient);
  bassa_messages_delete (bm);
  bassa_mutex bmut2;
  bassa_mutex_lock (&bmut2);
  THREAD_COUNT--;
  bassa_mutex_unlock(&bmut2);
  bassa_unblock_signal (1, SIGALRM);
  return NULL;
}


void bassa_register_globals (bassa_conf *conf)
{
  sys_addr = conf->svrcfg->server_address;
  sys_port = conf->svrcfg->server_port;
  sys_docroot = conf->repocfg->repo_path;
  if (conf->svrcfg->http_proxy_address)
    {
      int proxy_len = strlen(HTTP_PROTO_PREFIX) + strlen(conf->svrcfg->http_proxy_address) + 6;\
      char *proxy = (char*)malloc(proxy_len);
      memset (proxy, (int)'\0', proxy_len);
      sprintf (proxy, "%s%s:%i", HTTP_PROTO_PREFIX, conf->svrcfg->http_proxy_address, conf->svrcfg->http_proxy_port);\
      sys_proxy = proxy;
    }
  else
    sys_proxy = NULL;
}

struct bassa_optstruct*
bassa_getopts (int argc, char* argv[])
{
  struct bassa_optstruct *opts = 
    (struct bassa_optstruct*)malloc (sizeof (struct bassa_optstruct));
  int opt_index = 0;
  int c;
  while (1)
    {
      //int this_optind = optind ? optind : 1;
      opt_index = 0;
      static struct option long_options[] = {
	{"config-file", 1, 0, 'c'},
	{"help", 0, 0, 0},
	{0, 0, 0, 0}
      };
      c = getopt_long (argc, argv, "c:?",
		       long_options, &opt_index);
      if (c == -1)
	break;
      switch (c)
	{
	case 'c':
	  opts->config_file = (char*)malloc (strlen (optarg) + 1);
	  strcpy (opts->config_file, optarg);
	  break;
	case '?':
	  bassa_uinfo (c);
	  exit (-1);
	  break;
	default:
	  bassa_uinfo (0);
	  exit (-1);
	}
    }
  return opts;
}

void bassa_uinfo (int itype)
{
  switch (itype)
    {
    case 0:
      printf (BASSA_USAGE);
      break;
    case '?':
      printf (BASSA_HELP, BASSA_DEFAULT_CONF);
      break;
    }
}

void bassa_sigpipe_handler (int signum)
{
#ifdef DEBUG
  printf ("Handling SIGPIPE\n");
#endif //DEUG
  return;
}
