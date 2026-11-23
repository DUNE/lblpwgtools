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
#echo "git clone --recurse-submodules -b EtrimAnalysis https://github.com/icaracas/DUNE_ND_GeoEff.git "
#git clone --recurse-submodules -b EtrimAnalysis https://github.com/icaracas/DUNE_ND_GeoEff.git
#echo "cd ${_CONDOR_JOB_IWD}/DUNE_ND_GeoEff"
#cd ${_CONDOR_JOB_IWD}/DUNE_ND_GeoEff
echo "Moving contents of tarball to: ${_CONDOR_JOB_IWD}"
cp -r ${INPUT_TAR_DIR_LOCAL}/* ${_CONDOR_JOB_IWD}
echo "cd ${_CONDOR_JOB_IWD}/DUNE_ND_GeoEff"
cd ${_CONDOR_JOB_IWD}/DUNE_ND_GeoEff
echo "source scripts/env/setup_step3.sh"
source scripts/env/setup_step3.sh
#echo "cmake -DPYTHON_EXECUTABLE:FILEPATH=`which python` ."
#cmake -DPYTHON_EXECUTABLE:FILEPATH=`which python` .
# echo "make -j geoEff"
# make -j geoEff


echo "building Etrim analysis with CMake"
cmake -S . -B build -DNDGEOEFF_BUILD_STEP1=OFF -DNDGEOEFF_BUILD_STEP3=ON -DNDGEOEFF_BUILD_PYTHON=OFF
cmake --build build -j
