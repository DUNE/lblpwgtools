#!/bin/bash

: "${APP:?APP must be set}"
: "${OUTPUT_BASENAME:?OUTPUT_BASENAME must be set}"
: "${OUTDIR:?OUTDIR must be set}"

echo "Running on $(hostname) at ${GLIDEIN_Site}. GLIDEIN_DUNESite = ${GLIDEIN_DUNESite}"

BUNDLE_DIR="${INPUT_TAR_DIR_LOCAL}/GeoEff"
INPUT_DIR="${BUNDLE_DIR}/inputs"
SETUP_SCRIPT="${BUNDLE_DIR}/scripts/grid/step3_etrim/setupNDEff-grid.sh"

echo "pwd: $(pwd)"
echo "ls -l INPUT_TAR_DIR_LOCAL: ${INPUT_TAR_DIR_LOCAL}"
ls -l "${INPUT_TAR_DIR_LOCAL}"

if [ ! -e "${SETUP_SCRIPT}" ]; then
  echo "Error, setup script not found: ${SETUP_SCRIPT}"
  exit 1
fi
echo "Start to run . ${SETUP_SCRIPT}"
. "${SETUP_SCRIPT}"

setup ifdhc

export IFDH_CP_MAXRETRIES=2
export XRD_CONNECTIONRETRY=32
export XRD_REQUESTTIMEOUT=14400
export XRD_REDIRECTLIMIT=255
export XRD_LOADBALANCERTTL=7200
export XRD_STREAMTIMEOUT=14400

ifdh mkdir_p "${OUTDIR}" || { echo "Error creating or checking ${OUTDIR}"; exit 2; }
echo "Finished checking outdir: ${OUTDIR}"

myinfilehadron=""
(( LINE_N = ${PROCESS} + 1 ))

for ifile in $(cat "${INPUT_DIR}/myOutputFDGeoEffHadron.txt" | head -${LINE_N} | tail -1); do
  myinfilehadron=${ifile}
done

myinfilemu=""
for ifile2 in $(cat "${INPUT_DIR}/myOutputFDGeoEffMu.txt" | head -${LINE_N} | tail -1); do
  myinfilemu=${ifile2}
done

echo "Got input file hadron: ${myinfilehadron}"
echo "Got input file mu: ${myinfilemu}"

if [ -z "${myinfilehadron}" ] || [ -z "${myinfilemu}" ]; then
  echo "Error, missing input file for PROCESS=${PROCESS}. Exiting."
  exit 1
fi

cd "${_CONDOR_JOB_IWD}"
if [ -e data ] && [ ! -L data ]; then
  echo "Error, ${_CONDOR_JOB_IWD}/data already exists. Exiting."
  exit 1
fi
ln -sfn "${BUNDLE_DIR}/data" data

echo "${BUNDLE_DIR}/bin/${APP} ${myinfilehadron} ${myinfilemu}"
"${BUNDLE_DIR}/bin/${APP}" "${myinfilehadron}" "${myinfilemu}"
echo "Finish ${BUNDLE_DIR}/bin/${APP}"

OUTFILE_PREFIX="${OUTFILE_PREFIX:-${OUTPUT_BASENAME%.root}}"
OUTFILE="${OUTFILE_PREFIX}_${CLUSTER}_${PROCESS}.root"

if [ -f "${OUTPUT_BASENAME}" ]; then
  echo "mv ${OUTPUT_BASENAME} ${OUTFILE}"
  mv "${OUTPUT_BASENAME}" "${OUTFILE}"

  ifdh cp -D "${OUTFILE}" "${OUTDIR}"
  IFDH_RESULT=$?
  if [ ${IFDH_RESULT} -ne 0 ]; then
    echo "Error during output copyback. See output logs."
    exit ${IFDH_RESULT}
  fi
else
  echo "Error, output file not found. Exiting."
  exit 1
fi

echo "Completed successfully."
exit 0
