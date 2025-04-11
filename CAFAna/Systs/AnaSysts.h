#pragma once

#include "CAFAna/Core/ISyst.h"

#include <vector>

class TH1D;

namespace ana
{

  std::vector<std::string> SplitString(std::string input, char delim = ' ');

  void KeepSysts(std::vector<const ISyst *> &systlist, std::vector<std::string> const &systsToInclude);

  void KeepSysts(std::vector<const ISyst *> &systlist, std::vector<const ISyst *> const &systsToInclude);

  void RemoveSysts(std::vector<const ISyst *> &systlist, std::vector<std::string> const &namesToRemove);

}
