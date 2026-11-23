# Grid submissions on FNAL machines

The code now lives in `lblpwgtools/CAFAna/PRISM/GeoEff`. Work from the directory
that *contains* your `lblpwgtools` checkout, and copy the grid scripts there:

```bash
cp lblpwgtools/CAFAna/PRISM/GeoEff/scripts/grid/step3_etrim/*.sh .
```

The worker node builds only the GeoEff sub-project (standalone CMake build of
`CAFAna/PRISM/GeoEff`). The oscillated apps take the default oscillation
parameters from `lblpwgtools/CAFAna/Analysis/Calcs.cxx`, which is compiled
directly, so those files have to be in the tarball too.

Write input file paths into `.txt` files:

```bash 
ls -d "/pnfs/dune/persistent/users/path/to/hadron/files"/* | sed "s\/pnfs\root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr\g" > myOutputFDGeoEffHadron.txt
ls -d "/pnfs/dune/persistent/users/path/to/muon/files"/* | sed "s\/pnfs\root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr\g" > myOutputFDGeoEffMu.txt
```

Make tarballs:

```bash
tar -czvf TarEtrimAnalysis.tar.gz --exclude='build*' --exclude='__pycache__' \
  lblpwgtools/CAFAna/PRISM/GeoEff \
  lblpwgtools/CAFAna/Analysis/Calcs.h lblpwgtools/CAFAna/Analysis/Calcs.cxx \
  lblpwgtools/CAFAna/Analysis/CalcsVars.h lblpwgtools/CAFAna/Core/OscCalcFwdDeclare.h \
  setupNDEff-grid.sh myOutputFDGeoEffHadron.txt myOutputFDGeoEffMu.txt
```

>[!WARNING]
>This command assumes the code is in `lblpwgtools/CAFAna/PRISM/GeoEff` relative to the current directory.

>[!IMPORTANT]
>The step 3 apps read `data/step3/*.root` (coefficients, splines) relative to
>`CAFAna/PRISM/GeoEff`. These ROOT files are ignored by git (`CAFAna/.gitignore`),
>so make sure they are present in your working copy before making the tarball.

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
>Modify paths in submission command!
>Modify the output path (`OUTDIR`) in the `run_NDEff_autogrid*.sh` script you submit!


Eventually move output to more permanent location:

```bash 
cp /pnfs/dune/scratch/users/amunoz1/test/FDGeoEffinND/Etrim/Osc/* /pnfs/dune/persistent/users/amunoz1/test/FDGeoEffinND/Etrim/Osc
cp /pnfs/dune/scratch/users/amunoz1/test/FDGeoEffinND/Etrim/NoOsc/* /pnfs/dune/persistent/users/amunoz1/test/FDGeoEffinND/Etrim/NoOsc
```

Sum Etrim histos, when you have multiple output files from the previous step:

```bash
root -q 'lblpwgtools/CAFAna/PRISM/GeoEff/macros/step3_etrim/SumEtrimHistos.C("/pnfs/dune/persistent/users/path/to/outputs/",{"hist_visEnuFDEnergy", "hist_EnuFDEnergy", "Oschist_EnuFDEnergy", "LepMomTot","hist_FDTotEnergy","hist_muEdep","hist_muTrackLength","AllThrownEventsVsOAPosVsTotalETrim_FDEvt_","SelectedEventsTwoDHisto_FDEvt_"})'
```

## Comparison PRISM prediction

Use the code `CAFAna/PRISM/GeoEff/macros/step3_etrim/PlotAllOAPosWithCAFLikeCutClean.C` to plot the comparison plots between geometric efficiency method and the standard PRISM prediction. The file containing the standard prediction is `CAFAna/PRISM/GeoEff/data/step3/PRISMPred_ERecFromDepWithNeutronEnergyAndTrueEmu_NoSysts_NoOsc_FlatRunPlan_AllOAPos_correcPerFileWeight_NDEffVsEreco.root`. 
