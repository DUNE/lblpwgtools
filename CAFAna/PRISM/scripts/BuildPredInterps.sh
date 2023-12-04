#!/bin/bash

INPUTDIR=/dune/data/users/chasnip
OUTPUTDIR=/dune/data/users/chasnip/StateFiles_2023

NDFHCPNFSDIR=/pnfs/dune/persistent/users/chasnip/NDCAF_OnAxisHadd/FHC
NDRHCPNFSDIR=/pnfs/dune/persistent/users/chasnip/NDCAF_OnAxisHadd/RHC

# NDFHCPNFSDIR=/root/work/software/lblpwgtools/CAFAna/inputs/ndcafs/FHC
# NDRHCPNFSDIR=/root/work/software/lblpwgtools/CAFAna/inputs/ndcafs/RHC

#ListOSysts="allsyst"
#ListOSysts="noxsec:nodet"
#ListOSysts="noflux:nodet"
#ListOSysts="noxsec:noflux"
#ListOSysts="nosyst"
#ListOSysts="nov17flux:nodet:noxsec"
#ListOSysts="list:MissingProtonFakeData"
#ListOSysts="list:flux_Nov17_5"
#ListOSysts="list:CorrMuSqrt:RecoEnergyScaleND"
ListOSysts="list:CorrHadSqrt:CorrHadInvSqrt:CorrMuSqrt:CorrMuInvSqrt:CorrEMSqrt:CorrEMInvSqrt:EScaleMuLAr:ChargedHadCorr:EMCorr:RecoEnergyScaleND:RecoEnergySqrtND:RecoEnergyInvSqrtND:MuonRecoResND:EMRecoResND:ChargedHadRecoResND"
FakeData="MissingProtonFakeData_pos"
AnaBinning="prism_default"
AvaVar="EnuReco"

#FD
#MakePRISMPredInterps -o ${OUTPUTDIR}/FDSystStateFiles_NoPionMass_EVisReco_Feb23/PRISMState_EVisReco_NoPionMass_XSecSyst_RHC_FDOnly_9Feb23.root \
#    -F-nub ${INPUTDIR}/OffAxisCAFs/FD_RHC_nonswap.root \
#    -Fe-nub ${INPUTDIR}/OffAxisCAFs/FD_RHC_nueswap.root \
#    -Ft-nub ${INPUTDIR}/OffAxisCAFs/FD_RHC_tauswap.root \
#    --bin-descriptor ${AnaBinning} \
#    --syst-descriptor ${ListOSysts} \
#    -A ${AvaVar} \
#    --PRISM-fake-data ${FakeData} \
#    --no-fakedata-dials \
#    --UseSelection \
    #-n 10000

#ND nu
MakePRISMPredInterps -o ${OUTPUTDIR}/PRISMState_EnuReco_CorrDet_ND_FHC_Feb23.root \
    -N-nu "${NDFHCPNFSDIR}/CAFv7_*.root" \
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
