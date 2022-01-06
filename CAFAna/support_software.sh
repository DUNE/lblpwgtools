#!/bin/bash

set -e

SUPPORT_SOFTWARE_BUILD_DIR=$(readlink -f ${1})
BUILD_UPS_REPLACEMENT_SOFTWARE=${2}

mkdir -p ${SUPPORT_SOFTWARE_BUILD_DIR}
cd ${SUPPORT_SOFTWARE_BUILD_DIR}

(
cat <<'EOF'
if [ ${BASH_VERSINFO[0]} -le 2 ]; then
  echo "[ERROR]: You must source this setup script (not run it in a sub-shell). Use like $ source setup.sh"
  exit 1
fi

if ! type add_to_PATH &> /dev/null; then

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

fi

if ! type add_to_LD_LIBRARY_PATH &> /dev/null; then

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

fi
EOF
) > support_software_env.sh


if ! hash root-config; then
  echo "[ERROR]: Not using UPS, but cannot find root-config in the path. Is root installed?"
  exit 1
fi

if [ -z "${BOOST_INC}" ]; then
  if [ -e /usr/include/boost ]; then
    BOOST_INC=/usr/include
  else
    echo "[ERROR]: Not using UPS, but couldn't find system boost (/usr/include/boost) and BOOST_INC wasn't defined in the environment."
    exit 1
  fi
  echo -e "export BOOST_INC=\"${BOOST_INC}\"" >> support_software_env.sh
fi

if [ -z "${BOOST_LIB}" ]; then
  if [ -e /usr/lib/x86_64-linux-gnu/libboost_filesystem.so* ]; then
    BOOST_LIB=/usr/lib/x86_64-linux-gnu/
  elif [ -e /usr/lib/libboost_filesystem.so* ]; then
    BOOST_LIB=/usr/lib/
  else
    echo "[ERROR]: Not using UPS, but couldn't find system boost libboost_filesystem.so and BOOST_LIB wasn't defined in the environment."
    exit 1
  fi
  if [ -e /usr/lib/x86_64-linux-gnu/libboost_system.so* ]; then
    BOOST_LIB=/usr/lib/x86_64-linux-gnu/
  elif [ -e /usr/lib/libboost_system.so* ]; then
    BOOST_LIB=/usr/lib/
  else
    echo "[ERROR]: Not using UPS, but couldn't find system boost libboost_system.so and BOOST_LIB wasn't defined in the environment."
    exit 1
  fi
  echo -e "export BOOST_LIB=\"${BOOST_LIB}\"" >> support_software_env.sh
fi

if [ -z "${BOOST_DIR}" ]; then
  BOOST_DIR=$(readlink -f ${BOOST_INC}/../)
  echo -e "export BOOST_DIR=\"${BOOST_DIR}\"" >> support_software_env.sh
fi

if [ -z "${BOOST_VERSION}" ]; then
(
cat <<EOF
#include "boost/version.hpp"
#include <iostream>
int main(){ 
  std::cout << BOOST_VERSION / 100000 << "." 
            << (BOOST_VERSION / 100) % 1000 << "." 
            << BOOST_VERSION % 100 
            << std::endl; }
EOF
) > boost_version.cxx
  g++ boost_version.cxx -I ${BOOST_INC}

  BOOST_VERSION=$(./a.out)
  rm boost_version.cxx a.out
  echo -e "export BOOST_VERSION=\"${BOOST_VERSION}\"" >> support_software_env.sh
fi

#Build linedoc
if [ ! -e linedoc ]; then
  git clone git@github.com:luketpickering/linedoc
  cd ${SUPPORT_SOFTWARE_BUILD_DIR}
fi

if [ ! -e linedoc/build/Linux/setup.sh ]; then
  mkdir -p linedoc/build
  cd linedoc/build
  cmake ../
  make install
  cd ${SUPPORT_SOFTWARE_BUILD_DIR}
fi

source linedoc/build/Linux/setup.sh

#Build fhiclcpp
if [ ! -e fhiclcpp-simple ]; then
  git clone git@github.com:luketpickering/fhiclcpp-simple
  cd ${SUPPORT_SOFTWARE_BUILD_DIR}
fi

if [ ! -e fhiclcpp-simple/build/Linux/setup.sh ]; then
  mkdir -p fhiclcpp-simple/build
  cd fhiclcpp-simple/build
  cmake ../
  make install
  cd ${SUPPORT_SOFTWARE_BUILD_DIR}
fi

