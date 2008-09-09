/***************************************************************************
 *   Copyright (C) 2007 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#ifndef NOC_FILTER_CONFIGURE_H
#define NOC_FILTER_CONFIGURE_H

#include <config.h>
#include <noc_filter_util.h>

//prefix of the http protocol
#define HTTP_PROTO_PREFIX "http://"

//Indicates the currently selected section of the configuration.
#define SERVER_CONF_MODULE 0
#define DOWNLOADER_CONF_MODULE 1
#define REPOSITORY_CONF_MODULE 2
#define MODULE_CONF_MODULE 3
#define DBI_CONF_MODULE 4

//This is the server section..
#define BASSA_CONF_ID 0
#define BASSA_CONF_TAG "bassa_conf"
#define SERVER_ID 1
#define SERVER_TAG "server"
#define SERVER_NAME_ID 2
#define SERVER_NAME_TAG "server_name"
#define SERVER_ADDRESS_ID 3
#define SERVER_ADDRESS_TAG "server_address"
#define SERVER_PORT_ID 4
#define SERVER_PORT_TAG "server_port"
#define SERVER_CHILDREN_ID 5
#define SERVER_URL_TAG "server_url"
#define SERVER_URL_ID 30
#define SERVER_CHILDREN_TAG "server_children"
#define MAX_CHILDREN_ID 6
#define MAX_CHILDREN_TAG "max_children"
#define CHILD_TIMEOUT_ID 7
#define CHILD_TIMEOUT_TAG "child_timeout"
#define SERVER_HTTP_PROXY_ADDRESS_ID 8
#define SERVER_HTTP_PROXY_ADDRESS_TAG "server_http_proxy_address"
#define SERVER_HTTP_PROXY_PORT_ID 9
#define SERVER_HTTP_PROXY_PORT_TAG "server_http_proxy_port"
#define SERVER_BACKLOG_ID 10
#define SERVER_BACKLOG_TAG "server_backlog"
#define SERVER_EVENT_BUS_ID 39
#define SERVER_EVENT_BUS_TAG "server_event_bus"
#define SERVER_UID_ID 40
#define SERVER_UID_TAG "server_uid"
#define SERVER_GID_ID 41
#define SERVER_GID_TAG "server_gid"
#define SERVER_ERROR_LOG_ID 42
#define SERVER_ERROR_LOG_TAG "server_error_log"
#define SERVER_ACCESS_LOG_ID 43
#define SERVER_ACCESS_LOG_TAG "server_access_log"

//This is the downloader section..
#define DOWNLOADER_ID 11
#define DOWNLOADER_TAG "downloader"
#define DOWNLOADER_ACTIVATION_TIME_ID 12
#define DOWNLOADER_ACTIVATION_TIME_TAG "downloader_activation_time"
#define DOWNLOADER_DEACTIVATION_TIME_ID 13
#define DOWNLOADER_DEACTIVATION_TIME_TAG "download_deactivation_time"
#define HOURS_ID 14
#define HOURS_TAG "hours"
#define MINUTES_ID 15
#define MINUTES_TAG "minutes"
#define SECONDS_ID 16
#define SECONDS_TAG "seconds"
#define DOWNLOADER_CHILDREN_ID 17
#define DOWNLOADER_CHILDREN_TAG "downloader_children"
#define MONITOR_PROGRESS_ID 18
#define MONITOR_PROGRESS_TAG "monitor_progress"
#define MAX_TRIES_ID 19
#define MAX_TRIES_TAG "max_tries"
#define DOWNLOADER_QUEUE_LENGTH_ID 20
#define DOWNLOADER_QUEUE_LENGTH_TAG "downloader_queue_length"
#define CONNECT_TIMEOUT_ID 21
#define CONNECT_TIMEOUT_TAG "connect_timeout"
#define DOWNLOADER_RETRIES_ID 22
#define DOWNLOADER_RETRIES_TAG "max_tries"

//This is the repository section
#define REPOSITORY_ID 23
#define REPOSITORY_TAG "repository"
#define REPOSITORY_PATH_ID 24
#define REPOSITORY_PATH_TAG "repository_path"
#define REPOSITORY_URL_ID 38
#define REPOSITORY_URL_TAG "url"

//This is the modules section
#define MODULES_ID 25
#define MODULES_TAG "modules"
#define MODULE_ID 26
#define MODULE_TAG "module"
#define MODULE_NAME_ID 27
#define MODULE_NAME_TAG "name"
#define MODULE_PATH_ID 28
#define MODULE_PATH_TAG "path"
#define MODULE_CONF_ID 29
#define MODULE_CONF_TAG "modconf"

//This is the dbi section
#define DBI_ID 31
#define DBI_TAG "db"
#define DBI_DBMS_ID 32
#define DBI_DBMS_TAG "db_dbms"
#define DBI_DBNAME_ID 33
#define DBI_DBNAME_TAG "db_name"
#define DBI_DBHOST_ID 34
#define DBI_DBHOST_TAG "db_host"
#define DBI_DBPORT_ID 35
#define DBI_DBPORT_TAG "db_port"
#define DBI_DBUSER_ID 36
#define DBI_DBUSER_TAG "db_user"
#define DBI_DBPASSWD_ID 37
#define DBI_DBPASSWD_TAG "db_passwd"

#define XML_READER_BUFFER_SIZE 512

/*
 * Contains all the configuration information read from server section..
 */
