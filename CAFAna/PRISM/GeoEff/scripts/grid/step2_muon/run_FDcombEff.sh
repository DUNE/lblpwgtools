#!/bin/bash

echo "Running on $(hostname) at ${GLIDEIN_Site}. GLIDEIN_DUNESite = ${GLIDEIN_DUNESite}"

OUTDIR=/pnfs/dune/scratch/users/${GRID_USER}/test/output_test

echo "pwd: $(pwd)"
echo "ls -l INPUT_TAR_DIR_LOCAL: ${INPUT_TAR_DIR_LOCAL}"
ls -l "${INPUT_TAR_DIR_LOCAL}"

BUNDLE_DIR=${INPUT_TAR_DIR_LOCAL}/DUNE_ND_GeoEff
INPUT_DIR=${BUNDLE_DIR}/inputs
SETUP_SCRIPT=${BUNDLE_DIR}/scripts/grid/step2_muon/setup_FDcombEff.sh

if [ ! -e "${SETUP_SCRIPT}" ]; then
  echo "Error, setup script not found: ${SETUP_SCRIPT}"
  exit 1
fi
echo "Start to run . ${SETUP_SCRIPT}"
. "${SETUP_SCRIPT}"

cd ${_CONDOR_JOB_IWD}

PYTHON3="${PYTHON3:-$(command -v python3)}"
if [ -z "${PYTHON3}" ]; then
  echo "Error, python3 not found"
  exit 1
fi

prepend_python_env_libs() {
  local env_dir="$1"
  if [ -d "${env_dir}/torch/lib" ]; then
    export LD_LIBRARY_PATH="${env_dir}/torch/lib:${LD_LIBRARY_PATH:-}"
  fi
  local nvlib
  for nvlib in "${env_dir}"/nvidia/*/lib; do
    if [ -d "${nvlib}" ]; then
      export LD_LIBRARY_PATH="${nvlib}:${LD_LIBRARY_PATH:-}"
    fi
  done
}

PYTHON_ENV_DIR="${NDGEOEFF_PYTHON_ENV_DIR:-}"
if [ -n "${PYTHON_ENV_DIR}" ]; then
  echo "Using preinstalled Python environment: ${PYTHON_ENV_DIR}"
else
  echo "No preinstalled Python environment found; installing dependencies in the job directory"
  TORCH_VERSION="${NDGEOEFF_TORCH_VERSION:-2.6.0}"
  TORCH_INDEX_URL="${NDGEOEFF_TORCH_INDEX_URL:-https://download.pytorch.org/whl/cpu}"
  PYTHON_ENV_DIR="${_CONDOR_JOB_IWD}/python_env"
  "${PYTHON3}" -m pip install --upgrade --target="${PYTHON_ENV_DIR}" --index-url "${TORCH_INDEX_URL}" "torch==${TORCH_VERSION}" || { echo "Error installing torch"; exit 1; }
  "${PYTHON3}" -m pip install --upgrade --target="${PYTHON_ENV_DIR}" numpy==1.26.1 || { echo "Error installing numpy"; exit 1; }
  "${PYTHON3}" -m pip install --upgrade --target="${PYTHON_ENV_DIR}" uproot==5.1.2 || { echo "Error installing uproot"; exit 1; }
  "${PYTHON3}" -m pip install --upgrade --target="${PYTHON_ENV_DIR}" scipy==1.11.3 || { echo "Error installing scipy"; exit 1; }
  export PYTHONPATH="${PYTHON_ENV_DIR}:${PYTHONPATH}"
  export NDGEOEFF_PYTHON_ENV_DIR="${PYTHON_ENV_DIR}"
fi

prepend_python_env_libs "${PYTHON_ENV_DIR}"

echo "python version:"
"${PYTHON3}" --version
"${PYTHON3}" -c "import numpy; import scipy; import uproot; import torch; print(numpy.__version__, scipy.__version__, uproot.__version__, torch.__version__)" || { echo "Error importing Python dependencies"; exit 1; }

if ! command -v ifdh >/dev/null 2>&1; then
  echo "Error, ifdh not found"
  exit 1
fi

export IFDH_CP_MAXRETRIES=2
export XRD_CONNECTIONRETRY=32
export XRD_REQUESTTIMEOUT=14400
export XRD_REDIRECTLIMIT=255
export XRD_LOADBALANCERTTL=7200
export XRD_STREAMTIMEOUT=14400

ifdh mkdir_p "${OUTDIR}" || { echo "Error creating or checking ${OUTDIR}"; exit 2; }
echo "Finished checking outdir: ${OUTDIR}"

myinfile=""
basefile=""
inputfile=""
(( LINE_N = ${PROCESS} + 1 ))

for ifile in $(cat "${INPUT_DIR}/FDCAFs.txt" | head -${LINE_N} | tail -1); do
  myinfile="${myinfile} ${ifile}"
  basefile=$(basename $ifile)
  inputfile="${basefile%.root}"
  echo "Input file without .root extension: $inputfile"
done

if [ -z "${myinfile}" ]; then
  echo "Error, no input file found for PROCESS ${PROCESS}"
  exit 2
fi

echo "Got xrootd url: $myinfile"

echo "ifdh cp -D $myinfile ${_CONDOR_JOB_IWD}"
if ! ifdh cp -D $myinfile ${_CONDOR_JOB_IWD}; then
  echo "Error copying input file(s): $myinfile"
  exit 2
fi

echo "ls -l _CONDOR_JOB_IWD"
ls -l ${_CONDOR_JOB_IWD}

shopt -s nullglob
root_files=(./*.root)
shopt -u nullglob
if [ ${#root_files[@]} -eq 0 ]; then
  echo "Error, no ROOT files found in ${_CONDOR_JOB_IWD}"
  exit 2
fi

echo "python3 ${BUNDLE_DIR}/app/make_tree_fd.py ${root_files[*]}"
"${PYTHON3}" "${BUNDLE_DIR}/app/make_tree_fd.py" "${root_files[@]}"
LAR_RESULT=$?

if [ $LAR_RESULT -ne 0 ]; then
  echo "python3 exited with abnormal status $LAR_RESULT. See error outputs."
  exit $LAR_RESULT
fi

echo "Have output"
ls -l ${_CONDOR_JOB_IWD}

OUTFILE=${inputfile}_MuCombinedEff.root

if [ -f Output_FDGeoEff.root ]; then
  echo "mv Output_FDGeoEff.root $OUTFILE"
  mv Output_FDGeoEff.root $OUTFILE

  echo "Found output root file, now copy to scratch area: $OUTDIR"
  ifdh cp -D $OUTFILE $OUTDIR
  IFDH_RESULT=$?
  if [ $IFDH_RESULT -ne 0 ]; then
    echo "Error during output copyback. See output logs."
    exit $IFDH_RESULT
  fi
else
  echo "Error, output file not found. Exiting."
  exit 1
fi

echo "Completed successfully."
exit 0
