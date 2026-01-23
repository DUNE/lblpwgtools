#ifndef LBL2025_CUTS_H
#define LBL2025_CUTS_H

#include "CAFAna/Core/Cut.h"
#include "CAFAna/Vars/Vars.h"
#include "lbl2025/Vars.h"

namespace lbl2025
{
	inline const ana::Cut kRecoNumuFD = kNumuCVNScore > 0.5;

	// cribbed from https://indico.fnal.gov/event/58097/contributions/276228/attachments/171330/231102/9_26_23_fdsim_cm.pdf
	// supposedly these are 70cm from the walls
	inline const ana::Cut kRecoVtxInFid =    (kRecoVtxX > -275 && kRecoVtxX < 300)
	                                      && (kRecoVtxY > -550 && kRecoVtxY < 550)
	                                      && (kRecoVtxZ > 50 && kRecoVtxZ < 850);

 // TODO: Currently set to correspond ~ to 2x2.
	inline const ana::Cut kRecoVtxInFidND =    (kRecoVtxX > -60 && kRecoVtxX < 60)
	                                        && (kRecoVtxY > -60 && kRecoVtxY < 60)
	                                        && (kRecoVtxZ > -60 && kRecoVtxZ < 60);

	inline const ana::TruthCut kTrueVtxInFid_NT =    (ana::kTrueVtxX_NT > -300 && ana::kTrueVtxX_NT < 300)
	                                              && (ana::kTrueVtxY_NT > -650 && ana::kTrueVtxY_NT < 650)
	                                              && (ana::kTrueVtxZ_NT > 50 && ana::kTrueVtxZ_NT < 850);
	inline const ana::Cut kTrueVtxInFid = ana::TruthCutToCut(kTrueVtxInFid_NT);

	extern const ana::RecoPartCut kPartIsMuon;

	extern const ana::Cut kHasOverlap;
}

#endif // LBL2025_CUTS_H
