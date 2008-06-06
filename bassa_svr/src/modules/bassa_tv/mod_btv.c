/***************************************************************************
 *   Copyright (C) 2007 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mrss.h>
#include <time.h>
#include <curl/curl.h>
#include <confuse.h>

#include <noc_filter_util.h>
#include <noc_filter_concur.h>
#include <bassa_module_manager.h>
#include <noc_filter_timer.h>

#define BTV \
"<?xml version='1.0'?>\
<btv>\
<notification>\
%s\
%s\
</notification>\
</btv>"

#define BTV_GET_BTVLEN \
  strlen (BTV) - 2

#define BTV_CHANNEL \
"<channel>\
<title>%s</title>\
<description>%s</description>\
<link>%s</link>\
<rating>%s</rating>\
<pub_date>%s</pub_date>\
<image_title>%s</image_title>\
<image_url>%s</image_url>\
</channel>"

#define BTV_GET_CHANLEN \
  strlen (BTV_CHANNEL) - 14

#define BTV_ITEM \
"<item>\
<title>%s</title>\
<link>%s</link>\
<author>%s</author>\
<description>%s</description>\
<category>%s</category>\
<comments>%s</comments>\
<enclosure_type>%s</enclosure_type>\
<enclosure_url>%s</enclosure_url>\
<enclosure_length>%d</enclosure_length>\
<guid>%s</guid>\
<pub_date>%s</pub_date>\
<source>%s</source>\
</item>"

#define BTV_GET_ITEMLEN \
  strlen (BTV_ITEM) - 24

#define BTV_CDATA \
"<![CDATA[%s]]>"

#define BTV_GET_CDATALEN \
  strlen (BTV_CDATA) -2;

#define BTV_FORM_NAME "bassa_form"
#define BTV_CONTENT_TYPE "text/html"

bassa_module_region module_region = ACT_REGION;
bassa_exec_point module_execpoint = ACT_X;
bassa_task_pool *bassa_tv_pool;


typedef struct 
{
  char *http_proxy;
  char *btv_notify;
  char **btv_channels;
  char *btv_dir;
  unsigned long int btv_timeout;
  int use_proxy;
} bassa_tv;

int
module_exec (void *x, char *conf);

void 
module_exit (void *ctx);

void*
bassa_tv_reader (void*);

char*
bassa_tv_genxml (char *chan, char *item);

unsigned long int
get_timeinsec (char*);

char** 
get_channellist (char*);

void 
bassa_tv_notify (bassa_tv *btv, char *msg);

int
module_exec (void *x, char *conf)
{
  char *btv_channels = NULL;
  char *btv_dir = NULL;
  char *http_proxy = NULL;
  char *timeout = NULL;
  char *btv_notification = NULL;
  int use_proxy = 0;
  bassa_tv *btv = (bassa_tv*)malloc(sizeof(bassa_tv));
  cfg_opt_t opts[] = {
    CFG_SIMPLE_STR("btv_notification", &btv_notification),
    CFG_SIMPLE_STR("btv_channels", &btv_channels),
    CFG_SIMPLE_STR("http_proxy", &http_proxy),
    CFG_SIMPLE_STR("btv_dir", &btv_dir),
    CFG_SIMPLE_STR("btv_timeout", &timeout),
    CFG_SIMPLE_INT("use_proxy", &use_proxy)
  };
  cfg_t *cfg = NULL;
  cfg = cfg_init (opts, 0);
  cfg_parse (cfg, conf);
  btv->btv_notify = btv_notification;
  btv->http_proxy = http_proxy;
  btv->btv_dir = btv_dir;
  btv->btv_channels = get_channellist (btv_channels);
  btv->btv_timeout = get_timeinsec (timeout);
  btv->use_proxy = use_proxy;
  bassa_tv_pool = bassa_task_pool_new (1);
  bassa_nowait_spawn (bassa_tv_pool, bassa_tv_reader, btv);
  return 0;
}

void 
module_exit (void *ctx)
{
  bassa_kill_task (bassa_tv_pool, bassa_tv_pool->tasklets[0]);
}

void*
bassa_tv_reader (void *arg)
{
  bassa_blockall_signals ();
  bassa_tv *btv = (bassa_tv*)arg;
  struct timespec ts;
  ts.tv_sec = btv->btv_timeout;
  ts.tv_nsec = 0; 
  mrss_t *data;
  mrss_item_t *item;
  int channel_count = 0;
  char *current_channel = NULL;
  while(1)
    {
      while ((current_channel = btv->btv_channels[channel_count]))
	{
	  mrss_options_t *options = mrss_options_new (-1,
						      btv->http_proxy,
						      NULL,
						      NULL,
						      NULL,
						      NULL,
						      0,
						      NULL,
						      NULL
						      ) ;
	  //Parse channel
	  int ret = mrss_parse_url_with_options (current_channel, &data, options);
	  if (ret)
	    {
	      fprintf (stderr, "MRSS return error: %s\n", mrss_strerror (ret));
	      mrss_options_free (options);
	      break;
	    }
	  mrss_options_free (options);
	  //Setup channel
	  int cdesclen = ((data->description)? strlen (data->description): 0) + BTV_GET_CDATALEN;
	  char *cdesc = (char*)malloc (cdesclen + 1);
	  sprintf (cdesc, BTV_CDATA, (data->description)? data->description: "");

	  int var_len = 
	    (data->title? strlen (data->title): 0) +
	    strlen (cdesc) +
	    (data->link? strlen (data->link): 0) +
	    (data->rating? strlen (data->rating): 0) +
	    (data->pubDate? strlen (data->pubDate): 0) +
	    (data->image_title? strlen (data->image_title): 0) +
	    (data->image_url? strlen (data->image_url): 0);

	  //Allocate memory for Chanel
	  char *channel = (char*)malloc(BTV_GET_CHANLEN + var_len + 1);
	  memset (channel, (int)'\0', BTV_GET_CHANLEN + var_len + 1);
	  sprintf (channel, BTV_CHANNEL, 
		   data->title? data->title: "",
		   cdesc,
		   data->link? data->link: "",
		   data->rating? data->rating: "",
		   data->pubDate? data->pubDate: "",
		   data->image_title? data->image_title: "",
		   data->image_url? data->image_url: ""
		   );

	  //Setup item
	  item = data->item;
	  while (item)
	    {
	      int idesclen = ((item->description)? strlen (item->description): 0) + BTV_GET_CDATALEN;
	      char *idesc = (char*)malloc (idesclen + 1);
	      sprintf (idesc, BTV_CDATA, (item->description)? item->description: "");
	      int icomlen = ((item->comments)? strlen (item->comments): 0) + BTV_GET_CDATALEN;
	      char *icom = (char*)malloc (icomlen + 1);
	      sprintf (icom, BTV_CDATA, (item->comments)? item->comments: "");
	      //Set category length
	      int cat_len = 0;
	      if (item->category)
		{
		  if (item->category->category)
		    cat_len = strlen (item->category->category);
		}
	      int item_len =
		((item->title)? strlen (item->title): 0) +
		((item->link)? strlen (item->link): 0) +
		((item->author)? strlen (item->author): 0) +
		strlen (idesc) +
		cat_len +
		strlen (icom) +
		((item->enclosure_type)? strlen (item->enclosure_type): 0) +
		((item->enclosure_url)? strlen (item->enclosure_url): 0) +
		10 +
		((item->guid)? strlen (item->guid): 0) +
		((item->pubDate)? strlen (item->pubDate): 0) +
		((item->source)? strlen (item->source): 0);

	      //Initialize category
	      char *category = "";
	      if (item->category)
		{
		  if (item->category->category)
		    category = item->category->category;
		}
	      char *tv_item = (char*)malloc(BTV_GET_ITEMLEN + item_len + 1);
	      memset (tv_item, (int)'\0', BTV_GET_ITEMLEN + item_len + 1);
	      sprintf (tv_item, BTV_ITEM,
		       item->title? item->title: "",
		       item->link? item->link: "",
		       item->author? item->author: "",
		       idesc,
		       category,
		       icom,
		       item->enclosure_type? item->enclosure_type: "",
		       item->enclosure_url? item->enclosure_url: "",
		       item->enclosure_length,
		       item->guid? item->guid: "",
		       item->pubDate? item->pubDate: "",
		       item->source? item->source: "");
	      char *msg =  bassa_tv_genxml(channel, tv_item);
	      bassa_tv_notify (btv, msg);
	      free (idesc);
	      free (icom);
	      free (tv_item);
	      free (msg);
	      item = item->next;
	    }
	  free (channel);
	  mrss_free (data);
	  channel_count++;
	}
      if (!current_channel)
	{
	  channel_count = 0;
	  nanosleep (&ts, NULL);
	}
    }
  bassa_unblockall_signals ();
}


char*
bassa_tv_genxml (char *chan, char *item)
{
  int msg_len = 
    strlen(chan) + 
    strlen(item) + 
    BTV_GET_BTVLEN +
    1;
  char *msg = (char*)malloc (msg_len);
  sprintf (msg, BTV, chan, item);
  return msg;
}

void 
bassa_tv_notify (bassa_tv *btv, char *msg)
{
  struct curl_httppost *post = NULL;
  struct curl_httppost *last = NULL;

  CURL *curl_handle = curl_easy_init();
  if (!curl_handle)
    return;
  curl_formadd(&post, &last, CURLFORM_COPYNAME, BTV_FORM_NAME,
	       CURLFORM_COPYCONTENTS, msg, 
	       CURLFORM_CONTENTTYPE, BTV_CONTENT_TYPE,
	       CURLFORM_END);
  curl_easy_setopt (curl_handle, CURLOPT_HTTPPOST, post);
  curl_easy_setopt (curl_handle, CURLOPT_URL, btv->btv_notify);
  if (btv->use_proxy)
    curl_easy_setopt (curl_handle, CURLOPT_PROXY, btv->http_proxy);
  else
    curl_easy_setopt (curl_handle, CURLOPT_PROXY, "");
  curl_easy_perform (curl_handle);
  curl_formfree (post);
  curl_easy_cleanup (curl_handle);
  return;
}

unsigned long int
get_timeinsec (char *timestr)
{
  int feild_count = 0;
  char *tok = NULL;
  char *token = ":";
  unsigned int h=0,m=0,s=0;
  while ((tok = strtok (timestr, token)))
    {
      timestr = NULL;
      feild_count++;
      if (feild_count > 3)
	break;
      switch (feild_count)
	{
	case 1:
	  h = atoi(tok);
	  break;
	case 2:
	  m = atoi(tok);
	  break;
	case 3:
	  s = atoi(tok);
	}
    }
  unsigned long int secs = NOC_FILTER_TIMER_CVT_TIME2SECS(h,m,s);
  return secs;
}

char** 
get_channellist (char *chanstr)
{
  char **chan_list = NULL;
  int count = 0;
  char *token = ";";
  char *tok = NULL;
  while ((tok = strtok (chanstr, token)))
    {
      count++;
      chanstr = NULL;
      chan_list = (char**)realloc(chan_list, count*sizeof(char*));
      chan_list [count-1] = tok;
    }
  if (count == 0)
    {
      count++;
      chan_list = (char**)realloc(chan_list, count*sizeof(char*));
      chan_list[count-1] = chanstr;
    }
  chan_list = (char**)realloc(chan_list, (count+1)*sizeof(char*));
  chan_list [count] = NULL;
  return chan_list;
}

