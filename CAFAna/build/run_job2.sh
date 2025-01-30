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
    echo $i
    stateFile=${stateFiles[$i]}

    for systSet in "${systSets[@]}"; do
        echo $systSet
        stateFileName="${stateFiles##*/}"

        # Loop through integers 0 to 36
        for intVal in {9..9}; do
            # Dynamically create the output file name based on stateFile, systSet, and intVal
            outputFile="${stateFileName%.*}_${systSet}_624ktmwyr_icp_${intVal}_movingtest.root"
            echo "Running: cpv_joint $stateFile $outputFile $systSet $intVal"
            cpv_joint "$stateFile" "$outputFile" "$systSet" "$intVal"
        done
    done
done

