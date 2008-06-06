/***************************************************************************
 *   Copyright (C) 2007 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <unistd.h>
#include <sys/wait.h>

#include "mod_flv.h"
#include "bassa_pop_context.h"
#include "noc_filter_file.h"

bassa_module_region module_region = POP_REGION;
bassa_exec_point module_execpoint = POP_NOTIFY;

int
bassa_notify_metavideo (bassa_video2flv *v2f)
{
#ifdef DEBUG
  printf ("File Name: %s\nMeta File Name: %s\nURL: %s\nOrigin URL: %s\nMeta URL: %s\n",
	  v2f->filename, v2f->meta_filename, v2f->notifyurl, v2f->origurl, v2f->metaurl);
#endif //DEBUG
  if (!(v2f->filename && v2f->meta_filename && v2f->notifyurl))
    return -1;
  struct curl_httppost *post = NULL;
  struct curl_httppost *last = NULL;
  int i=strlen(v2f->meta_filename)-1;
  char *file_only = NULL;
  for (; i>=0; i--)
    {
      if (v2f->meta_filename[i] == '/')
	break;
      file_only=&(v2f->meta_filename[i]);  
    }
  char *meta_url = NULL;
  int meta_url_len = strlen (file_only) + strlen(v2f->metaurl) + 2;
  meta_url = (char*)malloc (meta_url_len);
  memset (meta_url, (int)'\0', meta_url_len);
  strcpy (meta_url, v2f->metaurl);
  strcat (meta_url, "/");
  strcat (meta_url, file_only);
  char *xml_buffer =
    (char *) malloc (strlen (NOTIFICATION_XML) + strlen(file_only) +
		     strlen (v2f->origurl) + strlen (meta_url) + 1);
  sprintf (xml_buffer, NOTIFICATION_XML, file_only, v2f->origurl,
	   meta_url);
  xml_buffer[strlen(xml_buffer)] = '\0';

#ifdef DEBUG
  printf ("META-NOTIFICATION-XML-BUFFER: %s\n", xml_buffer);
#endif
  CURL *curl_handle = curl_easy_init();
  if (!curl_handle)
    return -1;
  bassa_setopts (curl_handle, post, last, FORM_NAME,
		 xml_buffer, CONTENT_TYPE, v2f->notifyurl, 20,
		 v2f->http_proxy);
  int perform, times;
  perform = 0;
  long int round_connect_timeout, sleep_time;

  for (times = 0; times < v2f->maxtries; times++)
    {
      perform = curl_easy_perform(curl_handle);
#ifdef DEBUG
      printf ("Trying %i time(s)...\n", times);
#endif //DEBUG
      if (perform)
	{
	  round_connect_timeout =
	    (long int)(v2f->timeout / (v2f->maxtries - times));
	  sleep_time =
	    (long int)(RETRY_GAP_MICROS / (v2f->maxtries - times));
	  bassa_setopts (curl_handle, post, last, FORM_NAME, xml_buffer,
			 CONTENT_TYPE, v2f->notifyurl, v2f->timeout,
			 v2f->http_proxy);
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
  if (meta_url)
    {
      free (meta_url);
      meta_url = NULL;
    }
  if (xml_buffer)
    {
      free(xml_buffer);
      xml_buffer = NULL;
    }
  curl_easy_cleanup (curl_handle);
  return perform;
  return 0;
}

void 
bassa_setopts (CURL *curl_handle, struct curl_httppost *post, struct curl_httppost *last, 
	       char *form_name, char *xml_buffer, char *content_type, char *url, int con_timeout,
	       char *proxy)
{
  curl_formadd (&post, &last, CURLFORM_COPYNAME, form_name,
		     CURLFORM_COPYCONTENTS, xml_buffer,
		     CURLFORM_CONTENTTYPE,
		     content_type, CURLFORM_END);
  curl_easy_setopt (curl_handle, CURLOPT_URL, url);
  curl_easy_setopt (curl_handle, CURLOPT_HTTPPOST, post);
  curl_easy_setopt (curl_handle, CURLOPT_PROXY, proxy);
  curl_easy_setopt (curl_handle, CURLOPT_CONNECTTIMEOUT, con_timeout);
}

__attribute__((constructor))void init_mod_flv ()
{

}

__attribute__((destructor))void fini_mod_flv ()
{

}

int 
module_exec (void *x, char *conf)
{
  if (!conf)
    return -1;
  bassa_pop_context *popc = (bassa_pop_context*)x;
  char *mime_type = mod_mime_gettype (popc->file_name);
  if (mime_type)
    {
      if (strstr(mime_type, "video") == NULL)
	return -1; 
    }
  else
    return -1;
  free (mime_type);
  mime_type = NULL;
  bassa_video2flv *v2f = (bassa_video2flv*)malloc (sizeof(bassa_video2flv));
  v2f->filename = strdup(popc->file_name);
  v2f->origurl = strdup(popc->url);
  bassa_video2flv_parse (v2f, conf);
  bassa_nowait_spawn (NULL, bassa_video_convert, v2f);
  return 0;
}

void module_exit (void *x)
{
}

void
bassa_video2flv_parse (bassa_video2flv *v2f, char *conf)
{
  if (!v2f && !conf)
    return;
  int maxtries = 0;
  int timeout = 0;
  char *metaurl = NULL;
  char *notifyurl = NULL;
  char *length = NULL;
  char *audio_codec = NULL;
  char *sample_freq = NULL;
  char *audio_bitrate = NULL;
  char *video_size = NULL;
  char *meta_repo = NULL;
  char *http_proxy = NULL;

  cfg_opt_t opts[] = {
    CFG_SIMPLE_STR("notify_url", &notifyurl),
    CFG_SIMPLE_STR("meta_url", &metaurl),
    CFG_SIMPLE_INT("maxtries", &maxtries),
    CFG_SIMPLE_INT("timeout", &timeout),
    CFG_SIMPLE_STR("length", &length),
    CFG_SIMPLE_STR("audio_codec", &audio_codec),
    CFG_SIMPLE_STR("sample_freq", &sample_freq),
    CFG_SIMPLE_STR("audio_bitrate", &audio_bitrate),
    CFG_SIMPLE_STR("video_size", &video_size),
    CFG_SIMPLE_STR("meta_repo", &meta_repo),
    CFG_SIMPLE_STR("http_proxy", &http_proxy)
  };
  cfg_t *cfg;
  cfg = cfg_init (opts, 0);
  cfg_parse (cfg, conf);
  v2f->notifyurl = notifyurl;
  v2f->metaurl = metaurl;
  v2f->maxtries = maxtries;
  v2f->timeout = timeout;
  if (!strcmp(length,FULL_ENCODING))
    v2f->flv_len = NULL;
  else
    {
      v2f->flv_len = length;
    }
  v2f->acodec = audio_codec;
  v2f->ar = sample_freq;
  v2f->ab = audio_bitrate;
  v2f->size = video_size;
  v2f->metarepo = meta_repo;
  v2f->http_proxy = http_proxy;
  cfg_free (cfg);
}

void*
bassa_video_convert (void *v2f)
{
  bassa_block_signal (SIGALRM);
  if (v2f == NULL)
    return NULL;
  bassa_video2flv *xv2f = (bassa_video2flv*)v2f;
  char *filename = xv2f->filename;
  int new_name_len = strlen(filename)+strlen(FLV_EXT)+2;
  char *new_name = (char*)malloc(new_name_len);
  memset(new_name, (int)'\0', new_name_len);
  strncpy(new_name, filename,strlen(filename));
  strcat(new_name, ".");
  strcat(new_name, FLV_EXT);
  char *flv_name = strdup(bassa_file_get_name (new_name));
  int flv_path_len = strlen(flv_name) + strlen(xv2f->metarepo) + 1;
  char *flv_path = (char*)malloc (flv_path_len);
  memset (flv_path, (int)'\0', flv_path_len);
  strcpy (flv_path, xv2f->metarepo);
  strcat (flv_path, flv_name);
  xv2f->meta_filename = flv_path;
  if (new_name)
    free(new_name);
  if (flv_name)
    free(flv_name);
  pid_t pid = fork();  
  if (pid == 0)
    {
#ifndef DEBUG
      int null_fd = open ("/dev/null", O_WRONLY);
      int stderr_fd = fileno (stderr);
      int stdout_fd = fileno (stdout);
      int stdin_fd = fileno (stdin);
      stdout_fd = dup2 (null_fd, stdout_fd);
      stderr_fd = dup2 (null_fd, stderr_fd);
      stdin_fd = dup2 (null_fd, stdin_fd);
#else
      //Dump parameters
      printf ("%s", FLV_MAKER);
      printf (" %s %s", FLV_MAKER_INPUT, filename);
      printf (" %s %s", FLV_MAKER_DURATION, xv2f->flv_len);
      printf (" %s %s", FLV_MAKER_AC, xv2f->acodec);
      printf (" %s %s", FLV_MAKER_AR, xv2f->ar);
      printf (" %s %s", FLV_MAKER_AB, xv2f->ab);
      printf (" %s %s", FLV_MAKER_FORMAT, FLV_EXT);
      printf (" %s %s", FLV_MAKER_SIZE, xv2f->size);
      printf (" %s", FLV_MAKER_OVERWRITE);
      printf (" %s\n", xv2f->meta_filename);
#endif //DEBUG
      if (xv2f->flv_len)
	{
	  char *args[] = {
	    FLV_MAKER,
	    FLV_MAKER_INPUT, filename,
	    FLV_MAKER_DURATION, xv2f->flv_len,
	    FLV_MAKER_AC, xv2f->acodec,
	    FLV_MAKER_AR, xv2f->ar,
	    FLV_MAKER_AB, xv2f->ab,
	    FLV_MAKER_FORMAT, FLV_EXT,
	    FLV_MAKER_SIZE, xv2f->size,
	    FLV_MAKER_OVERWRITE, xv2f->meta_filename,
	    NULL
	  };
	  execvp (FLV_MAKER, args);
	}
      else
	{
	  char *args[] = {
	    FLV_MAKER,
	    FLV_MAKER_INPUT, filename,
	    FLV_MAKER_AC, xv2f->acodec,
	    FLV_MAKER_AR, xv2f->ar,
	    FLV_MAKER_AB, xv2f->ab,
	    FLV_MAKER_FORMAT, FLV_EXT,
	    FLV_MAKER_SIZE, xv2f->size,
	    FLV_MAKER_OVERWRITE, xv2f->meta_filename,
	    NULL
	  };
	  execvp (FLV_MAKER, args);
	}
    }
  else
    {
      wait (NULL);
      bassa_notify_metavideo (xv2f);
      bassa_mutex *bmt = bassa_mutex_new ();
      bassa_mutex_lock (bmt);
      bassa_mutex_unlock (bmt);
      bassa_video2flv_delete (xv2f);
    }
  bassa_unblock_signal (SIGALRM);
  return NULL;
}

void
bassa_video2flv_delete (bassa_video2flv *v2f)
{
  if (!v2f)
    return;
  else
    {
      if (v2f->filename)
	free (v2f->filename);
      if (v2f->meta_filename)
	free (v2f->meta_filename);
      if (v2f->notifyurl)
	free (v2f->notifyurl);
      if (v2f->metaurl)
	free (v2f->metaurl);
      if (v2f->ar)
	free (v2f->ar);
      if (v2f->ab)
	free (v2f->ab);
      if (v2f->size)
	free (v2f->size);
      if (v2f->acodec)
	free (v2f->acodec);
      if (v2f->flv_len)
	free (v2f->flv_len);
      free (v2f);
      return;
    }
}
