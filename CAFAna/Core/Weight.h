#pragma once

#include "CAFAnaCore/CAFAna/Core/Weight.h"

#include "duneanaobj/StandardRecord/Proxy/FwdDeclare.h"

//namespace ana
//{
//  using Weight = _Weight<caf::SRProxy>;
//
//#define SIMPLEWEIGHT(CAFNAME) Weight([](const caf::SRProxy* sr){return sr->CAFNAME;})
//
//  const Weight kUnweighted = Unweighted<caf::SRProxy>();
//}

namespace ana
{
  typedef _Weight<caf::SRProxy> SRWeight;

  /// \brief Equivalent of \ref Weight acting on \ref caf::SRSpill
  typedef _Weight<caf::SRInteractionProxy> Weight;
  typedef _Weight<caf::SRTrueInteractionProxy> NuTruthWeight;

  /// The simplest possible Weight, always 1. Used as a default weight.
  const SRWeight kSRUnweighted = Unweighted<caf::SRProxy>();
  const Weight kUnweighted = Unweighted<caf::SRInteractionProxy>();


} // namespace