/***************************************************************************
 *   Copyright (C) 2007 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#include <stdlib.h>
#include <expat.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "noc_filter_configure.h"

bassa_conf*
bassa_conf_new()
{
  bassa_conf* bc = 
    (bassa_conf*)malloc(sizeof(bassa_conf));
  return bc;
}

bassa_server_conf*
bassa_server_conf_new()
{
  bassa_server_conf* sc =
    (bassa_server_conf*)malloc(sizeof(bassa_server_conf));
  return sc;
}

bassa_downloader_conf*
bassa_downloader_conf_new()
{
  bassa_downloader_conf* dc =
    (bassa_downloader_conf*)malloc(sizeof(bassa_downloader_conf));
  return dc;
}

bassa_repository_conf*
bassa_repository_conf_new()
{
  bassa_repository_conf* rc =
    (bassa_repository_conf*)malloc(sizeof(bassa_repository_conf));
  return rc;
}

bassa_conf_collection*
bassa_conf_collection_new()
{
  bassa_conf_collection* cc =
    (bassa_conf_collection*)malloc(sizeof(bassa_conf_collection));
  cc->modconf_list = NULL;
  cc->out_list = NULL;
  return cc;
}

void
bassa_start_tag_handler(void *udata, char *name, char **attr)
{
  bassa_conf *conf = (bassa_conf*)udata;
  if (!strcmp(name, SERVER_TAG))
    {
      conf->current_tag = SERVER_ID;
      conf->current_config_module = SERVER_CONF_MODULE;
      conf->svrcfg = bassa_server_conf_new();
    }
  else if (!strcmp(name, SERVER_ADDRESS_TAG))
    conf->current_tag = SERVER_ADDRESS_ID;
  else if (!strcmp(name, SERVER_NAME_TAG))
    conf->current_tag = SERVER_NAME_ID;
  else if (!strcmp(name, SERVER_PORT_TAG))
    conf->current_tag = SERVER_PORT_ID;
  else if (!strcmp(name, SERVER_BACKLOG_TAG))
    conf->current_tag = SERVER_BACKLOG_ID;
  else if (!strcmp(name, MAX_CHILDREN_TAG))
    conf->current_tag = MAX_CHILDREN_ID;
  else if (!strcmp(name, CHILD_TIMEOUT_TAG))
    conf->current_tag = CHILD_TIMEOUT_ID;
  else if (!strcmp(name, SERVER_HTTP_PROXY_ADDRESS_TAG))
    conf->current_tag = SERVER_HTTP_PROXY_ADDRESS_ID;
  else if (!strcmp(name, SERVER_HTTP_PROXY_PORT_TAG))
    conf->current_tag = SERVER_HTTP_PROXY_PORT_ID;
	
  if (!strcmp(name, DOWNLOADER_TAG))
    {
      conf->current_tag = DOWNLOADER_ID;
      conf->current_config_module = DOWNLOADER_CONF_MODULE;
      conf->dlcfg = bassa_downloader_conf_new();
    }
  else if (!strcmp(name, DOWNLOADER_ACTIVATION_TIME_TAG))
    conf->current_level = DOWNLOADER_ACTIVATION_TIME_ID;
  else if (!strcmp(name, DOWNLOADER_DEACTIVATION_TIME_TAG))
    conf->current_level = DOWNLOADER_DEACTIVATION_TIME_ID;
  else if (!strcmp(name, HOURS_TAG))
    conf->current_tag = HOURS_ID;
  else if (!strcmp(name, MINUTES_TAG))
    conf->current_tag = MINUTES_ID;
  else if (!strcmp(name, SECONDS_TAG))
    conf->current_tag = SECONDS_ID;
  else if (!strcmp(name, MAX_TRIES_TAG))
    conf->current_tag = MAX_TRIES_ID;
  else if (!strcmp(name, MONITOR_PROGRESS_TAG))
    conf->current_tag = MONITOR_PROGRESS_ID;
  else if (!strcmp(name, DOWNLOADER_QUEUE_LENGTH_TAG))
    conf->current_tag = DOWNLOADER_QUEUE_LENGTH_ID;
  else if (!strcmp(name, CONNECT_TIMEOUT_TAG))
    conf->current_tag = CONNECT_TIMEOUT_ID;
	
	
  if (!strcmp(name, REPOSITORY_TAG))
    {
      conf->current_tag = REPOSITORY_ID;
      conf->current_config_module = REPOSITORY_CONF_MODULE;
      conf->repocfg = bassa_repository_conf_new();
    }
  else if (!strcmp(name, REPOSITORY_PATH_TAG))
    conf->current_tag = REPOSITORY_PATH_ID;
	
  if (!strcmp(name, MODULES_TAG))
    {
      conf->current_tag = MODULES_ID;
      conf->current_config_module = MODULE_CONF_MODULE;
      conf->cfgcol = bassa_conf_collection_new();
    }
  else if (!strcmp(name, MODULE_TAG))
    {
      conf->current_tag = MODULE_ID;
      bassa_module_conf *mconf = (bassa_module_conf*)
				  malloc(sizeof(bassa_module_conf));
      mconf->name = NULL;
      mconf->path = NULL;
      mconf->modconf = NULL;
      conf->cfgcol->out_list = mconf;
    }
  else if (!strcmp(name, MODULE_NAME_TAG))
    conf->current_tag = MODULE_NAME_ID;     
  else if (!strcmp(name, MODULE_PATH_TAG))
    conf->current_tag = MODULE_PATH_ID;
  else if (!strcmp(name, MODULE_CONF_TAG))
    conf->current_tag = MODULE_CONF_ID;
}

void
bassa_stop_tag_handler(void *udata, char *name)
{
  bassa_conf *conf = (bassa_conf*)udata;
  if (!strcmp (name, MODULE_TAG))
    {
      conf->cfgcol->modconf_list = bassa_list_add (conf->cfgcol->modconf_list, 
						     (void*)conf->cfgcol->out_list);
      conf->cfgcol->out_list = NULL;
      /*bassa_list *l = conf->cfgcol->modconf_list;
      while (l)
        {
          printf (">Name: %s\n", ((bassa_module_conf*)(l->list_data))->name);
          printf (">Len : %i\n", strlen(((bassa_module_conf*)(l->list_data))->name));
          printf (">Module: %s\n", ((bassa_module_conf*)(l->list_data))->path);
          printf (">Len : %i\n", strlen(((bassa_module_conf*)(l->list_data))->path));
          printf (">Config: %s\n", ((bassa_module_conf*)(l->list_data))->modconf);
          if (((bassa_module_conf*)(l->list_data))->modconf)
            printf (">Len : %i\n", strlen(((bassa_module_conf*)(l->list_data))->modconf));
          printf ("\n\n");
          l = l->next;
        }*/
    }
  conf->current_tag = -1;
}


