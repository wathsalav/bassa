/***************************************************************************
 *   Copyright (C) 2006 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>

#include "noc_filter_file.h"

#define SEPARATOR "/"
		 
char *bassa_file_get_name(char *url)
{
  char *token = NULL;
  char *prev_token;
  char *sep = SEPARATOR;
  char *xurl = NULL;
  if(url)
    {
      xurl = (char*)malloc(strlen(url)+1);
      char *backup_xurl = xurl;
      char *xprev_token;
      strcpy(xurl, url);
      for(;;xurl = NULL)
	{
	  prev_token = token;
	  token = strtok(xurl, sep);
	  if(!token)
	    {
	      xprev_token = (char*)malloc(strlen(prev_token) + 1);
	      strcpy(xprev_token, prev_token);
	      break;
	    }
	}
      free(backup_xurl);
      int tok_len = strlen(xprev_token);
      char *tmp_tok = xprev_token;
      char qtriplet[3] = {'%', '3', 'F'};
      char quescap = '?';
      char atriplet[3] = {'%', '2', '6'};
      char auescap = '&';
      int c,i;
      for (c=0, i=0; c<tok_len; c++)
	{
	  if (tmp_tok[c] == quescap ||
	      tmp_tok[c] == auescap)
	    break;
	  if (tmp_tok[c] == qtriplet[i])
	    {
	      if (tmp_tok[++c] == qtriplet[++i] ||
		  tmp_tok[c] == atriplet[i])
		{
		  if (tmp_tok[++c] == qtriplet[++i] || 
		      tmp_tok[c] == atriplet[i])
		    {
		      c -= 2;
		      i = 0;
		      break;
		    }
		  else
		    {
		      c -=2;
		      i = 0;
		    }
		} 
	      else
		{
		  c -= 1;
		  i = 0;
		}
	    }	
	}
      char *file_name = (char*)malloc(c+1);
      strncpy (file_name, xprev_token, c);
      file_name [c] = '\0';
      free (xprev_token);
      xprev_token = NULL;
      return file_name;
    }
  return NULL;
}

char *bassa_get_location (char *url)
{
  int url_len = strlen (url);
  int i;
  for (i=url_len-1; i >= 0; i--)
    {
      if (url[i] == '/')
	break;
    }
  int file_name_len = i + 1;
  char *path = (char*)malloc(file_name_len + 1);
  strncpy (path, url, file_name_len);
  path [file_name_len] = '\0';
  return path;	
}

char *bassa_file_get_extension(char *file_name)
{
  int len = strlen(file_name);
  int i;
  char *dot = NULL;
  for(i=len; i>=0; i--)
    {
      if(file_name[i] == '.')
	{
	  dot = &(file_name[i]);
	}
    }
  char *ext = (char*)malloc(strlen(dot) + 1);
  strcpy(ext, dot);
  return ext;
}

char *bassa_file_trim_extension(char *file_name)
{
  int len = strlen(file_name);
  int i;
  char *dot = NULL;
  for (i=len; i>=0; i--)
    {
      if(file_name[i] == '.')
	{
	  dot = &(file_name[i]);
	  break;
	}
    }
  int dot_len = strlen(dot) - 1;
  int name_only_len = len - dot_len;
  char *name_only = (char*)malloc(name_only_len + 1);
  strncpy(name_only, file_name, name_only_len);
  name_only[name_only_len] = '\0';
  return name_only;
}

void bassa_file_set_max_size (int limit)
{
  struct rlimit xlimit;
  xlimit.rlim_cur = limit;
  xlimit.rlim_max = limit;

  if (setrlimit(RLIMIT_FSIZE, &xlimit))
    perror ("Setting up max file size failed");
}

long long bassa_file_get_size (char *file_name)
{
  struct stat stat_buff;
  if (!stat (file_name, &stat_buff))
    return stat_buff.st_size;
  else
    return -1;
}
