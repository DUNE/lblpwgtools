#!/bin/bash

PNFS_PATH_APPEND=CAFAnaInputs/StandardState
AXISBLOBNAME="default"
FORCE_REMOVE="0"
INPUT_DIR=""

LIFETIME_EXP_ND="1h"
DISK_EXP_ND="4GB"
MEM_EXP_ND="1.6GB"

LIFETIME_EXP_FD="6h"
DISK_EXP_FD="4GB"
MEM_EXP_FD="2GB"

SYSTDESCRIPTOR=""
NOFAKEDATA=""

ANAVERSION="4"

DO_FD="1"
DO_ND="1"

DO_FHC="1"
DO_RHC="1"

NMAXJOBS="100000"

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

      --syst-descriptor)

      if [[ ${#} -lt 2 ]]; then
        echo "[ERROR]: ${1} expected a value."
        exit 1
      fi

      SYSTDESCRIPTOR="$2"
      echo "[OPT]: Using --syst-descriptor argument SYSTDESCRIPTOR:\"${SYSTDESCRIPTOR}\"."
      shift # past argument
      ;;

      --no-fakedata-dials)

      NOFAKEDATA="1"
      echo "[OPT]: Don't use fake data dials."
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

      --nmax)

      if [[ ${#} -lt 2 ]]; then
        echo "[ERROR]: ${1} expected a value."
        exit 1
      fi

      NMAXJOBS="$2"
      echo "[OPT]: Will run a max of ${NMAXJOBS} jobs."
      shift # past argument
      ;;

      -?|--help)
      echo "[RUNLIKE] ${SCRIPTNAME}"
      echo -e "\t-p|--pnfs-path-append      : Path to append to output path: /pnfs/dune/persistent/users/${USER}/"
      echo -e "\t-i|--input-dir             : Path to search for files in."
      echo -e "\t-a|--axis-argument         : Argument to pass to remake_inputs to specify the axes"
      echo -e "\t--syst-descriptor          : Syst descriptor string to pass."
      echo -e "\t--no-fakedata-dials        : Remove fake data dials."
      echo -e "\t--nmax                     : Will run a maximum of --nmax jobs"
      echo -e "\t-f|--force-remove          : If files are found in the output directory, remove them."
      echo -e "\t-N|--ND-Only               : Only run ND."
      echo -e "\t-F|--FD-Only               : Only run FD."
      echo -e "\t-u|--FHC-Only              : Only run FHC."
      echo -e "\t-b|--RHC-Only              : Only run RHC."
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

if ! kx509; then
  echo "[ERROR]: Failed to setup kx509."
  exit 1
fi
if ! voms-proxy-info -exists; then
  voms-proxy-init -rfc -noregen -voms dune:/dune/Role=Analysis
  voms-proxy-info --all
fi

NJOBS=0

if [ "${DO_FD}" == "1" ]; then
  if [ "${DO_FHC}" == "1" ]; then
    echo "ifdh ls ${INPUT_DIR} | grep \"FD_FHC.*.root\" | sed \"s\/pnfs\root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr\g\""
    ifdh ls ${INPUT_DIR} | grep "FD_FHC.*.root" | sed "s\/pnfs\root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr\g" > InputCAFs.FD_FHC.list
    NFD_FHC=1
    NFD_FHC_FILES=$(cat InputCAFs.FD_FHC.list | wc -l)
    echo "[INFO]: Found ${NFD_FHC_FILES} FD_FHC input files"
    NJOBSWITHTHIS=$(( NJOBS + NFD_FHC ))
    if [ ${NJOBSWITHTHIS} -gt ${NMAXJOBS} ]; then
      NFD_FHC=${NMAXJOBS}
      DO_RHC="0"
      DO_ND="0"
    fi
    NJOBS=$(( NJOBS + NFD_FHC))
    echo "Will run ${NFD_FHC} FD_FHC jobs."
  fi
  if [ "${DO_RHC}" == "1" ]; then
    echo "ifdh ls ${INPUT_DIR} | grep \"FD_RHC.*.root\" | sed \"s\/pnfs\root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr\g\""
    ifdh ls ${INPUT_DIR} | grep "FD_RHC.*.root" | sed "s\/pnfs\root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr\g" > InputCAFs.FD_RHC.list
    NFD_RHC=1
    NFD_RHC_FILES=$(cat InputCAFs.FD_RHC.list | wc -l)
    echo "[INFO]: Found ${NFD_RHC_FILES} FD_RHC input files"
    NJOBSWITHTHIS=$(( NJOBS + NFD_RHC ))
    if [ ${NJOBSWITHTHIS} -gt ${NMAXJOBS} ]; then
      NFD_RHC=$(( NMAXJOBS - NJOBS ))
      DO_ND="0"
    fi
    NJOBS=$(( NJOBS + NFD_RHC))
    echo "Will run ${NFD_RHC} FD_RHC jobs."
  fi
fi
if [ "${DO_ND}" == "1" ]; then
  if [ "${DO_FHC}" == "1" ]; then
    echo "ifdh ls ${INPUT_DIR} | grep \"ND_FHC.*.root\" | sed \"s\/pnfs\root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr\g\""
    ifdh ls ${INPUT_DIR} | grep "ND_FHC.*.root" | sed "s\/pnfs\root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr\g" > InputCAFs.ND_FHC.list
    NND_FHC=$(cat InputCAFs.ND_FHC.list | wc -l)
    echo "[INFO]: Found ${NND_FHC} ND_FHC input files"
    NJOBSWITHTHIS=$(( NJOBS + NND_FHC ))
    if [ ${NJOBSWITHTHIS} -gt ${NMAXJOBS} ]; then
      NND_FHC=$(( NMAXJOBS - NJOBS ))
      DO_RHC="0"
    fi
    NJOBS=$(( NJOBS + NND_FHC))
    echo "Will run ${NND_FHC} ND_FHC jobs."
  fi

  if [ "${DO_RHC}" == "1" ]; then
    echo "ifdh ls ${INPUT_DIR} | grep \"ND_RHC.*.root\" | sed \"s\/pnfs\root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr\g\""
    ifdh ls ${INPUT_DIR} | grep "ND_RHC.*.root" | sed "s\/pnfs\root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr\g" > InputCAFs.ND_RHC.list
    NND_RHC=$(cat InputCAFs.ND_RHC.list | wc -l)
    echo "[INFO]: Found ${NND_RHC} ND_RHC input files"
    NJOBSWITHTHIS=$(( NJOBS + NND_RHC ))
    if [ ${NJOBSWITHTHIS} -gt ${NMAXJOBS} ]; then
      NND_RHC=$(( NMAXJOBS - NJOBS ))
    fi
    NJOBS=$(( NJOBS + NND_RHC))
    echo "Will run ${NND_RHC} ND_RHC jobs."
  fi
fi

echo "[INFO]: Will run ${NJOBS} jobs in total."

rm -f CAFAna.Blob.tar.gz
${CAFANA}/scripts/FermiGridScripts/tarball.sh

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
    FD_FHC_JID=$(jobsub_submit --group=${EXPERIMENT} --jobid-output-only --resource-provides=usage_model=OPPORTUNISTIC --expected-lifetime=${LIFETIME_EXP_FD} --disk=${DISK_EXP_FD} --memory=${MEM_EXP_FD} --cpu=1 --OS=SL7 --tar_file_name=dropbox://CAFAna.Blob.tar.gz file://${CAFANA}/scripts/FermiGridScripts/BuildInterps.sh ${PNFS_PATH_APPEND} FD_FHC ${ANAVERSION} ${AXISBLOBNAME} ${SYSTDESCRIPTOR} ${NOFAKEDATA})
    echo "FD_FHC_JID: ${FD_FHC_JID}"
  fi

  if [ "${DO_RHC}" == "1" ]; then
    FD_RHC_JID=$(jobsub_submit --group=${EXPERIMENT} --jobid-output-only --resource-provides=usage_model=OPPORTUNISTIC --expected-lifetime=${LIFETIME_EXP_FD} --disk=${DISK_EXP_FD} --memory=${MEM_EXP_FD} --cpu=1 --OS=SL7 --tar_file_name=dropbox://CAFAna.Blob.tar.gz file://${CAFANA}/scripts/FermiGridScripts/BuildInterps.sh ${PNFS_PATH_APPEND} FD_RHC ${ANAVERSION} ${AXISBLOBNAME} ${SYSTDESCRIPTOR} ${NOFAKEDATA})
    echo "FD_RHC_JID: ${FD_RHC_JID}"
  fi
fi

if [ "${DO_ND}" == "1" ]; then
  if [ "${DO_FHC}" == "1" ]; then
    ND_FHC_JID=$(jobsub_submit --group=${EXPERIMENT} --jobid-output-only --resource-provides=usage_model=OPPORTUNISTIC --expected-lifetime=${LIFETIME_EXP_ND} --disk=${DISK_EXP_ND} --memory=${MEM_EXP_ND} --cpu=1 --OS=SL7 -N ${NND_FHC} --tar_file_name=dropbox://CAFAna.Blob.tar.gz file://${CAFANA}/scripts/FermiGridScripts/BuildInterps.sh ${PNFS_PATH_APPEND} ND_FHC ${ANAVERSION} ${AXISBLOBNAME} ${SYSTDESCRIPTOR} ${NOFAKEDATA})
    echo "ND_FHC_JID: ${ND_FHC_JID}"
  fi

  if [ "${DO_RHC}" == "1" ]; then
    ND_RHC_JID=$(jobsub_submit --group=${EXPERIMENT} --jobid-output-only --resource-provides=usage_model=OPPORTUNISTIC --expected-lifetime=${LIFETIME_EXP_ND} --disk=${DISK_EXP_ND} --memory=${MEM_EXP_ND} --cpu=1 --OS=SL7 -N ${NND_RHC} --tar_file_name=dropbox://CAFAna.Blob.tar.gz file://${CAFANA}/scripts/FermiGridScripts/BuildInterps.sh ${PNFS_PATH_APPEND} ND_RHC ${ANAVERSION} ${AXISBLOBNAME} ${SYSTDESCRIPTOR} ${NOFAKEDATA})
    echo "ND_RHC_JID: ${ND_RHC_JID}"
  fi
fi
