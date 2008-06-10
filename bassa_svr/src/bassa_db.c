/*
 * All database access mechanisms are defined here. Go through these functions
 * when accessing database.
 *
 * Developed by:
 * Wathsala Vithanage: wvi@ucsc.cmb.ac.lk
 */
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
  bobj->last_modi_date = 0;
  return bobj;
}

bassa_irequest* bassa_irequest_new1(bassa_uri *uri, 
                                   unsigned long int content_len)
{
	if (!uri)
		return NULL;
  bassa_irequest *bir = (bassa_irequest*)malloc(sizeof(bassa_irequest));
  bir->buri = uri;
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
      printf (">>PATH: %s\n", bir->bobj->object_path);
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
      printf (">>URL: %s\n", bir->bobj->object_url);
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

int bassa_db_init()
{
  db_conf = conf;
  dbi_result *dbres = NULL;
  dbi_initialize (NULL);
  if (!db_conf)
    return BAD_CONF;
  if (!db_conf->dbicfg)
    return BAD_CONF;
  conn = dbi_conn_new (db_conf->dbicfg->dbms);
#ifdef DEBUG
  printf ("DBMS: %s\n", db_conf->dbicfg->dbms);
  printf ("DB Name: %s\n", db_conf->dbicfg->db_name);
  printf ("DB Host: %s\n", db_conf->dbicfg->db_host);
  printf ("DB Port: %i\n", db_conf->dbicfg->db_port);
  printf ("DB User: %s\n", db_conf->dbicfg->db_user);
  printf ("DB Password: %s\n", db_conf->dbicfg->db_passwd);
#endif //DEBUG
  dbi_conn_set_option (conn, "host", db_conf->dbicfg->db_host);
  dbi_conn_set_option_numeric (conn, "port", db_conf->dbicfg->db_port);
  dbi_conn_set_option (conn, "username", db_conf->dbicfg->db_user);
  dbi_conn_set_option (conn, "password", db_conf->dbicfg->db_passwd);
  dbi_conn_set_option (conn, "encoding", "UTF-8");
  int conn_stat = dbi_conn_connect (conn);
  if (conn_stat)
    return conn_stat;
  char *sql_query = "CREATE DATABASE IF NOT EXISTS %s";
  dbres = dbi_conn_queryf(conn, sql_query, db_conf->dbicfg->db_name);
  if (!dbres)
    return DB_CREATION_ERROR;
  else
    {
      dbi_result_free(dbres);
      dbi_conn_select_db(conn, db_conf->dbicfg->db_name);
    }
  sql_query = "CREATE TABLE IF NOT EXISTS cache_index(origin_url VARCHAR(1000) PRIMARY KEY, file_name VARCHAR(500), object_url VARCHAR(1000), object_path VARCHAR(1000), status VARCHAR(1), date_time TIMESTAMP, content_length  BIGINT UNSIGNED, hits BIGINT UNSIGNED)";
  dbres = dbi_conn_queryf(conn, sql_query);
  if (!dbres)
    return DB_CREATION_ERROR;
  else
    dbi_result_free(dbres);
  return SUCCESS;
}

int bassa_db_reinit()
{
  if (!db_conf)
    return BAD_CONF;
  if (!db_conf->dbicfg)
    return BAD_CONF;
  bassa_mutex *bm = bassa_mutex_new ();
  bassa_mutex_lock (bm);
  if (dbi_conn_ping (conn) == 0)
    {
      int conn_stat = dbi_conn_connect (conn);
      dbi_conn_select_db (conn, db_conf->dbicfg->db_name);
      if (conn_stat)
      	{
      		bassa_mutex_unlock (bm);
      		bassa_mutex_free (bm);
					return conn_stat;
      	}
    }
  bassa_mutex_unlock (bm);
  bassa_mutex_free (bm);
  return SUCCESS;
}

int bassa_db_shutdown()
{
  dbi_conn_close(conn);
  return 0;
}

int bassa_db_queue(bassa_irequest *irq)
{
  if (!irq)
    return -1;
  if (bassa_db_reinit())
    return -1;
  char *sql_query = NULL;
  dbi_result *dbres = NULL;
  sql_query = "INSERT INTO cache_index(origin_url, file_name, object_url, object_path, status, content_length, hits) VALUES('%s', '%s', '%s', '%s','%s', %i, %i)";
  dbres = dbi_conn_queryf(conn, sql_query, irq->buri->uri, 
                          irq->buri->file_name, irq->bobj->object_url, 
                          irq->bobj->object_path, irq->bobj->status, 
                          irq->bobj->content_length, 0);
  if(!dbres)
    return -1;
  else
    dbi_result_free(dbres);
  return SUCCESS;
}

int bassa_db_update_cache(bassa_irequest *irq)
{
  if (!irq)
    return -1;
	if (bassa_db_reinit())
    return -1;
  char *sql_query = NULL;
  dbi_result *dbres = NULL;
  sql_query = "UPDATE cache_index SET object_url='%s', object_path='%s', status='%s', content_length=%i WHERE origin_url='%s'";
  dbres = dbi_conn_queryf(conn, sql_query, irq->bobj->object_url, 
  												irq->bobj->object_path, irq->bobj->status, 
  												irq->bobj->content_length, irq->bobj->origin_url);
  if(!dbres)
    return -1;
  else
    dbi_result_free(dbres);
  return SUCCESS;
}

bassa_irequest* bassa_db_getpending()
{
	if (bassa_db_reinit())
    return NULL;
  char *sql_query = NULL;
  const char *temp_str = NULL;
  dbi_result *dbres = NULL;
  bassa_object *bobj = NULL;
  sql_query = "SELECT * FROM cache_index WHERE cache_index.status=\"P\" LIMIT 1";
  dbres = dbi_conn_query (conn, sql_query);
  if (!dbres)
		{
			return NULL;
		}
	else
		{
			if (dbi_result_next_row (dbres))
				{
					bobj = bassa_object_new (dbi_result_get_ulong(dbres, "content_length"));
      		temp_str = dbi_result_get_string (dbres, "origin_url");
      		if (temp_str)
        		bobj->origin_url = strdup (temp_str);
      		else
        		bobj->origin_url = NULL;
      		temp_str = dbi_result_get_string (dbres, "status");
      		if (temp_str)
        		bobj->status = strdup (temp_str);
      		else
        		bobj->status = NULL;
      		temp_str = dbi_result_get_string (dbres, "object_url");
      		if (temp_str)
        		bobj->object_url = strdup (temp_str);
      		else
        		bobj->object_url = NULL;
      		temp_str = dbi_result_get_string (dbres, "object_path");
      		if (temp_str)
        		bobj->object_path = strdup (temp_str);
      		else
        		bobj->object_path = NULL;
      		temp_str = dbi_result_get_string (dbres, "file_name");
      		if (temp_str)
        		bobj->file_name = strdup (temp_str);
      		else
        		bobj->file_name = NULL;
      		bobj->hits = dbi_result_get_ulong (dbres, "hits");
      		dbi_result_free (dbres);
      		bassa_irequest* bir = bassa_irequest_new2 (bobj);
      		return bir;
				}
			dbi_result_free (dbres);
			return NULL;
		}
}

int bassa_update_hits(char *origin_url)
{
  if (!origin_url)
    return -1;
	if (bassa_db_reinit())
    return -1;
  char *sql_query = NULL;
  dbi_result *dbres = NULL;
  sql_query = "UPDATE cache_index SET hits=hits+1 WHERE  WHERE origin_url='%s'";
  dbres = dbi_conn_queryf(conn, sql_query, origin_url);
  if(!dbres)
    return -1;
  else
    dbi_result_free(dbres);
  return SUCCESS;
}
	
int bassa_delete_file(int file_id)
{
  return 0;
}

bassa_object_set *bassa_list_all(int result_start)
{
  return 0;
}

bassa_object_set *bassa_search_file(char *file_name, 
                                    int result_start, int sort_type)
{
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
	if (bassa_db_reinit())
    return NULL;
  dbres = dbi_conn_queryf(conn, sql_query, file_name, file_name, 
                          st, RESULT_SET_SIZE, result_start);
  if (!dbres)
    return NULL;
  bassa_object *bobj = NULL;
  int count = 0;
  bassa_object_set *bobjs = (bassa_object_set*)malloc(sizeof(bassa_object_set));
  bobjs->total = dbi_result_get_numrows (dbres);
  bobjs->offset = result_start;
  const char *temp_str = NULL;
  while (dbi_result_next_row (dbres))
    {
      bobj = bassa_object_new (dbi_result_get_ulong(dbres, "content_length"));
      temp_str = dbi_result_get_string (dbres, "origin_url");
      if (temp_str)
        bobj->origin_url = strdup (temp_str);
      else
        bobj->origin_url = NULL;
      temp_str = dbi_result_get_string (dbres, "status");
      if (temp_str)
        bobj->status = strdup (temp_str);
      else
        bobj->status = NULL;
      temp_str = dbi_result_get_string (dbres, "object_url");
      if (temp_str)
        bobj->object_url = strdup (temp_str);
      else
        bobj->object_url = NULL;
      temp_str = dbi_result_get_string (dbres, "object_path");
      if (temp_str)
        bobj->object_path = strdup (temp_str);
      else
        bobj->object_path = NULL;
      temp_str = dbi_result_get_string (dbres, "file_name");
      if (temp_str)
        bobj->file_name = strdup (temp_str);
      else
        bobj->file_name = NULL;
      bobj->hits = dbi_result_get_ulong (dbres, "hits");
      bobjs->bobj[count] = bobj;
      count++;
    }
  dbi_result_free (dbres);
  return bobjs;
}

char *bassa_db_get_file_status(int id)
{
  return 0;
}


