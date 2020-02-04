include SoftRelTools/arch_spec_root.mk

PACKAGE     := Utilities
ROOTCINT    := yes
LIB_TYPE    := shared
LIB         := lib$(PACKAGE)
LIBCXXFILES := $(wildcard *.cxx)
JOBFILES    := $(wildcard *.fcl)
SUBDIRS     := func strip batch
BINS        := file_size_ana sam_evd plot_timetracker compare-hdf5-caf
SCRIPTS     := file_size_ana sam_evd plot_timetracker compare-hdf5-caf

LIBLINK    := -L$(SRT_PRIVATE_CONTEXT)/lib/$(SRT_SUBDIR) -L$(SRT_PUBLIC_CONTEXT)/lib/$(SRT_SUBDIR) -l$(PACKAGE) -l$(PACKAGE)Func -lCMap_service

########################################################################
include SoftRelTools/standard.mk
include SoftRelTools/arch_spec_art.mk
include SoftRelTools/arch_spec_novadaq.mk
include SoftRelTools/arch_spec_ifdhart.mk
include SoftRelTools/arch_spec_ifdhc.mk
include SoftRelTools/arch_spec_ifbeam.mk

override LIBLIBS += $(LOADLIBES) -L$(ART_LIB) -lart_Framework_Services_Optional_TFileService_service -L$(SRT_PRIVATE_CONTEXT)/lib/$(SRT_SUBDIR) -L$(SRT_PUBLIC_CONTEXT)/lib/$(SRT_SUBDIR) -lNovaTimingUtilities -lCMap_service
