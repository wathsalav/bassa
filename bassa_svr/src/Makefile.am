#SUBDIRS = modules
INCLUDES = -Isrc -I/usr/include/libxml2
bin_PROGRAMS=bassa bassa_client

include Makefile.defines
include Makefile.c_rules
AM_CFLAGS=-O0
bassa_SOURCES=noc_filter_file.h noc_filter_file.c\
		   noc_filter_timer.h noc_filter_timer.c noc_filter_transaction.h noc_filter_transaction.c\
		   main.c bassa_sec.c\
		   noc_filter_concur.h noc_filter_concur.c noc_filter_configure.h noc_filter_configure.c\
		   noc_filter_util.c bassa_pop_context.h bassa_pop_context.c\
		   bassa_prp_context.h bassa_prp_context.c bassa_module_manager.h bassa_module_manager.c\ 
		   bassa_module_info.h bassa_module_info.c bassa_db.c bassa_ws_services.c bassa_ws_server.c\ 
		   bassa_uri.c $(SOAP_SERVER_SRC) bassa_trigger.c bassa_sched.c

bassa_LDADD = -lcurl -lpthread -lexpat -ldl -ldbi -lm -lgsoap -lxml2 -lssl

bassa_CFLAGS =-DPOSIX_THREADS -DDEBUG -D_FILE_OFFSET_BITS=64 -D_FORTIFY_SOURCE=2 -Wall -DPOSIX
#bassa_LDFLAGS = -static-libtool-libs

bassa_client_SOURCES=bassa_client_emu.c $(SOAP_CLIENT_SRC)
bassa_client_LDADD = -lgsoap

