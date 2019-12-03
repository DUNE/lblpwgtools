#!/bin/bash

LOG_TO_IFDH=0

LOGYLOG () {
  echo ${1}
  if [ ${LOG_TO_IFDH} == "1" ]; then
    ifdh log ${1}
  fi
}

if [ -z ${INPUT_TAR_FILE} ]; then
  LOGYLOG "[ERROR]: Expected to recieve an input file."
  exit 1
fi

PNFS_PATH_APPEND=${1}
if [ -z ${1} ]; then
  LOGYLOG "[ERROR]: Failed to find PNFS_PATH_APPEND passed on command line."
  exit 2
fi

SAMPLE_NAME=${2}
if [ -z ${2} ]; then
  LOGYLOG "[ERROR]: Failed to find SAMPLE_NAME passed on command line."
  exit 2
fi

IS_FD="0"
if echo ${SAMPLE_NAME} | grep "FD"; then
  IS_FD="1"
fi

ANAVERSION="4"
if [ ! -z ${3} ]; then
  ANAVERSION="${3}"
fi

AXBLOBARG=""
if [ ! -z ${4} ]; then
  AXBLOBARG=" -A ${4}"
fi

SYSTDESCRIPTORARG=""
if [ ! -z ${5} ]; then
  SYSTDESCRIPTORARG=" --syst-descriptor ${5}"
fi

NOFAKEDATAARG=""
if [ ! -z ${6} ]; then
  NOFAKEDATAARG=" --no-fakedata-dials"
fi

if [ ! -e CAFAna/InputCAFs.${SAMPLE_NAME}.list ]; then
  LOGYLOG "[ERROR]: Expected to recieve a CAF file list @ CAFAna/InputCAFs.${SAMPLE_NAME}.list but didn't."
  ls CAFAna
  exit 2
fi

printenv

set -x #start bash debugging at this point
LOGYLOG "Start $(date)"
LOGYLOG "Site:${GLIDEIN_ResourceName}"
LOGYLOG "The worker node is " `hostname` "OS: " `uname -a`
LOGYLOG "The user id is $(whoami)"
LOGYLOG "The output of id is: $(id)"
set +x #stop bash debugging at this point

if [ -z ${GRID_USER} ]; then
  GRID_USER=$(basename $X509_USER_PROXY | cut -d "_" -f 2)
fi

if [ -z ${GRID_USER} ]; then
  LOGYLOG "Failed to get GRID_USER."
  exit 2
fi

mv CAFAna $_CONDOR_SCRATCH_DIR/

cd $_CONDOR_SCRATCH_DIR

export CAFANA=$(readlink -f CAFAna)
source ${CAFANA}/CAFAnaEnv.sh

voms-proxy-info --all

setup ifdhc

ups active

export IFDH_CP_UNLINK_ON_ERROR=1;
export IFDH_CP_MAXRETRIES=2;

PNFS_OUTDIR=/pnfs/dune/persistent/users/${GRID_USER}/${PNFS_PATH_APPEND}
LOGYLOG "Output dir is ${PNFS_OUTDIR}"

INPFILE=""
if [ ${IS_FD} == "1" ]; then
  #For FD we want to read all at once
  for i in $(cat ${CAFANA}/InputCAFs.${SAMPLE_NAME}.list); do
    if echo ${i} | grep "nonswap"; then
      echo "Found nonswap file: ${i}"
      INPFILE="${INPFILE} -i ${i}"
    elif echo ${i} | grep "nueswap"; then
      echo "Found nueswap file: ${i}";
      INPFILE="${INPFILE} -e ${i}"
    elif echo ${i} | grep "tauswap"; then
      echo "Found tauswap file: ${i}";
      INPFILE="${INPFILE} -t ${i}"
    fi;
  done

else
  (( LINE_N = ${PROCESS} + 1 ))

  INPFILE="-i $(cat ${CAFANA}/InputCAFs.${SAMPLE_NAME}.list | head -${LINE_N} | tail -1)"
fi

ifdh ls ${PNFS_OUTDIR}

if [ $? -ne 0 ]; then
  LOGYLOG "Unable to read ${PNFS_OUTDIR}. Make sure that you have created this directory and given it group write permission (chmod g+w ${PNFS_OUTDIR})."
  exit 10
fi

LOGYLOG "Building interps @ $(date)"

OUTFILENAME=""
if [ ${IS_FD} == "1" ]; then
  OUTFILENAME=${SAMPLE_NAME}.State.root
else
  OUTFILENAME=${SAMPLE_NAME}.State.${CLUSTER}.${PROCESS}.root
fi

LOGYLOG "Output file name: ${OUTFILENAME}"

export CAFANA_ANALYSIS_VERSION=${ANAVERSION}
echo "CAFANA_ANALYSIS_VERSION=${CAFANA_ANALYSIS_VERSION}"

LOGYLOG "MakePredInterps ${INPFILE} -S ${SAMPLE_NAME} ${AXBLOBARG} ${SYSTDESCRIPTORARG} ${NOFAKEDATAARG} -o ${OUTFILENAME}"
MakePredInterps ${INPFILE} -S ${SAMPLE_NAME} ${AXBLOBARG} ${SYSTDESCRIPTORARG} ${NOFAKEDATAARG} -o ${OUTFILENAME}

LOGYLOG "Copying output @ $(date)"

if [ ! -e ${OUTFILENAME} ]; then
  LOGYLOG "[ERROR]: Failed to produce expected output file."
  exit 1
fi

LOGYLOG "ifdh cp -D $IFDH_OPTION ${OUTFILENAME} ${PNFS_OUTDIR}/"
ifdh cp -D $IFDH_OPTION ${OUTFILENAME} ${PNFS_OUTDIR}/

LOGYLOG "All stop @ $(date)"
