## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src po

remendo_gtkdocdir = ${prefix}/doc/remendo_gtk
remendo_gtkdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


INTLTOOL_FILES = intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

EXTRA_DIST = $(remendo_gtkdoc_DATA) \
	$(INTLTOOL_FILES)

DISTCLEANFILES = intltool-extract \
	intltool-merge \
	intltool-update \
	po/.intltool-merge-cache


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(remendo_gtkdocdir)
