#pragma once

#include "CAFAnaCore/CAFAna/Core/HistAxis.h"

#include "CAFAna/Core/Var.h"

namespace ana
{
  typedef _HistAxis<Var> HistAxis;
  typedef _HistAxis<SRVar> SRHistAxis;
  typedef _HistAxis<TruthVar> TruthHistAxis;
  typedef _HistAxis<RecoPartVar> RecoPartHistAxis;
  typedef _HistAxis<NDVar> NDHistAxis;
}
