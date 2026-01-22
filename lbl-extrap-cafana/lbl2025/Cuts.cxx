#include "lbl2025/Cuts.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include "duneanasel/nd/ndlar/Selections.h"

namespace lbl2025
{
	const ana::RecoPartCut kPartIsMuon([](const caf::SRRecoParticleProxy * part)
	{
		return std::abs(part->pdg) == 13;
	});

	const ana::Cut kNumuCCLikeCont([](const caf::SRInteractionProxy * sr)
	{
		return sel::beam::ndlar::numode::NuMuCCLikeContained(*sr);
	});
}