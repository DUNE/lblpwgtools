#!/usr/bin/expect -f

set timeout 86400
set systs [list MaCCQE VecFFCCQEshape CCQEPauliSupViaKF MaCCRES MvCCRES Theta_Delta2Npi AhtBY BhtBY CV1uBY CV2uBY MaNCRES MvNCRES FrCEx_N FrElas_N FrInel_N FrAbs_N FrPiProd_N FrCEx_pi FrElas_pi FrInel_pi FrAbs_pi FrPiProd_pi BeRPA_A BeRPA_B BeRPA_D C12ToAr40_2p2hScaling_nu C12ToAr40_2p2hScaling_nubar E2p2h_A_nu E2p2h_B_nu E2p2h_A_nubar E2p2h_B_nubar NR_nu_np_CC_1Pi NR_nu_n_CC_2Pi NR_nu_n_CC_3Pi NR_nu_p_CC_2Pi NR_nu_p_CC_3Pi NR_nu_n_NC_1Pi NR_nu_n_NC_2Pi NR_nu_n_NC_3Pi NR_nu_p_NC_1Pi NR_nu_p_NC_2Pi NR_nu_p_NC_3Pi NR_nubar_n_CC_1Pi NR_nubar_n_CC_2Pi NR_nubar_n_CC_3Pi NR_nubar_p_CC_1Pi NR_nubar_p_CC_2Pi NR_nubar_p_CC_3Pi NR_nubar_n_NC_1Pi NR_nubar_n_NC_2Pi NR_nubar_n_NC_3Pi NR_nubar_p_NC_1Pi NR_nubar_p_NC_2Pi NR_nubar_p_NC_3Pi nuenumu_xsec_ratio nuenuebar_xsec_ratio]
###################
spawn sh
expect "$ "
send "cd /dune/app/users/weishi/PRISMAnalysis/lblpwgtools/CAFAna/PRISM/scripts\r"
expect "$ "
send "source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.24.02/x86_64-centos7-gcc48-opt/bin/thisroot.sh\r"
expect "$ "

