#include <bassa_prp_context.h>
#include <bassa_module_manager.h>
#include <stdio.h>
#include <stdlib.h>

bassa_module_region module_region = PRP_REGION;
bassa_exec_point module_execpoint = PRP_QUEUE;

int module_exec (void *ctx);
void module_close (void *ctx);


int 
module_exec (void *ctx)
{
  bassa_prp_context *prpc = (bassa_prp_context*)ctx;
  prpc->routing_url = 1;
  prpc->url = "http://www.bakamuna.com/abc.tgz";
  printf ("Hello World\n");
  return 0;
}

void 
module_exit (void *ctx)
{
  printf ("Goodbye World\n");
}

