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
fi

# Separate Python env tarball (make_bundle.sh pyenv), passed as an extra
# --tar_file_name. jobsub unpacks tarball N to $INPUT_TAR_DIR_LOCAL (first) or
# $INPUT_TAR_DIR_LOCAL_N; scan all of them so the order does not matter.
if [ -z "${_PYTHON_ENV_DIR}" ]; then
  for _tarvar in ${!INPUT_TAR_DIR_LOCAL*}; do
    _tardir="${!_tarvar}"
    if [ -n "${_tardir}" ] && [ -f "${_tardir}/GeoEffPyEnv/GEOEFF_PYENV" ]; then
      _PYTHON_ENV_DIR="${_tardir}/GeoEffPyEnv"
      echo "Using Python env tarball from ${_tarvar}=${_tardir}:"
      cat "${_PYTHON_ENV_DIR}/GEOEFF_PYENV"
      break
    fi
  done
  unset _tarvar _tardir
fi

if [ -z "${_PYTHON_ENV_DIR}" ] && [ -d "${BUNDLE_ROOT}/python_env" ]; then
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
