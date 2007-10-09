/***************************************************************************
 *   Copyright (C) 2006 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "noc_filter_transaction.h"
#include "noc_filter_file.h"
#include "noc_filter_concur.h"

bassa_transaction *
bassa_transaction_new (char *url, char *local_path, long long size/*, char *http_proxy*/)
{
  if (url && local_path)
    {
      char *part_local_lpath = (char *) malloc (strlen (local_path) + strlen (DOWNLOAD_PART_EXT) + 1);
      strcpy (part_local_lpath, local_path);
      strcat (part_local_lpath, DOWNLOAD_PART_EXT);
      bassa_transaction *transaction = (bassa_transaction *) 
	malloc (sizeof (bassa_transaction));
      transaction->file_complete = local_path;
      transaction->file_part = part_local_lpath;
      transaction->url = url;
      transaction->path = bassa_get_location (transaction->url);
      transaction->size = size;
      transaction->http_proxy = sys_proxy;
      transaction->header_list = NULL;
      transaction->curl_handle = curl_easy_init ();
      transaction->http_bf = 0;
      transaction->disposition_type = HTTP_CONTENT_DISP_ATCHMNT_ID;
      transaction->disposition_fname = NULL;
      //Set the protocol before encoding URL
      if (strlen (HTTP_PROTO) < strlen (transaction->url))
	{
	  if (!strncmp (HTTP_PROTO, transaction->url, 
			strlen (HTTP_PROTO)))
	    {
	      SET_PROTO(transaction->http_bf, HTTP_PROTO_FLAG);
	    }
	}
      else if (strlen (HTTPS_PROTO) < strlen (transaction->url))
	{
	  if (!strncmp (HTTPS_PROTO, transaction->url, 
			strlen (HTTPS_PROTO)))
	    {
	      SET_PROTO(transaction->http_bf, HTTP_PROTO_FLAG);
	    }
	}
      else if (strlen (FTP_PROTO) < strlen (transaction->url))
	{
	  if (!strncmp (FTP_PROTO, transaction->url, 
			strlen (FTP_PROTO)))
	    {
	      SET_PROTO(transaction->http_bf, FTP_PROTO_FLAG);
	    }
	}
      else
	{
	  SET_PROTO(transaction->http_bf, UNKNOWN_PROTO);
	}
      bassa_transaction_encode_url(transaction);
      return transaction;
    }
  else
    {
      return NULL;
    }
}

int
bassa_transaction_download (bassa_transaction * transaction)
{
  if (!transaction)
    return -1;
#ifdef DEBUG
  printf ("The Transaction HANDLE: %p\n", transaction->curl_handle);
#endif //DEBUG
  int current_size = bassa_file_get_size (transaction->file_part);
#ifdef DEBUG
  printf ("SIZE OF THE FILE: %i\n", current_size);
#endif //DEBUG
  int status = 0; 
  int num_tries = 0;
  long int sleep_time = 0;
  for (num_tries = 0; num_tries < transaction->max_tries; num_tries++)
    {
      if ( current_size > 0 && GET_PROTO(transaction->http_bf) == HTTP_PROTO_FLAG)
	{
#ifdef DEBUG
	  printf ("Trying %i time(s)...\n", num_tries);
#endif //DEBUG
	  bassa_transaction_set_http_resume (transaction, current_size);
	}
      else if (GET_PROTO(transaction->http_bf) == HTTP_PROTO_FLAG)
	{
#ifdef DEBUG
	  printf ("Trying %i time(s)...\n", num_tries);
#endif //DEBUG
	  bassa_transaction_set_http_init (transaction);
	}
      status = curl_easy_perform (transaction->curl_handle);
#ifdef DEBUG
      printf ("CURLcode %i\n", status);
#endif //DEBUG
      if (status)
	{
	  usleep (sleep_time);
	  continue;
	}
      else
	break;
    }
  fclose (transaction->local_file);
  /*if (transaction->disposition_type == HTTP_CONTENT_DISP_ATCHMNT_ID)
    {
      rename (transaction->file_part, transaction->disposition_fname);
      }*/
  rename (transaction->file_part, transaction->file_complete);
  return status;
}