void
bassa_conf_cdata_handler (void *udata, char *s, int len)
{
  bassa_conf *conf = (bassa_conf*)udata;
  switch (conf->current_config_module)
    {
    case SERVER_CONF_MODULE :
      bassa_setup_server_configuration (conf, s, len);
      break;
    case DOWNLOADER_CONF_MODULE :
      bassa_setup_downloader_configuration (conf, s, len);
      break;
    case REPOSITORY_CONF_MODULE :
      bassa_setup_repository_configuration (conf, s, len);
      break;
    case MODULE_CONF_MODULE :
      bassa_setup_module_configuration (conf, s, len);
      break;
    }
}

char*
bassa_assemble_configuration (char *element, char *s, int len)
{
  if (!(element))
    {
      element = (char *) malloc (len + 1);
      strncpy (element , s, len);
      int length = len;
      element[length] = '\0';
    }
  else
    {
      int length = strlen (element) + len;
      element = 
	realloc (element, length + 1);
      strncat (element, s, len);
      element[length] = '\0';
    }
  return element;
}

void
bassa_setup_server_configuration (bassa_conf *conf, char *s, int len)
{
  if (conf->current_tag == SERVER_ADDRESS_ID)
    {
      conf->svrcfg->server_address = NULL;
      conf->svrcfg->server_address = 
	bassa_assemble_configuration (conf->svrcfg->server_address, s, len);
#ifdef DEBUG
      printf ("SERVER_ADDRESS: %s\n", conf->svrcfg->server_address);
#endif //DEBUG
    }
  else if (conf->current_tag == SERVER_PORT_ID)
    {	
      char* str_port = NULL;
      str_port = bassa_assemble_configuration (str_port, s, len);
      conf->svrcfg->server_port = atoi(str_port);
#ifdef DEBUG
      printf ("SERVER_PORT: %i\n", conf->svrcfg->server_port);
#endif //DEBUG
      free(str_port);
    }
  else if (conf->current_tag == SERVER_NAME_ID)
    {
      conf->svrcfg->server_name = NULL;
      conf->svrcfg->server_name = 
	bassa_assemble_configuration (conf->svrcfg->server_name, s, len);
#ifdef DEBUG
      printf ("SERVER_NAME: %s\n", conf->svrcfg->server_name);
#endif //DEBUG
    }
  else if (conf->current_tag == SERVER_BACKLOG_ID)
    {
      char* str_bklog = NULL;
      str_bklog = bassa_assemble_configuration (str_bklog, s, len);
      conf->svrcfg->server_backlog = atoi (str_bklog);
      free (str_bklog);
#ifdef DEBUG
      printf ("SERVER_BACKLOG: %i\n", conf->svrcfg->server_backlog );
#endif //DEBUG
    }
  else if (conf->current_tag == SERVER_HTTP_PROXY_ADDRESS_ID)
    {
      conf->svrcfg->http_proxy_address = NULL;
      conf->svrcfg->http_proxy_address = 
	bassa_assemble_configuration (conf->svrcfg->http_proxy_address, s, len);
#ifdef DEBUG
      printf ("HTTP_PROXY: %s\n", conf->svrcfg->http_proxy_address);
#endif //DEBUG
    }
  else if (conf->current_tag == SERVER_HTTP_PROXY_PORT_ID)
    {
      char* str_prport = NULL;
      str_prport = bassa_assemble_configuration (str_prport, s, len);
      conf->svrcfg->http_proxy_port = atoi (str_prport);
#ifdef DEBUG
      printf ("HTTP_PROXY_PORT: %i\n", conf->svrcfg->http_proxy_port);
#endif //DEBUG
      free (str_prport);
    }
  else if (conf->current_tag == MAX_CHILDREN_ID)
    {
      char* str_maxchild = NULL;
      str_maxchild = bassa_assemble_configuration (str_maxchild, s, len);
      conf->svrcfg->max_children = atoi (str_maxchild);
      free (str_maxchild);
#ifdef DEBUG
      printf ("SERVER_MAX_CHILDREN: %i\n", conf->svrcfg->max_children);
#endif //DEBUG
    }
  else if (conf->current_tag == CHILD_TIMEOUT_ID)
    {
      char* str_chltimeout = NULL;
      str_chltimeout = bassa_assemble_configuration (str_chltimeout, s, len);
      conf->svrcfg->child_timeout = atoi (str_chltimeout);
      free (str_chltimeout);
#ifdef DEBUG
      printf ("CHILD_TIMEOUT: %i\n", conf->svrcfg->child_timeout);
#endif //DEBUG
    }
}

