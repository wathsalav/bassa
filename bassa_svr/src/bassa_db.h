/*
 * All database access mechanisms are defined here. Go through these functions
 * when accessing database.
 *
 * Developed by:
 * Wathsala Vithanage
 */

#include <stdio.h>
#include <string.h>
#include <dbi/dbi.h>

#include <noc_filter_configure.h> 
#include <bassa_uri.h>

#define SUCCESS 0
#define BAD_CONF -1
#define DB_CREATION_ERROR -2

#define RESULT_SET_SIZE 10

#define PENDING "P"
#define COMPLETED "C"
#define FAILED "F"

#define ASEND 0
#define DESND 1

dbi_conn conn;
bassa_conf *db_conf;
extern bassa_conf *conf;

typedef struct
{
  unsigned long int content_length;
  char *origin_url;
  char *status;
  char *object_url;
  char *object_path;
  char *file_name;
  unsigned long int hits;
  long int date;
  long int last_modi_date;
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
  unsigned int total;
  unsigned int offset;
}bassa_object_set;

bassa_object* bassa_object_new(unsigned long int content_len);

bassa_irequest* bassa_irequest_new(bassa_uri *uri, 
                                   unsigned long int content_len);

void bassa_irequest_free(bassa_irequest *bir);

void bassa_object_free(bassa_object *bobj);

void bassa_object_set_free(bassa_object_set *bobjs);

int bassa_db_init();

int bassa_db_reinit();

int bassa_db_shutdown();

int bassa_db_queue(bassa_irequest *irq);

int bassa_db_update_status(bassa_irequest *irq);

int bassa_update_hits(char *origin_url);
	
int bassa_delete_file(int file_id);

bassa_object_set *bassa_list_all(int result_start);

bassa_object_set *bassa_search_file(char *file_name, int result_start, int sort_type);

char *bassa_db_get_file_status(int id);

int bassa_db_set_local_url(int id, char *local_url);

