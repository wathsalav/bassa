SUBDIRS = src
soapclean:
	find . -name *.xsd | xargs $(RM)
	find . -name *.xml | xargs $(RM)
	find . -name *.nsmap | xargs $(RM)
	find . -name *.wsdl | xargs $(RM)
	find . -name soap*.c | xargs $(RM)
	find . -name soap*.h | xargs $(RM)

clean: soapclean
	find . -name \*~ | xargs $(RM)
	find . -name \*.o | xargs $(RM)
realclean: maintainer-clean
.PHONY: realclean
realclean: soapclean
	$(RM) -r .libs
	find . -name \*~ | xargs $(RM)
	find . -name Makefile.in | xargs $(RM)