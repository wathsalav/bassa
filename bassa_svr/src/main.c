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
#include "noc_filter_timer.h"
#include "noc_filter_transaction.h"
#include "noc_filter_concur.h"
#include "noc_filter_configure.h"
#include "bassa_sched.h"
#include "bassa_module_manager.h"
#include "bassa_sec.h"
#include "bassa_ws_server.h"

//Location of the default configuration file
#define BASSA_DEFAULT_CONF "/etc/bassa/bassa.conf"

//Bassa usage message
#define BASSA_USAGE "Usage: bassa -c CONFIG_FILE\n Try `bassa --help' for more information\n"

//Bassa help message
#define BASSA_HELP "Usage: bassa -c CONFIG_FILE\nBassa is an offline downloader (policy based)\n\n -c, --config-file\tpass a configuration file,\n\t\t\tdefault is %s\n"

//Global pointer for the configuration object
bassa_conf *conf;

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

void bassa_uinfo (int itype);

/**
 * Register all global vairable and set environment
 */
void bassa_register_globals (bassa_conf *conf);

/**
 * Handles SIGPIPE 
 */
void bassa_sigpipe_handler (int signum);

/**
 * Handles SIGPIPE 
 */
void bassa_sighup_handler (int signum);

/**
 * Check and create database
 */
void bassa_database_check();

int THREAD_COUNT;	 //Number of threads running
int MAX_THREADS = 5;	 //Default maximum number of threads
bassa_conf *conf;	 //Configuration data structure
bassa_module_table *bmt; //Bassa module table


int
main (int argc, char *argv[])
{
  signal (SIGPIPE, bassa_sigpipe_handler);
  signal (SIGHUP, bassa_sighup_handler);
  struct bassa_optstruct *opts = bassa_getopts (argc, argv);
  conf = bassa_parse_configuration (opts->config_file);
  //Set global values
  bassa_register_globals(conf);
  if (!conf)
    {
      printf ("Invalid configuration file: %s\n", 
              opts->config_file);
      exit (-1);
    }
  //Initialize database
  bassa_database_check();
  //Create and load module table
  bmt = bassa_module_table_new ();
  bassa_list *modconflist = conf->cfgcol->modconf_list->next;
  while (modconflist)
    {
      bassa_module_conf *bmc = modconflist->list_data;
      int i = bassa_register_module (bmt,bmc->name,
			     bmc->path, bmc->modconf);
      modconflist = modconflist->next;
    }
  bassa_print_modtab (bmt);
  //Start core region modules
  bassa_exec_path (NULL, bmt, COR_REGION, COR_X);
  //Start active region modules
  bassa_exec_path (NULL, bmt, ACT_REGION, ACT_X);  
  //Start web service
  bassa_nowait_spawn(NULL, bassa_ws_start, NULL);
  //Initialise and start scheduler
  bassa_sched *bsch = bassa_sched_new (conf); 
  int sown = bassa_sec_setowner(conf->svrcfg->server_uid, 
      	                        conf->svrcfg->server_gid,
      			        conf->svrcfg->server_event_bus);
  if (sown < 0)
    printf ("Unable to setowner to %s:%s for %s\n", 
	    conf->svrcfg->server_uid, conf->svrcfg->server_gid,
	    conf->svrcfg->server_event_bus);
  int sid = bassa_sec_setgid(conf->svrcfg->server_gid);
  if (sid < 0)
    printf ("Unable to setgid to %s\n", conf->svrcfg->server_gid);
  sid = bassa_sec_setuid(conf->svrcfg->server_uid);
  if (sid < 0)
    printf ("Unable to setuid to %s\n", conf->svrcfg->server_uid); 
  bassa_sched_start (bsch);
  bassa_modtab_delete (bmt);
  exit (0);
}

void bassa_register_globals (bassa_conf *xconf)
{
  conf = xconf;
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

void bassa_database_check()
{
  bassa_db *dbd = bassa_db_init();
  if (!dbd)
  {
    fprintf (stderr, 
             "Error occured while initializing database\n");
    exit (-1);
  }
  else
  {
    bassa_db_shutdown(dbd);
  }

}

void bassa_sigpipe_handler (int signum)
{
#ifdef DEBUG
  printf ("Handling SIGPIPE\n");
#endif //DEUG
  return;
}

void bassa_sighup_handler (int signum)
{
#ifdef DEBUG
  printf ("Handling SIGPIPE\n");
#endif //DEUG
  return;
}
