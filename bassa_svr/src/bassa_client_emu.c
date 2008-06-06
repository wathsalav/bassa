#include "soapH.h"
#include "bassa.nsmap"
#include <stdio.h>

#ifndef ASEND
#define ASEND 0
#endif //ASEND

#ifndef DESND
#define DESND 1
#endif //DESND

int main(int argc, char *argv[])
{
  char svr[] = "http://localhost:8095/bassa";
  struct bassa__file_set *fs = (struct bassa__file_set*)
                                malloc(sizeof(struct bassa__file_set));
  struct bassa__request br;
  br.url = "http://www.google.com/ghop2012.pdf";
  br.content_length = 1287;
  char *response = NULL;
  struct soap s;
  soap_init (&s);
  int offset = 0;
  int i;
  switch (*argv[1])
  {
    case 's':
      soap_call_bassa__search (&s, svr, "", offset, "google.com", ASEND, &fs);
      for (i=0; i<fs->total; i++)
        {
          printf ("File Name: %s\n", fs->file[i]);
          printf ("Origin URL: %s\n", fs->url[i]);
          printf ("Cache URL: %s\n", fs->local_url[i]);
          printf ("Content Length: %i\n", fs->content_length[i]);
          printf ("Status: %s\n", fs->status[i]);
        }
      break;
    case 'q':
      soap_call_bassa__enqueue (&s, svr, "", &br, &response);
      printf ("Response: %s\n", response);
      break;
  }
  if (s.error)
  { 
    soap_print_fault(&s, stderr);
    exit(1);
  }
  exit (0);
}
