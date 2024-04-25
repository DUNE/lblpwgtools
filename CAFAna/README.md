
# Making your first plot in CAFAna

This is a simplified guide on how to install and run `CAFAna` to make your first plot . A lot more information can be found in [this tutorial](https://github.com/chenel/dune-nd-lar-reco/blob/main/tute/2021-12-02%20CAFAna-NDLAr-Howto.md) by Jeremy Wolcott.

Also some information is duplicated from a directory above, but gathered here for your easy access.

## How to install

Clone this branch of the repository:
```
git clone -b feature/2x2-tutorial  git@github.com:DUNE/lblpwgtools.git
cd lblpwgtools
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
- These examples work perfectly with `caf` files. `flatcaf`s are showing errors, needs to be resolved.
- Systematic variations are not implemented in truth branches yet
- Missing truth to reco piping (More complex than I imagined ...)
- Add a feature to count number of events read (without having to convert to histogram and compute integral)
- More realistic examples of systematics

##

Last updated on Apr 23, 2024, by Maria Martinez-Casales (mcasales@fnal.gov)
