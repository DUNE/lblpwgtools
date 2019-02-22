#!/bin/bash

if [ -z ${CAFANA} ]; then
  # Build the exe so that backgrounded root processes don't fight.
  if ! g++ remake_inputs.C -o remake_inputs.exe $(root-config --cflags) -I ${SRT_DIST}/releases/development/OscLib/ -I ${SRT_DIST}/releases/development/ ${SRT_DIST}/releases/development/lib/Linux*-GCC-maxopt/lib*.so $(root-config --glibs) -lMinuit2 ${CLHEP_LIB_DIR}/libCLHEP.a; then
    echo "[ERROR]: Failed to build remake_inputs."
    exit 1
  fi
else
  CAFBuild remake_inputs.C
fi

if [ ! -e remake_inputs.exe ]; then
  exit 1
fi

# for STATE in standard_state, FromDep_state,FromDep 1DND_state,1DND CoarseBin1D_state,CoarseBin1D VeryCoarseBin1D_state,VeryCoarseBin1D OneBin_state,OneBin; do

# Build FakeData inputs, A,B where A is the sub directory that inputs will end up in and B is the axes identifier, see remake_inputs.C for available identifiers.
# These inputs can be used without hadding by passing e.g. FakeData_state/common_state_mcc11v3 as the input name and kAddSampleTypeToFilename to RunFitPoint.
for STATE in FakeData_state,FakeData; do

  STATEDIR=$(echo $STATE | cut -d , -f 1)
  STATESTR=$(echo $STATE | cut -d , -f 2)
  echo "STATEDIR: \"${STATEDIR}\" STATESTR:\"${STATESTR}\""

  mkdir -p ${STATEDIR}
  baseFileName=${STATEDIR}/common_state_mcc11v3
  for sample in FD_FHC FD_RHC ND_FHC ND_RHC; do
      ./remake_inputs.exe "${baseFileName}_${sample}.root" "$sample" 10000000 ${STATESTR}
  done

done
