#include "CAFAna/Analysis/AnalysisVersion.h"

#include <cstdlib>

AnaVersion GetAnaVersion() {
  static AnaVersion AnaV = kV4;
  static bool first = true;
  if (first && getenv("CAFANA_ANALYSIS_VERSION")) {
    int v = atoi(getenv("CAFANA_ANALYSIS_VERSION"));
    if (v == 3) {
      AnaV = kV3;
    } else if (v == 4) {
      AnaV = kV4;
    }
    first = false;
  }

  return AnaV;
}
