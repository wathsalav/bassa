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

