#!/bin/bash

source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh
export PRODUCTS=/dune/app/users/jwolcott/ups:$PRODUCTS

if [ -z "${UPS_SHELL}" ]; then
  echo "[ERROR]: Is UPS set up?"
  exit 1
fi

setup root v6_18_04d -q e19:prof:py2
setup boost v1_70_0 -q e19:prof
setup cmake v3_12_2

setup clhep v2_4_1_2 -q e19:prof

# Stan insists on its dependencies being loaded, so we only need to load it
setup stan v2_18_0 -q e19:prof
export STAN_INC=$STAN_DIR/include   # only necessary until v2_18_0a is ready