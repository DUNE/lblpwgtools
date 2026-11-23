# Grid submissions on FNAL machines
 
>[!WARNING]
>This method may be subject to change depending on the current submission system.

Put paths of Hadron efficiency files in `FDCAFs.txt` file:

```bash
ls -d "/pnfs/dune/persistent/path/to/inputs"/* | sed "s\/pnfs\root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr\g" > FDCAFs.txt
```

Where this command changes the path so that the worker node can access the files. 

>[!WARNING] 
>Remember change code for on/off axis

Make tarball:

```bash
tar -czvf FD_work.tar.gz setup_FDcombEff.sh app FDCAFs.txt
```

>[!NOTE]
>Copy the flat `app/` directory from the cloned repository into the tarball working directory.

Submit jobs:
```bash
jobsub_submit -G dune -N 9635 --memory=5GB --disk=15GB --expected-lifetime=8h --cpu=1 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC,OFFSITE --tar_file_name=dropbox:///exp/dune/app/users/amunoz1/test/NDEff/submit2Grid/SubmitMuonEffJobs/FD_work.tar.gz --use-cvmfs-dropbox -l '+SingularityImage=\"/cvmfs/singularity.opensciencegrid.org/fermilab/fnal-wn-sl7:latest\"' --append_condor_requirements='(TARGET.HAS_Singularity==true&&TARGET.HAS_CVMFS_dune_opensciencegrid_org==true&&TARGET.HAS_CVMFS_larsoft_opensciencegrid_org==true&&TARGET.CVMFS_dune_opensciencegrid_org_REVISION>=1105&&TARGET.HAS_CVMFS_fifeuser1_opensciencegrid_org==true&&TARGET.HAS_CVMFS_fifeuser2_opensciencegrid_org==true&&TARGET.HAS_CVMFS_fifeuser3_opensciencegrid_org==true&&TARGET.HAS_CVMFS_fifeuser4_opensciencegrid_org==true)' file:///exp/dune/app/users/amunoz1/test/NDEff/submit2Grid/SubmitMuonEffJobs/run_FDcombEff.sh
```

>[!CAUTION] 
>Modify paths in submission command!
>Modify output path in `run_FDcomEff.sh`!

This is command submits 9635, to match the files in `FDCAFs.txt` use:

```bash
wc -l FDCAFs.txt
```

Eventually move output to more permanent location:
```bash 
cp /pnfs/dune/scratch/users/path/to/outputs/* /pnfs/dune/persistent/users/path/to/outputs/
```
