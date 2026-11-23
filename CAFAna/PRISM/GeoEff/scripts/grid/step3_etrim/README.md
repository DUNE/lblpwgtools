# Grid submissions on FNAL machines

Write input file paths into `.txt` files:

```bash 
ls -d "/pnfs/dune/persistent/users/path/to/hadron/files"/* | sed "s\/pnfs\root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr\g" > myOutputFDGeoEffHadron.txt
ls -d "/pnfs/dune/persistent/users/path/to/muon/files"/* | sed "s\/pnfs\root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr\g" > myOutputFDGeoEffMu.txt
```

Make tarballs:

```bash
tar -czvf TarEtrimAnalysis.tar.gz DUNE_ND_GeoEff setupNDEff-grid.sh myOutputFDGeoEffHadron.txt myOutputFDGeoEffMu.txt
```

>[!WARNING]
>This command assumes code is in `DUNE_ND_GeoEff` directory. 

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
>Modify output path in `run_NDcomEff.sh`!


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
