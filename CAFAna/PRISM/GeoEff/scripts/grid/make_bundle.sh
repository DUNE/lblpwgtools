#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: scripts/grid/make_bundle.sh <step1|step2|step3> <output.tar.gz> [input-file ...]

Build the runtime files first. For step 1:
  cmake -S . -B build -DNDGEOEFF_BUILD_STEP1=ON
  cmake --build build --target runGeoEffFDEvtSim -j

For step 3:
  cmake -S . -B build-step3 -DNDGEOEFF_BUILD_STEP1=OFF -DNDGEOEFF_BUILD_STEP3=ON
  cmake --build build-step3 -j

Set NDGEOEFF_BUILD_DIR to override the build directory.
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

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
BUILD_DIR="${NDGEOEFF_BUILD_DIR:-${REPO_ROOT}/build}"
STAGE="$(mktemp -d "${TMPDIR:-/tmp}/ndgeoeff-bundle.XXXXXX")"
trap 'rm -rf "${STAGE}"' EXIT
DEST="${STAGE}/DUNE_ND_GeoEff"

require_file() {
  if [[ ! -f "$1" ]]; then
    echo "Missing required file: $1" >&2
    exit 1
  fi
}

mkdir -p "${DEST}/scripts/env" "${DEST}/scripts/grid" "${DEST}/inputs"
cp -a "${REPO_ROOT}/scripts/env/setup_${STEP}.sh" "${DEST}/scripts/env/"
cp -a "${REPO_ROOT}/scripts/grid/${STEP_DIR}" "${DEST}/scripts/grid/"

case "${STEP}" in
  step1)
    require_file "${BUILD_DIR}/bin/runGeoEffFDEvtSim"
    require_file "${BUILD_DIR}/lib/libgeoEff.so"
    mkdir -p "${DEST}/bin" "${DEST}/lib"
    cp -a "${BUILD_DIR}/bin/runGeoEffFDEvtSim" "${DEST}/bin/"
    cp -a "${BUILD_DIR}/lib/libgeoEff.so" "${DEST}/lib/"
    ;;
  step2)
    cp -a "${REPO_ROOT}/app" "${DEST}/app"
    find "${DEST}/app" -type d -name __pycache__ -prune -exec rm -rf {} +
    if [[ -n "${NDGEOEFF_PYTHON_ENV:-}" ]]; then
      if [[ ! -d "${NDGEOEFF_PYTHON_ENV}" ]]; then
        echo "NDGEOEFF_PYTHON_ENV is not a directory: ${NDGEOEFF_PYTHON_ENV}" >&2
        exit 1
      fi
      if [[ -d "${NDGEOEFF_PYTHON_ENV}/nvidia" ]]; then
        echo "Warning: NDGEOEFF_PYTHON_ENV contains CUDA libraries; the bundle will be much larger." >&2
      fi
      cp -a "${NDGEOEFF_PYTHON_ENV}" "${DEST}/python_env"
    fi
    ;;
  step3)
    for app in \
      NtupleOutVetoAndTrimE_AssumeEqualEffAtAllOA_NOOscSpectrumNOCAFAna_FDRateAtND \
      NtupleOutVetoAndTrimE_AssumeEqualEffAtAllOA_WithOscSpectrumNOCAFAna_FDRateAtND \
      NtupleOutVetoAndTrimE_AllOApos_WithOscSpectrumNOCAFAna_FDRateAtND; do
      require_file "${BUILD_DIR}/bin/${app}"
    done
    mkdir -p "${DEST}/bin" "${DEST}/data"
    cp -a "${BUILD_DIR}/bin/NtupleOutVetoAndTrimE_"* "${DEST}/bin/"
    cp -a "${REPO_ROOT}/data/step3" "${DEST}/data/"
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
tar -czf "${OUT}" -C "${STAGE}" DUNE_ND_GeoEff

echo "Wrote ${OUT}"
du -h "${OUT}"
tar -tzf "${OUT}" | sed -n '1,40p'
