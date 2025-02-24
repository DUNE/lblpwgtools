#pragma once

#include "CAFAnaCore/CAFAna/Core/Cut.h"

#include "duneanaobj/StandardRecord/Proxy/FwdDeclare.h"

namespace ana
{
//  using Cut = _Cut<caf::SRProxy>;

  /// \brief Representation of a cut (selection) to be applied to a \ref
  /// caf::StandardRecord object
  ///
  /// A Cut consists of a function, taking a StandardRecord and returning a
  /// boolean indicating if that event passes the cut.
  ///
  /// Cut objects may be combined with the standard boolean operations && ||
  /// and !
  typedef _Cut<caf::SRInteractionProxy> InteractionCut;
  typedef _Cut<caf::SRInteractionProxy> Cut;
  typedef _Cut<caf::SRTrueInteractionProxy> TruthCut;
  typedef _Cut<caf::SRTrueParticleProxy> TruthPartCut;
  typedef _Cut<caf::SRNDLArIntProxy> NDCut;

  /// \brief Equivalent of \ref Cut acting on \ref caf::StandardRecord. For use in
  /// spill-by-spill data quality cuts ????
  typedef _Cut<caf::SRProxy> SRCut;

  /// Convert a Cut over a true interaction to one over a reco interaction with associated truth.
  Cut TruthCutToCut(const TruthCut & truthcut);


  /// The simplest possible cut: pass everything, used as a default
  const Cut kNoCut(NoCut<caf::SRInteractionProxy>{});
  const TruthCut kNoTruthCut(NoCut<caf::SRTrueInteractionProxy>{});
  const TruthPartCut kNoTruthPartCut(NoCut<caf::SRTrueParticleProxy>{});
  const NDCut kNoNDCut(NoCut<caf::SRNDLArIntProxy>{});
  const SRCut kNoSRCut(NoCut<caf::SRProxy>{});

  typedef _Cut<caf::SRRecoParticleProxy> RecoPartCut;

  const RecoPartCut kNoPartCut(NoCut<caf::SRRecoParticleProxy>{});

//  typedef _Cut<caf::SRRecoParticlesBranchProxy> RecoPartCut;
//  const RecoPartCut kNoPartCut(NoCut<caf::SRRecoParticlesBranchProxy>{});


}
