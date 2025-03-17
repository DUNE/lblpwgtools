#ifndef LBL2025_CUTS_H
#define LBL2025_CUTS_H

#include "CAFAna/Core/Cut.h"
#include "lbl2025/Vars.h"

namespace lbl2025
{
	inline const ana::Cut kRecoNumuFD = kNumuCVNScore > 0.5;

	// cribbed from https://indico.fnal.gov/event/58097/contributions/276228/attachments/171330/231102/9_26_23_fdsim_cm.pdf
	// supposedly these are 70cm from the walls
	inline const ana::Cut kRecoVtxInFid =    (kRecoVtxX > -300 && kRecoVtxX < 300)
	                                      && (kRecoVtxY > -600 && kRecoVtxY < 600)
	                                      && (kRecoVtxZ > 100 && kRecoVtxZ < 800);

	extern const ana::RecoPartCut kPartIsMuon;
}

#endif // LBL2025_CUTS_H