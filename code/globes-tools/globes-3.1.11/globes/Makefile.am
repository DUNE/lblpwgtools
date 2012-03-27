pkginclude_HEADERS = globes.h glb-modules.h

insthook = :
uninsthook = :

# building a perl module with swig's help
perlexecdir=$(pkglibdir)/perl
EXTRA_DIST = globes.i globes.pm
BUILT_SOURCES = globes_perl.c

globes_perl.c: globes.pm            

globes.pm: globes.i globes.h	
	$(SWIG) -perl5 $(SWIG_ARGS) -o globes_perl.c globes.i

if PERL

perlexec_DATA = globes.pm
perlexec_LTLIBRARIES = globes_perl.la
globes_perl_la_SOURCES = globes_perl.c
globes_perl_la_LIBADD = $(top_builddir)/source/libglobes.la
globes_perl_la_LDFLAGS = -avoid-version -module
INCLUDES= -I$(perl_includes)
globes_perl_la_DEPENDENCIES = globes.pm
globes_perl_la_CFLAGS = $(perl_cflags)

insthook += && cd $(DESTDIR)$(perlexecdir) &&\
 $(LN_S) -f globes_perl$(shrext_cmds) globes$(shrext_cmds) &&\
cd -

uninsthook += && cd $(DESTDIR)$(perlexecdir) &&\
	rm -f globes$(shrext_cmds) && cd -

endif

install-exec-hook:
	$(insthook)

uninstall-hook:
	$(uninsthook)