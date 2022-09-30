#pragma once

#include "cafanacore/IRecordSource.h"

#include "StandardRecord/Proxy/FwdDeclare.h"

namespace ana
{
  using IRecordSource = _IRecordSource<caf::SRProxy>;
}
