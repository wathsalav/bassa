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

#include "mod_thumb.h"
#include "bassa_pop_context.h"
#include "noc_filter_file.h"

bassa_module_region module_region = POP_REGION;
bassa_exec_point module_execpoint = POP_NOTIFY;

int
bassa_notify_thumb (bassa_video2thumb *v2t)
{
#ifdef DEBUG
  printf ("File Name: %s\nMeta File Name: %s\nURL: %s\nOrigin URL: %s\nMeta URL: %s\n",
	  v2t->filename, v2t->meta_filename, v2t->notifyurl, v2t->origurl, v2t->metaurl);
#endif //DEBUG
  if (!(v2t->filename && v2t->meta_filename && v2t->notifyurl))
    return -1;
  struct curl_httppost *post = NULL;
  struct curl_httppost *last = NULL;
  int i=strlen(v2t->meta_filename)-1;
  char *file_only = NULL;
  for (; i>=0; i--)
    {
      if (v2t->meta_filename[i] == '/')
	break;
      file_only=&(v2t->meta_filename[i]);  
    }
  char *meta_url = NULL;
  int meta_url_len = strlen (file_only) + strlen(v2t->metaurl) + 2;
  meta_url = (char*)malloc (meta_url_len);
  memset (meta_url, (int)'\0', meta_url_len);
  strcpy (meta_url, v2t->metaurl);
  strcat (meta_url, "/");
  strcat (meta_url, file_only);
  char *xml_buffer =
    (char *) malloc (strlen (NOTIFICATION_XML) + strlen(file_only) +
		     strlen (v2t->origurl) + strlen (meta_url) + 1);
  sprintf (xml_buffer, NOTIFICATION_XML, file_only, v2t->origurl,
	   meta_url);
  xml_buffer[strlen(xml_buffer)] = '\0';

#ifdef DEBUG
  printf ("META-NOTIFICATION-XML-BUFFER: %s\n", xml_buffer);
#endif
  CURL *curl_handle = curl_easy_init();
  if (!curl_handle)
    return -1;
  bassa_setopts (curl_handle, post, last, FORM_NAME,
		 xml_buffer, CONTENT_TYPE, v2t->notifyurl, 20,
		 v2t->http_proxy);
  int perform, times;
  perform = 0;
  long int round_connect_timeout, sleep_time;

  for (times = 0; times < v2t->maxtries; times++)
    {
      perform = curl_easy_perform(curl_handle);
#ifdef DEBUG
      printf ("Trying %i time(s)...\n", times);
#endif //DEBUG
      if (perform)
	{
	  round_connect_timeout =
	    (long int)(v2t->timeout / (v2t->maxtries - times));
	  sleep_time =
	    (long int)(RETRY_GAP_MICROS / (v2t->maxtries - times));
	  bassa_setopts (curl_handle, post, last, FORM_NAME, xml_buffer,
			 CONTENT_TYPE, v2t->notifyurl, v2t->timeout,
			 v2t->http_proxy);
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

__attribute__((constructor))void init_mod_png ()
{

}

__attribute__((destructor))void fini_mod_png ()
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
  bassa_video2thumb *v2t = (bassa_video2thumb*)malloc (sizeof(bassa_video2thumb));
  v2t->filename = strdup(popc->file_name);
  v2t->origurl = strdup(popc->url);
  bassa_video2thumb_parse (v2t, conf);
  bassa_nowait_spawn (NULL, bassa_gen_thumbnail, v2t);
  return 0;
}

void module_exit (void *x)
{
}

void
bassa_video2thumb_parse (bassa_video2thumb *v2t, char *conf)
{
  if (!v2t && !conf)
    return;
  int maxtries = 0;
  int timeout = 0;
  char *metaurl = NULL;
  char *notifyurl = NULL;
  char *seek = NULL;
  char *image_size = NULL;
  char *meta_repo = NULL;
  char *http_proxy = NULL;

  cfg_opt_t opts[] = {
    CFG_SIMPLE_STR("notify_url", &notifyurl),
    CFG_SIMPLE_STR("meta_url", &metaurl),
    CFG_SIMPLE_INT("maxtries", &maxtries),
    CFG_SIMPLE_INT("timeout", &timeout),
    CFG_SIMPLE_STR("image_size", &image_size),
    CFG_SIMPLE_STR("meta_repo", &meta_repo),
    CFG_SIMPLE_STR("http_proxy", &http_proxy),
    CFG_SIMPLE_STR("seek", &seek)
  };
  cfg_t *cfg;
  cfg = cfg_init (opts, 0);
  cfg_parse (cfg, conf);
  v2t->notifyurl = notifyurl;
  v2t->metaurl = metaurl;
  v2t->maxtries = maxtries;
  v2t->timeout = timeout;
  v2t->seek = seek;
  v2t->image_size = image_size;
  v2t->metarepo = meta_repo;
  v2t->http_proxy = http_proxy;
  cfg_free (cfg);
}

void*
bassa_gen_thumbnail (void *v2t)
{
  bassa_block_signal (SIGALRM);
  if (v2t == NULL)
    return NULL;
  bassa_video2thumb *xv2t = (bassa_video2thumb*)v2t;
  char *filename = xv2t->filename;
  int new_name_len = strlen(filename)+strlen(PNG_EXT)+2;
  char *new_name = (char*)malloc(new_name_len);
  memset(new_name, (int)'\0', new_name_len);
  strncpy(new_name, filename,strlen(filename));
  strcat(new_name, ".");
  strcat(new_name, PNG_EXT);
  char *png_name = strdup(bassa_file_get_name (new_name));
  int png_path_len = strlen(png_name) + strlen(xv2t->metarepo) + 1;
  char *png_path = (char*)malloc (png_path_len);
  memset (png_path, (int)'\0', png_path_len);
  strcpy (png_path, xv2t->metarepo);
  strcat (png_path, png_name);
  xv2t->meta_filename = png_path;
  if (new_name)
    free(new_name);
  if (png_name)
    free(png_name);
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
      printf ("%s", PNG_MAKER);
      printf (" %s %s", PNG_MAKER_INPUT, filename);
      printf (" %s %s", PNG_MAKER_SEEK, xv2t->seek);
      printf (" %s %s", PNG_MAKER_FORMAT, PNG_MAKER_OFMT);
      printf (" %s %s", PNG_MAKER_SIZE, xv2t->image_size);
      printf (" %s", PNG_MAKER_OVERWRITE);
      printf (" %s %s", PNG_MAKER_VFRAMES, PNG_MAKER_NFRAMES);
      printf (" %s", PNG_MAKER_AN);
      printf (" %s %s", PNG_MAKER_VCODEC, PNG_EXT);
      printf (" %s\n", xv2t->meta_filename);
#endif //DEBUG
      if (xv2t->seek)
	{
	  char *args[] = {
	    PNG_MAKER,
	    PNG_MAKER_INPUT, filename,
	    PNG_MAKER_SEEK, xv2t->seek,
	    PNG_MAKER_FORMAT, PNG_MAKER_OFMT,
	    PNG_MAKER_SIZE, xv2t->image_size,
	    PNG_MAKER_OVERWRITE, 
	    PNG_MAKER_VFRAMES, PNG_MAKER_NFRAMES,
	    PNG_MAKER_AN,
	    PNG_MAKER_VCODEC, PNG_EXT,
            xv2t->meta_filename,
	    NULL
	  };
	  execvp (PNG_MAKER, args);
	}
      else
	{
	  char *args[] = {
	    PNG_MAKER,
	    PNG_MAKER_INPUT, filename,
	    PNG_MAKER_FORMAT, PNG_MAKER_OFMT,
	    PNG_MAKER_SIZE, xv2t->image_size,
	    PNG_MAKER_OVERWRITE, 
            PNG_MAKER_VFRAMES, PNG_MAKER_NFRAMES,
	    PNG_MAKER_AN,
	    PNG_MAKER_VCODEC, PNG_EXT,
            xv2t->meta_filename,
	    NULL
	  };
	  execvp (PNG_MAKER, args);
	}
    }
  else
    {
      wait (NULL);
      bassa_notify_thumb (xv2t);
      bassa_mutex *bmt = bassa_mutex_new ();
      bassa_mutex_lock (bmt);
      bassa_mutex_unlock (bmt);
      bassa_video2thumb_delete (xv2t);
    }
  bassa_unblock_signal (SIGALRM);
  return NULL;
}

void
bassa_video2thumb_delete (bassa_video2thumb *v2t)
{
  if (!v2t)
    return;
  else
    {
      if (v2t->filename)
	free (v2t->filename);
      if (v2t->meta_filename)
	free (v2t->meta_filename);
      if (v2t->notifyurl)
	free (v2t->notifyurl);
      if (v2t->metaurl)
	free (v2t->metaurl);
      if (v2t->image_size)
	free (v2t->image_size);
      if (v2t->seek)
	free (v2t->seek);
      free (v2t);
      return;
    }
}

