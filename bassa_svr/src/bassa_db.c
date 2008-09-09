/*
 * All database access mechanisms are defined here. Go through these functions
 * when accessing database.
 *
 * Developed by:
 * Wathsala Vithanage: wvi@ucsc.cmb.ac.lk
 */
#include <unistd.h>

#include <bassa_db.h>
#include <noc_filter_concur.h>

bassa_object* bassa_object_new(unsigned long int content_len)
{
  bassa_object *bobj = (bassa_object*)malloc(sizeof(bassa_object));
  bobj->content_length = content_len;
  bobj->status = PENDING;
  bobj->object_url = NULL;
  bobj->object_path = NULL;
  bobj->file_name = NULL;
  bobj->origin_url = NULL;
  bobj->hits = 0;
  bobj->end_time = 0;
  bobj->uuid = NULL;
  return bobj;
}

bassa_irequest* bassa_irequest_new1(bassa_uri *uri, 
				    unsigned long int content_len)
{
  if (!uri)
    return NULL;
  bassa_irequest *bir = (bassa_irequest*)malloc(sizeof(bassa_irequest));
  bir->buri = uri;
  printf ("FILE_NAME: %s\n", uri->file_name);
  bir->bobj = bassa_object_new (content_len);
  (bir->buri->file_name != NULL)?
    (bir->bobj->file_name = strdup(bir->buri->file_name)):
    (bir->bobj->file_name = NULL);
  if (bir->buri->file_name != NULL)
    {
      int len = strlen(conf->repocfg->repo_path)
	+ strlen(bir->buri->file_name) + 1;
      bir->bobj->object_path = (char*)malloc(len);
      memset (bir->bobj->object_path, (int)'\0', len);
      strcpy (bir->bobj->object_path, conf->repocfg->repo_path);
      strcat (bir->bobj->object_path, bir->buri->file_name);
    }
  else
    bir->bobj->object_path = NULL;
  if (bir->buri->file_name != NULL)
    {
      int len = strlen(conf->repocfg->url)
	+ strlen(bir->buri->file_name) + 1;
      bir->bobj->object_url = (char*)malloc(len);
      memset (bir->bobj->object_url, (int)'\0', len);
      strcpy (bir->bobj->object_url, conf->repocfg->url);
      strcat (bir->bobj->object_url, bir->buri->file_name);
    }
  else
    bir->bobj->object_url = NULL;
  (bir->buri->uri != NULL)?
    (bir->bobj->origin_url = strdup(bir->buri->uri)):
    (bir->bobj->origin_url = NULL);
  return bir;
}

bassa_irequest* bassa_irequest_new2(bassa_object *bobj)
{
  bassa_irequest *bir = (bassa_irequest*)malloc(sizeof(bassa_irequest));
  bir->bobj = bobj;
  bir->ip = NULL;
  bir->buri = NULL;
  return bir;
}

void bassa_object_free(bassa_object *bobj)
{
  if (!bobj)
    return;
  if (bobj->object_path)
  {
    free(bobj->object_path);
    bobj->object_path = NULL;
  }
  if (bobj->object_url)
  {
    free(bobj->object_url);
    bobj->object_url = NULL;
  }
  if (bobj->file_name)
  {
    free(bobj->file_name);
    bobj->file_name = NULL;
  }
  if (bobj->origin_url)
  {
    free(bobj->origin_url);
    bobj->origin_url = NULL;
  }
  free(bobj);
  bobj = NULL;
}

void bassa_object_set_free(bassa_object_set *bobjs)
{
  if (!bobjs)
    return;
  int i;
  for (i=0; i<bobjs->total; i++)
    {
      bassa_object_free (bobjs->bobj[i]);
    }
  free (bobjs);
  bobjs = NULL;
}

void bassa_irequest_free (bassa_irequest *bir)
{
  if (!bir)
    return;
  if (bir->bobj)
    {
      bassa_object_free(bir->bobj);
      bir->bobj = NULL;
    }
  free(bir);
  bir = NULL;
}

