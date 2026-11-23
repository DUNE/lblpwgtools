#!/usr/bin/env bash
# Step 1 environment: CMake, GCC, Eigen, and ROOT 6.28.
# Run from lblpwgtools/CAFAna/PRISM/GeoEff: source scripts/env/setup_step1.sh
# (only needed for the standalone GeoEff build; a full CAFAna build uses
# CAFAna/standalone_configure_and_build.sh -u and CAFAnaEnv.sh instead)
# Override NDGEOEFF_PYTHON_VERSION if your DUNE release provides another Python 3.

source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh

setup cmake v3_19_6
setup gcc v9_3_0
setup eigen v3_3_5
setup root v6_28_10b -q e20:p3915:prof
#setup python "${NDGEOEFF_PYTHON_VERSION:-v3_9_15}"

#_NDGEOEFF_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
#export PYTHONPATH="${_NDGEOEFF_ROOT}:${_NDGEOEFF_ROOT}/lib:${PYTHONPATH:-}"

which gcc
which root
which python3
cmake --version
