/***************************************************************************
 *   Copyright (C) 2006 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#ifndef NOC_FILTER_FILE
#define NOC_FILTER_FILE

#include <config.h>

#define BASSA_FILE_CVT_BYTES2GIGS(x)\
  (((x/1024)/1024)/1024)

#define BASSA_FILE_CVT_BYTES2MEGS(x)\
  ((x/1024)/1024)

#define BASSA_FILE_CVT_BYTES2KILOS(x)\
  (x/1024)

#define BASSA_FILE_CVT_GIGS2BYTES(x)\
  x*1024*1024*1024

#define BASSA_FILE_CVT_MEGS2BYTES(x)\
  x*1024*1024

#define BASSA_FILE_CVT_KILOS2BYTES(x)\
  x*1024

/**
 *Extract file name from url 
 */
char *bassa_file_get_name(char *url);

/**
 *Extract file extension from file name
 */
char *bassa_file_get_extension(char *file_name);

/**
 *Trim the extension and return the pointer
 */
char *bassa_file_trim_extension(char *file_name);
/**
 *Set the hard limit and soft limit to limit
 */
void bassa_file_set_max_size (int limit);

/**
 *Get the file size
 */
long long bassa_file_get_size (char *file_name);

/**
 *Get the path in a URL
 */
char *bassa_get_location (char *url);

#endif //NOC_FILTER_FILE
