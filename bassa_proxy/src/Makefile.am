INCLUDES = -Isrc
bin_PROGRAMS=bproxy

bproxy_SOURCES=bassa_htmsg.c bassa_htrep.c bassa_pack.c bassa_proxy.c main.c

bproxy_LDADD=-lpthread

bproxy_CFLAGS=-DDEBUG -D_FILE_OFFSET_BITS=64 -D_FORTIFY_SOURCE=2 -Wall -DPOSIX

