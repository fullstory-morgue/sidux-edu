sharedir = "/usr/share"
#sharedir = $(pkgdatadir)
install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(sharedir)
	cp -aR $(srcdir)/sidux-edu $(DESTDIR)$(sharedir)/
	cp -aR $(srcdir)/kappinstaller $(DESTDIR)$(sharedir)/
	$(mkinstalldirs) $(DESTDIR)$(sharedir)/sidux-edu/descriptions
	$(mkinstalldirs) $(DESTDIR)$(sharedir)/sidux-edu/descriptions/de
	$(mkinstalldirs) $(DESTDIR)$(sharedir)/kappinstaller/descriptions; \
	$(mkinstalldirs) $(DESTDIR)$(sharedir)/kappinstaller/descriptions/de2; \
	cd $(srcdir)/sidux-edu/pmap ;\
	for apps in *.pmap; do \
		subname=$$(basename $$apps .pmap); \
		  cp -aR $(srcdir)/common/descriptions/de/$$subname.txt $(DESTDIR)$(sharedir)/sidux-edu/descriptions/de/; \
	done; \
	cd ../../kappinstaller/pmap ;\
	for apps2 in *.pmap; do \
		subname=$$(basename $$apps2 .pmap); \
		cp ../../common/descriptions/de/$$subname.txt $(DESTDIR)$(sharedir)/kappinstaller/descriptions/de2/; \
	done;


#cp -aR $(srcdir)/appdata/common/* $(DESTDIR)$(sharedir)/edu/
#cp -aR $(srcdir)/appdata/common/* $(DESTDIR)$(sharedir)/kappinstaller/




