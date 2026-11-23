#!/bin/bash

BUNDLE_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../.." && pwd)"
export NDGEOEFF_BUNDLE_ROOT="${BUNDLE_ROOT}"
export WORKDIR="${_CONDOR_JOB_IWD:-$PWD}"

source "${BUNDLE_ROOT}/scripts/env/setup_step1.sh" || { echo "Error loading ${BUNDLE_ROOT}/scripts/env/setup_step1.sh"; exit 1; }

export LD_LIBRARY_PATH="${BUNDLE_ROOT}/lib:${LD_LIBRARY_PATH:-}"
export PATH="${BUNDLE_ROOT}/bin:${PATH}"
