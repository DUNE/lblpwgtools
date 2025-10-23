# CAFAna: README
Last updated: June 2025 (JW)

Contributors:
* Maria Martinez-Casales [[mcasales@fnal.gov](mcasales@fnal.gov)]
* Jeremy Wolcott [[jwolcott@fnal.gov](jwolcott@fnal.gov)]

## What is CAFAna?

CAFAna is an analysis framework designed to work with CAFs ("Common Analysis Format" files).
You can read more about the toolkit itself in its dedicated GitHub organization: https://github.com/cafana.
You've landed onto DUNE's adaptation of CAFAna, which was first introduced for work in the Long Baseline physics working group
(hence the repository name `lblpwgtools`).

In a nutshell, CAFAna does the following things:
* Wraps **loops over analysis ntuples** in a user-friendly way
* Provides a user-friendly interface for **aggregating histograms** (and many more complex structures built from histograms) during the aforementioned loops
* Offers extensive tools for **fitting models** to reference data via minimizers or MCMC sampling


## Installing CAFAna

CAFAna is a C++ library that needs to be compiled & linked in order to use it.
Currently, the `lblpwgtools` edition of CAFAna only compiles inside a Scientific Linux 7 environment
with DUNE software products available via CVMFS.
**The only official support for this environment is using a DUNE GPVM** (though if you are knowledgable about CVMFS and containers you may be able
to make it work locally).
Alma Linux 9 support will be introduced soon.

### Setting up the container

DUNE computing has instructions on how to work with the SL7 containers
on GPVMS here: https://wiki.dunescience.org/wiki/SL7_to_Alma9_conversion 

* Note: for initial installation and other instances where you want to _compile_ code,
  you should use one of the `dunebuild0X` nodes instead of a GPVM.
  In that case, remove `/pnfs/dune/` from the incantation below.

* You can start the container on a GPVM this way (see previous bullet if you're compiling instead of running CAFAna):

```bash
/cvmfs/oasis.opensciencegrid.org/mis/apptainer/current/bin/apptainer shell --shell=/bin/bash \
-B /cvmfs,/exp,/nashome,/pnfs/dune,/opt,/run/user,/etc/hostname,/etc/hosts,/etc/krb5.conf --ipc --pid \
/cvmfs/singularity.opensciencegrid.org/fermilab/fnal-dev-sl7:latest
```


* You will also need to have the UPS areas ready for use:  
```bash
# before anything else, set up UPS
source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh
```

### CAFAna installation

* Clone the repository from GitHub.  In general, the default branch should produce usable results,
  but if desired or directed, you can use a specific tag instead.

```basjh
source_dir=/exp/dune/app/users/$USER/src  # or wherever you like

cd $source_dir && git clone git@github.com:DUNE/lblpwgtools.git
cd $source_dir/lblpwgtools/CAFAna

# if a different branch is needed
git switch <branchname>
```

* A helper build script lives in this `CAFAna` subdirectory that will build everything for you:

```bash
build_dir=/exp/dune/app/users/$USER/build/cafana      # or wherever you like
install_dir=/exp/dune/app/users/$USER/install/cafana  # or wherever you like

cd $source_dir/lblpwgtools/CAFAna
./standalone_configure_and_build.sh -u -b $build_dir -I $install_dir
```

You can add a `-j N` arguments to use N cores on the build machine (just verify that you aren't clobbering someone else by doing so).

**Note:** as of June 2025, you'll see some warnings during the build process about CAFAna components that are currently disabled.
This is normal and expected.  You don't need those pieces unless you know you need them. :)

### Subsequent use

A setup script called `CAFAnaEnv.sh` is installed into the install area.  Source it to begin every CAFAna session!
```bash
# obviously you can hard-code whatever $install_dir corresponds to in a setup script/.bashrc if you prefer 
source $install_dir/CAFAnaEnv.sh
``` 

**There are two ways to run CAFAna scripts**.  Examples of how to do each are below.

#### Using `cafe`

The `cafe` executable is installed with CAFAna.  
This compiles a supplied ROOT C++ script into an executable using `gcc`.
You can examine its options with `cafe --help`, but the general idea is:
```
# if you haven't already
source /path/to/CAFAna/CAFAnaEnv.sh

cafe /path/to/your/script.C [arguments] [passed] [to] [your] [script]
```

This method of operation is straightforward, requires no extra infrastructure, and can work well for simple analysis tests.

However, for larger analysis projects that develop shared centralized code,
we recommend constructing a standalone project that depends on `lblpwgtools`.
This way you can build your own shared libraries, compile your scripts into standalone executables, etc.

#### Building a CMake project using `lblpwgtools`

`lblpwgtools` exports enough CMake information into its installation that it can be used as a CMake package.

Simply ensure that `CAFAnaEnv.sh` is sourced before attempting to run `cmake` on your project.

The `tute` directory can be built as an example standalone project:

```bash
source /path/to/CAFAna/CAFAnaEnv.sh

# or whatever you like
tute_src_dir=/exp/dune/app/users/$USER/src/lblpwgtools/CAFAna/tute
tute_build_dir=/exp/dune/app/users/$USER/build/cafana-tute
tute_install_dir=/exp/dune/app/users/$USER/install/cafana-tute

cd $tute_build_dir
cmake -DCMAKE_INSTALL_PREFIX=$tute_install_dir $tute_src_dir
make install
```

after which the `demo0` executable should be available in `$tute_install_dir`.

## Plotting examples 

The examples below are all written with the `cafe` style,
but could equally be written in the CMake style described above.

#### Assumptions
These examples assume you are running them from a Fermilab GPVM.

Further, they assume you have the appropriate credentials to read files form PNFS.
As of June 2025, this means you need a "token."
See the DUNE Computing wiki on [Getting a token](https://wiki.dunescience.org/wiki/DUNE_Computing/Using_the_Physics_Groups_Persistent_Space_at_Fermilab#Getting_a_token).

#### Included examples

The CAFAna source contains a `tute` directory with some examples.  They can all be invoked like this:
```
cafe -bq -l 10 /path/to/CAFAna/tute/<name_of_demo>.C
```
(The `-l` option limits the number of files read by the script so you can obtain plots faster. 
See other options with `cafe --help`.)

The example scripts in the `tute` directory have the following content:

##### demo_cutsvars.C
Basic distributions in 1D and 2D of variables from the ` SRCommonRecoBranch`.

##### demo_systs.C
Demonstration how systematic shifts can be managed

##### demo_ensemble.C
Show how to create "ensembles" of systematics and manipulate them together 

##### demo_2x2_*.C
Worked examples reproducing various 2x2 analyses (as of late 2024). 

## To do's / caveats / wishlist
- Systematic variations are not implemented in truth branches yet
- More realistic examples of systematics
##

