#!/bin/bash

source `ups setup srproxy v00.04 -q py2 -z /cvmfs/nova.opensciencegrid.org/externals/`

gen_srproxy -i StandardRecord/StandardRecord.h -o SRProxy -t caf::StandardRecord --include-path $CAFANA/include/ --output-path StandardRecord/Proxy --epilog-fwd EpilogFwd.h

g++ SRProxy.cxx -I../.. -I$SRPROXY_INC `root-config --cflags --libs` -c -oSRProxy.so

g++ $SRPROXY_INC/SRProxy/BasicTypesProxy.cxx -I$SRPROXY_INC `root-config --cflags --libs` -c -oBasicTypesProxy.so
