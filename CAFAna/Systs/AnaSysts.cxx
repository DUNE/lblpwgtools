#include "CAFAna/Systs/AnaSysts.h"

#include "CAFAna/Systs/EnergySysts.h"
#include "CAFAna/Systs/FDRecoSysts.h"
#include "CAFAna/Systs/NDRecoSysts.h"
#include "CAFAna/Systs/NuOnESysts.h"

#include "TFile.h"
#include "TH1.h"

#include <vector>

namespace ana
{

  std::vector<const ISyst*> getDetectorSysts(bool useFD, bool useND, bool useNueOnE)
  {

    std::vector<const ISyst*> systs = GetEnergySysts();

    std::vector<const ISyst *> fdlist = GetFDRecoSysts();
    std::vector<const ISyst *> ndlist = GetNDRecoSysts();
    std::vector<const ISyst *> nuelist = GetNuOnESysts();

    if (useFD) {
      systs.insert(systs.end(), fdlist.begin(), fdlist.end());
    }
    if (useND) {
      systs.insert(systs.end(), ndlist.begin(), ndlist.end());
    }
    if (useND && useNueOnE) {
      systs.insert(systs.end(), nuelist.begin(), nuelist.end());
    }

    return systs;
  }

}