bassa_db* bassa_db_init()
{
  const char *dberr = NULL;
  bassa_db *dbd = (bassa_db*)malloc(sizeof(bassa_db));
  bassa_conf *db_conf = conf;
  dbi_result *dbres = NULL;
  dbi_initialize (NULL);
  if (!db_conf)
    return NULL;
  if (!db_conf->dbicfg)
    return NULL;
  dbd->conn = dbi_conn_new (db_conf->dbicfg->dbms);
#ifdef DEBUG
  printf ("DBMS: %s\n", db_conf->dbicfg->dbms);
  printf ("DB Name: %s\n", db_conf->dbicfg->db_name);
  printf ("DB Host: %s\n", db_conf->dbicfg->db_host);
  printf ("DB Port: %i\n", db_conf->dbicfg->db_port);
  printf ("DB User: %s\n", db_conf->dbicfg->db_user);
  printf ("DB Password: %s\n", db_conf->dbicfg->db_passwd);
#endif //DEBUG
  dbi_conn_set_option (dbd->conn, "host", db_conf->dbicfg->db_host);
  dbi_conn_set_option_numeric (dbd->conn, "port", db_conf->dbicfg->db_port);
  dbi_conn_set_option (dbd->conn, "username", db_conf->dbicfg->db_user);
  dbi_conn_set_option (dbd->conn, "password", db_conf->dbicfg->db_passwd);
  dbi_conn_set_option (dbd->conn, "encoding", "UTF-8");
  int conn_stat = dbi_conn_connect (dbd->conn);
  if (conn_stat)
    {
      dbi_conn_error(dbd->conn, &dberr);
      printf ("Error: %s\n", dberr);
      return NULL;
    }
  char *sql_query = "CREATE DATABASE IF NOT EXISTS %s";
  dbres = dbi_conn_queryf(dbd->conn, sql_query, db_conf->dbicfg->db_name);
  if (!dbres)
    {
      dbi_conn_error(dbd->conn, &dberr);
      printf ("Error: %s\n", dberr);
      return NULL;
    }
  else
    {
      dbi_result_free(dbres);
      dbi_conn_select_db(dbd->conn, db_conf->dbicfg->db_name);
    }
  dbi_conn_set_option (dbd->conn, "dbname", db_conf->dbicfg->db_name);
  sql_query = "CREATE TABLE IF NOT EXISTS cache_index(id BIGINT AUTO_INCREMENT, PRIMARY KEY(id), origin_url VARCHAR(1000) UNIQUE NOT NULL, file_name VARCHAR(512), object_url VARCHAR(1024), object_path VARCHAR(1024), status VARCHAR(1), date_time TIMESTAMP, content_length  BIGINT, hits BIGINT, proto_bf INT, client_uuid VARCHAR(256) NOT NULL)";
  dbres = dbi_conn_queryf(dbd->conn, sql_query);
  if (!dbres)
    {
      dbi_conn_error(dbd->conn, &dberr);
      printf ("Error: %s\n", dberr);
      return NULL;
    }
  else
    dbi_result_free(dbres);
  return dbd;
}

int bassa_db_reinit(bassa_db *dbd)
{ 
  bassa_conf *db_conf = conf;
  if (!dbd)
    return BAD_DBD;
  if (!db_conf)
    return BAD_CONF;
  if (!db_conf->dbicfg)
    return BAD_CONF;
  if (dbi_conn_ping (dbd->conn) == 0)
    {
      int conn_stat = dbi_conn_connect (dbd->conn);
      dbi_conn_select_db (dbd->conn, db_conf->dbicfg->db_name);
      if (conn_stat)
	{
	  return conn_stat;
	}
    }
  return SUCCESS;
}

int bassa_db_shutdown(bassa_db *dbd)
{
  if (!dbd)
    return BAD_DBD;
  dbi_conn_close(dbd->conn);
  free (dbd);
  dbd = NULL;
  return 0;
}

