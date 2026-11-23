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
echo "Moving contents of tarball to: ${_CONDOR_JOB_IWD}"
cp -r ${INPUT_TAR_DIR_LOCAL}/* ${_CONDOR_JOB_IWD}
# The tarball contains lblpwgtools/CAFAna/PRISM/GeoEff (see README.md)
export GEOEFF_DIR=${_CONDOR_JOB_IWD}/lblpwgtools/CAFAna/PRISM/GeoEff
echo "cd ${GEOEFF_DIR}"
cd ${GEOEFF_DIR}
echo "source scripts/env/setup_step3.sh"
source scripts/env/setup_step3.sh


# Standalone GeoEff build (no CAFAna needed). The oscillated apps compile
# lblpwgtools/CAFAna/Analysis/Calcs.cxx, so the tarball must include it.
echo "building Etrim analysis with CMake"
cmake -S . -B build
cmake --build build -j
