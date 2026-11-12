#!/bin/bash

# not sure I need to source setup_dune.sh
source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh   # tweak 1
# source the environment (only if not already done)
if [ -z ${CAFANA+x} ]; then
  echo "[debug] INPUT_TAR_DIR_LOCAL=${INPUT_TAR_DIR_LOCAL}"; find ${INPUT_TAR_DIR_LOCAL} -name CAFAnaEnv.sh
  export CAFANA=${INPUT_TAR_DIR_LOCAL}
  source ${CAFANA}/CAFAnaEnv.sh
  echo "If running interactively, run 'source /nashome/l/lorenzog/PRISM_SW/lblpwgtools/CAFAna/build/Linux/CAFAnaEnv.sh' first."
fi
echo "[debug] CAFANA=${CAFANA}"
set -e  # Exit immediately if a command fails

# get a token
# htgettoken -a htvaultprod.fnal.gov -i dune

# ==========================
# 1. CONFIGURATION VARIABLES
# ==========================
  # inputs read over xrootd; output written locally then copied to scratch dCache

INPUTDIR="root://fndca1.fnal.gov:1094//pnfs/fnal.gov/usr/dune/scratch/users/lorenzog/CAFFiles"
NDFHCPNFSDIR="root://fndca1.fnal.gov:1094//pnfs/fnal.gov/usr/dune/persistent/users/chasnip/NDCAF_OnAxisHadd/FHC"
NDRHCPNFSDIR="root://fndca1.fnal.gov:1094//pnfs/fnal.gov/usr/dune/persistent/users/chasnip/NDCAF_OnAxisHadd/RHC"
OUTPUTDIR="."                                              # job scratch
OUTPUT_PNFS="/pnfs/dune/scratch/users/lorenzog/prism/state" # copy-out target

# INPUTDIR="/exp/dune/data/users/icaracas/CAFFiles/FDCAFs"
# OUTPUTDIR="/exp/dune/data/users/lorenzog/prism/state" # Using $USER is safer for HPC environments
# NDFHCPNFSDIR="/pnfs/dune/persistent/users/chasnip/NDCAF_OnAxisHadd/FHC"
# NDRHCPNFSDIR="/pnfs/dune/persistent/users/chasnip/NDCAF_OnAxisHadd/RHC"

# Analysis Parameters
LIST_OSYSTS="nosyst"          # No systematic uncertainties
FAKE_DATA="MissingProtonFakeData_pos"
ANA_BINNING="prism_default"
AVA_VAR="EnuReco"

stage_out() {   # $1 = output filename produced in $OUTPUTDIR
    ifdh cp -D "${OUTPUTDIR}/$1" "${OUTPUT_PNFS}/"
}

# do pwd check
echo "[debug] Current working directory: $(pwd)"
# show contents of working directory
echo "[debug] Contents of working directory:"
ls -l

# ==========================
# 2. FUNCTION DEFINITIONS
# ==========================
run_fd_nu() {
    echo ">>> Running FD nu..."
    MakePRISMPredInterps -o ${OUTPUTDIR}/prism_state_FD_nu_batch.root \
        -F-nu ${INPUTDIR}/FDCAFs/OffAxisCAFs/FD_FHC_nonswap.root \
        -Fe-nu ${INPUTDIR}/FDCAFs/OffAxisCAFs/FD_FHC_nueswap.root \
        -Ft-nu ${INPUTDIR}/FDCAFs/OffAxisCAFs/FD_FHC_tauswap.root \
        --bin-descriptor ${ANA_BINNING} \
        --syst-descriptor ${LIST_OSYSTS} \
        -A ${AVA_VAR} \
        --PRISM-fake-data ${FAKE_DATA} \
        --no-fakedata-dials \
        --UseSelection
    
    stage_out "prism_state_FD_nu_batch.root"
    echo ">>> FD nu completed."
}

run_fd_nub() {
    echo ">>> Running FD nub..."
    MakePRISMPredInterps -o ${OUTPUTDIR}/prism_state_FD_nub_batch.root \
        -F-nub ${INPUTDIR}/FDCAFs/OffAxisCAFs/FD_RHC_nonswap.root \
        -Fe-nub ${INPUTDIR}/FDCAFs/OffAxisCAFs/FD_RHC_nueswap.root \
        -Ft-nub ${INPUTDIR}/FDCAFs/OffAxisCAFs/FD_RHC_tauswap.root \
        --bin-descriptor ${ANA_BINNING} \
        --syst-descriptor ${LIST_OSYSTS} \
        -A ${AVA_VAR} \
        --PRISM-fake-data ${FAKE_DATA} \
        --no-fakedata-dials \
        --UseSelection
    stage_out "prism_state_FD_nub_batch.root"
    echo ">>> FD nub completed."
}

run_nd_nu() {
    echo ">>> Running ND nu..."
    MakePRISMPredInterps -o ${OUTPUTDIR}/prism_state_ND_nu_batch.root \
        -N-nu "${INPUT_TAR_DIR_LOCAL}/CAFv7_file_list_ND_FHC_xrootd.txt" \
        --bin-descriptor ${ANA_BINNING} \
        --syst-descriptor ${LIST_OSYSTS} \
        --no-fakedata-dials \
        -A ${AVA_VAR} \
        --PRISM-fake-data ${FAKE_DATA} \
        --UseSelection
    stage_out "prism_state_ND_nu_batch.root"
    echo ">>> ND nu completed."
}

run_nd_nub() {
    echo ">>> Running ND nub..."
    MakePRISMPredInterps -o ${OUTPUTDIR}/prism_state_ND_nub_batch.root \
        -N-nub "${INPUT_TAR_DIR_LOCAL}/CAFv7_file_list_ND_RHC_xrootd.txt" \
        --bin-descriptor ${ANA_BINNING} \
        --syst-descriptor ${LIST_OSYSTS} \
        --no-fakedata-dials \
        -A ${AVA_VAR} \
        --PRISM-fake-data ${FAKE_DATA} \
        --UseSelection
    stage_out "prism_state_ND_nub_batch.root"
    echo ">>> ND nub completed."
}

# ==========================
# 3. MAIN WORKFLOW
# ==========================
# Check if executable exists
if ! command -v MakePRISMPredInterps &> /dev/null; then
    echo "Error: MakePRISMPredInterps not found in PATH."
    exit 1
fi


# Select configuration based on first argument
case "${1}" in
    fd_nu)
        run_fd_nu
        ;;
    fd_nub)
        run_fd_nub
        ;;
    nd_nu)
        run_nd_nu
        ;;
    nd_nub)
        run_nd_nub
        ;;
    all)
        echo ">>> Running all configurations sequentially..."
        run_fd_nu
        run_fd_nub
        run_nd_nu
        run_nd_nub
        echo ">>> All configurations completed."
        ;;
    *)
        echo "Usage: $0 {fd_nu|fd_nub|nd_nu|nd_nub|all}"
        echo "Example: ./BuildPredInterps.sh fd_nu"
        exit 1
        ;;
esac