void
bassa_transaction_delete (bassa_transaction * transaction)
{
  if (transaction)
    {
      curl_easy_cleanup (transaction->curl_handle);
      if (transaction->file_complete)
	free (transaction->file_complete);
      if (transaction->file_part)
	free (transaction->file_part);
      if (transaction->header_list)
	curl_slist_free_all (transaction->header_list);
      if (transaction->url_encoded)
	curl_free (transaction->url_encoded);
      if (transaction->disposition_fname)
	free (transaction->disposition_fname);
      free (transaction);
    }
}

int 
bassa_transaction_open(bassa_transaction  *transaction)
{
  transaction->local_file = fopen (transaction->file_part, "w+");
  if (!transaction->local_file)
    {
      perror("FOPEN");
      return -1;
    }
  curl_easy_setopt (transaction->curl_handle, CURLOPT_WRITEDATA, transaction->local_file);
  return 0;
}

void
bassa_transaction_set_options (bassa_transaction *transaction,
			       int connect_timeout, int status)
{
#ifdef DEBUG
  printf ("URL-encoded: %s\n", transaction->url_encoded);
#endif //DEBUG
  curl_easy_setopt (transaction->curl_handle, CURLOPT_URL, 
		    transaction->url_encoded);
  curl_easy_setopt (transaction->curl_handle, CURLOPT_FOLLOWLOCATION,
		    1);
  curl_easy_setopt (transaction->curl_handle, CURLOPT_MAXREDIRS,
		    3);
  curl_easy_setopt (transaction->curl_handle, CURLOPT_NOSIGNAL,
		    1);
  curl_easy_setopt (transaction->curl_handle, CURLOPT_CONNECTTIMEOUT,
		    connect_timeout);
  curl_easy_setopt (transaction->curl_handle,
		    CURLOPT_HTTPHEADER,
		    transaction->header_list);
  curl_easy_setopt (transaction->curl_handle,
		    CURLOPT_VERBOSE,
		    1);
  curl_easy_setopt (transaction->curl_handle,
		    CURLOPT_DEBUGFUNCTION,
		    bassa_transaction_get_response);
  curl_easy_setopt (transaction->curl_handle,
		    CURLOPT_DEBUGDATA,
		    transaction);
  curl_easy_setopt (transaction->curl_handle,
		    CURLOPT_WRITEDATA, 
		    transaction->local_file);
  curl_easy_setopt (transaction->curl_handle,
		    CURLOPT_PROXY,
		    transaction->http_proxy);
  switch (status)
    {
    case CURLE_PARTIAL_FILE:
      curl_easy_setopt (transaction->curl_handle,
			CURLOPT_HTTP_VERSION,
			CURLOPT_IGNORE_CONTENT_LENGTH);
      break;
    default:
      break;
    }
}

