# Instruction for Study Single Systematic Impact on DUNE-PRISM Oscillation Analysis


## Install CAFAna Framework

[First time only]

```
# Set up from DUNE FNAL machines (dunegpvm*)
kinit -f weishi@FNAL.GOV                 # Use your FNAL kerberos password
ssh -X weishi@dunegpvm03.fnal.gov        
cd /dune/app/users/weishi
mkdir PRISMAnalysis
cd PRISMAnalysis
git clone https://github.com/weishi10141993/lblpwgtools.git
cd lblpwgtools
git checkout Catastrophic_Yolo_PRISM_Merge
cd CAFAna
# Build the code, the -u option rely on relevant dependencies from FNAL scisoft
./standalone_configure_and_build.sh -u -r --db
# To recompile: ./standalone_configure_and_build.sh -u -r --db -f
# May need to set export GSL_LIB=/usr/lib64
```

## Study systematic impacts on DUNE PRISM event rates

1. First produce the state files with systematics.

```
cd /dune/app/users/weishi/PRISMAnalysis/lblpwgtools/CAFAna
source build/Linux/CAFAnaEnv.sh                                 # set up the environment
# Ignore the error of can't find the example directory
```

To run interactively in terminal,

```
cd PRISM/app
# For usage: MakePRISMPredInterps --help
```

2. Suppose you have a state file from step 1, create fcl file for each systematic based on the template ```NuisanceSyst_Scan/Basic_PRISMPred_PlaceHolder.fcl```, then run the script below to run ```PRISMPrediction```,

```
cd /dune/app/users/weishi/PRISMAnalysis/lblpwgtools/CAFAna/PRISM/scripts/FermiGridPRISMScripts
chmod a+x PRISMPredVary1Syst.sh
./PRISMPredVary1Syst.sh
```

Now to apply the systematic shift to one specific step in the analysis, change ```(fVaryNDFDMCData ? kNoShift : shift)``` in following lines to ```kNoShift``` in ```PRISM/PredictionPRISM.cxx```,

```
L638/650: ND NC Bkg MC
L663/674: ND WL Bkg MC
L687/699: ND WS Bkg MC
L726: Calculate LC coefficient
L794/799: numu nue xsec correction
```

and change ```shift``` in following lines to ```kNoShift``` in ```PRISM/PRISMDetectorExtrapolation.cxx```,

```
L162: ND Unfold
L165: FD Smear
```

and change ```syst``` in following lines to ```kNoShift``` in ```PRISM/PRISMMCEffCorrection.cxx```,

```
L111/114/128/131: ND Eff corr
L142/146: FD Eff corr
```

I temporarily copy the changed files from another repo (no automatic script to do this in case there are future code updates).

Then run the script below (it only shifts each step above by changing ```kNoShift``` to ```shift``` and recompile and re-run ```PRISMPrediction```),

```
chmod a+x PRISMPredShift1StepND.sh
./PRISMPredShift1StepND.sh
```

3. Draw and plot.

Change ```channame``` in ```QuickHistPlotter.C``` to one of the following channels: ```FD_nu_numu```, ```FD_nu_nue```, ```FD_nub_numu```, ```FD_nub_nue```. The max y-axis range is set at ```MaxAbsY``` (default as 0.02).

```
# This makes the sigma variation plots for various component in PRISM
chmod a+x PlotManySysts.sh
./PlotManySysts.sh
```

Change ```isApp``` in ```MergeCanvas.C``` to match the channel you are plotting as the number of plots is different on the final canvas.

```
# This puts many plots on one canvas and prints tables
source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.24.02/x86_64-centos7-gcc48-opt/bin/thisroot.sh
root -l -b -q MergeCanvas.C
```
