#!/bin/sh

#  create_manifest.sh
#  Created by Brian Rebel on 12/15/16.


# build novasoft
# use mrb
# designed to work on Jenkins
# this is a proof of concept script

echo "novasoft version: $NOVA"
echo "base qualifiers: $QUAL"
echo "nutools qualifiers: $NUTOOLS_QUAL"
echo "build type: $BUILDTYPE"
echo "workspace: $WORKSPACE"

# Get number of cores to use.

if [ `uname` = Darwin ]; then
#ncores=`sysctl -n hw.ncpu`
#ncores=$(( $ncores / 4 ))
ncores=16
else
ncores=`cat /proc/cpuinfo 2>/dev/null | grep -c -e '^processor'`
fi
if [ $ncores -lt 1 ]; then
ncores=1
fi
echo "Building using $ncores cores."

# Environment setup, uses /grid/fermiapp or cvmfs.

if [ -f /grid/fermiapp/products/nova/externals/setup ]; then
source /grid/fermiapp/products/nova/externals/setup || exit 1
else
echo "No setup file found."
exit 1
fi

# Use system git on macos.

if ! uname | grep -q Darwin; then
setup git || exit 1
fi
setup gitflow || exit 1
export MRB_PROJECT=novasoft
echo "Mrb path:"
which mrb

set -x
rm -rf $WORKSPACE/temp || exit 1
mkdir -p $WORKSPACE/temp || exit 1
mkdir -p $WORKSPACE/copyBack || exit 1
rm -f $WORKSPACE/copyBack/* || exit 1
cd $WORKSPACE/temp || exit 1
mrb newDev -v $NOVA -q $QUAL:$BUILDTYPE || exit 1

set +x
source localProducts*/setup || exit 1

# some shenanigans so we can use getopt v1_1_6
if [ `uname` = Darwin ]; then
setup getopt v1_1_6  || exit 1
fi

set -x
cd $MRB_SOURCE  || exit 1
# make sure we get a read-only copy
mrb g -r -t $NOVA novasoft || exit 1

cd $MRB_BUILDDIR || exit 1
mrbsetenv || exit 1
mrb b -j$ncores || exit 1
mrb mp -n novasoft -- -j$ncores || exit 1

manifest=novasoft-*_MANIFEST.txt

# Extract the versions of the various nova specific external products needed: nutools, caffe, fann, 
# novaenv, mrb, novadaqdeps, novadaq, novaddtdeps, novaddt, calibcsvs

nutools_version=`grep nutools $MRB_SOURCE/novasoft/ups/product_deps | grep -v qualifier | awk '{print $2}'`
novadaq_version=`grep novadaq $MRB_SOURCE/novasoft/ups/product_deps | grep -v qualifier | awk '{print $2}'`
novaenv_version=`grep novaenv $MRB_SOURCE/novasoft/ups/product_deps | grep -v qualifier | awk '{print $2}'`
   fann_version=`grep fann    $MRB_SOURCE/novasoft/ups/product_deps | grep -v qualifier | awk '{print $2}'`
  caffe_version=`grep caffe   $MRB_SOURCE/novasoft/ups/product_deps | grep -v qualifier | awk '{print $2}'`

nutools_dot_version=`echo ${nutools_version} | sed -e 's/_/./g' | sed -e 's/^v//'`
novadaq_dot_version=`echo ${novadaq_version} | sed -e 's/_/./g' | sed -e 's/^v//'`
novaenv_dot_version=`echo ${novadaq_version} | sed -e 's/_/./g' | sed -e 's/^v//'`
   fann_dot_version=`echo ${novadaq_version} | sed -e 's/_/./g' | sed -e 's/^v//'`
  caffe_dot_version=`echo ${novadaq_version} | sed -e 's/_/./g' | sed -e 's/^v//'`

# add external products to the manifest
echo "novadaq        ${novadaq_version}          novadaq-${novadaq_dot_version}-noarch.tar.gz" >>  $manifest

# Extract flavor.

flvr=''
if uname | grep -q Darwin; then
flvr=`ups flavor -2`
else
flvr=`ups flavor`
fi

# Construct name of larsoft manifest.

nutools_hyphen_qual=`echo $NUTOOLS_QUAL | tr : - | sed 's/-noifdh//'`
nutools_manifest=nutools-${nutools_dot_version}-${flvr}-${nutools_hyphen_qual}-${BUILDTYPE}_MANIFEST.txt
echo "Nutools manifest:"
echo $nutools_manifest
echo

# Fetch nutools manifest from scisoft and append to novasoft manifest.

curl --fail --silent --location --insecure http://scisoft.fnal.gov/scisoft/bundles/nutools/${nutools_version}/manifest/${nutools_manifest} >> $manifest || exit 1

# Special handling of noifdh builds goes here.

if echo $QUAL | grep -q noifdh; then

if uname | grep -q Darwin; then

# If this is a macos build, then rename the manifest to remove noifdh qualifier in the name

noifdh_manifest=`echo $manifest | sed 's/-noifdh//'`
mv $manifest $noifdh_manifest

else

# Otherwise (for slf builds), delete the manifest entirely.

rm -f $manifest

fi
fi

# Save artifacts.

mv *.bz2  $WORKSPACE/copyBack/ || exit 1
manifest=novasoft-*_MANIFEST.txt
if [ -f $manifest ]; then
mv $manifest  $WORKSPACE/copyBack/ || exit 1
fi
#cp $MRB_BUILDDIR/lariatsoft/releaseDB/*.html $WORKSPACE/copyBack/
ls -l $WORKSPACE/copyBack/
cd $WORKSPACE || exit 1
rm -rf $WORKSPACE/temp || exit 1
set +x

exit 0