int bassa_db_queue(bassa_db *dbd, bassa_irequest *irq)
{
  if (!dbd)
    return BAD_DBD;
  if (!irq)
    return -1;
  if (bassa_db_reinit(dbd))
    return -1;
  char *sql_query = NULL;
  dbi_result *dbres = NULL;
  sql_query = "INSERT INTO cache_index(origin_url, file_name, object_url, object_path, status, content_length, hits, proto_bf, client_uuid) VALUES('%s', '%s', '%s', '%s','%s', %i, %i, %i, '%s')"; 
  dbres = dbi_conn_queryf(dbd->conn, sql_query, irq->buri->uri, 
      irq->buri->file_name, irq->bobj->object_url, 
      irq->bobj->object_path, irq->bobj->status, 
      irq->bobj->content_length, 0, 0, irq->bobj->uuid);
  if(!dbres)
    return -1;
  else
    dbi_result_free(dbres);
  return SUCCESS;
}

int bassa_db_update_cache(bassa_db *dbd, bassa_irequest *irq)
{
  if (!dbd)
    return BAD_DBD;
  if (!irq)
    return -1;
  if (bassa_db_reinit(dbd))
    return -1;
  char *sql_query = NULL;
  dbi_result *dbres = NULL;
  sql_query = "UPDATE cache_index SET object_url='%s', object_path='%s', status='%s', content_length=%i, proto_bf=%i WHERE origin_url='%s'";
  dbres = dbi_conn_queryf(dbd->conn, sql_query, irq->bobj->object_url, 
			  irq->bobj->object_path, irq->bobj->status, 
			  irq->bobj->content_length, irq->bobj->proto_bf,
			  irq->bobj->origin_url);
  if(!dbres)
    return -1;
  else
    dbi_result_free(dbres);
  return SUCCESS;
}

bassa_irequest* bassa_db_getpending(bassa_db *dbd)
{
  const char *dberr = NULL;
  if(!dbd)
    return NULL;
  if (bassa_db_reinit(dbd))
    return NULL;
  char *sql_query = NULL;
  dbi_result *dbres = NULL;
  bassa_object *bobj = NULL;
  sql_query = "SELECT * FROM cache_index WHERE cache_index.status='Q' LIMIT 1";
  dbres = dbi_conn_query (dbd->conn, sql_query);
  if (!dbres)
  {
    dbi_conn_error(dbd->conn, &dberr);
    printf ("Error: %s\n", dberr);
    return NULL;
  }
  else
  {
    if (dbi_result_next_row (dbres))
    {
      bobj = bassa_object_new (dbi_result_get_ulonglong(dbres, "content_length"));
      bobj->origin_url = dbi_result_get_string_copy (dbres, "origin_url");
      bobj->status = dbi_result_get_string_copy (dbres, "status");
      bobj->object_url = dbi_result_get_string_copy (dbres, "object_url");
      bobj->object_path = dbi_result_get_string_copy (dbres, "object_path");
      bobj->file_name = dbi_result_get_string_copy (dbres, "file_name");

      bobj->proto_bf = dbi_result_get_int (dbres, "proto_bf");
      printf ("PBF: %u\n", bobj->proto_bf);
      
      printf ("DBI Address: %p\n", dbi_result_get_string(dbres, "file_name"));
      printf ("BObj Address: %p\n", bobj->file_name);
      bobj->hits = dbi_result_get_ulonglong (dbres, "hits");
      bassa_update_status(dbd, bobj->origin_url, PROCESSING);
      dbi_result_free (dbres);
      bassa_irequest* bir = bassa_irequest_new2 (bobj);
      return bir;
    }
    else
    {
      dbi_result_free (dbres);
      return NULL;
    }
  }
}

int bassa_db_try_getpending(bassa_db *dbd)
{
  const char *dberr = NULL;
  if(!dbd)
    return BAD_DBD;
  if (bassa_db_reinit(dbd))
    return 0;
  char *sql_query = NULL;
  dbi_result *dbres = NULL;
  sql_query = "SELECT * FROM cache_index WHERE cache_index.status='Q' LIMIT 1";
  dbres = dbi_conn_query (dbd->conn, sql_query);
  if (!dbres)
  {
    dbi_conn_error(dbd->conn, &dberr);
    printf ("Error: %s\n", dberr);
    return 0;
  }
  else
  {
    if (dbi_result_next_row (dbres))
    {
      printf ("New Elements\n");
      dbi_result_free (dbres);
      return 1;
    }
    else
    {
      return 0;
    }
  }
}

