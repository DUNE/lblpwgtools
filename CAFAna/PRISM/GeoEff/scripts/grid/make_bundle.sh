#!/usr/bin/env bash
# Build a self-contained runtime bundle (tarball) for a GeoEff grid step.
# Run it with bash (do NOT source it):
#   bash scripts/grid/make_bundle.sh <step1|step2|step3> <output.tar.gz> [input-file ...]
if [[ "${BASH_SOURCE[0]}" != "${0}" ]]; then
  echo "make_bundle.sh must be run with bash, not sourced." >&2
  return 1
fi
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: bash scripts/grid/make_bundle.sh <step1|step2|step3> <output.tar.gz> [input-file ...]

Run from lblpwgtools/CAFAna/PRISM/GeoEff. Build the runtime files first with the
*standalone* GeoEff build, inside the fnal-wn-sl7 image and with the same
environment script the grid job will source:

  step 1:
    source scripts/env/setup_step1.sh
    cmake -S . -B build-step1 -DGEOEFF_BUILD_STEP3_OSC=OFF
    cmake --build build-step1 --target runGeoEffFDEvtSim -j

  step 3:
    source scripts/env/setup_step3.sh
    cmake -S . -B build-step3
    cmake --build build-step3 -j

The binaries are taken from build-<step>/ by default; set NDGEOEFF_BUILD_DIR to
override. Binaries from the full CAFAna build are refused, because they link
the CAFAna shared libraries, which are not in the bundle.

For step 2, set NDGEOEFF_PYTHON_ENV to a preinstalled Python target directory to embed it.
Create that environment with scripts/env/setup_python_env.sh, which installs
CPU-only Torch by default. Set NDGEOEFF_TORCH_INDEX_URL to a CUDA wheel index
only if the jobs need GPU support.
EOF
}

STEP="${1:-}"
OUT="${2:-}"
if [[ -z "${STEP}" || -z "${OUT}" ]]; then
  usage
  exit 2
fi
shift 2 || true

case "${STEP}" in
  step1) STEP_DIR=step1_hadron ;;
  step2) STEP_DIR=step2_muon ;;
  step3) STEP_DIR=step3_etrim ;;
  *)
    usage
    exit 2
    ;;
esac

# Top-level directory name inside the tarball; the run_*.sh scripts expect
# ${INPUT_TAR_DIR_LOCAL}/GeoEff.
BUNDLE_NAME=GeoEff

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
BUILD_DIR="${NDGEOEFF_BUILD_DIR:-${REPO_ROOT}/build-${STEP}}"
STAGE="$(mktemp -d "${TMPDIR:-/tmp}/ndgeoeff-bundle.XXXXXX")"
trap 'rm -rf "${STAGE}"' EXIT
DEST="${STAGE}/${BUNDLE_NAME}"

require_file() {
  if [[ ! -f "$1" ]]; then
    echo "Missing required file: $1" >&2
    exit 1
  fi
}

# Refuse binaries that need libraries the bundle does not ship (i.e. built
# as part of CAFAna rather than with the standalone GeoEff build).
require_standalone_binary() {
  if command -v readelf >/dev/null 2>&1 && \
     readelf -d "$1" 2>/dev/null | grep -q 'NEEDED.*libCAFAna'; then
    echo "$1 links CAFAna shared libraries (built inside CAFAna?)." >&2
    echo "Use the standalone GeoEff build instead, see: bash $0 --help" >&2
    exit 1
  fi
}

mkdir -p "${DEST}/scripts/env" "${DEST}/scripts/grid" "${DEST}/inputs"
cp -a "${REPO_ROOT}/scripts/env/setup_${STEP}.sh" "${DEST}/scripts/env/"
cp -a "${REPO_ROOT}/scripts/grid/${STEP_DIR}" "${DEST}/scripts/grid/"
# Documentation is not needed on the worker
rm -f "${DEST}/scripts/grid/${STEP_DIR}"/README*.md

case "${STEP}" in
  step1)
    require_file "${BUILD_DIR}/bin/runGeoEffFDEvtSim"
    require_file "${BUILD_DIR}/lib/libgeoEff.so"
    require_standalone_binary "${BUILD_DIR}/bin/runGeoEffFDEvtSim"
    mkdir -p "${DEST}/bin" "${DEST}/lib"
    cp -a "${BUILD_DIR}/bin/runGeoEffFDEvtSim" "${DEST}/bin/"
    cp -a "${BUILD_DIR}/lib/libgeoEff.so" "${DEST}/lib/"
    ;;
  step2)
    # Only the Python sources and the trained network are needed
    mkdir -p "${DEST}/app"
    cp -a "${REPO_ROOT}"/app/*.py "${DEST}/app/"
    cp -a "${REPO_ROOT}/app/assets" "${DEST}/app/"
    if [[ -n "${NDGEOEFF_PYTHON_ENV:-}" ]]; then
      if [[ ! -d "${NDGEOEFF_PYTHON_ENV}" ]]; then
        echo "NDGEOEFF_PYTHON_ENV is not a directory: ${NDGEOEFF_PYTHON_ENV}" >&2
        exit 1
      fi
      if [[ -d "${NDGEOEFF_PYTHON_ENV}/nvidia" ]]; then
        echo "Warning: NDGEOEFF_PYTHON_ENV contains CUDA libraries; the bundle will be much larger." >&2
      fi
      cp -a "${NDGEOEFF_PYTHON_ENV}" "${DEST}/python_env"
      find "${DEST}/python_env" -type d -name __pycache__ -prune -exec rm -rf {} +
    fi
    ;;
  step3)
    for app in \
      NtupleOutVetoAndTrimE_AssumeEqualEffAtAllOA_NOOscSpectrumNOCAFAna_FDRateAtND \
      NtupleOutVetoAndTrimE_AssumeEqualEffAtAllOA_WithOscSpectrumNOCAFAna_FDRateAtND \
      NtupleOutVetoAndTrimE_AllOApos_WithOscSpectrumNOCAFAna_FDRateAtND; do
      require_file "${BUILD_DIR}/bin/${app}"
      require_standalone_binary "${BUILD_DIR}/bin/${app}"
    done
    mkdir -p "${DEST}/bin" "${DEST}/data/step3"
    cp -a "${BUILD_DIR}/bin/NtupleOutVetoAndTrimE_"* "${DEST}/bin/"
    # Ship only the data files the step 3 apps actually open (the
    # "data/step3/..." paths hard-coded in app/*.C), not all of data/step3.
    data_files="$(grep -ho '"data/step3/[^"]*"' "${REPO_ROOT}"/app/NtupleOutVetoAndTrimE_*.C | tr -d '"' | sort -u)"
    for data_file in ${data_files}; do
      require_file "${REPO_ROOT}/${data_file}"
      cp -a "${REPO_ROOT}/${data_file}" "${DEST}/data/step3/"
    done
    ;;
esac

for input_file in "$@"; do
  if [[ ! -f "${input_file}" ]]; then
    echo "Input file not found: ${input_file}" >&2
    exit 1
  fi
  cp -a "${input_file}" "${DEST}/inputs/"
done

mkdir -p "$(dirname "${OUT}")"
tar -czf "${OUT}" -C "${STAGE}" "${BUNDLE_NAME}"

echo "Wrote ${OUT}"
du -h "${OUT}"
tar -tzf "${OUT}" | sed -n '1,40p'