void
bassa_setup_downloader_configuration (bassa_conf *conf, char *s, int len)
{
  if (conf->current_tag == HOURS_ID)
    {
      char* str_hours = NULL;
      str_hours = bassa_assemble_configuration (str_hours, s, len);
      if (conf->current_level == DOWNLOADER_ACTIVATION_TIME_ID)
	conf->dlcfg->hours = atoi (str_hours);
      else if (conf->current_level == DOWNLOADER_DEACTIVATION_TIME_ID)
	conf->dlcfg->dhours = atoi (str_hours);
      free (str_hours);
#ifdef DEBUG
      printf ("HOURS: %i\n", conf->dlcfg->hours);
      printf ("HOURS: %i\n", conf->dlcfg->dhours);
#endif //DEBUG
    }
  else if (conf->current_tag == MINUTES_ID)
    {
      char* str_mins = NULL;
      str_mins = bassa_assemble_configuration (str_mins, s, len);
      if (conf->current_level == DOWNLOADER_ACTIVATION_TIME_ID)
	conf->dlcfg->minutes = atoi (str_mins);
      else if (conf->current_level == DOWNLOADER_DEACTIVATION_TIME_ID)
	conf->dlcfg->dminutes = atoi (str_mins);
      free (str_mins);
#ifdef DEBUG
      printf ("MINUTES: %i\n", conf->dlcfg->minutes);
      printf ("MINUTES: %i\n", conf->dlcfg->dminutes);
#endif //DEBUG
    }
  else if (conf->current_tag == SECONDS_ID)
    {
      char* str_secs = NULL;
      str_secs = bassa_assemble_configuration (str_secs, s, len);
      if (conf->current_level == DOWNLOADER_ACTIVATION_TIME_ID)
	conf->dlcfg->seconds = atoi (str_secs);
      else if (conf->current_level == DOWNLOADER_DEACTIVATION_TIME_ID)
	conf->dlcfg->dseconds = atoi (str_secs);
      free (str_secs);
#ifdef DEBUG
      printf ("SECONDS: %i\n", conf->dlcfg->seconds);
      printf ("SECONDS: %i\n", conf->dlcfg->dseconds);
#endif //DEBUG
    }
  else if (conf->current_tag == MAX_CHILDREN_ID)
    {
      char* str_chld = NULL;
      str_chld = bassa_assemble_configuration (str_chld, s, len);
      conf->dlcfg->max_children = atoi (str_chld);
      free (str_chld);
#ifdef DEBUG
      printf ("DOWNLOADER_MAX_CHILD: %i\n", conf->dlcfg->max_children);
#endif //DEBUG
    }
  else if (conf->current_tag == DOWNLOADER_QUEUE_LENGTH_ID)
    {
      char* str_queuelen = NULL;
      str_queuelen = bassa_assemble_configuration (str_queuelen, s, len);
      conf->dlcfg->max_queue_len = atoi (str_queuelen);
      free (str_queuelen);
#ifdef DEBUG
      printf ("DOWNLOADER_QUEUE_LENGTH: %i\n", conf->dlcfg->max_queue_len);
#endif //DEBUG
    }
  else if (conf->current_tag == CONNECT_TIMEOUT_ID)
    {
      char* ctimeout = NULL;
      ctimeout = bassa_assemble_configuration (ctimeout, s, len);
      conf->dlcfg->connect_timeout = atoi(ctimeout);
#ifdef DEBUG
      printf ("CONNECT_TIMEOUT: %i\n", conf->dlcfg->connect_timeout);
#endif //DEBUG
    }
  else if (conf->current_tag == MAX_TRIES_ID)
    {
      char* maxt = NULL;
      maxt = bassa_assemble_configuration (maxt, s, len);
      conf->dlcfg->max_tries = atoi(maxt);
#ifdef DEBUG
      printf ("DOWNLOAD_TRIES: %i\n", conf->dlcfg->max_tries);
#endif //DEBUG
    }
}

