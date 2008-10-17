#ifndef BASSA_REPLY_H
#define BASSA_REPLY_H

#include "bassa_htmsg.h"
#include "bassa_proxy.h"

void bassa_reply_GET (bassa_client *bc, bassa_http_msg *msg_req, bassa_http_msg *msg_rep);
void bassa_reply_POST (bassa_client *bc, bassa_http_msg *msg_req, bassa_http_msg *msg_rep);
void bassa_reply_HEAD (bassa_client *bc, bassa_http_msg *msg_req, bassa_http_msg *msg_rep);

#endif //BASSA_REPLY_H

