## Process this file with automake to produce Makefile.in
## Created by Anjuta


uidir = $(pkgdatadir)/ui
ui_DATA = remendo_gtk.ui


## The main script
bin_SCRIPTS = remendo_gtk.py

## Directory where .class files will be installed
remendo_gtkdir = $(pythondir)/remendo_gtk


remendo_gtk_PYTHON = \
	remendo_gtk.py


EXTRA_DIST = $(ui_DATA)


# Remove ui directory on uninstall
uninstall-local:

	-rm -r $(uidir)

	-rm -r $(pkgdatadir)
