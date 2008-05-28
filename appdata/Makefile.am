sharedir = $(pkgdatadir)
install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(sharedir)
	$(mkinstalldirs) $(DESTDIR)$(sharedir)/edu
	$(mkinstalldirs) $(DESTDIR)$(sharedir)/kappinstaller
	$(mkinstalldirs) $(DESTDIR)$(sharedir)
	cp -aR $(srcdir)/appdata/edu/* $(DESTDIR)$(sharedir)/edu/
	cp -aR $(srcdir)/categories $(DESTDIR)$(sharedir)/edu/
	cp -aR $(srcdir)/html $(DESTDIR)$(sharedir)/edu/
	cp -aR $(srcdir)/appdata/kappinstaller/* $(DESTDIR)$(sharedir)/kappinstaller/

#cp -aR $(srcdir)/appdata/common/* $(DESTDIR)$(sharedir)/edu/
#cp -aR $(srcdir)/appdata/common/* $(DESTDIR)$(sharedir)/kappinstaller/
