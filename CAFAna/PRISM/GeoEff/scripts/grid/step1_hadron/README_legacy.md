> Paths in this document refer to the legacy `app/` layout. In the current layout the step 1 sources are under `app/`, headers under `include/`, macros under `macros/step1_hadron/`, and the CMake build outputs executables in `build/bin/`.

# 🟢 How to Submit Jobs for the Geometric Efficiency
This guide explains how to submit **Hadron Geometric Efficiency** jobs on the Fermi computing grid.

## 1. Hadron Geometric Efficiency

### 1.0 Create a working directory 
Create a directory for your jobs (example: `SubmitHadronEffJobs`) and enter it:

```bash
mkdir SubmitHadronffJobs
cd SubmitHadronEffJobs
```

### 1.1 First get the work env setup
you find it either at ```/exp/dune/app/users/icaracas/SubmitNDEffJobsOnGrid/``` or on git at: https://github.com/icaracas/DUNE_ND_GeoEff/tree/Documentation/SubmitHadronGeoEff

you need: ```setupNDEff-grid.sh``` and ```run_NDEff_autogrid.sh```

Things you might want to change:
 - in ```run_NDEff_autogrid.sh``` the output directory is, ```OUTDIR=/pnfs/dune/scratch/users/${GRID_USER}/FDGeoEffinND/NDFV400m``` -> you might want to make your own named directory etc
 - in ```setupNDEff-grid.sh``` -- the curretnly working and cloned git repository is my git repo: https://github.com/icaracas/DUNE_ND_GeoEff.git

   	- for the future you might want to clone everything from my repo and make your own git repository where you would make any modifications, so the replace this repo with your own repository

### 1.2 make txt input file with the FD ntuples
 - so far we use the ntuples made by Flynn and saved at ```/pnfs/dune/persistent/users/flynnguo/myFDntuples/myntuple_53993115```
 - if more ntuples in the future set the path correspondingly

```bash
ls -d "/pnfs/dune/persistent/users/flynnguo/myFDntuples/myntuple_53993115"/* | sed "s\/pnfs\root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr\g" > myFDntuples.txt
```
 - this command also changes pnfs to xrootd so that worker node can access

### 1.3 Now make the tarball
```bash
tar -czvf NDEff.tar.gz setupNDEff-grid.sh myFDntuples.txt
```

### 1.4 Submit jobs -- this submits N jobs ((N = number of input files, in this case ntuples, so each job runs 1 file)
```bash
jobsub_submit -G dune -N 9083 --memory=8GB --disk=1GB --expected-lifetime=3h --cpu=1 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC,OFFSITE --tar_file_name=dropbox:///exp/dune/app/users/icaracas/SubmitNDEffJobsOnGrid/NDEff.tar.gz --use-cvmfs-dropbox -l '+SingularityImage=\"/cvmfs/singularity.opensciencegrid.org/fermilab/fnal-wn-sl7:latest\"' --append_condor_requirements='(TARGET.HAS_Singularity==true&&TARGET.HAS_CVMFS_dune_opensciencegrid_org==true&&TARGET.HAS_CVMFS_larsoft_opensciencegrid_org==true&&TARGET.CVMFS_dune_opensciencegrid_org_REVISION>=1105&&TARGET.HAS_CVMFS_fifeuser1_opensciencegrid_org==true&&TARGET.HAS_CVMFS_fifeuser2_opensciencegrid_org==true&&TARGET.HAS_CVMFS_fifeuser3_opensciencegrid_org==true&&TARGET.HAS_CVMFS_fifeuser4_opensciencegrid_org==true)' file:///exp/dune/app/users/icaracas/SubmitNDEffJobsOnGrid/run_NDEff_autogrid.sh
```

-- replace ```/exp/dune/app/users/icaracas/SubmitNDEffJobsOnGrid``` to your working directory ```SubmitHadronffJobs``` (i.e same folder where you saved myFDntuples.txt and created the tarball, from 1.0)
 
 ### REMEMBER: 
 files on ```/pnfs/dune/scratch``` are only saved temporarily. Once your jobs finished copy all of your files from ```/scratch``` somewhere on ```/pnfs/dune/persistent/users/```
