
# Making your first plot in CAFAna

This is a simplified guide on how to install and run `CAFAna` to make your first plot . A lot more information can be found in [this tutorial](https://github.com/chenel/dune-nd-lar-reco/blob/main/tute/2021-12-02%20CAFAna-NDLAr-Howto.md) by Jeremy Wolcott.

Also some information is duplicated from a directory above, but gathered here for your easy access.

## How to install

Clone this branch of the repository:
```
git clone git@github.com:DUNE/lblpwgtools.git
checkout feature/source-sink-overhaul
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

Once CAFAna is built and the environment setup, proceed to run an example scripts doing:
```
cafe tute/demo0b.C
```
