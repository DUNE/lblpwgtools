#!/bin/bash
# Usage (on dunegpvm, OUTSIDE the container):
#   ./run_study.sh baseline uniform fluxcomp nearonly fluxcomp_moreoffaxis
# Each plan runs sequentially inside the SL7 container; logs -> logs/<plan>.log
STUDY=/nashome/l/lorenzog/PRISM_SW/lblpwgtools/RunPlanStudy
for p in "$@"; do
  echo "[$(date)] starting $p"
  bash ~/start_sl7_container.sh > ${STUDY}/logs/${p}.log 2>&1 <<INNER
source ~/CAFAnaEnv.sh

cd ${STUDY}/configs
/usr/bin/time -v PRISMPrediction --fcl RunPlanStudy_${p}.fcl
echo "EXIT CODE: \$?"
INNER
  echo "[$(date)] finished $p: $(grep 'EXIT CODE' ${STUDY}/logs/${p}.log)"
done
