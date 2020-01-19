# PRISM Tools

## Input generation

Before being used in the PRISM analysis, a production of CAF files must be
combined correctly. Because in a PRISM analysis the detector sits at multiple
positions, different files cannot simply be summed and total POT tallied.
The `OffAxisNDCAFCombiner` script performs this task. I can be used with cafe,
but can also be built with `CAFBuild`. Example usage to produce an
input ND CAF can be found in `BuildOffAxisInputs.sh`.

The output combined file will contain a number of extra trees, the most
important is the `POTWeightFriend` tree, this contains two branches, both of
which should be used to weight all predictions as a function of off axis angle
to per POT.

When this tree is found in an input file `Core/SpectrumLoader` will hook up the
`perPOTWeight` and `perFileWeight` data members on the DUNE `StandardRecord`.

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

`SetStoreDebugMatches` can be used to store the results of flux/event rate
matches and `Write` can be used to write these to a directory.

*N.B.* The event rate matching does work, but the regularization strength needs
to be tuned. With no regularization the problem is usually too degenerate for
the linear algebra methods to find a useful solution, and with too strong
regularization the result is useless for physics. Try in powers of 10, 1E-15
has been close-ish in the passed.

The flux matcher also provides access to the most recent match residual, which
is used by `PredictionPRISM` to include a far detector MC correction.

Any flux systematic shifts are only used by the event rate matcher (They could be hooked up to the flux matcher, but currently aren't), and require that you have a set of flux uncertainties that span a wide enough off axis range.

### PredictionPRISM

#### Instantiation

This is the workhorse class for building a PRISM far detector prediction.
The various corrections that go into the PRISM far detector prediction are all
opt-in, though without the flux matcher, the results will not be sensible.

The main constructor:

```
  PredictionPRISM(SpectrumLoaderBase &, const HistAxis &recoAxis,
                  const HistAxis &offAxis, const Cut &cut,
                  const Var &wei = kUnweighted);
```

Is used to define the projection axes, the off axis axis and the 'data' cuts
and weights.

A near detector MC prediction can also be added with:

```
  void AddNDMCLoader(Loaders &, const Cut &cut = kNoCut,
                     const Var &wei = kUnweighted,
                     std::vector<ana::ISyst const *> systlist = {});
```

This is used to correct for selected NC, wrong-sign-backgrounds, and nue events
in the 'data'. As these backgrounds do not follow the same distribution off
axis they cannot be extrapolated with the signal. They have to then be added
back in at the far detector, to do this a far detector loader can be set up
with:

```
  void AddFDMCLoader(Loaders &, const Cut &cut = kNoCut,
                     const Var &wei = kUnweighted,
                     std::vector<ana::ISyst const *> systlist = {});
```

These three background corrections can be enabled and disable with:

```
  void SetNCCorrection(bool v = true);
  void SetWSBCorrection(bool v = true);
  void SetNueCorrection(bool v = true);
```

These set the subtraction at the near detector and the addition at the far
detector simultaneously.

Without the far detector prediction, the flux match residual correction cannot
be made and so event truth-selected predictions will fail to match the far
detector prediction well across the whole spectrum.

The final opt in component is the flux matcher, it can be set with:

```
void SetFluxMatcher(PRISMExtrapolator const *flux_matcher);
```

The `PredictionPRISM` class should save and load to a `TDirectory` in the same
way as other `IPrediction` subclasses.

The second constructor:

```
PredictionPRISM(const HistAxis &recoAxis, const HistAxis &offAxis);
```

can be used to instantiate a `PredictionPRISM` without any 'data', in this
case, `AddNDMCLoader` must be called for anything meaningful to happen.
This constructor is also the one used by `PredictionPRISM::LoadFrom`

#### Making a Prediction

As the `PredictionPRISM` is a subclass of IPrediction, it exposes the two
standard Predict methods:

```
  virtual Spectrum Predict(osc::IOscCalculator *calc) const override;
  virtual Spectrum PredictComponent(osc::IOscCalculator *calc,
                                    Flavors::Flavors_t flav,
                                    Current::Current_t curr,
                                    Sign::Sign_t sign) const override;
```

Although, at the time of writing, PredictComponent does not do what you might
expect. Instead, you should use:

```
virtual std::map<PRISMComponent, Spectrum>
PredictPRISMComponents(osc::IOscCalculator *calc,
                       ana::SystShifts shift = kNoShift) const;
```

To examine any correction components. Included components are enumerated by:

```
  enum PRISMComponent {
    kNDData = (1 << 0),
    kNDDataCorr = (1 << 1),
    kNDDataCorr2D = (1 << 2),
    kNDSig = (1 << 3),
    kNDWSBkg = (1 << 4),
    kNDNCBkg = (1 << 5),
    kNDNueBkg = (1 << 6),
    kFDFluxCorr = (1 << 7),
    kFDNCBkg = (1 << 8),
    kFDWSBkg = (1 << 9),
    kFDNueBkg = (1 << 10)
  };
```

`kNDDataCorr2D` is the un-linearly-combined corrected near detector data.

*N.B.* If you used the main constructor (**i.e.** the `PredictionPRISM` has
  'data'), then `PredictSyst` will not apply systematic shifts to the uncorrected
  ND prediction (as it will have come from the 'data'). You can use:

```
void SetIgnoreData(bool v = true)
```

To make a prediction using consistent near and far components.

### PRISMPrediction.C

There is an example script that shows how to make PRISM predictions. It can be
used with `cafe`, but can also be compiled with `CAFBuild` like:

```
CAFBuild -O3 -I ${CAFANA}/../Ext/src/eigen/ -I ${CAFANA}/scripts PRISMPrediction.C
```

It has 2 required and 4 optional position command line arguments:

```
PredictionPRISM.exe <Input Offaxis ND file> <output file> [<is fhc? true/false>\
  <state file name> <force regenerate state file? true/false>]
```

### Utils

Some useful methods and `ana::Var`s and `ana::Cut`s live in
`PRISM/PRISMUtils.h`. Most notably the FV mass corrector which applies a
correction weight to each detector selection slice based on the volume of
liquid argon. Some slices contain the ArgonCube module walls and readout
electronics.
