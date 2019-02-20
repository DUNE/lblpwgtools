#!/bin/bash

PNFS_OUTPUT_APPEND=CAFAnaInputs/StandardState
AXISBLOBNAME=""
NMAXEVENTS=10000

LIFETIME_EXP_ND="2h"
DISK_EXP_ND="2GB"
MEM_EXP_ND="2GB"

LIFETIME_EXP_FD="2h"
DISK_EXP_FD="2GB"
MEM_EXP_FD="2GB"

if [ -z "${CAFANA}" ]; then
  echo "[ERROR]: (NOvA-less)CAFAna is not set up, cannot farm jobs."
  exit 1
fi

source /cvmfs/fermilab.opensciencegrid.org/products/common/etc/setups.sh

setup jobsub_client
setup ifdhc

kx509
voms-proxy-init -rfc -noregen -voms dune:/dune/Role=Analysis
voms-proxy-info --all

if [ -e sub_tmp ]; then rm -r sub_tmp; fi

mkdir sub_tmp
cd sub_tmp

ifdh ls /pnfs/dune/persistent/users/${USER}/${PNFS_OUTPUT_APPEND}

if [ $? -ne 0 ]; then
  mkdir -p /pnfs/dune/persistent/users/${USER}/${PNFS_OUTPUT_APPEND}
  ifdh ls /pnfs/dune/persistent/users/${USER}/${PNFS_OUTPUT_APPEND}
  if [ $? -ne 0 ]; then
    echo "Unable to make /pnfs/dune/persistent/users/${USER}/${PNFS_OUTPUT_APPEND}."
    exit 2
  fi
fi

${CAFANA}/scripts/FermiGridScripts/tarball.sh

if [ ! -e CAFAna.Blob.tar.gz ]; then
  echo "[ERROR]: Failed to build source tarball."
  exit 2
fi

FD_FHC_JID=$(jobsub_submit --group=${EXPERIMENT} --jobid-output-only --resource-provides=usage_model=OPPORTUNISTIC --expected-lifetime=${LIFETIME_EXP_FD} --disk=${DISK_EXP_FD} --memory=${MEM_EXP_FD} --cpu=1 --OS=SL6 --tar_file_name=dropbox://CAFAna.Blob.tar.gz file://${CAFANA}/scripts/FermiGridScripts/BuildInterps.sh ${PNFS_OUTPUT_APPEND} FD_FHC ${NMAXEVENTS} ${AXISBLOBNAME})

FD_RHC_JID=$(jobsub_submit --group=${EXPERIMENT} --jobid-output-only --resource-provides=usage_model=OPPORTUNISTIC --expected-lifetime=${LIFETIME_EXP_FD} --disk=${DISK_EXP_FD} --memory=${MEM_EXP_FD} --cpu=1 --OS=SL6 --tar_file_name=dropbox://CAFAna.Blob.tar.gz file://${CAFANA}/scripts/FermiGridScripts/BuildInterps.sh ${PNFS_OUTPUT_APPEND} FD_RHC ${NMAXEVENTS} ${AXISBLOBNAME})

ND_FHC_JID=$(jobsub_submit --group=${EXPERIMENT} --jobid-output-only --resource-provides=usage_model=OPPORTUNISTIC --expected-lifetime=${LIFETIME_EXP_ND} --disk=${DISK_EXP_ND} --memory=${MEM_EXP_ND} --cpu=1 --OS=SL6 --tar_file_name=dropbox://CAFAna.Blob.tar.gz file://${CAFANA}/scripts/FermiGridScripts/BuildInterps.sh ${PNFS_OUTPUT_APPEND} ND_FHC ${NMAXEVENTS} ${AXISBLOBNAME})

ND_RHC_JID=$(jobsub_submit --group=${EXPERIMENT} --jobid-output-only --resource-provides=usage_model=OPPORTUNISTIC --expected-lifetime=${LIFETIME_EXP_ND} --disk=${DISK_EXP_ND} --memory=${MEM_EXP_ND} --cpu=1 --OS=SL6 --tar_file_name=dropbox://CAFAna.Blob.tar.gz file://${CAFANA}/scripts/FermiGridScripts/BuildInterps.sh ${PNFS_OUTPUT_APPEND} ND_RHC ${NMAXEVENTS} ${AXISBLOBNAME})

echo "FD_FHC_JID: ${FD_FHC_JID}" > InterpJobs.jids
echo "FD_RHC_JID: ${FD_RHC_JID}" >> InterpJobs.jids
echo "ND_FHC_JID: ${ND_FHC_JID}" >> InterpJobs.jids
echo "ND_RHC_JID: ${ND_RHC_JID}" >> InterpJobs.jids
