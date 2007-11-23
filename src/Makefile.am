# set the include path for X, qt and KDE
AM_CPPFLAGS = $(all_includes)

# these are the headers for your project
noinst_HEADERS = edu.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

KDE_ICON = AUTO

# the application source, library search path, and link libraries
sidux_edu_SOURCES = edu.cpp edudialog.ui main.cpp
sidux_edu_LDFLAGS = -avoid-version -module -no-undefined $(all_libraries)
sidux_edu_LDADD = -lkio -lknewstuff $(LIB_KDEUI)



# application
xdg_apps_DATA = sidux-edu.desktop
bin_PROGRAMS = sidux-edu
bin_SCRIPTS = siduxEduInstallApp

sharedir = $(pkgdatadir)
install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(sharedir)
	cp -aR $(srcdir)/apps $(DESTDIR)$(sharedir)/
	cp -aR $(srcdir)/categories $(DESTDIR)$(sharedir)/
	cp -aR $(srcdir)/list $(DESTDIR)$(sharedir)/


# translation
#messages: rc.cpp
#	$(EXTRACTRC) `find . -name \*.ui -o -name \*.rc` > rc.cpp
#	$(XGETTEXT) *.cpp -o $(podir)/edu.pot
