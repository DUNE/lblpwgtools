#!/bin/bash

# Path to custom UPS products
export MY_UPS=/exp/dune/app/users/fmlopez/ndgar_tests/ups
# Path to CAFAna
export CAFANA=/exp/dune/app/users/fmlopez/ndgar_tests/lblpwgtools/CAFAna/build/Linux
# Path to macro directory
export GArAnaScripts=/exp/dune/app/users/fmlopez/ndgar_tests/lblpwgtools/CAFAna/GArAnaScripts
# Path to grid setup script
export SetupScript=/exp/dune/app/users/fmlopez/ndgar_tests/lblpwgtools/CAFAna/GArAnaGrid/setup_grid.sh

if [ -z "${CAFANA}" ]; then
  echo "[ERROR]: CAFAna is not set up, cannot tar up required binaries."
  exit 1
fi

mkdir tar_state
cp -r ${MY_UPS} ./tar_state/ups
cp -r ${CAFANA} ./tar_state/CAFAna
cp -r ${GArAnaScripts} ./tar_state/GArAnaScripts
cp ${SetupScript} ./tar_state

# Copy also any additinal files
for var in "$@"
do
    path=$(realpath "$var")
    cp ${path} ./tar_state
done

cd tar_state

tar -zcf CAFAna.tar.gz ./*

cd ..

mv tar_state/CAFAna.tar.gz .

rm -r tar_state
