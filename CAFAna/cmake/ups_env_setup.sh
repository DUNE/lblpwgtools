#!/bin/bash

source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh

if [ -z "${UPS_SHELL}" ]; then
  echo "[ERROR]: Is UPS set up?"
  exit 1
fi

setup boost v1_75_0 -q e20:prof || exit 1
setup cmake v3_22_2 || exit 1
setup jobsub_client || exit 1

setup stan v2_26_1 -q e20:prof || exit 1

setup clhep v2_4_4_1 -q e20:prof || exit 1

setup duneanaobj v02_00_01 -qe20:prof || exit 1
setup srproxy v00.35 -q py3 || exit 1
# There is something wrong with the ups packaging of pygcxxml. It omits the
# "python3.9" when adding itself to $PYTHONPATH
export PYTHONPATH=$PYGCCXML_DIR/lib/python3.9/site-packages:$PYTHONPATH

setup osclib v00.21 -q e20:prof:n311:stan || exit 1
setup cafanacore v01.29 -q e20:prof:n311 || exit 1

# To get setup_fnal_security which helps reading files over xrootd
setup duneutil v09_53_00d02 -q e20:prof
