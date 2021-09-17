# PRISM Tools

## Input generation

Before being used in the PRISM analysis, a production of CAF files must be
combined correctly. Because in a PRISM analysis the detector sits at multiple
positions, different files cannot simply be summed and total POT tallied.
The `OffAxisNDCAFCombiner` script performs this task. It can be used with cafe,
but can also be built with `CAFBuild`. Example usage to produce an
input ND CAF can be found in `BuildOffAxisInputs.sh`.

The output combined file will contain a number of extra trees, the most
important is the `POTWeightFriend` tree, this contains two branches, both of
which should be used to weight all predictions as a function of off axis angle
to per POT.

When this tree is found in an input file `Core/SpectrumLoader` will hook up the
`perPOTWeight` and `perFileWeight` data members on the DUNE `StandardRecord`.

## Analysis Components

### MakePRISMPredInterps

Before running a PRISM analysis, a so-called 'state file' must be produced. This is
essentially loading the MC CAF file into ROOT histograms, which can be then be used
for analysis by the main PRISM code.

The compiled executable script `MakePRISMPredInterps` requires numerous inputs, 
including the near and far detector CAF files, the desired binning and any systematic dials.

`MakePRISMPredInterps` draws a distinction between what is regarded as 'data' and 'MC'.
The 'data' are saved as either `ReweightableSpectrum` or `OscillatableSpectrum` objects,
which are not subject to systematic variations (except an optional 'fake data' shift).
The 'MC' components are all saved as `PredictionInterp` objects, which are subject to 
systematic variations. 

*N.B.* Saving a `PredictionInterp` object saves an additional histogram in your 
'state file' for each (+/-)1,2,3 sigma variations of each systematic dial included.
As a result, the amount of time this script can take to run increases 
dramatically with each additional systematic dial. This is why the FermiGrid 
scripts have been written, see below.


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
  PredictionPRISM(const HistAxis &AnalysisAxisND, const HistAxis &AnalysisAxisFD,
                  const HistAxis &NDOffAxis,
                  const HistAxis &ND280kAAxis,
                  const HistAxis &NDFDEnergyMatchAxis);
```

Is used to define the near and far projection axes, the 293kA and 280kA off axis axis and 
a true energy axis.

The near detector 'data' can be added to you `PRISMPrediction` object with:

```
  void AddNDDataLoader(SpectrumLoaderBase &, const Cut &cut,
                       const Var &wei = kUnweighted,
                       ana::SystShifts shift = kNoShift,
                       PRISM::BeamChan NDChannel = PRISM::kNumu_Numode);
```

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
  void SetWrongSignBackgroundCorrection(bool v = true);
  void SetWrongLeptonBackgroundCorrection(bool v = true);
  void SetIntrinsicBackgroundCorrection(bool v = true);
```

These set the subtraction at the near detector and the addition at the far
detector simultaneously.

Without the far detector prediction, the flux match residual correction cannot
be made and so event truth-selected predictions will fail to match the far
detector prediction well across the whole spectrum.

Another important opt in component is the flux matcher, it can be set with:

```
  void SetFluxMatcher(PRISMExtrapolator const * flux_matcher);  
```

The `PredictionPRISM` class should save and load to a `TDirectory` in the same
way as other `IPrediction` subclasses.

#### Near-to-Far Detector Extrapolation

The detector acceptance and resolution at the near and far detector will be 
different and this must be corrected for when producing a PRISM prediction
with selected events and reconstructed energy. Two classes have been written to
handle this: `NDFD_Matrix` and `MCEffCorrection`.

`NDFD_Matrix` produces smearing matrices for the near and far detector. Its
constructor is:

```
  NDFD_Matrix(PredictionInterp const * ND,
              PredictionInterp const * FD,
              double reg);
```

The `PredictionInterp` objects in the constructor refer to the ND and FD smearing
matrices. Give your `PredictionPRISM` class access to the `NDFD_Matrix` object
by calling in your executable:

```
  void SetNDFDDetExtrap(NDFD_Matrix const * det_extrap);
```

`MCEffCorrection` calculates the selection efficiency from the MC as a function
of true event energy. Initialize with constructor:

```
  MCEffCorrection(PredictionInterp const * NDunsel_293kA,
                  PredictionInterp const * NDsel_293kA,
                  PredictionInterp const * NDunsel_280kA,
                  PredictionInterp const * NDsel_280kA,
                  PredictionInterp const * FDunsel,
                  PredictionInterp const * FDsel);

```

The `PredictionInterp` objects will be contained in your state file.

Give your `PredictionPRISM` class access to the `MCEffCorrection` object using

```
  void SetMC_NDFDEff(MCEffCorrection const * eff_corr);
```

in your executable.

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

This function is where the background subtraction and addition, detector extrapolation
and linear combination happens. The extrapolation is largely performed in `PredictPRISMComponents`
by calling the function