int bassa_update_hits(bassa_db *dbd, char *origin_url)
{
  if (!dbd)
    return BAD_DBD;
  if (!origin_url)
    return -1;
  if (bassa_db_reinit(dbd))
    return -1;
  char *sql_query = NULL;
  dbi_result *dbres = NULL;
  sql_query = "UPDATE cache_index SET hits=hits+1 WHERE origin_url='%s'";
  dbres = dbi_conn_queryf(dbd->conn, sql_query, origin_url);
  if(!dbres)
    return -1;
  else
    dbi_result_free(dbres);
  return SUCCESS;
}

int bassa_update_status(bassa_db *dbd, char *origin_url, char *status)
{
  if (!dbd)
    return BAD_DBD;
  if (!origin_url)
    return -1;
  if (bassa_db_reinit(dbd))
    return -1;
  char *sql_query = NULL;
  dbi_result *dbres = NULL;
  sql_query = "UPDATE cache_index SET status='%s' WHERE origin_url='%s'";
  dbres = dbi_conn_queryf(dbd->conn, sql_query, status, origin_url);
  if(!dbres)
    return -1;
  else
    dbi_result_free(dbres);
  return SUCCESS;
}
	
int bassa_delete_file(bassa_db *dbd, int file_id)
{
  if (!dbd)
    return BAD_DBD;
  return 0;
}

bassa_object_set *bassa_list_all(bassa_db *dbd, int offset, int sort_type)
{
  if (!dbd)
    return NULL;
  if (sort_type != 0)
    sort_type = 1;
  char *sql_query = NULL;
  dbi_result *dbres = NULL;
  char *st = NULL;
  if (sort_type > 0)
    st = "DESC";
  else
    st = "ASC";
  sql_query = "SELECT * FROM cache_index ORDER BY date_time %s LIMIT %i OFFSET %i";
#ifdef DEBUG
  printf (sql_query, st, RESULT_SET_SIZE, offset);
  printf ("\n");
#endif //DEBUG
  if (bassa_db_reinit(dbd))
    return NULL;
  dbres = dbi_conn_queryf(dbd->conn, sql_query,
                          st, RESULT_SET_SIZE, offset);
  if (!dbres)
    return NULL;
  bassa_object *bobj = NULL;
  int count = 0;
  bassa_object_set *bobjs = (bassa_object_set*)malloc(sizeof(bassa_object_set));
  bobjs->object_limit = RESULT_SET_SIZE;
  bobjs->total = dbi_result_get_numrows (dbres);
  bobjs->offset = offset;
  while (dbi_result_next_row (dbres))
    {
      bobj = bassa_object_new (dbi_result_get_ulonglong(dbres, "content_length"));
      bobj->origin_url = dbi_result_get_string_copy (dbres, "origin_url");
      bobj->status = dbi_result_get_string_copy (dbres, "status");
      bobj->object_url = dbi_result_get_string_copy (dbres, "object_url");
      bobj->object_path = dbi_result_get_string_copy (dbres, "object_path");
      bobj->file_name = dbi_result_get_string_copy (dbres, "file_name");
      bobj->start_time = dbi_result_get_datetime (dbres, "start_time");
      bobj->end_time = dbi_result_get_datetime (dbres, "end_time");
      bobj->hits = dbi_result_get_ulonglong (dbres, "hits");
      bobjs->bobj[count] = bobj;
      count++;
    }
  dbi_result_free (dbres);
  return bobjs;
}

