/***************************************************************************
 *   Copyright (C) 2007 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/
#ifndef NOC_FILTER_SERVER_H
#define NOC_FILTER_SERVER_H

#include <config.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define DEFAULT_PORT 8095
#define DEFAULT_BACKLOG 10
#define EOT 0x04
#define LF 0x0A
#define CR 0x0D

typedef struct
{
  char *local_addr;
  int socket;
  int backlog;
  struct sockaddr_in addr;
  socklen_t addr_len;
  int optval;
} bassa_server;

typedef struct
{
  struct sockaddr_in peer_addr;
  int client_socket;
  int timeout;
} bassa_client;

bassa_server *bassa_server_new(char *local_addr, int port, int backlog);

bassa_client *bassa_client_new();

int bassa_server_bind(bassa_server *server);

int bassa_server_listen(bassa_server *server);

bassa_client *bassa_server_accept(bassa_server *server);

int bassa_server_shutdown(bassa_server *server);

int bassa_client_close(bassa_client *client);

int bassa_client_shutdown(bassa_client *client);

char *bassa_client_read(bassa_client *client, int size);

int bassa_client_write(bassa_client *client, char *data);

void bassa_server_delete(bassa_server *server);

void bassa_client_delete(bassa_client *client);

int bassa_client_settimeout(bassa_client *client, int timeout);
#endif	//NOC_FILTER_SERVER_H
