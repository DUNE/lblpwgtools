#!/usr/bin/expect -f

set timeout 86400
set systs [list MaCCQE VecFFCCQEshape CCQEPauliSupViaKF MaCCRES MvCCRES Theta_Delta2Npi AhtBY BhtBY CV1uBY CV2uBY MaNCRES MvNCRES FrCEx_N FrElas_N FrInel_N FrAbs_N FrPiProd_N FrCEx_pi FrElas_pi FrInel_pi FrAbs_pi FrPiProd_pi BeRPA_A BeRPA_B BeRPA_D C12ToAr40_2p2hScaling_nu C12ToAr40_2p2hScaling_nubar E2p2h_A_nu E2p2h_B_nu E2p2h_A_nubar E2p2h_B_nubar NR_nu_np_CC_1Pi NR_nu_n_CC_2Pi NR_nu_n_CC_3Pi NR_nu_p_CC_2Pi NR_nu_p_CC_3Pi NR_nu_n_NC_1Pi NR_nu_n_NC_2Pi NR_nu_n_NC_3Pi NR_nu_p_NC_1Pi NR_nu_p_NC_2Pi NR_nu_p_NC_3Pi NR_nubar_n_CC_1Pi NR_nubar_n_CC_2Pi NR_nubar_n_CC_3Pi NR_nubar_p_CC_1Pi NR_nubar_p_CC_2Pi NR_nubar_p_CC_3Pi NR_nubar_n_NC_1Pi NR_nubar_n_NC_2Pi NR_nubar_n_NC_3Pi NR_nubar_p_NC_1Pi NR_nubar_p_NC_2Pi NR_nubar_p_NC_3Pi nuenumu_xsec_ratio nuenuebar_xsec_ratio flux_Nov17_0 flux_Nov17_1 flux_Nov17_2 flux_Nov17_3 flux_Nov17_4 flux_Nov17_5 flux_Nov17_6 flux_Nov17_7 flux_Nov17_8 flux_Nov17_9 flux_Nov17_10 flux_Nov17_11 flux_Nov17_12 flux_Nov17_13 flux_Nov17_14 flux_Nov17_15 flux_Nov17_16 flux_Nov17_17 flux_Nov17_18 flux_Nov17_19 flux_Nov17_20 flux_Nov17_21 flux_Nov17_22 flux_Nov17_23 flux_Nov17_24 flux_Nov17_25 flux_Nov17_26 flux_Nov17_27]
###################
spawn sh
expect "$ "
send "cd /dune/app/users/weishi/PRISMAnalysis/lblpwgtools/CAFAna/PRISM/scripts/FermiGridPRISMScripts\r"
expect "$ "

for { set syst 0 } { $syst < [llength $systs] } { incr syst } {

set SYST [lindex $systs $syst]
send "./FarmBuildPRISMInterps.sh -i /pnfs/dune/persistent/users/chasnip/CAF_MC_FILES_4FLAVOUR/ --no-fakedata-dials -a EVisReco -N -u --syst-descriptor list:$SYST\r"
expect "$ "
sleep 5
send "./FarmBuildPRISMInterps.sh -i /pnfs/dune/persistent/users/chasnip/CAF_MC_FILES_4FLAVOUR/ --no-fakedata-dials -a EVisReco -F -u --syst-descriptor list:$SYST\r"
expect "$ "
sleep 5
}

send "jobsub_q --user weishi\r"
expect "$ "
sleep 5
send "exit\r"
expect "$ "
