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

add_to_PATH "@CMAKE_INSTALL_PREFIX@/bin"
add_to_LD_LIBRARY_PATH "@CMAKE_INSTALL_PREFIX@/lib"

if [ ! "${ROOTSYS}" ]; then
  echo "[INFO]: Sourcing ROOT from: @CMAKE_ROOTSYS@"
  source "@CMAKE_ROOTSYS@/bin/thisroot.sh"
fi

if [ "@USE_NEUT@" != "FALSE" ]; then
  echo "[INFO]: Adding NEUT library paths to the environment."
  export NEUT_ROOT=@NEUT_ROOT@
  export CERN=@CERN@
  export CERN_LEVEL=@CERN_LEVEL@
  add_to_LD_LIBRARY_PATH "${NEUT_LIB_DIR}" "${NEUT_ROOT}/src/reweight"
fi

if [ "@USE_NUWRO@" != "FALSE" ]; then
  echo "[INFO]: Adding NuWro library paths to the environment."
  export NUWRO="@NUWRO@"

  add_to_PATH "@NUWRO@/bin"
  add_to_LD_LIBRARY_PATH "@NUWRO@/build/@CMAKE_SYSTEM_NAME@/lib"

  if [ "@NUWRO_INC@" ]; then
    export NUWRO_INC=@NUWRO_INC@
  fi
fi

if [ "@NEED_PYTHIA6@" != "FALSE" ]; then
  echo "[INFO]: Adding PYTHIA6 library paths to the environment."
  export PYTHIA6="@PYTHIA6@"

  add_to_LD_LIBRARY_PATH "@PYTHIA6@"
fi

if [ "@USE_GENIE@" != "FALSE" ]; then
  echo "[INFO]: Adding GENIE paths to the environment."

  export GENIE="@GENIE@"

  export LHAPDF_LIB="@LHAPDF_LIB@"
  export LHAPDF_INC="@LHAPDF_INC@"

  export LIBXML2_LIB="@LIBXML2_LIB@"
  export LIBXML2_INC="@LIBXML2_INC@"

  export LOG4CPP_LIB="@LOG4CPP_LIB@"
  export LOG4CPP_INC="@LOG4CPP_INC@"

  if [ "@GSL_INC@" ]; then
    export GSL_INC="@GSL_INC@"
  fi

  if [ "@GSL_LIB@" ]; then
    export GSL_LIB="@GSL_LIB@"
  fi

  if [ "@LHAPATH@" ]; then
    export LHAPATH="@LHAPATH@"
  fi

  add_to_PATH "@GENIE@/bin"

  add_to_LD_LIBRARY_PATH "@GENIE@/lib" "@LHAPDF_LIB@" "@LIBXML2_LIB@" "@LOG4CPP_LIB@"

  if [ "@GENIE_REWEIGHT@" ]; then
    echo "[INFO]: Adding GENIE ReWeight library paths to the environment."
    export GENIE_REWEIGHT="@GENIE_REWEIGHT@"
    add_to_LD_LIBRARY_PATH "@GENIE_REWEIGHT@/lib"
  fi

  if [ "@GSL_LIB@" ]; then
    add_to_LD_LIBRARY_PATH "@GSL_LIB@"
  fi

fi

source @CMAKE_INSTALL_PREFIX@/fhiclcpp.setup.sh

add_to_FHICL_FILE_PATH @CMAKE_INSTALL_PREFIX@/fcl

export NUISANCE="@CMAKE_SOURCE_DIR@"
