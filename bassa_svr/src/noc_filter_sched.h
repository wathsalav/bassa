#ifndef NOC_FILTER_SCHED_H
#define NOC_FILTER_SCHED_H
#include <signal.h>

#include "noc_filter_file.h"
#include "noc_filter_queue.h"
#include "noc_filter_timer.h"
#include "noc_filter_transaction.h"
#include "noc_filter_user.h"
#include "noc_filter_server.h"
#include "noc_filter_concur.h"
#include "noc_filter_messages.h"
#include "noc_filter_repo.h"
#include "noc_filter_configure.h"
#include "noc_filter_util.h"
/**
 * Parameters for scheduler
 */

typedef struct
{
  bassa_queue *gqueue;
  bassa_timer *timer;
  bassa_conf *conf;
  int bassa_downloaders;
  pthread_t *bassa_downloader_ids;
  int *bassa_cleaner_status;
  int bassa_max_downloaders;
  int bassa_downloader_count;
  bassa_task_pool *mtp, *stp;
  unsigned int trigger;
  unsigned int sigignore_flag;
} bassa_sched;


bassa_sched *nfs;
/**
 * Create a new scheduler.
 */
bassa_sched *bassa_sched_new (bassa_conf *conf);

/**
 * This is the function that calls for HTTP transaction when downloading user requests.
 * @param un The user object responsible for the request.
 */
void *bassa_sched_outop_thread (void *un);

/**
 * This is a function used for debugging a user.
 * @param ux The user to be debuged.
 */
void bassa_sched_debug_user (bassa_request * ux);

/**
 * This is the function handling the alarm signal.
 * @param signum Signal number.
 */
void bassa_sigalarm_handler (int signum);

/**
 * This function runs in a separate thread and handles concurrent downloads.
 * @param param
 */
void *bassa_sched_downloader_loop (void *param);

/**
 * This is the thread cleanup handler, called when timer expires and this function will,
 * cancel all other concurrent downloader threads.
 * @param param
 */
void bassa_sched_downloader_cleanup_loop (void *param);

/**
 * This function clean the data structures and state of the downloading thread.
 * This function is called when bassa_main_downloader_cleanup_loop() wants to
 * cancel downloading thread.
 */
void bassa_sched_outop_thread_clean (void *param);

/**
 * Start scheduler
 */
int bassa_sched_start (bassa_sched *nfs);

int bassa_cleaners_done (bassa_sched *nfs);

#endif
