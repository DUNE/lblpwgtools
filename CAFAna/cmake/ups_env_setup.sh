#!/bin/bash

source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh

if [ -z "${UPS_SHELL}" ]; then
  echo "[ERROR]: Is UPS set up?"
  exit 1
fi
setup root v6_22_08d -q e20:prof:p392 || exit 1
setup boost v1_75_0 -q e20:prof || exit 1
setup cmake v3_14_3 || exit 1
setup jobsub_client || exit 1
setup eigen v3_3_9a || exit 1
setup stan v2_26_1 -q e20:prof || exit 1
setup osclib v00.17 -q e20:prof:n311:stan || exit 1
setup cafanacore v01.26 -q e20:prof:n311 || exit 1
setup fhiclcpp v4_15_03 -q e20:prof || exit 1 # for prism

# To get setup_fnal_security which helps reading files over xrootd
#setup duneutil v09_39_01 -q e20:prof
echo "Finished ups setup."
