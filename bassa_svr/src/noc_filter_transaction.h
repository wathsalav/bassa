/***************************************************************************
 *   Copyright (C) 2006 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#ifndef NOC_FILTER_TRANSACTION_H
#define NOC_FILTER_TRANSACTION_H

#include <config.h>
#include <curl/curl.h>
#include <stdio.h>

#include <noc_filter_configure.h>
#include <bassa_uri.h>
#include <bassa_db.h>

//Protocols
//HTTP
#define HTTP_PROTO			"http"
#define HTTP_PROTO_FLAG 		0x1
#define IS_HTTP_PROTO(bf)			\
  (GET_PROTO(bf)==HTTP_PROTO_FLAG)?1:0

//HTTPS
#define HTTPS_PROTO 			"https"
#define HTTPS_PROTO_FLAG 		0x2
#define IS_HTTPS_PROTO(bf)			\
  (GET_PROTO(bf)==HTTPS_PROTO_FLAG)?1:0

//FTP
#define FTP_PROTO			"ftp"
#define FTP_PROTO_FLAG 			0x4
#define IS_FTP_PROTO(bf)			\
  (GET_PROTO(bf)==FTP_PROTO_FLAG)?1:0

//Unknown to Bassa
#define UNKNOWN_PROTO 			0x8

//Internal file extensions
#define DOWNLOAD_PART_EXT 		".part"

//Special HTTP headers
#define HTTP_RANGE_X_TO_END 		"Range: bytes=%i-"
#define HTTP_RANGE_X_TO_Y 		"Range: bytes=%i-%i"
#define HTTP_RANGE_LAST_X		"Range: bytes=-%i"

//OK response
#define HTTP_OK				"HTTP/1.1 200 OK\r\n"
#define HTTP_OK_1_0			"HTTP/1.0 200 OK\r\n"
#define HTTP_OK_FLAG			200
#define IS_OK(bf)				\
  (GET_STATUS(bf)==HTTP_OK_FLAG)?1:0

//No content
#define HTTP_NO_CONTENT			"HTTP/1.1 204 No Content\r\n"
#define HTTP_NO_CONTENT_1_0		"HTTP/1.0 204 No Content\r\n"
#define HTTP_NO_CONTENT_FALG		204
#define IS_NO_CONTENT(bf)			\
  (GET_STATUS(bf)==HTTP_NO_CONTENT_FALG)?1:0

//Partial content headers
#define HTTP_PARTIAL_CONTENT		"HTTP/1.1 206 Partial Content\r\n"
#define HTTP_PARTIAL_CONTENT_1_0 	"HTTP/1.0 206 Partial Content\r\n"
#define HTTP_PARTIAL_CONTENT_FLAG 	206
#define IS_PARTIAL_CONTENT(bf)				\
  (GET_STATUS(bf)==HTTP_PARTIAL_CONTENT_FLAG)?1:0

//Bad request
#define HTTP_BAD_REQUEST		"HTTP/1.1 400 Bad Request\r\n"
#define HTTP_BAD_REQUEST_1_0		"HTTP/1.0 400 Bad Request\r\n"
#define HTTP_BAD_REQUEST_FLAG		400
#define IS_BAD_REQUEST(bf)			\
  (GET_STATUS(bf)==HTTP_BAD_REQUEST_FLAG)?1:0

//Unauthorized
#define HTTP_UNAUTHORIZED		"HTTP/1.1 401 Unathorized\r\n"
#define HTTP_UNAUTHORIZED_1_0		"HTTP/1.0 401 Unathorized\r\n"
#define HTTP_UNAUTHORIZED_FLAG		401
#define IS_UNAUTHORIZED(bf)			\
  (GET_STATUS(bf)==HTTP_UNAUTHORIZED_FLAG)?1:0

//Forbidden
#define HTTP_FORBIDDEN			"HTTP/1.1 403 Forbidden\r\n"
#define HTTP_FORBIDDEN_1_0		"HTTP/1.0 403 Forbidden\r\n"
#define HTTP_FORBIDDEN_FLAG		403
#define IS_FORBIDDEN(bf)			\
  (GET_STATUS(bf)==HTTP_FORBIDDEN_FLAG)?1:0

//Not found
#define HTTP_NOT_FOUND			"HTTP/1.1 404 Not Found\r\n"
#define HTTP_NOT_FOUND_1_0		"HTTP/1.0 404 Not Found\r\n"
#define HTTP_NOT_FOUND_FLAG		404
#define IS_NOT_FOUND(bf)			\
  (GET_STATUS(bf)==HTTP_NOT_FOUND_FLAG)?1:0

//TImeout
#define HTTP_REQUEST_TIMEOUT		"HTTP/1.1 408 Request Timeout\r\n"	
#define HTTP_REQUEST_TIMEOUT_1_0	"HTTP/1.0 408 Request Timeout\r\n"
#define HTTP_REQUEST_TIMEOUT_FLAG	408
#define IS_REQUEST_TIMEOUT(bf)				\
  (GET_STATUS(bf)==HTTP_REQUEST_TIMEOUT_FLAG)?1:0

//Internal server error
#define HTTP_INTERNAL_SERVER_ERROR	"HTTP/1.1 500 Internal Server Error\r\n"
#define HTTP_INTERNAL_SERVER_ERROR_1_0	"HTTP/1.0 500 Internal Server Error\r\n"
#define HTTP_INTERNAL_SERVER_ERROR_FLAG	500
#define IS_INTERNAL_SERVER_ERROR(bf)			\
  (GET_STATUS(bf)==HTTP_INTERNAL_SERVER_ERROR_FLAG)?1:0

//HTTP version not supported
#define HTTP_VERSION_NOT_SUPPORTED	"HTTP/1.1 505 HTTP Version Not Supported\r\n"
#define HTTP_VERSION_NOT_SUPPORTED_1_0	"HTTP/1.0 505 HTTP Version Not Supported\r\n"
#define HTTP_VERSION_NOT_SUPPORTED_FLAG	505
#define IS_VERSION_NOT_SUPPORTED(bf)			\
  (GET_STATUS(bf)==HTTP_VERSION_NOT_SUPPORTED_FLAG)?1:0

#define HTTP_SERVICE_UNAVAILABLE	"HTTP/1.1 503 Service Unavailable\r\n"
#define HTTP_SERVICE_UNAVAILABLE_1_0	"HTTP/1.0 503 Service Unavailable\r\n"
#define HTTP_SERVICE_UNAVAILABLE_FLAG	503
#define IS_SERVICE_UNAVAILABLE(bf)			\
  (GET_STATUS(bf)==HTTP_SERVICE_UNAVAILABLE_FLAG)?1:0

//HTTP Content-Length header
#define HTTP_CONTENT_LENGTH		"Content-Length:"

//HTTP Content-Disposition header
#define HTTP_CONTENT_DISPOSITION	"Content-Disposition:"
#define HTTP_CONTENT_DISP_ATTACHMENT	"attachment"
#define HTTP_CONTENT_DISP_ATCHMNT_ID	0x01
#define HTTP_CONTENT_DISP_FNAME "filename"

//HTTP response mask
#define HTTP_RESPONSE_MASK		0x1FF << 19

//Protocol mask
#define PROTOCOL_MASK			0xF << 28

//Set the HTTP status in a given bit feild
#define SET_STATUS(bf, status)					\
  unsigned int macro_set_stat = ((unsigned int)status) << 19;	\
  (bf) |= (macro_set_stat)

//Get the HTTP status in a given bit feild
#define GET_STATUS(bf)				\
  ((bf) & (HTTP_RESPONSE_MASK)) >> 19

//Set the protocol type in a bit feild
#define SET_PROTO(bf, proto)			\
  unsigned long int macro_set_proto = proto;	\
  macro_set_proto = macro_set_proto << 28;	\
  (bf) |= (macro_set_proto)

//Get the protocol 
#define GET_PROTO(bf)				\
  ((bf) & (PROTOCOL_MASK)) >> 28

//Tmie gap between retries
#ifndef RETRY_GAP_MICROS
#define RETRY_GAP_MICROS 1000*1000*1
#endif
//Default connect_timeout 
#define CONNECT_TIMEOUT 20
//Defailt write size in bytes
#define WRITE_SIZE 1024

//Externs 
extern char *sys_proxy;

typedef unsigned int flag_t;

typedef struct
{
  char *file_part;
  char *file_complete;
  //char *url;
  char *http_proxy;
  FILE *local_file;
  CURL *curl_handle;
  struct curl_slist *header_list;
  unsigned int protocol_flag;
  long connect_timeout;
  int max_tries;
  flag_t http_bf;
  char* disposition_fname;
  int disposition_type;
  int bassa_status;
  bassa_irequest *birq;
} bassa_transaction;

extern bassa_conf *conf;

bassa_transaction*
bassa_transaction_new(bassa_irequest *bir);

int 
bassa_transaction_download(bassa_transaction *transaction);

void 
bassa_transaction_free(bassa_transaction *transaction);

int 
bassa_transaction_open(bassa_transaction *transaction);

//void
//bassa_transaction_set_flag (bassa_transaction_flags *flags, int flag);

void
bassa_transaction_set_options (bassa_transaction *transaction, int connect_timeout,
			       int status);

int
bassa_transaction_get_response (CURL* handle, curl_infotype type, 
				char* data, size_t size, void* para);

void
bassa_transcation_set_network_opts (bassa_transaction *transaction,
				    long connect_timeout, int max_tries);

char*
bassa_transaction_get_value (char *header_line, char *header_name);

void
bassa_transaction_set_http_init (bassa_transaction *transaction);

void
bassa_transaction_set_http_resume (bassa_transaction *transaction, int current_size);

void
bassa_transaction_prp_reinit (bassa_transaction *transaction, char *url, char *local_name, 
			  long long size, char *http_proxy);

size_t 
bassa_transaction_fwriter (void *ptr, size_t size, 
			  size_t nmemb, void *stream);
#endif

