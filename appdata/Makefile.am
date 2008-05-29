sharedir = "/usr/share"
#sharedir = $(pkgdatadir)
install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(sharedir)
	$(mkinstalldirs) $(DESTDIR)$(sharedir)/edu
	$(mkinstalldirs) $(DESTDIR)$(sharedir)/kappinstaller
	$(mkinstalldirs) $(DESTDIR)$(sharedir)
	cp -aR $(srcdir)/appdata/edu/* $(DESTDIR)$(sharedir)/sidux-edu/
	cp -aR $(srcdir)/categories $(DESTDIR)$(sharedir)/sidux-edu/
	cp -aR $(srcdir)/html $(DESTDIR)$(sharedir)/sidux-edu/
	cp -aR $(srcdir)/appdata/kappinstaller/* $(DESTDIR)$(sharedir)/kappinstaller/
	cd $(top_srcdir)/appdata/html;\
	for potfile in *.html; do \
		if test "$$subdir" = "."; then :; else \
			subname=$$(basename $$potfile .html); \
			cp $$subname.html $(DESTDIR)$(sharedir)/edu/test.fabian; \
		fi; \
	done;


#cp -aR $(srcdir)/appdata/common/* $(DESTDIR)$(sharedir)/edu/
#cp -aR $(srcdir)/appdata/common/* $(DESTDIR)$(sharedir)/kappinstaller/




