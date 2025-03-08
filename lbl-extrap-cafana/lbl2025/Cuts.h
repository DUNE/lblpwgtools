#ifndef LBL2025_CUTS_H
#define LBL2025_CUTS_H

#include "CAFAna/Core/Cut.h"
#include "lbl2025/Vars.h"

namespace lbl2025
{
	inline const ana::Cut kRecoNumuFD = kNumuCVNScore > 0.5;

	extern const ana::RecoPartCut kPartIsMuon;
}

#endif // LBL2025_CUTS_H