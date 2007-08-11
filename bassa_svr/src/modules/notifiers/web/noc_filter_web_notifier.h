#ifndef MOD_WEBNOT_H
#define MOD_WEBNOT_H

#include "bassa_module_manager.h"

#define DOWNLOAD_COMPLETE_STAT_ID 0
#define DOWNLOAD_COMPLETE_STAT_STR "completed"

#define DOWNLOAD_FAIL_STAT_ID 1
#define DOWNLOAD_FAIL_STAT_STR "failed"

#define NOTIFICATION_XML "<bassa>\
<file_name>%s</file_name>\
<status>%s</status>\
<origin_url>%s</origin_url>\
<cache_url>%s</cache_url>\
<server_response>%i</server_response>\
<size>%lli</size>\
</bassa>\n\r"

#define RETRY_GAP_MICROS 1000*1000*2
#define FORM_NAME "bassa_form"
#define CONTENT_TYPE "text/html"

#include <curl/curl.h>
#include <curl/easy.h>
#include <confuse.h>

typedef struct
{
  char *notifyurl;	 //Deallocate this memory after using
  int timeout;
  int maxtries;
  char *filename;
  char *origurl;
  int status;
  int server_response;
  unsigned long long size;
  char *cache_url;       //Deallocate this memory after using
}bassa_web_notification;


int
bassa_notify_web(bassa_web_notification *webnot);

char*
bassa_get_astatus(int status);

void
bassa_setopts (CURL *curl_handle, struct curl_httppost *post, struct curl_httppost *last, 
		    char *form_name, char *xml_buffer, char *content_type, char *url, int con_timeout);

int module_exec (void *x, char *conf);
void module_exit (void *x);

void
bassa_webnotconf_parse (bassa_web_notification *webnot, char *conf);

void
bassa_webnotconf_delete (bassa_web_notification *webnot);
#endif	//MOD_WEBNOT_H