for { set syst 0 } { $syst < [llength $systs] } { incr syst } {

set SYST [lindex $systs $syst]
send "sed -i '49s/PlaceHolder/$SYST\/' QuickHistPlotter.C\r"
expect "$ "
sleep 1
send "root -l -b -q QuickHistPlotter.C\r"
expect "$ "
sleep 2

send "sed -i '50s/PRISMPred_EVisReco_%s.root/PRISMPred_EVisReco_%s_OnlyVaryNDWSBkg.root/' QuickHistPlotter.C\r"
expect "$ "
sleep 1
send "sed -i '106s/Overlay_%s.root/Overlay_%s_OnlyVaryNDWSBkg.root/' QuickHistPlotter.C\r"
expect "$ "
sleep 1
send "root -l -b -q QuickHistPlotter.C\r"
expect "$ "
sleep 2

send "sed -i '50s/PRISMPred_EVisReco_%s_OnlyVaryNDWSBkg.root/PRISMPred_EVisReco_%s_OnlyVaryNDNCBkg.root/' QuickHistPlotter.C\r"
expect "$ "
sleep 1
send "sed -i '106s/Overlay_%s_OnlyVaryNDWSBkg.root/Overlay_%s_OnlyVaryNDNCBkg.root/' QuickHistPlotter.C\r"
expect "$ "
sleep 1
send "root -l -b -q QuickHistPlotter.C\r"
expect "$ "
sleep 2

send "sed -i '50s/PRISMPred_EVisReco_%s_OnlyVaryNDNCBkg.root/PRISMPred_EVisReco_%s_OnlyVaryNDWLBkg.root/' QuickHistPlotter.C\r"
expect "$ "
sleep 1
send "sed -i '106s/Overlay_%s_OnlyVaryNDNCBkg.root/Overlay_%s_OnlyVaryNDWLBkg.root/' QuickHistPlotter.C\r"
expect "$ "
sleep 1
send "root -l -b -q QuickHistPlotter.C\r"
expect "$ "
sleep 2

send "sed -i '50s/PRISMPred_EVisReco_%s_OnlyVaryNDWLBkg.root/PRISMPred_EVisReco_%s_OnlyVaryNDUnfold.root/' QuickHistPlotter.C\r"
expect "$ "
sleep 1
send "sed -i '106s/Overlay_%s_OnlyVaryNDWLBkg.root/Overlay_%s_OnlyVaryNDUnfold.root/' QuickHistPlotter.C\r"
expect "$ "
sleep 1
send "root -l -b -q QuickHistPlotter.C\r"
expect "$ "
sleep 2

send "sed -i '50s/PRISMPred_EVisReco_%s_OnlyVaryNDUnfold.root/PRISMPred_EVisReco_%s_OnlyVaryNDEffCorr.root/' QuickHistPlotter.C\r"
expect "$ "
sleep 1
send "sed -i '106s/Overlay_%s_OnlyVaryNDUnfold.root/Overlay_%s_OnlyVaryNDEffCorr.root/' QuickHistPlotter.C\r"
expect "$ "
sleep 1
send "root -l -b -q QuickHistPlotter.C\r"
expect "$ "
sleep 2

send "sed -i '50s/PRISMPred_EVisReco_%s_OnlyVaryNDEffCorr.root/PRISMPred_EVisReco_%s_OnlyVaryFDEffCorr.root/' QuickHistPlotter.C\r"
expect "$ "
sleep 1
send "sed -i '106s/Overlay_%s_OnlyVaryNDEffCorr.root/Overlay_%s_OnlyVaryFDEffCorr.root/' QuickHistPlotter.C\r"
expect "$ "
sleep 1
send "root -l -b -q QuickHistPlotter.C\r"
expect "$ "
sleep 2

send "sed -i '50s/PRISMPred_EVisReco_%s_OnlyVaryFDEffCorr.root/PRISMPred_EVisReco_%s_OnlyVaryFDSmear.root/' QuickHistPlotter.C\r"
expect "$ "
sleep 1
send "sed -i '106s/Overlay_%s_OnlyVaryFDEffCorr.root/Overlay_%s_OnlyVaryFDSmear.root/' QuickHistPlotter.C\r"
expect "$ "
sleep 1
send "root -l -b -q QuickHistPlotter.C\r"
expect "$ "
sleep 2

send "sed -i '50s/PRISMPred_EVisReco_%s_OnlyVaryFDSmear.root/PRISMPred_EVisReco_%s_OnlyVaryXsecCorr.root/' QuickHistPlotter.C\r"
expect "$ "
sleep 1
send "sed -i '106s/Overlay_%s_OnlyVaryFDSmear.root/Overlay_%s_OnlyVaryXsecCorr.root/' QuickHistPlotter.C\r"
expect "$ "
sleep 1
send "root -l -b -q QuickHistPlotter.C\r"
expect "$ "
sleep 2

send "sed -i '50s/PRISMPred_EVisReco_%s_OnlyVaryXsecCorr.root/PRISMPred_EVisReco_%s_OnlyVaryLCWeights.root/' QuickHistPlotter.C\r"
expect "$ "
sleep 1
send "sed -i '106s/Overlay_%s_OnlyVaryXsecCorr.root/Overlay_%s_OnlyVaryLCWeights.root/' QuickHistPlotter.C\r"
expect "$ "
sleep 1
send "root -l -b -q QuickHistPlotter.C\r"
expect "$ "
sleep 2

#change back
send "sed -i '50s/PRISMPred_EVisReco_%s_OnlyVaryLCWeights.root/PRISMPred_EVisReco_%s.root/' QuickHistPlotter.C\r"
expect "$ "
sleep 1
send "sed -i '106s/Overlay_%s_OnlyVaryLCWeights.root/Overlay_%s.root/' QuickHistPlotter.C\r"
expect "$ "
sleep 1
send "sed -i '49s/$SYST\/PlaceHolder/' QuickHistPlotter.C\r"
expect "$ "
sleep 1
}

sleep 2
send "exit\r"
expect "$ "
