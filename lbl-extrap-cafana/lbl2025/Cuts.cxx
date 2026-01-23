#include "lbl2025/Cuts.h"
#include <iostream>

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include "duneanasel/nd/ndlar/Selections.h"

namespace lbl2025
{
	const ana::RecoPartCut kPartIsMuon([](const caf::SRRecoParticleProxy * part)
	{
		return std::abs(part->pdg) == 13;
	});

	const ana::Cut kHasOverlap([](const caf::SRInteractionProxy * sri)
	{
   if(!sri->truthOverlap.size()) return false;
   //std::cout << sri->truthOverlap[0] << std::endl;
		 return !(sri->truthOverlap[0] < 0.5);
	});
	const ana::Cut kNumuCCLikeCont([](const caf::SRInteractionProxy * sr)
	{
		return sel::beam::ndlar::numode::NuMuCCLikeContained(*sr);
	});
}
