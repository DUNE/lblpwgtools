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
#AnaBinning="prism_default"
AnaBinning="fine_prism"
#AvaVar="EnuReco"
AvaVar="EnuRecoFDExtrapPred"

#FD
#MakePRISMPredInterps -o ${OUTPUTDIR}/FDState_OnlyNu_FHC_EnuReco_visEtrue_NoSysts_NDCAFsUpdated_SameBinningInFDErecAsNDErec_FDEffVsFDErec_ForTrainWithOscSpectrumi_CorrectOABinning_FDDataNoSelectionCuts.root \
#    -F-nu ${INPUTDIR}/OffAxisCAFs/FD_FHC_nonswap.root \
#    -Fe-nu ${INPUTDIR}/OffAxisCAFs/FD_FHC_nueswap.root \
#    -Ft-nu ${INPUTDIR}/OffAxisCAFs/FD_FHC_tauswap.root \
#   --bin-descriptor ${AnaBinning} \
#   --syst-descriptor ${ListOSysts} \
#    -A ${AvaVar} \
#    --PRISM-fake-data ${FakeData} \
#    --no-fakedata-dials \
#    --UseSelection \
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
#MakePRISMPredInterps -o ${OUTPUTDIR}/ND_FHC._EnuRecoFDExtrapPred_VisEtrue_NoSysts_CAFSFromAlexCorrected_AllEvents_sameBinsNDErecAndFDErec_noCVNcutPred.root \
#    -N-nu "/pnfs/dune/persistent/users/awilkins/ND_CAF_fdrecopreds/chasnip-NDCAF_OnAxisHadd-FHC_fdrecopreds/CAFv7_*.root" \
MakePRISMPredInterps -o ${OUTPUTDIR}/ND_FHC._EnuRecoFDExtrapPred_VisEtrue_NoSysts_NewMuonPairedData_nolognorm_nocausalnearmask_resample-oscEv.root \
    -N-nu "/pnfs/dune/persistent/users/awilkins/ND_CAF_fdrecopreds/chasnip-NDCAF_OnAxisHadd-FHC_fdrecopreds_muresim_nolognorm_nocausalnearmask_resample-oscEv/CAFv7*" \
    --bin-descriptor ${AnaBinning} \
    --syst-descriptor ${ListOSysts} \
    --no-fakedata-dials \
    -A ${AvaVar} \
    --PRISM-fake-data ${FakeData} \
    --UseSelection \
    #-n 10000


#ND nub
#MakePRISMPredInterps -o ${OUTPUTDIR}/ND_RHC_EnuReco_VisETrue_NoSysts_StandardTestSelection_1E4Events.root \
#    -N-nub "${NDRHCPNFSDIR}/CAFv7_*.root" \
#    --bin-descriptor ${AnaBinning} \
#    --syst-descriptor ${ListOSysts} \
#    --no-fakedata-dials \
#    -A ${AvaVar} \
#    --PRISM-fake-data ${FakeData} \
#    --UseSelection \
#    -n 10000
