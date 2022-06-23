#! /usr/bin/env bash

# This a template script for submitting jobs
# to grid using a Cmake build and SAM files.
#
# Original script in /Utilities/batch/SAMSubmitTemplate.sh
# Current script in  /Utilities/batch/SAMSubmitTemplate_cmake.sh
#
# Questions, comments, concerns?
# Bother: Keith Matera (kmatera@fnal.gov)
#         Brian Rebel  (brebel@fnal.gov)
# Last edited Dec 4, 2014

# ******This script assumes that you have built novasoft with the prof qualifier************
# ******That assumption is made because the prof build runs faster than the     ************
# ******debug build and there is no reason to have debugging symbols on for grid************
# ******running                                                                 ************

############################################
# Users EDIT THE BELOW PARAMETERS          #
############################################
# Setup the Job specific parameters
GROUP=nova                                                     # nova
USER=you                                                       # your username
NOVAENV_VER='v01_06_03'                                        # default version of novaenv to use if not already setup
JOBNAME='DescriptiveName'                                      # a name for your job
QUALS='e9:s28:grid:prof'                                           # novasoft qualifiers
DATASET='datasetyouwanttouse'                                  # sam dataset containing files you want to analyze
OUTPUT_DIR='/nova/ana/users/${USER}/'                          # specify location for output files, should be /nova/ana/
NUMFILES=10                                                    # number of files for each job to view
NUMJOBS=100                   				                         # if <= 0, it submits all of the jobs in the project
JOBFCL='yourfavorite.fcl'   				                           #Check your FHICL_FILE_PATH if you're having trouble
ROLE="Analysis"                          		                   # Which role to use, options are Analysis (regular use), 
                                                               # Production (production user)     
TFILEOUT_NAME="nameforhistogramoutput"                         # name of the TFileService output, without .root extension
LOCAL_PRODS=/nova/app/users/${USER}                            # full path to location of where your novasoft libraries are kept

#############################################
# USER INTERVENTION ENDS HERE, IN PRINCIPLE #
#############################################

############################################
# Setup IFDH/grid-friendly environment     #
############################################
source /grid/fermiapp/products/nova/externals/setup
export PRODUCTS=/grid/fermiapp/products/common/db:/nova/data/pidlibs/products:${LOCAL_PRODS}:${PRODUCTS}

# check that novaenv was set up to get the necessary SAM and IFDH environmental variables
# novaenv sets EXPERIMENT, SCRIPT, SITES, IFDH* and SAM* variables
if [ -z "${NOVAENV_VERSION}" ]; then
    setup novaenv ${NOVAENV_VER} -q grid
fi

############################################
# Setup the SAM project here               #
############################################
if [[ "x$PROJECTNAME" == "x" ]]; then
    DATE="`date +%Y%m%d_%H%M`"
    PROJECTNAME="${USER}-${JOBNAME}-${DATE}"
    ifdh startProject "${PROJECTNAME}" "${SAM_STATION}" "${DATASET}" "${USER}" "${SAM_STATION}"
    if [ $? -eq 0 ]; then
	export SAM_PROJECT_NAME="${PROJECTNAME}"
    fi
else
  export SAM_PROJECT_NAME="${PROJECTNAME}"
fi

if [ ${NUMJOBS} -le 0 ]; then
  export NUMPROJECTFILES=`samweb -e nova count-definition-files ${DATASET}`
  if [ ${NUMFILES} -eq 1 ]; then
    export NUMJOBS=${NUMPROJECTFILES}
  else
    export NUMJOBS=`echo "(${NUMPROJECTFILES}/ ${NUMFILES}) + 1" | bc`
  fi
fi

############################################
# Give log output names a sensible name    #
# based on the PROJECTNAME                 #
############################################
#SCRIPT is set to the $IFDH_ART_FQ_DIR/bin version of art_sam_wrap.sh by novaenv
ln -s ${SCRIPT} /nova/app/condor-exec/${USER}/${PROJECTNAME}.sh
JOBSCRIPT=/nova/app/condor-exec/${USER}/${PROJECTNAME}.sh
echo JOBSCRIPT is ${JOBSCRIPT} 

############################################
# Print out some useful information        #
############################################
echo "NUMJOBS is ${NUMJOBS} with ${NUMFILES} per job"
echo "DATASET: " ${DATASET}
echo "Project name is: ${SAM_PROJECT_NAME}"
echo "Station Monitor: http://samweb.fnal.gov:8480/station_monitor/nova/stations/nova/projects/${SAM_PROJECT_NAME}"
echo "${SAM_PROJECT_NAME}" >> $0.projectname.log

############################################
# Make source file to setup software       #
############################################
SETUP_NOVA=/nova/app/condor-exec/${USER}/'setup_nova_ups.sh'
echo '#! /usr/bin/env bash' > ${SETUP_NOVA}
echo ' ' >> ${SETUP_NOVA}
echo 'main(){' >> ${SETUP_NOVA}
echo '    source /grid/fermiapp/products/nova/externals/setup' >> ${SETUP_NOVA}
echo '    export PRODUCTS=/grid/fermiapp/products/common/db:/nova/data/pidlibs/products:'${LOCAL_PRODS}':${PRODUCTS}' >> ${SETUP_NOVA}
echo '    setup novasoft devel -q '${QUALS} >> ${SETUP_NOVA}
echo '}' >> ${SETUP_NOVA}
echo 'main $*' >> ${SETUP_NOVA}
chmod ug+x ${SETUP_NOVA}

############################################
# Make copyOut.sh to copy completed files  #
############################################
COPYOUT=/nova/app/condor-exec/${USER}/'copyOut'${JOBNAME}'.sh'
echo '#! /usr/bin/env bash' > ${COPYOUT}
echo ' ' >> ${COPYOUT}
echo 'main(){' >> ${COPYOUT}
echo '    hadd '${TFILEOUT_NAME}'_${PROCESS}.root '${TFILEOUT_NAME}'*.root' >> ${COPYOUT}
echo '    ifdh cp '${TFILEOUT_NAME}'_${PROCESS}.root ${DEST}/'${TFILEOUT_NAME}'_${PROCESS}.root' >> ${COPYOUT}
echo '    rm '${TFILEOUT_NAME}'*.root' >> ${COPYOUT}
echo '}' >> ${COPYOUT}
echo 'main $*' >> ${COPYOUT}
chmod ug+x ${COPYOUT}

############################################
# The Magic Jobsub command                 #
############################################

# file is the script for jobsub_submit to execute
# in this case it is art_sam_wrap.sh
# use the art dynamic naming for the output histogram file, lowest run and subrun in list is used

jobsub_submit \
    -N ${NUMJOBS} \
    --OS=SL6 \
    -G nova \
    -e SAM_PROJECT_NAME \
    -e SAM_STATION \
    -e IFDH_BASE_URI \
    -e IFDH_DEBUG \
    -e EXPERIMENT \
    --role=${ROLE} \
    file://${JOBSCRIPT} \
      --multifile \
      --config ${JOBFCL} \
      --source ${SETUP_NOVA} \
      --limit ${NUMFILES} \
      --export DEST=${OUTPUT_DIR} \
      --postscript ${COPYOUT} \
      -X nova -T ${TFILEOUT_NAME}_%r_%s.root
