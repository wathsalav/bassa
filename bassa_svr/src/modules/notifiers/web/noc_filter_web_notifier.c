#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <unistd.h>

#include "noc_filter_web_notifier.h"
#include "bassa_pop_context.h"

bassa_module_region module_region = POP_REGION;
bassa_exec_point module_execpoint = POP_NOTIFY;

int
bassa_notify_web (bassa_web_notification *webnot)
{
if (!webnot)
  return -1;
if (webnot->cache_url == NULL)
  {
    fprintf (stderr, "Notifications Are Useless Without Cache URL\n");
    return -1;
  }
#ifdef DEBUG
  printf ("File Name: %s\nURL: %s\nStatus: %i\nServer Respone: %i\nOrigin URL: %s\nCache URL: %s\n", 
	  webnot->filename, webnot->notifyurl, webnot->status, webnot->server_response,
	  webnot->origurl, webnot->cache_url);
#endif //DEBUG
  if (!(webnot->filename && webnot->notifyurl))
    return -1;
  struct curl_httppost *post = NULL;
  struct curl_httppost *last = NULL;
  char *stat = bassa_get_astatus (webnot->status);
  int i=strlen(webnot->filename)-1;
  char *file_only = NULL;
  for (; i>=0; i--)
    {
      if (webnot->filename[i] == '/')
	break;
      file_only=&(webnot->filename[i]);
    }
  if (!stat)
    return -1;
  char *cache_url = NULL;
  int cache_url_len = strlen (file_only) + strlen(webnot->cache_url) + 2;
  cache_url = (char*)malloc (cache_url_len);
  memset (cache_url, (int)'\0', cache_url_len);
  strcpy (cache_url, webnot->cache_url);
  strcat (cache_url, "/");
  strcat (cache_url, file_only);
  char *xml_buffer =
    (char *) malloc (strlen (NOTIFICATION_XML) + strlen (stat) + strlen(file_only) +
		     strlen (webnot->origurl) + strlen (cache_url) + 1);
  sprintf (xml_buffer, NOTIFICATION_XML, file_only, stat, webnot->origurl, 
	   cache_url, webnot->server_response, webnot->size);
  xml_buffer[strlen(xml_buffer)] = '\0';

#ifdef DEBUG
  printf ("NOTTIFICATION-XML-BUFFER: %s\n", xml_buffer);
#endif
  CURL *curl_handle = curl_easy_init();
  if (!curl_handle)
    return -1;
  bassa_setopts (curl_handle, post, last, FORM_NAME, 
		      xml_buffer, CONTENT_TYPE, webnot->notifyurl, 20);
  int perform, times;
  perform = 0;
  long int round_connect_timeout, sleep_time;

  for (times = 0; times < webnot->maxtries; times++)
    {
      perform = curl_easy_perform(curl_handle);
#ifdef DEBUG
      printf ("Trying %i time(s)...\n", times);
#endif //DEBUG
      if (perform)
	{
	  round_connect_timeout = 
	    (long int)(webnot->timeout / (webnot->maxtries - times));
	  sleep_time = 
	    (long int)(RETRY_GAP_MICROS / (webnot->maxtries - times));
	  bassa_setopts (curl_handle, post, last, FORM_NAME, xml_buffer, 
			      CONTENT_TYPE, webnot->notifyurl, webnot->timeout);
#ifdef DEBUG
	  printf ("CONNECT_TIMEOUT: %li\n", round_connect_timeout);
	  printf ("SLEEP_TIME_GAP: %li\n", sleep_time);
#endif //DEBUG
	  usleep (sleep_time);
	  continue;
	}
      else
	break;
    }
  if (cache_url)
    {
      free (cache_url);
      cache_url = NULL;
    }
  if (xml_buffer)
    {
      free(xml_buffer);
      xml_buffer = NULL;
    }
  bassa_webnotconf_delete (webnot);
  curl_easy_cleanup (curl_handle);
  return perform;
}

char *
bassa_get_astatus (int status)
{
  switch (status)
    {
    case DOWNLOAD_COMPLETE_STAT_ID:
      return DOWNLOAD_COMPLETE_STAT_STR;
    case DOWNLOAD_FAIL_STAT_ID:
      return DOWNLOAD_FAIL_STAT_STR;
    default:
      return NULL;
    }
}

void 
bassa_setopts (CURL *curl_handle, struct curl_httppost *post, struct curl_httppost *last, 
	       char *form_name, char *xml_buffer, char *content_type, char *url, int con_timeout)
{
  curl_formadd (&post, &last, CURLFORM_COPYNAME, form_name,
		     CURLFORM_COPYCONTENTS, xml_buffer,
		     CURLFORM_CONTENTTYPE,
		     content_type, CURLFORM_END);
  curl_easy_setopt (curl_handle, CURLOPT_URL, url);
  curl_easy_setopt (curl_handle, CURLOPT_HTTPPOST, post);
  curl_easy_setopt (curl_handle, CURLOPT_PROXY, getenv("http_proxy"));
  curl_easy_setopt (curl_handle, CURLOPT_CONNECTTIMEOUT, con_timeout);
}

int 
module_exec (void *x, char *conf)
{
  if (!conf)
    return -1;
  bassa_web_notification *webnot = (bassa_web_notification*) 
                                    malloc (sizeof (bassa_web_notification));
  bassa_webnotconf_parse (webnot, conf);
  bassa_pop_context *popc = (bassa_pop_context*)x;
  webnot->filename = strdup(popc->file_name);
  webnot->origurl = strdup(popc->url);
  webnot->status = popc->status;
  webnot->server_response = popc->server_response;
  webnot->size = popc->size;
  bassa_notify_web (webnot);
  return 0;
}

void module_exit (void *x)
{
}

void
bassa_webnotconf_parse (bassa_web_notification *webnot, char *conf)
{
  if (!webnot && !conf)
    return;
  int maxtries = 0;
  int timeout = 0;
  char *cacheurl = NULL;
  char *notifyurl = NULL;
  
  cfg_opt_t opts[] = {
    CFG_SIMPLE_STR("notifyurl", &notifyurl),
    CFG_SIMPLE_STR("cacheurl", &cacheurl),
    CFG_SIMPLE_INT("maxtries", &maxtries),
    CFG_SIMPLE_INT("timeout", &timeout)
  };
  cfg_t *cfg = NULL;
  cfg = cfg_init (opts, 0);
  cfg_parse (cfg, conf);
  webnot->notifyurl = notifyurl;
  webnot->timeout = timeout;
  webnot->maxtries = maxtries;
  webnot->cache_url = cacheurl;
  cfg_free (cfg);
}

void
bassa_webnotconf_delete (bassa_web_notification *webnot)
{
  if (webnot)
    {
      if (webnot->notifyurl)
	{
	  free (webnot->notifyurl);
	  webnot->notifyurl = NULL;
	}
      if (webnot->cache_url)
	{
	  free (webnot->cache_url);
	  webnot->cache_url = NULL;
	}
      if (webnot->filename)
	{
	  free(webnot->filename);
	  webnot->filename = NULL;
	}
      if (webnot->origurl)
	{
	  free(webnot->origurl);
	  webnot->origurl = NULL;
	}
      free (webnot);
      webnot = NULL;
    }
}

