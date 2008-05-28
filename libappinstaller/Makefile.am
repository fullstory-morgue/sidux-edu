noinst_LTLIBRARIES = libappinstaller.la

noinst_HEADERS = \
 unique.h  functions.h fancylistviewitem.h process.h \
 edu.h systray.h \
 kappinstaller.h


libappinstaller_la_SOURCES = \
 fancylistviewitem.cpp functions.cpp unique.cpp process.cpp \
 edu.cpp edudialog.ui systray.cpp \
 kappinstaller.cpp kappinstallerdialog.ui
libappinstaller_la_LDFLAGS  = $(all_libraries) -module -avoid-version -no-undefined
libappinstaller_la_LIBADD = @LIBOBJS@ 


INCLUDES = -I$(top_builddir) -I$(top_srcdir) $(all_includes)
METASOURCES = AUTO
# translation
messages: rc.cpp
	$(EXTRACTRC) `find . -name \*.ui -o -name \*.rc` > rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/appinstaller.pot