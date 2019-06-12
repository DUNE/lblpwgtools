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

AXBLOBARG=""
if [ ! -z ${3} ]; then
  AXBLOBARG=" -A ${3}"
fi

SYSTDESCRIPTORARG=""
if [ ! -z ${4} ]; then
  SYSTDESCRIPTORARG=" --syst-descriptor ${4}"
fi

NOFAKEDATAARG=""
if [ ! -z ${4} ]; then
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

(( LINE_N = ${PROCESS} + 1 ))

INPFILE=$(cat ${CAFANA}/InputCAFs.${SAMPLE_NAME}.list | head -${LINE_N} | tail -1)

OUTFILENAME=${SAMPLE_NAME}.State.${CLUSTER}.${PROCESS}.root
LOGYLOG "Output file name: ${OUTFILENAME}"

ifdh ls ${PNFS_OUTDIR}

if [ $? -ne 0 ]; then
  LOGYLOG "Unable to read ${PNFS_OUTDIR}. Make sure that you have created this directory and given it group write permission (chmod g+w ${PNFS_OUTDIR})."
  exit 10
fi

LOGYLOG "Building interps @ $(date)"

OUTFILENAME=${SAMPLE_NAME}.State.${CLUSTER_ID}.${PROCESS_ID}.root

LOGYLOG "MakePredInterps -i ${INPFILE} -S ${SAMPLE_NAME} ${AXBLOBARG} ${SYSTDESCRIPTORARG} ${NOFAKEDATAARG} -o ${OUTFILENAME}"
MakePredInterps -i ${INPFILE} -S ${SAMPLE_NAME} ${AXBLOBARG} ${SYSTDESCRIPTORARG} ${NOFAKEDATAARG} -o ${OUTFILENAME}

LOGYLOG "Copying output @ $(date)"

if [ ! -e ${OUTFILENAME} ]; then
  LOGYLOG "[ERROR]: Failed to produce expected output file."
  exit 1
fi

LOGYLOG "ifdh cp -D $IFDH_OPTION ${OUTFILENAME} ${PNFS_OUTDIR}/"
ifdh cp -D $IFDH_OPTION ${OUTFILENAME} ${PNFS_OUTDIR}/

LOGYLOG "All stop @ $(date)"
