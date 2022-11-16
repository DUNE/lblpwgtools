# FermiGrid Scripts for PRISM

Some starter scripts for submitting PRISM jobs to FermiGrid. User scripts are
named `Farm<something>.sh`, the main one being `FarmCAFPRISMNodeScript.sh`. There
is a script for building the state files on the grid, `FarmBuildPRISMInterps.sh`. Both
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
the CAFExecutor is available with `which cafe`. However PRISM scripts are generally
setup to be run as c++ executables

## State file location

Currently the location of the ND MC CAF files is 

```
/pnfs/dune/persistent/physicsgroups/dunelbl/abooth/PRISM/Production/Simulation/ND_CAFMaker/v7/CAF
```

However for the PRISM analysis we use some pre-processed hadded state files in 

```
ND FHC:  /pnfs/dune/persistent/users/chasnip/NDCAF_OnAxisHadd/FHC
ND RHC: /pnfs/dune/persistent/users/chasnip/NDCAF_OnAxisHadd/RHC
```

To make a state file on the grid run `FarmBuildPRISMInterps.sh`, which has a help text:

```
[RUNLIKE]
        -p|--pnfs-path-append      : Path to append to output path: /pnfs/dune/persistent/users/chasnip/
        -i|--input-dir             : Path to search for files in.
        -a|--axis-argument         : Argument to pass to remake_inputs to specify the axes
        --bin-descriptor          : Bin descriptor string to pass.
        --syst-descriptor          : Syst descriptor string to pass.
        --no-fakedata-dials        : Remove fake data dials.
        --nmax                     : Will run a maximum of --nmax jobs
        -f|--force-remove          : If files are found in the output directory, remove them.
        -N|--ND-Only               : Only run ND.
        -F|--FD-Only               : Only run FD.
        -u|--FHC-Only              : Only run FHC.
        -b|--RHC-Only              : Only run RHC.
        -v|--analysis-version <v=4>: Set analysis version, (3 or 4).
        -?|--help                  : Print this message.
```

Give the option `-i` either the ND FHC or the ND RHC file path. The script will run a grid job for each
CAF file. Make sure to specify options `-N` and either `-u` or `-b` to specify either FHC or RHC files.

It is recommended that the user runs the FD state file maker interactively, since this is much faster.

## `FarmCAFPRISMNodeScript.sh`

This is the main user submission helper script. It checks the CAFAna and grid
authentication environments, tars up a built version of CAFAna, checks that
the cafe scripts for requested jobs will exist on the node and then submits
job clusters for execution. The latest help text is copied here:

```
[RUNLIKE]  [opts] --cafe-comands <args>
        -p|--pnfs-path-append      : Path to append to output path: /pnfs/dune/persistent/users/${USER}/
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

Hopefully most of the options are self-explanatory. `--expected-mem` is very important for running fits with systematics - 
typically a total fit with all systematics needs roughly 20-25GB of RAM.

### CAFE Command file

The CAFE command files are just text files that contain the commands to run the PRISM scripts. The two command files
that run either 1D scans or 2D scans are `PRISM_1DScan_Commands.cmd` and `PRISM_2DScan_Commands.cmd` respectively.
