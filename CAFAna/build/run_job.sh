#!/bin/bash

stateFiles=("/exp/dune/data/users/gyang/cafana_state/State")
#stateFiles=("/exp/dune/data/users/gyang/cafana_state/State")
#systSets=("nosyst" "allsyst" "fluxsyst" "fluxdetsyst")
systSets=("nosyst" "noxsec" "noxsec:nodet" "allsyst")

#systSets=("allsyst")

for i in "${!stateFiles[@]}"; do
    echo $i
    stateFile=${stateFiles[$i]}
    
    for systSet in "${systSets[@]}"; do
        echo $systSet
        stateFileName="${stateFiles##*/}"
        # Dynamically create the output file name based on stateFile and systSet
        outputFile="${stateFileName%.*}_${systSet}_624ktmwyr_2nd.root"
        echo "cpv_joint "$stateFile" "$outputFile" "$systSet""
	cpv_joint "$stateFile" "$outputFile" "$systSet"
    done
done


