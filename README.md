# DUNE Long Baseline Physics Working Group Tools

As of the TDR analysis (2019) this now mostly comprises just CAFAna.

## Build

A helper build script lives in the CAFAna subdirectory. You can build and install the code like:

```
cd CAFAna
./standalone_configure_and_build.sh -r
```

If you are on a CENTOS6/7 machine and have `cvmfs` access, you can
rely on relevant dependencies from FNAL scisoft by add in the `-u`
option:

```
./standalone_configure_and_build.sh -r -u
```

If you don't, then you will need ROOT, GSL, CLHEP, and some BOOST components to build. The above script will check these dependencies
via relevant XXX-config helpers or some poor guesswork in the case of
BOOST. Most of these dependencies should be available from distribution package managers. Some details follow:

### ROOT

Your set up ROOT install must have the minuit2 capabilities included as this is used for minimization by CAFAna. Check by:

```
$ root-config --features | grep "minuit2"
# e.g. asimage astiff builtin_afterimage builtin_clang builtin_davix builtin_ftgl builtin_gl2ps builtin_glew builtin_llvm builtin_lz4 builtin_lzma builtin_tbb builtin_vdt builtin_xxhash clad cling cxx17 davix exceptions explicitlink fftw3 fitsio gdml http imt mathmore minuit2 opengl pch pythia6 python roofit root7 shared soversion ssl thread tmva tmva-cpu tmva-pymva vdt x11 xft xml
```

If you can see `minuit2`, you're good to go.

### BOOST

Most-likely you will have to specify `BOOST_INC` and `BOOST_LIB`
via your environment. The directory pointed to by `BOOST_INC` is expected to contain the `boost` subdirectory. The directory pointed to by `BOOST_LIB` is expected to contain `libboost_filesystem.so`.

### Detailed build script options

At the time of writing the full response to a request for help to `standalone_configure_and_build` was:

```
$ ./standalone_configure_and_build.sh -?
[RUNLIKE]
	-f|--force-remove      : Remove previous build directory if it exists.
	-r|--release           : Compile with CMAKE_BUILD_TYPE=RELEASE
	--rdb                  : Compile with CMAKE_BUILD_TYPE=RELWITHDEBINFO
	--knl                  : Build with -march=knl
	--use-gperftools       : Compile libunwind and gperftools
	-u|--use-UPS           : Try and use ups to set up required packages, rather than assuming they exist on the local system.
	-j|--n-cores           : Number of cores to pass to make install.
	-O|--omp               : Enable OMP features of CAFAna.
	-I|--install-to        : Directory to install to.
	-?|--help              : Print this message.
```

## Getting started

Once CAFAna has been built, to set up the environment you will need to `source /path/to/install/CAFAnaEnv.sh`. If `standalone_configure_and_build.sh` was not passed a `-I` argument, then this will be `/path/to/repo/CAFAna/build/Linux/CAFAnaEnv.sh` by default.

## For NOvAns
