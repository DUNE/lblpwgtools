# DUNE ND Geometric Efficiency and Etrim/PRISM Analysis

This repository contains the three-stage DUNE ND geometric-efficiency workflow:

1. **Step 1 — hadron geometric efficiency** (`app/`, `include/`, `src/`)
2. **Step 2 — muon/combined efficiency** (`app/`)
3. **Step 3 — Etrim/PRISM event-rate analysis** (`app/`, `include/`, `src/`)

The layout is language-oriented. Executables and Python code live in the flat
`app/` directory; shared C++ headers and sources live under `include/` and
`src/`; ROOT macros live under `macros/`; operational scripts live under
`scripts/`; and step data lives under `data/step1/` and `data/step3/`.

## Environment

Starting from the working directory, clone the current repository:

```bash
git clone -b develop git@github.com:anfe29/DUNE_ND_GeoEff.git
```

If working on FNAL computers start an Apptainer:

```bash
/cvmfs/oasis.opensciencegrid.org/mis/apptainer/current/bin/apptainer shell --shell=/bin/bash -B /cvmfs,/exp,/nashome,/pnfs/dune,/opt,/run/user,/etc/hostname,/etc/hosts,/etc/krb5.conf --ipc --pid /cvmfs/singularity.opensciencegrid.org/fermilab/fnal-dev-sl7:latest
```

For each step, source the appropriate script from `scripts/env/`.

This sets up CMake, GCC, Eigen, ROOT 6.28, and Python 3. 

>[!NOTE] 
>Run every step from an Apptainer with their respective environment scripts for the purpose having a consistent environment for testing and debugging.

## Step 1 — hadron geometric efficiency

After starting an Apptainer source environment:

```bash
source scripts/env/setup_step1.sh
```

Build code with cmake.
```bash
cmake -S . -B build -DNDGEOEFF_BUILD_STEP3=OFF
cmake --build build --target runGeoEffFDEvtSim -j
```

Run the code in the `build` directory:
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

Step 3 reads and writes several files by relative name, so run the executables
from `data/step3/`:

After starting an Apptainer source environment:

```bash
source scripts/env/setup_step3.sh
```

Build the necessary files:
```bash
cmake -S . -B build-step3 \
  -DNDGEOEFF_BUILD_STEP1=OFF \
  -DNDGEOEFF_BUILD_STEP3=ON \
  -DNDGEOEFF_BUILD_PYTHON=OFF
cmake --build build-step3 -j
```

Assuming equal effect on all off-axis positions (this might be depreciated), run with:
 
```bash
build-step3/bin/NtupleOutVetoAndTrimE_AssumeEqualEffAtAllOA_NOOscSpectrumNOCAFAna_FDRateAtND HadronOutputFile  MuonOutputFile
```

Or with osccillations:
```bash 
build-step3/bin/NtupleOutVetoAndTrimE_AssumeEqualEffAtAllOA_WithOscSpectrumNOCAFAna_FDRateAtND HadronOutputFile  MuonOutputFile
```

With all off-axis positions:
```bash
build-step3/bin/NtupleOutVetoAndTrimE_AllOApos_WithOscSpectrumNOCAFAna_FDRateAtND HadronOutputFile  MuonOutputFile
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

## Tests

>[!WARNING] 
>Under Construction

```bash
python3 -m pytest tests/unit
```

See `tests/integration/README.md` for the full three-stage smoke-test procedure.

## Build

Step 1 (shared library and hadron-efficiency apps):

```bash
cmake -S . -B build -DNDGEOEFF_BUILD_STEP3=OFF
cmake --build build -j
```

The executables are placed in `build/bin/` and the libraries in `build/lib/`.

The optional `pyGeoEff` Python bindings are disabled by default. Enable them with
`-DNDGEOEFF_BUILD_PYTHON=ON`; this requires a pybind11 version that supports your
Python (pybind11 >= 2.10 for Python >= 3.11).

Step 3 (requires ROOT and OscLib):

```bash
source scripts/env/setup_step3.sh
cmake -S . -B build-step3 \
  -DNDGEOEFF_BUILD_STEP1=OFF \
  -DNDGEOEFF_BUILD_STEP3=ON \
  -DNDGEOEFF_BUILD_PYTHON=OFF
cmake --build build-step3 -j
```

The original step 3 `Makefile` is kept at `app/Makefile` as a fallback.

## Legacy files

Duplicated and experimental files from the previous layout are preserved under
`legacy/`. They are not part of the supported build.

## Additional documentation

The detailed physics workflow written by Ionna is described in [documentation](doc/README.md). Stage-specific legacy grid submission
notes are in [Step1](scripts/grid/step1_hadron/README.md),
[Step2](scripts/grid/step2_muon/README.md), and [Step3](scripts/grid/step3_etrim/README.md).
