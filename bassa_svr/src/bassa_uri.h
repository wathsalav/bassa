#ifndef BASSA_URI
#define BASSA_URI

#define URI_RESERVED_CHARS ";/?:@&=+$,"

typedef struct
{
  char *scheme;
  char *opaque;
  char *authority;
  char *server;
  int port;
  char *user;
  char *path;
  char *query;
  char *fragment;
  char *file_name;
  char *uri;
  char *uri_unencoded;
}bassa_uri;


/**
 * Create new bassa_uri
 */
bassa_uri *bassa_uri_new (char *uri);

/**
 * Delete bassa_uri
 */
void bassa_uri_free (bassa_uri *bu);

/**
 *Extract file name from url 
 */
char *bassa_uri_get_filename(char *path);

/**
 * Unescape URI
 */
char *bassa_uri_unescape(char *uri);

/**
 * Escape URI
 */
char *bassa_uri_escape(char *uri);

#endif //BASSA_URI

