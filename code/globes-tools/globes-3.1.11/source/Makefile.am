INCLUDES = -I$(top_builddir) -I$(top_srcdir) -I$(top_srcdir)/lib $(GSLINC) 

AM_YFLAGS =  -d 

lib_LTLIBRARIES = libglobes.la
BUILT_SOURCES = glb_parser.h glb_parser.c
libglobes_la_SOURCES =  glb_minimize.c glb_error.c glb_path.c glb_multiex.c \
 glb_version.c glb_rate_engine.c  glb_parser.y glb_min_sup.c glb_fluxes.c \
 glb_smear.c glb_lexer.l glb_probability.c glb_profile.c glb_sys.c glb_wrapper.c \
 glb_minimize.h glb_error.h glb_path.h glb_parser_addons.h glb_multiex.h \
 glb_probability.h glb_smear.h glb_rate_engine.h glb_types.h \
 glb_min_sup.h  glb_version.h glb_lexer.h glb_fluxes.h glb_parser_type.h \
 glb_wrapper.h glb_profile.h glb_sys.h glb_glb.c glb_prior.h glb_prior.c glb_compat.c

libglobes_la_LDFLAGS = @LIBS@ -version-info @GLBLIBVERSION@
libglobes_la_CPPFLAGS = -DGLB_MODULE_DIR=\"$(pkglibdir)\"
libglobes_la_DEPENDENCIES =
libglobes_la_LDFLAGS += $(top_builddir)/lib/libgnu.la

bin_PROGRAMS = globes globes-accuracy globes-deltatheta globes-discovery\
 globes-simple

globes_SOURCES = globes.c 
globes_LDADD = libglobes.la @LIBS@

globes_accuracy_SOURCES = glb_tools_parser.h glb_tools_parser.c \
glb_tools_header.h  glb_tools_header.c glb_tools_eightfold.h \
glb_tools_eightfold.c globes-accuracy.c

globes_deltatheta_SOURCES =  glb_tools_parser.h  glb_tools_parser.c \
glb_tools_header.h  glb_tools_header.c globes-deltatheta.c

globes_discovery_SOURCES =  glb_tools_parser.h  glb_tools_parser.c \
glb_tools_header.h  glb_tools_header.c glb_tools_eightfold.h \
glb_tools_eightfold.c globes-discovery.c

globes_simple_SOURCES =  glb_tools_parser.h  glb_tools_parser.c \
glb_tools_header.h  glb_tools_header.c glb_tools_eightfold.h globes-simple.c

globes_deltatheta_LDADD =   libglobes.la @LIBS@ 
globes_accuracy_LDADD =   libglobes.la @LIBS@ 
globes_discovery_LDADD =   libglobes.la @LIBS@ 
globes_simple_LDADD =   libglobes.la @LIBS@ 
