PACKAGE     := OscLibFunc
LIB_TYPE    := shared
LIB         := lib$(PACKAGE)
LIBCXXFILES := $(wildcard *.cxx)

########################################################################
include SoftRelTools/standard.mk
include SoftRelTools/arch_spec_root.mk
include SoftRelTools/arch_spec_boost.mk

override CPPFLAGS  += -I$(GSL_INC)
override LDFLAGS   += -L$(GSL_LIB)
override LOADLIBES += -lgsl -lgslcblas
override LIBLIBS += -L$(GSL_LIB) -lgsl -lgslcblas
