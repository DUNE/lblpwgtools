#! /usr/bin/env bash

# This is a template script for submitting jobs to
# grid using a Cmake build and local data files.
#
# Questions, comments, concerns?
# Bother: Keith Matera (kmatera@fnal.gov)
#
# Last edited Sep 29, 2014
#

#-------------------------------
# USER INTERVENTION STARTS HERE
export GROUP=nova
export USER=kmatera
export MY_JOB_FILE=mipanajob.fcl
export DATA_FILE_DIR=/nova/ana/users/xbbu/NearDet/FULL/Data/Reco/newSlice20140915/10407
export OUTPUT_DIR=/nova/ana/users/kmatera/NEARDET_STUDIES/Data/Results/newSlice20140915_newVars_sep28/10407
export DISPLAY=localhost0.0
# USER INTERVENTION STOPS HERE (in theory)
# Note: if calibration files have changed, you will
#       want to udpate the calib files sent to 
#       jobcmd, below
#-------------------------------

# Should not have to change this...
MY_TEST_REL=${CETPKG_BUILD}/..

#Ensure that files are user and group writable by default
umask 002

# Setup some useful variables.
export CPN=/grid/fermiapp/common/tools/cpn
export MVN=/grid/fermiapp/common/tools/mvn

# Some output for debugging
#echo "GROUP=             $GROUP"
#echo "USER=              $USER"
#echo "CPN=               $CPN"
#echo "MVN=               $MVN"
#echo "Data files dir:    $DATA_FILE_DIR"
#echo "Output dir:        $OUTPUT_DIR"
#echo "Test release:      $MY_TEST_REL"

# Make sure that the output directory exists
mkdir -p $OUTPUT_DIR

# Set up CONDOR variables
source /grid/fermiapp/products/nova/externals/setup
export PRODUCTS=/grid/fermiapp/products/common/db:/nova/data/pidlibs/products:${MY_TEST_REL}/localProducts_nova_devel_e6_debug/:${PRODUCTS}
setup jobsub_tools

# Move into $CONDOR_EXEC, setup SCRATCH dir
#echo ""
#echo "Moving into dir      $CONDOR_EXEC"
cd $CONDOR_EXEC

# Loop over input data files
echo "Beginning loop over input files of form 'nd*.root' in $DATA_FILE_DIR"
for file in $( find $DATA_FILE_DIR -maxdepth 1  -name 'nd*.root' |sort -r )  
do {
	
      #extract file name by cutting out path
      filename=`basename $file`
      shortname=${filename:10}
      outhist=mipanatool$shortname
      jobcmd=$CONDOR_EXEC/mipanatool$shortname.sh
      echo "    filename: $filename" 
      echo "        w/ shortname: $shortname"
      echo "        w/ outhist:   $outhist"
      echo "        w/ jobcmd:    $jobcmd"
      
      #remove jobcmd if it exists already
      if [ -e $jobcmd ];then
	  rm -f $jobcmd
      fi

      #construct the jobcmd file
      echo "#!/bin/bash" >> $jobcmd
      echo "umask 002"   >> $jobcmd
      echo "export GROUP="$GROUP >> $jobcmd
      echo "export USER="$USER >> $jobcmd     
      echo "source /grid/fermiapp/products/nova/externals/setup" >> $jobcmd
      echo "export PRODUCTS=/grid/fermiapp/products/common/db:/nova/data/pidlibs/products:"$MY_TEST_REL"/localProducts_nova_devel_e6_debug:${PRODUCTS}" >> $jobcmd
      echo "setup jobsub_tools" >> $jobcmd
      echo "setup novasoft devel -qe6:debug:grid" >> $jobcmd
      echo "export DISPLAY=localhost0.0" >> $jobcmd
	
      # this makes sure we have the appropriate commands to cp and mv on the grid
      echo "export CPN=/grid/fermiapp/common/tools/cpn" >> $jobcmd
      echo "export MVN=/grid/fermiapp/common/tools/mvn" >> $jobcmd

      # makes the temporary scratch directories for grid work
      echo 'mkdir ${_CONDOR_SCRATCH_DIR}/input' >> $jobcmd
      echo 'mkdir ${_CONDOR_SCRATCH_DIR}/output' >> $jobcmd
      
      # copies necessary file for running over
      echo '${CPN} '"$file"' ${_CONDOR_SCRATCH_DIR}/input' >> $jobcmd
      echo '${CPN} '"/nova/ana/calibration/csvs/v4/calib_atten_consts.nd.mc.v4.csv"' ${_CONDOR_SCRATCH_DIR}/input' >> $jobcmd
      echo '${CPN} '"/nova/ana/calibration/csvs/v4/calib_atten_points.nd.mc.v4.csv"' ${_CONDOR_SCRATCH_DIR}/input' >> $jobcmd
      echo '${CPN} '"/nova/ana/calibration/csvs/v4/calib_abs_consts.nd.mc.v4.csv"' ${_CONDOR_SCRATCH_DIR}/input' >> $jobcmd
      
      # change directory to the input area on scratch
      echo 'cd ${_CONDOR_SCRATCH_DIR}/input' >> $jobcmd
      echo "nova -c $MY_JOB_FILE -s $filename -T $outhist" >> $jobcmd

      # moving and finally copying output files to your /nova/data area
      echo '${MVN} ${_CONDOR_SCRATCH_DIR}/input/'"$outhist"' ${_CONDOR_SCRATCH_DIR}/output/'"$outhist" >> $jobcmd
      echo '${CPN} ${_CONDOR_SCRATCH_DIR}/output/'"$outhist $OUTPUT_DIR"'/' >> $jobcmd
      echo "chmod a+rwx $OUTPUT_DIR"'/*' >> $jobcmd

      # more courtesy removals
      echo "rm -f "'${_CONDOR_SCRATCH_DIR}/input/*' >> $jobcmd      

      chmod a+rwx $jobcmd

      # -M, --mail+always (Send an email for any job completion (default is error only))
      # -T, --test_queue  (Submit as test job---highest priority, only one such job at a time)
      jobsub -q -g --opportunistic $jobcmd
      
} done
cd -
