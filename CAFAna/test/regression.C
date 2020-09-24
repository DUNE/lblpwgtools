#include "CAFAna/Analysis/common_fit_definitions.h"
using namespace ana;

void regression(std::string state = "/pnfs/dune/persistent/stash/LongBaseline/state_files/final_v4/State")
{
  // RunFitPoint needs both of these things. The osc is unused for our nd-only
  // test, and the systs are nominal (and unused in the fit).
  osc::IOscCalcAdjustable* dummyOsc = NuFitOscCalc(+1, 1, "0");
  SystShifts dummySyst;

  RunFitPoint(state, "nd", dummyOsc, dummySyst, false, {}, {}, dummyOsc, dummySyst);
}
