#!/bin/bash

if ! g++ remake_inputs.C -o remake_inputs.exe $(root-config --cflags) -I ../../releases/development/OscLib/ -I ../../releases/development/ ../../releases/development/lib/Linux4.1-GCC-maxopt/lib*.so $(root-config --glibs) -lMinuit2 ${CLHEP_LIB_DIR}/libCLHEP.a; then
  echo "[ERROR]: Failed to build remake_inputs."
  exit 1
fi

# for STATE in standard_state, FromDep_state,FromDep 1DND_state,1DND CoarseBin1D_state,CoarseBin1D VeryCoarseBin1D_state,VeryCoarseBin1D OneBin_state,OneBin; do

for STATE in ETrue_state,ETrue; do

  STATEDIR=$(echo $STATE | cut -d , -f 1)
  STATESTR=$(echo $STATE | cut -d , -f 2)
  echo "STATEDIR: \"${STATEDIR}\" STATESTR:\"${STATESTR}\""

  mkdir -p ${STATEDIR}
  baseFileName=${STATEDIR}/common_state_mcc11v3
  for sample in FD_FHC FD_RHC ND_FHC ND_RHC; do
      ./remake_inputs.exe "${baseFileName}_${sample}.root" "$sample" 10000000 ${STATESTR} &> /dev/null &
  done

  wait

done

# STATEDIR="standard_state"
# mkdir -p ${STATEDIR}
# baseFileName=${STATEDIR}/common_state_mcc11v3
# for sample in FD_FHC FD_RHC ND_FHC ND_RHC
# do
#     ./remake_inputs.exe "${baseFileName}_${sample}.root" "$sample" 10000000 &
# done

# STATEDIR="FromDep_state"
# mkdir -p ${STATEDIR}
# baseFileName=${STATEDIR}/common_state_mcc11v3
# for sample in FD_FHC FD_RHC ND_FHC ND_RHC
# do
#     ./remake_inputs.exe "${baseFileName}_${sample}.root" "$sample" 10000000 "FromDep" &
# done
#
# STATEDIR="1DND_state"
# mkdir -p ${STATEDIR}
# baseFileName=${STATEDIR}/common_state_mcc11v3
# for sample in ND_FHC ND_RHC
# do
#     ./remake_inputs.exe "${baseFileName}_${sample}.root" "$sample" 10000000 "1DND" &
# done

# STATEDIR="CoarseBin1D_state"
# mkdir -p ${STATEDIR}
# baseFileName=${STATEDIR}/common_state_mcc11v3
# for sample in FD_FHC FD_RHC ND_FHC ND_RHC
# do
#     ./remake_inputs.exe "${baseFileName}_${sample}.root" "$sample" 10000000 "CoarseBin1D" &
# done

# STATEDIR="VeryCoarseBin1D_state"
# mkdir -p ${STATEDIR}
# baseFileName=${STATEDIR}/common_state_mcc11v3
# for sample in FD_FHC FD_RHC ND_FHC ND_RHC
# do
#     ./remake_inputs.exe "${baseFileName}_${sample}.root" "$sample" 1000000 "VeryCoarseBin1D" &
# done

# STATEDIR="OneBin_state"
# mkdir -p ${STATEDIR}
# baseFileName=${STATEDIR}/common_state_mcc11v3
# for sample in FD_FHC FD_RHC ND_FHC ND_RHC
# do
#     ./remake_inputs.exe "${baseFileName}_${sample}.root" "$sample" 1000000 "OneBin" &
# done
# Now hadd the files together: hadd ${baseFileName}.root ${baseFileName}\*.root
