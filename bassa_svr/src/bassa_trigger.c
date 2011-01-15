#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include "bassa_trigger.h"
#include "noc_filter_concur.h"

bassa_trigger* bassa_trigger_new (char *tn)
{
  bassa_trigger *bt = (bassa_trigger*)malloc(sizeof(
	bassa_trigger));
  bt->fifo_name = tn;
  int i = mkfifo(bt->fifo_name, 0600);
  if (i<0)
  {
    if (errno == EEXIST)
      goto fifo_open;
    perror("MKFIFO");
    free (bt);
    bt = NULL;
    return NULL;
  }

fifo_open:bt->fifo_fd = open(bt->fifo_name, O_RDWR, 0);

  if (bt->fifo_fd<0)
  {
    perror("OPEN");
    free(bt);
    bt = NULL;
    return NULL;
  }
  refcount++;
  return bt;
}

void bassa_trigger_free (bassa_trigger *btrig)
{
  if (btrig)
  {
    refcount--;
    if (!refcount)
      close(btrig->fifo_fd);
    free (btrig);
    btrig = NULL;
  }
}  

int bassa_trigger_sleep (bassa_trigger *t)
{
  int rt;
  int val;
  fd_set readfs;
  FD_SET(t->fifo_fd, &readfs);
SELECT:if ((rt=select(t->fifo_fd+1, &readfs, NULL, NULL, NULL)) < 0)
  {
    if (errno != EINTR)
    {
      perror("Select");
      if (errno == EBADF)
      {
        printf("Fifo FD: %i\n", t->fifo_fd);
        bassa_trigger_reinit(t);
        goto SELECT;
      }
      else
        return BASSA_EVENT_ERROR;
    }
    else
    {
      FD_SET(t->fifo_fd, &readfs);
      goto SELECT;
    }
  }
  else
  {
    usleep(500);
    if((rt=read(t->fifo_fd, &val, sizeof(int)))<0)
    {
      perror ("Read");
      return -1;
    }
    else
      return val;
  }
}

int bassa_trigger_wake (bassa_trigger *t, type_t type)
{
  int i;
  if ((i=write(t->fifo_fd, &type, sizeof(type_t))) < 0)
  {
    perror("FIFO Problem");
  }
  return i;
}

void bassa_trigger_reinit(bassa_trigger *bt)
{
  close(bt->fifo_fd);
  unlink(bt->fifo_name);
  int i = mkfifo(bt->fifo_name, 0600);
  if (!i)
    bt->fifo_fd = open(bt->fifo_name, O_RDWR, 0);
  else
  {
    printf("Exiting: Irrecoverable Error. Unable to reinitialize event fifo!\n");
    exit(-1);
  }
}

