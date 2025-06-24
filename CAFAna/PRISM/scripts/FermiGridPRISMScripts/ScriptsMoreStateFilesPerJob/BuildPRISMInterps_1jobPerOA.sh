#!/bin/bash

LOG_TO_IFDH=0

LOGYLOG () {
  echo ${1}
  if [ ${LOG_TO_IFDH} == "1" ]; then
    ifdh log ${1}
  fi
}

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

IS_FHC="0"
if echo ${SAMPLE_NAME} | grep "FHC"; then
  IS_FHC="1"
fi

ANAVERSION="4"
if [ ! -z ${3} ]; then
  ANAVERSION="${3}"
fi

AXBLOBARG=""
if [ ! -z ${4} ]; then
  AXBLOBARG=" -A ${4}"
fi

BINDESCRIPTORARG=""
if [ ! -z ${5} ]; then
  BINDESCRIPTORARG=" --bin-descriptor ${5}"
fi

SYSTDESCRIPTORARG=""
if [ ! -z ${6} ]; then
  SYSTDESCRIPTORARG=" --syst-descriptor ${6}"
fi

NOFAKEDATAARG=""
if [ ! -z ${7} ]; then
  NOFAKEDATAARG=" --no-fakedata-dials"
fi

PRISMFAKEDATAARG=" --PRISM-fake-data MissingProtonFakeData_pos" # nominal MaCCQE_pos MissingProtonFakeData_pos
SELECTIONARG=" --UseSelection"
#SELECTIONARG=""

if [ ! -e ${INPUT_TAR_DIR_LOCAL}/CAFAna/InputCAFs.${SAMPLE_NAME}.list ]; then
  LOGYLOG "[ERROR]: Expected to recieve a CAF file list @ CAFAna/InputCAFs.${SAMPLE_NAME}.list but didn't."
  ls ${INPUT_TAR_DIR_LOCAL}/CAFAna
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

echo "Start to move files, but will see warning mv: cannot remove ${INPUT_TAR_DIR_LOCAL}/CAFAna/*': Read-only file system (ignore for now)"
mv ${INPUT_TAR_DIR_LOCAL}/CAFAna $_CONDOR_SCRATCH_DIR/

cd $_CONDOR_SCRATCH_DIR

export CAFANA=$(readlink -f CAFAna)
source ${CAFANA}/CAFAnaEnv.sh

#voms-proxy-info --all
setup ifdhc
source ${CAFANA}/CAFAnaEnv.sh

export IFDH_CP_UNLINK_ON_ERROR=1;
export IFDH_CP_MAXRETRIES=2;

PNFS_OUTDIR=/pnfs/dune/scratch/users/${GRID_USER}/${PNFS_PATH_APPEND}
LOGYLOG "Output dir is ${PNFS_OUTDIR}"

#define the off-axis positions
OAPOS_LIST=("_0m" "_1.75m" "_2m" "_4m" "_5.75m" "_8m" "_9.75m" "_12m" "_13.75m" "_16m" "_17.75m" "_20m" "_21.75m" "_24m" "_25.75m" "_26.25m" "_28m" "_28.25m" "_28.5m")
INPUT_LIST="${CAFANA}/InputCAFs.${SAMPLE_NAME}.list"
FILELIST_DIR="OAFileLists"

INPFILE=""
if [ ${IS_FD} == "1" ]; then
  #For FD we want to read all at once
  for i in $(cat ${CAFANA}/InputCAFs.${SAMPLE_NAME}.list); do
    if echo ${i} | grep "nonswap"; then
      echo "Found nonswap file: ${i}"
      if [ ${IS_FHC} == "1" ]; then
        INPFILE="${INPFILE} -F-nu ${i}"
      else
        INPFILE="${INPFILE} -F-nub ${i}"
      fi
    elif echo ${i} | grep "nueswap"; then
      echo "Found nueswap file: ${i}";
      if [ ${IS_FHC} == "1" ]; then
        INPFILE="${INPFILE} -Fe-nu ${i}"
      else
        INPFILE="${INPFILE} -Fe-nub ${i}"
      fi
    elif echo ${i} | grep "tauswap"; then
      echo "Found tauswap file: ${i}";
      if [ ${IS_FHC} == "1" ]; then
        INPFILE="${INPFILE} -Ft-nu ${i}"
      else
        INPFILE="${INPFILE} -Ft-nub ${i}"
      fi
    fi;
  done

