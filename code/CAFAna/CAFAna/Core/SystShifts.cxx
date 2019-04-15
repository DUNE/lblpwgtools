#include "CAFAna/Core/SystShifts.h"

#include "Utilities/func/MathUtil.h"

#include <cassert>
#include <iostream>

#include "TString.h"

namespace ana
{
  // Reserve 0 for unshifted
  int SystShifts::fgNextID = 1;

  //----------------------------------------------------------------------
  SystShifts::SystShifts() : fID(0)
  {
  }

  //----------------------------------------------------------------------
  SystShifts::SystShifts(const ISyst* syst, double shift)
    : fID(fgNextID++)
  {
    if(shift != 0) fSysts.emplace(syst, Clamp(shift, syst));
  }

  //----------------------------------------------------------------------
  SystShifts::SystShifts(const std::map<const ISyst*, double>& shifts)
    : fID(fgNextID++)
  {
    for(auto it: shifts) if(it.second != 0) fSysts.emplace(it.first, Clamp(it.second, it.first));
  }

  //----------------------------------------------------------------------
  void SystShifts::SetShift(const ISyst* syst, double shift)
  {
    fID = fgNextID++;

    fSysts.erase(syst);
    if(shift != 0) fSysts.emplace(syst, Clamp(shift, syst));
  }

  //----------------------------------------------------------------------
  double SystShifts::GetShift(const ISyst* syst) const
  {
    assert(syst);

    auto it = fSysts.find(syst);
    return (it == fSysts.end()) ? 0 : it->second;
  }

  //----------------------------------------------------------------------
  void SystShifts::ResetToNominal()
  {
    fID = 0;

    fSysts.clear();
  }

  //----------------------------------------------------------------------
  void SystShifts::Shift(Restorer& restore,
                         caf::StandardRecord* sr,
                         double& weight) const
  {
    for(auto it: fSysts) it.first->Shift(it.second, restore, sr, weight);
  }

  //----------------------------------------------------------------------
  std::string SystShifts::ShortName() const
  {
    if(fSysts.empty()) return "nominal";

    std::string ret;
    for(auto it: fSysts){
      if(!ret.empty()) ret += ",";
      ret += it.first->ShortName() + TString::Format("=%+g", it.second).Data();
    }

    return ret;
  }

  //----------------------------------------------------------------------
  std::string SystShifts::LatexName() const
  {
    if(fSysts.empty()) return "Nominal";

    std::string ret;
    for(auto it: fSysts){
      if(!ret.empty()) ret += ", ";
      ret += it.first->LatexName() + TString::Format(" = %+g", it.second).Data();
    }

    return ret;
  }

  //----------------------------------------------------------------------
  std::vector<const ISyst*> SystShifts::ActiveSysts() const
  {
    std::vector<const ISyst*> ret;
    for(auto it: fSysts) ret.push_back(it.first);
    return ret;
  }

  //----------------------------------------------------------------------
  double SystShifts::Clamp(double x, const ISyst* s)
  {
    static bool once = true;
    static bool shouldClamp;
    if(once){
      once = false;
      char* e = getenv("CAFANA_DONT_CLAMP_SYSTS");
      shouldClamp = (e && TString(e) != "0");
      if(!shouldClamp){
        std::cout << "SystShifts.cxx: clamping disabled by environment variable" << std::endl;
      }
    }

    if(shouldClamp)
      return std::min(std::max(x, s->Min()), s->Max());
    else
      return x;
  }
}
