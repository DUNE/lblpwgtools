#!/bin/bash

source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh

#Jeremys versions of cafanacore, osclib, duneanaobm srproxy
#export PRODUCTS="/exp/dune/app/users/jwolcott/ups:$PRODUCTS"

if [ -z "${UPS_SHELL}" ]; then
  echo "[ERROR]: Is UPS set up?"
  exit 1
fi
#setup root v6_28_10b -q e20:p3915:prof || exit 1 #  v6_26_06b  -q e20:p3913:prof $ 
setup root v6_26_06b  -q e20:p3913:prof || exit 1  
setup boost v1_75_0 -q e20:prof || exit 1
setup cmake v3_22_2 || exit 1
setup jobsub_client || exit 1


setup fhiclcpp v4_15_03 -q e20:prof || exit 1 # for prism

setup eigen v3_4_0 || exit 1
setup stan v2_26_1 -q e20:prof || exit 1
setup osclib testing -q e20:prof:stanfree || exit 1
setup cafanacore testing -q e20:prof || exit 1
setup duneanaobj v03_06_01b -q e20:prof || exit 1


# To get setup_fnal_security which helps reading files over xrootd
#setup duneutil v09_53_00d02 -q e20:prof
#setup duneutil v09_89_01d01 -q e26:prof
echo "Finished ups setup."
