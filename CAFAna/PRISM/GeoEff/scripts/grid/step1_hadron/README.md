# Grid submissions on FNAL machines

>[!WARNING]
>This method may be subject to change depending on the current submission system.

Build the runtime files once in the same `fnal-wn-sl7` Singularity image used for submission:

```bash
cmake -S . -B build -DNDGEOEFF_BUILD_STEP1=ON
cmake --build build --target runGeoEffFDEvtSim -j
```

Make `myFDntuples.txt` with the inputs to be processed using:

```bash
ls -d "/pnfs/dune/persistent/users/flynnguo/myFDntuples/myntuple_53993115"/* | sed "s\/pnfs\root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr\g" > myFDntuples.txt
```

Where this command changes the path so that the worker node can access the files.

>[!NOTE]
>This command takes the current set of ntuples, if this ever changes modify the command accordingly.

Make output directory if not done so already:

```bash
mkdir /pnfs/dune/scratch/users/path/to/output
```

Create the runtime bundle:

```bash
source scripts/grid/make_bundle.sh step1 NDEff.tar.gz myFDntuples.txt
```

The bundle contains `bin/runGeoEffFDEvtSim`, `lib/libgeoEff.so`, the setup scripts, and the input list. Worker jobs do not compile the code or copy the full repository.

Also to note:
 - in `run_NDEff_autogrid.sh` the output directory is `OUTDIR=/pnfs/dune/scratch/users/${GRID_USER}/FDGeoEffinND/NDFV400m` -> you might want to make your own named directory.
 - the setup script uses the runtime bundle directly and only loads the CVMFS environment.

Command to submit jobs:

```bash
jobsub_submit -G dune -N 9083 --memory=8GB --disk=1GB --expected-lifetime=3h --cpu=1 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC,OFFSITE --tar_file_name=dropbox:///exp/dune/app/users/amunoz1/test/NDEff/submit2Grid/SubmitHadronEffJobs/NDEff.tar.gz --use-cvmfs-dropbox -l '+SingularityImage=\"/cvmfs/singularity.opensciencegrid.org/fermilab/fnal-wn-sl7:latest\"' --append_condor_requirements='(TARGET.HAS_Singularity==true&&TARGET.HAS_CVMFS_dune_opensciencegrid_org==true&&TARGET.HAS_CVMFS_larsoft_opensciencegrid_org==true&&TARGET.CVMFS_dune_opensciencegrid_org_REVISION>=1105&&TARGET.HAS_CVMFS_fifeuser1_opensciencegrid_org==true&&TARGET.HAS_CVMFS_fifeuser2_opensciencegrid_org==true&&TARGET.HAS_CVMFS_fifeuser3_opensciencegrid_org==true&&TARGET.HAS_CVMFS_fifeuser4_opensciencegrid_org==true)' file:///exp/dune/app/users/amunoz1/test/NDEff/submit2Grid/SubmitHadronEffJobs/run_NDEff_autogrid.sh
```

>[!CAUTION]
>N is the number of input files, change accordingly. Check with `wc -l myFDntuples.txt`.

Eventually move output to more permanent location:

```bash
cp /pnfs/dune/scratch/users/path/to/output* /pnfs/dune/persistent/path/to/output/
```
