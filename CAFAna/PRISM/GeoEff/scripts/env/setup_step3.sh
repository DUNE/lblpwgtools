#!/usr/bin/env bash
# Step 3 environment: ROOT 6.28 + OscLib v00.27 (e26 qualifier), for the
# standalone GeoEff build (cmake -S . -B build from this directory).
# Run from lblpwgtools/CAFAna/PRISM/GeoEff: source scripts/env/setup_step3.sh

source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh

setup cmake v3_19_6
setup root v6_28_12 -q e26:prof:p3915
setup boost v1_82_0 -qdebug:e26
setup eigen v23_08_01_66e8f
setup stan_math v4_9_0a
setup sundials v7_1_1
setup osclib v00.27 -q e26:prof:n319:stanfree

export OSCLIB_INC="${OSCLIB_INC:-${OSCLIB_ROOT}/include}"
export OSCLIB_LIB="${OSCLIB_LIB:-${OSCLIB_ROOT}/lib}"
_NDGEOEFF_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
export PYTHONPATH="${_NDGEOEFF_ROOT}:${PYTHONPATH:-}"
