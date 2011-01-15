#include <string.h>

#include <bassa.nsmap>
#include <soapH.h>
#include <bassa_ws.h>
#include <bassa_ws_server.h>
#include <noc_filter_configure.h>
#include <bassa_module_manager.h>
#include <bassa_prp_context.h>
#include <bassa_uri.h>
#include <bassa_db.h>
#include <bassa_trigger.h>

extern bassa_module_table *bmt;

int bassa__enqueue(struct soap *soap, struct bassa__request *r, char **response)
{ 
  bassa_uri *bu = NULL;
  bassa_irequest *bir = NULL;
  bassa_trigger *btrig = NULL;
  bassa_db *dbd = NULL; 
  if (soap == NULL || r == NULL)
    return SOAP_FAULT;
  btrig = bassa_trigger_new(conf->svrcfg->server_event_bus);
  dbd = bassa_db_init(); 
  if (!dbd)
  {
    *response = "FAIL: Internal Server Error";
    return SOAP_OK;
  }
  if (!r->url)
  {
    *response = "FAIL: No URL Sprecified";
    goto cleanup;
  }
  bu = bassa_uri_new(r->url); 
  if (!bu)
  {
    *response = "FAIL: Bad URL Submitted";
    goto cleanup;
  }
  bir = bassa_irequest_new1(bu, r->contentLength);
  if (r->uuid)
    bir->bobj->uuid = r->uuid;
  else
  {
    *response = "FAIL: Missing UUID";
    goto cleanup;
  }
  //Execute PRP Modules
  bassa_exec_path(bir->bobj, bmt, PRP_REGION, PRP_QUEUE);
  int ret = bassa_db_queue(dbd, bir);
  if (ret)
  {
    *response = "OK: URL Exist";
  }
  else
  {
    *response = "OK: URL Queued";
    bassa_trigger_wake(btrig, BASSA_INIT_PROC);
  }
cleanup: bassa_db_shutdown(dbd);
  bassa_irequest_free(bir);
  bassa_uri_free(bu);
  bassa_trigger_free(btrig);
  return SOAP_OK;
}

int bassa__search(struct soap *soap, int offset, int sort_type, char *url, struct bassa__file_set *r)
{
  printf("Search called\n");
  if (soap == NULL || url == NULL || r == NULL)
  {
    r->svr_msg = "FAULT: Invalid Parameter";
    return SOAP_FAULT;
  }
  bassa_db *dbd = bassa_db_init();
  if (!dbd)
  {
    r->svr_msg = "FAIL: Internal Server Error";
    return SOAP_OK;
  }
  bassa_object_set *bobjs = bassa_search_file(dbd, url, offset, sort_type);
  r->offset = bobjs->offset;
  r->total = bobjs->total;
  r->object_limit = bobjs->object_limit;
  int i;
  for (i=0; i<r->total; i++)
  {
    r->status[i] = soap_strdup(soap, bobjs->bobj[i]->status);
    r->local_url[i] = soap_strdup(soap, bobjs->bobj[i]->object_url);
    r->url[i] = soap_strdup(soap, bobjs->bobj[i]->origin_url);
    r->file[i] = soap_strdup(soap, bobjs->bobj[i]->file_name);
    r->uuid[i] = soap_strdup(soap, bobjs->bobj[i]->uuid);
    r->hits[i] = bobjs->bobj[i]->hits;
    r->content_length[i] = bobjs->bobj[i]->content_length;
    r->start_time[i] = bobjs->bobj[i]->start_time;
    r->end_time[i] = bobjs->bobj[i]->end_time;
  }
  bassa_object_set_free(bobjs);
  bassa_db_shutdown(dbd);
  return SOAP_OK;
}

int bassa__list_all(struct soap *soap, int offset, int sort_type, struct bassa__file_set *r)
{
  if (soap == NULL || r == NULL)
    return SOAP_FAULT;
  bassa_db *dbd = bassa_db_init();
  if (!dbd)
  {
    r->svr_msg = "FAIL: Internal Server Error";
    return SOAP_OK;
  }
  bassa_object_set *bobjs = bassa_list_all(dbd, offset, sort_type);
  r->offset = bobjs->offset;
  r->total = bobjs->total;
  r->object_limit = bobjs->object_limit;
  int i;
  for (i=0; i<r->total; i++)
  {
    r->status[i] = soap_strdup(soap, bobjs->bobj[i]->status);
    r->local_url[i] = soap_strdup(soap, bobjs->bobj[i]->object_url);
    r->url[i] = soap_strdup(soap, bobjs->bobj[i]->origin_url);
    r->file[i] = soap_strdup(soap, bobjs->bobj[i]->file_name);
    r->uuid[i] = soap_strdup(soap, bobjs->bobj[i]->uuid);
    r->hits[i] = bobjs->bobj[i]->hits;
    r->content_length[i] = bobjs->bobj[i]->content_length;
    r->start_time[i] = bobjs->bobj[i]->start_time;
    r->end_time[i] = bobjs->bobj[i]->end_time;
  }
  bassa_object_set_free(bobjs);
  bassa_db_shutdown(dbd);
  return SOAP_OK;
}

