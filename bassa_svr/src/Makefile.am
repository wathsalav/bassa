SUBDIRS = modules
INCLUDES = -Isrc -I/usr/include/libxml2
bin_PROGRAMS=bassa

include Makefile.defines
include Makefile.c_rules

bassa_SOURCES=noc_filter_file.h noc_filter_file.c\
		   noc_filter_timer.h noc_filter_timer.c noc_filter_transaction.h noc_filter_transaction.c\
		   noc_filter_server.h noc_filter_server.c\
		   noc_filter_messages.c noc_filter_repo.h noc_filter_repo.c main.c\
		   noc_filter_concur.h noc_filter_concur.c noc_filter_configure.h noc_filter_configure.c\
		   noc_filter_messages.h noc_filter_util.c noc_filter_sched.c\
		   bassa_pop_context.h bassa_pop_context.c bassa_prp_context.h bassa_prp_context.c\
		   bassa_module_manager.h bassa_module_manager.c bassa_module_info.h bassa_module_info.c\
		   bassa_db.c bassa_ws_services.c bassa_ws_server.c bassa_uri.c $(SOAP_C_SRC)
		
bassa_LDADD = -lcurl -lpthread -lexpat -ldl -ldbi -lm -lgsoap -lxml2

AM_CFLAGS = -DPOSIX_THREADS -DDEBUG -D_FILE_OFFSET_BITS=64 -D_FORTIFY_SOURCE=2 -Wall
AM_LDFLAGS = -rdynamic
