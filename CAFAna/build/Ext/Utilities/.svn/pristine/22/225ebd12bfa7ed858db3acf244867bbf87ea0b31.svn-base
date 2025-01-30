PACKAGE     := UtilitiesFunc
LIB_TYPE    := shared
LIB         := lib$(PACKAGE)
LIBCXXFILES := $(wildcard *.cxx)

override CXXFLAGS += -I$(CLHEP_INC) -I$(STAN_INC) -I$(STAN_MATH_INC) -I$(EIGEN_INC) -I$(CVODES_INC) -I$(BOOST_INC)
override CCFLAGS  += -I$(CLHEP_INC)

########################################################################
include SoftRelTools/standard.mk
include SoftRelTools/arch_spec_mf.mk
include SoftRelTools/arch_spec_root.mk
