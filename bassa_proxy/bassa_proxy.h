#ifndef BASSA_PROXY_H
#define BASSA_PROXY_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define DEFAULT_PORT 3128
#define DEFAULT_BACKLOG 25

#define DEFAULT_TIMEOUT 300

#define PROXY_CONNECTION 0
#define HOST_CONNECTION 1

#ifdef THIS_VERSION
#define VERSION THIS_VERSION
#endif //THIS_VERSION

#define SYSTEM_VENDOR "Bassa"

#define DEFAULT_READ_SIZE 1024
/*
 * Bassa Proxy
 */
typedef struct
{
  int port;
  char* host;
  int backlog;
  int socket;
  struct sockaddr_in addr;
  socklen_t addr_len;
  int optval;
} bassa_proxy; 

/*
 * Bassa client (browser/proxy)
 */
typedef struct
{
  struct sockaddr_in peer_addr;
  int client_socket;
  int server_socket;
  int timeout;
} bassa_client;

bassa_proxy* bassa_proxy_new (char *addr, int port, int backlog);
int bassa_proxy_start (bassa_proxy *bp);
int bassa_set_timeout (bassa_client *bc);

bassa_client* bassa_client_new ();
void* bassa_client_process (void *arg);
void bassa_client_free (bassa_client *bc);
int bsssa_client_connected (bassa_client *bc);

int bassa_make_connection (char *host_addr, int port);

int bassa_http_send (int socket, char *buff);

int bassa_send_header (int socket, char *first_line, int first_line_len, 
                       char *fields, int fields_len);

int bassa_send_first_line (int socket, char *first_line, int first_line_len);

int bassa_send_field (int socket, char *field, int field_len);

int bassa_send_body (int socket, char *body, int body_len);

int bassa_body_pipe (int iskt, int oskt, int body_len);

void bassa_sigpipe_handler (int signum);

#endif //BASSA_PROXY_H

