#include "lbl2025/Cuts.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

namespace lbl2025
{
	const ana::RecoPartCut kPartIsMuon([](const caf::SRRecoParticleProxy * part)
	{
		return std::abs(part->pdg) == 13;
	});
}