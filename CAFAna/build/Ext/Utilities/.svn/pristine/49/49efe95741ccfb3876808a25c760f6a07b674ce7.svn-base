#!/bin/bash

# This bash script will run the blessed plots page generation. Intended to be called
# from a cronjob.


# Configuration
RELEASE="development"
RELEASE_DIR=~/

# Setup nova
echo Setting up test release
source /grid/fermiapp/nova/novaart/novasvn/setup/setup_nova.sh -r ${RELEASE}
pushd ${RELEASE_DIR}/Utilities/BlessedPlots
srt_setup -a

# Utilities/BlessedPlots is not recursed into when make is called from
# higher levels.  Make sure that it gets built before trying to run it.
make

# Get the latest DocDB json file
echo Updating BlessedPlots.json
svn update BlessedPlots.json
svn update BlessedPlotsProposed.json

# Run the blessed plot maker
echo Running Blessed Plots generation
python RunBlessedPlots.py

# Generate static html
echo Generating html
python make_static_page.py

popd

echo DONE
