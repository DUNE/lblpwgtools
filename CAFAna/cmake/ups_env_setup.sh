#!/bin/bash

source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh

if [ -z "${UPS_SHELL}" ]; then
  echo "[ERROR]: Is UPS set up?"
  exit 1
fi

setup root v6_22_06a -q e19:prof:p383b || exit 1
setup boost v1_73_0 -q e19:prof || exit 1
setup cmake v3_12_2 || exit 1
setup jobsub_client || exit 1

setup clhep v2_4_1_2 -q e19:prof || exit 1

setup eigen v3_3_9a || exit
setup stan v2_26_1 -q e19:prof || exit 1

setup osclib v00.14 -q e19:prof:n309:stan || exit 1
setup cafanacore v01.16 -q e19:prof:n309 || exit 1

# To get setup_fnal_security which helps reading files over xrootd
setup duneutil v09_21_00 -q e19:prof
