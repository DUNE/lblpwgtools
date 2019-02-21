#!/bin/bash

PNFS_PATH_APPEND=CAFAnaInputs/StandardState
AXISBLOBNAME=""
NMAXEVENTS=10000
FORCE_REMOVE="0"

LIFETIME_EXP_ND="12h"
DISK_EXP_ND="200MB"
MEM_EXP_ND="1.5GB"

LIFETIME_EXP_FD="12h"
DISK_EXP_FD="200MB"
MEM_EXP_FD="1.5GB"

DO_FD="1"
DO_ND="1"

DO_FHC="1"
DO_RHC="1"

if [ -z "${CAFANA}" ]; then
  echo "[ERROR]: (NOvA-less)CAFAna is not set up, cannot farm jobs."
  exit 1
fi

while [[ ${#} -gt 0 ]]; do

  key="$1"
  case $key in

      -p|--pnfs-path-append)

      if [[ ${#} -lt 2 ]]; then
        echo "[ERROR]: ${1} expected a value."
        exit 1
      fi

      PNFS_PATH_APPEND="$2"
      echo "[OPT]: Writing output to /pnfs/dune/persistent/users/${USER}/${PNFS_PATH_APPEND}"
      shift # past argument
      ;;

      -a|--axis-argument)

      if [[ ${#} -lt 2 ]]; then
        echo "[ERROR]: ${1} expected a value."
        exit 1
      fi

      AXISBLOBNAME="$2"
      echo "[OPT]: Using Axis argument AXISBLOBNAME:\"${AXISBLOBNAME}\"."
      shift # past argument
      ;;

      -n|--n-max-events)

      if [[ ${#} -lt 2 ]]; then
        echo "[ERROR]: ${1} expected a value."
        exit 1
      fi

      NMAXEVENTS="$2"
      echo "[OPT]: Each job will handle at most \"${NMAXEVENTS}\" events."
      shift # past argument
      ;;

      -f|--force-remove)

      FORCE_REMOVE="1"
      echo "[OPT]: Will remove extant files in output directory."
      ;;

      -N|--ND-Only)

      DO_FD="0"
      echo "[OPT]: Will only generate ND inputs."
      ;;

      -F|--FD-Only)

      DO_ND="0"
      echo "[OPT]: Will only generate FD inputs."
      ;;

      -u|--FHC-Only)

      DO_RHC="0"
      echo "[OPT]: Will only generate FHC inputs."
      ;;

      -b|--RHC-Only)

      DO_FHC="0"
      echo "[OPT]: Will only generate RHC inputs."
      ;;

      -?|--help)
      echo "[RUNLIKE] ${SCRIPTNAME}"
      echo -e "\t-p|--pnfs-path-append      : Path to append to output path: /pnfs/dune/persistent/users/${USER}/"
      echo -e "\t-a|--axis-argument         : Argument to pass to remake_inputs to specify the axes"
      echo -e "\t-n|--n-max-events          : Maximum number of events to process (0 for all)."
      echo -e "\t-f|--force-remove          : If files are found in the output directory, remove them."
      echo -e "\t-N|--ND-Only               : Only run ND."
      echo -e "\t-F|--FD-Only               : Only run FD."
      echo -e "\t-u|--FHC-Only              : Only run FHC."
      echo -e "\t-b|--RHC-Only              : Only run RHC."
      echo -e "\t-?|--help                  : Print this message."
      exit 0
      ;;

      *)
              # unknown option
      echo "Unknown option $1"
      exit 1
      ;;
  esac
  shift # past argument or value
done

source /cvmfs/fermilab.opensciencegrid.org/products/common/etc/setups.sh

setup jobsub_client
setup ifdhc

if ! kx509; then
  echo "[ERROR]: Failed to setup kx509."
  exit 1
fi
if ! voms-proxy-info -exists; then
  voms-proxy-init -rfc -noregen -voms dune:/dune/Role=Analysis
  voms-proxy-info --all
fi

if [ ! -e CAFAna.Blob.tar.gz ]; then
  ${CAFANA}/scripts/FermiGridScripts/tarball.sh
fi

if [ ! -e CAFAna.Blob.tar.gz ]; then
  echo "[ERROR]: Failed to build source tarball."
  exit 2
fi

ifdh ls /pnfs/dune/persistent/users/${USER}/${PNFS_PATH_APPEND}

if [ $? -ne 0 ]; then
  mkdir -p /pnfs/dune/persistent/users/${USER}/${PNFS_PATH_APPEND}
  ifdh ls /pnfs/dune/persistent/users/${USER}/${PNFS_PATH_APPEND}
  if [ $? -ne 0 ]; then
    echo "Unable to make /pnfs/dune/persistent/users/${USER}/${PNFS_PATH_APPEND}."
    exit 2
  fi
elif [ ${FORCE_REMOVE} == "1" ]; then
  echo "[INFO]: Force removing previous existant output directories: \"/pnfs/dune/persistent/users/${USER}/${PNFS_PATH_APPEND}\" "
  rm -rf /pnfs/dune/persistent/users/${USER}/${PNFS_PATH_APPEND}
  mkdir -p /pnfs/dune/persistent/users/${USER}/${PNFS_PATH_APPEND}
fi

if [ "${DO_FD}" == "1" ]; then
  if [ "${DO_FHC}" == "1" ]; then
    FD_FHC_JID=$(jobsub_submit --group=${EXPERIMENT} --jobid-output-only --resource-provides=usage_model=OPPORTUNISTIC --expected-lifetime=${LIFETIME_EXP_FD} --disk=${DISK_EXP_FD} --memory=${MEM_EXP_FD} --cpu=1 --OS=SL6 --tar_file_name=dropbox://CAFAna.Blob.tar.gz file://${CAFANA}/scripts/FermiGridScripts/BuildInterps.sh ${PNFS_PATH_APPEND} FD_FHC ${NMAXEVENTS} ${AXISBLOBNAME})
    echo "FD_FHC_JID: ${FD_FHC_JID}"
  fi

  if [ "${DO_RHC}" == "1" ]; then
    FD_RHC_JID=$(jobsub_submit --group=${EXPERIMENT} --jobid-output-only --resource-provides=usage_model=OPPORTUNISTIC --expected-lifetime=${LIFETIME_EXP_FD} --disk=${DISK_EXP_FD} --memory=${MEM_EXP_FD} --cpu=1 --OS=SL6 --tar_file_name=dropbox://CAFAna.Blob.tar.gz file://${CAFANA}/scripts/FermiGridScripts/BuildInterps.sh ${PNFS_PATH_APPEND} FD_RHC ${NMAXEVENTS} ${AXISBLOBNAME})
    echo "FD_RHC_JID: ${FD_RHC_JID}"
  fi
fi

if [ "${DO_ND}" == "1" ]; then
  if [ "${DO_FHC}" == "1" ]; then
    ND_FHC_JID=$(jobsub_submit --group=${EXPERIMENT} --jobid-output-only --resource-provides=usage_model=OPPORTUNISTIC --expected-lifetime=${LIFETIME_EXP_ND} --disk=${DISK_EXP_ND} --memory=${MEM_EXP_ND} --cpu=1 --OS=SL6 --tar_file_name=dropbox://CAFAna.Blob.tar.gz file://${CAFANA}/scripts/FermiGridScripts/BuildInterps.sh ${PNFS_PATH_APPEND} ND_FHC ${NMAXEVENTS} ${AXISBLOBNAME})
    echo "ND_FHC_JID: ${ND_FHC_JID}"
  fi

  if [ "${DO_RHC}" == "1" ]; then
    ND_RHC_JID=$(jobsub_submit --group=${EXPERIMENT} --jobid-output-only --resource-provides=usage_model=OPPORTUNISTIC --expected-lifetime=${LIFETIME_EXP_ND} --disk=${DISK_EXP_ND} --memory=${MEM_EXP_ND} --cpu=1 --OS=SL6 --tar_file_name=dropbox://CAFAna.Blob.tar.gz file://${CAFANA}/scripts/FermiGridScripts/BuildInterps.sh ${PNFS_PATH_APPEND} ND_RHC ${NMAXEVENTS} ${AXISBLOBNAME})
    echo "ND_RHC_JID: ${ND_RHC_JID}"
  fi
fi
