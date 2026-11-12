#!/bin/bash
# set -e

# Run this as a bash script. The job submission is done internally.

# make software tarball for transfer to the cluster
TARBALL_SOURCE=/nashome/l/lorenzog/PRISM_SW/lblpwgtools/CAFAna/build/Linux
TARBALL_DEST=/exp/dune/app/users/lorenzog/cafana.tar.gz
tar czf ${TARBALL_DEST} -C ${TARBALL_SOURCE} .

MODE="fd_nu"  # default mode
if [ $# -gt 0 ]; then
  MODE=$1
fi

ARGS=(
  -G dune --mail_always
  -N 1
  --memory=16GB --disk=10GB --cpu=1 --expected-lifetime=8h
  --singularity-image /cvmfs/singularity.opensciencegrid.org/fermilab/fnal-wn-sl7:latest
  --tar_file_name=dropbox://${TARBALL_DEST}
  --append_condor_requirements='(TARGET.HAS_CVMFS_fifeuser1_opensciencegrid_org==true&&TARGET.HAS_CVMFS_fifeuser2_opensciencegrid_org==true&&TARGET.HAS_CVMFS_fifeuser3_opensciencegrid_org==true&&TARGET.HAS_CVMFS_fifeuser4_opensciencegrid_org==true)'
  --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC,OFFSITE
  -e GFAL_PLUGIN_DIR=/usr/lib64/gfal2-plugins
  -e GFAL_CONFIG_DIR=/etc/gfal2.d
  file:///nashome/l/lorenzog/PRISM_SW/lblpwgtools/CAFAna/PRISM/scripts/BuildPredInterps_batch.sh ${MODE}
)

# Get a token
htgettoken -a htvaultprod.fnal.gov -i dune

jobsub_submit "${ARGS[@]}"
