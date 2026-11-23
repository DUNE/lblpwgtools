#!/bin/bash

echo "Running on $(hostname) at ${GLIDEIN_Site}. GLIDEIN_DUNESite = ${GLIDEIN_DUNESite}"

# Set the output location for copyback
#OUTDIR=/pnfs/dune/persistent/users/${GRID_USER}/FDGeoEffinND
OUTDIR=/pnfs/dune/scratch/users/${GRID_USER}/FDGeoEffinND/NDFV400m

# Make sure we see what we expect
echo "See where are at: pwd"
pwd

echo "ls -l"
ls -l

echo "tarball is copied and untarred at this worker node directory CONDOR_DIR_INPUT: ${CONDOR_DIR_INPUT}"

echo "ls -l CONDOR_DIR_INPUT: ${CONDOR_DIR_INPUT}"
# Tarball is copied and untarred into a directory on the worker node, accessed via this CONDOR_DIR_INPUT environment variable
ls -l $CONDOR_DIR_INPUT

echo "ls -l INPUT_TAR_DIR_LOCAL: ${INPUT_TAR_DIR_LOCAL}"
ls -l $INPUT_TAR_DIR_LOCAL

if [ -e ${INPUT_TAR_DIR_LOCAL}/setupNDEff-grid.sh ]; then
  echo "Start to run . ${INPUT_TAR_DIR_LOCAL}/setupNDEff-grid.sh"
  . ${INPUT_TAR_DIR_LOCAL}/setupNDEff-grid.sh
else
  echo "Error, setup script not found. Exiting."
  exit 1
fi

echo "Finished run setupNDEff-grid.sh"

# Go back to the top-level directory since we know that's writable
echo "cd _CONDOR_JOB_IWD: ${_CONDOR_JOB_IWD}"
cd ${_CONDOR_JOB_IWD}

echo "ls _CONDOR_JOB_IWD"
ls
echo "And ls _CONDOR_DIR_INPUT: ${_CONDOR_DIR_INPUT}"
ls ${_CONDOR_DIR_INPUT}

setup ifdhc

# Set some other very useful environment variables for xrootd and IFDH
export IFDH_CP_MAXRETRIES=2
export XRD_CONNECTIONRETRY=32
export XRD_REQUESTTIMEOUT=14400
export XRD_REDIRECTLIMIT=255
export XRD_LOADBALANCERTTL=7200
export XRD_STREAMTIMEOUT=14400 # may vary for your job/file type

# Comment the below out!!!
# # Make sure the output directory exists
# ifdh ls $OUTDIR 0 # set recursion depth to 0 since we are only checking for the directory; we don't care about the full listing.

if [ $? -ne 0 ]; then
  # if ifdh ls failed, try to make the directory
  ifdh mkdir_p $OUTDIR || { echo "Error creating or checking $OUTDIR"; exit 2; }
fi

echo "Finished checking outdir: $OUTDIR"

myinfile=""
# PROCESS starts from 0, 1, ... N-1
(( LINE_N = ${PROCESS} + 1 ))

# Loop over ntuple list in txt file
for ifile in $(cat ${INPUT_TAR_DIR_LOCAL}/myFDntuples.txt | head -${LINE_N} | tail -1); do
  myinfile=${ifile}
done

echo "Got input file: $myinfile"

# Run program
echo "cd ${_CONDOR_JOB_IWD}/DUNE_ND_GeoEff/build/bin"
cd ${_CONDOR_JOB_IWD}/DUNE_ND_GeoEff/build/bin
echo "ls -l ${_CONDOR_JOB_IWD}/DUNE_ND_GeoEff/build/bin"
ls -l ${_CONDOR_JOB_IWD}/DUNE_ND_GeoEff/build/bin
echo "./runGeoEffFDEvtSim $myinfile"
./runGeoEffFDEvtSim $myinfile

echo "Finish ./runGeoEffFDEvtSim"

# Unique name in case we send multiple jobs.
OUTFILE=FDHadronGeoEff_withMuTrackLengthInfo_${CLUSTER}_${PROCESS}.root

if [ -f Output_FDGeoEff.root ]; then

  echo "mv Output_FDGeoEff.root $OUTFILE"
  mv Output_FDGeoEff.root $OUTFILE

  # and copy our output file back
  ifdh cp -D $OUTFILE $OUTDIR

  # check the exit status to see if the copyback actually worked. Print a message if it did not.
  IFDH_RESULT=$?
  if [ $IFDH_RESULT -ne 0 ]; then
    echo "Error during output copyback. See output logs."
    exit $IFDH_RESULT
  fi
fi

#If we got this far, we succeeded.
echo "Completed successfully."
exit 0
