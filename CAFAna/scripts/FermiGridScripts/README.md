# FermiGrid Scripts

Some starter scripts for submitting cafe jobs to FermiGrid. User scripts are
named `Farm<something>.sh`, the main one being `FarmCAFENodeScript.sh`. There
is a script for building the state files on the grid, `FarmBuildInterps.sh`,
but it is almost certainly faster and more efficient to build them
interactively, as such, this script won't be documented in detail here. Both
scripts will respond to -? with hopefully helpful help text describing the
various arguments.

## Generic CAFAna environment setup

These scripts are designed to work with the NOvA-less/cmake build of the DUNE
version of CAFAna. You will need to build against Scisoft UPS products
distributed via UPS. A helper script for building can be found in the CAFAna
root directory: `standalone_configure_and_build.sh`. Configure and build like:

```
./standalone_configure_and_build.sh \
  --use-UPS \
  --release \
  --n-cores 4
```

This should produce a `build` directory, containing a `Linux` directory,
containing a script named `CAFAnaEnv.sh`. Source this script to set up the
`CAFAna` environment. You're now ready to run CAFAna analyses. Check that the
`CAFANA` environment variable is set correctly with `echo $CAFANA`, and that
the CAFExecutor is available with `which cafe`.

## State file location

This documentation will not go through (yet) the production of 'state' files.
These are root files that contain serialized CAFAna `PredictionInterp` objects.
However, it will say that for running on the grid, your statefiles should be
somewhere accessible via `xrootd`, **i.e.**: `/pnfs/dune/resilient/${USER}/somewhere`. We will use resilient as it is configured optimally for simultaneous i/o (**i.e.** hundreds of jobs starting at once). This is not the ideal way to distribute statefiles, stashcache/cvmfs is, but given that they are being modified fairly regularly at the moment, this will do. To stream a file on `/pnfs/dune/XXXXX/path/to/file.root` with `xrootd`, you should use:

```
  TFile *fin = TFile::Open("root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr/dune/XXXXX/path/to/file.root");
```

*N.B.* The standard scripts need groups of state files for the different detector samples, these should be passed to your analysis scripts as a state file location stub, like: `/path/to/statefile<_SAMPLE.root>`, where
`<_SAMPLE.root>` is omitted. To be clear, a full example would be:
```
  root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr/dune/resilient/MYUSERNAME/CAFAnaStateFiles/DefaultState_vX
```
which might be expanded to:
```
  root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr/dune/resilient/MYUSERNAME/CAFAnaStateFiles/DefaultState_vX_ND_FHC.root
```
by the analysis script helpers.

*N.B.* If the files do end up on stashcache, then
```
  /cvmfs/dune.osgstorage.org/some/location/DefaultState_vX
```
would be a perfectly valid state file stub.

## `FarmCAFENodeScript.sh`

This is the main user submission helper script. It checks the CAFAna and grid
authentication environments, tars up a built version of CAFAna, checks that
the cafe scripts for requested jobs will exist on the node and then submits
job clusters for execution. The latest help text is copied here:

```
[RUNLIKE]  [opts] --cafe-comands <args>
        -p|--pnfs-path-append      : Path to append to output path: /pnfs/dune/scratch/users/${USER}/
        -c|--cafe-command-file     : File containing <script name>: [arg1 [arg2 [...]]]. One job is submitted per line in the input file.
        --cafe-commands            : All arguments passed after this will be passed to cafe on the node.
        -S|--cafe-script           : Path to non-standard (i.e. not in ${CAFANA}/scripts) cafe script that should be included in the tarball.
        -f|--force-remove          : Removes output directory before starting.
        -d|--dry-run               : Will not submit anything to the grid.
        --expected-disk            : Expected disk usage to pass to jobsub (default: 1GB)
        --expected-mem             : Expected mem usage to pass to jobsub (default: 2GB)
        --expected-walltime        : Expected disk usage to pass to jobsub (default: 4h)
        -?|--help                  : Print this message.
```

Hopefully most of the options are self-explanatory. Those in need of further explanation are detailed here:

* `-p|--pnfs-path-append`: All output files copied back on success will end up in `/pnfs/dune/scratch/users/${USER}/<-p argument>`.
* `-c|--cafe-command-file`: Path to a CAFE command file which contains a newline-delimited list of cafe commands to submit jobs for. A single cluster of jobs will be submitted, with n processes, each of which will run one line from the input file.
* `--cafe-commands`: Can be used to pass a single CAFE command on the command line rather than in a single-line CAFE command file.
* `-S|--cafe-script`: Not all of the CAFAna source is tar'ed up and shipped off to the grid nodes, all of the CAFE scripts that you will want to run need to be available, this allows you to pass a single script to be added to the tarball.

### CAFE Command file

For clarity an example CAFE command file is shown here:

```
MyAnalysisScript.C root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr/dune/resilient/MYUSERNAME/CAFAnaStateFiles/DefaultState_vX MyAnalysisOut.root opt1 opt2 opt3 0
MyAnalysisScript.C root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr/dune/resilient/MYUSERNAME/CAFAnaStateFiles/DefaultState_vX MyAnalysisOut.root opt1 opt2 opt3 1
MyAnalysisScript.C root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr/dune/resilient/MYUSERNAME/CAFAnaStateFiles/DefaultState_vX MyAnalysisOut.root opt1 opt2 opt3 2
MyAnalysisScript.C root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr/dune/resilient/MYUSERNAME/CAFAnaStateFiles/DefaultState_vX MyAnalysisOut.root opt1 opt2 opt3 3
```

Running `FarmCAFENodeScript.sh -p myoutput -c MyCAFECommands.cmd` would check that `MyAnalysisScript.C` exists, and if it doesn't, report something like:

```
[ERROR]: In CAFECommands.cmd on line 0:
  MyAnalysisScript.C root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr/dune/resilient/MYUSERNAME/CAFAnaStateFiles/DefaultState_vX MyAnalysisOut.root opt1 opt2 opt3 0
[ERROR]: Trying to run job for script MyAnalysisScript.C. But it doesn't exist in the tarball at CAFAna/scripts/ and wasn't passed with -S. The script needs to be installed with CAFAna or passed to this submission script to run.
[INFO]: tar -tf CAFAna.Blob.tar.gz | grep CAFAna/scripts:
CAFAna/scripts/
CAFAna/scripts/common_fit_definitions.C
CAFAna/scripts/fit_covar.C
CAFAna/scripts/remake_inputs.C
CAFAna/scripts/asimov_joint_coarse.C
```

If it did, it would submit a single cluster of 4 jobs, requesting 1 Gb of disk space, 2 Gb of memory, and 4h walltime, writing the output files to `/pnfs/dune/scratch/users/${USER}/myoutput/$CLUSTERID.$JOBID/`.

### CAFE Scripts

If you need more than one script for a given command file to be included in the tarball, the best way is to add these scripts to the installation target of the build, so that they exist in `${CAFANA}/scripts/`. This can be done in `scripts/CMakeLists.txt` by adding script names to the `scripts_to_install` list variable. *N.B.* You must re-configure and install before doing so.

The `FarmCAFENodeScript.sh` will check each line of the CAFE command file for
if the script is included in the tar, if it isn't the submission will fail to
help preserve your precious queue priority from jobs that will insta-fail.

### Files copied back

Most of the CAFE scripts used in the DUNE LBL analysis have a standard first
few arguments, as such, the 3rd positional argument in a CAFE Command line is
guessed as the name of the output root file. If this file is not found after
`cafe` has finished executing, a warning is added to the log file. However, all
`.root` files are copied back to the output directory along with the stdout/err
or the `cafe` invocation.
