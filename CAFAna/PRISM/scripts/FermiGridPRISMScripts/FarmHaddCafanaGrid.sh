#!/bin/bash

PNFS_PATH_APPEND=CAFAnaInputs/StandardState/ELepEHadReco/Hadded
FORCE_REMOVE="0"
INPUT_DIR=""

LIFETIME_EXP_FD="60h"
DISK_EXP_FD="5GB"
MEM_EXP_FD="65GB"

ANAVERSION="4"

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

      -i|--input-dir)

      if [[ ${#} -lt 2 ]]; then
        echo "[ERROR]: ${1} expected a value."
        exit 1
      fi

      INPUT_DIR="$2"
      echo "[OPT]: Will read files from:\"${INPUT_DIR}\"."
      shift # past argument
      ;;

      -v|--analysis-version)

      if [[ ${#} -lt 2 ]]; then
        echo "[ERROR]: ${1} expected a value."
        exit 1
      fi

      ANAVERSION="$2"
      echo "[OPT]: Using AnaVersion: \"${ANAVERSION}\"."
      shift # past argument
      ;;

      -f|--force-remove)

      FORCE_REMOVE="1"
      echo "[OPT]: Will remove extant files in output directory."
      ;;

      -?|--help)
      echo "[RUNLIKE] ${SCRIPTNAME}"
      echo -e "\t-p|--pnfs-path-append      : Path to append to output path: /pnfs/dune/persistent/users/${USER}/"
      echo -e "\t-i|--input-dir             : Path to search for files in."
      echo -e "\t-f|--force-remove          : If files are found in the output directory, remove them."
      echo -e "\t-v|--analysis-version <v=4>: Set analysis version, (3 or 4)."
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
${CAFANA}/CAFAnaEnv.sh
ups active

if ! kx509; then
  echo "[ERROR]: Failed to setup kx509."
  exit 1
fi
if ! voms-proxy-info -exists; then
  voms-proxy-init -rfc -noregen -voms dune:/dune/Role=Analysis
  voms-proxy-info --all
fi

NJOBS=1

ifdh ls ${INPUT_DIR}

echo "ifdh ls ${INPUT_DIR} | grep \".root\" | sed \"s\/pnfs\root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr\g\""
ifdh ls ${INPUT_DIR} | grep ".root" | sed "s\/pnfs\root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr\g" > InputStateFiles.list

echo "[INFO]: Will run ${NJOBS} jobs in total."

rm -f CAFAna.Blob.tar.gz
${CAFANA}/scripts/FermiGridScripts/tarballPRISM.sh

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

setup jobsub_client
${CAFANA}/CAFAnaEnv.sh

Hadd_JID=$(jobsub_submit --group=${EXPERIMENT} --jobid-output-only --append_condor_requirements="(TARGET.HAS_CVMFS_fifeuser1_opensciencegrid_org==true&&TARGET.HAS_CVMFS_fifeuser2_opensciencegrid_org==true&&TARGET.HAS_CVMFS_fifeuser3_opensciencegrid_org==true&&TARGET.HAS_CVMFS_fifeuser4_opensciencegrid_org==true)" --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC,OFFSITE --expected-lifetime=${LIFETIME_EXP_FD} --disk=${DISK_EXP_FD} --memory=${MEM_EXP_FD} -l 'FERMIHTC_AutoRelease==True' -l 'FERMIHTC_GraceLifetime==36000' --cpu=1 --OS=SL7 --tar_file_name=dropbox://CAFAna.Blob.tar.gz file://${CAFANA}/scripts/FermiGridScripts/HaddCafana.sh ${PNFS_PATH_APPEND} ${ANAVERSION})

echo "Hadd_JID: ${Hadd_JID}"
