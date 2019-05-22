#!/bin/bash

if [ "$(sudo docker images -q cafana/centos7 2> /dev/null)" == "" ] || [ "$1" == "build" ]; then

NCORES=8
echo -e "#!/bin/bash\nyum install -y --setopt=tsflags=nodocs svn which make redhat-lsb-core glibc-devel && yum clean all;cd /opt;source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh;git clone https://github.com/DUNE/lblpwgtools.git;cd lblpwgtools;git checkout strong_and_stable;cd code/CAFAna/CAFAna;./standalone_configure_and_build.sh -j ${NCORES} -u -r;cat /build_scripts/CAFEnvWrapper.sh > /CAFEnvWrapper.sh; chmod +x /CAFEnvWrapper.sh" > CAFBuildScript.sh
echo -e '#!/bin/bash\nsource /opt/lblpwgtools/code/CAFAna/CAFAna/build/Linux/CAFAnaEnv.sh; cd /workdir; echo $@; $@' > CAFEnvWrapper.sh

sudo docker rm CAFAnaCENTOS7

sudo docker run --name CAFAnaCENTOS7 -v /cvmfs:/cvmfs:shared -v $(pwd):/build_scripts  centos:latest bash /build_scripts/CAFBuildScript.sh
REV=$(sudo docker run --rm -v /cvmfs:/cvmfs:shared cafana/centos7 bash -c "source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh &> /dev/null; cd /opt/lblpwgtools;git rev-parse --short HEAD")
sudo docker commit CAFAnaCENTOS7 cafana_dune:$REV
sudo docker rmi cafana_dune:latest
sudo docker commit CAFAnaCENTOS7 cafana_dune:latest

  if [ "$1" == "build" ]; then
    shift;
  fi

fi

if [ "$1" == "test" ]; then
  mkdir -p workdir
  sudo docker run \
    -v /cvmfs:/cvmfs:shared \
    -v $(pwd)/statefiles:/statefiles:shared \
    -v $(pwd)/workdir:/workdir:shared \
    --rm \
    cafana_dune:latest \
      ./CAFEnvWrapper.sh asimov_joint \
        /statefiles/common_state_mcc11v3  \
        asimov_test.root \
        deltapi \
        nosyst \
        fd
fi

