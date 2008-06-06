/***************************************************************************
 *   Copyright (C) 2007 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <expat.h>
#include <string.h>

#include "noc_filter_messages.h"


bassa_od_message *
bassa_od_message_new (char *name, char *email, char *org_unit, char *ip,
		      char *url, char *size)
{
  bassa_od_message *msg =
    (bassa_od_message *) malloc (sizeof (bassa_od_message));
  msg->name = name;
  msg->email = email;
  msg->org_unit = org_unit;
  msg->ip = ip;
  msg->url = url;
  msg->size_str = size;
  return msg;
}

bassa_messages *
bassa_messages_decode (char *xml)
{
  if (!xml)
    return NULL;
  XML_StartElementHandler sh = (XML_StartElementHandler)bassa_message_start_handler;
  XML_EndElementHandler eh = (XML_EndElementHandler)bassa_message_end_handler;
  XML_CharacterDataHandler ch = (XML_CharacterDataHandler)bassa_message_cdata_handler;

  bassa_od_message *od_message =
    bassa_od_message_new (NULL, NULL, NULL, NULL, NULL, NULL);
  bassa_search_message *search_msg = bassa_search_message_new (NULL);
  bassa_messages *bassa = bassa_messages_new (od_message, search_msg);
	
  XML_Parser parser = XML_ParserCreate (NULL);
  XML_SetUserData (parser, (void *) bassa);
  XML_SetStartElementHandler (parser, sh);
  XML_SetEndElementHandler (parser, eh);
  XML_SetCharacterDataHandler (parser, ch);
  XML_Parse (parser, xml, strlen (xml), 1);
  XML_ParserFree (parser);
  return bassa;
}

char *
bassa_od_message_encode (bassa_od_message * msg)
{
  return NULL;
}

void
bassa_od_message_delete (bassa_od_message * msg)
{
  free (msg);
  msg = NULL;
}

void
bassa_search_message_delete (bassa_search_message * msg)
{
  free (msg);
  msg = NULL;
}

void
bassa_messages_delete (bassa_messages * msg)
{
  if (msg)
    {
      if (msg->od_message)
	{
	  free (msg->od_message);
	  msg->od_message = NULL;
	}
      if (msg->search_message)
	{
	  free (msg->search_message);
	  msg->search_message = NULL;
	}
      free (msg);
      msg = NULL;
    }
}

void
bassa_message_start_handler (void *udata, char *name, char **attr)
{
  bassa_messages *nm = (bassa_messages *) udata;
	
  if (!strcmp (name, BASSA_OD))
    {
      nm->current_tag = BASSA_OD_ID;
      nm->is_od_message = 1;
    }

  else if (!strcmp (name, BASSA_SEARCH_MSG))
    {
      nm->current_tag = BASSA_SEARCH_MSG_ID;
      nm->is_search_message = 1;
    }

  else if (!strcmp (name, BASSA_NAME))
    nm->current_tag = BASSA_NAME_ID;
	
  else if (!strcmp (name, BASSA_EMAIL))
    nm->current_tag = BASSA_EMAIL_ID;

  else if (!strcmp (name, BASSA_IP))
    nm->current_tag = BASSA_IP_ID;
	
  else if (!strcmp (name, BASSA_URL))
    nm->current_tag = BASSA_URL_ID;

  else if (!strcmp (name, BASSA_TIME))
    nm->current_tag = BASSA_TIME_ID;

  else if (!strcmp (name, BASSA_ORG_UNIT))
    nm->current_tag = BASSA_ORG_UNIT_ID;
	
  else if (!strcmp (name, BASSA_SEARCH_MSG))
    nm->current_tag = BASSA_SEARCH_MSG_ID;
	
  else if (!strcmp (name, BASSA_SEARCH_FILE))
    nm->current_tag = BASSA_SEARCH_FILE_ID;

  else if (!strcmp (name, BASSA_SIZE))
    nm->current_tag = BASSA_SIZE_ID;
}

void
bassa_message_end_handler (void *udata, char *name)
{
  ((bassa_messages *) udata)->current_tag = -1;
}

void
bassa_message_cdata_handler (void *udata, char *s, int len)
{
  bassa_messages *nm = (bassa_messages *) udata;
  if (nm->current_tag == BASSA_NAME_ID)
    nm->od_message->name =
      bassa_assemble_message(nm->od_message->name, s, len);
	
  else if (nm->current_tag == BASSA_EMAIL_ID)
    nm->od_message->email =
      bassa_assemble_message(nm->od_message->email, s, len);
		
  else if (nm->current_tag == BASSA_ORG_UNIT_ID)
    nm->od_message->org_unit =
      bassa_assemble_message(nm->od_message->org_unit, s, len);
	
  else if (nm->current_tag == BASSA_IP_ID)
    nm->od_message->ip =
      bassa_assemble_message(nm->od_message->ip, s, len);
		
  else if (nm->current_tag == BASSA_URL_ID)
    nm->od_message->url =
      bassa_assemble_message(nm->od_message->url, s, len);
	
  else if (nm->current_tag == BASSA_TIME_ID)
    nm->od_message->time =
      bassa_assemble_message(nm->od_message->time, s, len);
	
  else if (nm->current_tag == BASSA_SEARCH_FILE_ID)
    nm->search_message->file_name = 
      bassa_assemble_message(nm->search_message->file_name, s, len);
	
  else if (nm->current_tag == BASSA_SIZE_ID)
    nm->od_message->size_str =
      bassa_assemble_message(nm->od_message->size_str, s, len);
}

bassa_search_message *
bassa_search_message_new (char *file_name)
{
  bassa_search_message *msg =
    (bassa_search_message *) malloc (sizeof (bassa_search_message));
  msg->file_name = file_name;
  return msg;
}

bassa_messages *
bassa_messages_new (bassa_od_message * od_message,
		    bassa_search_message * search_msg)
{
  bassa_messages *bassa =
    (bassa_messages *) malloc (sizeof (bassa_messages));
  bassa->od_message = od_message;
  bassa->search_message = search_msg;
  bassa->is_search_message = 0;
  bassa->is_od_message = 0;
  return bassa;
}

char *
bassa_search_message_encode (char *file_name)
{
  char *otag =
    (char *) malloc (strlen (BASSA_OTAG) +
		     strlen (BASSA_SEARCH_RESULT));
  char *ctag =
    (char *) malloc (strlen (BASSA_CTAG) +
		     strlen (BASSA_SEARCH_RESULT));
  sprintf (otag, BASSA_OTAG, BASSA_SEARCH_RESULT);
  sprintf (ctag, BASSA_CTAG, BASSA_SEARCH_RESULT);
  otag = (char *) realloc (otag, strlen (otag) + strlen (file_name) + 1);
  strcat (otag, file_name);
  otag = (char *) realloc (otag, strlen (otag) + strlen (ctag) + 1);
  strcat (otag, ctag);
  free (ctag);
  return otag;
}

char *
bassa_ackmessage_encode (char *status, char *code, char *redurl)
{
  char *msg = (char*)malloc(BASSA_ACK_LEN(code,status,redurl));
  memset(msg, (int)'\0', BASSA_ACK_LEN(code,status,redurl));
  if (redurl)
    sprintf (msg, BASSA_ACK_MSG, code, status, redurl);
  else 
    sprintf (msg, BASSA_ACK_MSG, code, status, "");
  return msg;
}

char *
bassa_message_encode (char *child_msg)
{
  char *otag =
    (char *) malloc (strlen (BASSA_OTAG) + strlen (BASSA_BASSA) - 
		     1);
  char *ctag =
    (char *) malloc (strlen (BASSA_CTAG) + strlen (BASSA_BASSA) -
		     1);
  sprintf (otag, BASSA_OTAG, BASSA_BASSA);
  sprintf (ctag, BASSA_CTAG, BASSA_BASSA);
  otag = (char *) realloc (otag, strlen (otag) + strlen (child_msg) + 1);
  strcat (otag, child_msg);
  otag = (char *) realloc (otag, strlen (otag) + strlen (ctag) + 1);
  strcat (otag, ctag);
  free (ctag);
  return otag;
}

char *
bassa_xml_document_encode (char *bassa_msg)
{
  char *xml_doc =
    (char *) malloc (strlen (bassa_msg) + strlen (BASSA_XML_1_0) + 1);
  strcpy (xml_doc, BASSA_XML_1_0);
  strcat (xml_doc, bassa_msg);
  return xml_doc;
}

char*
bassa_assemble_message(char *element, char *s, int len)
{
  if (!(element))
    {
      element = (char *) malloc (len + 1);
      strncpy (element , s, len);
      int length = len;
      element[length] = '\0';
    }
  else
    {
      int length = strlen (element) + len;
      element = 
	realloc (element, length + 1);
      strncat (element, s, len);
      element[length] = '\0';
    }
  return element;
}

