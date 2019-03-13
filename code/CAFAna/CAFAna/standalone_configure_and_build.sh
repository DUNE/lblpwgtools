#!/bin/bash

FORCE_REMOVE="0"
USE_UPS="0"
CORES=1
USE_GPERF=0
CMAKE_BUILD_TYPE=DEBUG

while [[ ${#} -gt 0 ]]; do

  key="$1"
  case $key in

      -f|--force-remove)

      FORCE_REMOVE="1"
      echo "[OPT]: Will remove extant build directory."
      ;;

      -u|--use-UPS)

      USE_UPS="1"
      echo "[OPT]: Will source dependencies from ups."
      ;;

      --use-gperftools)

      USE_GPERF="1"
      echo "[OPT]: Will compile in gperftools support."
      ;;

      -r|--release)

      CMAKE_BUILD_TYPE="RELEASE"
      echo "[OPT]: Will compile release build type."
      ;;


      -j|--n-cores)

      if [[ ${#} -lt 2 ]]; then
        echo "[ERROR]: ${1} expected a value."
        exit 1
      fi

      CORES="$2"
      echo "[OPT]: Will build with \"${CORES}\" cores."
      shift # past argument
      ;;

      -?|--help)
      echo "[RUNLIKE] ${SCRIPTNAME}"
      echo -e "\t-f|--force-remove      : Remove previous build directory if it exists."
      echo -e "\t-r|--release           : Compile with CMAKE_BUILD_TYPE=RELEASE"
      echo -e "\t--use-gperftools       : Compile libunwind and gperftools"

      echo -e "\t-u|--use-UPS           : Try and use ups to set up required packages, rather than assuming they exist on the local system."
      echo -e "\t-j|--n-cores           : Number of cores to pass to make install."
      echo -e "\t-?|--help              : Print this message."
      exit 0
      ;;

      *)
              # unknown option
      echo "Unknown option $1"
      exit 1
      ;;
  esac
  shift # past argument or value
done

if [ -e build ]; then
  if [ "${FORCE_REMOVE}" == "1" ]; then
    rm -rf build
  else
    echo "[ERROR]: Extant build directory, will not overwrite, remove it or rebuild within it."
    exit 1
  fi
fi

mkdir build
cd build

mkdir Ext
cd Ext

svn checkout http://cdcvs.fnal.gov/subversion/novaart.pkgs.svn/trunk/OscLib
svn checkout http://cdcvs.fnal.gov/subversion/novaart.pkgs.svn/trunk/Utilities

cd ../

if [ "${USE_UPS}" == "1" ]; then
  source ../cmake/ups_env_setup.sh
else
  if ! hash clhep-config; then
    echo "[ERROR]: Not using UPS, but cannot find clhep-config in the path. Is libclhep-dev installed?"
    exit 1
  fi

  if ! hash root-config; then
    echo "[ERROR]: Not using UPS, but cannot find root-config in the path. Is root installed?"
    exit 1
  fi

  if ! hash gsl-config; then
    echo "[ERROR]: Not using UPS, but cannot find gsl-config in the path. Is gsl installed?"
    exit 1
  fi

  if [ -z "${BOOST_INC}" ]; then
    if [ -e /usr/include/boost ]; then
      export BOOST_INC=/usr/include
    else
      echo "[ERROR]: Not using UPS, but couldn't find system boost (/usr/include/boost) and BOOST_INC wasn't defined in the environment."
      exit 1
    fi
  fi

fi

cmake ../ -DSRC_ROOT_PARENT=$(readlink -f ../../) -DUSED_UPS=${USE_UPS} -DUSE_GPERFTOOLS=${USE_GPERF} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
make install -j ${CORES}
