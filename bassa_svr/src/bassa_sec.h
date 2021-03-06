#ifndef BASSA_SEC_H
#define BASSA_SEC_H

#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <openssl/pem.h>

typedef struct
{
#ifdef POSIX
  mode_t fmode;
#endif //POSIX
  X509 *cert;
  EVP_PKEY *rsa_privkey;
  EVP_PKEY *rsa_pubkey;
}
bassa_sec_mode;

int bassa_sec_setuid(char *user);
int bassa_sec_setgid(char *user);

int bassa_sec_setowner(char *user, char *cgroup, char *file);
int bassa_sec_setmode(bassa_sec_mode *mode, char *file);

#endif //BASSA_SEC_H
