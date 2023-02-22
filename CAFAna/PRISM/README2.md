# Instruction for running DUNE-PRISM analysis on dunegpvm*

## Build CAFAna analysis code

[First time only]

```
kinit -f <username>@FNAL.GOV                 # use your FNAL kerberos password
ssh -X <username>@dunegpvm03.fnal.gov        
cd /dune/app/users/<username>
mkdir PRISMAnalysis
cd PRISMAnalysis
git clone https://github.com/DUNE/lblpwgtools.git -b feature/prism
cd CAFAna
# Build the code, the -u option rely on relevant dependencies from FNAL scisoft
./standalone_configure_and_build.sh -u -r --db

# set up work area environment
source build/Linux/CAFAnaEnv.sh
```

To recompile, normally:
```
cd /dune/app/users/<username>/PRISMAnalysis/CAFAna/build
make install -j 4
```

The following is seldom used. It removes old compile and recompile from scratch:
```
./standalone_configure_and_build.sh -u -r --db -f
```

## ND CAF files

New ND CAF files with half stops:
```
ND FHC: /pnfs/dune/persistent/users/chasnip/NDCAF_OnAxisHadd/FHC
ND RHC: /pnfs/dune/persistent/users/chasnip/NDCAF_OnAxisHadd/RHC
```

Raw CAF files (pre-hadded) in LBL dir (geometric efficiency correction needs this)
```
/pnfs/dune/persistent/physicsgroups/dunelbl/abooth/PRISM/Production/Simulation/ND_CAFMaker/v7/CAF
```

## FD CAF files

```
/pnfs/dune/persistent/users/chasnip/CAF_MC_FILES_4FLAVOUR/

# Copied separately under:
/pnfs/dune/persistent/users/weishi/FDCAF/FHC

# Note: the nue or tauswap state file production actually runs over nonswap+nueswap/tauswap as nonswap always need to be included (not clear why) so the relevant folder have two FD files
```

## How to run apps after CAFAna code merge (Since Sep 23, 2022)

### Produce a state file

Below are examples of producing stat-only state files interactively without submitting a job for testing purposes:

```
cd PRISM/app

# FDFHC
MakePRISMPredInterps -o FDFHCState_stat_only.root -F-nu /dune/data/users/chasnip/OffAxisCAFs/FD_FHC_nonswap.root -Fe-nu /dune/data/users/chasnip/OffAxisCAFs/FD_FHC_nueswap.root -Ft-nu /dune/data/users/chasnip/OffAxisCAFs/FD_FHC_tauswap.root --bin-descriptor lep_default --no-fakedata-dials -A ELepEHadVisReco --UseSelection --syst-descriptor "nosyst"

# NDFHC
MakePRISMPredInterps -o NDFHCState_stat_only.root -N-nu /pnfs/dune/persistent/users/chasnip/NDCAF_OnAxisHadd/FHC/CAFv7_0m_132_FHC.root -A ELepEHadVisReco --bin-descriptor lep_default --syst-descriptor "nosyst" --no-fakedata-dials --UseSelection

# NDRHC
MakePRISMPredInterps -o NDRHCState_stat_only.root -N-nub root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr/dune/persistent/users/chasnip/NDCAF_OnAxisHadd/RHC/CAFv7_280kA_0m_118_RHC.root -A ELepEHadVisReco --bin-descriptor lep_default --syst-descriptor "nosyst" --no-fakedata-dials --UseSelection
```

#### 2D state file production on grid

Below is an example of submit a 2D (```ELepEHadVisReco```) state file production job without adding any systematic shifts.

Change ```"nosyst"``` to ```list:syst1:syst2:...``` to include systematics. More systematic options available in ```CAFAna/Systs/AnaSysts```.

Default binning is ```lep_default```, but can be ```prism_default``` as well.

Here is a reference of computing requirement for state file production:
| Type of state file             | Memory (GB) | Disk (GB) | Lifetime (h) |
| ------------------------------ | ----------- | --------- | ------------ |
| Stat only (ND or FD)           | 2           | 1         | 1            |
| Single systematic (ND or FD)   | 3           | 1         | 10           |
| All 25 flux systematics (ND)   | 8           | 1         | 18           |
| All 25 flux systematics (FD)   | 6           | 1         | 3            |

Here is a reference of computing requirement for hadd state files (especially hadd ND state files):
| Type of state file              | Memory (GB) | Disk (GB) | Lifetime (h) |
| ------------------------------- | ----------- | --------- | ------------ |
| Stat only (ND or FD)            | 1           | 1         | 1            |
| Single systematic (ND or FD)    | 4           | 1         | 3            |
| All 25 flux systematics (ND)    | 17          | 1         | 19           |
| All 25 flux systematics (ND+FD) | 20          | 2         | 1            |

