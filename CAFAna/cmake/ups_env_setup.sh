#!/bin/bash

source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh

if [ -z "${UPS_SHELL}" ]; then
  echo "[ERROR]: Is UPS set up?"
  exit 1
fi
#setup root v6_28_10b -q e20:p3915:prof || exit 1 #  v6_26_06b  -q e20:p3913:prof $ 
setup root v6_28_12  -q e26:p3915:prof || exit 1  
setup boost v1_82_0 -q e26:prof || exit 1
setup cmake v3_22_2 || exit 1
setup sundials v7_1_1 || exit 1
setup eigen v23_08_01_66e8f || exit 1
setup stan v2_35_0a || exit 1
setup osclib v00.27 -q e26:prof:n319:stan || exit 1
setup cafanacore v02.03 -q e26:prof:n319 || exit 1 
setup duneanaobj v03_07_00 -q e26:prof || exit 1 #v03_02_01 v03_01_00
setup fhiclcpp v4_18_04 -q e26:prof || exit 1 # for prism

# To get setup_fnal_security which helps reading files over xrootd
#setup duneutil v09_53_00d02 -q e20:prof
echo "Finished ups setup."
