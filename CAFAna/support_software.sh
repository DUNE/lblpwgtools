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
    cd $(dirname "$1"); 
    cur_dir=$(pwd); 
    if [ "$cur_dir" = "/" ]; then 
      echo "$cur_dir$(basename "$1")"; 
    else echo "$cur_dir/$(basename "$1")"; 
    fi; 
  fi; 
  cd ${ABS_PATH_OPWD}
}

DSOEXT="so"
if [ $(uname) = "Darwin" ]; then
  DSOEXT="dylib"
fi

echo "OPTS: ${@}"

set -e

SUPPORT_SOFTWARE_BUILD_DIR=$(abspath ${1})
BUILD_UPS_REPLACEMENT_SOFTWARE=${2}

mkdir -p ${SUPPORT_SOFTWARE_BUILD_DIR}
cd ${SUPPORT_SOFTWARE_BUILD_DIR}

(
cat <<'EOF'
if [ ! -z ${BASH_VERSINFO} ] && (( BASH_VERSINFO[0] < 2 )); then
  echo "[ERROR]: You must source this setup script (not run it in a sub-shell). Use like $ source setup.sh"
  exit 1
fi

if ! type abspath &> /dev/null; then

#Adapted from
# https://superuser.com/questions/205127/how-to-retrieve-the-absolute-path-of-an-arbitrary-file-from-the-os-x/218684#218684
function abspath() { 
  ABS_PATH_OPWD=$(pwd)
  if [ ! -e "${1}" ]; then
    :
  elif [ -d "$1" ]; then 
    cd "$1"; pwd; 
  else 
    cd $(dirname "$1"); 
    cur_dir=$(pwd); 
    if [ "$cur_dir" = "/" ]; then 
      echo "$cur_dir$(basename "$1")"; 
    else echo "$cur_dir/$(basename "$1")"; 
    fi; 
  fi; 
  cd ${ABS_PATH_OPWD}
}

fi

if ! type add_to_PATH &> /dev/null; then

### Adapted from https://unix.stackexchange.com/questions/4965/keep-duplicates-out-of-path-on-source
function add_to_PATH () {
  for d; do

    d=$(cd -- "$d" && { pwd -P || pwd; }) 2>/dev/null  # canonicalize symbolic links
    if [ -z "$d" ]; then continue; fi  # skip nonexistent directory

    if [ "$d" = "/usr/bin" ] || [ "$d" = "/usr/bin64" ] || [ "$d" = "/usr/local/bin" ] || [ "$d" = "/usr/local/bin64" ]; then
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

    if [ "$d" = "/usr/lib" ] || [ "$d" = "/usr/lib64" ] || [ "$d" = "/usr/local/lib" ] || [ "$d" = "/usr/local/lib64" ]; then
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


#Check for ROOT. Recent ROOTs don't use thisroot.sh, so we might have to look for things ourselves
if [ -z ${ROOTSYS} ] || [ ! -e ${ROOTSYS} ]; then
  if hash root-config; then
    export ROOTSYS=$(root-config --prefix)
    export ROOT_INC=$(root-config --incdir)
    export ROOT_LIB=$(root-config --libdir)
    export ROOT_VERSION=$(root-config --version)
    export ROOT_DIR=${ROOTSYS}
    export ROOT_ROOT=${ROOTSYS}
  else
    echo "[ERROR]: Not using UPS, but couldn't find system ROOT (root-config) and ROOTSYS wasn't defined in the environment."
    exit 1
  fi
else
  if [ -z ${ROOT_INC} ]; then
    if [ -e ${ROOTSYS}/include/Rtypes.h ]; then
      export ROOT_INC=${ROOTSYS}/include
    elif [ -e ${ROOTSYS}/include/root/Rtypes.h ]; then
      export ROOT_INC=${ROOTSYS}/include/root
    else
      echo "[ERROR]: ROOTSYS is defined: \"${ROOTSYS}\", but cannot find expected include dir. Checked for \${ROOTSYS}/include/Rtypes.h and \${ROOTSYS}/root/include/Rtypes.h"
      exit 1
    fi
  fi
  if [ -z ${ROOT_LIB} ]; then
    if [ -e ${ROOTSYS}/lib/libCore.so ]; then
      export ROOT_INC=${ROOTSYS}/lib
    elif [ -e ${ROOTSYS}/lib/root/libCore.so ]; then
      export ROOT_INC=${ROOTSYS}/lib/root
    else
      echo "[ERROR]: ROOTSYS is defined: \"${ROOTSYS}\", but cannot find expected lib dir. Checked for \${ROOTSYS}/lib/libCore.so and \${ROOTSYS}/root/lib/libCore.so"
      exit 1
    fi
  fi
  if [ -z ${ROOT_DIR} ]; then
    export ROOT_DIR=${ROOTSYS}
  fi
  if [ -z ${ROOT_ROOT} ]; then
    export ROOT_ROOT=${ROOTSYS}
  fi
fi #end if have ROOTSYS

if [ -z "${BOOST_INC}" ]; then
  if [ -e /usr/include/boost ]; then
    export BOOST_INC=/usr/include
  elif hash brew; then #check homebrew if it exists
    if brew --prefix boost &> /dev/null; then
      export BOOST_INC=$(brew --prefix boost)/include
    fi
  else
    echo "[ERROR]: Not using UPS, but couldn't find system boost (/usr/include/boost) and BOOST_INC wasn't defined in the environment."
    exit 1
  fi
  echo -e "export BOOST_INC=\"${BOOST_INC}\"" >> support_software_env.sh
fi

if [ -z "${BOOST_LIB}" ]; then
  if [ $(uname) = "Linux" ] && [ -e /usr/lib/$(arch)-linux-gnu/libboost_filesystem.${DSOEXT} ]; then
    export BOOST_LIB=/usr/lib/$(arch)-linux-gnu/
  elif [ -e /usr/lib/libboost_filesystem.${DSOEXT} ]; then
    export BOOST_LIB=/usr/lib/
  elif hash brew; then #check homebrew if it exists
    if brew --prefix boost &> /dev/null && [ -e $(brew --prefix boost)/lib/libboost_filesystem.${DSOEXT} ]; then
      export BOOST_LIB=$(brew --prefix boost)/lib
    fi
  else
    echo "[ERROR]: Not using UPS, but couldn't find system boost libboost_filesystem.${DSOEXT} and BOOST_LIB wasn't defined in the environment."
    exit 1
  fi
  if [ $(uname) = "Linux" ] && [ -e /usr/lib/$(arch)-linux-gnu/libboost_system.${DSOEXT} ]; then
    export BOOST_LIB=/usr/lib/$(arch)-linux-gnu/
  elif [ -e /usr/lib/libboost_system.${DSOEXT}* ]; then
    export BOOST_LIB=/usr/lib/
  elif hash brew; then #check homebrew if it exists
    if brew --prefix boost &> /dev/null && [ -e $(brew --prefix boost)/lib/libboost_system.${DSOEXT} ]; then
      export BOOST_LIB=$(brew --prefix boost)/lib
    fi
  else
    echo "[ERROR]: Not using UPS, but couldn't find system boost libboost_system.${DSOEXT} and BOOST_LIB wasn't defined in the environment."
    exit 1
  fi
  echo -e "export BOOST_LIB=\"${BOOST_LIB}\"" >> support_software_env.sh
fi

if [ -z "${BOOST_DIR}" ]; then
  export BOOST_DIR=$(abspath ${BOOST_INC}/../)
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

  export BOOST_VERSION=$(./a.out)
  rm boost_version.cxx a.out
  echo -e "export BOOST_VERSION=\"${BOOST_VERSION}\"" >> support_software_env.sh
fi

#Build linedoc
if [ ! -e linedoc ]; then
  git clone https://github.com/luketpickering/linedoc
  cd ${SUPPORT_SOFTWARE_BUILD_DIR}
fi

if [ ! -e linedoc/build/$(uname)/setup.sh ]; then
  mkdir -p linedoc/build
  cd linedoc/build
  cmake ../
  make install
  cd ${SUPPORT_SOFTWARE_BUILD_DIR}
fi

source linedoc/build/$(uname)/setup.sh

#Build fhiclcpp
if [ ! -e fhiclcpp-simple ]; then
  git clone https://github.com/luketpickering/fhiclcpp-simple
  cd ${SUPPORT_SOFTWARE_BUILD_DIR}
fi

if [ ! -e fhiclcpp-simple/build/$(uname)/setup.sh ]; then
  mkdir -p fhiclcpp-simple/build
  cd fhiclcpp-simple/build
  cmake ../
  make install
  cd ${SUPPORT_SOFTWARE_BUILD_DIR}
fi

source fhiclcpp-simple/build/$(uname)/setup.sh

#Build TH2Jagged
if [ ! -e TH2Jagged ]; then
  git clone https://github.com/luketpickering/TH2Jagged
  cd ${SUPPORT_SOFTWARE_BUILD_DIR}
fi

if [ ! -e TH2Jagged/build/$(uname)/setup.sh ]; then
  mkdir -p TH2Jagged/build
  cd TH2Jagged/build
  cmake ../ 
  make install
  cd ${SUPPORT_SOFTWARE_BUILD_DIR}
fi

source TH2Jagged/build/$(uname)/setup.sh

echo -e "export linedoc_ROOT=\"${linedoc_ROOT}\"" >> support_software_env.sh
echo -e "export fhiclcpp_ROOT=\"${fhiclcpp_ROOT}\"" >> support_software_env.sh
echo -e "export TH2Jagged_ROOT=\"${TH2Jagged_ROOT}\"" >> support_software_env.sh

#Build CAFAnaCore, OscLib, Stan and friends
if [ "${BUILD_UPS_REPLACEMENT_SOFTWARE}" = "BUILD_UPS_REPLACEMENT_SOFTWARE" ]; then

  if [ -z ${STAN_INC} ] || [ ! -e ${STAN_INC} ] \
    || [ -z ${SUNDIALS_INC} ] || [ ! -e ${SUNDIALS_INC} ]\
    || [ -z ${EIGEN_INC} ] || [ ! -e ${EIGEN_INC} ]; then

    if [ ! -e stan ]; then
      git clone https://github.com/stan-dev/stan.git
      cd stan
      git checkout v2.28.2
      cd ${SUPPORT_SOFTWARE_BUILD_DIR}
    fi

    export STAN_INC=$(abspath stan/src)
    export STAN_VERSION=v2.28.2

    if [ ! -e stan/lib/stan_math/make/standalone ]; then
      cd stan
      git submodule update --init --recursive
      cd ${SUPPORT_SOFTWARE_BUILD_DIR}
    fi

    export STAN_MATH_INC=$(abspath stan/lib/stan_math)
    export STAN_MATH_DIR=$(abspath stan/lib/stan_math)
    export STAN_MATH_VERSION=v4.2.1

    #Hard coded versions for the moment
    export SUNDIALS_VERSION=5.7.0
    export SUNDIALS_DIR=$(abspath stan/lib/stan_math/lib/sundials_${SUNDIALS_VERSION})
    export SUNDIALS_INC=${SUNDIALS_DIR}/include
    export SUNDIALS_LIB=${SUNDIALS_DIR}/lib

    if [ ! -e ${SUNDIALS_LIB}/libsundials_cvodes.a ]; then
      cd stan
      BOOST=${BOOST_INC} make -f lib/stan_math/make/standalone math-libs
      cd ${SUPPORT_SOFTWARE_BUILD_DIR}
    fi

    export EIGEN_VERSION=3.3.9
    export EIGEN_INC=$(abspath stan/lib/stan_math/lib/eigen_${EIGEN_VERSION})
    export EIGEN_DIR=$(abspath stan/lib/stan_math/lib/eigen_${EIGEN_VERSION})

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

    if [ -z ${TBB_INC} ]; then # Check if Stan built TBB
      if [ -e stan/lib/stan_math/lib/tbb_*/include/tbb/tbb_stddef.h ]; then # Check if Stan built it
        export TBB_INC=$(abspath stan/lib/stan_math/lib/tbb_*/include)
      else
        echo "[ERROR]: Cannot find TBB, yet Stan doesn't seem to contain it in $(abspath stan/lib/stan_math/lib)"
        exit 1
      fi
    fi

    if [ -z ${TBB_DIR} ]; then
      export TBB_DIR=$(abspath ${TBB_INC}/..)
    fi

    if [ -z ${TBB_LIB} ]; then # Check if Stan built TBB
      if [ -e stan/lib/stan_math/lib/tbb/libtbb.${DSOEXT} ]; then # Check if Stan built it
        export TBB_LIB=$(abspath stan/lib/stan_math/lib/tbb/)
      else
        echo "[ERROR]: Cannot find TBB, yet Stan doesn't seem to contain it in $(abspath stan/lib/stan_math/lib)"
        exit 1
      fi
    fi

    if [ ! -e ${TBB_LIB}/libtbb.${DSOEXT} ]; then
      echo "[ERROR]: Cannot find libtbb.${DSOEXT} in \$TBB_LIB=${TBB_LIB}. This requires manual intervention."
      exit 1
    fi

    echo -e "export TBB_INC=\"${TBB_INC}\"" >> support_software_env.sh
    echo -e "export TBB_LIB=\"${TBB_LIB}\"" >> support_software_env.sh
    echo -e "export TBB_DIR=\"${TBB_DIR}\"" >> support_software_env.sh

  fi

  if [ -z ${GSL_INC} ] || [ ! -e ${GSL_INC} ]  \
    || [ -z ${GSL_LIB} ] || [ ! -e ${GSL_LIB} ]; then
    if hash gsl-config; then
      export GSL_INC=$(gsl-config --prefix)/include
      export GSL_LIB=$(gsl-config --prefix)/lib
    else
      echo "[ERROR]: Not using UPS, but couldn't find system gsl (gsl-config) and GSL_INC wasn't defined in the environment."
      exit 1
    fi
  fi

  if [ -z ${TBB_VERSION} ]; then # Don't have TBB version set
(
cat <<EOF
#include "tbb/tbb.h"
#include <iostream>
int main(){ 
  std::cout << TBB_VERSION_MAJOR << "." 
            << TBB_VERSION_MINOR 
            << std::endl; }
EOF
) > tbb_version.cxx
    g++ -std=c++11 tbb_version.cxx -I ${TBB_INC} -Wl,-rpath,${TBB_LIB} -L ${TBB_LIB} -ltbb 

    export TBB_VERSION=$(./a.out)
    rm tbb_version.cxx a.out
    echo -e "export TBB_VERSION=\"${TBB_VERSION}\"" >> support_software_env.sh
  fi

  if [ -z ${TBB_UPS_VERSION} ]; then # Don't have TBB version set
    export TBB_UPS_VERSION=${TBB_VERSION}
  fi

  if [ -z ${OSCLIB_INC} ] || [ ! -e ${OSCLIB_INC} ]; then
    if [ ! -e OscLib ]; then
      git clone https://github.com/luketpickering/OscLib.git
      # checkout version
    fi

    if [ ! -e OscLib/OscLib/lib/libOscLib.so ]; then
      cd OscLib
      make
      cd ${SUPPORT_SOFTWARE_BUILD_DIR}
    fi

    export OSCLIB_INC=$(abspath OscLib/)
    export OSCLIB_LIB=$(abspath OscLib/OscLib/lib)
    
    echo -e "export OSCLIB_INC=\"${OSCLIB_INC}\"" >> support_software_env.sh
    echo -e "export OSCLIB_LIB=\"${OSCLIB_LIB}\"" >> support_software_env.sh
    [ "$(uname -s)" = "Linux" ] && echo -e "add_to_LD_LIBRARY_PATH \${OSCLIB_LIB}" >> support_software_env.sh

  fi

  if [ -z ${CAFANACORE_INC} ] || [ ! -e ${CAFANACORE_INC} ]; then

    if [ ! -e CAFAnaCore ]; then
      git clone https://github.com/luketpickering/CAFAnaCore.git
      # checkout version
    fi

    if [ ! -e CAFAnaCore/build/lib/libCAFAnaCoreExt.${DSOEXT} ]; then
      mkdir -p CAFAnaCore/build
      cd CAFAnaCore/build;
      cmake ../ -DCMAKE_INSTALL_PREFIX=$(pwd) -DTBB_VERSION=${TBB_VERSION} -DNO_IFDHC=ON
      make install
      ln -s $(abspath inc/) inc/CAFAnaCore
      cd ${SUPPORT_SOFTWARE_BUILD_DIR}
    fi

    export CAFANACORE_INC=$(abspath CAFAnaCore/build/inc)
    export CAFANACORE_LIB=$(abspath CAFAnaCore/build/lib)

    echo -e "export CAFANACORE_INC=\"${CAFANACORE_INC}\"" >> support_software_env.sh
    echo -e "export CAFANACORE_LIB=\"${CAFANACORE_LIB}\"" >> support_software_env.sh
    [ "$(uname -s)" = "Linux" ] && echo -e "add_to_LD_LIBRARY_PATH \${CAFANACORE_LIB}" >> support_software_env.sh

  fi
fi

if [ -z ${TBB_VERSION} ]; then # Don't have TBB version set
(
cat <<EOF
#include "tbb/tbb.h"
#include <iostream>
int main(){ 
  std::cout << TBB_VERSION_MAJOR << "." 
            << TBB_VERSION_MINOR 
            << std::endl; }
EOF
) > tbb_version.cxx
  g++ -std=c++11 tbb_version.cxx -I ${TBB_INC} -Wl,-rpath,${TBB_LIB} -L ${TBB_LIB} -ltbb 

  export TBB_VERSION=$(./a.out)
  rm tbb_version.cxx a.out
  echo -e "export TBB_VERSION=\"${TBB_VERSION}\"" >> support_software_env.sh
fi
