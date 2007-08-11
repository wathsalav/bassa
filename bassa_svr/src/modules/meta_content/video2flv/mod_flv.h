#ifndef MOD_FLV_H
#define MOD_FLV_H

#include "bassa_module_manager.h"

#define DOWNLOAD_COMPLETE_STAT_ID 0
#define DOWNLOAD_COMPLETE_STAT_STR "completed"

#define DOWNLOAD_FAIL_STAT_ID 1
#define DOWNLOAD_FAIL_STAT_STR "failed"

#define NOTIFICATION_XML "<bassa>\
<file_name>%s</file_name>\
<origin_url>%s</origin_url>\
<meta_url>%s</meta_url>\
</bassa>\n\r"

#define FLV_EXT "flv"
#define FLV_MAKER "ffmpeg"
#define FLV_MAKER_INPUT "-i"
#define FLV_MAKER_AC "-acodec"
#define FLV_MAKER_AR "-ar"
#define FLV_MAKER_AB "-ab"
#define FLV_MAKER_FORMAT "-f"
#define FLV_MAKER_SIZE "-s"
#define FLV_MAKER_OVERWRITE "-y"
#define FLV_MAKER_DURATION "-t"

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
  char *acodec;          //Audio codec to use
  char *ar;              //Sample frequency
  char *ab;              //Bitrate
  char *size;            //Video size
  char *flv_len;         //Length of the flv
}bassa_video2flv;

//bassa_list *v2fl;

__attribute__((constructor))void init_mod_flv();

__attribute__((destructor))void fini_mod_flv();

int
bassa_notify_metavideo (bassa_video2flv *v2f);

void
bassa_setopts (CURL *curl_handle, struct curl_httppost *post, struct curl_httppost *last, 
		    char *form_name, char *xml_buffer, char *content_type, char *url, int con_timeout);

int module_exec (void *x, char *conf);
void module_exit (void *x);

void
bassa_video2flv_parse (bassa_video2flv *video2flv, char *conf);

void*
bassa_video_convert (void*);

void
bassa_video2flv_delete (bassa_video2flv *v2f);

/*All external functions are defined here*/
extern char* 
mod_mime_gettype (char *filename);

#endif	//MOD_FLV_H
