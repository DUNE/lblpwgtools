# PRISM Tools

## Input generation

Before being used in the PRISM analysis, a production of CAF files must be
combined correctly. Because in a PRISM analysis the detector sits at multiple
positions, different files cannot simply be summed and total POT tallied.
The `OffAxisNDCAFCombiner` script performs this task. I can be used with cafe,
but can also be built with `CAFBuild`. Example usage to produce an
input ND CAF can be found in `BuildOffAxisInputs.sh`.

The output combined file will contain a number of extra trees, the most important is the `POTWeightFriend` tree, this contains two branches, both of which should be used to weight all predictions as a function of off axis angle to per POT.

When this tree is found in an input file `Core/SpectrumLoader` will hook up the `perPOTWeight` and `perFileWeight` data members on the DUNE `StandardRecord`.

## Analysis Components

### PRISMExtrapolator

This performs the flux/event rate matching between the off-axis ND spectrum
and the predicted FD spectrum. You can either initialize one with a flux
prediction input, or with an ND and an FD `PredictionInterp`. There are some
flux match conditioning options, notably min and max energy and a regularization
strength parameter. There is a space for a Gaussian low energy tail, but at the
time of writing, it has not been implemented.

*N.B.* The passed `PredictionInterp` must be use a true energy axis with no
physics selection applied.

`SetStoreDebugMatches` can be used to store the results of flux/event rate matches and `Write` can be used to write these to a directory.

*N.B.* The event rate matching does work, but the regularization strength needs
to be tuned. With no regularization the problem is ususally too degenerate for
the linear algebra methods to find a useful solution, and with too strong
regularization the result is useless for physics.

The flux matcher also provides access to the most recent match residual, which
is used by `PredictionPRISM` to include a far detector MC correction.


### PredictionPRISM

### Utils
