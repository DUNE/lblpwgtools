# Container for CAFAna

# Start with latest CentOS
FROM centos:7

ARG N_CORE=8

# Set up a more modern system (gcc6)
RUN yum install -y --setopt=tsflags=nodocs centos-release-scl && yum clean all
RUN yum install -y --setopt=tsflags=nodocs devtoolset-7 && yum clean all
SHELL ["scl", "enable", "devtoolset-7"]

RUN echo BUILDING WITH ${N_CORE} CORES

# Get some useful packages
RUN yum install -y --setopt=tsflags=nodocs git tar wget gsl-devel boost-devel && yum clean all
ENV BOOST_INC=/usr/include

# cmake
RUN mkdir -p /opt/cmake-src
WORKDIR /opt/cmake-src
RUN wget https://cmake.org/files/v3.9/cmake-3.9.1.tar.gz
RUN tar xfvz cmake-3.9.1.tar.gz
WORKDIR /opt/cmake-src/cmake-3.9.1
RUN ./bootstrap
RUN make install -j ${N_CORE}
WORKDIR /opt
RUN rm -rf /opt/cmake-src

# CLHEP
RUN mkdir -p /opt/clhep-src
WORKDIR /opt/clhep-src/
RUN git clone https://gitlab.cern.ch/CLHEP/CLHEP.git
WORKDIR /opt/clhep-src/CLHEP
RUN git checkout CLHEP_2_4_1_0
RUN mkdir -p /opt/clhep-build
WORKDIR /opt/clhep-build/
RUN cmake /opt/clhep-src/CLHEP -DCMAKE_CXX_STANDARD=14
RUN make -j ${N_CORE}
RUN make install -j ${N_CORE}
WORKDIR /opt/
RUN rm -rf /opt/clhep-src /opt/clhep-build

# PYTHIA 6
RUN mkdir -p /opt/pythia6-src
WORKDIR /opt/pythia6-src/
RUN wget https://root.cern.ch/download/pythia6.tar.gz
RUN wget --no-check-certificate http://www.hepforge.org/archive/pythia6/pythia-6.4.28.f.gz
RUN gunzip pythia-6.4.28.f.gz
RUN tar xfvz pythia6.tar.gz
RUN mv pythia-6.4.28.f pythia6/pythia6428.f
RUN rm -rf pythia6/pythia6416.f
WORKDIR /opt/pythia6-src/pythia6
RUN ./makePythia6.linuxx8664
RUN mv libPythia6.so /usr/lib64/
WORKDIR /opt/
RUN rm -rf /opt/pythia6-src/

# ROOT
RUN mkdir -p /opt/root-src
RUN mkdir -p /opt/root-build
RUN mkdir -p /opt/root
RUN git clone https://github.com/root-project/root.git /opt/root-src
WORKDIR /opt/root-src
RUN git checkout tags/v6-12-06
WORKDIR /opt/root-build
ENV USE_PARALLEL_MINUIT2=0
RUN cmake /opt/root-src -Dmathmore=ON -Dpythia6=ON -DPYTHIA6_LIBRARY=/usr/lib64/libPythia6.so -Dminimal=ON -DCMAKE_CXX_STANDARD=14 -Dcxx14=ON -Dminuit2=ON
RUN make -j ${N_CORE}
RUN make install -j ${N_CORE}
WORKDIR /opt/
RUN rm -rf /opt/root-src /opt/root-build

# Apply effect of running /usr/local/bin/this.sh in the container
ENV LD_LIBRARY_PATH "/usr/local/lib:${LD_LIBRARY_PATH}"
ENV LIBPATH /usr/local/lib
ENV JUPYTER_PATH /usr/local/etc/notebook
ENV PATH "/usr/local/bin:/usr/local/sbin:${PATH}"
ENV DYLD_LIBRARY_PATH /usr/local/lib
ENV PYTHONPATH /usr/local/lib
ENV SHLIB_PATH /usr/local/lib
ENV CMAKE_PREFIX_PATH /usr/local
ENV ROOTSYS /usr/local

RUN yum install -y --setopt=tsflags=nodocs svn && yum clean all

# CAFAna
RUN mkdir -p /opt/CAFAna-src
WORKDIR /opt/CAFAna-src/
RUN git clone https://github.com/DUNE/lblpwgtools.git
WORKDIR /opt/CAFAna-src/lblpwgtools
RUN git checkout strong_and_less_stable
RUN mkdir -p /opt/CAFAna-build
WORKDIR /opt/CAFAna-build/
RUN mkdir /opt/CAFAna-build/Ext
WORKDIR /opt/CAFAna-build/Ext
RUN svn checkout https://cdcvs.fnal.gov/subversion/novaart.pkgs.svn/trunk/OscLib
RUN svn checkout https://cdcvs.fnal.gov/subversion/novaart.pkgs.svn/trunk/Utilities
WORKDIR /opt/CAFAna-build/Ext/OscLib/func/
RUN sed -i "s/array_wrapper.hpp/array.hpp/g" /opt/CAFAna-build/Ext/OscLib/func/OscCalculatorGeneral.cxx
WORKDIR /opt/CAFAna-build/
RUN cmake /opt/CAFAna-src/lblpwgtools/code/CAFAna/CAFAna -DCMAKE_CXX_STANDARD=14 -DCMAKE_INSTALL_PREFIX=/opt/CAFAna -DCMAKE_BUILD_TYPE=RELEASE -DUSED_UPS=0 -DSRC_ROOT_PARENT=/opt/CAFAna-src/lblpwgtools/code/CAFAna -DGSL_LIB=/usr/lib64
RUN make -j ${N_CORE}
RUN make install -j ${N_CORE}
WORKDIR /opt/
RUN rm -rf /opt/CAFAna-src /opt/CAFAna-build

SHELL ["/usr/bin/bash", "-c"]

RUN yum remove -y centos-release-scl devtoolset-7 git wget gsl-devel boost-devel svn && yum clean all
RUN rm -rf /opt/root /opt/rh

RUN yum install -y --setopt=tsflags=nodocs gsl && yum clean all

ADD statefiles/*.root /statefiles/

WORKDIR /

ENTRYPOINT ["/usr/bin/bash", "-c", "/opt/CAFAna/Linux/CAFAnaEnv.sh"]
CMD []
