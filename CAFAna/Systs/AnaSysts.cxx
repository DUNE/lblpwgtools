#include "CAFAna/Systs/AnaSysts.h"
#include "CAFAna/Systs/CrazyFluxFakeData.h"
#include "CAFAna/Systs/DUNEFluxSysts.h"
#include "CAFAna/Systs/XSecSysts.h"

#include "TFile.h"
#include "TH1.h"

#include <sstream>
#include <string>
#include <vector>

namespace ana
{
  // Because most particle physics is stuck in C++
  // TO DO: Define this elsewhere, in some kind of general Utilities
  std::vector<std::string> SplitString(std::string input, char delim) {
    std::vector<std::string> output;
    std::stringstream ss(input);
    std::string token;
    while (std::getline(ss, token, delim))
      output.push_back(token);
    return output;
  }

  // Take a list of all the systs known about, and retain the named systs
  void KeepSysts(std::vector<const ISyst *> &systlist,
                 std::vector<std::string> const &systsToInclude){
    systlist.erase(std::remove_if(systlist.begin(), systlist.end(),
                                  [&](const ISyst *s) {
                                    return (std::find(systsToInclude.begin(),
                                                      systsToInclude.end(),
                                                      s->ShortName()) ==
                                            systsToInclude.end());
                                  }), systlist.end());
  }

  void KeepSysts(std::vector<const ISyst *> &systlist,
                 std::vector<const ISyst *> const &systsToInclude){

    systlist.erase(std::remove_if(systlist.begin(), systlist.end(),
                                  [&](const ISyst *s) {
                                    return (std::find(systsToInclude.begin(),
                                                      systsToInclude.end(),
                                                      s) == systsToInclude.end());
                                  }), systlist.end());
  }

  void RemoveSysts(std::vector<const ISyst *> &systlist,
                   std::vector<std::string> const &namesToRemove){

    systlist.erase(std::remove_if(systlist.begin(), systlist.end(),
                                  [&](const ISyst *s) {
                                    return (std::find(namesToRemove.begin(),
                                            namesToRemove.end(),
                                            s->ShortName()) != namesToRemove.end());
                                  }), systlist.end());
  }

}
