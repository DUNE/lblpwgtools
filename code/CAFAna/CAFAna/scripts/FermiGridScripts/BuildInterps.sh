#!/bin/bash

if [ -z ${INPUT_TAR_FILE} ]; then
  echo "[ERROR]: Expected to recieve an input file."
  exit 1
fi

PNFS_PATH_APPEND=${1}
if [ -z ${1} ]; then
  echo "[ERROR]: Failed to find PNFS_PATH_APPEND passed on command line."
  exit 2
fi

SAMPLE_NAME=${2}
if [ -z ${2} ]; then
  echo "[ERROR]: Failed to find SAMPLE_NAME passed on command line."
  exit 2
fi

NMAX_EVENTS=-1
if [ ! -z ${3} ]; then
  NMAX_EVENTS=${3}
fi

AXBLOBNAME=""
if [ ! -z ${4} ]; then
  AXBLOBNAME=${4}
fi

printenv

set -x #start bash debugging at this point
echo "Start $(date)"
echo "Site:${GLIDEIN_ResourceName}"
echo "The worker node is " `hostname` "OS: " `uname -a`
echo "The user id is $(whoami)"
echo "The output of id is: $(id)"
set +x #stop bash debugging at this point

if [ -z ${GRID_USER} ]; then
  GRID_USER=$(basename $X509_USER_PROXY | cut -d "_" -f 2)
fi

if [ -z ${GRID_USER} ]; then
  echo "Failed to get GRID_USER."
  exit 2
fi

mv CAFAna $_CONDOR_SCRATCH_DIR/

cd $_CONDOR_SCRATCH_DIR

export CAFANA=$(readlink -f CAFAna)
source ${CAFANA}/CAFAnaEnv.sh

kx509
voms-proxy-init -rfc -noregen -voms dune:/dune/Role=Analysis
voms-proxy-info --all

setup ifdhc v2_3_9

ups active

export IFDH_CP_UNLINK_ON_ERROR=1;
export IFDH_CP_MAXRETRIES=1;

PNFS_OUTDIR=/pnfs/dune/persistent/users/${GRID_USER}/${PNFS_PATH_APPEND}
echo "Output dir is ${PNFS_OUTDIR}"

ifdh ls ${PNFS_OUTDIR}

if [ $? -ne 0 ]; then
  echo "Unable to read ${PNFS_OUTDIR}. Make sure that you have created this directory and given it group write permission (chmod g+w ${PNFS_OUTDIR})."
  exit 10
fi

cp ${CAFANA}/scripts/common_fit_definitions.C .
cp ${CAFANA}/scripts/remake_inputs.C .

echo "Building interps @ $(date)"

cafe -q -b remake_inputs.C common_state_mcc11v3_${SAMPLE_NAME}.root ${SAMPLE_NAME} ${NMAX_EVENTS} ${AXBLOBNAME} &> common_state_mcc11v3_${SAMPLE_NAME}.log

echo "Copying output @ $(date)"

echo "ifdh cp -D $IFDH_OPTION common_state_mcc11v3_${SAMPLE_NAME}.log ${PNFS_OUTDIR}/"
ifdh cp -D $IFDH_OPTION common_state_mcc11v3_${SAMPLE_NAME}.log ${PNFS_OUTDIR}/

if [ ! -e common_state_mcc11v3_${SAMPLE_NAME}.root ]; then
  echo "[ERROR]: Failed to produce expected output file."
  exit 1
fi

echo "ifdh cp -D $IFDH_OPTION common_state_mcc11v3_${SAMPLE_NAME}.root ${PNFS_OUTDIR}/"
ifdh cp -D $IFDH_OPTION common_state_mcc11v3_${SAMPLE_NAME}.root ${PNFS_OUTDIR}/

echo "All stop @ $(date)"
