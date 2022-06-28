#!/bin/bash
if [ -z "${CAFANA}" ]; then
  echo "[ERROR]: CAFAna is not set up, cannot tar up required binaries."
  exit 1
fi

mkdir tar_state; cd tar_state
cp -r ${CAFANA} ./CAFAna

if [ ! -z "${EXTRA_SCRIPT_NAME}" ]; then
  cp ${EXTRA_SCRIPT_NAME} CAFAna/scripts/
  echo "[INFO]: ls CAFAna/scripts/"
  ls CAFAna/scripts/
fi
if [ -e ../CAFECommands.cmd ]; then
  cp ../CAFECommands.cmd CAFAna/
fi

cp ../InputCAFs.*.list CAFAna/


tar -zcvf CAFAna.Blob.tar.gz CAFAna/*

cd ..

mv tar_state/CAFAna.Blob.tar.gz .
rm -r tar_state
