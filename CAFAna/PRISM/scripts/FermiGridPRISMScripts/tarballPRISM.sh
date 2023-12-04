#!/bin/bash
if [ -z "${CAFANA}" ]; then
  echo "[tarballPRISM ERROR]: CAFAna is not set up, cannot tar up required binaries."
  exit 1
fi

INPUT_STATE_FILE=${1}
if [ -z ${1} ]; then
  echo "[tarballPRISM WARNING]: State file not passed to tarball. This may slow down your jobs. (Ignore this message if you are producing a state file)"
  # but don't exit as users can still choose not to put the state file in tarball
fi

mkdir tar_state; cd tar_state
cp -r ${CAFANA} ./CAFAna

if [ ! -z "${EXTRA_SCRIPT_NAME}" ]; then
  cp ${EXTRA_SCRIPT_NAME} CAFAna/scripts/
  echo "[tarballPRISM INFO]: ls CAFAna/scripts/"
  ls CAFAna/scripts/
fi
if [ -e ../CAFECommands.cmd ]; then
  cp ../CAFECommands.cmd CAFAna/
fi

cp ../InputCAFs.*.list CAFAna/
cp ../InputStateFiles.list CAFAna/
cp ${INPUT_STATE_FILE} CAFAna/

tar -zcvf CAFAna.Blob.tar.gz CAFAna/*

cd ..

mv tar_state/CAFAna.Blob.tar.gz .
rm -r tar_state