*N.B.* It's recommended to split ND state files into three batches and add separately. Socket often timed out with too many ND state files.

```
cd PRISM/scripts/FermiGridPRISMScripts

# ND FHC
./FarmBuildPRISMInterps.sh -i /pnfs/dune/persistent/users/chasnip/NDCAF_OnAxisHadd/FHC --no-fakedata-dials -a ELepEHadVisReco --bin-descriptor lep_default --syst-descriptor "nosyst" -N -u

# ND RHC
./FarmBuildPRISMInterps.sh -i /pnfs/dune/persistent/users/chasnip/NDCAF_OnAxisHadd/RHC --no-fakedata-dials -a ELepEHadVisReco --bin-descriptor lep_default --syst-descriptor "nosyst" -N -b

# FD FHC
./FarmBuildPRISMInterps.sh -i /pnfs/dune/persistent/users/weishi/FDCAF/FHC/nonswap --no-fakedata-dials -a ELepEHadVisReco --bin-descriptor lep_default --syst-descriptor "nosyst" -F -u
./FarmBuildPRISMInterps.sh -i /pnfs/dune/persistent/users/weishi/FDCAF/FHC/nueswap --no-fakedata-dials -a ELepEHadVisReco --bin-descriptor lep_default --syst-descriptor "nosyst" -F -u
./FarmBuildPRISMInterps.sh -i /pnfs/dune/persistent/users/weishi/FDCAF/FHC/tauswap --no-fakedata-dials -a ELepEHadVisReco --bin-descriptor lep_default --syst-descriptor "nosyst" -F -u

# FD RHC
./FarmBuildPRISMInterps.sh -i /pnfs/dune/persistent/users/weishi/FDCAF/RHC/nonswap --no-fakedata-dials -a ELepEHadVisReco --bin-descriptor lep_default --syst-descriptor "nosyst" -F -b
./FarmBuildPRISMInterps.sh -i /pnfs/dune/persistent/users/weishi/FDCAF/RHC/nueswap --no-fakedata-dials -a ELepEHadVisReco --bin-descriptor lep_default --syst-descriptor "nosyst" -F -b
./FarmBuildPRISMInterps.sh -i /pnfs/dune/persistent/users/weishi/FDCAF/RHC/tauswap --no-fakedata-dials -a ELepEHadVisReco --bin-descriptor lep_default --syst-descriptor "nosyst" -F -b

# Add state files
./FarmHaddCafanaGrid.sh -i /pnfs/dune/persistent/users/weishi/CAFAnaInputs/StandardState/ELepEHadVisReco_lep_default_binning_TrueObs_fixed/StatOnly/Hadded
```

#### List of up-to-date state files:

1. 2D variable ```ELepEHadVisReco``` for osc fit, with ```lep_default``` binning (fewer hadronic energy bins), all 25 flux systematic (```flux_Nov17_<0-24>```), true observable HistAxis match "ELepEHadVisReco", size 1.6GB:

```
/pnfs/dune/persistent/users/weishi/CAFAnaInputs/StandardState/ELepEHadVisReco_lep_default_HalfHadbins/fluxsyst_Nov17/ELepEHadVisReco_lep_default_HalfHadbins_all_flux_systs_flux_Nov17_0-25_Hadded_State.root
```

2. 2D variable ```ELepEHadVisReco``` for osc fit, with ```lep_default``` binning (fewer hadronic energy bins), one single systematic (```flux_Nov17_0```), true observable HistAxis match "ELepEHadVisReco":

```
root://fndca1.fnal.gov:1094//pnfs/fnal.gov/usr/dune/persistent/users/weishi/CAFAnaInputs/StandardState/ELepEHadVisReco_lep_default_HalfHadbins/flux_Nov17_0/ELepEHadVisReco_lep_default_HalfHadbins_flux_Nov17_0_Hadded_State.root

/pnfs/dune/persistent/users/weishi/CAFAnaInputs/StandardState/ELepEHadVisReco_lep_default_HalfHadbins/flux_Nov17_<0-24>/ELepEHadVisReco_lep_default_HalfHadbins_flux_Nov17_0_Hadded_State.root
```

3. 2D variable ```ELepEHadVisReco``` for osc fit, with ```lep_default``` binning (fewer hadronic energy bins), stat-only, true observable HistAxis match "ELepEHadVisReco":

