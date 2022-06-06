#!/usr/bin/expect -f

set timeout 86400
set systs [list MaCCQE VecFFCCQEshape CCQEPauliSupViaKF MaCCRES MvCCRES Theta_Delta2Npi AhtBY BhtBY CV1uBY CV2uBY MaNCRES MvNCRES FrCEx_N FrElas_N FrInel_N FrAbs_N FrPiProd_N FrCEx_pi FrElas_pi FrInel_pi FrAbs_pi FrPiProd_pi BeRPA_A BeRPA_B BeRPA_D C12ToAr40_2p2hScaling_nu C12ToAr40_2p2hScaling_nubar E2p2h_A_nu E2p2h_B_nu E2p2h_A_nubar E2p2h_B_nubar NR_nu_np_CC_1Pi NR_nu_n_CC_2Pi NR_nu_n_CC_3Pi NR_nu_p_CC_2Pi NR_nu_p_CC_3Pi NR_nu_n_NC_1Pi NR_nu_n_NC_2Pi NR_nu_n_NC_3Pi NR_nu_p_NC_1Pi NR_nu_p_NC_2Pi NR_nu_p_NC_3Pi NR_nubar_n_CC_1Pi NR_nubar_n_CC_2Pi NR_nubar_n_CC_3Pi NR_nubar_p_CC_1Pi NR_nubar_p_CC_2Pi NR_nubar_p_CC_3Pi NR_nubar_n_NC_1Pi NR_nubar_n_NC_2Pi NR_nubar_n_NC_3Pi NR_nubar_p_NC_1Pi NR_nubar_p_NC_2Pi NR_nubar_p_NC_3Pi nuenumu_xsec_ratio nuenuebar_xsec_ratio]
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
send "./standalone_configure_and_build.sh -u -r --db -f\r"
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