bassa_object_set *bassa_search_file(bassa_db *dbd, char *file_name, 
                                    int offset, int sort_type)
{
  if (!dbd)
    return NULL;
  if (!file_name)
    return NULL;
  if (sort_type != 0)
    sort_type = 1;
  char *sql_query = NULL;
  dbi_result *dbres = NULL;
  char *st = NULL;
  if (sort_type > 0)
    st = "DESC";
  else
    st = "ASC";
  sql_query = "SELECT * FROM cache_index WHERE origin_url LIKE '%%%s%%' OR file_name LIKE '%%%s%%' ORDER BY date_time %s LIMIT %i OFFSET %i";
#ifdef DEBUG
  printf (sql_query, file_name, file_name, st, RESULT_SET_SIZE, offset);
  printf ("\n");
#endif //DEBUG
  if (bassa_db_reinit(dbd))
    return NULL;
  dbres = dbi_conn_queryf(dbd->conn, sql_query, file_name, file_name,
                          st, RESULT_SET_SIZE, offset);
  if (!dbres)
    return NULL;
  bassa_object *bobj = NULL;
  int count = 0;
  bassa_object_set *bobjs = (bassa_object_set*)malloc(sizeof(bassa_object_set));
  bobjs->object_limit = RESULT_SET_SIZE;
  bobjs->total = dbi_result_get_numrows (dbres);
  bobjs->offset = offset;
  while (dbi_result_next_row (dbres))
    {
      bobj = bassa_object_new (dbi_result_get_ulonglong(dbres, "content_length"));
      bobj->origin_url = dbi_result_get_string_copy (dbres, "origin_url");
      bobj->status = dbi_result_get_string_copy (dbres, "status");
      bobj->object_url = dbi_result_get_string_copy (dbres, "object_url");
      bobj->object_path = dbi_result_get_string_copy (dbres, "object_path");
      bobj->file_name = dbi_result_get_string_copy (dbres, "file_name");
      bobj->start_time = dbi_result_get_datetime (dbres, "start_time");
      bobj->end_time = dbi_result_get_datetime (dbres, "end_time");
      bobj->hits = dbi_result_get_ulonglong (dbres, "hits");
      bobjs->bobj[count] = bobj;
      count++;
    }
  dbi_result_free (dbres);
  return bobjs;
}

bassa_object_set *bassa_list_latest(bassa_db *dbd, int offset)
{
  if (!dbd)
    return NULL;
  char *sql_query = NULL;
  dbi_result *dbres = NULL;
  sql_query = "SELECT * FROM cache_index ORDER BY date_time DESC LIMIT %i OFFSET %i";
#ifdef DEBUG
  printf (sql_query, RESULT_SET_SIZE, offset);
  printf ("\n");
#endif //DEBUG
  if (bassa_db_reinit(dbd))
    return NULL;
  dbres = dbi_conn_queryf(dbd->conn, sql_query,
                          RESULT_SET_SIZE, offset);
  if (!dbres)
    return NULL;
  bassa_object *bobj = NULL;
  int count = 0;
  bassa_object_set *bobjs = (bassa_object_set*)malloc(sizeof(bassa_object_set));
  bobjs->object_limit = RESULT_SET_SIZE;
  bobjs->total = dbi_result_get_numrows (dbres);
  bobjs->offset = offset;
  while (dbi_result_next_row (dbres))
    {
      bobj = bassa_object_new (dbi_result_get_ulonglong(dbres, "content_length"));
      bobj->origin_url = dbi_result_get_string_copy (dbres, "origin_url");
      bobj->status = dbi_result_get_string_copy (dbres, "status");
      bobj->object_url = dbi_result_get_string_copy (dbres, "object_url");
      bobj->object_path = dbi_result_get_string_copy (dbres, "object_path");
      bobj->file_name = dbi_result_get_string_copy (dbres, "file_name");
      bobj->start_time = dbi_result_get_datetime (dbres, "start_time");
      bobj->end_time = dbi_result_get_datetime (dbres, "end_time");
      bobj->hits = dbi_result_get_ulonglong (dbres, "hits");
      bobjs->bobj[count] = bobj;
      count++;
    }
  dbi_result_free (dbres);
  return bobjs;
}

char *bassa_db_get_file_status(bassa_db *dbd, int id)
{
  if (!dbd)
    return NULL;
  return NULL;
}


int bassa_invert_status(bassa_db *dbd, char *current, char *next)
{
  if (!dbd)
    return BAD_DBD;
  if (!current || !next)
    return -1;
  if (bassa_db_reinit(dbd))
    return -1;
  char *sql_query = NULL;
  dbi_result *dbres = NULL;
  sql_query = "UPDATE cache_index SET status='%s' WHERE status='%s'";
  dbres = dbi_conn_queryf(dbd->conn, sql_query, next, current);
  if(!dbres)
    return -1;
  else
    dbi_result_free(dbres);
  return SUCCESS;
}
