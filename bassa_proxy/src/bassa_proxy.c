#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "bassa_proxy.h"
#include "bassa_htmsg.h"
#include "bassa_field_macros.h"
#include "bassa_htrep.h"
#include "bassa_pack.h"

bassa_proxy* bassa_proxy_new (char *host, int port, int backlog)
{
  //Register Signal handlers
  signal(SIGPIPE, bassa_sigpipe_handler);
  bassa_proxy *bp = (bassa_proxy*)malloc(sizeof(bassa_proxy));
  bp->port = port;
  bp->host = host;
  bp->backlog = backlog;
  bp->socket = socket(AF_INET, SOCK_STREAM, 0);
  bp->addr.sin_family = AF_INET;
  bp->addr_len = sizeof(struct sockaddr_in);
  if(port)
    bp->addr.sin_port = htons(port);
  else
    bp->addr.sin_port = htons(DEFAULT_PORT);
  if(backlog)
    bp->backlog = backlog;
  else
    bp->backlog = DEFAULT_BACKLOG;
  bp->addr.sin_addr.s_addr = inet_addr(host);
  memset(&(bp->addr.sin_zero), '\0', 8);
  if(!bp->socket)
    return NULL;
  else
    {
      bp->optval = 1;
      setsockopt(bp->socket, SOL_SOCKET, SO_REUSEADDR, &(bp->optval), sizeof(bp->optval));
      return bp;
    }
  return bp;
}

int bassa_proxy_start (bassa_proxy *bp)
{
  int r = bind(bp->socket, (struct sockaddr*)&(bp->addr), sizeof(bp->addr));
  if(r<0)
    perror("BIND");
  r = listen(bp->socket, bp->backlog);
  if(r<0)
    perror("LISTEN");
  struct sockaddr_in peer_addr;
  int socket;
  while((socket=accept(bp->socket, (struct sockaddr*)&(peer_addr), &(bp->addr_len))))
    {
      pthread_t tid;
      int *hsock = (int*)malloc(sizeof(int));
      *hsock = socket;
      pthread_create (&tid, NULL, bassa_client_process, (void*)hsock);
    }
  return r;
}

bassa_client* bassa_client_new ()
{
  bassa_client *bc = (bassa_client*)malloc(sizeof(bassa_client));
  return bc;
}

void bassa_client_free (bassa_client *bc)
{
  if (bc)
    {
      free(bc);
      bc = NULL;
    }
}

int bassa_set_timeout (bassa_client *bc)
{
  return 0;
}

void* bassa_client_process (void *arg)
{
  bassa_http_msg *htmsg = NULL, *htmsg2 = NULL;
  //struct sockaddr_in peer_addr;
  int *clskt = (int*)(arg);
  bassa_client *bc = bassa_client_new();
  //bc->peer_addr = peer_addr;
  bc->client_socket = *clskt;
  int ret = 0;
  struct timeval to;
  to.tv_sec = 30;
  to.tv_usec = 0; 
  fd_set rs, es;
  FD_ZERO(&rs);
  FD_ZERO(&es);
  FD_SET(*clskt, &rs);
  FD_SET(*clskt, &es);
  int count = 0, conn_stat = 0;
  char *conn = NULL;
  while (1)
    {
      if ((ret=select((*clskt)+1, &rs, NULL, &es, &to)) > 0)
        {
          if (ret < 0)
            break;
          if (FD_ISSET(*clskt, &rs))
            {  
              if (!bsssa_client_connected(bc))
                break;
              printf (">ACCEPTED NEW CONNECTION: %i\n", ++count);
              htmsg = bassa_parse_msg(bc->client_socket, HTTP_MSG_REQUEST);
              bassa_header_field *hdf = bassa_get_field (HOST, htmsg->hdr_fields, htmsg->field_count);
	      if (!hdf)
                {
                  bassa_http_msg_free(htmsg);
                  break;
                }
	      bassa_header_field *kaf = bassa_get_field (PROXY_CONN, htmsg->hdr_fields, htmsg->field_count);
	      if (kaf)
		conn = bassa_unpack_val(kaf->field, kaf->field_len);
	      else
		conn = "close";
	      if (!strcmp(conn, "close"))
		conn_stat = 0;
	      else
		conn_stat = 1;
              char *host = bassa_unpack_val(hdf->field, hdf->field_len);
	      printf ("Host: %s\n", host);
              bc->server_socket = bassa_make_connection ("192.248.16.123", 80);
              if (bassa_send_header (bc->server_socket, htmsg->first_line, htmsg->first_line_len,
                                     htmsg->header, htmsg->header_len)<0)
                printf ("Error Sending\n");
              printf (">>>>Recieving Reply\n");
              htmsg2 = bassa_parse_msg(bc->server_socket, HTTP_MSG_REPLY);
              printf (">>>>Rceieved Reply\n");
	      //Sending recieved header and starting part of the body read with header
              if (bassa_send_header (bc->client_socket, htmsg2->first_line, htmsg2->first_line_len,
                                 htmsg2->header, htmsg2->header_len)<0)
                printf ("Error Sending Header\n");
              if (bassa_send_body (bc->client_socket, htmsg2->body, htmsg2->body_init_len) < 0)
                printf ("Error sending bodylet\n");
	      printf ("Sent partial content and complete headers\n");
              hdf = bassa_get_field (CONTENT_LENGTH, htmsg2->hdr_fields, htmsg2->field_count);
              int rem_len = 0;
	      if (hdf)
	      {
		char *clen = bassa_unpack_val(hdf->field, hdf->field_len);
		int len = atoi(clen);
		rem_len = len - htmsg2->body_init_len;
		printf ("Length: %i\n", len);
		printf ("Body_init_len: %i\n", htmsg2->body_init_len);
		printf ("Rem_len: %i\n", rem_len);                 
	      }
	      else
	      {
		printf ("COUNLD NOT FIND CONTENT LENGTH\n");
		rem_len = -1;
	      }
	      printf ("Going to pipe body\n");
              bassa_body_pipe (bc->server_socket, bc->client_socket, rem_len);
              printf ("Piped body\n");
	      bassa_http_msg_free(htmsg);
              printf (">COMPLETED CONNECTION\n");
	      if (!conn_stat)
                break;		
            }
          if (FD_ISSET(*clskt, &es))
            {  
              printf ("Exception\n");
              break;
            }
        } 
      else
        break;
    }
  printf ("FINISHED\n");
  close(bc->client_socket);
  bassa_client_free(bc);
  free(clskt);
  return NULL;
}

