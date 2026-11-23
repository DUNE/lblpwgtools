> Paths in this document refer to the legacy `Grid/` layout. In the current layout the step 3 sources are under `app/`, headers under `include/`, macros under `macros/step3_etrim/`, input data under `data/step3/`, and these scripts under `scripts/grid/step3_etrim/`.

# Instructions to Run EtrimAnalysis Interactively on Fermi Grid

## 1. open an SL7 Container 
```
/cvmfs/oasis.opensciencegrid.org/mis/apptainer/current/bin/apptainer shell --shell=/bin/bash -B /cvmfs,/exp,/nashome,/pnfs/dune,/opt,/run/user,/etc/hostname,/etc/hosts,/etc/krb5.conf --ipc --pid /cvmfs/singularity.opensciencegrid.org/fermilab/fnal-dev-sl7:latest
```

## 2. (First time only) Setup directory:

  2.1 Move to your directory cd ```/exp/dune/app/users/$USER/NDEff```

  2.2 Make directory for muon code: ```mkdir EtrimAnalysis```

  2.3 Move to muon code directory: ```cd EtrimAnalysis```

  2.4 Clone muon code branch: ```git clone -b EtrimAnalysis https://github.com/AllisonSchroeder/DUNE_ND_GeoEff.git```

## 3. Move to EtrimAnalysis Directory
```
cd  /path/to/DUNE_ND_GeoEff
```
## 4. Setup
```
source scripts/env/setup_step3.sh
```
(re)compile code
```
make
```
## 5. Run with or without oscillations
for no oscillations:
```
./NtupleOutVetoAndTrimE_AssumeEqualEffAtAllOA_NOOscSpectrumNOCAFAna_FDRateAtND HadronOutputFile  MuonOutputFile
```
for oscillations:
```
./NtupleOutVetoAndTrimE_AssumeEqualEffAtAllOA_WithOscSpectrumNOCAFAna_FDRateAtND HadronOutputFile  MuonOutputFile
```
where HadronOutputFile is the output file from the Hadron Geometric Efficiency step (see readme in master branch) and MuonOutputFile is the output file from the Muon Geometric Efficiency step (see readme in MuonCode branch)
