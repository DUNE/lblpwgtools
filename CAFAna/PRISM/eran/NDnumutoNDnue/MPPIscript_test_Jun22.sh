#!/bin/bash

INPUTDIR=/dune/data/users/erea

LOS="nosyst"

#MakePRISMPredInterps -n 1000 -o /dune/data/users/erea/PRISMPredInterps/v7smallMC_NDnumutoNDnue_withsel_06172021.root \
MakePRISMPredInterps -o /dune/data/users/erea/PRISMPredInterps/v7fullMC_NDnumutoNDnue_test_Jun22.root \
	-N-nu ${INPUTDIR}/CombinedNDCAFs/CAFv7Full_3E20OnAxis_PRISM_Feb21_FHC.root \
	-F-nu ${INPUTDIR}/FDCAFs/FD_FHC_nonswap.root \
	-Fe-nu ${INPUTDIR}/FDCAFs/FD_FHC_nueswap.root \
	--bin-descriptor uniform --syst-descriptor "nosyst" --no-fakedata-dials \
	-A ETrue --UseSelection 