```
root://fndca1.fnal.gov:1094//pnfs/fnal.gov/usr/dune/persistent/users/weishi/CAFAnaInputs/StandardState/ELepEHadVisReco_lep_default_HalfHadbins/StatOnly/ELepEHadVisReco_lep_default_HalfHadbins_StatOnly_Hadded_State.root

/pnfs/dune/persistent/users/weishi/CAFAnaInputs/StandardState/ELepEHadVisReco_lep_default_HalfHadbins/StatOnly/ELepEHadVisReco_lep_default_HalfHadbins_StatOnly_Hadded_State.root
```

4. 2D variable ```ELepEHadVisReco``` for osc fit, with ```lep_default``` binning (hadronic energy binning too fine, over 80hr fit time), stat-only, true observable HistAxis match "ELepEHadVisReco":
```
root://fndca1.fnal.gov:1094//pnfs/fnal.gov/usr/dune/persistent/users/weishi/CAFAnaInputs/StandardState/ELepEHadVisReco_lep_default_binning_TrueObs_fixed/StatOnly/ELepEHadVisReco_lep_default_binning_TrueObs_fixed_StatOnly_Hadded_State.root
```

5. 2D variable ```ELepEHad``` (truth level) for osc fit, with ```prism_default``` binning, stat-only:
```
# Fit doesn't work well using this file: [WARN]: When un-runplan weighting histogram found bad bin content: 0 @ -29.75

/pnfs/dune/persistent/users/weishi/CAFAnaInputs/StandardState/ELepEHad_prism_default_binning/StatOnly/Hadded_AllChannel_State_ELepEHad_StatOnly.42052729.0.root

root://fndca1.fnal.gov:1094//pnfs/fnal.gov/usr/dune/persistent/users/weishi/CAFAnaInputs/StandardState/ELepEHad_prism_default_binning/StatOnly/Hadded_AllChannel_State_ELepEHad_StatOnly.42052729.0.root
```

### Produce PRISM predictions

PRISM predictions can only be done with an input state file.

```
export CAFANA_STAT_ERRS=1 (add to source script as required by CAFAnaCore)
kx509
voms-proxy-init -rfc -noregen -voms dune:/dune/Role=Analysis

PRISMPrediction --fcl ../../fcl/PRISM/PRISMPred_Grid.fcl
```

### Produce PRISM sensitivities

PRISM fits can only be done with an input state file.

Here is a reference of computing requirement for doing fits:
| Type of fit                     | Memory (GB) | Disk (GB)                                | Lifetime (h) |
| ------------------------------- | ----------- | ---------------------------------------- | ------------ |
| Stat only                       | 2           | 1                                        | 1            |
| Single systematic state file    | 2           | 1                                        | 23           |
| 25 flux systs, but only fit one | 35          | 1 (read state file from persistent pool) | 24-48        |

*N.B.*  Number of bins (hadronic/lepton) affect fit time, typically 4hrs with 9 hadronic energy bins and 15 lepton bins
This increases to over ~80hrs if hadronic bins double.

Example of doing PRISM fits:
```
export CAFANA_STAT_ERRS=1 (add to source script as required by CAFAnaCore)
kx509
voms-proxy-init -rfc -noregen -voms dune:/dune/Role=Analysis

# PRISM 1D fit interactively
PRISM_4Flavour_dChi2Scan --fcl ../../fcl/PRISM/PRISMOscScan_Grid.fcl --binx 1

# PRISM 2D fit interactively
PRISM_4Flavour_dChi2Scan --fcl ../../fcl/PRISM/PRISMOscScan_Grid.fcl --binx 1 --biny 1

# PRISM 1D fit on grid
# Read state file from persistent pool
./FarmCAFPRISMNodeScript.sh -c PRISM_1DScan_Commands.cmd
# Read state file from worker node
./FarmCAFPRISMNodeScript.sh -c PRISM_1DScan_Commands.cmd -i /pnfs/dune/persistent/users/weishi/CAFAnaInputs/StandardState/ELepEHadVisReco_lep_default_HalfHadbins/StatOnly/ELepEHadVisReco_lep_default_HalfHadbins_StatOnly_Hadded_State.root

# PRISM 2D fit on grid
# Read state file from persistent pool
./FarmCAFPRISMNodeScript.sh -c PRISM_2DScan_Commands.cmd
# Read state file from worker node
./FarmCAFPRISMNodeScript.sh -c PRISM_2DScan_Commands.cmd -i /pnfs/dune/persistent/users/weishi/CAFAnaInputs/StandardState/ELepEHadVisReco_lep_default_HalfHadbins/fluxsyst_Nov17/ELepEHadVisReco_lep_default_HalfHadbins_all_flux_systs_flux_Nov17_0-25_Hadded_State.root

# Missing proton fake data fit, set in PRISMOscScan_Grid.fcl:
use_fake_data: true
```

The --binx and --biny inputs for the fitting script are optional and are only used for when submitting many jobs in parallel to the grid.
