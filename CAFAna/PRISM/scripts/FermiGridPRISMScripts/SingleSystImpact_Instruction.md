# Instruction for Study Single Systematic Impact on DUNE-PRISM Oscillation Analysis


## Install CAFAna Framework

[First time only]

```
# Set up from DUNE FNAL machines (dunegpvm*)
kinit -f weishi@FNAL.GOV                 # Use your FNAL kerberos password
ssh -X weishi@dunegpvm03.fnal.gov        
cd /dune/app/users/weishi
mkdir PRISMAnalysis
cd PRISMAnalysis
git clone https://github.com/weishi10141993/lblpwgtools.git
cd lblpwgtools
git checkout feature/PRISM               # NOTE: This branch is not used anymore after Jan 31, 2022
cd CAFAna
# Build the code, the -u option rely on relevant dependencies from FNAL scisoft
./standalone_configure_and_build.sh --use-PRISM -u --rdb
# To recompile: ./standalone_configure_and_build.sh --use-PRISM -u --rdb -f
# May need to set export GSL_LIB=/usr/lib64
```

## Study systematic impacts on DUNE PRISM event rates

1. First produce the state files with systematics.

```
cd /dune/app/users/weishi/PRISMAnalysis/lblpwgtools/CAFAna
source build/Linux/CAFAnaEnv.sh                                 # set up the environment
# Ignore the error of can't find the example directory
```

The full list of systematics are,

```
# Xsec systs (56 in tot)
list:MaCCQE:VecFFCCQEshape:CCQEPauliSupViaKF:MaCCRES:MvCCRES:Theta_Delta2Npi:AhtBY:BhtBY:CV1uBY:CV2uBY:MaNCRES:MvNCRES:FrCEx_N:FrElas_N:FrInel_N:FrAbs_N:FrPiProd_N:FrCEx_pi:FrElas_pi:FrInel_pi:FrAbs_pi:FrPiProd_pi:BeRPA_A:BeRPA_B:BeRPA_D:C12ToAr40_2p2hScaling_nu:C12ToAr40_2p2hScaling_nubar:E2p2h_A_nu:E2p2h_B_nu:E2p2h_A_nubar:E2p2h_B_nubar:NR_nu_np_CC_1Pi:NR_nu_n_CC_2Pi:NR_nu_n_CC_3Pi:NR_nu_p_CC_2Pi:NR_nu_p_CC_3Pi:NR_nu_n_NC_1Pi:NR_nu_n_NC_2Pi:NR_nu_n_NC_3Pi:NR_nu_p_NC_1Pi:NR_nu_p_NC_2Pi:NR_nu_p_NC_3Pi:NR_nubar_n_CC_1Pi:NR_nubar_n_CC_2Pi:NR_nubar_n_CC_3Pi:NR_nubar_p_CC_1Pi:NR_nubar_p_CC_2Pi:NR_nubar_p_CC_3Pi:NR_nubar_n_NC_1Pi:NR_nubar_n_NC_2Pi:NR_nubar_n_NC_3Pi:NR_nubar_p_NC_1Pi:NR_nubar_p_NC_2Pi:NR_nubar_p_NC_3Pi:nuenumu_xsec_ratio:nuenuebar_xsec_ratio

# Flux systs (28 in tot, there are more flux PCA systs, but now just use first 15 PCA components: 13-27)
list:flux_Nov17_0:flux_Nov17_1:flux_Nov17_2:flux_Nov17_3:flux_Nov17_4:flux_Nov17_5:flux_Nov17_6:flux_Nov17_7:flux_Nov17_8:flux_Nov17_9:flux_Nov17_10:flux_Nov17_11:flux_Nov17_12:flux_Nov17_13:flux_Nov17_14:flux_Nov17_15:flux_Nov17_16:flux_Nov17_17:flux_Nov17_18:flux_Nov17_19:flux_Nov17_20:flux_Nov17_21:flux_Nov17_22:flux_Nov17_23:flux_Nov17_24:flux_Nov17_25:flux_Nov17_26:flux_Nov17_27

# Detector systs (39 in tot)
(!!! These 6 are not working for now !!!) list:FDRecoNumuSyst:FDRecoNueSyst:FVNumuFD:FVNueFD:RecoNCSyst:FVNumuND
list:RecoEnergyScaleND:RecoEnergySqrtND:RecoEnergyInvSqrtND:EMRecoUncorrND:EMRecoUncorrSqrtND:EMRecoUncorrInvSqrtND:ChargedHadRecoUncorrND:ChargedHadRecoUncorrSqrtND:ChargedHadRecoUncorrInvSqrtND:ERecoScaleMuLArND:ERecoScaleMuLArSqrtND:ERecoScaleMuLArInvSqrtND:MuonRecoResND:EMRecoResND:ChargedHadRecoResND
list:RecoEnergyScaleFD:RecoEnergySqrtFD:RecoEnergyInvSqrtFD:EMRecoUncorrFD:EMRecoUncorrSqrtFD:EMRecoUncorrInvSqrtFD:ChargedHadRecoUncorrFD:ChargedHadRecoUncorrSqrtFD:ChargedHadRecoUncorrInvSqrtFD:ERecoScaleMuLArFD:ERecoScaleMuLArSqrtFD:ERecoScaleMuLArInvSqrtFD:MuonRecoResFD:EMRecoResFD:ChargedHadRecoResFD
list:NuOnECCBkgSyst:NuOnENCBkgSyst:NuOnERecoEffSyst
```

