#!/bin/sh

#  gpvm_novasoft_mrb.sh
#  create a novasoft mrb build area on a novagpvm node
#  Created by Brian Rebel on 3/22/16.

if [ "$#" -ne 3 ]; then
    echo "Usage: `basename ${0}` <novasoft qualifiers> <buildtype> <novasoft destination directory>"
    echo " For example: `basename ${0}` eII <debug | prof> $HOME/nova/"
    echo " the qualifiers are the same as for the nutools ups product or you can look in ups/product_deps for the indicated default quals"
    exit
fi

QUALS=${1}
BUILDTYPE=${2}
NOVADIR=${3}

BUNDLEQUALS=`echo ${QUALS} | sed -e 's/:/-/g'`

echo "Check inputs: "
echo "   NOvASoft Base version:    ${BASEV}"
echo "   NOvASoft Qualifiers:      ${QUALS}"
echo "   Build type:               ${BUILDTYPE}"
echo "   NOvASoft destination:     ${NOVADIR}"


# source the ups products setup
PRODDIR=/cvmfs/fermilab.opensciencegrid.org/products/common/etc/
source ${PRODDIR}/setups.sh
export PRODUCTS=${PRODUCTS}:/cvmfs/nova.opensciencegrid.org/externals/

# make sure there are no other UPS products setup.
unsetup_all

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

setup ninja v1_8_2

echo "If no errors were reported, you are good to go"
echo " "
echo "Now type: mrb i --generator ninja "
echo " "
echo "Here is an example function you can add to your $HOME/.bash_profile to setup for novasoft development:"
echo " "
echo "develop_nova ()"
echo "{"
echo " "
echo " source ${PRODDIR}/setups.sh"
echo " export PRODUCTS=${PRODUCTS}:/cvmfs/nova.opensciencegrid.org/externals/:/cvmfs/nova.opensciencegrid.org/pidlibs/products"
echo " source ${MRB_INSTALL}/setup"
echo " setup mrb"
echo " setup ninja v1_8_2 #optional if using ninja to build the code, requires mrb i --generator ninja"
echo " cd ${MRB_BUILDDIR}"
echo " mrbsetenv"
echo " echo 'now in ${PWD}' "
echo "}"

echo " "
echo "Here is an example function you can add to your $HOME/.bash_profile to setup novasoft for running:"
echo " "
echo "setup_nova ()"
echo "{"
echo " "
echo " source ${PRODDIR}/setup"
echo " source ${NOVADIR}/localProducts_${MRB_PROJECT}/setup"
echo " setup novasoft develop -q ${QUALS}:${BUILDTYPE}"
echo " cd ${NOVADIR}"
echo " echo 'ready to run nova in ${PWD}' "
echo "}"
