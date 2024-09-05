
# Making your first plots in CAFAna

This is a simplified guide on how to install and run `CAFAna` to make your first plot . A lot more information can be found in [this tutorial](https://github.com/chenel/dune-nd-lar-reco/blob/main/tute/2021-12-02%20CAFAna-NDLAr-Howto.md) by Jeremy Wolcott.

Also some information is duplicated from a directory above, but gathered here for your easy access.

## Setup a container

As you may know, we are living now in AL9 world, but I don't know how to use spack to build CAFAna. Instead you can setup this standard container provided for us as in https://wiki.dunescience.org/wiki/SL7_to_Alma9_conversion 

```
/cvmfs/oasis.opensciencegrid.org/mis/apptainer/current/bin/apptainer shell --shell=/bin/bash \
-B /cvmfs,/exp,/nashome,/pnfs/dune,/opt,/run/user,/etc/hostname,/etc/hosts,/etc/krb5.conf --ipc --pid \
/cvmfs/singularity.opensciencegrid.org/fermilab/fnal-dev-sl7:latest
```
The reccommendation is that you use `dunebuild03` instead of a gpvm to build your code. Remove `/pnfs/dune/` when setting up the container, otherwise it will complain. Later on, you can run code and access `pnfs` on a regular gpvm. 

```
# before anything else, set up UPS
source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh
```
Now you can go ahead and install.

## How to install

Clone this branch of the repository:
```
git clone git@github.com:DUNE/lblpwgtools.git
cd lblpwgtools
checkout feature/source-sink-overhaul
```

We currently don't have official versions of `cafanacore` ,`duneanaobj`,`osclib`, and `srproxy` , that have truth-reco matching, so we need this extra step to fetch working versions (eventually these should be published):
```
export PRODUCTS="/exp/dune/app/users/jwolcott/ups:$PRODUCTS"
export PRODUCTS="/exp/dune/app/users/mcasales/ups:$PRODUCTS" 
```

A helper build script lives in this `CAFAna` subdirectory. You can build and install the code like:

```
cd CAFAna
./standalone_configure_and_build.sh -r
```

If you are in a DUNE `gpvm` and therefore have `cvmfs` access, you can rely on relevant dependencies from FNAL scisoft by add in the `-u` option:

```
./standalone_configure_and_build.sh -r -u -j 4
```

Once CAFAna has been built, to set up the environment you will need to `source /path/to/install/CAFAnaEnv.sh`. If `standalone_configure_and_build.sh` was not passed a `-I` argument, then this will be `/path/to/repo/CAFAna/build/Linux/CAFAnaEnv.sh` by default. 

## Examples 

Once CAFAna is built and the environment setup, proceed to run an example script doing:
```
cafe tute/demo0b.C
```
```
cafe -l 10 tute/demo0b.C
```
The `-l` option limits the number of files read by the script so you can obtain plots faster. See other options with `cafe --help`.

The three example scripts in the `tute` directory have the following content:

### demo0b 
Basic distributions in 1D and 2D of variables from the ` SRCommonRecoBranch`.

### demo1b
More distributions, more manipulations of variables/cuts, now of variables from the `SRTruthBranch`. Also showing how to open/read files.

### demo0eb
Examples of systematic variations applied to distributions from ` SRCommonRecoBranch` variables. 

## To do's / caveats / wishlist
- Systematic variations are not implemented in truth branches yet
- More realistic examples of systematics
- Note: If you run into issues accessing files, which looks like ```Error in <TNetXNGFile::Open>: [FATAL] Auth failed: No protocols left to try```, you probably need to `setup duneutil v09_89_01d01 -q e26:prof` in order to do  `setup_fnal_security`. You might need to re-build after doing this. I am working on adding a detailed instruction on this. Remember to source `CAFAnaEnv.sh` after building. 
##

Last updated September 4, 2024, by Maria Martinez-Casales (mcasales@fnal.gov)


