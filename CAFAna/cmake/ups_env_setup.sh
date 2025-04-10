#!/bin/bash

source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh

# temporary until we get updated builds of all the dependent products
export PRODUCTS="/exp/dune/app/users/jwolcott/ups:$PRODUCTS"

if [ -z "${UPS_SHELL}" ]; then
  echo "[ERROR]: Is UPS set up?"
  exit 1
fi

setup root v6_28_12  -q e20:p3915:prof || exit 1
setup boost v1_75_0 -q e20:prof || exit 1
setup cmake v3_22_2 || exit 1
setup jobsub_client || exit 1
setup fhiclcpp v4_15_03 -q e20:prof || exit 1 # for prism

setup eigen v3_4_0 || exit 1
setup stan v2_26_1 -q e20:prof || exit 1

# these will need to be updated as soon as we have official builds for them
setup osclib testing -q e20:prof:stanfree || exit 1
setup cafanacore testing -q e20:prof || exit 1
setup duneanaobj vtesting -q e20:debug || exit 1



# To get setup_fnal_security which helps reading files over xrootd
#setup duneutil v09_53_00d02 -q e20:prof
#setup duneutil v09_89_01d01 -q e26:prof
echo "Finished ups setup."
