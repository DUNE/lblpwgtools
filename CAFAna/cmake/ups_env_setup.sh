#!/bin/bash

source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh

if [ -z "${UPS_SHELL}" ]; then
  echo "[ERROR]: Is UPS set up?"
  exit 1
fi

setup root v6_18_04d -q e19:prof || exit 1
setup boost v1_70_0 -q e19:prof || exit 1
setup cmake v3_12_2 || exit 1
setup jobsub_client || exit 1

setup clhep v2_4_1_2 -q e19:prof || exit 1

setup stan v2_18_0a -q e19:prof || exit 1

setup osclib v00.09 -q e19:prof:stan || exit 1
setup cafanacore v01.11 -q e19:prof || exit 1

# To get setup_fnal_security which helps reading files over xrootd
setup duneutil v09_09_02 -q e19:prof
