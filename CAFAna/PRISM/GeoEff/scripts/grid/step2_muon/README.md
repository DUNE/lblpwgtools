# Grid submissions on FNAL machines

>[!WARNING]
>This method may be subject to change depending on the current submission system.

All commands below are run from `lblpwgtools/CAFAna/PRISM/GeoEff`. Jobs run from a
prebuilt runtime bundle made by `scripts/grid/make_bundle.sh`; worker nodes do not
compile anything.

Put paths of Hadron efficiency files in `FDCAFs.txt` file:

```bash
ls -d "/pnfs/dune/persistent/path/to/inputs"/* | sed "s\/pnfs\root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr\g" > FDCAFs.txt
```

Where this command changes the path so that the worker node can access the files.

>[!WARNING]
>Remember change code for on/off axis

Create the runtime bundle:

```bash
bash scripts/grid/make_bundle.sh step2 FD_work.tar.gz FDCAFs.txt
```

The Python sources from `app/` (`*.py` and `assets/muonEff30.nn`) are included automatically. Worker jobs use the bundle directly and do not copy the repository.

To avoid installing Python dependencies in every job, create a reusable target directory and embed it in the bundle:

```bash
bash scripts/env/setup_python_env.sh /path/to/ndgeoeff-pyenv
export NDGEOEFF_PYTHON_ENV=/path/to/ndgeoeff-pyenv
bash scripts/grid/make_bundle.sh step2 FD_work.tar.gz FDCAFs.txt
```

`setup_python_env.sh` installs CPU-only Torch by default, which is all
`app/make_tree_fd.py` needs and keeps the bundle much smaller. Use a fresh
target directory when switching from a CUDA environment; old CUDA packages are
not removed automatically. If GPU support is required, point
`NDGEOEFF_TORCH_INDEX_URL` at a CUDA wheel index before installing, for
example:

```bash
NDGEOEFF_TORCH_INDEX_URL=https://download.pytorch.org/whl/cu124 \
  bash scripts/env/setup_python_env.sh /path/to/ndgeoeff-pyenv
```

If no preinstalled environment is provided, the job installs the CPU
dependencies in its own working directory as a fallback.

Submit jobs:

```bash
jobsub_submit -G dune -N 9635 --memory=5GB --disk=15GB --expected-lifetime=8h --cpu=1 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC,OFFSITE --tar_file_name=dropbox:///exp/dune/app/users/amunoz1/test/NDEff/submit2Grid/SubmitMuonEffJobs/FD_work.tar.gz --use-cvmfs-dropbox -l '+SingularityImage=\"/cvmfs/singularity.opensciencegrid.org/fermilab/fnal-wn-sl7:latest\"' --append_condor_requirements='(TARGET.HAS_Singularity==true&&TARGET.HAS_CVMFS_dune_opensciencegrid_org==true&&TARGET.HAS_CVMFS_larsoft_opensciencegrid_org==true&&TARGET.CVMFS_dune_opensciencegrid_org_REVISION>=1105&&TARGET.HAS_CVMFS_fifeuser1_opensciencegrid_org==true&&TARGET.HAS_CVMFS_fifeuser2_opensciencegrid_org==true&&TARGET.HAS_CVMFS_fifeuser3_opensciencegrid_org==true&&TARGET.HAS_CVMFS_fifeuser4_opensciencegrid_org==true)' file:///exp/dune/app/users/amunoz1/test/NDEff/submit2Grid/SubmitMuonEffJobs/run_FDcombEff.sh
```

>[!CAUTION]
>Modify paths in submission command!
>Modify output path in `run_FDcombEff.sh`!

This command submits 9635 jobs. To match the files in `FDCAFs.txt` use:

```bash
wc -l FDCAFs.txt
```

Eventually move output to more permanent location:

```bash
cp /pnfs/dune/scratch/users/path/to/outputs/* /pnfs/dune/persistent/users/path/to/outputs/
```
