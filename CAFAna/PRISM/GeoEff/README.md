# DUNE ND Geometric Efficiency and Etrim/PRISM Analysis

This directory (`lblpwgtools/CAFAna/PRISM/GeoEff`) contains the three-stage DUNE ND
geometric-efficiency workflow, imported from the former standalone
[DUNE_ND_GeoEff](https://github.com/anfe29/DUNE_ND_GeoEff) repository:

1. **Step 1 — hadron geometric efficiency** (`app/`, `include/`, `src/`)
2. **Step 2 — muon/combined efficiency** (`app/`)
3. **Step 3 — Etrim/PRISM event-rate analysis** (`app/`, `include/`)

The layout is language-oriented. Executables and Python code live in the flat
`app/` directory; shared C++ headers and sources live under `include/` and
`src/`; ROOT macros live under `macros/`; operational scripts live under
`scripts/`; and step data lives under `data/step1/` and `data/step3/`.

Unless stated otherwise, all commands below are run from
`lblpwgtools/CAFAna/PRISM/GeoEff`.

>[!NOTE]
>The default oscillation parameters used by the step 3 oscillated apps
>(`ana::DefaultOscCalc()`) now come from CAFAna itself,
>`lblpwgtools/CAFAna/Analysis/Calcs.cxx`. The old private copy of `Calcs.cxx`
>was identical and has been removed. Changing the parameters there changes them
>for CAFAna/PRISM as well, which keeps both sides of the comparison consistent.

>[!IMPORTANT]
>The ROOT files in `data/step1/` and `data/step3/` are ignored by git
>(`CAFAna/.gitignore` ignores `*.root`). Copy them from the old DUNE_ND_GeoEff
>repository (or wherever they are kept) into `data/` before running steps 1/3.

## Environment

Clone lblpwgtools:

```bash
git clone git@github.com:DUNE/lblpwgtools.git
cd lblpwgtools/CAFAna/PRISM/GeoEff
```

If working on FNAL computers start an Apptainer:

```bash
/cvmfs/oasis.opensciencegrid.org/mis/apptainer/current/bin/apptainer shell --shell=/bin/bash -B /cvmfs,/exp,/nashome,/pnfs/dune,/opt,/run/user,/etc/hostname,/etc/hosts,/etc/krb5.conf --ipc --pid /cvmfs/singularity.opensciencegrid.org/fermilab/fnal-dev-sl7:latest
```

There are two ways to build the code (see [Build](#build)):

- **with CAFAna**: GeoEff is built automatically as part of lblpwgtools. Use the
  CAFAna environment (`standalone_configure_and_build.sh -u`, then
  `source CAFAna/build/Linux/CAFAnaEnv.sh`).
- **standalone**: only this directory is built, using the per-step environment
  scripts in `scripts/env/`. These set up CMake, GCC, Eigen, ROOT 6.28, and Python 3
  (plus OscLib for step 3). This is what the grid jobs do.

>[!NOTE]
>Run every step from an Apptainer with their respective environment scripts for the purpose having a consistent environment for testing and debugging.

## Build

### As part of CAFAna

`CAFAna/PRISM/CMakeLists.txt` adds this directory, so the normal lblpwgtools build
also builds GeoEff:

```bash
cd lblpwgtools/CAFAna
./standalone_configure_and_build.sh -r -u -j 4
source build/Linux/CAFAnaEnv.sh
```

This builds `libgeoEff.so`, `runGeoEffFDEvtSim` and the three
`NtupleOutVetoAndTrimE_*` apps, and installs them to `build/Linux/{lib,bin}`. The
oscillated step 3 apps link against `CAFAnaAnalysis`.

### Standalone (without CAFAna)

```bash
source scripts/env/setup_step1.sh      # or setup_step3.sh for the step 3 apps
cmake -S . -B build
cmake --build build -j
```

The executables are placed in `build/bin/` and the libraries in `build/lib/`.
ROOT and Eigen are required. The oscillated step 3 apps are built only if OscLib is
found (`OSCLIB_INC`/`OSCLIB_LIB`, set by `setup_step3.sh`); they compile
`../../Analysis/Calcs.cxx` directly. Pass `-DGEOEFF_BUILD_STEP3_OSC=OFF` to skip them.

### Options (both modes)

| Option | Default | Builds |
|---|---|---|
| `GEOEFF_BUILD_EDEPSIM` | `OFF` | `runGeoEffEDepSimBeamDir`; runs ROOT `MakeProject` on `data/step1/EDepSimForMakeProject_20200424.root` |
| `GEOEFF_BUILD_PYTHON` | `OFF` | `pyGeoEff` Python bindings; needs an installed pybind11 (>= 2.10 for Python >= 3.11) |
| `GEOEFF_BUILD_STEP3_OSC` | `ON` | standalone only: the two oscillated step 3 apps |

In the commands below, `build/bin/` refers to the standalone build. With the CAFAna
build the executables are on your `PATH` after sourcing `CAFAnaEnv.sh`.

## Step 1 — hadron geometric efficiency

After starting an Apptainer source environment:

```bash
source scripts/env/setup_step1.sh
```

Build code with cmake (standalone, step 1 only):
```bash
cmake -S . -B build -DGEOEFF_BUILD_STEP3_OSC=OFF
cmake --build build --target runGeoEffFDEvtSim -j
```

Run the code:
```bash
build/bin/runGeoEffFDEvtSim /path/to/inputFDntuple.root
```

This writes `Output_FDGeoEff.root` in the current directory.

To calculate the FD hadron containment efficiency from that output:

```bash
root -l -b -q macros/step1_hadron/FDEffCalc.C
```

Grid submission scripts can be found [here](scripts/grid/step1_hadron/).

## Step 2 — muon/combined efficiency

After starting an Apptainer source environment:

```bash
source scripts/env/setup_step2.sh
```

Install the Python dependencies into `my_python_env` (the setup script adds it
to `PYTHONPATH`):

```bash
bash scripts/env/setup_python_env.sh
```

Run the FD or ND code with:
```bash
python app/make_tree_fd.py InputRootFile.root
python app/make_tree_nd.py InputCAFFile.root
```

The ND script writes `Output_NDGeoEff.root`; the FD script writes
`Output_FDGeoEff.root`. The trained network is bundled at
`app/assets/muonEff30.nn`.

Grid and Slurm scripts are located [here](scripts/grid/step2_muon/).

## Step 3 — Etrim/PRISM analysis

The step 3 executables read their inputs with paths relative to this directory
(`data/step3/FileWithCoeffsNuMu*.root`, `data/step3/Splines_FDEventRateAtND*.root`)
and write their outputs into the current working directory. **Run them from
`lblpwgtools/CAFAna/PRISM/GeoEff`.**

After starting an Apptainer source environment:

```bash
source scripts/env/setup_step3.sh
```

Build the necessary files (standalone):
```bash
cmake -S . -B build
cmake --build build -j
```

Assuming equal effect on all off-axis positions (this might be depreciated), run with:

```bash
build/bin/NtupleOutVetoAndTrimE_AssumeEqualEffAtAllOA_NOOscSpectrumNOCAFAna_FDRateAtND HadronOutputFile  MuonOutputFile
```

Or with osccillations:
```bash
build/bin/NtupleOutVetoAndTrimE_AssumeEqualEffAtAllOA_WithOscSpectrumNOCAFAna_FDRateAtND HadronOutputFile  MuonOutputFile
```

With all off-axis positions:
```bash
build/bin/NtupleOutVetoAndTrimE_AllOApos_WithOscSpectrumNOCAFAna_FDRateAtND HadronOutputFile  MuonOutputFile
```

Available executables:

- `NtupleOutVetoAndTrimE_AssumeEqualEffAtAllOA_NOOscSpectrumNOCAFAna_FDRateAtND`
- `NtupleOutVetoAndTrimE_AssumeEqualEffAtAllOA_WithOscSpectrumNOCAFAna_FDRateAtND`
- `NtupleOutVetoAndTrimE_AllOApos_WithOscSpectrumNOCAFAna_FDRateAtND`

Analysis macros are in `macros/step3_etrim/`. Grid submission scripts are [here](scripts/grid/step3_etrim/).

## Macros

>[!WARNING]
>TODO
>Write READMEs to explain the macros in `macros/`.

## What was not migrated

From the standalone DUNE_ND_GeoEff repository, the following were left behind:
the standalone top-level CMake files and `app/Makefile` (replaced by
`CMakeLists.txt` here), the vendored `pybind11/` submodule (use an installed
pybind11 instead), `legacy/`, and `tests/`.

## Additional documentation

The detailed physics workflow written by Ionna is described in [documentation](doc/README.md). Stage-specific grid submission
notes are in [Step1](scripts/grid/step1_hadron/README.md),
[Step2](scripts/grid/step2_muon/README.md), and [Step3](scripts/grid/step3_etrim/README.md).
