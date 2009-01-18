#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bassa_pack.h"

void bassa_pack_first_line (bassa_http_msg *msg)
{
  int fl_len = FIRSTLINE_TEMPLATE_LEN;
  char *method = NULL;
  char *version = NULL;
  if (msg->type == HTTP_MSG_REQUEST)
    {
      switch(msg->method)
        {
          case HTTP_GET:
            fl_len += 3;
            method = "GET";
            break;
          case HTTP_POST:
            fl_len += 4;
            method = "POST";
            break;
          case HTTP_HEAD:
            fl_len += 4;
            method = "HEAD";
            break;
         }
      fl_len += strlen(msg->path);
      //Setup the HTTP version
      switch(msg->version)
        {
          case HTTP_1_0:
            fl_len += 8;
            version = "HTTP/1.0";
            break;
          case HTTP_1_1:
            fl_len += 8;
            version = "HTTP/1.1";
            break;
        }
      //Allocate 1 byte for NULL char
      fl_len += 1;
      msg->first_line = (char*)malloc(fl_len);
      memset(msg->first_line, '\0', fl_len);
      sprintf(msg->first_line, FIRSTLINE_TEMPLATE, method, msg->path, version); 
    }
  else if(msg->type == HTTP_MSG_REPLY)
    {
      int fl_len = FIRSTLINE_TEMPLATE_LEN;
      char *version = NULL;
      char *status_code = NULL;
      char *status_str  = NULL;
      switch(msg->version)
        {
          case HTTP_1_0:
            fl_len += 8;
            version = "HTTP/1.0";
            break;
          case HTTP_1_1:
            fl_len += 8;
            version = "HTTP/1.1";
            break;
        }
      switch(msg->status)
        {
          case HTTP_200_OK:
            fl_len += 5;
            status_code = "200";
            status_str = "OK";
            break;
        }
      //Allocate for NULL byte
      fl_len += 1;
      msg->first_line = (char*)malloc(fl_len);
      memset(msg->first_line, '\0', fl_len);
      sprintf(msg->first_line, FIRSTLINE_TEMPLATE, version, status_code, status_str); 
    }
}

char* bassa_pack_field (char *key, char *val)
{
  int field_len = FIELD_TEMPLATE_LEN + strlen(key) + strlen(val);
  char *field = (char*)malloc(field_len);
  sprintf (field, FIELD_TEMPLATE, key, val);
  return field;
}

char* bassa_unpack_val (char *field, int field_len)
{
  if (field_len <= 0)
    return NULL;
  char *val = (char*)memchr(field, ':', field_len);
  if ((field_len - (val - field) + 1) > 2)
    {
      if (*(++val) == WS)
        return ++val;
      else
        return val;
    }
  else
    return NULL;
}

