#!/bin/bash

echo "Running on $(hostname) at ${GLIDEIN_Site}. GLIDEIN_DUNESite = ${GLIDEIN_DUNESite}"

# Set the output location for copyback
OUTDIR=/pnfs/dune/scratch/users/${GRID_USER}/CAFAna/numuCC_selection_raw

# Let's rename the output file so it's unique in case we send multiple jobs
OUTFILE=numuCC_selection_raw_${CLUSTER}_${PROCESS}_$(date -u +%Y%m%d).root

# Make sure we see what we expect
pwd

ls -l $CONDOR_DIR_INPUT

# cd back to the top-level directory since we know that's writable
cd ${_CONDOR_JOB_IWD}

if [ -e ${INPUT_TAR_DIR_LOCAL}/setup_grid.sh ]; then    
    . ${INPUT_TAR_DIR_LOCAL}/setup_grid.sh
else
  echo "Error, setup script not found. Exiting."
  exit 1
fi

# Set some other very useful environment variables for xrootd and IFDH
export IFDH_CP_MAXRETRIES=2
export XRD_CONNECTIONRETRY=32
export XRD_REQUESTTIMEOUT=14400
export XRD_REDIRECTLIMIT=255
export XRD_LOADBALANCERTTL=7200
export XRD_STREAMTIMEOUT=14400 # many vary for your job/file type

# Make sure the output directory exists
#ifdh ls $OUTDIR 0 # set recursion depth to 0 since we are only checking for the directory; we don't care about the full listing.
gfal-stat $OUTDIR

if [ $? -ne 0 ]; then
    # if ifdh ls failed, try to make the directory
    ifdh mkdir_p $OUTDIR || { echo "Error creating or checking $OUTDIR"; exit 2; }
fi

# Copy script directory to current working directory
cp -r ${INPUT_TAR_DIR_LOCAL}/GArAnaScripts ./GArAnaScripts

#load file with input parameters
input_file="${INPUT_TAR_DIR_LOCAL}/input_params_numucc_raw.dat"
line=`expr "${PROCESS}" + 1`
input_line=$(sed "${line}!d" ${input_file})
params_array=($input_line)

cafe -bq GArAnaScripts/numuCC_selection_raw.C ${params_array[0]} ${params_array[1]}

# Copy back outputs
if [ -f numuCC_selection_raw.root ]; then

    # Rename output file(s)...
    mv numuCC_selection_raw.root $OUTFILE
    
    # ...and copy back
    ifdh cp -D $OUTFILE $OUTDIR

    # Check the exit status to see if the copyback actually worked. Print a message if it did not
    IFDH_RESULT=$?
    if [ $IFDH_RESULT -ne 0 ]; then
	echo "Error during output copyback. See output logs."
	exit $IFDH_RESULT
    fi
fi

# If we got this far, we succeeded
echo "Completed successfully."
exit 0