To run interactively with flux + cross section systematics,

```
cd PRISM/app
# For usage: MakePRISMPredInterps --help

# Run w/ 3.5 yr ND MC
MakePRISMPredInterps -o hadd_state_file_xsec_49_to_54.root -N-nu "/pnfs/dune/persistent/users/abooth/Production/ND_CAFMaker/nd_offaxis/v7/CAF/Hadded/subsets/FHC/*.root" -F-nu /dune/data/users/chasnip/OffAxisCAFs/FD_FHC_nonswap.root -Fe-nu /dune/data/users/chasnip/OffAxisCAFs/FD_FHC_nueswap.root -Ft-nu /dune/data/users/chasnip/OffAxisCAFs/FD_FHC_tauswap.root -N-nub "/pnfs/dune/persistent/users/abooth/Production/ND_CAFMaker/nd_offaxis/v7/CAF/Hadded/subsets/RHC/*.root" -F-nub /dune/data/users/chasnip/OffAxisCAFs/FD_RHC_nonswap.root -Fe-nub /dune/data/users/chasnip/OffAxisCAFs/FD_RHC_nueswap.root -Ft-nub /dune/data/users/chasnip/OffAxisCAFs/FD_RHC_tauswap.root --bin-descriptor default --no-fakedata-dials -A EVisReco --UseSelection --syst-descriptor list:NR_nubar_n_NC_1Pi:NR_nubar_n_NC_2Pi:NR_nubar_n_NC_3Pi:NR_nubar_p_NC_1Pi:NR_nubar_p_NC_2Pi:NR_nubar_p_NC_3Pi

# Can run in tmux sessions
# no syst: ~4hr
# 4 syst: ~16hr
# 9 syst: ~19hr

# 1-year ND MC
FHC: /pnfs/dune/persistent/users/chasnip/CAF_MC_FILES_4FLAVOUR/ND_FHC_v7Full_6E20OnAxis_absxPOT_Apr21.root
RHC: /dune/data/users/chasnip/CombinedNDCAFs/ND_RHC_absxPOT_Sep21.root

# 3.5-year ND MC
FHC: /pnfs/dune/persistent/users/abooth/Production/ND_CAFMaker/nd_offaxis/v7/CAF/Hadded/subsets/FHC/*.root
RHC: /pnfs/dune/persistent/users/abooth/Production/ND_CAFMaker/nd_offaxis/v7/CAF/Hadded/subsets/RHC/*.root

hadd_state_file_xsec_1_to_6.root list:MaCCQE:VecFFCCQEshape:CCQEPauliSupViaKF:MaCCRES:MvCCRES:Theta_Delta2Npi
hadd_state_file_xsec_7_to_12.root list:AhtBY:BhtBY:CV1uBY:CV2uBY:MaNCRES:MvNCRES
hadd_state_file_xsec_13_to_18.root list:FrCEx_N:FrElas_N:FrInel_N:FrAbs_N:FrPiProd_N:FrCEx_pi
hadd_state_file_xsec_19_to_24.root list:FrElas_pi:FrInel_pi:FrAbs_pi:FrPiProd_pi:BeRPA_A:BeRPA_B
hadd_state_file_xsec_25_to_30.root list:BeRPA_D:C12ToAr40_2p2hScaling_nu:C12ToAr40_2p2hScaling_nubar:E2p2h_A_nu:E2p2h_B_nu:E2p2h_A_nubar
hadd_state_file_xsec_31_to_36.root list:E2p2h_B_nubar:NR_nu_np_CC_1Pi:NR_nu_n_CC_2Pi:NR_nu_n_CC_3Pi:NR_nu_p_CC_2Pi:NR_nu_p_CC_3Pi
hadd_state_file_xsec_37_to_42.root list:NR_nu_n_NC_1Pi:NR_nu_n_NC_2Pi:NR_nu_n_NC_3Pi:NR_nu_p_NC_1Pi:NR_nu_p_NC_2Pi:NR_nu_p_NC_3Pi
hadd_state_file_xsec_43_to_48.root list:NR_nubar_n_CC_1Pi:NR_nubar_n_CC_2Pi:NR_nubar_n_CC_3Pi:NR_nubar_p_CC_1Pi:NR_nubar_p_CC_2Pi:NR_nubar_p_CC_3Pi
hadd_state_file_xsec_49_to_54.root list:NR_nubar_n_NC_1Pi:NR_nubar_n_NC_2Pi:NR_nubar_n_NC_3Pi:NR_nubar_p_NC_1Pi:NR_nubar_p_NC_2Pi:NR_nubar_p_NC_3Pi
hadd_state_file_xsec_55_to_56.root list:nuenumu_xsec_ratio:nuenuebar_xsec_ratio

hadd_state_file_flux_0_to_5.root list:flux_Nov17_0:flux_Nov17_1:flux_Nov17_2:flux_Nov17_3:flux_Nov17_4:flux_Nov17_5
hadd_state_file_flux_6_to_11.root list:flux_Nov17_6:flux_Nov17_7:flux_Nov17_8:flux_Nov17_9:flux_Nov17_10:flux_Nov17_11
hadd_state_file_flux_12_to_17.root list:flux_Nov17_12:flux_Nov17_13:flux_Nov17_14:flux_Nov17_15:flux_Nov17_16:flux_Nov17_17
hadd_state_file_flux_18_to_23.root list:flux_Nov17_18:flux_Nov17_19:flux_Nov17_20:flux_Nov17_21:flux_Nov17_22:flux_Nov17_23
hadd_state_file_flux_24_to_27.root list:flux_Nov17_24:flux_Nov17_25:flux_Nov17_26:flux_Nov17_27

(!!! NOT USABLE !!!) hadd_state_file_det_1_to_6.root list:FDRecoNumuSyst:FDRecoNueSyst:FVNumuFD:FVNueFD:RecoNCSyst:FVNumuND
hadd_state_file_det_7_to_12.root list:RecoEnergyScaleND:RecoEnergySqrtND:RecoEnergyInvSqrtND:EMRecoUncorrND:EMRecoUncorrSqrtND:EMRecoUncorrInvSqrtND
hadd_state_file_det_13_to_18.root list:ChargedHadRecoUncorrND:ChargedHadRecoUncorrSqrtND:ChargedHadRecoUncorrInvSqrtND:ERecoScaleMuLArND:ERecoScaleMuLArSqrtND:ERecoScaleMuLArInvSqrtND
hadd_state_file_det_19_to_24.root list:RecoEnergyScaleFD:RecoEnergySqrtFD:RecoEnergyInvSqrtFD:EMRecoUncorrFD:EMRecoUncorrSqrtFD:EMRecoUncorrInvSqrtFD
hadd_state_file_det_25_to_30.root list:ChargedHadRecoUncorrFD:ChargedHadRecoUncorrSqrtFD:ChargedHadRecoUncorrInvSqrtFD:ERecoScaleMuLArFD:ERecoScaleMuLArSqrtFD:ERecoScaleMuLArInvSqrtFD
hadd_state_file_det_31_to_36.root list:MuonRecoResND:EMRecoResND:ChargedHadRecoResND:MuonRecoResFD:EMRecoResFD:ChargedHadRecoResFD
hadd_state_file_det_37_to_39.root list:NuOnECCBkgSyst:NuOnENCBkgSyst:NuOnERecoEffSyst
```

