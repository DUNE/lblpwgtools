#ifndef LBL2025_AXES_H
#define LBL2025_AXES_H

#include "CAFAna/Core/HistAxis.h"

#include "lbl2025/Vars.h"

namespace lbl2025
{
	inline const ana::HistAxis ax_NumuScore("CVN numu score", ana::Binning::Simple(50, 0, 1), kNumuCVNScore);
	inline const ana::HistAxis ax_RecoEnu("Reco neutrino energy (GeV)", ana::Binning::Simple(50, 0, 5), kRecoEnu_lepCalo);

	inline const ana::RecoPartHistAxis ax_PartScore("Particle score", ana::Binning::Simple(50, 0, 1), kRecoParticleScore);

}

#endif //LBL2025_AXES_H
