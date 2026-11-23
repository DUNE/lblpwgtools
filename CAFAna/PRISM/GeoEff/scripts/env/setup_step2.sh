#!/usr/bin/env bash
# Step 2 environment: ROOT 6.28 + Python 3 for the muon/combined efficiency.
# Run from the repository root: source scripts/env/setup_step2.sh

source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh

setup dunetpc v09_41_00_02 -q e20:prof

_REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
export PYTHONPATH="${_REPO_ROOT}:${_REPO_ROOT}/my_python_env:${PYTHONPATH:-}"

which pip
which python3
