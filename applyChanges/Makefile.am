# set the include path for X, qt and KDE
AM_CPPFLAGS = $(all_includes)

# these are the headers for your project
noinst_HEADERS = ac.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

KDE_ICON = AUTO

# the application source, library search path, and link libraries
sidux_edu_applyChanges_SOURCES = ac.cpp acdialog.ui main.cpp
sidux_edu_applyChanges_LDFLAGS = -avoid-version -module -no-undefined $(all_libraries)
sidux_edu_applyChanges_LDADD = -lkio -lknewstuff $(LIB_KDEUI)



# application
bin_PROGRAMS = sidux-edu-applyChanges

sharedir = $(pkgdatadir)
install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(sharedir)
	cp -aR $(srcdir)/sh $(DESTDIR)$(sharedir)/


# translation
messages: rc.cpp
	$(EXTRACTRC) `find . -name \*.ui -o -name \*.rc` > rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/sidux-edu-applyChanges.pot
