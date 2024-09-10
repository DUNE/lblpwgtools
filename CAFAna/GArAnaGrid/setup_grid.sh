# we cannot rely on "whoami" in a grid job. We have no idea what the local username will be. 
# Use the GRID_USER environment variable instead (set automatically by jobsub).
USERNAME=${GRID_USER}

export WORKDIR=${_CONDOR_JOB_IWD} # if we use the RCDS then our tarball will be placed in $INPUT_TAR_DIR_LOCAL.
if [ ! -d "$WORKDIR" ]; then
  export WORKDIR=`echo .`
fi

source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh
setup ifdhc

export MY_UPS_DIR=${INPUT_TAR_DIR_LOCAL}/ups
export PRODUCTS=$MY_UPS_DIR:$PRODUCTS

export CAFANA=${INPUT_TAR_DIR_LOCAL}/CAFAna
export CAFANA_ENV_SETUP=${INPUT_TAR_DIR_LOCAL}/CAFAna/CAFAnaEnv.sh
source $CAFANA_ENV_SETUP