source fhiclcpp-simple/build/Linux/setup.sh

#Build TH2Jagged
if [ ! -e TH2Jagged ]; then
  git clone git@github.com:luketpickering/TH2Jagged
  cd ${SUPPORT_SOFTWARE_BUILD_DIR}
fi

if [ ! -e TH2Jagged/build/Linux/setup.sh ]; then
  mkdir -p TH2Jagged/build
  cd TH2Jagged/build
  cmake ../
  make install
  cd ${SUPPORT_SOFTWARE_BUILD_DIR}
fi

source TH2Jagged/build/Linux/setup.sh

echo -e "export linedoc_ROOT=\"${linedoc_ROOT}\"" >> support_software_env.sh
echo -e "export fhiclcpp_ROOT=\"${fhiclcpp_ROOT}\"" >> support_software_env.sh
echo -e "export TH2Jagged_ROOT=\"${TH2Jagged_ROOT}\"" >> support_software_env.sh

echo -e "add_to_LD_LIBRARY_PATH \${TH2Jagged_ROOT}/lib" >> support_software_env.sh

#Build CAFAnaCore, OscLib, Stan and friends
if [ ${BUILD_UPS_REPLACEMENT_SOFTWARE} == "BUILD_UPS_REPLACEMENT_SOFTWARE" ]; then

  if [ ! -e stan ]; then
    git clone https://github.com/stan-dev/stan.git
    cd stan
    git checkout v2.28.2
    cd ${SUPPORT_SOFTWARE_BUILD_DIR}
  fi

  if [ ! -e stan/lib/stan_math/make/standalone ]; then
    cd stan
    git submodule update --init --recursive
    cd ${SUPPORT_SOFTWARE_BUILD_DIR}
  fi

  #Hard coded versions for the moment
  export SUNDIALS_VERSION=5.7.0
  export SUNDIALS_INC=$(readlink -f stan/lib/stan_math/lib/sundials_${SUNDIALS_VERSION}/include)
  export SUNDIALS_LIB=$(readlink -f stan/lib/stan_math/lib/sundials_${SUNDIALS_VERSION}/lib)
  export SUNDIALS_DIR=$(readlink -f stan/lib/stan_math/lib/sundials_${SUNDIALS_VERSION})

  if [ ! -e ${SUNDIALS_LIB}/libsundials_cvodes.a ]; then
    cd stan
    BOOST=${BOOST_INC} make -f lib/stan_math/make/standalone math-libs
    cd ${SUPPORT_SOFTWARE_BUILD_DIR}
  fi

  export EIGEN_VERSION=3.3.9
  export EIGEN_INC=$(readlink -f stan/lib/stan_math/lib/eigen_${EIGEN_VERSION})
  export EIGEN_DIR=$(readlink -f stan/lib/stan_math/lib/eigen_${EIGEN_VERSION})

  export STAN_INC=$(readlink -f stan/src)
  export STAN_VERSION=v2.28.2

  export STAN_MATH_INC=$(readlink -f stan/lib/stan_math)
  export STAN_MATH_DIR=$(readlink -f stan/lib/stan_math)
  export STAN_MATH_VERSION=v4.2.1

  if [ -z ${TBB_INC} ]; then # Check if Stan built TBB
    if [ -e stan/lib/stan_math/lib/tbb_*/include/tbb/tbb_stddef.h ]; then # Check if Stan built it
      export TBB_INC=$(readlink -f stan/lib/stan_math/lib/tbb_*/include)
    else
      echo "[ERROR]: Cannot find TBB, yet Stan doesn't seem to contain it in $(readlink -f stan/lib/stan_math/lib)"
      exit 1
    fi
  fi

  if [ -z ${TBB_DIR} ]; then
    export TBB_DIR=$(readlink -f ${TBB_INC}/..)
  fi

  if [ -z ${TBB_LIB} ]; then # Check if Stan built TBB
    if [ -e stan/lib/stan_math/lib/tbb/libtbb.so ]; then # Check if Stan built it
      export TBB_LIB=$(readlink -f stan/lib/stan_math/lib/tbb/)
    else
      echo "[ERROR]: Cannot find TBB, yet Stan doesn't seem to contain it in $(readlink -f stan/lib/stan_math/lib)"
      exit 1
    fi
  fi

  if [ ! -e ${TBB_LIB}/libtbb.so ]; then
    echo "[ERROR]: Cannot find libtbb.so in \$TBB_LIB=${TBB_LIB}. This requires manual intervention."
    exit 1
  fi

  if [ -z ${TBB_VERSION} ]; then # Don't have TBB version set
