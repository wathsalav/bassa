/***************************************************************************
 *   Copyright (C) 2007 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#ifndef MOD_FLV_H
#define MOD_FLV_H

#include "bassa_module_manager.h"

#define DOWNLOAD_COMPLETE_STAT_ID 0
#define DOWNLOAD_COMPLETE_STAT_STR "completed"

#define DOWNLOAD_FAIL_STAT_ID 1
#define DOWNLOAD_FAIL_STAT_STR "failed"

#define NOTIFICATION_XML "<bassa>\
<meta_info>\
<file_name>%s</file_name>\
<origin_url>%s</origin_url>\
<meta_url>%s</meta_url>\
<mime>image/png</mime>\
</meta_info>\
</bassa>\n\r"

#define PNG_EXT "png"
#define PNG_MAKER "ffmpeg"
#define PNG_MAKER_INPUT "-i"
#define PNG_MAKER_AN "-an"
#define PNG_MAKER_FORMAT "-f"
#define PNG_MAKER_SIZE "-s"
#define PNG_MAKER_OVERWRITE "-y"
#define PNG_MAKER_VCODEC "-vcodec"
#define PNG_MAKER_OFMT "rawvideo"
#define PNG_MAKER_SEEK "-ss"
#define PNG_MAKER_VFRAMES "-vframes"
#define PNG_MAKER_NFRAMES "1"

#define RETRY_GAP_MICROS 1000*1000*2
#define FORM_NAME "bassa_form"
#define CONTENT_TYPE "text/html"

#define FULL_ENCODING "full"

#include <curl/curl.h>
#include <curl/easy.h>
#include <confuse.h>

#include <noc_filter_concur.h>
#include <noc_filter_util.h>

typedef struct
{
  char *notifyurl;	 //Deallocate this memory after using
  int timeout;
  int maxtries;
  char *filename;
  char *meta_filename;
  char *origurl;
  char *metaurl;         //Deallocate this memory after using
  char *metarepo;        //Meta content repository
  char *image_size;      //Video size
  char *seek;            //From where we make png
  char *http_proxy;      //HTTP Proxy
  char *vcodec;
}bassa_video2thumb;

//bassa_list *v2fl;

__attribute__((constructor))void init_mod_flv();

__attribute__((destructor))void fini_mod_flv();

int
bassa_notify_thumb (bassa_video2thumb *v2t);

void
bassa_setopts (CURL *curl_handle, struct curl_httppost *post, struct curl_httppost *last, 
	       char *form_name, char *xml_buffer, char *content_type, char *url, int con_timeout,
	       char *http_proxy);

int module_exec (void *x, char *conf);
void module_exit (void *x);

void
bassa_video2thumb_parse (bassa_video2thumb *video2thumb, char *conf);

void*
bassa_gen_thumbnail (void*);

void
bassa_video2thumb_delete (bassa_video2thumb *v2t);

/*All external functions are defined here*/
extern char* 
mod_mime_gettype (char *filename);

#endif	//MOD_FLV_H
