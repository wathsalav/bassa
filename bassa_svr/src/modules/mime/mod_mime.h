/***************************************************************************
 *   Copyright (C) 2007 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#ifndef MOD_MIME_H
#define MOD_MIME_H
#include <magic.h>


__attribute__((destructor))void fini_mod_mime ();

char*
mod_mime_gettype (char *filename);

void
module_init (char *conf);

#endif //MOD_MIME_H 
