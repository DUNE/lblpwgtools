#!/bin/bash

PNFS_PATH_APPEND=CAFAnaOutputs/MyOutput
CAFE_COMMAND_FILE="CAFECommands.cmd"
SCRIPT_TO_INCLUDE=""

LIFETIME_EXP="4h"
DISK_EXP="1GB"
MEM_EXP="2GB"

FORCE_REMOVE="0"

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

      -c|--cafe-command-file)

      if [[ ${#} -lt 2 ]]; then
        echo "[ERROR]: ${1} expected a value."
        exit 1
      fi

      CAFE_COMMAND_FILE="$2"
      echo "[OPT]: Running jobs for each line in \"${CAFE_COMMAND_FILE}\"."
      shift # past argument
      ;;

      -f|--force-remove)

      FORCE_REMOVE="1"
      echo "[OPT]: Will remove extant files in output directory."
      ;;

      --cafe-commands)

      shift
      echo "[OPT]: Will pass ${@} to cafe."
      break;
      ;;

      -?|--help)
      echo "[RUNLIKE] ${SCRIPTNAME} [opts] --cafe-comands <args>"
      echo -e "\t-p|--pnfs-path-append      : Path to append to output path: /pnfs/dune/persistent/users/${USER}/"
      echo -e "\t-c|--cafe-command-file     : File containing <script name>: [arg1 [arg2 [...]]]. One job is submitted per line in the input file."
      echo -e "\t--cafe-commands            : All arguments passed after this will be passed to cafe on the node."
      echo -e "\t-f|--force-remove          : Removes output directory before starting."
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

if ! mkdir sub_dir; then
  echo "[ERROR]; Tried to make transient submission directory (sub_dir), but one exists."
  exit
fi

cd sub_dir;

if [ ${#} -gt 0 ]; then
  echo ${@} > CAFECommands.cmd
fi

if [ ! -z ${CAFE_COMMAND_FILE} ]; then
  cp ${CAFE_COMMAND_FILE} CAFECommands.cmd
fi

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

NJOBSTORUN=$(cat CAFECommands.cmd | wc -l)

if [ ! -z "${SCRIPT_TO_INCLUDE}" ]; then
  SCRIPT_NAME=${SCRIPT_TO_INCLUDE##*/}
  ${CAFANA}/scripts/FermiGridScripts/tarball.sh ${SCRIPT_TO_INCLUDE}
else
  ${CAFANA}/scripts/FermiGridScripts/tarball.sh
fi

if [ ! -e CAFAna.Blob.tar.gz ]; then
  echo "[ERROR]: Failed to build source tarball."
  exit 2
fi

for i in $(cat CAFECommands.cmd | cut -f 1 -d " "); do

  if [ ! -e ${CAFANA}/scripts/${i} ] && [ "${i}" != "${SCRIPT_NAME}" ]; then
    echo "Trying to run job for script ${i}. But it doesn't exist in ${CAFANA}/scripts/ and wasn't passed with -s. The script needs to be installed with CAFAna or passed to this submission script to run."
    exit
  fi

done

if [ ${NJOBSTORUN} -eq 1 ]; then
  JID=$(jobsub_submit --group=${EXPERIMENT} --jobid-output-only --resource-provides=usage_model=OPPORTUNISTIC --expected-lifetime=${LIFETIME_EXP} --disk=${DISK_EXP} --memory=${MEM_EXP} --cpu=1 --OS=SL6 --tar_file_name=dropbox://CAFAna.Blob.tar.gz file://${CAFANA}/scripts/FermiGridScripts/CAFENodeScript.sh ${PNFS_PATH_APPEND} )
else
  JID=$(jobsub_submit --group=${EXPERIMENT} --jobid-output-only --resource-provides=usage_model=OPPORTUNISTIC -N ${NJOBSTORUN} --expected-lifetime=${LIFETIME_EXP} --disk=${DISK_EXP} --memory=${MEM_EXP} --cpu=1 --OS=SL6 --tar_file_name=dropbox://CAFAna.Blob.tar.gz file://${CAFANA}/scripts/FermiGridScripts/CAFENodeScript.sh ${PNFS_PATH_APPEND} )
fi

echo "JID = ${JID}"

cd ../; rm -r sub_dir
