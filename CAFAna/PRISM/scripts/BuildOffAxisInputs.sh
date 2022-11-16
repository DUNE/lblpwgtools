#!/bin/bash

# REMEMBER -f arg for FHC, -r arg for RHC!!

for i in RHC; do # RHC; do

  #On axis is treated differently

  ## Standard on axis run
  for j in 00 01 02 03 04 05 06 07 08 09; do
    
    OffAxisNDCAFCombiner \
      -i "/pnfs/dune/persistent/users/marshalc/nd_offaxis/v7/CAF/0mRHC/${j}/${i}*.root" \
      -o CAFv7_0m_${j}_${i}.root \
      -t caf \
      -p \
      -x -4000,400,25 \
      -r \
      -s 200 \
        
    ## Special 280 kA run
  
    OffAxisNDCAFCombiner \
      -i "/pnfs/dune/persistent/users/marshalc/nd_offaxis/v7/CAF/0mRHC/${j}/${i}*.root" \
      -o CAFv7_280kA_0m_${j}_${i}.root \
      -t caf \
      -p \
      -x -4000,400,25 \
      -r \
      -n 200 \
      --280kA
 
  done

  for p in 2mRHC 4mRHC 8mRHC 12mRHC 16mRHC 20mRHC 24mRHC 28mRHC; do

    for k in 00 01 02 03 04 05 06 07 08 09; do 
 
      OffAxisNDCAFCombiner \
        -i "/pnfs/dune/persistent/users/marshalc/nd_offaxis/v7/CAF/${p}/${k}/${i}*.root" \
        -o CAFv7_${p}_${k}_${i}.root \
        -t caf \
        -p \
        -x -4000,400,25\
        -r \
    
    done

  done

  wait

  # Combine them all
  OffAxisNDCAFCombiner \
    -i "./CAFv7_*_${i}.root" \
    -o ND_${i}_absxPOT_Sep21.root \
    -x -4000,400,25 \
    -r \
    -C

done
