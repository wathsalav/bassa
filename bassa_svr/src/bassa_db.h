/*
 * All database access mechanisms are defined here. Go through these functions
 * when accessing database.
 *
 * Developed by:
 * Wathsala Vithanage
 */

#ifndef BASSA_DB_H
#define BASSA_DB_H

#include <stdio.h>
#include <string.h>
#include <dbi/dbi.h>

#include <noc_filter_configure.h> 
#include <bassa_uri.h>

#define SUCCESS 0
#define BAD_CONF -1
#define DB_CREATION_ERROR -2
#define BAD_DBD -3

#define RESULT_SET_SIZE 5

#define PROCESSING "P"
#define PENDING "Q"
#define COMPLETED "C"
#define FAILED "F"

#define ASEND 0
#define DESND 1

extern bassa_conf *conf;

typedef struct
{
  dbi_conn conn;
}bassa_db;

typedef struct
{
  unsigned long int content_length;
  char *origin_url;
  char *status;
  char *object_url;
  char *object_path;
  char *file_name;
  unsigned long int hits;
  unsigned int proto_bf;
  time_t start_time;
  time_t end_time;
  char *uuid;
}bassa_object;

typedef struct
{
  bassa_uri *buri;
  bassa_object *bobj;
  char *ip;
}bassa_irequest;

typedef struct
{
  bassa_object* bobj[RESULT_SET_SIZE];
  unsigned int object_limit;
  unsigned int total;
  unsigned int offset;
}bassa_object_set;

bassa_object* bassa_object_new(unsigned long int content_len);

bassa_irequest* bassa_irequest_new1(bassa_uri *uri, 
                                   unsigned long int content_len);
                                   
bassa_irequest* bassa_irequest_new2(bassa_object *bobj);

void bassa_irequest_free(bassa_irequest *bir);

void bassa_object_free(bassa_object *bobj);

void bassa_object_set_free(bassa_object_set *bobjs);

bassa_db* bassa_db_init();

int bassa_db_reinit(bassa_db *dbd);

int bassa_db_shutdown(bassa_db *dbd);

int bassa_db_queue(bassa_db *dbd, bassa_irequest *irq);

int bassa_db_update_cache(bassa_db *dbd, bassa_irequest *irq);

bassa_irequest* bassa_db_getpending(bassa_db *dbd);

int bassa_db_try_getpending(bassa_db *dbd);

int bassa_update_hits(bassa_db *dbd, char *origin_url);

int bassa_update_status(bassa_db *dbd, char *origin_url, char *status);
	
int bassa_delete_file(bassa_db *dbd, int file_id);

bassa_object_set *bassa_list_all(bassa_db *dbd, int offset, int sort_type);

bassa_object_set *bassa_list_latest(bassa_db *dbd, int offset);

bassa_object_set *bassa_search_file(bassa_db *dbd, char *file_name, int offset, int sort_type);

char *bassa_db_get_file_status(bassa_db *dbd, int id);

int bassa_invert_status(bassa_db *dbd, char *current, char *next);

#endif //BASSA_DB_H
