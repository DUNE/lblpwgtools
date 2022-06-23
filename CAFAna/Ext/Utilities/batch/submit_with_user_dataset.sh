
#####################################################################
# Setup the Job specific parameters
#####################################################################

DATASET="fd_sag_birk_study_base_nonswap_with_lem"

# if <= 0, it submits all of the jobs in the project
NUMJOBS=1 
NUMFILES=1

OUTPUT_DIR="/nova/ana/users/ksachdev"
JOBFCL='/nova/app/users/ksachdev/sag-sim/scripts/lidbuilderjob.fcl'

# we don't currently put development builds on cvmfs. Set the 
# following to false if you want to use development. Otherwise
# let it be true.
USE_CVMFS=true 

#####################################################################
# you really have no business touching anything below this line. It
# should all work with the parameters provided above.
#####################################################################

# the script that copies the output files to OUTPUT_DIR 
COPY_OUT_SCRIPT='$SRT_PUBLIC_CONTEXT/Utilities/batch/copy_out.sh'

# check that the output dir is group writable
permission=`stat -c "%a" ${OUTPUT_DIR} |  grep ".[67]."`
if [ -z ${permission} ]; then
    echo "Output directory is not group writable."
    exit 1
fi
    
# where are we setting up the software from?
SETUP_NOVA="'/grid/fermiapp/nova/novaart/novasvn/setup/setup_nova.sh:-r:''$SRT_BASE_RELEASE:-b:$SRT_QUAL'"

if [ ${USE_CVMFS} = true ]; then 
    CVMFS_DISTRO_BASE='/cvmfs/nova.opensciencegrid.org'
    SETUP_NOVA="${CVMFS_DISTRO_BASE}"'/novasvn/setup/setup_nova.sh:-r:'"${SRT_BASE_RELEASE}:-b:${SRT_QUAL}:-e:${CVMFS_DISTRO_BASE}/externals:-5:${CVMFS_DISTRO_BASE}/novasvn:-6:${CVMFS_DISTRO_BASE}/novasvnslf6"
fi

echo ${SETUP_NOVA}

ROLE="Analysis"
JOBNAME=$DATASET

# Setup the SAM project here
DATE="`date +%Y%m%d_%H%M`"
PROJECTNAME="${USER}-${JOBNAME}-${DATE}"
ifdh startProject "${PROJECTNAME}" $SAM_STATION "$DATASET" $USER $SAM_STATION

if [ $? -eq 0 ]; then
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


echo "NUMJOBS IS $NUMJOBS"
echo "DATASET" $DATASET

ln -s $NOVAGRIDUTILS_DIR/bin/art_sam_wrap.sh /nova/app/condor-exec/$USER/${PROJECTNAME}.sh
JOBSCRIPT=/nova/app/condor-exec/$USER/${PROJECTNAME}.sh

echo "Project name is: $SAM_PROJECT_NAME"
echo "Station Monitor: http://samweb.fnal.gov:8480/station_monitor/nova/stations/nova/projects/$SAM_PROJECT_NAME"
echo "$SAM_PROJECT_NAME" >> $0.projectname.log

export GRID_USER=$USER

#####################################################################
# ready to submit job 
#####################################################################

jobsub_submit \
    -N ${NUMJOBS} \
    --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC \
    -G nova \
    -e SAM_PROJECT_NAME    -e SAM_STATION    -e IFDH_BASE_URI    -e IFDH_DEBUG    -e EXPERIMENT    -e GRID_USER \
    --role=${ROLE} \
    file://${JOBSCRIPT} \
      --limit  ${NUMFILES}\
      --multifile \
      --export DEST=${OUTPUT_DIR} \
      --export CVMFS_DISTRO_BASE=${CVMFS_DISTRO_BASE} \
      --config ${JOBFCL} \
      --source ${SETUP_NOVA} \
      --postscript ${COPY_OUT_SCRIPT} \
      -X nova \
