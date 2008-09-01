#include <string.h>

#include <bassa.nsmap>
#include <soapH.h>
#include <bassa_ws.h>
#include <noc_filter_configure.h>
#include <bassa_module_manager.h>
#include <bassa_uri.h>
#include <bassa_db.h>
#include <bassa_trigger.h>

int bassa__enqueue(struct soap *soap, struct bassa__request *r, char **response)
{
  bassa_trigger *btrig = bassa_trigger_new("/tmp/bassa_sched_trigger");
  bassa_db *dbd = bassa_db_init();
  if (!dbd)
  {
    *response = "FAIL: Internal Server Error";
    return SOAP_OK;
  }
  bassa_uri *bu = bassa_uri_new(r->url);
  bassa_irequest *bir = bassa_irequest_new1(bu, r->content_length);
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
  bassa_db_shutdown(dbd);
  bassa_irequest_free(bir);
  bassa_uri_free(bu);
  bassa_trigger_free(btrig);
  return SOAP_OK;
}

int bassa__search(struct soap *soap, int offset, char *url, int sort_type, struct bassa__file_set *r)
{
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
    r->hits[i] = bobjs->bobj[i]->hits;
    r->content_length[i] = bobjs->bobj[i]->content_length;
    r->date[i] = bobjs->bobj[i]->date;
  }
  bassa_object_set_free(bobjs);
  bassa_db_shutdown(dbd);
  return SOAP_OK;
}

int bassa__list_all(struct soap *soap, int offset, int sort_type, struct bassa__file_set *r)
{
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
    r->hits[i] = bobjs->bobj[i]->hits;
    r->content_length[i] = bobjs->bobj[i]->content_length;
    r->date[i] = bobjs->bobj[i]->date;
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
    r->hits[i] = bobjs->bobj[i]->hits;
    r->content_length[i] = bobjs->bobj[i]->content_length;
    r->date[i] = bobjs->bobj[i]->date;
  }
  bassa_object_set_free(bobjs);
  bassa_db_shutdown(dbd);
  return SOAP_OK;
}

int bassa__get_status(struct soap *soap, int id, char **response)
{
  return SOAP_OK;
}

