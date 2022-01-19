#!/bin/bash

INPUTDIR=/dune/data/users/chasnip
OUTPUTDIR=/dune/data/users/chasnip
NDFHCPNFSDIR=/pnfs/dune/persistent/users/abooth/Production/ND_CAFMaker/nd_offaxis/v7/CAF/Hadded/subsets/FHC
NDRHCPNFSDIR=/pnfs/dune/persistent/users/abooth/Production/ND_CAFMaker/nd_offaxis/v7/CAF/Hadded/subsets/RHC

#ListOSysts="allsyst"
#ListOSysts="noxsec:nodet"
#ListOSysts="noflux:nodet"
ListOSysts="noxsec:noflux"
FakeData="nominal"
AnaBinning="default"
AvaVar="EVisReco"

#FD
#MakePRISMPredInterps -o PRISMState_EVisReco_3.5YrFull_FDOnly.root \
#    -F-nu ${INPUTDIR}/OffAxisCAFs/FD_FHC_nonswap.root \
#    -F-nub ${INPUTDIR}/OffAxisCAFs/FD_RHC_nonswap.root \
#    -Fe-nu ${INPUTDIR}/OffAxisCAFs/FD_FHC_nueswap.root \
#    -Fe-nub ${INPUTDIR}/OffAxisCAFs/FD_RHC_nueswap.root \
#    -Ft-nu ${INPUTDIR}/OffAxisCAFs/FD_FHC_tauswap.root \
#    -Ft-nub ${INPUTDIR}/OffAxisCAFs/FD_RHC_tauswap.root \
#    --bin-descriptor ${AnaBinning} \
#    --syst-descriptor ${ListOSysts} \
#    --no-fakedata-dials \
#    -A ${AvaVar} \
#    --PRISM-fake-data ${FakeData} \
#    --UseSelection \
#    -n 10000

#ND nu
MakePRISMPredInterps -o ${OUTPUTDIR}/PRISMState_EVisReco_3.5YrFull_0m_det_ND_nuOnly.root \
    -N-nu "${NDFHCPNFSDIR}/CAFv7_*0m*.root" \
    --bin-descriptor ${AnaBinning} \
    --syst-descriptor ${ListOSysts} \
    --no-fakedata-dials \
    -A ${AvaVar} \
    --PRISM-fake-data ${FakeData} \
    --UseSelection \
    #-n 10000

#ND nub
#MakePRISMPredInterps -o PRISMState_EVisReco_3.5YrFull_ND_nubOnly.root \
#    -N-nub "${NDRHCPNFSDIR}/CAFv7_*.root" \
#    --bin-descriptor ${AnaBinning} \
#    --syst-descriptor ${ListOSysts} \
#    --no-fakedata-dials \
#    -A ${AvaVar} \
#    --PRISM-fake-data ${FakeData} \
#    --UseSelection \
#    -n 10000

