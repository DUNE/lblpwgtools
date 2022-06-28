#pragma once

#include "CAFAna/Core/Cut.h"

namespace PRISM {
extern const ana::Cut kETrueLT10GeV;

extern const ana::Cut kETrue8GeV;

extern const ana::Cut kERecoProxy8GeV;

extern const ana::Cut kIsOutOfTheDesert;
// Cut to check if it is reconstructed -> removes
// unreconstructed events in ND
extern const ana::Cut kIsReco;

extern const ana::Cut kCut280kARun;
extern const ana::Cut kSel280kARun;

extern const ana::Cut kPRISMNDSignal_Selected_numu;
extern const ana::Cut kPRISMNDSignal_Selected_numub;
extern const ana::Cut kPRISMNDSignal_True_numu;
extern const ana::Cut kPRISMNDSignal_True_numub;

extern const ana::Cut kPRISMNDSignal_Selected_nue;
extern const ana::Cut kPRISMNDSignal_Selected_nueb;
extern const ana::Cut kPRISMNDSignal_True_nue;
extern const ana::Cut kPRISMNDSignal_True_nueb;

extern const ana::Cut kPRISMFDSignal_Selected_numu;
extern const ana::Cut kPRISMFDSignal_Selected_numub;
extern const ana::Cut kPRISMFDSignal_True_numu;
extern const ana::Cut kPRISMFDSignal_True_numub;

extern const ana::Cut kPRISMFDSignal_Selected_nue;
extern const ana::Cut kPRISMFDSignal_Selected_nueb;
extern const ana::Cut kPRISMFDSignal_True_nue;
extern const ana::Cut kPRISMFDSignal_True_nueb;

} // namespace PRISM