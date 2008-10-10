#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bassa_htmsg.h"
#include "bassa_field_defs.h"

bassa_http_msg* bassa_http_msg_new(int type)
{
  bassa_http_msg *htmsg = (bassa_http_msg*)malloc(sizeof(bassa_http_msg));
  htmsg->first_line = NULL;
  htmsg->header = NULL;
  htmsg->hdr_fields = NULL;
  htmsg->field_count = 0;
  htmsg->path = NULL;
  htmsg->type = type;
  htmsg->body = NULL;
  htmsg->body_init_len = 0;
  htmsg->total_recv = 0;
  htmsg->first_line_len = 0;
  htmsg->first_line_done = 0;
  htmsg->header_len = 0;
  htmsg->header_done = 0;
  return htmsg;
}

int bassa_get_first_line(char *start, int len, bassa_http_msg *htmsg)
{
  char *new_start = NULL;
  if (len > MAX_HEADER_SIZE)
    {
      htmsg->first_line_len = -1;
      return -1;
    }
  char *first_line = start;
  new_start = (char*)memchr(first_line, (int)'\r', len);
  if (!new_start)
    return 0;
  else if (len < (new_start - first_line)+2)
  {
    return 0;
  }
  else if (len >= (new_start - first_line)+2)
    {
      htmsg->first_line_len = (new_start - first_line);
      htmsg->first_line = first_line;
      htmsg->first_line_done = 1;
      htmsg->header_len += htmsg->first_line_len + 2;
      return len - ((htmsg->first_line_len)+2);
    }
  else
    return -1;
}

int bassa_parse_header(char *start, int len, bassa_http_msg *htmsg)
{
  int com_len = 0, rem_len = len;
  char *start_cur = start;
  if (len > MAX_HEADER_SIZE)
    return -1;
  while(1)
  {
    //write(fileno(stdout), start_cur, len);
    char *next_line = (char*)memchr(start_cur, (int)'\r', len);
    int new_line_len = 0;
    if (next_line != NULL)
    {
      new_line_len = next_line - start_cur;	/*find the length of line excluding \r\n*/
      com_len += new_line_len;	/*length of the completed header, excluding \r\n*/
      rem_len -= new_line_len;	/*length of the remaining header, excluding \r\n*/
    }
    else
      return rem_len;

    if (rem_len < 2)	/*if new_line is not a complete header return reverted rem_len*/
      return (rem_len + new_line_len);
    else	/*if new_line is a complete header then update rem_len, com_len and start_cur*/
    {
      com_len += 2;
      rem_len -= 2;
      char *field = (char*)malloc(new_line_len + 1);
      memset(field, (int)'\0', new_line_len + 1);
      memcpy(field, start_cur, new_line_len);
      bassa_http_add_field (htmsg, field, new_line_len + 1);
      htmsg->header_len += (new_line_len + 2);
      start_cur += (new_line_len + 2);
      if (*start_cur == '\r')	/*if start_cur points to a \r then we are done parsing the header*/  
      {
	htmsg->header_len += 2;
	htmsg->header_done = 1;
	return 0;
      }
    }
  }
}

bassa_http_msg* bassa_parse_msg(int socket, int type)
{
  int buf_len = /*MAX_HEADER_SIZE/4*/15;
  char buf[buf_len];
  memset(buf, (int)'\0', buf_len);
  int rlen = 0, tlen = 0;
  int rem = 0;
  char *content = NULL;
  bassa_http_msg *htmsg = bassa_http_msg_new(type);
  while ((rlen=recv(socket, buf, buf_len, 0))>0)
  {
    content = (char*)realloc(content, tlen + rlen);
    memcpy(content+tlen, buf, rlen);
    tlen += rlen;
    htmsg->total_recv = tlen;

    printf ("xrem: %i\n", rem);
    printf ("xtlen: %i\n", tlen);
    printf ("xrlen: %i\n", rlen);
    
    if (!FIRST_LINE_READ(htmsg))
    {
      rem = bassa_get_first_line(content, tlen, htmsg);
      if (rem == 0)
	continue;
      if (rem > 0)
      {
	printf ("Fist line rem: %i\n", rem);
	bassa_parse_first_line (htmsg);
      }
      if (rem == -1)
	return NULL;
    }
    if (!HEADER_READ(htmsg))
    {
      printf ("rem: %i\n", rem);
      printf ("tlen: %i\n", tlen);
      printf ("rlen: %i\n", rlen);
      if (tlen == rlen)
	rem = bassa_parse_header(content + (tlen-rem), rem, htmsg);
      else
	rem = bassa_parse_header(content + (tlen-rem), rem+rlen, htmsg);
      if (rem == 0)
      {
	printf ("PARSING DONE\n");
	//htmsg->header = content + rem;
	//htmsg->first_line = content;
	/*htmsg->body += 2;
	  htmsg->body_init_len -= 2;*/
	break;
      }
      else if (rem > 0)
      {
	continue;
      }
      else
	return NULL;
    }
  }
  return htmsg;
}

