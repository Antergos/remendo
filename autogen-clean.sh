#!/bin/sh
if [ -f Makefile ]; then
	echo "Making make distclean..."
	make distclean
fi
echo "Removing autogenned files..."
rm -rf aclocal.m4 autom4te.cache depcomp intltool-extract.in \
		intltool-merge.in intltool-update.in config.guess \
		config.sub configure install-sh missing mkinstalldirs \
		Makefile.in ltmain.sh stamp-h.in */Makefile.in ltconfig \
		stamp-h config.h.in 
echo "Done."







