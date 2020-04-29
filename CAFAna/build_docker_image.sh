#!/bin/bash

DO_BUILD="0"
DO_RELEASE="1"
USE_GPERF="0"
DO_PUSH="0"
BASE_IMAGE="centos:latest"
TAGNAME="latest"
DO_KNL="0"
REMOVE_SOURCE="1"
USE_PRISM="0"

NCORES=8

IMAGE_NAME="${USER}/dune_cafana"
BRANCH_NAME="strong_and_stable"
REPO_URL="https://github.com/DUNE/lblpwgtools.git"
STATE_FILE_LOC=""

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
      IMAGE_NAME="${USER}/dune_cafana_PRISM"
      BRANCH_NAME="feature/PRISM"
      echo "[OPT]: Will compile in PRISM support."
      ;;

      --keep-source)

      REMOVE_SOURCE="0"
      echo "[OPT]: Will not remove the source."
      ;;

      -d|--debug)

      DO_RELEASE="0"
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


LATEST_EXISTS=$(sudo docker images -q ${IMAGE_NAME}:latest 2> /dev/null)

if [ "${LATEST_EXISTS}" == "" ] || [ "${DO_BUILD}" == "1" ]; then

  ROPT=""
  if [ "${DO_RELEASE}" == "1" ]; then
    ROPT="-r "
  fi

  GOPT=""
  if [ "${USE_GPERF}" == "1" ]; then
    GOPT="--use-gperftools "
    ROPT="--rdb "
  fi
  POPT=""
  if [ "${USE_PRISM}" == "1" ]; then
    POPT="--use-PRISM"
  fi

  echo -e "#!/bin/bash\nyum install -y --setopt=tsflags=nodocs svn which make redhat-lsb-core glibc-devel automake libtool && yum clean all;cd /opt;source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh;if ! git clone ${REPO_URL} lblpwgtools; then exit 1; fi;cd lblpwgtools;git checkout ${BRANCH_NAME};cd code/CAFAna/CAFAna;./standalone_configure_and_build.sh -j ${NCORES} -u ${ROPT}${GOPT}${POPT}-I /opt/CAFAna/; cat /build_scripts/CAFEnvWrapper.sh > /opt/CAFAna/CAFEnvWrapper.sh; chmod -R 775 /opt/CAFAna; if [ \"${DO_KNL}\" == \"1\" ]; then ./standalone_configure_and_build.sh -f -j ${NCORES} --knl -u ${ROPT}${GOPT}-I /opt/CAFAna_knl/; cat /build_scripts/CAFEnvWrapper_knl.sh > /opt/CAFAna_knl/CAFEnvWrapper.sh; chmod -R 775 /opt/CAFAna_knl; fi; if [ \"${REMOVE_SOURCE}\" == \"1\" ]; then rm -rf /opt/lblpwgtools; fi; if [ -e /statefiles_in ]; then cp -r /statefiles_in /statefiles; chmod -R 744 /statefiles; fi" > CAFBuildScript.sh
  echo -e '#!/bin/bash\nsource /opt/CAFAna/CAFAnaEnv.sh; echo "[CAFENV]: $@"; $@' > CAFEnvWrapper.sh
  echo -e '#!/bin/bash\nsource /opt/CAFAna_knl/CAFAnaEnv.sh; echo "[CAFENV]: $@"; $@' > CAFEnvWrapper_knl.sh

  sudo docker rm CAFAnaImageBuild_tmp

  ETAG=""
  if [ ! -z "${STATE_FILE_LOC}" ]; then
    ETAG="_wsf"
    CMD="sudo docker run --name CAFAnaImageBuild_tmp -v /cvmfs:/cvmfs:shared -v $(pwd):/build_scripts -v ${STATE_FILE_LOC}:/statefiles_in ${BASE_IMAGE} bash /build_scripts/CAFBuildScript.sh"
    echo "${CMD}"
    ${CMD}
  else
    CMD="sudo docker run --name CAFAnaImageBuild_tmp -v /cvmfs:/cvmfs:shared -v $(pwd):/build_scripts ${BASE_IMAGE} bash /build_scripts/CAFBuildScript.sh"
    echo "${CMD}"
    ${CMD}
  fi
  TAGNAME="${TAGNAME}${ETAG}"


  BRANCH_HEAD_REV=$(git ls-remote ${REPO_URL} | awk "/${BRANCH_NAME}/ {print \$1}" | cut -f 1 | cut -c-10)

  REV_TAG="${BRANCH_HEAD_REV}"
  if [ "${TAGNAME}" != "latest" ]; then
    REV_TAG="${BRANCH_HEAD_REV}_${TAGNAME}"
  fi

  sudo docker rmi ${USER}/dune_cafana:${REV_TAG}
  sudo docker commit CAFAnaImageBuild_tmp ${USER}/dune_cafana:${REV_TAG}
  sudo docker rmi ${USER}/dune_cafana:${TAGNAME}
  sudo docker commit CAFAnaImageBuild_tmp ${USER}/dune_cafana:${TAGNAME}

  sudo docker rm CAFAnaImageBuild_tmp

  if [ "${DO_PUSH}" == "1" ]; then
    echo "[INFO]: Pushing to docker hub..."
    sudo docker push ${USER}/dune_cafana:${REV_TAG}
    sudo docker push ${USER}/dune_cafana:${TAGNAME}
  fi
else
  echo "[INFO]: Image exists, rebuild not forced."
  if [ "${DO_PUSH}" == "1" ]; then
    echo "[INFO]: Pushing to docker hub..."
    sudo docker push ${USER}/dune_cafana:${TAGNAME}
  fi
fi
