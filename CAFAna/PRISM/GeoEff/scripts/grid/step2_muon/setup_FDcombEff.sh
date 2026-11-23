#!/bin/bash

BUNDLE_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../.." && pwd)"
export NDGEOEFF_BUNDLE_ROOT="${BUNDLE_ROOT}"
export WORKDIR="${_CONDOR_JOB_IWD:-$PWD}"

source "${BUNDLE_ROOT}/scripts/env/setup_step2.sh" || { echo "Error loading ${BUNDLE_ROOT}/scripts/env/setup_step2.sh"; exit 1; }

export PYTHON3="${PYTHON3:-$(command -v python3)}"
export PYTHONPATH="${BUNDLE_ROOT}:${PYTHONPATH:-}"

_PYTHON_ENV_DIR=""
if [ -n "${NDGEOEFF_PYTHON_ENV:-}" ]; then
  if [ ! -d "${NDGEOEFF_PYTHON_ENV}" ]; then
    echo "Error, NDGEOEFF_PYTHON_ENV is not a directory: ${NDGEOEFF_PYTHON_ENV}"
    exit 1
  fi
  _PYTHON_ENV_DIR="${NDGEOEFF_PYTHON_ENV}"
elif [ -d "${BUNDLE_ROOT}/python_env" ]; then
  _PYTHON_ENV_DIR="${BUNDLE_ROOT}/python_env"
fi

if [ -n "${_PYTHON_ENV_DIR}" ]; then
  export NDGEOEFF_PYTHON_ENV_DIR="${_PYTHON_ENV_DIR}"
  export PYTHONPATH="${_PYTHON_ENV_DIR}:${PYTHONPATH}"
  if [ -d "${_PYTHON_ENV_DIR}/torch/lib" ]; then
    export LD_LIBRARY_PATH="${_PYTHON_ENV_DIR}/torch/lib:${LD_LIBRARY_PATH:-}"
  fi
  for _nvlib in "${_PYTHON_ENV_DIR}"/nvidia/*/lib; do
    if [ -d "${_nvlib}" ]; then
      export LD_LIBRARY_PATH="${_nvlib}:${LD_LIBRARY_PATH:-}"
    fi
  done
fi

unset _PYTHON_ENV_DIR _nvlib
