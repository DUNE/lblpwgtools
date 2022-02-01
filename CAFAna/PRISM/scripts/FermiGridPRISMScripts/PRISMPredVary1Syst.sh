#!/usr/bin/expect -f

set timeout 86400
#set systs [list MaCCQE VecFFCCQEshape CCQEPauliSupViaKF MaCCRES MvCCRES Theta_Delta2Npi]
#set systs [list AhtBY BhtBY CV1uBY CV2uBY MaNCRES MvNCRES]
#set systs [list FrCEx_N FrElas_N FrInel_N FrAbs_N FrPiProd_N FrCEx_pi]
#set systs [list FrElas_pi FrInel_pi FrAbs_pi FrPiProd_pi BeRPA_A BeRPA_B]
#set systs [list BeRPA_D C12ToAr40_2p2hScaling_nu C12ToAr40_2p2hScaling_nubar E2p2h_A_nu E2p2h_B_nu E2p2h_A_nubar]
#set systs [list E2p2h_B_nubar NR_nu_np_CC_1Pi NR_nu_n_CC_2Pi NR_nu_n_CC_3Pi NR_nu_p_CC_2Pi NR_nu_p_CC_3Pi]
#set systs [list NR_nu_n_NC_1Pi NR_nu_n_NC_2Pi NR_nu_n_NC_3Pi NR_nu_p_NC_1Pi NR_nu_p_NC_2Pi NR_nu_p_NC_3Pi]
#set systs [list NR_nubar_n_CC_1Pi NR_nubar_n_CC_2Pi NR_nubar_n_CC_3Pi NR_nubar_p_CC_1Pi NR_nubar_p_CC_2Pi NR_nubar_p_CC_3Pi]
#set systs [list NR_nubar_n_NC_1Pi NR_nubar_n_NC_2Pi NR_nubar_n_NC_3Pi NR_nubar_p_NC_1Pi NR_nubar_p_NC_2Pi NR_nubar_p_NC_3Pi]
#set systs [list nuenumu_xsec_ratio nuenuebar_xsec_ratio]
#set systs [list flux_Nov17_0 flux_Nov17_1 flux_Nov17_2 flux_Nov17_3 flux_Nov17_4 flux_Nov17_5]
#set systs [list flux_Nov17_6 flux_Nov17_7 flux_Nov17_8 flux_Nov17_9 flux_Nov17_10 flux_Nov17_11]
#set systs [list flux_Nov17_12 flux_Nov17_13 flux_Nov17_14 flux_Nov17_15 flux_Nov17_16 flux_Nov17_17]
#set systs [list flux_Nov17_18 flux_Nov17_19 flux_Nov17_20 flux_Nov17_21 flux_Nov17_22 flux_Nov17_23]
#set systs [list flux_Nov17_24 flux_Nov17_25 flux_Nov17_26 flux_Nov17_27]
#set systs [list RecoEnergyScaleND RecoEnergySqrtND RecoEnergyInvSqrtND EMRecoUncorrND EMRecoUncorrSqrtND EMRecoUncorrInvSqrtND]
#set systs [list ChargedHadRecoUncorrND ChargedHadRecoUncorrSqrtND ChargedHadRecoUncorrInvSqrtND ERecoScaleMuLArND ERecoScaleMuLArSqrtND ERecoScaleMuLArInvSqrtND]
#set systs [list RecoEnergyScaleFD RecoEnergySqrtFD RecoEnergyInvSqrtFD EMRecoUncorrFD EMRecoUncorrSqrtFD EMRecoUncorrInvSqrtFD]
#set systs [list ChargedHadRecoUncorrFD ChargedHadRecoUncorrSqrtFD ChargedHadRecoUncorrInvSqrtFD ERecoScaleMuLArFD ERecoScaleMuLArSqrtFD ERecoScaleMuLArInvSqrtFD]
#set systs [list MuonRecoResND EMRecoResND ChargedHadRecoResND MuonRecoResFD EMRecoResFD ChargedHadRecoResFD]
set systs [list NuOnECCBkgSyst NuOnENCBkgSyst NuOnERecoEffSyst]
###################
spawn sh
expect "$ "
send "cd /dune/app/users/weishi/PRISMAnalysis/lblpwgtools/CAFAna/fcl/PRISM/NuisanceSyst_Scan\r"
expect "$ "

for { set syst 0 } { $syst < [llength $systs] } { incr syst } {

set SYST [lindex $systs $syst]
send "cp Basic_PRISMPred_PlaceHolder.fcl Basic_PRISMPred_$SYST\.fcl\r"
expect "$ "
sleep 2
send "sed -i 's/PlaceHolder/$SYST\/g' Basic_PRISMPred_$SYST\.fcl\r"
expect "$ "
sleep 2
}

# recompile
send "cd /dune/app/users/weishi/PRISMAnalysis/lblpwgtools/CAFAna\r"
expect "$ "
sleep 2
send "./standalone_configure_and_build.sh --use-PRISM -u --rdb -f\r"
expect "$ "
sleep 2
send "source build/Linux/CAFAnaEnv.sh\r"
expect "$ "
sleep 2

# Produce prism prediction plots
send "cd /dune/app/users/weishi/PRISMAnalysis/lblpwgtools/CAFAna/PRISM/app\r"
expect "$ "

for { set syst 0 } { $syst < [llength $systs] } { incr syst } {

set SYST [lindex $systs $syst]
send "PRISMPrediction ../../fcl/PRISM/NuisanceSyst_Scan/Basic_PRISMPred_$SYST\.fcl\r"
expect "$ "
sleep 2
}

sleep 2
send "exit\r"
expect "$ "
