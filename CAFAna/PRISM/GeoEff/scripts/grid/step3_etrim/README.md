# Grid submissions on FNAL machines

Build the runtime files once in the same `fnal-wn-sl7` Singularity image used for submission:

```bash
cmake -S . -B build-step3 -DNDGEOEFF_BUILD_STEP1=OFF -DNDGEOEFF_BUILD_STEP3=ON
cmake --build build-step3 -j
```

Write input file paths into `.txt` files:

```bash
ls -d "/pnfs/dune/persistent/users/path/to/hadron/files"/* | sed "s\/pnfs\root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr\g" > myOutputFDGeoEffHadron.txt
ls -d "/pnfs/dune/persistent/users/path/to/muon/files"/* | sed "s\/pnfs\root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr\g" > myOutputFDGeoEffMu.txt
```

Create the runtime bundle:

```bash
export NDGEOEFF_BUILD_DIR=build-step3
source scripts/grid/make_bundle.sh step3 TarEtrimAnalysis.tar.gz myOutputFDGeoEffHadron.txt myOutputFDGeoEffMu.txt
```

The bundle contains the step 3 binaries, `data/step3`, the setup scripts, and the input lists. Worker jobs do not compile the code or copy the full repository. At run time the job creates a `data` symlink to the bundle so the fixed `data/step3/...` paths continue to work.

Submit jobs:

```bash
jobsub_submit -G dune -N 9083 --memory=8GB --disk=1GB --expected-lifetime=3h --cpu=1 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC,OFFSITE --tar_file_name=dropbox:///exp/dune/app/users/amunoz1/test/NDEff/submit2Grid/SubmitEtrimAnalysisJobs/TarEtrimAnalysis.tar.gz --use-cvmfs-dropbox -l '+SingularityImage=\"/cvmfs/singularity.opensciencegrid.org/fermilab/fnal-wn-sl7:latest\"' --append_condor_requirements='(TARGET.HAS_Singularity==true&&TARGET.HAS_CVMFS_dune_opensciencegrid_org==true&&TARGET.HAS_CVMFS_larsoft_opensciencegrid_org==true&&TARGET.CVMFS_dune_opensciencegrid_org_REVISION>=1105&&TARGET.HAS_CVMFS_fifeuser1_opensciencegrid_org==true&&TARGET.HAS_CVMFS_fifeuser2_opensciencegrid_org==true&&TARGET.HAS_CVMFS_fifeuser3_opensciencegrid_org==true&&TARGET.HAS_CVMFS_fifeuser4_opensciencegrid_org==true)' file:///exp/dune/app/users/amunoz1/test/NDEff/submit2Grid/SubmitEtrimAnalysisJobs/run_NDEff_autogrid.sh
```

>[!NOTE]
>To run with oscillation spectrum use `run_NDEff_autogridOscSpectrum.sh`

For all positions use:

```bash
jobsub_submit -G dune -N 9055 --memory=8GB --disk=1GB --expected-lifetime=3h --cpu=1 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC,OFFSITE --tar_file_name=dropbox:///exp/dune/app/users/amunoz1/test/NDEff/submit2Grid/SubmitEtrimAnalysisAllPosJobs/TarEtrimAnalysis.tar.gz --use-cvmfs-dropbox -l '+SingularityImage=\"/cvmfs/singularity.opensciencegrid.org/fermilab/fnal-wn-sl7:latest\"' --append_condor_requirements='(TARGET.HAS_Singularity==true&&TARGET.HAS_CVMFS_dune_opensciencegrid_org==true&&TARGET.HAS_CVMFS_larsoft_opensciencegrid_org==true&&TARGET.CVMFS_dune_opensciencegrid_org_REVISION>=1105&&TARGET.HAS_CVMFS_fifeuser1_opensciencegrid_org==true&&TARGET.HAS_CVMFS_fifeuser2_opensciencegrid_org==true&&TARGET.HAS_CVMFS_fifeuser3_opensciencegrid_org==true&&TARGET.HAS_CVMFS_fifeuser4_opensciencegrid_org==true)' file:///exp/dune/app/users/amunoz1/test/NDEff/submit2Grid/SubmitEtrimAnalysisAllPosJobs/run_NDEff_autogridOscSpectrum_AllPos.sh
```

>[!CAUTION]
>Modify paths in the submission command and the output path in the selected `run_NDEff_autogrid*.sh` wrapper.

Eventually move output to more permanent location:

```bash
cp /pnfs/dune/scratch/users/amunoz1/test/FDGeoEffinND/Etrim/Osc/* /pnfs/dune/persistent/users/amunoz1/test/FDGeoEffinND/Etrim/Osc
cp /pnfs/dune/scratch/users/amunoz1/test/FDGeoEffinND/Etrim/NoOsc/* /pnfs/dune/persistent/users/amunoz1/test/FDGeoEffinND/Etrim/NoOsc
```

Sum Etrim histos, when you have multiple output files from the previous step:

```bash
root -q 'SumEtrimHistos.C("/pnfs/dune/persistent/users/path/to/outputs/",{"hist_visEnuFDEnergy", "hist_EnuFDEnergy", "Oschist_EnuFDEnergy", "LepMomTot","hist_FDTotEnergy","hist_muEdep","hist_muTrackLength","AllThrownEventsVsOAPosVsTotalETrim_FDEvt_","SelectedEventsTwoDHisto_FDEvt_"})'
```

## Comparison PRISM prediction

Use the code `macros/step3_etrim/PlotAllOAPosWithCAFLikeCutClean.C` to plot the comparison plots between geometric efficiency method and the standard PRISM prediction. The file containing the standard prediction is `data/step3/PRISMPred_ERecFromDepWithNeutronEnergyAndTrueEmu_NoSysts_NoOsc_FlatRunPlan_AllOAPos_correcPerFileWeight_NDEffVsEreco.root`.
