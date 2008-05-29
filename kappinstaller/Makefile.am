# set the include path for X, qt and KDE
AM_CPPFLAGS = $(all_includes)

# these are the headers for your project
noinst_HEADERS = kappinstaller.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

KDE_ICON = AUTO

# the application source, library search path, and link libraries
kappinstaller_SOURCES = main.cpp kappinstaller.cpp kappinstallerdialog.ui
kappinstaller_LDFLAGS = -avoid-version -module -no-undefined $(all_libraries)
kappinstaller_LDADD = $(LIB_KDEUI) ../libappinstaller/libappinstaller.la



# application
xdg_apps_DATA = kappinstaller.desktop
bin_PROGRAMS = kappinstaller

#sharedir = $(pkgdatadir)
sharedir= "/usr/share"
install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(sharedir)
	$(mkinstalldirs) $(DESTDIR)$(sharedir)/kappinstaller
	cp -aR $(srcdir)/lists $(DESTDIR)$(sharedir)/kappinstaller/

# translation
messages: rc.cpp
	$(EXTRACTRC) `find . -name \*.ui -o -name \*.rc` > rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kappinstaller.pot
