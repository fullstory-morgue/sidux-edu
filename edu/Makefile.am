# set the include path for X, qt and KDE
AM_CPPFLAGS = $(all_includes)

# these are the headers for your project
noinst_HEADERS = edu.h systray.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

KDE_ICON = AUTO

# the application source, library search path, and link libraries
sidux_edu_SOURCES = main.cpp  edu.cpp edudialog.ui systray.cpp
sidux_edu_LDFLAGS = -avoid-version -module -no-undefined $(all_libraries)
sidux_edu_LDADD = $(LIB_KDEUI) ../libappinstaller/libappinstaller.la



# application
xdg_apps_DATA = sidux-edu.desktop
bin_PROGRAMS = sidux-edu

#sharedir = $(pkgdatadir)
sharedir = "/usr/share"
install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(sharedir)
	$(mkinstalldirs) $(DESTDIR)$(sharedir)/edu
	cp -aR $(srcdir)/sh $(DESTDIR)$(sharedir)/edu/

# translation
messages: rc.cpp
	$(EXTRACTRC) `find . -name \*.ui -o -name \*.rc` > rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/siduxedu.pot
