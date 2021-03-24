#!/bin/bash

INPUTDIR=/dune/data/users/erea

LOS="nosyst"

gdb --args MakePRISMPredInterps -o /dune/data/users/erea/PRISMPredInterps/v7smallMC_NDnumutoNDnue_withsel_seltest.root \
	-N-nu ${INPUTDIR}/CombinedNDCAFs/CAFv7small_PRISM_Feb2021_FHC.root \
	-F-nu ${INPUTDIR}/FDCAFs/FD_FHC_nonswap.root \
	-Fe-nu ${INPUTDIR}/FDCAFs/FD_FHC_nueswap.root \
	--bin-descriptor uniform --syst-descriptor "nosyst" --no-fakedata-dials \
	-A EProxy --UseSelection 
