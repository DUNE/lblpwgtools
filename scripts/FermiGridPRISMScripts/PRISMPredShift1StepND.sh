#!/usr/bin/expect -f

set timeout 86400
set systs [list MaCCQE VecFFCCQEshape CCQEPauliSupViaKF MaCCRES MvCCRES Theta_Delta2Npi AhtBY BhtBY CV1uBY CV2uBY MaNCRES MvNCRES FrCEx_N FrElas_N FrInel_N FrAbs_N FrPiProd_N FrCEx_pi FrElas_pi FrInel_pi FrAbs_pi FrPiProd_pi BeRPA_A BeRPA_B BeRPA_D C12ToAr40_2p2hScaling_nu C12ToAr40_2p2hScaling_nubar E2p2h_A_nu E2p2h_B_nu E2p2h_A_nubar E2p2h_B_nubar NR_nu_np_CC_1Pi NR_nu_n_CC_2Pi NR_nu_n_CC_3Pi NR_nu_p_CC_2Pi NR_nu_p_CC_3Pi NR_nu_n_NC_1Pi NR_nu_n_NC_2Pi NR_nu_n_NC_3Pi NR_nu_p_NC_1Pi NR_nu_p_NC_2Pi NR_nu_p_NC_3Pi NR_nubar_n_CC_1Pi NR_nubar_n_CC_2Pi NR_nubar_n_CC_3Pi NR_nubar_p_CC_1Pi NR_nubar_p_CC_2Pi NR_nubar_p_CC_3Pi NR_nubar_n_NC_1Pi NR_nubar_n_NC_2Pi NR_nubar_n_NC_3Pi NR_nubar_p_NC_1Pi NR_nubar_p_NC_2Pi NR_nubar_p_NC_3Pi nuenumu_xsec_ratio nuenuebar_xsec_ratio]
set steps [list OnlyVaryNDWSBkg OnlyVaryNDNCBkg OnlyVaryNDWLBkg OnlyVaryLCWeights OnlyVaryNDUnfold OnlyVaryNDEffCorr OnlyVaryFDEffCorr OnlyVaryFDSmear OnlyVaryXsecCorr]
###################
spawn sh
expect "$ "

#Loop over steps want to shift
for { set step 0 } { $step < [llength $steps] } { incr step } {
  set STEP [lindex $steps $step]

  if { $STEP in [list OnlyVaryNDWSBkg] } {
    set lines [list 687 699]
  } elseif { $STEP in [list OnlyVaryNDNCBkg] } {
    set lines [list 638 650]
  } elseif { $STEP in [list OnlyVaryNDWLBkg] } {
    set lines [list 663 674]
  } elseif { $STEP in [list OnlyVaryLCWeights] } {
    set lines [list 726]
  } elseif { $STEP in [list OnlyVaryXsecCorr] } {
    set lines [list 794 799]
  } elseif { $STEP in [list OnlyVaryNDUnfold] } {
    set lines [list 162]
  } elseif { $STEP in [list OnlyVaryNDEffCorr] } {
    set lines [list 111 114 128 131]
  } elseif { $STEP in [list OnlyVaryFDEffCorr] } {
    set lines [list 142 146]
  } elseif { $STEP in [list OnlyVaryFDSmear] } {
    set lines [list 165]
  } else {
    sleep 1
  }

  #change output name in fcl file
  send "cd /dune/app/users/weishi/PRISMAnalysis/lblpwgtools/CAFAna/fcl/PRISM/NuisanceSyst_Scan\r"
  expect "$ "
  sleep 2

  for { set syst 0 } { $syst < [llength $systs] } { incr syst } {

  set SYST [lindex $systs $syst]

  send "sed -i 's/PRISMPred_EVisReco_$SYST\.root/PRISMPred_EVisReco_$SYST\_$STEP\.root/g' Basic_PRISMPred_$SYST\.fcl\r"
  expect "$ "
  sleep 2
  }

  #loop over lines that need shift
  send "cd /dune/app/users/weishi/PRISMAnalysis/lblpwgtools/CAFAna/PRISM\r"
  expect "$ "
  sleep 2

  for { set line 0 } { $line < [llength $lines] } { incr line } {

    set LINE [lindex $lines $line]

    if { $STEP in [list OnlyVaryNDWSBkg OnlyVaryNDNCBkg OnlyVaryNDWLBkg OnlyVaryLCWeights OnlyVaryXsecCorr] } {
      send "sed -i '$LINE\s/kNoShift/shift/' PredictionPRISM.cxx\r"
      expect "$ "
      sleep 2
    } elseif { $STEP in [list OnlyVaryNDUnfold OnlyVaryFDSmear] } {
      send "sed -i '$LINE\s/kNoShift/shift/' PRISMDetectorExtrapolation.cxx\r"
      expect "$ "
      sleep 2
    } elseif { $STEP in [list OnlyVaryNDEffCorr OnlyVaryFDEffCorr] } {
      send "sed -i '$LINE\s/kNoShift/shift/' PRISMMCEffCorrection.cxx\r"
      expect "$ "
      sleep 2
    } else {
      sleep 1
    }
  }

  send "cd /dune/app/users/weishi/PRISMAnalysis/lblpwgtools/CAFAna\r"
  expect "$ "
  sleep 2
  send "./standalone_configure_and_build.sh -u -r --db -f\r"
  expect "$ "
  sleep 2
  send "source build/Linux/CAFAnaEnv.sh\r"
  expect "$ "
  sleep 2

  #Produce prism prediction plots
  send "cd /dune/app/users/weishi/PRISMAnalysis/lblpwgtools/CAFAna/PRISM/app\r"
  expect "$ "

  for { set syst 0 } { $syst < [llength $systs] } { incr syst } {

  set SYST [lindex $systs $syst]

  send "PRISMPrediction ../../fcl/PRISM/NuisanceSyst_Scan/Basic_PRISMPred_$SYST\.fcl\r"
  expect "$ "
  sleep 2

  #change back output name in fcl file
  send "sed -i 's/PRISMPred_EVisReco_$SYST\_$STEP\.root/PRISMPred_EVisReco_$SYST\.root/g' ../../fcl/PRISM/NuisanceSyst_Scan/Basic_PRISMPred_$SYST\.fcl\r"
  expect "$ "
  sleep 2
  }

  #loop over lines to change back to no shift
  send "cd /dune/app/users/weishi/PRISMAnalysis/lblpwgtools/CAFAna/PRISM\r"
  expect "$ "
  sleep 2

  for { set line 0 } { $line < [llength $lines] } { incr line } {

    set LINE [lindex $lines $line]

    if { $STEP in [list OnlyVaryNDWSBkg OnlyVaryNDNCBkg OnlyVaryNDWLBkg OnlyVaryLCWeights OnlyVaryXsecCorr] } {
      send "sed -i '$LINE\s/shift/kNoShift/' PredictionPRISM.cxx\r"
      expect "$ "
      sleep 2
    } elseif { $STEP in [list OnlyVaryNDUnfold OnlyVaryFDSmear] } {
      send "sed -i '$LINE\s/shift/kNoShift/' PRISMDetectorExtrapolation.cxx\r"
      expect "$ "
      sleep 2
    } elseif { $STEP in [list OnlyVaryNDEffCorr OnlyVaryFDEffCorr] } {
      send "sed -i '$LINE\s/shift/kNoShift/' PRISMMCEffCorrection.cxx\r"
      expect "$ "
      sleep 2
    } else {
      sleep 1
    }
  }

  sleep 5

}

send "exit\r"
expect "$ "
