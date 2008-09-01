#include <libxml/uri.h>
#include <stdlib.h>
#include <string.h>
#include <bassa_uri.h>
#include <bassa_uri.h>

bassa_uri *bassa_uri_new (char *uri)
{
  bassa_uri *bu = (bassa_uri*)malloc(sizeof(bassa_uri));
  bu->uri = bassa_uri_unescape(uri);
  xmlURI *pURI = xmlParseURI(bu->uri);
  if (pURI)
  {
    (pURI->scheme != NULL)?(bu->scheme = strdup(pURI->scheme)):(bu->scheme = NULL);  
    (pURI->authority != NULL)?(bu->authority = strdup(pURI->authority)):(bu->authority = NULL); 
    (pURI->server != NULL)?(bu->server = strdup(pURI->server)):(bu->server = NULL); 
    bu->port = pURI->port;
    (pURI->path != NULL)?(bu->path = strdup(pURI->path)):(bu->path = NULL); 
    (pURI->user != NULL)?(bu->user = strdup(pURI->user)):(bu->user = NULL); 
    (pURI->query_raw != NULL)?(bu->query = strdup(pURI->query_raw)):(bu->query = NULL); 
    (pURI->fragment != NULL)?(bu->fragment = strdup(pURI->fragment)):(bu->fragment = NULL); 
    (pURI->opaque)?(bu->opaque = strdup(pURI->opaque)):(bu->opaque = NULL); 
    bu->file_name = bassa_uri_get_filename(bu->path); //Don't use pURI->path as we delete it before returning. 
    xmlFree(pURI);
    return bu;
  }
  else
    return NULL;
}

/**
 * Delete bassa_uri
 */
void bassa_uri_free (bassa_uri *bu)
{
  if (!bu)
    return;
  if (bu->scheme)
    {
      free(bu->scheme);
      bu->scheme = NULL;
    }
  if (bu->authority)
    {
      free(bu->authority);
      bu->authority = NULL;
    }
  if (bu->server)
    {
      free(bu->server);
      bu->server = NULL;
    }
  if (bu->path)
    {
      free(bu->path);
      bu->path = NULL;
    }
  if (bu->user)
    {
      free(bu->user);
      bu->user = NULL;
    }
  if (bu->query)
    {
      free(bu->query);
      bu->query = NULL;
    }
  if (bu->fragment)
    {
      free(bu->fragment);
      bu->fragment = NULL;
    }
  if (bu->opaque)
    {
      free(bu->opaque);
      bu->opaque = NULL;
    }
  if (bu->file_name)
    {
      free(bu->file_name);
      bu->file_name = NULL;
    }
  if (bu->uri)
    {
      free(bu->uri);
      bu->uri = NULL;
    }
  free(bu);
  bu = NULL;
}

/**
 *Extract file name from url 
 */
char *bassa_uri_get_filename(char *path)
{
  char *filename = NULL;
  char *xpath = NULL;
  if (!path)
    return NULL;
  int last = strlen(path)-1;
  int i;
  for (i=last; i>-1; i--)
  {
    if(path[i] =='/')
    {
      xpath = &path[i];
      filename = strdup(xpath);
      break;
    }
  }
  return filename;
}

char *bassa_uri_unescape(char *uri)
{
  char *escaped_uri = xmlURIUnescapeString(uri, strlen(uri), NULL);
  return escaped_uri;
}

