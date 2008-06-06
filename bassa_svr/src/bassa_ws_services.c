#include <string.h>

#include "bassa.nsmap"
#include "soapH.h"
#include "noc_filter_configure.h"
#include "bassa_module_manager.h"
#include "bassa_uri.h"
#include "bassa_db.h"

int bassa__enqueue(struct soap *soap, struct bassa__request *r, char **response)
{
	bassa_uri *bu = bassa_uri_new(r->url);
	bassa_irequest *bir = bassa_irequest_new(bu, r->content_length);
	int ret = bassa_db_queue(bir);
  if (ret)
    *response = "FAIL: URL Exists";
  else
    *response = "OK: URL Queued";
	bassa_irequest_free(bir);
	bassa_uri_free(bu);
  return SOAP_OK;
}

int bassa__search(struct soap *soap, int offset, char *url, int sort_type, struct bassa__file_set **r)
{
  bassa_object_set *bobjs = bassa_search_file(url, offset, sort_type);
  (*r) = (struct bassa__file_set*)soap_malloc(soap, sizeof(struct bassa__file_set));
  (*r)->offset = bobjs->offset;
  (*r)->total = bobjs->total;
  int i;
  for (i=0; i<(*r)->total; i++)
    {
      (*r)->status[i] = soap_strdup(soap, bobjs->bobj[i]->status);
      (*r)->local_url[i] = soap_strdup(soap, bobjs->bobj[i]->object_url);
      (*r)->url[i] = soap_strdup(soap, bobjs->bobj[i]->origin_url);
      (*r)->file[i] = soap_strdup(soap, bobjs->bobj[i]->file_name);
      (*r)->hits[i] = bobjs->bobj[i]->hits;
      (*r)->content_length[i] = bobjs->bobj[i]->content_length;
    }
  bassa_object_set_free(bobjs);
  printf ("XXX\n");
  return SOAP_OK;
}

int bassa__list_all(struct soap *soap, int offset, struct bassa__file_set **r)
{
  return SOAP_OK;
}

int bassa__delete(struct soap *soap, int id, char **response)
{
  return SOAP_OK;
}

int bassa__latest_downloads(struct soap *soap, int offset, struct bassa__file_set **r)
{
  return SOAP_OK;
}

int bassa__get_status(struct soap *soap, int id, char **response)
{
  return SOAP_OK;
}

