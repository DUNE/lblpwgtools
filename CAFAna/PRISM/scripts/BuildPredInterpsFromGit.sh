#!/bin/bash

INPUTDIR=/dune/data/users/chasnip
OUTPUTDIR=/dune/data/users/icaracas/StateFiles/FDStateFiles
#OUTPUTDIR=/dune/data/users/icaracas/StateFiles/NDStateFiles/NewFluxSep21Etrue/AllFluxPars

NDFHCPNFSDIR=/pnfs/dune/persistent/users/chasnip/NDCAF_OnAxisHadd/FHC
NDRHCPNFSDIR=/pnfs/dune/persistent/users/chasnip/NDCAF_OnAxisHadd/RHC

# NDFHCPNFSDIR=/root/work/software/lblpwgtools/CAFAna/inputs/ndcafs/FHC
# NDRHCPNFSDIR=/root/work/software/lblpwgtools/CAFAna/inputs/ndcafs/RHC

#ListOSysts="allsyst"
#ListOSysts="noxsec:nodet"
#ListOSysts="noflux:nodet"
#ListOSysts="noxsec:noflux"
#ListOSysts="nosyst"
ListOSysts="sept21flux:noxsec:nodet"
#ListOSysts="nov17flux:noxsec:nodet"
#ListOSysts="nov17flux:nodet:noxsec"
#ListOSysts="list:MissingProtonFakeData"
#ListOfSyst="noxsec:nodet"
#ListOSysts="list:flux_Nov17_5"
#ListOSysts="list:CorrMuSqrt:RecoEnergyScaleND"
#ListOSysts="list:ShiftTotalXSecByWeight"
#ListOSysts="list:FrInel_pi"
#ListOSysts="list:MaCCQE"
#ListOSysts="list:CorrHadSqrt:CorrHadInvSqrt:CorrMuSqrt:CorrMuInvSqrt:CorrEMSqrt:CorrEMInvSqrt:EScaleMuLAr:ChargedHadCorr:EMCorr:RecoEnergyScaleND:RecoEnergySqrtND:RecoEnergyInvSqrtND:MuonRecoResND:EMRecoResND:ChargedHadRecoResND"
FakeData="MissingProtonFakeData_pos"
AnaBinning="prism_default"
AvaVar="EnuReco"

#FD
MakePRISMPredInterps -o ${OUTPUTDIR}/FDState_OnlyNu_FHC_EnuReco_NewSep21_FluxSysts_AllFocusingParams.root \
    -F-nu ${INPUTDIR}/OffAxisCAFs/FD_FHC_nonswap.root \
    -Fe-nu ${INPUTDIR}/OffAxisCAFs/FD_FHC_nueswap.root \
    -Ft-nu ${INPUTDIR}/OffAxisCAFs/FD_FHC_tauswap.root \
   --bin-descriptor ${AnaBinning} \
   --syst-descriptor ${ListOSysts} \
    -A ${AvaVar} \
    --PRISM-fake-data ${FakeData} \
    --no-fakedata-dials \
    --UseSelection \
    #-n 10000

#ND nu example for making state file with CAF files from "TempListWithInputFilesForStateFiles.txt"
#MakePRISMPredInterps -o ${OUTPUTDIR}/ND_StateTestFromInputList.root \
#    -N-nu "TempListWithInputFilesForStateFiles.txt" \
#    --bin-descriptor ${AnaBinning} \
#    --syst-descriptor ${ListOSysts} \
#    --no-fakedata-dials \
#    -A ${AvaVar} \
#    --PRISM-fake-data ${FakeData} \
#    --UseSelection \
    #-n 10000

#ND nu
#MakePRISMPredInterps -o ${OUTPUTDIR}/ND_FHC.TESTNDState.70741375.154.root \
#    -N-nu "${NDFHCPNFSDIR}/CAFv7_*.root" \
#    --bin-descriptor ${AnaBinning} \
#    --syst-descriptor ${ListOSysts} \
#    --no-fakedata-dials \
#    -A ${AvaVar} \
#    --PRISM-fake-data ${FakeData} \
#    --UseSelection \
    #-n 10000


#ND nub
#MakePRISMPredInterps -o ${OUTPUTDIR}/ND_RHC.TESTNDState.2797757.668.root \
#    -N-nub "${NDRHCPNFSDIR}/CAFv7_*.root" \
#    --bin-descriptor ${AnaBinning} \
#    --syst-descriptor ${ListOSysts} \
#    --no-fakedata-dials \
#    -A ${AvaVar} \
#    --PRISM-fake-data ${FakeData} \
#    --UseSelection \
#    -n 10000
