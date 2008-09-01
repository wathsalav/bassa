#ifndef BASSA_WS_H
#define BASSA_WS_H

//gsoap bassa service name:	bassa
//gsoap bassa service style:	rpc
//gsoap bassa service encoding:	encoded
//gsoap bassa service namespace:	http://localhost:8095/bassa.wsdl
//gsoap bassa service location:	http://localhost:8095/bassa
//gsoap bassa schema namespace: urn:bassa

#define MAX_RES_LEN 10

struct bassa__request
{
  char *url;
  unsigned long int content_length;
};

struct bassa__file_set
{
  unsigned int offset;
  unsigned int total;
  unsigned int object_limit;
  char *svr_msg;
  char *file[10];
  char *url[10];
  char *status[10];
  int hits[10];
  char *local_url[10];
  unsigned long int content_length[10];
  time_t date[10];
};

/**
 * Enqueue a request in bassa
 */
int bassa__enqueue(struct bassa__request *r, char **response);
/**
 * Search for a file name or an URL
 */
int bassa__search(int offset,char *url, int sort_type, struct bassa__file_set *r);
/**
 * List all objects in cache
 */
int bassa__list_all(int offset, int sort_type, struct bassa__file_set *r);
/**
 * Delete an object in cache
 */
int bassa__delete(int id, char **response);
/**
 * Get lateset cache objects
 */
int bassa__latest_downloads(int offset, struct bassa__file_set *r);
/**
 * Get status of the cache
 */
int bassa__get_status(int id, char **response);

#endif //BASSA_WS_H

