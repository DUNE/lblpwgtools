#!/bin/bash

source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh

if [ -z "${UPS_SHELL}" ]; then
  echo "[ERROR]: Is UPS set up?"
  exit 1
fi

setup root v6_16_00 -q e17:prof
setup boost v1_66_0a -q e17:prof
setup clhep v2_3_4_6 -q e17:prof
setup cmake v3_12_2
setup jobsub_client

setup srproxy v00.08 -z /cvmfs/nova.opensciencegrid.org/externals/ -q py2
