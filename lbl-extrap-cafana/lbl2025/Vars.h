#ifndef LBL2025_VARS_H
#define LBL2025_VARS_H

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"
#include "CAFAna/Core/Var.h"

namespace lbl2025
{
	const ana::Var kNumuCVNScore = ana::SIMPLEVAR(nuhyp.cvn.numu);
	const ana::Var kRecoEnu_lepCalo = ana::SIMPLEVAR(Enu.lep_calo);

	const ana::RecoPartVar kRecoParticleScore = ana::SIMPLEPARTVAR(score);
}

#endif //LBL2025_VARS_H
