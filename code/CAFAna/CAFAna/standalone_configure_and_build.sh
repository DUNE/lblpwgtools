#!/bin/bash

if [ "${1}" == "-f" ]; then
  rm -rf build
fi

if [ -e build ]; then
  echo "[ERROR]: Extant build directory, will not overwrite, remove it or rebuild within it."
  exit 1
fi

mkdir build
cd build

mkdir Ext
cd Ext

svn checkout http://cdcvs.fnal.gov/subversion/novaart.pkgs.svn/trunk/OscLib
svn checkout http://cdcvs.fnal.gov/subversion/novaart.pkgs.svn/trunk/Utilities

cd ../

source ../cmake/ups_env_setup.sh

cmake ../ -DCMAKE_BUILD_TYPE=DEBUG -DSRC_ROOT_PARENT=$(readlink -f ../../)
make install
