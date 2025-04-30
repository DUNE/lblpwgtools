#!/bin/bash

source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh

if [ -z "${UPS_SHELL}" ]; then
  echo "[ERROR]: Is UPS set up?"
  exit 1
fi

setup cmake v3_22_2 || exit 1

setup root v6_28_12  -q e26:p3915:prof || exit 1
setup boost v1_82_0 -q e26:prof || exit 1

setup fhiclcpp v4_18_04 -q e26:prof || exit 1 # for prism

setup eigen v23_08_01_66e8f || exit 1
setup sundials v6_5_0 || exit 1
setup stan v2_35_0a || exit 1

setup osclib v00.27 -q e26:n319:prof:stan || exit 1

#setup cafanacore v02.03 -q e26:n319:prof || exit 1

export PRODUCTS="/exp/dune/app/users/jwolcott/ups"
setup cafanacore testing -q e26:prof || exit 1


setup duneanaobj v03_07_00 -q e26:prof || exit 1



# To get setup_fnal_security which helps reading files over xrootd
#setup duneutil v09_53_00d02 -q e20:prof
#setup duneutil v09_89_01d01 -q e26:prof
echo "Finished ups setup."
