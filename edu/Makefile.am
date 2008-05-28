# set the include path for X, qt and KDE
AM_CPPFLAGS = $(all_includes)

# these are the headers for your project
#noinst_HEADERS = edu.h unique.h systray.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

KDE_ICON = AUTO

# the application source, library search path, and link libraries
sidux_edu_SOURCES = main.cpp
sidux_edu_LDFLAGS = -avoid-version -module -no-undefined $(all_libraries)
sidux_edu_LDADD = $(LIB_KDEUI) ../libappinstaller/libappinstaller.la



# application
xdg_apps_DATA = sidux-edu.desktop
bin_PROGRAMS = sidux-edu
#bin_SCRIPTS = siduxEduInstallApp

sharedir = $(pkgdatadir)
install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(sharedir)
	$(mkinstalldirs) $(DESTDIR)$(sharedir)/edu
	cp -aR $(srcdir)/sh $(DESTDIR)$(sharedir)/edu/