int bassa__delete(struct soap *soap, int id, char **response)
{
  return SOAP_OK;
}

int bassa__latest_downloads(struct soap *soap, int offset, struct bassa__file_set *r)
{
  if (soap == NULL || r == NULL)
    return SOAP_FAULT;
  bassa_db *dbd = bassa_db_init();
  if (!dbd)
  {
    r->svr_msg = "FAIL: Internal Server Error";
    return SOAP_OK;
  }
  bassa_object_set *bobjs = bassa_list_latest(dbd, offset);
  r->offset = bobjs->offset;
  r->total = bobjs->total;
  r->object_limit = bobjs->object_limit;
  int i;
  for (i=0; i<r->total; i++)
  {
    r->status[i] = soap_strdup(soap, bobjs->bobj[i]->status);
    r->local_url[i] = soap_strdup(soap, bobjs->bobj[i]->object_url);
    r->url[i] = soap_strdup(soap, bobjs->bobj[i]->origin_url);
    r->file[i] = soap_strdup(soap, bobjs->bobj[i]->file_name);
    r->uuid[i] = soap_strdup(soap, bobjs->bobj[i]->uuid);
    r->hits[i] = bobjs->bobj[i]->hits;
    r->content_length[i] = bobjs->bobj[i]->content_length; 
    r->start_time[i] = bobjs->bobj[i]->start_time;
    r->end_time[i] = bobjs->bobj[i]->end_time;
  }
  bassa_object_set_free(bobjs);
  bassa_db_shutdown(dbd);
  return SOAP_OK;
}

int bassa__get_status(struct soap *soap, int id, char **response)
{
  return SOAP_OK;
}

int bassa__list_byuuid(struct soap *soap, int offset, int sort_type, char *uuid, struct bassa__file_set *r)
{
  if (soap == NULL || r == NULL || uuid == NULL)
    return SOAP_FAULT;
  bassa_db *dbd = bassa_db_init();
  if (!dbd)
  {
    r->svr_msg = "FAIL: Internal Server Error";
    return SOAP_OK;
  }
  bassa_object_set *bobjs = bassa_list_byuuid(dbd, uuid, offset, sort_type);
  r->offset = bobjs->offset;
  r->total = bobjs->total;
  r->object_limit = bobjs->object_limit;
  int i;
  for (i=0; i<r->total; i++)
  {
    r->status[i] = soap_strdup(soap, bobjs->bobj[i]->status);
    r->local_url[i] = soap_strdup(soap, bobjs->bobj[i]->object_url);
    r->url[i] = soap_strdup(soap, bobjs->bobj[i]->origin_url);
    r->file[i] = soap_strdup(soap, bobjs->bobj[i]->file_name);
    r->uuid[i] = soap_strdup(soap, bobjs->bobj[i]->uuid);
    r->hits[i] = bobjs->bobj[i]->hits;
    r->content_length[i] = bobjs->bobj[i]->content_length;
    r->start_time[i] = bobjs->bobj[i]->start_time;
    r->end_time[i] = bobjs->bobj[i]->end_time;
  }
  bassa_object_set_free(bobjs);
  bassa_db_shutdown(dbd);
  return SOAP_OK;
}

int bassa__update_hits(struct soap *soap, struct bassa__request *r, char **response)
{
  if (soap == NULL || r == NULL)
    return SOAP_FAULT;
  if (r->url == NULL)
    return SOAP_FAULT;
  bassa_db *dbd = bassa_db_init();
  if (!dbd)
  {
    *response = "FAIL: Internal Server Error";
    return SOAP_OK;
  }
  if (bassa_update_hits(dbd, r->url))
  {
    *response = "FAIL: Internal Server Error";
    return SOAP_OK;
  }
  bassa_db_shutdown(dbd);
  return SOAP_OK;
}

int bassa__popular_downloads(struct soap *soap, int offset, struct bassa__file_set *r)
{
  if (soap == NULL || r == NULL)
    return SOAP_FAULT;
  bassa_db *dbd = bassa_db_init();
  if (!dbd)
  {
    r->svr_msg = "FAIL: Internal Server Error";
    return SOAP_OK;
  }
  bassa_object_set *bobjs = bassa_list_popular(dbd, offset);
  r->offset = bobjs->offset;
  r->total = bobjs->total;
  r->object_limit = bobjs->object_limit;
  int i;
  for (i=0; i<r->total; i++)
  {
    r->status[i] = soap_strdup(soap, bobjs->bobj[i]->status);
    r->local_url[i] = soap_strdup(soap, bobjs->bobj[i]->object_url);
    r->url[i] = soap_strdup(soap, bobjs->bobj[i]->origin_url);
    r->file[i] = soap_strdup(soap, bobjs->bobj[i]->file_name);
    r->uuid[i] = soap_strdup(soap, bobjs->bobj[i]->uuid);
    r->hits[i] = bobjs->bobj[i]->hits;
    r->content_length[i] = bobjs->bobj[i]->content_length; 
    r->start_time[i] = bobjs->bobj[i]->start_time;
    r->end_time[i] = bobjs->bobj[i]->end_time;
  }
  bassa_object_set_free(bobjs);
  bassa_db_shutdown(dbd);
  return SOAP_OK;
}
