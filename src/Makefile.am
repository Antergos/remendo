## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(REMENDO_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = remendo

remendo_SOURCES = \
	remendo.c \
	remendo.h \
	xml_handler.c \
	xml_handler.h \
	dl_events.c \
	dl_events.h
	
	

remendo_LDFLAGS = 

remendo_LDADD = $(REMENDO_LIBS)

