#!/bin/bash

INPUTDIR=/exp/dune/data/users/icaracas/CAFFiles/FDCAFs
OUTPUTDIR=/exp/dune/data/users/icaracas/StateFiles/NDStateFiles
#OUTPUTDIR=/dune/data/users/icaracas/StateFiles/NDStateFiles/NewFluxSep21Etrue/AllFluxPars

NDFHCPNFSDIR=/pnfs/dune/persistent/users/chasnip/NDCAF_OnAxisHadd/FHC
NDRHCPNFSDIR=/pnfs/dune/persistent/users/chasnip/NDCAF_OnAxisHadd/RHC

# NDFHCPNFSDIR=/root/work/software/lblpwgtools/CAFAna/inputs/ndcafs/FHC
# NDRHCPNFSDIR=/root/work/software/lblpwgtools/CAFAna/inputs/ndcafs/RHC

#ListOSysts="allsyst"
#ListOSysts="noxsec:nodet"
#ListOSysts="noflux:nodet"
#ListOSysts="noxsec:noflux"
ListOSysts="nosyst"
#ListOSysts="sept21flux:noxsec:nodet"
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
#AnaBinning="fine_prism"
AvaVar="EnuReco"
#AvaVar="EnuRecoForGeoEffMatch"
#AvaVar="EnuRecoFDExtrapPred"
#AvaVar="ETrue"

#FD
#MakePRISMPredInterps -o ${OUTPUTDIR}/FDState_OnlyNu_RHC_EnuReco_NoSyts_TestNEWTokenPRISM.root \
#    -F-nub ${INPUTDIR}/OffAxisCAFs/FD_RHC_nonswap.root \
#    -Fe-nub ${INPUTDIR}/OffAxisCAFs/FD_RHC_nueswap.root \
#    -Ft-nub ${INPUTDIR}/OffAxisCAFs/FD_RHC_tauswap.root \
#   --bin-descriptor ${AnaBinning} \
#   --syst-descriptor ${ListOSysts} \
#    -A ${AvaVar} \
#    --PRISM-fake-data ${FakeData} \
#    --no-fakedata-dials \
#    --UseSelection \
#    #-n 10000

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
#MakePRISMPredInterps -o ${OUTPUTDIR}/ND_FHC_Etrue_NoSysts_sameBinsAsErec.root \
#    -N-nu "/pnfs/dune/persistent/users/chasnip/NDCAF_OnAxisHadd/FHC/CAFv7_*.root" \
#MakePRISMPredInterps -o ${OUTPUTDIR}/ND_FHC._EnuReco_VisEtrue_NoSysts_standardSameFDBinningasND.root \
#    -N-nu "/pnfs/dune/persistent/users/awilkins/ND_CAF_fdrecopreds/chasnip-NDCAF_OnAxisHadd-FHC_fdrecopreds_muresim_nolognorm_nocausalnearmask_seed38/CAFv7*" \
MakePRISMPredInterps -o ${OUTPUTDIR}/TestNewToken_NoSysts_8m.root \
    -N-nu "/pnfs/dune/persistent/users/chasnip/NDCAF_OnAxisHadd/FHC//CAFv7_CM_8m_subset09.root" \
    --bin-descriptor ${AnaBinning} \
    --syst-descriptor ${ListOSysts} \
    --no-fakedata-dials \
    -A ${AvaVar} \
    --PRISM-fake-data ${FakeData} \
    --UseSelection \
    #-n 10000


#ND nub
#MakePRISMPredInterps -o ${OUTPUTDIR}/ND_RHC_EnuReco_VisETrue_NoSysts_28p5m.root \
#    -N-nub "${NDRHCPNFSDIR}/*_28.5m*.root" \
#    --bin-descriptor ${AnaBinning} \
#    --syst-descriptor ${ListOSysts} \
#    --no-fakedata-dials \
#    -A ${AvaVar} \
#    --PRISM-fake-data ${FakeData} \
#    --UseSelection \
#    #-n 10000
