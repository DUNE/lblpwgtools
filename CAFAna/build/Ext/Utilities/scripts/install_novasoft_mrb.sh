#!/bin/sh

#  install_novasoft_mrb.sh
#  Install the ups products needed by novasoft
#  Created by Brian Rebel on 3/22/16.

if [ "$#" -ne 5 ]; then
  echo "Usage: install_novasoft_mrb.sh <external ups destination directory> <novasoft base version> <novasoft qualifiers> <buildtype> <novasoft destination directory>"
  echo " For example: install_novasoft_ups.sh /cvmfs/nova.opensciencegrid.org/externals vXX_YY_ZZ sJJ:eII <debug | prof> $HOME/nova/"
  echo " the lastest version of the novasoft bundle can be found at http://scisoft.fnal.gov/scisoft/bundles/novasoft/"
  exit
fi

PRODDIR=${1}
BASEV=${2}
QUALS=${3}
BUILDTYPE=${4}
NOVADIR=${5}

BUNDLEQUALS=`echo ${QUALS} | sed -e 's/:/-/g'`

echo "Check inputs: "
echo "   External UPS destination: ${PRODUCTS}"
echo "   NOvASoft Base version:    ${BASEV}"
echo "   NOvASoft Qualifiers:      ${QUALS}"
echo "   Build type:               ${BUILDTYPE}"
echo "   NOvASoft destination:     ${NOVADIR}"

# determine the OS
OS=`uname`
if [ "${OS}" = "Linux" ]
then
  flvr=slf`lsb_release -r | sed -e 's/[[:space:]]//g' | cut -f2 -d":" | cut -f1 -d"."`
elif [ "${OS}" = "Darwin" ]
then
  flvr=d`uname -r | cut -f1 -d"."`
  QUALS="${QUALS}:osx"
else
  echo "ERROR: unrecognized operating system ${OS}"
  exit 1
fi

cd ${PRODDIR}
curl -O http://scisoft.fnal.gov/scisoft/bundles/tools/pullProducts
chmod +x pullProducts
./pullProducts ${PRODDIR} ${flvr} novasoft-${BASEV} ${BUNDLEQUALS} ${BUILDTYPE}

# source the ups products setup
source ${PRODDIR}/setup

# setup mrb and create the project
echo "setting up the mrb working area"

if [ ! -d "${NOVADIR}" ]; then
  echo "NOvASoft destination directory not found, make it"
  mkdir ${NOVADIR}
fi

cd ${NOVADIR}

setup mrb

export MRB_PROJECT=novasoft

mrb newDev -v develop -q ${QUALS}:${BUILDTYPE}

# make the local products directory name a bit more generic
BUNDLEQUALS=`echo ${QUALS} | sed -e 's/:/_/g'`

mv localProducts_${MRB_PROJECT}_develop_${BUNDLEQUALS}_${BUILDTYPE} localProducts_${MRB_PROJECT}

sed -i~ 's/localProducts_'"${MRB_PROJECT}"'_develop_'"${BUNDLEQUALS}_${BUILDTYPE}"'/localProducts_'"${MRB_PROJECT}"'/g' localProducts_${MRB_PROJECT}/setup

source localProducts_${MRB_PROJECT}/setup

echo "checking out the novasoft code"

cd ${MRB_SOURCE}
mrb svn -d novasoft svn+ssh://p-novaart@cdcvs.fnal.gov/cvs/projects/novaart/pkgs.svn/trunk
mrb uc

echo "set the build environment"
cd ${MRB_BUILDDIR}
mrbsetenv

echo "If no errors were reported, you are good to go"
echo " "
echo "Here is an example function you can add to your $HOME/.bash_profile to setup for novasoft development:"
echo " "
echo "develop_nova ()"
echo "{"
echo " "
echo " source ${PRODDIR}/setup"
echo " setup mrb"
echo " setup ninja v1_8_2 #optional if using ninja to build the code, requires mrb i --generator ninja"
echo " source ${NOVADIR}/localProducts_${MRB_PROJECT}/setup"
echo " cd ${MRB_BUILDDIR}"
echo " mrbsetenv"
echo " echo now in ${PWD} "
echo "}"

echo " "
echo "Here is an example function you can add to your $HOME/.bash_profile to setup novasoft for running:"
echo " "
echo "setup_nova ()"
echo "{"
echo " "
echo " source ${PRODDIR}/setup"
echo " export PRODUCTS=/grid/fermiapp/products/common/db:/nova/data/pidlibs/products:${PRODUCTS}"
echo " export PRODUCTS=${NOVADIR}/localProducts_${MRB_PROJECT}/:${PRODUCTS}"
echo " "
echo "  # setup sam to be able to find files"
echo "  kx509"
echo "  kxlist -p"
echo "  voms-proxy-init --rfc --voms=fermilab:/fermilab/nova/Role=Analysis --noregen"
echo "  export X509_USER_PROXY=/tmp/x509up_u`id -u`"
echo " "
echo " #setup the novasoft ups product"
echo " setup novasoft develop -q ${QUALS}:${BUILDTYPE}
echo " cd ${NOVADIR}"
echo " "
echo " ready to run nova in ${PWD} "
echo "}"