(
cat <<EOF
#include "tbb/tbb_stddef.h"
#include <iostream>
int main(){ 
  std::cout << TBB_VERSION_MAJOR << "." 
            << TBB_VERSION_MINOR 
            << std::endl; }
EOF
) > tbb_version.cxx
    g++ tbb_version.cxx -I ${TBB_INC}

    TBB_VERSION=$(./a.out)
    rm tbb_version.cxx a.out
  fi

  echo -e "export SUNDIALS_VERSION=\"${SUNDIALS_VERSION}\"" >> support_software_env.sh
  echo -e "export SUNDIALS_INC=\"${SUNDIALS_INC}\"" >> support_software_env.sh
  echo -e "export SUNDIALS_LIB=\"${SUNDIALS_LIB}\"" >> support_software_env.sh
  echo -e "export SUNDIALS_DIR=\"${SUNDIALS_DIR}\"" >> support_software_env.sh

  echo -e "export EIGEN_VERSION=\"${EIGEN_VERSION}\"" >> support_software_env.sh
  echo -e "export EIGEN_INC=\"${EIGEN_INC}\"" >> support_software_env.sh
  echo -e "export EIGEN_DIR=\"${EIGEN_DIR}\"" >> support_software_env.sh

  #Helps find_package
  echo -e "export Eigen3_ROOT=\"${EIGEN_DIR}\"" >> support_software_env.sh

  echo -e "export STAN_INC=\"${STAN_INC}\"" >> support_software_env.sh
  echo -e "export STAN_VERSION=\"${STAN_VERSION}\"" >> support_software_env.sh

  echo -e "export STAN_MATH_INC=\"${STAN_MATH_INC}\"" >> support_software_env.sh
  echo -e "export STAN_MATH_DIR=\"${STAN_MATH_DIR}\"" >> support_software_env.sh
  echo -e "export STAN_MATH_VERSION=\"${STAN_MATH_VERSION}\"" >> support_software_env.sh

  echo -e "export TBB_VERSION=\"${TBB_VERSION}\"" >> support_software_env.sh
  echo -e "export TBB_INC=\"${TBB_INC}\"" >> support_software_env.sh
  echo -e "export TBB_LIB=\"${TBB_LIB}\"" >> support_software_env.sh
  echo -e "export TBB_DIR=\"${TBB_DIR}\"" >> support_software_env.sh

  if [ ! -e OscLib ]; then
    git clone https://github.com/cafana/OscLib.git
    # checkout version
  fi

  if [ ! -e OscLib/OscLib/lib/libOscLib.so ]; then
    cd OscLib
    make
    cd ${SUPPORT_SOFTWARE_BUILD_DIR}
  fi

  OSCLIB_INC=$(readlink -f OscLib/)
  OSCLIB_LIB=$(readlink -f OscLib/OscLib/lib)
  
  echo -e "export OSCLIB_INC=\"${OSCLIB_INC}\"" >> support_software_env.sh
  echo -e "export OSCLIB_LIB=\"${OSCLIB_LIB}\"" >> support_software_env.sh
  echo -e "add_to_LD_LIBRARY_PATH \${OSCLIB_LIB}" >> support_software_env.sh

  if [ ! -e CAFAnaCore ]; then
    git clone https://github.com/luketpickering/CAFAnaCore.git
    # checkout version
  fi

  if [ ! -e CAFAnaCore/build/lib/libCAFAnaCoreExt.so ]; then
    mkdir -p CAFAnaCore/build
    cd CAFAnaCore/build;
    cmake ../ -DCMAKE_INSTALL_PREFIX=$(pwd) -DTBB_VERSION=${TBB_VERSION}
    make install
    ln -s $(readlink -f inc/) inc/CAFAnaCore
    cd ${SUPPORT_SOFTWARE_BUILD_DIR}
  fi

  CAFANACORE_INC=$(readlink -f CAFAnaCore/build/inc)
  CAFANACORE_LIB=$(readlink -f CAFAnaCore/build/lib)

  echo -e "export CAFANACORE_INC=\"${CAFANACORE_INC}\"" >> support_software_env.sh
  echo -e "export CAFANACORE_LIB=\"${CAFANACORE_LIB}\"" >> support_software_env.sh
  echo -e "add_to_LD_LIBRARY_PATH \${CAFANACORE_LIB}" >> support_software_env.sh

fi