#!/bin/bash


for i in RHC; do # RHC; do

  #On axis is treated differently

  ## Standard on axis run
#  for j in 00 01 02 03 04 05 06 07 08 09; do
#10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29; do
    
#    OffAxisNDCAFCombiner \
#      -i "/pnfs/dune/persistent/users/marshalc/nd_offaxis/v7/CAF/0mRHC/${j}/${i}*.root" \
#      -o CAFv7_0m_${j}_${i}.root \
#      -t caf \
#      -p \
#      -x -4000,400,25 \
#      -f \
#      -s 200 \
        
    ## Special 280 kA run
  
#    OffAxisNDCAFCombiner \
#      -i "/pnfs/dune/persistent/users/marshalc/nd_offaxis/v7/CAF/0mRHC/${j}/${i}*.root" \
#      -o CAFv7_280kA_0m_${j}_${i}.root \
#      -t caf \
#      -p \
#      -x -4000,400,25 \
#      -f \
#      -n 200 \
#      --280kA
# 
#  done

  for p in 16mRHC 20mRHC 24mRHC 28mRHC; do

    for k in 00 01 02 03 04 05 06 07 08 09; do 
#10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29; do
 
      OffAxisNDCAFCombiner \
        -i "/pnfs/dune/persistent/users/marshalc/nd_offaxis/v7/CAF/${p}/${k}/${i}*.root" \
        -o CAFv7_${p}_${k}_${i}.root \
        -t caf \
        -p \
        -x -4000,400,25\
        -f \
    
    done

  done

  wait

  # Combine them all
  OffAxisNDCAFCombiner \
    -i "./CAFv7_*_${i}.root" \
    -o ND_${i}_absxPOT_Jul21.root \
    -x -4000,400,25\
    -f \
    -C

done
