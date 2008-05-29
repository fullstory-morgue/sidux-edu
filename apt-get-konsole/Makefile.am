# set the include path for X, qt and KDE
AM_CPPFLAGS = $(all_includes)

# these are the headers for your project
noinst_HEADERS = ac.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

KDE_ICON = AUTO

# the application source, library search path, and link libraries
apt_get_konsole_SOURCES = ac.cpp acdialog.ui main.cpp
apt_get_konsole_LDFLAGS = -avoid-version -module -no-undefined $(all_libraries)
apt_get_konsole_LDADD = -lkio -lknewstuff $(LIB_KDEUI)



# application
bin_PROGRAMS = apt-get-konsole

#sharedir = $(pkgdatadir)
sharedir = /usr/share
install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(sharedir)
	$(mkinstalldirs) $(DESTDIR)$(sharedir)/apt-get-konsole
	cp -aR $(srcdir)/sh $(DESTDIR)$(sharedir)/apt-get-konsole/


# translation
messages: rc.cpp
	$(EXTRACTRC) `find . -name \*.ui -o -name \*.rc` > rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/aptgetkonsole.pot
