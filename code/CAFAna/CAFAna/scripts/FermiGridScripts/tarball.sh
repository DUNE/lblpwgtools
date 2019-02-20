#!/bin/bash

if [ -z "${CAFANA}" ]; then
  echo "[ERROR]: (NOvA-less)CAFAna is not set up, cannot tar up required binaries."
  exit 1
fi

mkdir tar_state; cd tar_state
cp -r ${CAFANA} ./CAFAna
tar -zcvf CAFAna.Blob.tar.gz CAFAna/*
cd ..
mv tar_state/CAFAna.Blob.tar.gz .
rm -r tar_state
