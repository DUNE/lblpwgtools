#!/bin/bash

if [ "$0" == "$BASH_SOURCE" ]
then
    echo 'Please source this script (it needs to modify your environment)'
    exit 1
fi

source srt/srt.sh

source setup/setup_nova.sh -b maxopt -6 $SRT_DIST

cd releases/development/CAFAna/
