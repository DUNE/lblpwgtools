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

# Stan insists on its dependencies being loaded, so we only need to load it
setup stan v2_18_0a -q e19:prof || exit 1

setup osclib v00.04 -q e19:prof:stan || exit 1
