#!/bin/bash

#Adapted from
# https://superuser.com/questions/205127/how-to-retrieve-the-absolute-path-of-an-arbitrary-file-from-the-os-x/218684#218684
function abspath() { 
  ABS_PATH_OPWD=$(pwd)
  if [ ! -e "${1}" ]; then
    :
  elif [ -d "$1" ]; then 
    cd "$1"; pwd; 
  else 
    cd $(dirname \"$1\"); 
    cur_dir=$(pwd); 
    if [ "$cur_dir" = "/" ]; then 
      echo "$cur_dir$(basename \"$1\")"; 
    else echo "$cur_dir/$(basename \"$1\")"; 
    fi; 
  fi; 
  cd ${ABS_PATH_OPWD}
}

#script to build...
FORCE_REMOVE="0"
USE_UPS="0"
CORES=1
CMAKE_BUILD_TYPE=RelWithDebInfo
BUILD_DIR="build"
INSTALL_DIR=""
USE_OMP="0"

while [[ ${#} -gt 0 ]]; do

  key="$1"
  case $key in

      -b|--build-dir)
      if [[ ${#} -lt 2 ]]; then
        echo "[ERROR]: ${1} expected a value."
        exit 1
      fi
      BUILD_DIR="$2"
      echo "[OPT]: Will build in directory $BUILD_DIR."
      shift
      ;;

      -f|--force-remove)

      FORCE_REMOVE="1"
      echo "[OPT]: Will remove extant build directory."
      ;;

      -u|--use-UPS)

      USE_UPS="1"
      echo "[OPT]: Will source dependencies from ups."
      ;;

      -r|--release)

      CMAKE_BUILD_TYPE="Release"
      echo "[OPT]: Will compile Release build type."
      ;;

      --db)

      CMAKE_BUILD_TYPE="Debug"
      echo "[OPT]: Will compile the Debug build type."
      ;;

      -O|--omp)

      USE_OMP="1"
      echo "[OPT]: Will compile OMP components."
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

      -I|--install-to)

      if [[ ${#} -lt 2 ]]; then
        echo "[ERROR]: ${1} expected a value."
        exit 1
      fi

      INSTALL_DIR="$2"
      echo "[OPT]: Will install to \"${INSTALL_DIR}\"."
      shift # past argument
      ;;

      -?|--help)
      echo "[RUNLIKE] ${SCRIPTNAME}"
      echo -e "\t-b|--build-dir         : Build directory"
      echo -e "\t-f|--force-remove      : Remove previous build directory if it exists."
      echo -e "\t-r|--release           : Compile with CMAKE_BUILD_TYPE=Release"
      echo -e "\t--db                   : Compile with CMAKE_BUILD_TYPE=Debug"
      echo -e "\t--use-gperftools       : Compile libunwind and gperftools"
      echo -e "\t-u|--use-UPS           : Try and use ups to set up required packages, "
      echo -e "\t                         rather than assuming they exist on the local system."
      echo -e "\t-j|--n-cores           : Number of cores to pass to make install."
      echo -e "\t-O|--omp               : Enable OMP features of CAFAna."
      echo -e "\t-I|--install-to        : Directory to install to."
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

if [ -e "$BUILD_DIR" ]; then
  if [ "${FORCE_REMOVE}" = "1" ]; then
    rm -rf "$BUILD_DIR"
  else
    echo "[ERROR]: Extant build directory in "$BUILD_DIR", will not overwrite, remove it or rebuild within it."
    exit 1
  fi
fi

SCRIPT_EXE_DIR=$(pwd)

mkdir "$BUILD_DIR"
cd "$BUILD_DIR"
BUILD_DIR=$(pwd)

if [ "${USE_UPS}" = "1" ]; then
  source ../cmake/ups_env_setup.sh
  source ../support_software.sh $(abspath ../support) USING_UPS
  cd "$BUILD_DIR"
  cmake ../ -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} \
            -DBUILD_ENV_SCRIPTS="$(abspath ../cmake/ups_env_setup.sh);$(abspath ../support/support_software_env.sh)"
else
  source ../support_software.sh $(abspath ../support) BUILD_UPS_REPLACEMENT_SOFTWARE
  cd "$BUILD_DIR"
  cmake ../ -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} \
            -DBUILD_ENV_SCRIPTS=$(abspath ../support/support_software_env.sh)
fi

make install -j ${CORES}
