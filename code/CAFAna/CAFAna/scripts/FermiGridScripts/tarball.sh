#!/bin/bash

if [ -z "${CAFANA}" ]; then
  echo "[ERROR]: (NOvA-less)CAFAna is not set up, cannot tar up required binaries."
  exit 1
fi

EXTRA_SCRIPT_NAME=""
if [ ! -z "${1}" ] && [ -e "${1}" ]; then
  EXTRA_SCRIPT_NAME=$(readlink -f ${1})
fi

mkdir tar_state; cd tar_state
cp -r ${CAFANA} ./CAFAna
if [ ! -z "${EXTRA_SCRIPT_NAME}" ]; then
  cp ${EXTRA_SCRIPT_NAME} CAFAna/scripts/
fi
if [ -e ../CAFECommands.cmd ]; then
  cp ../CAFECommands.cmd CAFAna/
fi
tar -zcvf CAFAna.Blob.tar.gz CAFAna/*
cd ..
mv tar_state/CAFAna.Blob.tar.gz .
rm -r tar_state
