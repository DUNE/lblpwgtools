##########################################################################
# This script is for submitting a SAM project via jobsub to FNAL resources.
# You need to have sourced the release/build that you intend to submit with
# and made a dataset to run over.

############################################
# USERS EDIT THE BELOW PARAMETERS          #
############################################
# Setup the Job specific parameters
JOBNAME='tute-RecoFDCRY.sh'
DATASET='tute-prodartdaq_S14-02-05CryFD_r1000001'
NUMFILES=1
NUMJOBS=1 # if <= 0, it submits all of the jobs in the project 
OUTPUT_DIR="/pnfs/nova/scratch/users/${USER}"
JOBFCL='Utilities/batch/recocosmicsproductionjob.fcl'

ROLE="" # Which role to use, options are blank (regular use), Production (production user)

DATA_TIER="reco" # Options are: reco, artcaf, artdaq, pclist, pcliststop, pid

WHICH_CVMFS="/cvmfs/nova.opensciencegrid.org"

#############################################
# USER INTERVENTION ENDS HERE, IN PRINCIPLE #
#############################################

# Setup the SAM project here
if [[ "x$PROJECTNAME" == "x" ]]; then
    DATE="`date +%Y%m%d_%H%M`"
    PROJECTNAME="${USER}-${JOBNAME}-${DATE}"
    ifdh startProject "${PROJECTNAME}" $SAM_STATION "$DATASET" $USER $SAM_STATION
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

##########################################
# Configure Grid Proxy
if [[ "x$ROLE" == "x" ]]; then
  X509_USER_PROXY=/scratch/$USER/grid/$USER.$EXPERIMENT.proxy
else
  X509_USER_PROXY=/scratch/$USER/grid/$USER.$EXPERIMENT.$ROLE.proxy
fi

#########################################
# Give log output filenames better name based on PROJECTNAME
ln -s $SCRIPT /nova/app/condor-exec/$USER/${PROJECTNAME}.sh
JOBSCRIPT=/nova/app/condor-exec/$USER/${PROJECTNAME}.sh

##########################################
# Print out some useful information
echo "NUMJOBS IS $NUMJOBS with $NUMFILES per job"
echo "DATASET: " $DATASET
echo "Project name is: $SAM_PROJECT_NAME"
echo "Station Monitor: http://samweb.fnal.gov:8480/station_monitor/nova/stations/nova/projects/$SAM_PROJECT_NAME"
echo "$SAM_PROJECT_NAME" >> $0.projectname.log

#export SITES="UCSD"
##########################################
# The Magic Jobsub command
jobsub \
    -N ${NUMJOBS} \
    --site ${SITES} \
    --OS=SL5,SL6 \
    --opportunistic \
    --nowrapfile \
    --X509_USER_PROXY $X509_USER_PROXY \
    -e SAM_PROJECT_NAME \
    -e SAM_STATION \
    -e IFDH_BASE_URI \
    -e IFDH_DEBUG \
    -e EXPERIMENT \
    -g \
    ${JOBSCRIPT} \
      --multifile \
      --export CVMFS_DISTRO_BASE=$WHICH_CVMFS \
      --config $JOBFCL \
      --source '${CVMFS_DISTRO_BASE}/novasvn/setup/setup_nova.sh:-r:'"${NOVA_RELEASE}:-b:${BUILD}:-e:${WHICH_CVMFS}/externals:-5:${WHICH_CVMFS}/novasvn:-6:${WHICH_CVMFS}/novasvnslf6" \
      --limit ${NUMFILES} \
      --export DEST=${OUTPUT_DIR} \
	-X ${EXECUTABLE} \
	--outTier=out1:${DATA_TIER} \
        --copyOut \
        #--cafTier=cafmaker:${DATA_TIER} \ 
#Uncomment above line if I'm a CAF job

          #--histTier histlabel \
# Uncomment above line if you want histogram files to be copied back. histlabel
# will appear before .hist.root in your hist file names
