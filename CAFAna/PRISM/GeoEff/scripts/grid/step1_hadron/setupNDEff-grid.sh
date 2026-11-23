#!/bin/bash

# we cannot rely on "whoami" in a grid job. We have no idea what the local username will be.
# Use the GRID_USER environment variable instead (set automatically by jobsub).
USERNAME=${GRID_USER}
echo "Got username"

export WORKDIR=${_CONDOR_JOB_IWD} # if we use the RCDS the localProducts area will be placed in $CONDOR_DIR_INPUT
if [ ! -d "$WORKDIR" ]; then
  export WORKDIR=`echo ~`
fi

# _CONDOR_JOB_IWD is /srv
echo "Check work dir _CONDOR_JOB_IWD: ls -l ${_CONDOR_JOB_IWD}"
ls -l ${_CONDOR_JOB_IWD}
cd ${_CONDOR_JOB_IWD}

echo "See where are at: pwd"
pwd

# Copy the untarred folder and remove the CVMFS linked read-only version
# because recompile need to modify files
# which can't be done in the CVMFS read-only version
#echo "git clone --recurse-submodules -b master https://github.com/icaracas/DUNE_ND_GeoEff.git "
#git clone --recurse-submodules -b master https://github.com/icaracas/DUNE_ND_GeoEff.git
# If no clone then copy tarball contents
echo "Moving contents of tarball to: ${_CONDOR_JOB_IWD}"
cp -r ${INPUT_TAR_DIR_LOCAL}/* ${_CONDOR_JOB_IWD}
echo "cd ${_CONDOR_JOB_IWD}/DUNE_ND_GeoEff"
cd ${_CONDOR_JOB_IWD}/DUNE_ND_GeoEff
echo "source scripts/env/setup_step1.sh"
source scripts/env/setup_step1.sh
# 1️⃣ Point to the GCC 9.3 compiler explicitly
export CC=/cvmfs/larsoft.opensciencegrid.org/products/gcc/v9_3_0/Linux64bit+3.10-2.17/bin/gcc
export CXX=/cvmfs/larsoft.opensciencegrid.org/products/gcc/v9_3_0/Linux64bit+3.10-2.17/bin/g++
# 2️⃣ Optional: make sure this GCC is first in PATH
export PATH=/cvmfs/larsoft.opensciencegrid.org/products/gcc/v9_3_0/Linux64bit+3.10-2.17/bin:$PATH
echo "cmake -S . -B build -DCMAKE_CXX_STANDARD=17 -DPYTHON_EXECUTABLE:FILEPATH=`which python3`"
cmake -S . -B build -DCMAKE_CXX_STANDARD=17 -DPYTHON_EXECUTABLE:FILEPATH=`which python3`
echo "cmake --build build --target runGeoEffFDEvtSim -j"
cmake --build build --target runGeoEffFDEvtSim -j
