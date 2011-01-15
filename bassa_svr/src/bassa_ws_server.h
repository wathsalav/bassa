#ifndef BASSA_WS_SERVER_H
#define BASSA_WS_SERVER_H
#include <signal.h>

#include <soapH.h>
#include <noc_filter_configure.h>
#include <noc_filter_concur.h>
#include <bassa_trigger.h>

extern bassa_conf *conf;

bassa_trigger *btrig;

void *bassa_ws_start(void *arg);
void *bassa_ws_exec(void *soap);

#endif //BASSA_WS_SERVER_H

