#include "CAFAna/PRISM/Cuts.h"

#include "CAFAna/Cuts/AnaCuts.h"
#include "CAFAna/Cuts/TruthCuts.h"

#include "StandardRecord/StandardRecord.h"

#include <cmath>

namespace PRISM {

const ana::Cut kIsOutOfTheDesert([](const caf::StandardRecord *sr) {
  return (std::fabs(sr->vtx_x) < 200);
});

const ana::Cut kCut280kARun([](const caf::StandardRecord *sr) {
  return (std::abs(sr->SpecialHCRunId) == 293);
});
const ana::Cut kSel280kARun([](const caf::StandardRecord *sr) {
  return (std::abs(sr->SpecialHCRunId) == 280);
});
const ana::Cut kIsReco([](const caf::StandardRecord *sr) {
  return (sr->Elep_reco > 0);
});

const ana::Cut kPRISMNDSignal_Selected_numu =
    ana::kPassND_FHC_NUMU && ana::kIsTrueFV && kIsOutOfTheDesert;
const ana::Cut kPRISMNDSignal_Selected_numub =
    ana::kPassND_RHC_NUMU && ana::kIsTrueFV && kIsOutOfTheDesert;
const ana::Cut kPRISMNDSignal_True_numu =
    ana::kIsNumuCC && (!ana::kIsAntiNu) && ana::kIsTrueFV && kIsOutOfTheDesert;
const ana::Cut kPRISMNDSignal_True_numub =
    ana::kIsNumuCC && (ana::kIsAntiNu) && ana::kIsTrueFV && kIsOutOfTheDesert;

const ana::Cut kPRISMFDSignal_Selected_numu =
    ana::kPassFD_CVN_NUMU && ana::kIsTrueFV;
const ana::Cut kPRISMFDSignal_Selected_numub =
    ana::kPassFD_CVN_NUMU && ana::kIsTrueFV;
const ana::Cut kPRISMFDSignal_True_numu =
    ana::kIsNumuCC && (!ana::kIsAntiNu) && ana::kIsTrueFV;
const ana::Cut kPRISMFDSignal_True_numub =
    ana::kIsNumuCC && (ana::kIsAntiNu) && ana::kIsTrueFV;

const ana::Cut kPRISMFDSignal_Selected_nue =
    ana::kPassFD_CVN_NUE && ana::kIsTrueFV;
const ana::Cut kPRISMFDSignal_Selected_nueb =
    ana::kPassFD_CVN_NUE && ana::kIsTrueFV;
const ana::Cut kPRISMFDSignal_True_nue =
    ana::kIsSig && (!ana::kIsAntiNu) && ana::kIsTrueFV;
const ana::Cut kPRISMFDSignal_True_nueb =
    ana::kIsSig && (ana::kIsAntiNu) && ana::kIsTrueFV;

const ana::Cut kPRISMFDSignal_Selected_nutau =
    ana::kPassFD_CVN_NUTAU && ana::kIsTrueFV;
const ana::Cut kPRISMFDSignal_Selected_nutaub =
    ana::kPassFD_CVN_NUTAU && ana::kIsTrueFV;
const ana::Cut kPRISMFDSignal_True_nutau =
    ana::kIsTauFromMu && (!ana::kIsAntiNu) && ana::kIsTrueFV;
const ana::Cut kPRISMFDSignal_True_nutaub =
    ana::kIsTauFromMu && (ana::kIsAntiNu) && ana::kIsTrueFV;

} // namespace PRISM