typedef struct
{
  char *server_name;         /*Server  name*/
  char *server_address;      /*Server address*/
  int server_port;           /*Server port*/
  int server_backlog;        /*Server backlog*/
  char *server_url;
  int max_children;          /*Maximum clients at a time*/
  int child_timeout;         /*Client timeout*/
  char *http_proxy_address;  /*HTTP proxy address*/
  int http_proxy_port;       /*HTTP proxy port*/
  char *http_proxy;
  char *server_event_bus;    /*FIFO used for server event handling*/
  char *server_error_log;    /*Path to error log*/
  char *server_access_log;   /*Path to access log*/
  char *server_uid;          /*Process UID*/
  char *server_gid;          /*Process GID*/
}bassa_server_conf;

typedef struct
{
  int hours;            /*Number of hours*/
  int minutes;          /*Number of minutes*/
  int seconds;          /*Number of seconds*/
  int dhours;
  int dminutes;
  int dseconds;
  int max_children;     /*Maximum downloaders*/
  int max_tries;        /*Maximum number of tries*/
  int connect_timeout;  /*Download timeout*/
  int max_queue_len;    /*Maximum length of the queue*/
}bassa_downloader_conf;

typedef struct
{
  char *repo_path;      /*Path to the local repository*/
  char *url;
}bassa_repository_conf;

typedef struct
{
  char *path;
  char *name;
  char *modconf;
}bassa_module_conf;

typedef struct
{
  bassa_list *modconf_list;
  bassa_module_conf *out_list;
}bassa_conf_collection;

typedef struct
{
  char *dbms;
  char *db_name;
  char *db_host;
  int  db_port;
  char *db_user;
  char *db_passwd;
}bassa_dbi_conf;

typedef struct
{
  int current_tag;                /*The currentXML tag read by expat*/
  int current_config_module;      /*The current section we are in, server|repository etc..*/
  int current_level;
  bassa_server_conf *svrcfg;      /*Pointer to the server configuration object*/
  bassa_downloader_conf *dlcfg;   /*Pointer to the downloader configuration object*/
  bassa_repository_conf *repocfg; /*Pointer to the repository configuration object*/
  bassa_conf_collection *cfgcol;  /*Pointer to the authentication configuration object*/
  bassa_dbi_conf *dbicfg;         /*Pointer to DBI configuration object*/ 
}bassa_conf;


/**
 * Create a new bassa configuration object
 * @return The newly created bassa configuration object.
 */
bassa_conf*
bassa_conf_new();

/**
 * Delete a bassa configuration object
 * @param conf Bassa configuration object
 */
void
bassa_conf_delete(bassa_conf *conf);

/**
 * Create a new server configuration object
 * @return The newly created server configuration object.
 */
bassa_server_conf*
bassa_server_conf_new();

/**
 * Delete a server configuration object
 * @param Server configuration object to be deleted.
 */
void
bassa_server_conf_delete(bassa_server_conf *conf);

/**
 * Create a new downloader configuration object
 * @return The newly created downloader configuration object.
 */
bassa_downloader_conf*
bassa_downloader_conf_new();

/**
 * Delete a downloader configuration object
 * @param The downloader object to be deleted.
 */
void
bassa_downloader_conf_delete(bassa_downloader_conf *conf);

/**
 * Create a new repository configuration object
 * @return The newly created repository configuration object.
 */
bassa_repository_conf*
bassa_repository_conf_new();

/**
 * Delete a downloader configuration object
 * @param The repository object to be deleted.
 */
void
bassa_repository_conf_delete(bassa_repository_conf *conf);

/**
 * Create a new authentication configuration object
 * @return The newly created authentication configuration object.
 */
bassa_conf_collection*
bassa_conf_collection_new();

/**
 * Creat a new dbi configuration object
 * @return The Newly created dbi configuration object
 */
bassa_dbi_conf*
bassa_dbi_conf_new();

/**
 * Delete a dbi configuration object
 * @param The dbi object to be deleted.
 */
void
bassa_dbi_conf_delete (bassa_dbi_conf *conf);

/**
 * Delete an authentication configuration object
 * @param conf The authentication object to be deleted.
 */
void
bassa_conf_collection_delete(bassa_conf_collection *conf);

void
bassa_start_tag_handler(void*, char*, char**);

void
bassa_stop_tag_handler(void*, char*);

void
bassa_conf_cdata_handler (void*, char*, int);

char*
bassa_assemble_configuration (char*, char*, int);

void
bassa_setup_server_configuration (bassa_conf*, char*, int);

void
bassa_setup_downloader_configuration (bassa_conf*, char*, int);

void
bassa_setup_repository_configuration (bassa_conf*, char*, int);

void
bassa_setup_module_configuration (bassa_conf*, char*, int);

void
bassa_setup_dbi_configuration (bassa_conf*, char*, int);

bassa_conf*
bassa_parse_configuration (char*);
#endif //NOC_FILTER_CONFIGURE_H

