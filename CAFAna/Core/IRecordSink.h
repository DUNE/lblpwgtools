#pragma once

#include "cafanacore/IRecordSink.h"
#include "cafanacore/IRecordEnsembleSink.h"

#include "duneanaobj/StandardRecord/Proxy/FwdDeclare.h"

namespace ana
{
  using ISRSink = _IRecordSink<caf::SRProxy>;
  using IInteractionSink = _IRecordSink<caf::SRInteractionProxy>;

//  using ISpillEnsembleSink = _IRecordEnsembleSink<caf::SRSpillProxy>;
  using IInteractionEnsembleSink = _IRecordEnsembleSink<caf::SRInteractionProxy>;
}