int
bassa_transaction_get_response (CURL* handle, curl_infotype type, 
				char* data, size_t size, void* para)
{
  bassa_transaction *t = (bassa_transaction*)para;
  if (type == CURLINFO_HEADER_IN)
    {
      char *str = (char*)malloc(size+1);
      int i;
      for (i=0; i<size; i++)
	str [i] = data[i];
      str [size] = '\0';
#ifdef DEBUG
      printf ("HEADERS: %s\n", str);
#endif //DEBUG
      if (!strcmp (str, HTTP_OK) || !strcmp (str, HTTP_OK_1_0))
	{
#ifdef DEBUG
	  printf ("Ok!\n");
#endif //DEBUG
	  SET_STATUS(t->http_bf, HTTP_OK_FLAG);
	}
      else if (!strcmp (str, HTTP_PARTIAL_CONTENT) || !strcmp (str, HTTP_PARTIAL_CONTENT_1_0))
	{
#ifdef DEBUG
	  printf ("Resume supported!\n");
#endif //DEBUG
	  SET_STATUS(t->http_bf, HTTP_PARTIAL_CONTENT_FLAG);
	}
      else if (!strcmp (str, HTTP_NO_CONTENT) || !strcmp (str, HTTP_NO_CONTENT_1_0))
	{
#ifdef DEBUG
	  printf ("No content!\n");
#endif //DEBUG
	  SET_STATUS(t->http_bf, HTTP_NO_CONTENT_FALG);
	}
      else if(!strcmp (str, HTTP_BAD_REQUEST) || !strcmp (str, HTTP_BAD_REQUEST_1_0))
	{
#ifdef DEBUG
	  printf ("Bad request!\n");
#endif //DEBUG
	  SET_STATUS(t->http_bf, HTTP_BAD_REQUEST_FLAG);
	}
      else if(!strcmp (str, HTTP_UNAUTHORIZED) || !strcmp (str, HTTP_UNAUTHORIZED_1_0))
	{
#ifdef DEBUG
	  printf ("Unauthoried!\n");
#endif //DEBUG
	  SET_STATUS(t->http_bf, HTTP_UNAUTHORIZED_FLAG);
	}
      else if(!strcmp (str, HTTP_FORBIDDEN) || !strcmp (str, HTTP_FORBIDDEN_1_0))
	{
#ifdef DEBUG
	  printf ("Forbidden!\n");
#endif //DEBUG
	  SET_STATUS(t->http_bf, HTTP_FORBIDDEN_FLAG);
	}
      else if(!strcmp (str, HTTP_NOT_FOUND) || !strcmp (str, HTTP_NOT_FOUND_1_0))
	{
#ifdef DEBUG
	  printf ("Not found!\n");
#endif //DEBUG
	  SET_STATUS(t->http_bf, HTTP_NOT_FOUND_FLAG);
	}
      else if(!strcmp (str, HTTP_REQUEST_TIMEOUT) || !strcmp (str, HTTP_REQUEST_TIMEOUT_1_0))
	{
#ifdef DEBUG
	  printf ("Request timeout!\n");
#endif //DEBUG
	  SET_STATUS(t->http_bf, HTTP_REQUEST_TIMEOUT_FLAG);
	}
      else if(!strcmp (str, HTTP_INTERNAL_SERVER_ERROR) || !strcmp (str, HTTP_INTERNAL_SERVER_ERROR_1_0))
	{
#ifdef DEBUG
	  printf ("Internal server error!\n");
#endif //DEBUG
	  SET_STATUS(t->http_bf, HTTP_INTERNAL_SERVER_ERROR_FLAG);
	}
      else if(!strcmp (str, HTTP_VERSION_NOT_SUPPORTED) || !strcmp (str, HTTP_VERSION_NOT_SUPPORTED_1_0))
	{
#ifdef DEBUG
	  printf ("HTTP version not supported!\n");
#endif //DEBUG
	  SET_STATUS (t->http_bf, HTTP_VERSION_NOT_SUPPORTED_FLAG);
	}
      else if(!strncmp (str, HTTP_CONTENT_LENGTH, strlen (HTTP_CONTENT_LENGTH)))
	{
	  char *length = bassa_transaction_get_value (str, HTTP_CONTENT_LENGTH);
	  t->size = atoll(length);
	  free (length);
	  length = NULL;
	}
      else if(!strncmp (str, HTTP_CONTENT_DISPOSITION, strlen (HTTP_CONTENT_DISPOSITION)))
	{
	  char *disposition = bassa_transaction_get_value (str, HTTP_CONTENT_DISPOSITION);
	  //Check whether disposition-type = attachment
	  if (!strncmp (disposition, HTTP_CONTENT_DISP_ATTACHMENT, 
			strlen(HTTP_CONTENT_DISP_ATTACHMENT)))
	    {
	      //Get the disposition param
	      char *disparm = &disposition[strlen(HTTP_CONTENT_DISP_ATTACHMENT)];
	      int i;
	      char *str1, *str2, *token, *subtoken1, *subtoken2;
	      char *saveptr1, *saveptr2;
	      char *delim1 = ";";
	      char *delim2 = "=";
	      for (str1 = disparm, i=0; ;str1 = NULL, i++) 
		{
		  token = strtok_r(str1, delim1, &saveptr1);
		  if (token == NULL)
		    break;
		  if (i == 0)
		    {
		      if (strstr (token, HTTP_CONTENT_DISP_ATTACHMENT))
			{
			  t->disposition_type=HTTP_CONTENT_DISP_ATCHMNT_ID;
			}
		      continue;
		    } 
		  str2 = token;
		  subtoken1 = strtok_r(str2, delim2, &saveptr2);
		  if (subtoken1 == NULL)
		    break;
		  subtoken2 = strtok_r(NULL, delim2, &saveptr2);
		  if (subtoken2 == NULL)
		    break;
		  if (strstr (subtoken1, HTTP_CONTENT_DISP_FNAME))
		    {
		      t->disposition_fname = subtoken2;
		    }	
		  free (token);
		  token = NULL; 
		  free (subtoken1);
		  subtoken1 = NULL;
		}
	    }
	  free (disposition);
	  disposition = NULL;
	}
      free (str);
    }
  return 0;
}