int bsssa_client_connected (bassa_client *bc)
{
  //Read one bit but do not take out data from socket using recv.
  //If recv returns 0 then the socket is closed by client.
  #define FAKE_BUF_SIZE 1
  if (!bc)
    return -1;
  char fake_buf[FAKE_BUF_SIZE];
  if (recv(bc->client_socket, fake_buf, FAKE_BUF_SIZE, MSG_PEEK | MSG_DONTWAIT) == 0)
    return 0;
  else
    return 1;
}

int bassa_make_connection (char *host_addr, int port)
{
  struct sockaddr_in addr;
  socklen_t addr_len;
  int skt;
  skt = socket(AF_INET, SOCK_STREAM, 0);
  addr.sin_family = AF_INET;
  addr.sin_port = port;
  if (port)
    addr.sin_port = htons(port);
  else
    {
      close (skt);
      return -1;
    }
  if (host_addr)
    addr.sin_addr.s_addr = inet_addr(host_addr);
  else
    {
      close (skt);
      return -1;
    }
  addr_len = sizeof(struct sockaddr_in);
  memset(&(addr.sin_zero), '\0', 8);
  if (connect(skt, (struct sockaddr*)&addr, addr_len)<0)
    return -1;
  else
    return skt;
}

int bassa_send_header (int skt, char *first_line, int first_line_len, 
                       char *fields, int fields_len)
{
  if (bassa_send_first_line(skt, first_line, first_line_len) == -1)
    return -1;
  if (bassa_send_field(skt, fields, fields_len) == -1)
    return -1;
  return 0;
}

int bassa_send_first_line (int skt, char *first_line, int first_line_len)
{
  //printf ("\n=============== START HEADERS ===============\n");
  //write (fileno(stdout), first_line, first_line_len);
  if (send(skt, first_line, first_line_len, 0) == -1)
  {
    perror("SEND");
    return -1;
  }
  return 0;
}

int bassa_send_field (int skt, char *field, int field_len)
{
  //write (fileno(stdout), field, field_len);
  //printf ("\n================ END HEADERS  ===============\n");
  if (send(skt, field, field_len, 0) == -1)
  {
    perror("SEND");
    return -1;
  }
  return 0;
}

int bassa_send_body (int skt, char *body, int body_len)
{
  //printf ("\n=============== START BODY ===============\n");
  //write (fileno(stdout), body, body_len);
  //printf ("\n================  END BODY ===============\n");
  if (send(skt, body, body_len, 0) < 0)
  {
    perror("Send body");
    return -1;
  }
  else
    return 0;
}

int bassa_body_pipe (int iskt, int oskt, int body_len)
{
  int rc = 0, trc = 0, sn = 0;
  printf ("Body Len: %i\n", body_len);
  if (body_len > 0)
  {
    if (sendfile(oskt, iskt, NULL, body_len)<0)
    {
      if (errno == EINVAL || errno == ENOSYS)
      {
	while (rc <= body_len)
	{
	  char *buff = (char*)malloc(DEFAULT_READ_SIZE);
	  //memset(buff, (int)'\0', DEFAULT_READ_SIZE);
	  if ((trc = recv(iskt, buff, DEFAULT_READ_SIZE, 0))<0)
	  {
	    free(buff);
	    return -1;
	  }
	  rc += trc;
	  if ((sn = send(oskt, buff, trc, 0))<0)
	  {
	    free(buff);
	    return -1;
	  }
	  if (trc != sn)
	  {
	    free(buff);
	    return -1;
	  }
	  free(buff);
	  buff = NULL;
	}
	return 0;
      }
      else
	return 0;
    }
    else
      return 0;
  }
  else if (body_len == -1)
  {
    while (1)
    {
      char *buff = (char*)malloc(DEFAULT_READ_SIZE);
      rc = recv(iskt, buff, DEFAULT_READ_SIZE, 0);
      sn = send(oskt, buff, rc, 0);
      if (sn != rc)
      {
	free(buff);
	return -1;
      }
      if (rc == DEFAULT_READ_SIZE)
      {
	free(buff);
	continue;
      }
      else if (rc == 0)
      {
	free(buff);
	return 0;
      }
    }
    return 0;
  }
  else
    return 0;
}

void bassa_sigpipe_handler (int signum)
{
  printf ("Error: Destination socket closed\n");
  return;
}

