#pragma once

#include "cafanacore/IRecordSink.h"

#include "duneanaobj/StandardRecord/Proxy/FwdDeclare.h"

namespace ana
{
  using ISRSink = _IRecordSink<caf::SRProxy>;
  using IInteractionSink = _IRecordSink<caf::SRInteractionProxy>;

// not dealing with Ensemble stuff yet
//  using ISpillEnsembleSink = _IRecordEnsembleSink<caf::SRSpillProxy>;
//  using ISliceEnsembleSink = _IRecordEnsembleSink<caf::SRSliceProxy>;
}