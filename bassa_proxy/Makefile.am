SUBDIRS = src
realclean: maintainer-clean
.PHONY: realclean
realclean: soapclean
	$(RM) -r .libs
	find . -name \*~ | xargs $(RM)
	find . -name Makefile.in | xargs $(RM)
