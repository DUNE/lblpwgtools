#!/usr/bin/env bash
# Step 2 environment: ROOT 6.28 + Python 3 for the muon/combined efficiency.
# Run from lblpwgtools/CAFAna/PRISM/GeoEff: source scripts/env/setup_step2.sh

source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh

setup ifdhc
setup dunetpc v09_41_00_02 -q e20:prof
setup gcc v9_3_0

_REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
export PYTHONPATH="${_REPO_ROOT}:${_REPO_ROOT}/my_python_env:${PYTHONPATH:-}"
if [ -n "${NDGEOEFF_PYTHON_ENV:-}" ]; then
  export PYTHONPATH="${NDGEOEFF_PYTHON_ENV}:${PYTHONPATH}"
fi
export PYTHON3="$(command -v python3)"

for _pyenv in "${_REPO_ROOT}/my_python_env" "${NDGEOEFF_PYTHON_ENV:-}"; do
  if [ -n "${_pyenv}" ] && [ -d "${_pyenv}/torch/lib" ]; then
    export LD_LIBRARY_PATH="${_pyenv}/torch/lib:${LD_LIBRARY_PATH:-}"
  fi
  if [ -n "${_pyenv}" ]; then
    for _nvlib in "${_pyenv}"/nvidia/*/lib; do
      if [ -d "${_nvlib}" ]; then
        export LD_LIBRARY_PATH="${_nvlib}:${LD_LIBRARY_PATH:-}"
      fi
    done
  fi
done
unset _pyenv _nvlib

which pip
which python3
echo "PYTHON3=${PYTHON3}"
