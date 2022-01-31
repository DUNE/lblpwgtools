#!/bin/bash

source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh

if [ -z "${UPS_SHELL}" ]; then
  echo "[ERROR]: Is UPS set up?"
  exit 1
fi
echo "ROOT"
setup root v6_22_08d -q e20:prof:p392 || exit 1
echo "BOOST"
setup boost v1_75_0 -q e20:prof || exit 1
echo "cmake"
setup cmake v3_14_3 || exit 1
echo "jobsub"
setup jobsub_client || exit 1
echo "eigen"
setup eigen v3_3_9a || exit 1
echo "stan"
setup stan v2_26_1 -q e20:prof || exit 1
echo "osclib"
setup osclib v00.17 -q e20:prof:n311:stan || exit 1
echo "cafanacore"
#setup cafanacore v01.23 -q e20:prof:n311 || exit 1
setup cafanacore v01.25 -q e20:prof:n311 || exit 1

# To get setup_fnal_security which helps reading files over xrootd
#setup duneutil v09_39_01 -q e20:prof
echo "Finished ups setup."
