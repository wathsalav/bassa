#ifndef NOC_FILTER_REPO_H
#define NOC_FILTER_REPO_H

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

typedef struct
{
  DIR *dp;
  char *repo;
} bassa_repo;

bassa_repo* bassa_repo_new(char *repository);

void bassa_repo_delete(bassa_repo *repository);

int bassa_repo_search(bassa_repo *repository, char *fname);

#endif
