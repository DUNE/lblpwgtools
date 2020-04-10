#!/bin/bash

source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh
export PRODUCTS=/dune/app/users/jwolcott/ups:$PRODUCTS

if [ -z "${UPS_SHELL}" ]; then
  echo "[ERROR]: Is UPS set up?"
  exit 1
fi

setup root v6_12_06a -q e15:prof
setup boost v1_66_0a -q e15:prof
setup cmake v3_12_2

# Stan insists on its dependencies being loaded, so we only need to load it
setup stan v2.18.0 -q e17:prof