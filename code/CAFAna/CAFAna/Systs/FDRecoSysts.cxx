#include "CAFAna/Systs/FDRecoSysts.h"
#include "CAFAna/Core/Utilities.h"

#include "TH2.h"
#include "TFile.h"

namespace ana {

  const FDRecoNumuSyst kFDRecoNumuSyst;
  const FDRecoNueSyst kFDRecoNueSyst;
  const FVNumuFDSyst kFVNumuFDSyst;
  const FVNueFDSyst kFVNueFDSyst;

  FDRecoSystVector GetFDRecoSysts()
  {
    FDRecoSystVector vec = {};
    vec.push_back(&kFDRecoNumuSyst);
    vec.push_back(&kFDRecoNueSyst);
    vec.push_back(&kFVNumuFDSyst);
    vec.push_back(&kFVNueFDSyst);
    return vec;
  }
  
}
