#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bassa_proxy.h"

int main(int arg, char* argv[])
{
  char *myaddr = "192.248.16.123";
  int port = 8080;
  int backlog = 10;
  bassa_proxy *bp = bassa_proxy_new(myaddr, port, backlog);
  if (bassa_proxy_start(bp)<0)
    perror("bassa_proxy_start");
  exit(0);
}

