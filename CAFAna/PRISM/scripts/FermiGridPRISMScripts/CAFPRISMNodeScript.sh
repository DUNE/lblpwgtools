#!/bin/bash

LOG_TO_IFDH=0

LOGYLOG () {
  echo ${1}
  if [ ${LOG_TO_IFDH} == "1" ]; then
    ifdh log ${1}
  fi
}

if [ ! -z ${2} ]; then
  LOG_TO_IFDH=${2}
  if [ ${LOG_TO_IFDH} == "1" ]; then
    LOGYLOG "[INFO]: Logging to ifdh log."
  fi
fi

if [ -z ${INPUT_TAR_FILE} ]; then
  LOGYLOG "[ERROR]: Expected to recieve an input file."
  #exit 1
fi

if [ ! -e CAFAna/CAFECommands.cmd ]; then
  LOGYLOG "[ERROR]: Expected to recieve a command file @ CAFAna/CAFECommands.cmd but didn't."
  ls CAFAna
  exit 2
fi

PNFS_PATH_APPEND=${1}
if [ -z ${1} ]; then
  LOGYLOG "[ERROR]: Failed to find PNFS_PATH_APPEND passed on command line."
  exit 3
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
  exit 4
fi

mv CAFAna $_CONDOR_SCRATCH_DIR/

cd $_CONDOR_SCRATCH_DIR

export CAFANA=$(readlink -f CAFAna)
source ${CAFANA}/CAFAnaEnv.sh
export CAFANA_STAT_ERRS=1

LOGYLOG "CAFANA dir = ${CAFANA}"

voms-proxy-info --all

setup ifdhc

source ${CAFANA}/CAFAnaEnv.sh

ups active

export IFDH_CP_UNLINK_ON_ERROR=1;
export IFDH_CP_MAXRETRIES=2;

PNFS_OUTDIR_STUB=/pnfs/dune/persistent/users/${GRID_USER}/${PNFS_PATH_APPEND}
LOGYLOG "Output stub dir is ${PNFS_OUTDIR_STUB}"

ifdh ls ${PNFS_OUTDIR_STUB}

LOGYLOG "Current directory ls:"
ifdh ls

if [ $? -ne 0 ]; then
  LOGYLOG "Unable to read ${PNFS_OUTDIR}. Make sure that you have created this directory and given it group write permission (chmod g+w ${PNFS_OUTDIR})."
  exit 5
fi

PNFS_OUTDIR=${PNFS_OUTDIR_STUB}

if [ $? -ne 0 ]; then
  LOGYLOG "Unable to make ${PNFS_OUTDIR}."
  exit 2
fi

LOGYLOG "Output dir is ${PNFS_OUTDIR}"

(( LINE_N = ${PROCESS} + 1 ))

LINE=$(cat ${CAFANA}/CAFECommands.cmd | head -${LINE_N} | tail -1)

LOGYLOG "Running command: ${LINE}"

BIN_X_CMD=""
BIN_X=""
BIN_Y_CMD=""
BIN_Y=""

SCRIPT_NAME=$(echo ${LINE} | cut -f 1 -d " ")
FCL_NAME=$(echo ${LINE} | cut -f 3 -d " ") # fcl file is third parameter in cmd file
BIN_X_CMD=$(echo ${LINE} | cut -f 4 -d " ")
BIN_X=$(echo ${LINE} | cut -f 5 -d " ")
BIN_Y_CMD=$(echo ${LINE} | cut -f 6 -d " ")
BIN_Y=$(echo ${LINE} | cut -f 7 -d " ")

LOGYLOG "Running script ${SCRIPT_NAME} using fcl file ${FCL_NAME} and bins x=${BIN_X} and y=${BIN_Y}"

if [ ! -e  ${CAFANA}/${FCL_NAME} ]; then
  LOGYLOG "[ERROR]: Failed to find expected fcl: ${CAFANA}/${FCL_NAME}"
  exit 6
fi

cp ${CAFANA}/${FCL_NAME} .
# If the state file is in the tarball, it will be copied to the work area on the node
cp ${CAFANA}/*.root .

LOGYLOG "Running script @ $(date)"

LOGYLOG "Running ${SCRIPT_NAME} --fcl ${CAFANA}/${FCL_NAME} ${BIN_X_CMD} ${BIN_X} ${BIN_Y_CMD} ${BIN_Y}"

${SCRIPT_NAME} --fcl ${CAFANA}/${FCL_NAME} ${BIN_X_CMD} ${BIN_X} ${BIN_Y_CMD} ${BIN_Y}

LOGYLOG "Copying output @ $(date)"

#LOGYLOG "ifdh cp -D $IFDH_OPTION ${SCRIPT_NAME}.log ${PNFS_OUTDIR}/"

#ifdh cp -D $IFDH_OPTION ${SCRIPT_NAME}.log ${PNFS_OUTDIR}/

#if [ ! -e ${OUTPUT_NAME} ]; then
#  LOGYLOG "[WARN]: Failed to produce expected output file (${OUTPUT_NAME})."
#fi

for f in *.root; do
  LOGYLOG "ifdh cp -D $IFDH_OPTION ${f} ${PNFS_OUTDIR}/"

  ifdh cp -D $IFDH_OPTION ${f} ${PNFS_OUTDIR}/
done

echo "All stop @ $(date)"
ifdh log "All stop @ $(date)"
