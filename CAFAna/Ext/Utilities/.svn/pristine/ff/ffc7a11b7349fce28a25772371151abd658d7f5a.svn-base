#!/bin/sh

# build_novasoft_cmake_jenkins.sh
# pull source code in $WORKSPACE/source
# build in $WORKSPACE/build
# copyback directory is $WORKSPACE/copyBack

usage()
{
cat 1>&2 <<EOF
Usage: $(basename ${0}) [-h]
$(basename ${0}) <version> <qual_set> <build_type>

Options:
-h    This help.

Arguments:

qual_set         Supported qualifier sets: s21:e9, etc

EOF
}

while getopts :h OPT; do
 case ${OPT} in
  h)
  usage
  exit 1
  ;;
  *)
  usage
  exit 1
 esac
done
shift `expr $OPTIND - 1`
OPTIND=1

working_dir=${WORKSPACE}
version=${NOVAVER}
qual_set="${QUAL}"
build_type=${BUILDTYPE}

case ${qual_set} in
 s21:e9)
 basequal=e9
 squal=s21
 nuver=v1_16_01
 novaenvver=v01_02_00
 fannver=v2_2_0f
 novadaqver=v04_03_03
 novadaqdepsver=v02_01_02
 novaddtdepsver=v01_01_07
 novaddtver=v03_04_03
 ;;
 *)
 usage
 exit 1
esac

case ${build_type} in
 debug) ;;
 prof) ;;
 *)
 usage
 exit 1
esac

dotver=`echo ${version} | sed -e 's/_/./g' | sed -e 's/^v//'`
fanndotver=`echo ${fannversion} | sed -e 's/_/./g' | sed -e 's/^v//'`
novadaqdotver=`echo ${novadaqversion} | sed -e 's/_/./g' | sed -e 's/^v//'`
novaddtdotver=`echo ${novaddtversion} | sed -e 's/_/./g' | sed -e 's/^v//'`

echo "building the novasoft base distribution for ${version} ${dotver} ${qual_set} ${build_type}"

OS=`uname`
if [ "${OS}" = "Linux" ]
then
 flvr=slf`lsb_release -r | sed -e 's/[[:space:]]//g' | cut -f2 -d":" | cut -f1 -d"."`
elif [ "${OS}" = "Darwin" ]
then
 flvr=d`uname -r | cut -f1 -d"."`
else
 echo "ERROR: unrecognized operating system ${OS}"
 exit 1
fi

echo "build flavor is ${flvr}"
echo ""

qualdir=`echo ${qual_set} | sed -e 's%:%-%'`

set -x

blddir=${working_dir}/build
srcdir=${working_dir}/source

# start with clean directories
rm -rf ${blddir}
rm -rf ${srcdir}
rm -rf $WORKSPACE/copyBack

# now make the dfirectories
mkdir -p ${srcdir} || exit 1
mkdir -p ${blddir} || exit 1
mkdir -p $WORKSPACE/copyBack || exit 1

cd ${blddir} || exit 1

curl --fail --silent --location --insecure -O http://scisoft.fnal.gov/scisoft/bundles/tools/pullProducts || exit 1
chmod +x pullProducts

./pullProducts ${blddir} source nu-${version} || \
{
 cat 1>&2 <<EOF
 ERROR: pull of nu-${version} failed
 EOF
 exit 1
}

# pull in the source tar balls for novadaq, novaddt, fann, then append
# those files to the nu source manifest
curl --fail --silent --location --insecure -O http://scisoft.fnal.gov/scisoft/packages/novadaq/${novadaqver}/novadaq-${novadaqdotver}-source.tar.bz2
curl --fail --silent --location --insecure -O http://scisoft.fnal.gov/scisoft/packages/novaddt/${novaddtver}/novaddt-${novaddtdotver}-source.tar.bz2
curl --fail --silent --location --insecure -O http://scisoft.fnal.gov/scisoft/packages/fann/${fannver}/fann-${fanndotver}-source.tar.bz2

mv ${blddir}/*source* ${srcdir}/

cd ${blddir} || exit 1

# pulling binaries is allowed to fail
./pullProducts ${blddir} ${flvr} nu-${nuver} ${squal}-${basequal} ${build_type}

# pull in the necessary nova products: novadaq, novaddt, novaenv, fann

echo
echo "begin build"
echo

./buildFW -t -b ${basequal} -s ${squal} ${blddir} ${build_type} novasoft-${version} || \
{
 mv ${blddir}/*.log  $WORKSPACE/copyBack/
 exit 1
}

echo
echo "move files"
echo

mv ${blddir}/*.bz2  $WORKSPACE/copyBack/
mv ${blddir}/*.txt  $WORKSPACE/copyBack/
mv ${blddir}/*.log  $WORKSPACE/copyBack/

exit 0
