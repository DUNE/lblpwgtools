#
# $Id: GNUmakefile,v 1.7 2012-08-07 16:06:50 brebel Exp $
#
# jpaley@indiana.edu
#

include SoftRelTools/arch_spec_root.mk

LIB_TYPE    := shared
LIB         := lib$(PACKAGE)
LIBCXXFILES := $(wildcard *.cxx)

LIBLINK    := -L$(SRT_PRIVATE_CONTEXT)/lib/$(SRT_SUBDIR) -L$(SRT_PUBLIC_CONTEXT)/lib/$(SRT_SUBDIR) -l$(PACKAGE) 

########################################################################
include SoftRelTools/standard.mk
#include SoftRelTools/arch_spec_art.mk



