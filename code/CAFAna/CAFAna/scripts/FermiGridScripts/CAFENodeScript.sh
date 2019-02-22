#!/bin/bash

if [ -z ${INPUT_TAR_FILE} ]; then
  echo "[ERROR]: Expected to recieve an input file."
  exit 1
fi

if [ ! -e CAFAna/CAFECommands.cmd ]; then
  echo "[ERROR]: Expected to recieve a command file @ CAFAna/CAFECommands.cmd but didn't."
  ls CAFAna
  exit 2
fi

PNFS_PATH_APPEND=${1}
if [ -z ${1} ]; then
  echo "[ERROR]: Failed to find PNFS_PATH_APPEND passed on command line."
  exit 3
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
  exit 4
fi

mv CAFAna $_CONDOR_SCRATCH_DIR/

cd $_CONDOR_SCRATCH_DIR

export CAFANA=$(readlink -f CAFAna)
source ${CAFANA}/CAFAnaEnv.sh

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
  exit 5
fi

(( LINE_N = ${PROCESS} + 1 ))

LINE=$(cat ${CAFANA}/CAFECommands.cmd | head -${LINE_N} | tail -1)

echo "Running command: ${LINE}"

SCRIPT_NAME=$(echo ${LINE} | cut -f 1 -d " ")
OUTPUT_NAME=$(echo ${LINE} | cut -f 3 -d " ")

echo "Running script ${SCRIPT_NAME} and expecting output ${OUTPUT_NAME}"

if [ ! -e  ${CAFANA}/scripts/${SCRIPT_NAME} ]; then
  echo "[ERROR]: Failed to find expected script: ${CAFANA}/scripts/${SCRIPT_NAME}"
  exit 6
fi

cp ${CAFANA}/scripts/common_fit_definitions.C .
cp ${CAFANA}/scripts/${SCRIPT_NAME} .

echo "Running script @ $(date)"

echo "cafe -q -b ${CAFANA}/scripts/${SCRIPT_NAME} $(echo ${LINE} | cut -f 2- -d " ") &> ${SCRIPT_NAME}.${CLUSTER}.${PROCESS}.log"
cafe -q -b ${CAFANA}/scripts/${SCRIPT_NAME} $(echo ${LINE} | cut -f 2- -d " ") &> ${SCRIPT_NAME}.${CLUSTER}.${PROCESS}.log

echo "Copying output @ $(date)"

echo "ifdh cp -D $IFDH_OPTION ${SCRIPT_NAME}.${CLUSTER}.${PROCESS}.log ${PNFS_OUTDIR}/"
ifdh cp -D $IFDH_OPTION ${SCRIPT_NAME}.${CLUSTER}.${PROCESS}.log ${PNFS_OUTDIR}/

if [ ! -e ${OUTPUT_NAME} ]; then
  echo "[WARN]: Failed to produce expected output file."
fi

for f in *.root; do
  echo "ifdh cp -D $IFDH_OPTION ${f} ${PNFS_OUTDIR}/"
  ifdh cp -D $IFDH_OPTION ${f} ${PNFS_OUTDIR}/

done

echo "All stop @ $(date)"
