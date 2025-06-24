#!/bin/bash


SAMPLE_NAME="ND_RHC"
#define the off-axis positions
OAPOS_LIST=("_0m_" "_1.75m_" "_2m_" "_4m_" "_5.75m_" "_8m_" "_9.75m_" "_12m_" "_13.75m_" "_16m_" "_17.75m_" "_20m_" "_21.75m_" "_24m_" "_25.75m_" "_26.25m_" "_28m_" "_28.25m_" "_28.5m_")
INPUT_LIST="InputCAFs.${SAMPLE_NAME}.list"
FILELIST_DIR="OAFileLists"



mkdir -p "$FILELIST_DIR"
# Prepare input file lists if they don't exist
if [ ! -f "${FILELIST_DIR}/.prepared" ]; then
  echo "Preparing OA input files..."

  JOB_INDEX=0
  for tag in "${OAPOS_LIST[@]}"; do
    echo "tag: " ${tag}
    CLEAN_TAG=${tag//_/}  # e.g., "_28m_" → "28m"
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

