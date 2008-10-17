#ifndef PACKER_H
#define PACKER_H

#include "bassa_htmsg.h"

#define FIELD_TEMPLATE "%s: %s\r\n"
#define FIELD_TEMPLATE_LEN 4

#define FIRSTLINE_TEMPLATE "%s %s %s\r\n"
#define FIRSTLINE_TEMPLATE_LEN 4

#define WS ' '

void bassa_pack_first_line (bassa_http_msg *msg);
char* bassa_pack_field (char *key, char *val);
char* bassa_unpack_val (char *field, int field_len);

#endif //PACKER_H

