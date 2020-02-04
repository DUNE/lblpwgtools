#!/bin/bash

### Adapted from https://unix.stackexchange.com/questions/4965/keep-duplicates-out-of-path-on-source
function add_to_PATH () {
  for d; do

    d=$(cd -- "$d" && { pwd -P || pwd; }) 2>/dev/null  # canonicalize symbolic links
    if [ -z "$d" ]; then continue; fi  # skip nonexistent directory

    if [ "$d" == "/usr/bin" ] || [ "$d" == "/usr/bin64" ] || [ "$d" == "/usr/local/bin" ] || [ "$d" == "/usr/local/bin64" ]; then
      case ":$PATH:" in
        *":$d:"*) :;;
        *) export PATH=$PATH:$d;;
      esac
    else
      case ":$PATH:" in
        *":$d:"*) :;;
        *) export PATH=$d:$PATH;;
      esac
    fi
  done
}

function add_to_LD_LIBRARY_PATH () {
  for d; do

    d=$(cd -- "$d" && { pwd -P || pwd; }) 2>/dev/null  # canonicalize symbolic links
    if [ -z "$d" ]; then continue; fi  # skip nonexistent directory

    if [ "$d" == "/usr/lib" ] || [ "$d" == "/usr/lib64" ] || [ "$d" == "/usr/local/lib" ] || [ "$d" == "/usr/local/lib64" ]; then
      case ":$LD_LIBRARY_PATH:" in
        *":$d:"*) :;;
        *) export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$d;;
      esac
    else
      case ":$LD_LIBRARY_PATH:" in
        *":$d:"*) :;;
        *) export LD_LIBRARY_PATH=$d:$LD_LIBRARY_PATH;;
      esac
    fi
  done
}

#If we aren't on the GRID, we won't know this.
if [ -z "${CAFANA}" ]; then
  export CAFANA=/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Linux
fi

add_to_PATH "${CAFANA}/bin"
add_to_LD_LIBRARY_PATH "${CAFANA}/lib"

if [ "1" == "1" ]; then
  source ${CAFANA}/ups_env_setup.sh
fi

export BOOST_INC=/cvmfs/larsoft.opensciencegrid.org/products/boost/v1_66_0a/Linux64bit+3.10-2.17-e15-prof/include
export BOOST_LIB=/cvmfs/larsoft.opensciencegrid.org/products/boost/v1_66_0a/Linux64bit+3.10-2.17-e15-prof/lib

#Recent versions of ROOT may enable omp for minuit2 by default
if [ -z ${OMP_NUM_THREADS} ]; then
  export OMP_NUM_THREADS=1
fi

#Env vars used in the code
export CAFANA_ANALYSIS_VERSION="4"
# export CAFANA_TOTALDURATION_MIN="240"
# export CAFANA_SAFEUNITDURATION_MIN="60"
# export CAFANA_CHKDURATION_MIN="30"
# export CAFANA_CHK_SEMAPHORE="hadding.smph"
export CAFANA_USE_UNCORRNDCOVMAT="0"
export CAFANA_USE_NDCOVMAT="1"
export CAFANA_IGNORE_CV_WEIGHT="0"
export CAFANA_IGNORE_SELECTION="0"
export CAFANA_DISABLE_DERIVATIVES="1"
export CAFANA_DONT_CLAMP_SYSTS="0"
export CAFANA_FIT_TURBOSE="0"
export CAFANA_FIT_FORCE_HESSE="1"
export CAFANA_PRED_MINMCSTATS="50"
