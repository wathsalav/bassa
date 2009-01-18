#ifndef BASSA_FIELD_DEFS_H
#define BASSA_FIELD_DEFS_H

#include "bassa_field_macros.h"

typedef struct
{
  char *field_name;
  int field_len;
  int field_id;
}field_def;

#define TBL_SIZE 16

field_def field_def_tbl[] = 
  {
    {"User-Agent:", 11, USER_AGENT},
    {"Accept:", 7, ACCEPT},
    {"Accept-Language:", 16, ACCEPT_LANG},
    {"Accept-Encoding:", 16, ACCEPT_ENC,},
    {"Proxy-Connection:", 17, PROXY_CONN},
    {"Keep-Alive:", 11, KEEP_ALIVE},
    {"Cookie:", 7, COOKIE},
    {"Host:", 5, HOST},
    {"Location", 8, LOCATION},
    {"Cache-Control", 13, CACHE_CONTROL},
    {"Content-Type", 12, CONTENT_TYPE},
    {"Server", 6, SERVER},
    {"Content-Length", 14, CONTENT_LENGTH},
    {"Date", 4, DATE},
    {"X-Cache", 7, X_CACHE},
    {"X-Cache-Lookup", 14, X_CACHE_LOOKUP}
  };


#endif //BASSA_FIELD_DEFS_H