else
  mkdir -p "$FILELIST_DIR"
  ls
  # Prepare input file lists if they don't exist
  if [ ! -f "${FILELIST_DIR}/.prepared" ]; then
    echo "Preparing OA input files..."

    JOB_INDEX=0
    for tag in "${OAPOS_LIST[@]}"; do
      echo "tag: " ${tag}
      echo "input caf: " ${INPUT_LIST} 
      CLEAN_TAG=${tag#_}  # e.g., "_28m_" → "28m"
      OUTFILE="${FILELIST_DIR}/Input_${CLEAN_TAG}.list"

      grep "${tag}" "$INPUT_LIST" > "$OUTFILE"
      if [ -s "$OUTFILE" ]; then
        JOB_INDEX=$((JOB_INDEX + 1))
      else
        echo "No files found for tag ${tag}, skipping..."
        rm -f "$OUTFILE"
      fi
    done

    touch "${FILELIST_DIR}/.prepared"
  fi


  # Map $PROCESS to OA tag
  OAPOS_SHORT=("0m" "1.75m" "2m" "4m" "5.75m" "8m" "9.75m" "12m" "13.75m" "16m" "17.75m" "20m" "21.75m" "24m" "25.75m" "26.25m" "28m" "28.25m" "28.5m")
  TAG=${OAPOS_SHORT[$PROCESS]}
  INFILE="${FILELIST_DIR}/Input_${TAG}.list"

  if [ ! -f "$INFILE" ]; then
    echo "Error: No input file for OA tag '${TAG}' at index $PROCESS"
    exit 1
  fi

  # Build INPFILE argument
  if [ "${IS_FHC}" == "1" ]; then
    INPFILE="-N-nu ${INFILE}"
  else
    INPFILE="-N-nub ${INFILE}"
  fi

  echo "[$(date)] Starting PROCESS=$PROCESS with OA=${TAG}"
  echo "Input file: ${INFILE}"
  echo "Command: ./MakePRISMPredInterps ${INPFILE}"



fi


ifdh ls ${PNFS_OUTDIR}

if [ $? -ne 0 ]; then
  LOGYLOG "Unable to read ${PNFS_OUTDIR}. Make sure that you have created this directory and given it group write permission (chmod g+w ${PNFS_OUTDIR})."
  exit 10
fi

LOGYLOG "Building interps @ $(date)"

OUTFILENAME=""
if [ ${IS_FD} == "1" ]; then
  OUTFILENAME=${SAMPLE_NAME}.FDState.${CLUSTER}.${PROCESS}.root
else
  OUTFILENAME=${SAMPLE_NAME}.TESTNDState.${CLUSTER}.${PROCESS}_${OAPOS_SHORT[$PROCESS]}.root
fi

LOGYLOG "Output file name: ${OUTFILENAME}"

export CAFANA_ANALYSIS_VERSION=${ANAVERSION}
echo "CAFANA_ANALYSIS_VERSION=${CAFANA_ANALYSIS_VERSION}"

source ${CAFANA}/CAFAnaEnv.sh

export PRISM_MULTIFILE="1"
echo "PRISM_MULTIFILE=${PRISM_MULTIFILE}"

LOGYLOG "MakePRISMPredInterps -o ${OUTFILENAME} \
                              ${INPFILE} \
                              ${AXBLOBARG} \
                              ${BINDESCRIPTORARG} \
                              ${SYSTDESCRIPTORARG} \
                              ${NOFAKEDATAARG} \
                              ${PRISMFAKEDATAARG} \
                              ${SELECTIONARG}"

MakePRISMPredInterps -o ${OUTFILENAME} \
                     ${INPFILE} \
                     ${AXBLOBARG} \
                     ${BINDESCRIPTORARG} \
                     ${SYSTDESCRIPTORARG} \
                     ${NOFAKEDATAARG} \
                     ${PRISMFAKEDATAARG} \
                     ${SELECTIONARG}

LOGYLOG "Copying output @ $(date)"

if [ ! -e ${OUTFILENAME} ]; then
  LOGYLOG "[ERROR]: Failed to produce expected output file."
  exit 1
fi

LOGYLOG "ifdh cp -D $IFDH_OPTION ${OUTFILENAME} ${PNFS_OUTDIR}/"
ifdh cp -D $IFDH_OPTION ${OUTFILENAME} ${PNFS_OUTDIR}/

LOGYLOG "All stop @ $(date)"
