#!/bin/bash

inputFileName=common_state_mcc11v2.root
for systs in nosyst flux det xsec allsyst
do
    cafe -q -b "cpv_joint.C+(\"${inputFileName}\", \"cpv_sens_fd_${systs}.root\", \"${systs}\", false)"
    cafe -q -b "cpv_joint.C+(\"${inputFileName}\", \"cpv_sens_ndfd_${systs}.root\", \"${systs}\", true)"
done

# Now hadd the files together: hadd ${baseFileName}.root ${baseFileName}\*.root
