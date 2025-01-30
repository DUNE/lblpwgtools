#!/bin/sh
#  gpvm_novasoft_mrb.sh
#  create a novasoft mrb build area on a novagpvm node
#  Created by Brian Rebel on 3/22/16.
#  Modified by Adam Lister on 2020-02-28

# catch attempt to execute script
if [ "${BASH_SOURCE[0]}" -ef "$0" ]
then
  echo "Hi there! Please source this script rather than executing it"
  exit 1
fi

# usage
if [ "$#" -ne 4 ]; then
  echo "Usage: source ${BASH_SOURCE[0]} <novasoft release> <novasoft qualifiers> <buildtype> <novasoft destination directory>"
  echo " -- <novasoft release>              : develop, or branches/<branch name>, tags/<tag name"
  echo " -- <novasoft qualifiers>           : look in the srcs/ups/product_deps to find what these should be"
  echo " -- <buildtype>                     : prof or debug, usually prof"
  echo " -- <novasoft destination directory>: location to setup the build, localproducts, and srcs directories"
  echo "For example: ${BASH_SOURCE[0]} develop eII <debug | prof> $HOME/nova/"
  return
fi

RELEASE=${1}
QUALS=${2}
BUILDTYPE=${3}
NOVADIR=${4}
BUNDLEQUALS=`echo ${QUALS} | sed -e 's/:/-/g'`
RELNAME=`echo ${RELEASE} | sed 's:.*/::'`
echo "Check inputs: "
echo "   NOvASoft Qualifiers:      ${QUALS}"
echo "   Build type:               ${BUILDTYPE}"
echo "   NOvASoft destination:     ${NOVADIR}"
echo "   Release Name:             ${RELNAME}"

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
mrb newDev -v ${RELNAME} -q ${QUALS}:${BUILDTYPE}

# make the local products directory name a bit more generic
BUNDLEQUALS=`echo ${QUALS} | sed -e 's/:/_/g'`
mv localProducts_${MRB_PROJECT}_${RELNAME}_${BUNDLEQUALS}_${BUILDTYPE} localProducts_${MRB_PROJECT}
sed -i~ 's/localProducts_'"${MRB_PROJECT}"'_'"${RELNAME}"'_'"${BUNDLEQUALS}_${BUILDTYPE}"'/localProducts_'"${MRB_PROJECT}"'/g' localProducts_${MRB_PROJECT}/setup
source localProducts_${MRB_PROJECT}/setup

echo "checking out the novasoft code"
cd ${MRB_SOURCE}
echo "checking out ${RELEASE}"

if [ "$RELEASE" = "develop" ]
then
  RELEASE="trunk"
fi

echo "RELEASE: ${RELEASE}"

mrb svn -d novasoft svn+ssh://p-novaart@cdcvs.fnal.gov/cvs/projects/novaart/pkgs.svn/${RELEASE}
mrb uc

echo "set the build environment"
cd ${MRB_BUILDDIR}
mrbsetenv
setup ninja v1_8_2

# this just picks up the product name from the product_deps
VERSION=`grep "parent novasoft" ${MRB_SOURCE}/novasoft/ups/product_deps | cut -c 17-`

echo "If no errors were reported, you are good to go"
echo " "
echo "Now type: mrb i --generator ninja "
echo " "
echo "Here is an example function you can add to your $HOME/.bash_profile to setup for novasoft development:"
echo " "
echo "develop_${VERSION}_nova ()"
echo "{"
echo " "
echo " source /cvmfs/nova.opensciencegrid.org/externals/setup"
echo " setup mrb"
echo " source ${MRB_INSTALL}/setup"
echo " export PRODUCTS="'${PRODUCTS}'":/cvmfs/nova.opensciencegrid.org/externals/:/cvmfs/nova.opensciencegrid.org/pidlibs/products"
echo " setup ninja v1_8_2 #optional if using ninja to build the code, requires mrb i --generator ninja"
echo " setup git"
echo " cd ${MRB_BUILDDIR}"
echo " mrbsetenv"
echo " "
echo "}"
echo " "
echo "Here is an example function you can add to your $HOME/.bash_profile to setup novasoft for running:"
echo " "
echo "setup_${VERSION}_nova ()"
echo "{"
echo " "
echo " setup mrb"
echo " source /cvmfs/fermilab.opensciencegrid.org/products/common/etc/setups.sh"
echo " export MYLP=${MRB_INSTALL}"
echo " export PRODUCTS="'${PRODUCTS}'":/cvmfs/nova.opensciencegrid.org/externals/:/cvmfs/nova.opensciencegrid.org/pidlibs/products"
echo " export PRODUCTS="'${MYLP}'":"'${PRODUCTS}'
echo " source "'${MYLP}'"/setup"
echo " setup novasoft ${VERSION} -q ${QUALS}:${BUILDTYPE}"
echo " "
echo "}"
echo " "
echo "Here is an example function you can add to your $HOME/.bash_profile to build a grid build of novasoft"
echo " "
echo "build_${VERSION}_grid ()"
echo "{"
echo " "
echo " mkdir ${MRB_TOP}/build_grid"
echo " cd ${MRB_TOP}/build_grid"
echo " source /cvmfs/fermilab.opensciencegrid.org/products/common/etc/setups.sh"
echo " export PRODUCTS=${MRB_INSTALL}:/cvmfs/nova.opensciencegrid.org/externals/:/grid/fermiapp/products/common/db:/nova/data/pidlibs/products:"'${PRODUCTS}'
echo " setup ninja v1_8_2"
echo " source ${MRB_SOURCE}/novasoft/ups/setup_for_development -p ${QUALS}:grid"
echo " export NOVASOFT_DIR="'$CETPKG_SOURCE'
echo " buildtool --generator ninja -i -I ${MRB_INSTALL}"
echo " "
echo "}"
echo " "
echo "Here is an example script you can add to your $HOME/.bash_profile to setup novasoft for grid running"
echo " "
echo "setup_${VERSION}_nova_grid ()"
echo "{"
echo " "
echo " source /cvmfs/fermilab.opensciencegrid.org/products/common/etc/setups.sh"
echo " export PRODUCTS="'${PRODUCTS}'":/cvmfs/nova.opensciencegrid.org/externals/:/cvmfs/nova.opensciencegrid.org/pidlibs/products"
echo " export PRODUCTS=${MRB_INSTALL}:"'${PRODUCTS}'
echo " setup novasoft ${VERSION} -q ${QUALS}:${BUILDTYPE}:grid"
echo " setup sam_web_client"
echo " "
echo "}"