void
bassa_transcation_set_network_opts (bassa_transaction *transaction,
				    long connect_timeout, int max_tries)
{
  if (transaction)
    {
      transaction->max_tries = max_tries;
      transaction->connect_timeout = connect_timeout;
    }
  return;
}

void
bassa_transaction_encode_url (bassa_transaction *t)
{
  char *encfile = NULL;
  if (IS_HTTP_PROTO(t->http_bf) || IS_HTTPS_PROTO(t->http_bf))
    {
      encfile = curl_easy_escape (NULL, bassa_file_get_name(t->url), 0);
      t->url_encoded = (char*)malloc(strlen(t->path) + strlen(encfile) + 1);
      strcpy (t->url_encoded, t->path);
      strcat (t->url_encoded, encfile);
    }
  else
    {
      t->url_encoded = t->url;
    }
  if (encfile)
    {
      curl_free (encfile);
      encfile = NULL;
    }
}

char*
bassa_transaction_get_value (char *header_line, char *header_name)
{
  int name_len = strlen(header_name);
  int line_len = strlen(header_line);
  int x = line_len - name_len + 1;
  char *value = (char*)malloc(x);
  int i,j;
  for (i=name_len,j=0; i<line_len-1; i++,j++)
    value[j] = header_line[i];
  value[j]='\0';
  return value;
}

void
bassa_transaction_set_http_init (bassa_transaction *transaction)
{	
  int os1, ot1;
  bassa_disable_cancel (&os1, &ot1);
  //Add header Range: bytes=0-, so we will be able to know whether
  //the remote server supports resuming downloads. However we will
  //be able to download the file from beginning irrespective of resume
  //support of the remote server.
#ifdef DEBUG
  printf ("bassa_transaction_SET_HTTP_INIT\n");
#endif //DEBUG
  char *header = (char*)malloc(strlen(HTTP_RANGE_X_TO_END) + 18);
  sprintf (header, HTTP_RANGE_X_TO_END, 0);
  if (!(transaction->header_list))
    transaction->header_list = curl_slist_append (transaction->header_list, header);
  free (header);
  //Set curl options
  bassa_transaction_set_options (transaction, CONNECT_TIMEOUT, 0);
  bassa_reset_cancel (&os1, &ot1);
}