void
bassa_setup_repository_configuration (bassa_conf *conf, char *s, int len)
{
  if (conf->current_tag == REPOSITORY_PATH_ID)
    {
      conf->repocfg->repo_path = 
	bassa_assemble_configuration (conf->repocfg->repo_path, s, len);
#ifdef DEBUG
      printf ("REPOSITORY_PATH: %s\n", conf->repocfg->repo_path);
#endif //DEBUG
    }
}

void
bassa_setup_module_configuration (bassa_conf *conf, char *s, int len)
{
  if (conf->current_tag == MODULE_NAME_ID)
    {
      conf->cfgcol->out_list->name = 
	bassa_assemble_configuration (conf->cfgcol->out_list->name, s, len);
    }
  else if (conf->current_tag == MODULE_PATH_ID)
    {
      conf->cfgcol->out_list->path = 
	bassa_assemble_configuration (conf->cfgcol->out_list->path, s, len);
    }
  else if (conf->current_tag == MODULE_CONF_ID)
    {
      conf->cfgcol->out_list->modconf = 
	bassa_assemble_configuration (conf->cfgcol->out_list->modconf, s, len);
    }
}

bassa_conf*
bassa_parse_configuration (char *file_name)
{
  int conf_fd = open (file_name, O_RDONLY);
  if (conf_fd == -1)
    {
      printf ("Could not open the configuration file\n");
      return NULL;
    }
  char *tmp_buffer = (char*)malloc (XML_READER_BUFFER_SIZE);
  char *xml_buffer = NULL;
  int byte_count = 0;
  int runs = 0;
  while((byte_count = read(conf_fd, tmp_buffer, XML_READER_BUFFER_SIZE)))
    {
      if (!runs)
	{
	  xml_buffer = (char*)malloc (byte_count + 1);
	  strncpy (xml_buffer, tmp_buffer, byte_count);
	  xml_buffer[byte_count] = '\0';
	  runs++;
	}
      else
	{
	  xml_buffer = (char*)realloc (xml_buffer, strlen (xml_buffer) + byte_count + 1);
	  strncat (xml_buffer, tmp_buffer, byte_count);
	}
      memset ((void*)tmp_buffer, (int)'\0', byte_count);
    }
  free (tmp_buffer);
  XML_StartElementHandler sh = (XML_StartElementHandler)bassa_start_tag_handler;
  XML_EndElementHandler eh = (XML_EndElementHandler)bassa_stop_tag_handler;
  XML_CharacterDataHandler ch = (XML_CharacterDataHandler)bassa_conf_cdata_handler;
	
  bassa_conf *conf = bassa_conf_new ();
  XML_Parser parser = XML_ParserCreate (NULL);
  XML_SetUserData (parser, (void *) conf);
  XML_SetStartElementHandler (parser, sh);
  XML_SetEndElementHandler (parser, eh);
  XML_SetCharacterDataHandler (parser, ch);
  XML_Parse (parser, xml_buffer, strlen (xml_buffer), 1);
  XML_ParserFree (parser);
  free (xml_buffer);
	
  return conf;
}
