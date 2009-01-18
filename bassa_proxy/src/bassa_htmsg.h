#ifndef HTTP_MSG_H
#define HTTP_MSG_H

#define HTTP_MSG_REQUEST 0
#define HTTP_MSG_REPLY 1

#define HTTP_BAD_METHOD -1
#define HTTP_NO_METHOD 0
#define HTTP_GET 1
#define HTTP_POST 2
#define HTTP_HEAD 3

#define HTTP_BAD_VERSION -1
#define HTTP_NO_VERSION 0
#define HTTP_1_0 1
#define HTTP_1_1 2

#define HTTP_BAD_STATUS -1
#define HTTP_NO_STATUS 0
#define HTTP_200_OK 1
#define HTTP_404_NOT_FOUND 2

#define MAX_HEADER_SIZE 2048

#define FIRST_LINE_READ(x)\
x->first_line_done?1:0

#define HEADER_READ(x)\
x->header_done?1:0


typedef struct
{
  char *field;
  int field_len;
  int field_id;
}bassa_header_field;

typedef struct
{
  int version;
  int method;
  int status;
  char *path;
  char *first_line;
  int first_line_len;
  int first_line_done;
  char *header;
  int header_len;
  int header_done;
  bassa_header_field* hdr_fields;
  int field_count;
  int type;
  char *body;
  int body_init_len;
  int total_recv;
}bassa_http_msg;

/**
 * Create a message with a type [HTTP_MSG_REPLY, HTTP_MSG_REQUEST]
 */
bassa_http_msg* bassa_http_msg_new(int type);

/**
 * Initialize a message of type HTTP_MSG_REPLY
 */
void bassa_http_init_reply(bassa_http_msg *bhm, int proto_v, 
                           int status);
/**
 * Initialize a message of type HTTP_MSG_REQUEST
 */
void bassa_http_init_request(bassa_http_msg *bhm, int proto_v,
                             int method, char *path);

void bassa_http_add_field (bassa_http_msg *msg, char *field, int field_len);

/**
 * Return 0 if buffer starting at start is not long enough to get the first line.
 * Return the length of first line if first line is found.
 * Return -1 on error.
 * start is first address of the memory buffer.
 * len is the length of the memory buffer.
 */
char* bassa_get_first_line(char *start, int len, bassa_http_msg *htmsg);

/**
 * Return 0 if header parsing is complete.
 * Returns a positive value if buffer is not enought to complete parsing.
 * Returns -1 on error.
 * len is the total length of the buffer from message start to curent end.
 * start is the starting address of the last recv()ed memory buffer. 
 */
char* bassa_parse_header(char *start, int len, bassa_http_msg *htmsg);

/**
 * Return bassa_http_msg after parsing the http msg.
 * Return NULL on error
 */
bassa_http_msg* bassa_parse_msg(int socket, int type);

bassa_header_field* bassa_header_field_new ();

void bassa_set_field_id (bassa_header_field *bhf);

void bassa_parse_first_line (bassa_http_msg *htmsg);

void bassa_http_msg_free (bassa_http_msg *htmsg);

bassa_header_field* bassa_get_field (int field, bassa_header_field *hdf, int len);

#endif //HTTP_MSG_H