void
bassa_transaction_set_http_resume (bassa_transaction *transaction, int current_size)
{
  int os1, ot1;
  bassa_disable_cancel (&os1, &ot1);
#ifdef DEBUG
  printf ("bassa_transaction_SET_HTTP_RESUME\n");
#endif
  //Clean every thing previously set
  if (transaction->header_list)
    {
      curl_slist_free_all (transaction->header_list);
      transaction->header_list = NULL;
#ifdef DEBUG
      printf ("CURL_SLSIT_CLEANED\n");
#endif //DEBUG
    }
  curl_easy_reset (transaction->curl_handle);
  if (IS_PARTIAL_CONTENT(transaction->http_bf))
    {
#ifdef DEBUG
      printf ("RESUMING DOWNLOAD!\n");
#endif //DEBUG
      //This is a resume download!, we should continue from the
      //point, where we have stoped this download earlier.
      //Create the header first
      char *header = (char*)malloc(strlen(HTTP_RANGE_X_TO_END) + 18);
      memset(header, '\0', strlen(HTTP_RANGE_X_TO_END) + 18);
      sprintf (header, HTTP_RANGE_X_TO_END, current_size);
      if (!(transaction->header_list))
	transaction->header_list = curl_slist_append (transaction->header_list, header);
      free (header);
      //Set curl options
      bassa_transaction_set_options (transaction, CONNECT_TIMEOUT, 0);
      fseek (transaction->local_file, current_size, SEEK_SET);
    }
  bassa_reset_cancel (&os1, &ot1);
}

void
bassa_transaction_prp_reinit (bassa_transaction *transaction, char *url, char *local_path, 
			  long long size, char *http_proxy)
{
  if (transaction == NULL)
    return;
  if (url && local_path)
    {
      char *part_local_lpath = (char *) malloc (strlen (local_path) + strlen (DOWNLOAD_PART_EXT) + 1);
      strcpy (part_local_lpath, local_path);
      strcat (part_local_lpath, DOWNLOAD_PART_EXT);
      if (transaction->file_complete)
        {
          free (transaction->file_complete);
          transaction->file_complete = local_path;
        }
      else
        transaction->file_complete = local_path;
      if (transaction->file_part)
        {
          free (transaction->file_part);
          transaction->file_part = part_local_lpath;
        }
      else
        transaction->file_part = part_local_lpath;
      transaction->url = url;
      transaction->path = bassa_get_location (transaction->url);
      transaction->size = size;
      transaction->http_proxy = http_proxy;
      transaction->header_list = NULL;
      transaction->http_bf = 0;
      transaction->disposition_type = HTTP_CONTENT_DISP_ATCHMNT_ID;
      //Set the protocol before encoding URL
      if (strlen (HTTP_PROTO) < strlen (transaction->url))
	{
	  if (!strncmp (HTTP_PROTO, transaction->url, 
			strlen (HTTP_PROTO)))
	    {
	      SET_PROTO(transaction->http_bf, HTTP_PROTO_FLAG);
	    }
	}
      else if (strlen (HTTPS_PROTO) < strlen (transaction->url))
	{
	  if (!strncmp (HTTPS_PROTO, transaction->url, 
			strlen (HTTPS_PROTO)))
	    {
	      SET_PROTO(transaction->http_bf, HTTP_PROTO_FLAG);
	    }
	}
      else if (strlen (FTP_PROTO) < strlen (transaction->url))
	{
	  if (!strncmp (FTP_PROTO, transaction->url, 
			strlen (FTP_PROTO)))
	    {
	      SET_PROTO(transaction->http_bf, FTP_PROTO_FLAG);
	    }
	}
      else
	{
	  SET_PROTO(transaction->http_bf, UNKNOWN_PROTO);
	}
      if (transaction->url_encoded)
        {
          curl_free (transaction->url_encoded);
          bassa_transaction_encode_url(transaction);
        }
    }
}

