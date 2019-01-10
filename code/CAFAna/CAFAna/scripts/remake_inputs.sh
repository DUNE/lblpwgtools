#!/bin/bash

baseFileName=common_state_mcc11v2
for sample in FD_FHC FD_RHC ND_FHC ND_RHC
do
    cafe -q -b "remake_inputs.C+(\"${baseFileName}_${sample}.root\", \"$sample\")" &
done

# Now hadd the files together: hadd ${baseFileName}.root ${baseFileName}\*.root
