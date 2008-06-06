#include <pthread.h> 
#include <signal.h>

#include <soapH.h>
#include <bassa_ws_server.h>
 
int bassa_ws_start() 
{ 
  struct soap soap;
  soap_init(&soap); 
  soap.max_keep_alive = 100; // max keep-alive sequence 
  struct soap *tsoap; 
  SOAP_SOCKET m, s; 
  m = soap_bind(&soap, conf->svrcfg->server_address, 
                conf->svrcfg->server_port, 
                conf->svrcfg->server_backlog); 
  if (!soap_valid_socket(m)) 
    exit(1); 
  fprintf(stderr, "Socket connection successful on port:%d\n", conf->svrcfg->server_port); 
  int i;
  i=0;
  while(1)
    { 
      s = soap_accept(&soap); 
      if (!soap_valid_socket(s)) 
		    { 
	        if (soap.errnum) 
	    	    { 
	      	    soap_print_fault(&soap, stderr); 
	            exit(1); 
	    	    } 
	  	    fprintf(stderr, "server timed out\n"); 
	  	    break; 
		    }
      tsoap = soap_copy(&soap); // make a safe copy 
      if (!tsoap) 
		    break; 
      bassa_nowait_spawn(NULL, bassa_ws_exec, (void*)tsoap); 
    }
  soap_done(&soap); // detach soap struct 
  return 0; 
} 

void *bassa_ws_exec(void *soap) 
{ 
  soap_serve((struct soap*)soap); 
  soap_destroy((struct soap*)soap); // dealloc C++ data 
  soap_end((struct soap*)soap);     // dealloc data and clean up 
  soap_done((struct soap*)soap);    // detach soap struct 
  free(soap); 
  return NULL; 
} 
 
 
