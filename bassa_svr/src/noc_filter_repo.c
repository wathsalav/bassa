/***************************************************************************
 *   Copyright (C) 2007 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#include "noc_filter_repo.h"
 
bassa_repo* bassa_repo_new(char *repository)
{
  bassa_repo *rp = (bassa_repo*)malloc(sizeof(bassa_repo));
  rp->repo = repository;
  if((rp->dp = opendir(repository)))
    return rp;
  else
    return NULL;
}

int bassa_repo_search(bassa_repo *repository, char *fname)
{
  if(!fname)
    return -1;

  struct dirent *dr;
  if(repository)
    {
      while((dr=readdir(repository->dp)))
        {
#ifdef DEBUG
	  printf ("dr->d_name: %s\nfname: %s\n", dr->d_name, fname);
#endif //DEBUG
	  if(!strcmp(dr->d_name, fname))
            {
              rewinddir (repository->dp);
              return 1;
            }
        }
      rewinddir (repository->dp);
    }
  return 0;
}

void bassa_repo_delete(bassa_repo *repository)
{
  closedir(repository->dp);
  free(repository);
}