2. Create fcl file for each systematic based on the template ```NuisanceSyst_Scan/Basic_PRISMPred_PlaceHolder.fcl```, then run the script below to run ```PRISMPrediction```,

```
cd /dune/app/users/weishi/PRISMAnalysis/lblpwgtools/CAFAna/PRISM/scripts/FermiGridPRISMScripts
chmod a+x PRISMPredVary1Syst.sh
./PRISMPredVary1Syst.sh
```

Now to apply the systematic shift to one specific step in the analysis, change ```(fVaryNDFDMCData ? shift : kNoShift))``` in following lines to ```kNoShift``` in ```PRISM/PredictionPRISM.cxx```,
```
L671/683: ND NC Bkg MC
L696/707: ND WL Bkg MC
L720/731: ND WS Bkg MC
L759: Calculate LC coefficient
L835/841: numu nue xsec correction
```
and change ```shift``` in following lines to ```kNoShift``` in ```PRISM/PRISMDetectorExtrapolation.cxx```,
```
L190: ND Unfold
L192: FD Smear
```
and change ```syst``` in following lines to ```kNoShift``` in ```PRISM/PRISMMCEffCorrection.cxx```,
```
L87/89/111/113: ND Eff corr
L127/130: FD Eff corr
```

I temporarily copy the changed files from another repo (no automatic script to do this in case there are future code updates).

Then run the script below (it only shifts each step above by changing ```kNoShift``` to ```shift``` and recompile and re-run ```PRISMPrediction```),

```
chmod a+x PRISMPredShift1StepND.sh
./PRISMPredShift1StepND.sh
```

3. Draw and plot,

```
# Do the following for all channels: FD_nu_numu, FD_nu_nue, FD_nub_numu, FD_nub_nue

# This makes the sigma variation plots for various component in PRISM
chmod a+x PlotManySysts.sh
./PlotManySysts.sh

# This puts many plots on one canvas and prints tables
source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.24.02/x86_64-centos7-gcc48-opt/bin/thisroot.sh
root -l -b -q MergeCanvas.C
```
