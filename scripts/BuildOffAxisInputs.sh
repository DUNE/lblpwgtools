#!/bin/bash

if ! CAFBuild OffAxisNDCAFCombiner.C; then
  exit 1
fi

for i in FHC; do # RHC; do

  # for j in PROD2 PROD3 PROD4 PROD5 PROD6 PROD7 PROD8 PROD9 PROD10 PROD11; do
  # for j in PROD11; do
  #   ./OffAxisNDCAFCombiner.exe \
  #        "/pnfs/dune/persistent/users/gyang/CAF/CAF/${j}/CAF_${i}_[0-9]{5}.root" \
  #        /dune/data/users/picker24/OffAxisCAFs/CAF_${i}_OffAxis_${j}.root \
  #        false \
  #        caf true
  # done

  # ./OffAxisNDCAFCombiner.exe \
  #      /dune/data/users/marshalc/CAFs/mcc11_v3/ND_${i}_CAF.root \
  #      /dune/data/users/picker24/OffAxisCAFs/CAF_${i}_OnAxis.root \
  #      false \
  #      caf true false 5000000

  ./OffAxisNDCAFCombiner.exe \
       "/dune/data/users/picker24/OffAxisCAFs/CAF_${i}_OffAxis_PROD[4-9].root,/dune/data/users/picker24/OffAxisCAFs/CAF_${i}_OffAxis_PROD10.root,/dune/data/users/picker24/OffAxisCAFs/CAF_${i}_OnAxis.root" \
       /dune/data/users/picker24/OffAxisCAFs/CAF_${i}_PRISM_PROD4-10.root \
       true


done
