#!/bin/bash


for i in FHC; do # RHC; do

  #On axis is treated differently

  ## Standard on axis run
  for j in 00 01 02 03 04 05 06 07 08 09; do
    
    OffAxisNDCAFCombiner \
      -i "/pnfs/dune/persistent/users/marshalc/nd_offaxis/v7/CAF/0m/${j}/${i}*.root" \
      -o CAFv7_0m_${j}_${i}.root \
      -t caf \
      -p \
      -x -4000,400,25 \
      -f \
      -s 200 \
      -n 300  

    ## Special 280 kA run
  
    OffAxisNDCAFCombiner \
      -i "/pnfs/dune/persistent/users/marshalc/nd_offaxis/v7/CAF/0m/${j}/${i}*.root" \
      -o CAFv7_280kA_0m_${j}_${i}.root \
      -t caf \
      -p \
      -x -4000,400,25 \
      -f \
      -n 200 \
      --280kA
  
  done

  for p in 2m 4m 8m 12m 16m 20m 24m 28m; do

    for k in 00 01 02 03 04 05 06 07 08 09; do
 
      OffAxisNDCAFCombiner \
        -i "/pnfs/dune/persistent/users/marshalc/nd_offaxis/v7/CAF/${p}/${k}/${i}*.root" \
        -o CAFv7_${p}_${k}_${i}.root \
        -t caf \
        -p \
        -x -4000,400,25\
        -f \
        -n 100 
    
    done

  done

  wait

  # Combine them all
  OffAxisNDCAFCombiner \
    -i "./CAFv7_*_${i}.root" \
    -o CAFv7small_PRISM_Dec2020_${i}.root \
    -x -4000,400,25\
    -f \
    -C

done
