#!/bin/bash

file=$1
root -l -b -q 'countEvents.C("'${file}'")' >& nevt.txt
evts=$(tail -1 nevt.txt)

rm nevt.txt
echo ${evts}
