#!/usr/bin/env bash
# Install the step 2 third-party Python dependencies into
# lblpwgtools/CAFAna/PRISM/GeoEff/my_python_env (or the directory given as $1).
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
TARGET="${1:-${REPO_ROOT}/my_python_env}"
TORCH_VERSION="${NDGEOEFF_TORCH_VERSION:-2.6.0}"
TORCH_INDEX_URL="${NDGEOEFF_TORCH_INDEX_URL:-https://download.pytorch.org/whl/cpu}"

mkdir -p "${TARGET}"
python3 -m pip install --upgrade --target="${TARGET}" \
  --index-url "${TORCH_INDEX_URL}" \
  "torch==${TORCH_VERSION}"
python3 -m pip install --upgrade --target="${TARGET}" \
  numpy==1.26.1 \
  uproot==5.1.2 \
  scipy==1.11.3

echo "Installed step 2 Python dependencies in ${TARGET}"
echo "export PYTHONPATH=${TARGET}:\${PYTHONPATH:-}"
