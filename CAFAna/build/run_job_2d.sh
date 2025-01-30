#!/bin/bash

#stateFiles=("/exp/dune/data/users/gyang/cafana_state/myown/myown_all")
#stateFiles=("/exp/dune/data/users/gyang/cafana_state/State")
#stateFiles=("/exp/dune/data/users/gyang/cafana_state/toy_2perEnergy_test")
stateFiles=("/exp/dune/data/users/gyang/cafana_state/myown_reweighting_test")

#systSets=("nosyst" "allsyst" "fluxsyst" "fluxdetsyst")
#systSets=("nosyst" "noxsec" "noxsec:nodet" "allsyst")
#systSets=("allsyst")
systSets=("nosyst")

#systSets=("allsyst")

for i in "${!stateFiles[@]}"; do
    echo "Processing index: $i"
    stateFile=${stateFiles[$i]}

    for systSet in "${systSets[@]}"; do
        echo "Applying systematics set: $systSet"
        stateFileName="${stateFile##*/}"

        # Loop through integers for intVal (currently set to just 9)
        for intVal in {9..9}; do
            # Loop through integers for intVal2 from 0 to 36
            for intVal2 in {10..10}; do
                # Dynamically create the output file name based on stateFile, systSet, intVal, and intVal2
                outputFile="${stateFileName%.*}_${systSet}_624ktmwyr_icp_${intVal}_int2_${intVal2}_movingtest.root"
                echo "Running: cpv_joint $stateFile $outputFile $systSet $intVal $intVal2"
                cpv_joint_2d "$stateFile" "$outputFile" "$systSet" "$intVal" "$intVal2"
            done
        done
    done
done

