sharedir = /usr/share
#sharedir = $(pkgdatadir)
install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(sharedir)
	cp -aR $(srcdir)/sidux-edu $(DESTDIR)$(sharedir)/
	cp -aR $(srcdir)/kappinstaller $(DESTDIR)$(sharedir)/
	$(mkinstalldirs) $(DESTDIR)$(sharedir)/sidux-edu/descriptions
	$(mkinstalldirs) $(DESTDIR)$(sharedir)/sidux-edu/descriptions/de
	$(mkinstalldirs) $(DESTDIR)$(sharedir)/kappinstaller/descriptions; \
	$(mkinstalldirs) $(DESTDIR)$(sharedir)/kappinstaller/descriptions/de; \
	for apps in $(srcdir)/sidux-edu/pmap/*.pmap; do \
		subname=$$(basename $$apps .pmap); \
		cp -aR $(srcdir)/common/descriptions/de/$$subname.txt $(DESTDIR)$(sharedir)/sidux-edu/descriptions/de/; \
	done; \
	for apps2 in $(srcdir)/kappinstaller/pmap/*.pmap; do \
		subname2=$$(basename $$apps2 .pmap); \
		cp -aR  $(srcdir)/common/descriptions/de/$$subname2.txt $(DESTDIR)$(sharedir)/kappinstaller/descriptions/de/; \
	done;





