/***************************************************************************
 *   Copyright (C) 2007 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#ifndef BASSA_MESSAGE_H
#define BASSA_MESSAGE_H

#include <config.h>

#define BASSA_XML_1_0 "<?xml version=\"1.0\"?>"

#define BASSA_BASSA "bassa"
#define BASSA_BASSA_ID 0

#define BASSA_OD "offline_download_query"
#define BASSA_OD_ID 1

#define BASSA_NAME "name"
#define BASSA_NAME_ID 2

#define BASSA_EMAIL "email"
#define BASSA_EMAIL_ID 3

#define BASSA_ORG_UNIT "org_unit"
#define BASSA_ORG_UNIT_ID 4

#define BASSA_IP "ip"
#define BASSA_IP_ID 5

#define BASSA_URL "url"
#define BASSA_URL_ID 6

#define BASSA_TIME "time"
#define BASSA_TIME_ID 7

#define BASSA_SEARCH_MSG "offline_download_search"
#define BASSA_SEARCH_MSG_ID 8

#define BASSA_SEARCH_FILE "file"
#define BASSA_SEARCH_FILE_ID 9

#define BASSA_SIZE "size"
#define BASSA_SIZE_ID 10

#define BASSA_SEARCH_RESULTS "offline_download_search_results"

#define BASSA_SEARCH_RESULT "search_result"

/**
 * Status can take 3 different values
 * 100 OK
 * 200 REDIRECTED
 * 300 FORBIDDEN
 * 400 FILE_EXIST 
 *
 * Human readable descriptions of status.
 * OK         Download request is queued 
 * REDIRECTED Download request is queued, but original url is modified
 * FORBIDDEN  Download is not allowed (causes by content filters)
 * FILE_EXIST Can not download since a file existst under the same name 
 *
 * The new URL after Bassa replaces the original URL based on network policy is in bassa_redirect_url
 */
#define OK_CODE "100"
#define RDIRECTED_CODE "200"
#define FORBIDDEN_CODE "300"
#define FEXIST_CODE "400"

#define OK_STAT "OK"
#define RDIRECTED_STAT "REDIRECTED"
#define FORBIDDEN_STAT "FORBIDDEN"
#define FEXIST_STAT "FILE_EXIST"

#define BASSA_ACK_MSG "<bassa_ack><bassa_code>%s</bassa_code><bassa_status>%s</bassa_status><bassa_redirect_url>%s</bassa_redirect_url></bassa_ack>"

#define BASSA_ACK_LEN(code,status,redurl)\
  (redurl != NULL)?strlen(BASSA_ACK_MSG)+strlen(code)+strlen(status)+strlen(redurl)-5:strlen(BASSA_ACK_MSG)+strlen(code)+strlen(status)-5

#define BASSA_OTAG "<%s>"

#define BASSA_CTAG "</%s>"

#define BASSA_OCTAG "<%s/>"

typedef struct 
{
  char *name;
  char *email;
  char *org_unit;
  char *ip;
  char *url;
  char *time;
  char *size_str;
} bassa_od_message;

typedef struct
{
  char *file_name;
} bassa_search_message;

typedef struct
{
  bassa_od_message *od_message;
  bassa_search_message *search_message;
  int current_tag;
  int is_search_message;
  int is_od_message;
} bassa_messages;

bassa_od_message* 
bassa_od_message_new(char* name, char *email, char* org_unit, char *ip, char *url, char *time);

bassa_search_message* 
bassa_search_message_new(char* file_name);

bassa_messages* 
bassa_messages_new(bassa_od_message *od_message, bassa_search_message *search_msg);

bassa_messages* 
bassa_messages_decode(char *xml);

char* 
bassa_od_message_encode(bassa_od_message *msg);

char* 
bassa_search_message_encode(char *file_name);

char* 
bassa_message_encode(char *child_msg);

char* 
bassa_xml_document_encode(char *bassa_msg);

void 
bassa_od_message_delete(bassa_od_message *msg);

void 
bassa_search_message_delete(bassa_search_message *msg);

void 
bassa_messages_delete(bassa_messages *msg);

void 
bassa_message_start_handler(void *udata, char *name, char **attr);

void 
bassa_message_end_handler(void *udata, char *name);

void 
bassa_message_cdata_handler(void *udata, char *s, int len);

char*
bassa_assemble_message(char *element, char *s, int len);

char *
bassa_ackmessage_encode (char *status, char *code, char *redurl);
#endif //BASSA_MESSAGE
