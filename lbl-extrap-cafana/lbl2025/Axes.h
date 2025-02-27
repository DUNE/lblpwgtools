#ifndef LBL2025_AXES_H
#define LBL2025_AXES_H

#include "CAFAna/Core/HistAxis.h"

#include "lbl2025/Vars.h"

namespace lbl2025
{
	inline const ana::RecoPartHistAxis ax_PartScore("Particle score", ana::Binning::Simple(50, 0, 1), kRecoParticleScore);

}

#endif //LBL2025_AXES_H
