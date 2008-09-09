#include "soapH.h"
#include "bassa.nsmap"
#include <stdio.h>

#ifndef ASEND
#define ASEND 0
#endif //ASEND

#ifndef DESND
#define DESND 1
#endif //DESND

#define RVAL_URL "url"

int main(int argc, char *argv[])
{
  printf ("Content-Type: text/html\r\n\n");
  char svr[] = "http://localhost:8095/bassa"; 
  printf ("<html>\n<head></head>\n<body>\n");
  struct bassa__request br;
  char *url = getenv("QUERY_STRING");
  char *nurl = NULL;
  if (strlen(url) > strlen(RVAL_URL))
  {
    nurl = url+strlen(RVAL_URL)+1;
  }
  br.url = nurl; 
  char *response = NULL;
  struct soap s;
  soap_init (&s);
  int offset = 0;
  int i,j;
  struct bassa__file_set fs;
  soap_call_bassa__enqueue(&s, svr, NULL, &br, &response);
  printf ("%s\n", response);
  j = 0;
  /*do
  {
    soap_call_bassa__list_all(&s, svr, NULL, j*(fs.object_limit), ASEND, &fs);
    //soap_call_bassa__search(&s, svr, NULL, j*(fs.object_limit), nurl, ASEND, &fs);
    printf ("<b>Total Results: %i</b><br/>\n", fs.total);
    printf ("<b>Offset: %i</b><br/>\n", fs.offset);
    printf ("<b>Object Limit: %i</b><br/>\n", fs.object_limit);
    for (i=0; i<fs.total; i++)
    {
      printf ("File Name: %s<br/>\n", fs.file[i]);
      printf ("File URL: %s<br/>\n", fs.url[i]);
      printf ("Local URL: %s<br/>\n", fs.local_url[i]);
      printf ("Content Length: %i<br/>\n", fs.content_length[i]);
      printf ("Date: %lu<br/>\n", fs.date[i]);
    }
    j++;
  }
  while(fs.total == fs.object_limit);*/
  /*soap_call_bassa__latest_downloads(&s, svr, NULL, j*(fs.object_limit), &fs);
  printf ("<b>Total Results: %i</b><br/>\n", fs.total);
  printf ("<b>Offset: %i</b><br/>\n", fs.offset);
  printf ("<b>Object Limit: %i</b><br/>\n", fs.object_limit);
  for (i=0; i<fs.total; i++)
  {
    printf ("File Name: %s<br/>\n", fs.file[i]);
    printf ("File URL: %s<br/>\n", fs.url[i]);
    printf ("Local URL: %s<br/>\n", fs.local_url[i]);
    printf ("Content Length: %i<br/>\n", fs.content_length[i]);
    printf ("Start Time: %lu<br/>\n", fs.start_time[i]);
    printf ("End Time: %lu<br/>\n", fs.end_time[i]);
  }*/
  printf ("</body>\n</html>\n");
  if (s.error)
  { 
    soap_print_fault(&s, stderr);
    exit(1);
  }
  exit (0);
}

