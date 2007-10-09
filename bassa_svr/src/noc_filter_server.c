/***************************************************************************
 *   Copyright (C) 2007 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <curl/curl.h>
#include <errno.h>

#include "noc_filter_server.h"

bassa_server* bassa_server_new(char *local_addr, int port, int backlog)
{
#ifdef DEBUG
  printf("SERVER_NEW_FUNCTION\n");
#endif
  bassa_server *server = (bassa_server*)malloc(sizeof(bassa_server));
  server->socket = socket(AF_INET, SOCK_STREAM, 0);
  server->addr.sin_family = AF_INET;
  server->addr_len = sizeof(struct sockaddr_in);
  if(port)
    server->addr.sin_port = htons(port);
  else
    server->addr.sin_port = htons(DEFAULT_PORT);
  if(backlog)
    server->backlog = backlog;
  else
    server->backlog = DEFAULT_BACKLOG;
  server->addr.sin_addr.s_addr = inet_addr(local_addr);
  memset(&(server->addr.sin_zero), '\0', 8);
  if(!server->socket)
    return NULL;
  else
    {
      server->optval = 1;
      setsockopt(server->socket, SOL_SOCKET, SO_REUSEADDR, &(server->optval), sizeof(server->optval));
      return server;
    }
}

bassa_client *bassa_client_new()
{
#ifdef DEBUG
  printf("CLIENT_NEW_FUNCTION\n");
#endif
  bassa_client *client = (bassa_client*)malloc(sizeof(bassa_client));
  return client;
}

int bassa_server_bind(bassa_server *server)
{
#ifdef DEBUG
  printf("BIND_FUNCTION\n");
#endif
  int r = bind(server->socket, (struct sockaddr*)&(server->addr), sizeof(server->addr));
  if(r<0)
    perror("BIND");
  return r;
}

int bassa_server_listen(bassa_server *server)
{
#ifdef DEBUG
  printf("LISTEN_FUNCTION\n");
#endif
  int r = listen(server->socket, server->backlog);
  if(r<0)
    perror("LISTEN");
  return r;
}

bassa_client* bassa_server_accept(bassa_server *server)
{
#ifdef DEBUG
  printf("ACCEPT_FUNCTION\n");
#endif
  bassa_client *client = bassa_client_new();
  client->client_socket = accept(server->socket, (struct sockaddr*)&(client->peer_addr), &(server->addr_len));
  if(client->client_socket > 0)
    {
#ifdef DEBUG
      printf("Client socket is: %i\n", client->client_socket);
#endif //DEBUG
      return client;
    }
  else
    {
      perror("ACCEPT");
      bassa_client_delete(client);
      return NULL;
    }
}

int bassa_server_shutdown(bassa_server *server)
{
#ifdef DEBUG
  printf("SHUTDOWN_FUNCTION\n");
#endif
  int r = shutdown (server->socket, SHUT_RDWR);
  if(r<0)
    perror("CLOSE");
  return r;
}

int bassa_client_close(bassa_client *client)
{
#ifdef DEBUG
  printf("CLOSE_FUNCTION\n");
#endif
  int r = close(client->client_socket);
  if(r<0)
    perror("CLOSE");
  return r;
}

int bassa_client_shutdown(bassa_client *client)
{
#ifdef DEBUG
  printf("SHUTDOWN_FUNCTION\n");
#endif
  int r = shutdown (client->client_socket, SHUT_RDWR);
  if(r<0)
    perror("CLOSE");
  return r;
}

char *bassa_client_read(bassa_client *client, int size)
{
#ifdef DEBUG
  printf("READ_FUNCTION\n");
#endif
  char *data = (char*)malloc(size+1);
  int xrd = read(client->client_socket, data, size);
  int read_err = errno;
  //if (xrd > 0)
  //  data[xrd] = '\0';
  if(xrd == 0)
    {
      free(data);
      return NULL;
    }
  else if(xrd == -1 && read_err == EWOULDBLOCK && read_err == EAGAIN)
    {
      free(data);
      return NULL;
    }
  else
    {
      data[xrd] = '\0';
      char *xdata = (char*)malloc(size+1);
      int i, xdata_idx;
      int chsize = size;
      char *amp = "&";
      for (i=0, xdata_idx=0; i<size; i++,xdata_idx++)
	{
	  if (data[i] == '&')
	    {
	      char *x = curl_easy_escape (NULL, amp, 0);
	      int j = strlen (x);				
	      chsize += j;
	      xdata_idx += j - 1; 
	      xdata = realloc(xdata, chsize + 1);
	      xdata[i] = '\0';
	      strcat(xdata, x);
	      free (x);
	    }
	  else
	    {
	      xdata[xdata_idx] = data[i];
	      xdata[xdata_idx + 1] = '\0';
	    }	
	}
      return xdata;
    }
}

int bassa_client_write(bassa_client *client, char *data)
{
#ifdef DEBUG
  printf("WRITE_FUNCTION\n");
#endif
  int r = write(client->client_socket, data, strlen(data));
#ifdef DEBUG
  printf("Written: %i\n", r);
#endif
  if(r<0)
    perror("WRITE");
  return r;
}

void bassa_server_delete(bassa_server *server)
{
#ifdef DEBUG
  printf("SERVER_DELETE_FUNCTION\n");
#endif
  //free(server->local_addr);
  free(server);
}

void bassa_client_delete(bassa_client *client)
{
#ifdef DEBUG
  printf("CLIENT_DELETE_FUNCTION\n");
#endif
  free(client);
}

int 
bassa_client_settimeout(bassa_client *client, int timeout)
{
#ifdef DEBUG
  printf ("CLIENT_SETTIMEOUT_FUNCTION\n");
#endif //DEBUG
  client->timeout = timeout;
  struct timeval tv;
  tv.tv_sec = client->timeout;
  tv.tv_usec = 0;
  int r1 = setsockopt (client->client_socket, SOL_SOCKET, SO_RCVTIMEO, &(tv), sizeof(struct timeval));
  //int r2 = setsockopt (client->client_socket, SOL_SOCKET, SO_SNDTIMEO, &(tv), sizeof(struct timeval));
  if (r1 < 0 /*|| r2 < 0*/)
    return -1;
  else
    return 0;
}