```
  void ExtrapolateNDtoFD(ReweightableSpectrum NDDataSpec,
                         double POT, const int kA, const TH1 *weights,
                         osc::IOscCalculator *calc, ana::SystShifts shift = kNoShift,
                         Flavors::Flavors_t NDflav = Flavors::kAll,
                         Flavors::Flavors_t FDflav = Flavors::kAll,
                         Current::Current_t curr = Current::kCC,
                         Sign::Sign_t NDsign = Sign::kBoth,
                         Sign::Sign_t FDsign = Sign::kBoth,
                         std::vector<std::vector<double>> NDefficiency = {{}},
                         std::vector<double> FDefficiency = {}) const;
```

which is a member function of `NDFD_Matrix` and takes the background-corrected ND data
and the selection efficiency calculated in `MCEffCorrection` as inputs.

Included components are enumerated by:

```
  enum PRISMComponent {
     kNDData_293kA = 0,
     kNDDataCorr_293kA = 1,
     kNDDataCorr2D_293kA = 2,
     kNDSig_293kA = 3,
     kNDSig2D_293kA = 4,
     kNDWSBkg_293kA = 5,
     kNDNCBkg_293kA = 6,
     kNDWrongLepBkg_293kA = 7,
     kNDFDWeightings_293kA = 8,
     .
     .
     . etc.
  };
```

At the time of writing there are over 40 of these `enum PRISMComponent`.
`kNDDataCorr2D_293kA` is the un-linearly-combined background-corrected 
near detector data for the 293kA horn current sample.
`kNDDataCorr_FDExtrap` is the linearly-combined, background-corrected and
detector-extrapolated PRISM prediction.

*N.B.* If you used the main constructor (**i.e.** the `PredictionPRISM` has
  'data'), then `PredictSyst` will not apply systematic shifts to the uncorrected
  ND prediction (as it will have come from the 'data'). You can use:

```
  void SetIgnoreData(bool v = true)
```

To make a prediction using consistent near and far components.

### PRISMPrediction.C


At the time of writing scripts such as `PRISMPrediction.C` - contained in CAFAna/PRISM/app - 
are run as compiled executables.

Only one command line input is needed: a FHiCL file, which contains all input information
needed for the script to run. E.g.:

``` 
  PRISMPrediction fcl/PRISM/Basic_NumuDisp.fcl
```

All FHiCL files are conained in the directory CAFAna/fcl/PRISM/.

It is possible in principle to setup the PRISM code to run with the `cafe` command by 
adding the PRISM libraries to the `load_libraries` script, although this has not been
implemented. Scripts can lso be compiled with `CAFBuild` like:

```
  CAFBuild -O3 -I ${CAFANA}/../Ext/src/eigen/ -I ${CAFANA}/scripts PRISMPrediction.C 
``` 

However, it is recommended to use the compiled executables in conjunction with the
FHiCL files.

### FHiCL files


The FHiCL files in the `CAFAna/fcl/PRISM/` directory contain all the inputs needed
to run the PRISM scripts and avoids the need for many command line arguments or
hard-coding. An example of an input in the FHiCL config file is below:

`vary_NDFD_data`: A boolean that if true keeps the MC componenets at their
nominal values and varies the 'data'. If false, the MC is varied by the inputed
systematics and the 'data' is constant. 


### Utils

Some useful methods and `ana::Var`s and `ana::Cut`s live in
`PRISM/PRISMUtils.h`. Most notably the FV mass corrector which applies a
correction weight to each detector selection slice based on the volume of
liquid argon. Some slices contain the ArgonCube module walls and readout
electronics.

## Running PRISM on FermiGrid

*N.B.* These scripts are not very user-friendly yet. Some hard-coding in the 
scripts may still be necessary to get the desired output.

Scripts and have been written that allow the user to run the PRISM code on the FermiGrid.
This should only be necessary when using implementing systematics with large MC files. The 
grid scripts are contained in `PRISM/scripts/FermiGridPRISMScripts/`. Here there are two
compiled bash executables: `FarmBuildPRISMInterps.sh` and `FarmCAFPRISMNodeScript.sh`.

`FarmBuildPRISMInterps.sh` is for building your state files on the grid. If you want to 
do an analysis with the full ND MC sample and numerous systematic dials, it is recommended
that you run this script once for each systematic dial. I.e. submit a job for each systematic
you are interested in (with the command line argument `--syst-descriptor list:<systdial>`) and 
add the resulting output state files together using `hadd_state`.

Similarly, `FarmCAFPRISMNodeScript.sh` is for running analysis executables on the grid. It
is mostly for running fits with many nuisance parameters, although any executable can be run 
with it. The most important command line input is `--cafe-command-file <CAFCommand Script>`, 
which submits one job per line. E.g. `Dmsq32ScanCommands.cmd` is set up to perform a 1D fit
for dmsq32 with one job submitted for each fit point.

If running a job for each fit-point there will be many output files. These can be added
together using `hadd_fits`.
