#!/usr/bin/env bash
# Install the step 2 third-party Python dependencies into
# lblpwgtools/CAFAna/PRISM/GeoEff/my_python_env (or the directory given as $1).
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
TARGET="${1:-${REPO_ROOT}/my_python_env}"

mkdir -p "${TARGET}"
python3 -m pip install --target="${TARGET}" \
  numpy==1.26.1 \
  uproot==5.1.2 \
  torch \
  scipy==1.11.3

echo "Installed step 2 Python dependencies in ${TARGET}"
echo "export PYTHONPATH=${TARGET}:\${PYTHONPATH:-}"
