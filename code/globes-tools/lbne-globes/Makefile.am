ACLOCAL_AMFLAGS = -I m4

DIST_SUBDIRS = lib source data  globes doc
SUBDIRS = lib source data globes doc
INCLUDES = $(F2CINC) $(GSLINC)

EXTRA_DIST = examples/Makefile.in examples/example1.c	      \
  examples/example3.c examples/myio.c examples/example-tour.c \
  examples/example2.c examples/example4.c examples/example5.c examples/example6.c \
  examples/myio.h \
  examples/NFstandard.glb examples/D-Chooz_far.glb examples/D-Chooz_near.glb \
  examples/Reactor1.glb examples/Reactor.dat examples/XCC.dat examples/XNC.dat \
  examples/XCCreactor.dat BUGS globes.m4 data/md5-sums \
  examples/DocPlots.nb m4/gnulib-cache.m4

# that is only needed if the testsuite should go into the release
# testsuite/01-probabilities.c             testsuite/03-chisys.glb   testsuite/06-chibars.c \
# testsuite/01-probabilities.glb           testsuite/04-chicorr.c    testsuite/06-chibars.glb  testsuite/myio.c \
# testsuite/02-probabilities-filtered.c    testsuite/04-chicorr.glb  testsuite/myio.h \
# testsuite/02-probabilities-filtered.glb  testsuite/05-chideg.c     testsuite/glbdiff.pl      testsuite/README \
# testsuite/03-chisys.c                    testsuite/05-chideg.glb   testsuite/run-tests


nodist_bin_SCRIPTS = globes-config

CLEANFILES = globes-config

AM_YFLAGS =  -d

m4datadir = $(datadir)/aclocal
m4data_DATA = globes.m4

distcheck-hook:
	cd $(distdir)/data && md5sum -c md5-sums

if MAKE_RPMS

rpm: $(DISTFILES)
	${MAKE} dist
# adding my own helpless stuff ...
	cp $(RPM_TARBALL) $(RPM_SOURCE_DIR)/
	$(RPM_PROG) $(RPM_ARGS) --clean @PACKAGE@.spec
	$(RPM_PROG) --rmsource @PACKAGE@.spec
	cp $(RPM_SRPM_NAME) ./
	cp $(RPM_TARGET) ./
	rm -f $(RPM_SRPM_NAME)
	rm -f $(RPM_SOURCE_DIR)/$(RPM_TARBALL)
	rm -rf $(RPM_BUILD_DIR)/$(RPM_VERSION)
	rm -f $(RPM_TARGET)
	@(echo "$(distdir) rpms ready for distribution: "; \
          list='$(RPM_CLEAN_NAMES)'; for i in $$list; do echo $$i; done) | \
          sed -e '1{h;s/./=/g;p;x;}' -e '$${p;x;}'

rpm-clean:
	rm -f $(RPM_SRPM_NAME)
	rm -f $(RPM_SOURCE_DIR)/$(RPM_TARBALL)
	rm -rf $(RPM_BUILD_DIR)/$(RPM_VERSION)
	rm -f $(RPM_TARGET)

.PHONY: rpm rpm-clean

else
endif
