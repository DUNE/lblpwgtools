#!/bin/bash

# we cannot rely on "whoami" in a grid job. We have no idea what the local username will be.
# Use the GRID_USER environment variable instead (set automatically by jobsub).
USERNAME=${GRID_USER}
echo "Got username"
source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh
# this enables pip install afterwards
#setup dunetpc v09_41_00_02 -q e20:prof
#echo "dunetpc is set"
#setup root v6_28_12 -q e26:p3915:prof
#echo " root setup"
echo "setup root v6_28_10b -q e20:p3915:prof"
setup root v6_28_10b -q e20:p3915:prof

echo "which pip"
which pip

export WORKDIR=${_CONDOR_JOB_IWD} # if we use the RCDS the localProducts area will be placed in $CONDOR_DIR_INPUT
if [ ! -d "$WORKDIR" ]; then
  export WORKDIR=`echo ~`
fi

# _CONDOR_JOB_IWD is /srv
echo "Check work dir _CONDOR_JOB_IWD: ls -l ${_CONDOR_JOB_IWD}"
ls -l ${_CONDOR_JOB_IWD}

echo "See where are we: pwd"
pwd
