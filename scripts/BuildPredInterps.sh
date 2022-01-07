#!/bin/bash

INPUTDIR=/dune/data/users/chasnip
NDFHCPNFSDIR=/pnfs/dune/persistent/users/abooth/Production/ND_CAFMaker/nd_offaxis/v7/CAF/Hadded/subsets/FHC
NDRHCPNFSDIR=/pnfs/dune/persistent/users/abooth/Production/ND_CAFMaker/nd_offaxis/v7/CAF/Hadded/subsets/RHC

ListOSysts="allsyst"
FakeData="nominal"

MakePRISMPredInterps -o PRISMState_EVisReco_3.5YrFull_5Jan22.root \
    -N-nu "${NDFHCPNFSDIR}/CAFv7_*.root" \
    -N-nub "${NDRHCPNFSDIR}/CAFv7_*.root" \
    -F-nu ${INPUTDIR}/OffAxisCAFs/FD_FHC_nonswap.root \
    -F-nub ${INPUTDIR}/OffAxisCAFs/FD_RHC_nonswap.root \
    -Fe-nu ${INPUTDIR}/OffAxisCAFs/FD_FHC_nueswap.root \
    -Fe-nub ${INPUTDIR}/OffAxisCAFs/FD_RHC_nueswap.root \
    -Ft-nu ${INPUTDIR}/OffAxisCAFs/FD_FHC_tauswap.root \
    -Ft-nub ${INPUTDIR}/OffAxisCAFs/FD_RHC_tauswap.root \
    --bin-descriptor default \
    --syst-descriptor ${ListOSysts} \
    --no-fakedata-dials \
    -A EVisReco \
    --PRISM-fake-data ${FakeData} \
    --UseSelection \
    -n 10000

