#!/bin/bash

INPUTDIR=/dune/data/users/chasnip

NDFHCPNFSDIR=/pnfs/dune/persistent/users/abooth/Production/ND_CAFMaker/nd_offaxis/v7/CAF/Hadded/subsets/FHC

NDRHCPNFSDIR=/pnfs/dune/persistent/users/abooth/Production/ND_CAFMaker/nd_offaxis/v7/CAF/Hadded/subsets/RHC

#LOS="nosyst"

#LOS="list:MaCCQE"

CAFv7smallNOabs="CAFv7small_PRISM_Feb2021_FHC.root"

CAFv7smallabs="CAFv7small_PRISM_absx_Feb2021_FHC.root"

CAFv7Fullabs="CAFv7Full_6E20OnAxis_absxPOT_Apr21_FHC.root"

CAFv7RHC="ND_RHC_absxPOT_Sep21.root"

CAFND_FHC_35Yr="CAFv7_*subset00.root"

CAFND_RHC_35Yr="CAFv7_*.root"

FakeData="nominal"

#FakeData="MaCCQE_pos"

#FakeData="ChargedHadTruthUncorrND_neg"

#FakeData="ETruthScaleMuLArFD_neg"

#PRISMState_Fullv7Prod_EVisRec_9May21.root

gdb --args MakePRISMPredInterps -n 1000 -o /dune/data/users/erea/PRISMPredInterps/v7fullMC_NDnumutoNDnue_220727_1000.root \
    -N-nu "${NDFHCPNFSDIR}/${CAFND_FHC_35Yr}" \
    -F-nu ${INPUTDIR}/OffAxisCAFs/FD_FHC_nonswap.root \
    -Fe-nu ${INPUTDIR}/OffAxisCAFs/FD_FHC_nueswap.root \
    -Ft-nu ${INPUTDIR}/OffAxisCAFs/FD_FHC_tauswap.root \
    --bin-descriptor prism_default --syst-descriptor "nosyst" --no-fakedata-dials \
    -A EVisReco --PRISM-fake-data ${FakeData} --UseSelection 
