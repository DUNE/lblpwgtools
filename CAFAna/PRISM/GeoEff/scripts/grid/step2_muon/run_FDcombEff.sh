#!/bin/bash

echo "Running on $(hostname) at ${GLIDEIN_Site}. GLIDEIN_DUNESite = ${GLIDEIN_DUNESite}"

# Set the output location for copyback
#OUTDIR=/pnfs/dune/persistent/users/${GRID_USER}/myFDntuples
#OUTDIR=/pnfs/dune/scratch/users/${GRID_USER}/test/FDGeoEffinND/MuonEff
OUTDIR=/pnfs/dune/scratch/users/${GRID_USER}/test/output_test

# Make sure we see what we expect
echo "See where are at: pwd" # this normally is _CONDOR_JOB_IWD
pwd

echo "tarball is copied and untarred at this worker node directory CONDOR_DIR_INPUT: ${CONDOR_DIR_INPUT}"

echo "ls -l CONDOR_DIR_INPUT"
# Tarball is copied and untarred into a directory on the worker node, accessed via this CONDOR_DIR_INPUT environment variable
ls -l $CONDOR_DIR_INPUT

echo "ls -l INPUT_TAR_DIR_LOCAL: ${INPUT_TAR_DIR_LOCAL} (should see untarred files)"
ls -l $INPUT_TAR_DIR_LOCAL

if [ -e ${INPUT_TAR_DIR_LOCAL}/setup_FDcombEff.sh ]; then
  echo "Start to run setup_FDcombEff.sh"
  . ${INPUT_TAR_DIR_LOCAL}/setup_FDcombEff.sh
else
  echo "Error, setup script not found. Exiting."
  exit 1
fi

echo "Finished run setupFDEffTarBall-grid.sh"

# Go back to the top-level directory since we know that's writable
echo "cd _CONDOR_JOB_IWD: ${_CONDOR_JOB_IWD}"
cd ${_CONDOR_JOB_IWD}

echo "Install dependencies"
echo "pip install --target=${_CONDOR_JOB_IWD} numpy==1.26.1"
/cvmfs/larsoft.opensciencegrid.org/products/python/v3_9_15/Linux64bit+3.10-2.17/bin/pip install --target=${_CONDOR_JOB_IWD} numpy==1.26.1
echo "pip install --target=${_CONDOR_JOB_IWD} uproot"
/cvmfs/larsoft.opensciencegrid.org/products/python/v3_9_15/Linux64bit+3.10-2.17/bin/pip install --target=${_CONDOR_JOB_IWD} uproot==5.1.2
# echo "pip install --target=${_CONDOR_JOB_IWD} uproot4"
# pip install --target=${_CONDOR_JOB_IWD} uproot4
# echo "pip install --target=${_CONDOR_JOB_IWD} uproot3"
# pip install --target=${_CONDOR_JOB_IWD} uproot3
echo "pip install --target=${_CONDOR_JOB_IWD} torch"
/cvmfs/larsoft.opensciencegrid.org/products/python/v3_9_15/Linux64bit+3.10-2.17/bin/pip install --target=${_CONDOR_JOB_IWD} torch

echo "pip install --target=${_CONDOR_JOB_IWD} scipy==1.11.3"
/cvmfs/larsoft.opensciencegrid.org/products/python/v3_9_15/Linux64bit+3.10-2.17/bin/pip install --target=${_CONDOR_JOB_IWD} scipy==1.11.3

echo "python version: "
python --version
python -c "import numpy; import scipy; print(numpy.__version__, scipy.__version__)"
pip show uproot

export PYTHONPATH=${_CONDOR_JOB_IWD}:$PYTHONPATH
echo "which root"
which root

echo "ls -l _CONDOR_JOB_IWD"
ls -l ${_CONDOR_JOB_IWD}

# The tarball contains the flat app/ directory.


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
basefile=""
inputfile=""
# PROCESS starts from 0, 1, ... N-1
(( LINE_N = ${PROCESS} + 1 ))

# Loop over file list in txt file
for ifile in $(cat ${INPUT_TAR_DIR_LOCAL}/FDCAFs.txt | head -${LINE_N} | tail -1); do
  myinfile="${myinfile} ${ifile}"
  basefile=$(basename $ifile)
  inputfile="${basefile%.root}"
  echo "Input file without .root extension: $inputfile"
done

echo "Got xrootd url: $myinfile"

# copy the input file to working dir becassue python3 can't read via root://fndca1.fnal.gov:1094
# this copy should be avoided however for large size files
echo "ifdh cp -D $myinfile ${_CONDOR_JOB_IWD}"
ifdh cp -D $myinfile ${_CONDOR_JOB_IWD}

echo "ls -l _CONDOR_JOB_IWD"
ls -l ${_CONDOR_JOB_IWD}

echo "python3 ${INPUT_TAR_DIR_LOCAL}/app/make_tree_fd.py ./*.root"
python3 ${INPUT_TAR_DIR_LOCAL}/app/make_tree_fd.py ./*.root
LAR_RESULT=$?   # check the exit status!!!

if [ $LAR_RESULT -ne 0 ]; then
  echo "lar exited with abnormal status $LAR_RESULT. See error outputs."
  exit $LAR_RESULT
fi

echo "Have output"

echo "ls -l _CONDOR_JOB_IWD"
ls -l ${_CONDOR_JOB_IWD}

# Unique name in case we send multiple jobs.
# OUTFILE=FDGeoEff_${CLUSTER}_${PROCESS}.root
OUTFILE=${inputfile}_MuCombinedEff.root

if [ -f Output_FDGeoEff.root ]; then

  echo "mv Output_FDGeoEff.root $OUTFILE"
  mv Output_FDGeoEff.root $OUTFILE

  echo "Found output root file, now copy to scratch area: $OUTDIR"
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
