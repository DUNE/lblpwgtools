#!/bin/bash

DO_BUILD="0"
USE_GPERF="0"
DO_PUSH="0"
BASE_IMAGE="centos:8"
TAGNAME="latest"
DO_KNL="0"
REMOVE_SOURCE="1"
USE_PRISM="0"

NCORES=8

IMAGE_NAME="${USER}/cafana"
BRANCH_NAME="master"
REPO_URL="https://github.com/DUNE/lblpwgtools.git"
STATE_FILE_LOC=""
ROOT_VERS="v6-12-06"
CLHEP_VERS="2.4.1.3"
BUILD_TYPE="RELEASE"

SCRIPTNAME=${0}

while [[ ${#} -gt 0 ]]; do

  key="$1"
  case $key in

      -f|--force-build)

      DO_BUILD="1"
      echo "[OPT]: Will rebuild container even if latest already exists."
      ;;

      -p|--push)

      DO_PUSH="1"
      echo "[OPT]: Will attempt to push to docker hub."
      ;;

      --use-gperftools)

      USE_GPERF="1"
      echo "[OPT]: Will compile in gperftools support."
      ;;

      --use-PRISM)

      USE_PRISM="1"
      IMAGE_NAME="${USER}/cafana_prism"
      BRANCH_NAME="feature/PRISM"
      echo "[OPT]: Will compile in PRISM support."
      ;;

      --keep-source)

      REMOVE_SOURCE="0"
      echo "[OPT]: Will not remove the source."
      ;;

      -d|--debug)

      BUILD_TYPE="DEBUG"
      TAGNAME="latest_deb"
      echo "[OPT]: Will compile debug image type."
      ;;

      -j|--n-cores)

      if [[ ${#} -lt 2 ]]; then
        echo "[ERROR]: ${1} expected a value."
        exit 1
      fi

      NCORES="$2"
      echo "[OPT]: Will use \"${NCORES}\" cores."
      shift # past argument
      ;;

      --knl)

      DO_KNL="1"
      echo "[OPT]: Will compile for KNL arch."
      ;;

      -I|--image-name)

      if [[ ${#} -lt 2 ]]; then
        echo "[ERROR]: ${1} expected a value."
        exit 1
      fi

      IMAGE_NAME="$2"
      echo "[OPT]: Will name image: \"${IMAGE_NAME}\"."
      shift # past argument
      ;;

      -S|--state-files)

      if [[ ${#} -lt 2 ]]; then
        echo "[ERROR]: ${1} expected a value."
        exit 1
      fi

      STATE_FILE_LOC="$(readlink -f $2)"
      if [ ! -e ${STATE_FILE_LOC} ]; then
        echo "[ERROR]: When trying to specify packaged statefiles from: \"${STATE_FILE_LOC}\", they do not appear to exist."
        exit
      fi
      echo "[OPT]: Will pull in statefiles from: \"${STATE_FILE_LOC}\"."
      shift # past argument
      ;;

      -R|--repo-url)

      if [[ ${#} -lt 2 ]]; then
        echo "[ERROR]: ${1} expected a value."
        exit 1
      fi

      REPO_URL="$2"
      echo "[OPT]: Will build from repo: \"${REPO_URL}\"."
      shift # past argument
      ;;

      -B|--branch-name)

      if [[ ${#} -lt 2 ]]; then
        echo "[ERROR]: ${1} expected a value."
        exit 1
      fi

      BRANCH_NAME="$2"
      echo "[OPT]: Will build from branch: \"${BRANCH_NAME}\"."
      shift # past argument
      ;;

      -T|--tag-name)

      if [[ ${#} -lt 2 ]]; then
        echo "[ERROR]: ${1} expected a value."
        exit 1
      fi

      TAGNAME="$2"
      echo "[OPT]: Will tag image: \"${TAGNAME}\"."
      shift # past argument
      ;;

      -?|--help)
      echo "[RUNLIKE] ${SCRIPTNAME}"
      echo -e "\t-f|--force-build       : Will rebuild even if a latest image exists."
      echo -e "\t-d|--debug             : Compile with CMAKE_BUILD_TYPE=DEBUG"
      echo -e "\t-p|--push              : Attempt to push up to docker hub. Note you should have issued a docker login before running this script."
      echo -e "\t--use-gperftools       : Compile libunwind and gperftools"
      echo -e "\t--use-PRISM            : Compile in PRISM support (sets branch and image name)"
      echo -e "\t-j|--n-cores           : Number of cores to pass to make install."
      echo -e "\t--base-image           : Base image to use (default: centos:latest)."
      echo -e "\t--knl                  : Build CAFAna copy with -march=knl"
      echo -e "\t--keep-source          : Do not remove CAFAna source"
      echo -e "\t-I|--image-name        : Built image name (default: ${USER}/dune_cafana)."
      echo -e "\t-S|--state-files <stub>: Include input state files in the container."
      echo -e "\t-T|--tag-name          : Built/pushed tag name (default: latest)."
      echo -e "\t-R|--repo-url          : git repository to use (default: ${REPO_URL})."
      echo -e "\t-B|--branch-name       : Remote branch to use (default: ${BRANCH_NAME})."
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

# Some useful functions for working with buildah
function bb_commit {
  local CONT=${1}
  local IMAGE_FQNAME=${2}
  local IMAGE_PRETTYNAME=${3}
  buildah config --entrypoint /bin/bash ${CONT}
  bb_cont_set_env ${CONT} container_name ${IMAGE_PRETTYNAME}
  bb_cont_set_psone ${CONT} ${IMAGE_PRETTYNAME}
  local ID=$(buildah commit ${CONT} ${IMAGE_FQNAME})
  echo "[BB]: Committed ${CONT} -> ${IMAGE_FQNAME}"
  local RMID=$(buildah rm ${CONT})
  echo "[BB]: Removed ${CONT}"
}

function bb_cont_get_env {
    local CONT=$1
    local ENVNAME=$2

    # Have to check if it is empty or it will print an error
    if buildah run ${CONT} printenv ${ENVNAME} &> /dev/null; then
      buildah run ${CONT} printenv ${ENVNAME}
    fi
}

function bb_cont_set_env {
  local CONT=$1
  local ENVNAME=$2
  local ENVVALUE=$3

  if [[ $ENVNAME =~ "=" ]]; then
    echo "Environment variable name passed to bb_cont_set_env contained an equals character, this is an invalid name. (Maybe you used bb_cont_set_env \${ENV} ENVNAME=ENVVAL? Should be bb_cont_set_env \${ENV} ENVNAME ENVVAL"
    return 1
  fi

  buildah config --env ${ENVNAME}=${ENVVALUE} ${CONT}
}

function bb_add_to_path_env {
  local CONT=$1
  local ENVNAME=$2
  local ENVVALUE=$3

  if [[ $ENVNAME =~ "=" ]]; then
    echo "Environment variable name passed to bb_cont_set_env contained an equals character, this is an invalid name. (Maybe you used bb_cont_set_env \${ENV} ENVNAME=ENVVAL? Should be bb_cont_set_env \${ENV} ENVNAME ENVVAL"
    return 1
  fi

  CONTVAL=$(bb_cont_get_env ${CONT} ${ENVNAME})
  bb_cont_set_env ${CONT} ${ENVNAME} "${ENVVALUE}:${CONTVAL}"
}

function bb_cont_set_psone {
    local CONT=$1
    local STR=$2
    buildah run ${CONT} sed -i -e "\$a PS1=\"\\\\[\\\\033[01;31m\\\\]${STR}\\\\[\\\\033[00m\\\\]:\\\\[\\\\033[01;34m\\\\]\\\\w\\\\[\\\\033[00m\\\\]# \"" /root/.bashrc
}

function bb_cont_to_cont_copy {
    local FROM=$1
    local TO=$2
    local SRC=$3
    local DEST=$4

cat << EOF > /tmp/$$.copy
#!/bin/bash
FROM_MP=\$(buildah mount ${FROM})
buildah copy ${TO} \$FROM_MP$SRC $DEST &> /dev/null
buildah unmount ${FROM} &> /dev/null
EOF

  chmod +x /tmp/$$.copy
  buildah unshare /tmp/$$.copy
  rm /tmp/$$.copy
}

function bb_cont_mv {
  local CONT=$1
  shift
  local CMD="mv $@"
  buildah run ${CONT} bash -c "${CMD}"
}

function bb_cont_cp {
  local CONT=$1
  shift
  local CMD="cp $@"
  buildah run ${CONT} bash -c "${CMD}"
}

function bb_cont_cd {
  local CONT=${1}
  local LOC=${2}

  buildah config --workingdir ${LOC} ${CONT}
}

function bb_cont_mkdir {
  local CONT=${1}
  shift
  local LOCS="$@"

  buildah run ${CONT} mkdir -p ${LOCS}
}

function bb_cont_mkdir_cd {
  local CONT=${1}
  local LOC=${2}

  bb_cont_mkdir ${CONT} ${LOC}
  bb_cont_cd ${CONT} ${LOC}
}


#Abort if anything at all goes wrong
set -e

#We're going to build this in stages so that you don't have to rebuild dependencies each time

#Check if we already have the buildbox
if ! podman image exists ${USER}/cafana_buildbox:latest; then

  BUILD_CONT=$(buildah from ${BASE_IMAGE})

  BUILD_LIBS="gsl-devel boost-devel bzip2-devel pcre-devel xz-devel zlib-devel freetype-devel openssl-devel fftw-devel"
  RETREVAL_SW="git wget subversion openssh"
  MISC_SW="python36"

  buildah run ${BUILD_CONT} dnf -y groupinstall "development tools" --with-optional
  buildah run ${BUILD_CONT} dnf -y install \
                                  ${BUILD_LIBS} \
                                  ${RETREVAL_SW} \
                                  ${MISC_SW}

  bb_commit ${BUILD_CONT} ${USER}/cafana_buildbox:latest

fi

ROOT_TAG="latest"
if [ "${BUILD_TYPE}" == "DEBUG" ]; then
  ROOT_TAG="latest_deb"
fi

# Do we have a rootbox?
if ! podman image exists ${USER}/cafana_rootbox:${ROOT_TAG}; then

  ROOT_BUILD_CONT=$(buildah from ${USER}/cafana_buildbox:latest)

  bb_cont_mkdir_cd ${ROOT_BUILD_CONT} /opt

  buildah run ${ROOT_BUILD_CONT} git clone \
    https://github.com/root-project/root.git /opt/root-src

  bb_cont_cd ${ROOT_BUILD_CONT} /opt/root-src

  buildah run ${ROOT_BUILD_CONT} git checkout tags/${ROOT_VERS}

  bb_cont_mkdir_cd ${ROOT_BUILD_CONT} /opt/root-build

  buildah run ${ROOT_BUILD_CONT} cmake /opt/root-src \
        -Dminuit2=ON \
        -Dmathmore=ON \
        -Dtmva=OFF -Drootfit=OFF \
        -Dx11=OFF -Dxft=OFF -Dopengl=OFF \
        -Dminimal=ON \
        -DCMAKE_CXX_STANDARD=14 \
        -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
        -DCMAKE_INSTALL_PREFIX=/opt/root/${ROOT_VERS}

  buildah run ${ROOT_BUILD_CONT} make install -j ${NCORES}

  bb_commit ${ROOT_BUILD_CONT} ${USER}/cafana_rootbox:${ROOT_TAG}

fi

# Do we have a clhep box?
if ! podman image exists ${USER}/cafana_clhepbox:latest; then

  CLHEP_BUILD_CONT=$(buildah from ${USER}/cafana_buildbox:latest)

  bb_cont_mkdir_cd ${CLHEP_BUILD_CONT} /opt/clhep-src

  buildah run ${CLHEP_BUILD_CONT} wget http://proj-clhep.web.cern.ch/proj-clhep/dist1/clhep-${CLHEP_VERS}.tgz
  buildah run ${CLHEP_BUILD_CONT} tar -zxvf clhep-${CLHEP_VERS}.tgz  

  bb_cont_mkdir_cd ${CLHEP_BUILD_CONT} /opt/clhep-build

  buildah run ${CLHEP_BUILD_CONT} cmake /opt/clhep-src/${CLHEP_VERS}/CLHEP \
        -DCMAKE_CXX_STANDARD=14 \
        -DCMAKE_BUILD_TYPE=RELEASE \
        -DCMAKE_INSTALL_PREFIX=/opt/clhep/${CLHEP_VERS}

  buildah run ${CLHEP_BUILD_CONT} make install -j ${NCORES}

  bb_commit ${CLHEP_BUILD_CONT} ${USER}/cafana_clhepbox:latest
  
fi

# image combining all the deps
if ! podman image exists ${IMAGE_NAME}_compilebox:${TAGNAME}; then

  CAF_BUILD_CONT=$(buildah from ${USER}/cafana_buildbox:latest)

  CLHEP_TEMP_CONT=$(buildah from ${USER}/cafana_clhepbox:latest)
  #copy in clhep
  bb_cont_to_cont_copy ${CLHEP_TEMP_CONT} ${CAF_BUILD_CONT} /opt/clhep /opt/clhep
  buildah rm  ${CLHEP_TEMP_CONT}

  ROOT_TEMP_CONT=$(buildah from ${USER}/cafana_rootbox:${ROOT_TAG})
  #copy in root
  bb_cont_to_cont_copy ${ROOT_TEMP_CONT} ${CAF_BUILD_CONT} /opt/root /opt/root
  buildah rm  ${ROOT_TEMP_CONT}

  #root env
  ROOTSYS="/opt/root/${ROOT_VERS}"
  bb_cont_set_env ${CAF_BUILD_CONT} ROOTSYS ${ROOTSYS}
  bb_add_to_path_env ${CAF_BUILD_CONT} PATH ${ROOTSYS}/bin
  bb_add_to_path_env ${CAF_BUILD_CONT} LD_LIBRARY_PATH ${ROOTSYS}/lib
  bb_add_to_path_env ${CAF_BUILD_CONT} LIBPATH ${ROOTSYS}/lib
  bb_add_to_path_env ${CAF_BUILD_CONT} JUPYTER_PATH ${ROOTSYS}/etc/notebook
  bb_add_to_path_env ${CAF_BUILD_CONT} DYLD_LIBRARY_PATH ${ROOTSYS}/lib
  bb_add_to_path_env ${CAF_BUILD_CONT} PYTHONPATH ${ROOTSYS}/lib
  bb_add_to_path_env ${CAF_BUILD_CONT} SHLIB_PATH ${ROOTSYS}/lib
  bb_add_to_path_env ${CAF_BUILD_CONT} CMAKE_PREFIX_PATH ${ROOTSYS}

  # clhep env
  CLHEP="/opt/clhep/${CLHEP_VERS}"
  bb_cont_set_env ${CAF_BUILD_CONT} CLHEP ${CLHEP}
  bb_add_to_path_env ${CAF_BUILD_CONT} PATH ${CLHEP}/bin

  bb_cont_mkdir_cd ${CAF_BUILD_CONT} /opt/cafana-src
  buildah run ${CAF_BUILD_CONT} git clone ${REPO_URL}
  bb_cont_cd ${CAF_BUILD_CONT} /opt/cafana-src/lblpwgtools
  buildah run ${CAF_BUILD_CONT} git checkout ${BRANCH_NAME}
  bb_cont_cd ${CAF_BUILD_CONT} /opt/cafana-src/lblpwgtools/CAFAna

  RELOPT="-r"
  if [ ${BUILD_TYPE} == "DEBUG" ]; then
    RELOPT=""
  fi

  PRISMOPT=""
  if [ ${USE_PRISM} == "1" ]; then
    PRISMOPT="--use-PRISM"
  fi

  bb_cont_set_env ${CAF_BUILD_CONT} BOOST_LIB /usr/lib64
  bb_cont_set_env ${CAF_BUILD_CONT} GSL_LIB /usr/lib64

  buildah run ${CAF_BUILD_CONT} ./standalone_configure_and_build.sh ${RELOPT} -j ${NCORES} -I /opt/cafana ${PRISMOPT}

  bb_commit ${CAF_BUILD_CONT} ${IMAGE_NAME}_compilebox:${TAGNAME}

fi

if ! podman image exists ${IMAGE_NAME}:${TAGNAME}; then

  CAF_RUN_CONT=$(buildah from ${BASE_IMAGE})

  #root needs c++ header files
  BUILD_LIBS="libstdc++-devel gsl bzip2 boost pcre xz zlib freetype openssl fftw"
  RETREVAL_SW="git wget krb5-libs openssh"
  MISC_SW="python36 python2 which nano sed vim"

  buildah run ${CAF_RUN_CONT} dnf -y install \
                                  ${BUILD_LIBS} \
                                  ${RETREVAL_SW} \
                                  ${MISC_SW}

  CLHEP_TEMP_CONT=$(buildah from ${USER}/cafana_clhepbox:latest)
  #copy in clhep
  bb_cont_to_cont_copy ${CLHEP_TEMP_CONT} ${CAF_RUN_CONT} /opt/clhep /opt/clhep
  buildah rm  ${CLHEP_TEMP_CONT}

  ROOT_TEMP_CONT=$(buildah from ${USER}/cafana_rootbox:${ROOT_TAG})
  #copy in root
  bb_cont_to_cont_copy ${ROOT_TEMP_CONT} ${CAF_RUN_CONT} /opt/root /opt/root
  buildah rm  ${ROOT_TEMP_CONT}

  CAFANA_TEMP_CONT=$(buildah from ${IMAGE_NAME}_compilebox:${TAGNAME})
  #copy in root
  bb_cont_to_cont_copy ${CAFANA_TEMP_CONT} ${CAF_RUN_CONT} /opt/cafana /opt/cafana
  buildah rm ${CAFANA_TEMP_CONT}

  # root env
  ROOTSYS="/opt/root/${ROOT_VERS}"
  bb_cont_set_env ${CAF_RUN_CONT} ROOTSYS ${ROOTSYS}
  bb_add_to_path_env ${CAF_RUN_CONT} PATH ${ROOTSYS}/bin
  bb_add_to_path_env ${CAF_RUN_CONT} LD_LIBRARY_PATH ${ROOTSYS}/lib
  bb_add_to_path_env ${CAF_RUN_CONT} LIBPATH ${ROOTSYS}/lib
  bb_add_to_path_env ${CAF_RUN_CONT} JUPYTER_PATH ${ROOTSYS}/etc/notebook
  bb_add_to_path_env ${CAF_RUN_CONT} DYLD_LIBRARY_PATH ${ROOTSYS}/lib
  bb_add_to_path_env ${CAF_RUN_CONT} PYTHONPATH ${ROOTSYS}/lib
  bb_add_to_path_env ${CAF_RUN_CONT} SHLIB_PATH ${ROOTSYS}/lib
  bb_add_to_path_env ${CAF_RUN_CONT} CMAKE_PREFIX_PATH ${ROOTSYS}

  # clhep env
  CLHEP="/opt/clhep/${CLHEP_VERS}"
  bb_cont_set_env ${CAF_RUN_CONT} CLHEP ${CLHEP}
  bb_add_to_path_env ${CAF_RUN_CONT} PATH ${CLHEP}/bin
  bb_add_to_path_env ${CAF_RUN_CONT} LD_LIBRARY_PATH ${CLHEP}/lib

  bb_cont_mkdir_cd ${CAF_RUN_CONT} /opt/cafana-src
  buildah run ${CAF_RUN_CONT} git clone ${REPO_URL}
  bb_cont_cd ${CAF_RUN_CONT} /opt/cafana-src/lblpwgtools
  buildah run ${CAF_RUN_CONT} git checkout ${BRANCH_NAME}
  bb_cont_cd ${CAF_RUN_CONT} /opt/cafana-src/lblpwgtools/CAFAna

  # cafana env
  bb_add_to_path_env ${CAF_RUN_CONT} PATH /opt/cafana/bin
  bb_add_to_path_env ${CAF_RUN_CONT} LD_LIBRARY_PATH /opt/cafana/lib
  bb_add_to_path_env ${CAF_RUN_CONT} FHICL_FILE_PATH /opt/cafana/fcl
  if [ "${USE_PRISM}" == "1" ]; then
    bb_add_to_path_env ${CAF_RUN_CONT} FHICL_FILE_PATH /opt/cafana/fcl/PRISM
    bb_add_to_path_env ${CAF_RUN_CONT} FHICL_FILE_PATH /opt/cafana/fcl/PRISM/example
  fi
  bb_cont_set_env ${CAF_RUN_CONT} CAFANA_USE_NDCOVMAT 1
  bb_cont_set_env ${CAF_RUN_CONT} CAFANA_USE_UNCORRNDCOVMAT 0
  bb_cont_set_env ${CAF_RUN_CONT} CAFANA_DONT_CLAMP_SYSTS 0
  bb_cont_set_env ${CAF_RUN_CONT} CAFANA /opt/cafana
  bb_cont_set_env ${CAF_RUN_CONT} CAFANA_ANALYSIS_VERSION 4
  bb_cont_set_env ${CAF_RUN_CONT} CAFANA_IGNORE_SELECTION 0
  bb_cont_set_env ${CAF_RUN_CONT} CAFANA_FIT_FORCE_HESSE 1
  bb_cont_set_env ${CAF_RUN_CONT} CAFANA_FIT_TURBOSE 0
  bb_cont_set_env ${CAF_RUN_CONT} CAFANA_DISABLE_DERIVATIVES 1
  bb_cont_set_env ${CAF_RUN_CONT} CAFANA_PRED_MINMCSTATS 50
  bb_cont_set_env ${CAF_RUN_CONT} CAFANA_IGNORE_CV_WEIGHT 0


  #add a python symlink, because...
  buildah run ${CAF_RUN_CONT} ln -s /usr/bin/python3 /usr/bin/python

  if [ ! -z ${STATE_FILE_LOC} ]; then
      bb_cont_mkdir ${CAF_RUN_CONT} /statefiles
      buildah add ${CAF_RUN_CONT} ${STATE_FILE_LOC}/*.root /statefiles/
  fi

  bb_commit ${CAF_RUN_CONT} ${IMAGE_NAME}:${TAGNAME}

fi