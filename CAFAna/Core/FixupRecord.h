#pragma once

#include "duneanaobj/StandardRecord/Proxy/FwdDeclare.h"

class TTree;

namespace ana
{
  void FixupRecord(caf::SRProxy* sr, TTree* tr);
}
