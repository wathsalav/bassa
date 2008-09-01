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
 *Set the hard limit and soft limit to limit
 */
void bassa_file_set_max_size (int limit);

/**
 *Get the file size
 */
long long bassa_file_get_size (char *file_name);

/**
 * Return 0 if file path fname exists,
 * otherwise return 1
 */
int bassa_transaction_fexist(char *fname);

#endif //NOC_FILTER_FILE
