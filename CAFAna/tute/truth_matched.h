#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Cut.h"

namespace ana{

// used in TestAncenstry.C
  const RecoPartVar kMatchedTruePartE([](const caf::SRRecoParticleProxy * part) -> float
  {
    if (part->truth.empty())
      return -999.;

    // Ancestor() returns the closest ancestor in the SR hierarchy with the template type.
    // Once we have the top-level object, we then can connect its
    const caf::SRProxy * top = part->Ancestor<caf::SRProxy>();
    return caf::FindParticle(top->mc, part->truth[0])->p.E;
  });

  // reco particles whose best-match particle is a true muon
  const RecoPartCut kMatchedToTrueMuon([](const caf::SRRecoParticleProxy * part) -> float
  {
    if (part->truth.empty())
      return false;

    const caf::SRProxy * top = part->Ancestor<caf::SRProxy>();
    return std::abs(caf::FindParticle(top->mc, part->truth[0])->pdg) == 13;
  });

  // Select reco particles that have a pdg of muon
  const RecoPartCut kIsMuon([](const caf::SRRecoParticleProxy* sr)
                            {
                              return abs(sr->pdg) == 13 ;
                            });

  const Var kMatchedTrueInteractionEnergy([](const caf::SRInteractionProxy * ixn) -> float
  {
    if (ixn->truth.empty())
      return -999.;

    // Ancestor() returns the closest ancestor in the SR hierarchy with the template type.
    // Once we have the top-level object, we then can connect its
    const caf::SRProxy * top = ixn->Ancestor<caf::SRProxy>();
    // should be a number...
    return caf::FindInteraction(top->mc, ixn->truth[0])->E;

  });

  // cut on true interaction mode as defined in StandardRecord/SREnums.h
  const Cut kMatchedToTrueRES([](const caf::SRInteractionProxy * ixn) 
  {
    if (ixn->truth.empty())
      return false;
    
    const caf::SRProxy * top = ixn->Ancestor<caf::SRProxy>();
    return caf::FindInteraction(top->mc, ixn->truth[0])->mode == caf::ScatteringMode::kRes;
  });

// used in demo_pions.C
const RecoPartVar kMatchedTruePartPDG([](const caf::SRRecoParticleProxy * part) -> int
{
  if (part->truth.empty())
    return -999;

  // Ancestor() returns the closest ancestor in the SR hierarchy with the template type.
  // Once we have the top-level object, we then can connect its
  const caf::SRProxy * top = part->Ancestor<caf::SRProxy>();
  int pdg = caf::FindParticle(top->mc, part->truth[0])->pdg;
  // recast in a different way for easier visualization
  //(1) muon
  if ( abs(pdg)==13) return 1.;
  //(2) electron
  if ( abs(pdg)==11) return 2.;
  //(3) proton
  if ( pdg==2212) return 3.;
  // (4) neutron
  if ( pdg==2112) return 4.;
  // (5) pi +
  if ( pdg==211) return 5.;
  // (6) pi -
  if ( pdg==-211) return 6.;
  // (7) pi0
  if ( pdg==111) return 7.;
  // (8) photon
  if (pdg==22) return 8.;
  // (9) Any K0S, K0L K0 or K+/-
  if (abs(pdg)==321 || pdg== 311|| pdg== 310|| pdg== 130) return 9.;
  else return 10.;
});
std::vector<std::string> PDGlabels = {"#mu", "e","p","n","#pi^{+}","#pi^{-}","#pi^{0}","#gamma","K","other"};

// reco particles whose best-match particle is a true pion
const RecoPartCut kMatchedToTruePion([](const caf::SRRecoParticleProxy * part) -> float
{
  if (part->truth.empty())
    return false;

  const caf::SRProxy * top = part->Ancestor<caf::SRProxy>();
  return std::abs(caf::FindParticle(top->mc, part->truth[0])->pdg) == 211;
});


}