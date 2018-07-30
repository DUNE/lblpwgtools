#pragma once

#include <string>
#include <vector>

namespace ana
{
  std::vector<std::string> GetGenieWeightNames();

  // -1 if not found, and an error printout unless quiet=true
  int GetGenieIndex(const std::string& name, bool quiet = false);
}
