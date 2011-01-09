#ifndef BASSA_WS_H
#define BASSA_WS_H

//gsoap bassa service name:	bassa
//gsoap bassa service style:	rpc
//gsoap bassa service encoding:	encoded
//gsoap bassa service namespace:	http://localhost:8095/bassa.wsdl
//gsoap bassa service location:	http://localhost:8095/bassa
//gsoap bassa schema namespace: urn:bassa

#define MAX_RES_LEN 10

#define AUTH_OK 100
#define AUTH_FAIL 200


struct bassa__authRequest
{
  char *uid;	//User name
  char *passwd;	//Password
};

struct bassa__authToken
{
  unsigned long int authTokenVal;
  unsigned long int authTokenTimeout;
  char *authMessage;
  int authStatus;
};

struct bassa__request
{
  char *url;
  unsigned long int contentLength;
  unsigned long int authTokenVal;
  char *uuid;
};

struct bassa__file_set
{
  int auth_status;
  char *auth_message;
  unsigned int offset;
  unsigned int total;
  unsigned int object_limit;
  char *svr_msg;
  char *file[100];
  char *url[100];
  char *status[100];
  int hits[100];
  char *local_url[100];
  unsigned long int content_length[100];
  time_t start_time[100];
  time_t end_time[100];
  char *uuid[100];
  char *bassa_id[100];
};

/**
 * Authenticate to use bassa
 */
//int bassa__authenticate(struct bassa_auth_request *rq, struct bassa_auth_token *rp);

/**
 * Enqueue a request in bassa
 */
int bassa__enqueue(struct bassa__request *r, char **response);
/**
 * Search for a file name or an URL
 */
int bassa__search(int offset, int sort_type, char *url, struct bassa__file_set *r);
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

/**
 * List requests by user
 */
int bassa__list_byuuid(int offset, int sort_type, char *uuid, struct bassa__file_set *r);

#endif //BASSA_WS_H

