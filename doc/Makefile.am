SUBDIRS = de en C
EXTRA_DIST = example.sh enigma-referat.ps.bz2

docdir = $(datadir)/doc/$(PACKAGE)

install-data-local:
	$(mkinstalldirs) $(docdir)
	$(INSTALL_DATA) example.sh $(docdir)/example.sh
	$(INSTALL_DATA) enigma-referat.ps.bz2 $(docdir)/enigma-referat.ps.bz2
	if test -d $(srcdir)/enigma-referat; then \
	  $(mkinstalldirs) $(docdir)/enigma-referat; \
	  for files in $(srcdir)/enigma-referat/*; do \
	    if test -f $$files; then \
	      $(INSTALL_DATA) $$files $(docdir)/enigma-referat; \
	    fi \
	  done \
	fi

dist-hook:
	if test -d enigma-referat; then \
	  mkdir $(distdir)/enigma-referat; \
	  for files in enigma-referat/*; do \
	    if test -f $$files; then \
	     cp -p $$files $(distdir)/enigma-referat; \
	    fi \
	  done \
	fi
