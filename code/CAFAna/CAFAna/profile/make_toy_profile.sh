#!/bin/bash
export CPUPROFILE=${1}
time make_toy_throws ${2} ${3} ${4} fluxxsecdet
rm ${4}