bassa_header_field* bassa_header_field_new ()
{
  bassa_header_field* bhf = (bassa_header_field*)malloc(sizeof(bassa_header_field));
  bhf->field = NULL;
  bhf->field_len = 0;
  bhf->field_id = -1;
  return bhf;
}

void bassa_set_field_id (bassa_header_field *bhf)
{
  int count;
  for (count=0; count<TBL_SIZE; count++)
    {
      if (!memcmp(bhf->field, field_def_tbl[count].field_name, field_def_tbl[count].field_len))
        {
          bhf->field_id = field_def_tbl[count].field_id;
#ifdef DEBUG
          printf ("[%s : %i]\n", bhf->field, bhf->field_id);
#endif //DEBUG
          return;
        }
    }
}

void bassa_parse_first_line (bassa_http_msg *htmsg)
{
switch (htmsg->type)
  {
    case HTTP_MSG_REQUEST:
      if (!memcmp(htmsg->first_line, "GET", 3))
        htmsg->method = HTTP_GET;
      else if (!memcmp(htmsg->first_line, "POST", 4))
        htmsg->method = HTTP_POST;
      else if (!memcmp(htmsg->first_line, "HEAD", 4))
        htmsg->method = HTTP_HEAD;
      else
        {
          htmsg->method = HTTP_BAD_METHOD;
          return;
        }
      char *fl = htmsg->first_line;
      fl += htmsg->first_line_len - 10; /*HTTP/x.x\r\n*/
      if (!memcmp(fl, "HTTP/1.1", 8))
        htmsg->version = HTTP_1_1;
      else if (!memcmp(fl, "HTTP/1.0", 8))
        htmsg->version = HTTP_1_0;
      else
        {
          htmsg->version = HTTP_BAD_VERSION;
          return;
        }
      break;
    case HTTP_MSG_REPLY:
      break;
  }
}

void bassa_http_msg_free (bassa_http_msg *htmsg)
{
  if (htmsg)
    {
      htmsg->version = 0;
      htmsg->first_line_len = 0;
      htmsg->header_len = 0;
      htmsg->field_count = 0;
      if (htmsg->first_line)
        {
          free(htmsg->first_line);
          htmsg->first_line = NULL;
        }
      if (htmsg->hdr_fields)
        {
          free(htmsg->hdr_fields);
          htmsg->hdr_fields = NULL;
        }
      free(htmsg);
      htmsg = NULL;
    }
}

void bassa_http_init_reply(bassa_http_msg *msg, int proto_v, 
                           int status)
{
  msg->version = proto_v;
  msg->status = status;
}
void bassa_http_init_request(bassa_http_msg *msg, int proto_v,
                             int method, char *path)
{
  msg->version = proto_v;
  msg->method = method;
  msg->path = path;
}

void bassa_http_add_field (bassa_http_msg *msg, char *field, int field_len)
{
  msg->field_count++;
  msg->hdr_fields = (bassa_header_field*)realloc(msg->hdr_fields, 
                      (msg->field_count)*sizeof(bassa_header_field));
  msg->hdr_fields[msg->field_count - 1].field = field;
  msg->hdr_fields[msg->field_count - 1].field_len = field_len;
  bassa_set_field_id (&(msg->hdr_fields[msg->field_count - 1]));
}

bassa_header_field* bassa_get_field (int field, bassa_header_field *hdf, int len)
{
  if (!hdf)
    return NULL;
  int count;
  for (count = 0; count < len; count++)
    {
      if (hdf[count].field_id == field)
        return &hdf[count];
    }
  return NULL;
}

