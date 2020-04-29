# Container for CAFAna

# Start with latest debian slim
FROM debian:stretch-slim

ARG N_CORE=8

RUN apt update && apt install -y build-essential gfortran-6 libgsl-dev libboost-dev libboost-filesystem-dev cmake subversion git python
ENV BOOST_INC=/usr/include
ENV BOOST_LIB=/usr/lib/x86_64-linux-gnu

# CLHEP
RUN mkdir -p /opt/clhep-src
WORKDIR /opt/clhep-src/
RUN git clone https://gitlab.cern.ch/CLHEP/CLHEP.git
WORKDIR /opt/clhep-src/CLHEP
RUN git checkout CLHEP_2_4_1_0
RUN mkdir -p /opt/clhep-build
WORKDIR /opt/clhep-build/
RUN cmake /opt/clhep-src/CLHEP -DCMAKE_CXX_STANDARD=14 -DCMAKE_BUILD_TYPE=RELEASE
RUN make -j ${N_CORE}
RUN make install -j ${N_CORE}
WORKDIR /opt/
RUN rm -rf /opt/clhep-src /opt/clhep-build

# ROOT
RUN mkdir -p /opt/root-src
RUN mkdir -p /opt/root-build
RUN git clone https://github.com/root-project/root.git /opt/root-src
WORKDIR /opt/root-src
RUN git checkout tags/v6-12-06
WORKDIR /opt/root-build
ENV USE_PARALLEL_MINUIT2=0
RUN cmake /opt/root-src -Dminimal=ON -DCMAKE_CXX_STANDARD=14 -Dcxx14=ON -Dminuit2=ON -DCMAKE_BUILD_TYPE=RELEASE
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

# CAFAna
RUN mkdir -p /opt/CAFAna-src
WORKDIR /opt/CAFAna-src/
ARG CAFANA_GIT_REF
RUN git clone https://github.com/DUNE/lblpwgtools.git
WORKDIR /opt/CAFAna-src/lblpwgtools
RUN git checkout ${CAFANA_GIT_REF}
RUN mkdir -p /opt/CAFAna-build
WORKDIR /opt/CAFAna-build/
RUN mkdir /opt/CAFAna-build/Ext
WORKDIR /opt/CAFAna-build/Ext
RUN svn -r 37166 checkout https://cdcvs.fnal.gov/subversion/novaart.pkgs.svn/trunk/OscLib
RUN svn -r 37166 checkout https://cdcvs.fnal.gov/subversion/novaart.pkgs.svn/trunk/Utilities
WORKDIR /opt/CAFAna-build/Ext/OscLib/func/
RUN sed -i "s/array_wrapper.hpp/array.hpp/g" /opt/CAFAna-build/Ext/OscLib/func/OscCalculatorGeneral.cxx
WORKDIR /opt/CAFAna-build/
RUN cmake /opt/CAFAna-src/lblpwgtools/code/CAFAna/CAFAna -DCMAKE_CXX_STANDARD=14 -DCMAKE_INSTALL_PREFIX=/opt/CAFAna -DCMAKE_BUILD_TYPE=RELEASE -DUSED_UPS=0 -DSRC_ROOT_PARENT=/opt/CAFAna-src/lblpwgtools/code/CAFAna -DGSL_LIB=/usr/lib  -DBOOST_INC=${BOOST_INC} -DBOOST_LIB=${BOOST_LIB} -DUSE_OPENMP=1 -DUSE_PRISM=${USE_PRISM}
RUN make -j ${N_CORE}
RUN make install -j ${N_CORE}
WORKDIR /opt/
RUN rm -rf /opt/CAFAna-src /opt/CAFAna-build

ENV PATH "/opt/CAFAna/bin:${PATH}"
ENV LD_LIBRARY_PATH "/opt/CAFAna/lib:${LD_LIBRARY_PATH}"
ENV CAFANA /opt/CAFAna
ENV CAFANA_ANALYSIS_VERSION "4"
ENV CAFANA_USE_UNCORRNDCOVMAT "0"
ENV CAFANA_USE_NDCOVMAT "1"
ENV CAFANA_IGNORE_CV_WEIGHT "0"
ENV CAFANA_IGNORE_SELECTION "0"
ENV CAFANA_DISABLE_DERIVATIVES "1"
ENV CAFANA_DONT_CLAMP_SYSTS "0"
ENV CAFANA_FIT_TURBOSE "0"
ENV CAFANA_FIT_FORCE_HESSE "1"
ENV CAFANA_PRED_MINMCSTATS "50"

#Get rid of everything we don't need
RUN apt remove -y build-essential gfortran-6 libgsl-dev libboost-dev cmake subversion git python
RUN apt install -y libgsl2 libstdc++-6-dev procps
RUN apt autoremove -y --purge &&  apt clean

# Tidy up some stuff ROOT installs needlessly
RUN rm -rf /usr/local/{test,tutorials,README}

ADD statefiles/*.root /statefiles/

WORKDIR /

ENTRYPOINT ["/bin/bash"]
CMD []
