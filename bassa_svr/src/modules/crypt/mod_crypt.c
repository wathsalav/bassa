/***************************************************************************
 *   Copyright (C) 2007 by wathsala vithanage   *
 *   wvi@ucsc.cmb.ac.lk   *
 ***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <confuse.h>
#include <openssl/evp.h>
#include <openssl/bn.h>

#include <bassa_module_manager.h>

#define SHA1 "sha1"
#define MD5 "md5"
#define MD4 "md4"
#define HMAC "hmac"

void 
module_init (char *conf);

void 
module_exit (void *ctx);

char*
mod_crypt_gethash (char *message, char *algo);

char* 
mod_crypt_getsha1(char *mess);

char*
mod_crypt_getmd5(char* mess);

char*
mod_crypt_getmd4(char* mess);

bassa_module_region module_region = COR_REGION;
bassa_exec_point module_execpoint = COR_X;

void
module_init (char *conf)
{
  OpenSSL_add_all_digests();
}

void 
module_exit (void *ctx)
{
}

char*
mod_crypt_gethash (char *message, char *algo)
{
  EVP_MD_CTX mdctx;
  const EVP_MD *md;
  unsigned char md_value[EVP_MAX_MD_SIZE];
  unsigned int md_len;
  md = EVP_get_digestbyname (algo);
  EVP_MD_CTX_init(&mdctx);
  EVP_DigestInit_ex(&mdctx, md, NULL);
  EVP_DigestUpdate(&mdctx, message, strlen(message));
  EVP_DigestFinal_ex(&mdctx, md_value, &md_len);
  EVP_MD_CTX_cleanup(&mdctx);
  BIGNUM *bn = NULL;
  bn = BN_bin2bn(md_value, md_len, bn);
  char *hash = BN_bn2hex(bn);
  if (bn)
    {
      free (bn);
      bn = NULL;
    }
  return hash;
}

char* 
mod_crypt_getsha1(char *mess)
{
  return mod_crypt_gethash (mess,SHA1);
}

char*
mod_crypt_getmd5(char* mess)
{
  return mod_crypt_gethash (mess, MD5);
}

char*
mod_crypt_getmd4(char* mess)
{
  return mod_crypt_gethash (mess, MD4);
}

