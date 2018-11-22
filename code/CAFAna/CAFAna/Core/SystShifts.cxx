#include "CAFAna/Core/SystShifts.h"

#include "Utilities/func/MathUtil.h"

#include <cassert>

#include "TString.h"

namespace ana
{
  // Reserve 0 for unshifted
  int SystShifts::fgNextID = 1;

  //----------------------------------------------------------------------
  SystShifts::SystShifts()
    : fID(0)
  {
  }

  //----------------------------------------------------------------------
  SystShifts::SystShifts(const ISyst* syst, double shift)
    : fID(fgNextID++)
  {
    fSysts.push_back(std::make_pair(syst, shift));
  }

  //----------------------------------------------------------------------
  SystShifts::SystShifts(const std::map<const ISyst*, double>& shifts)
    : fID(fgNextID++)
  {
    for(auto it: shifts) fSysts.push_back(it);
  }

  //----------------------------------------------------------------------
  void SystShifts::SetShift(const ISyst* syst, double shift)
  {
    fID = fgNextID++;

    for(auto it = fSysts.begin(); it != fSysts.end(); ++it){
      if(it->first == syst){
        if(shift == 0)
          fSysts.erase(it);
        else
          it->second = shift;
        return;
      }
    }

    // We didn't find it by pointer. Let's try by name. TODO - this whole
    // "pointers are unreliable IDs" thing is a total mess.
    const std::string sn = syst->ShortName();
    for(auto it = fSysts.begin(); it != fSysts.end(); ++it){
      if(it->first->ShortName() == sn){
        if(shift == 0)
          fSysts.erase(it);
        else
          it->second = shift;
        return;
      }
    }

    // Didn't find it. They must want to add it (unless they're trying to clear
    // it)
    if(shift != 0) fSysts.push_back(std::make_pair(syst, shift));
  }

  //----------------------------------------------------------------------
  double SystShifts::GetShift(const ISyst* syst) const
  {
    assert(syst);

    // TODO: maybe better as a map?
    for(auto it: fSysts) if(it.first == syst) return it.second;

    // We didn't find it by pointer. Let's try by name. TODO - this whole
    // "pointers are unreliable IDs" thing is a total mess.
    const std::string sn = syst->ShortName();
    for(auto it: fSysts) if(it.first->ShortName() == sn) return it.second;

    return 0;
  }

  //----------------------------------------------------------------------
  void SystShifts::ResetToNominal()
  {
    fID = 0;

    fSysts.clear();
  }

  //----------------------------------------------------------------------
  double SystShifts::Penalty() const
  {
    double ret = 0;
    // Systematics are all expressed in terms of sigmas
    for(auto it: fSysts) {
      // Only apply a penalty for systematics where this has been requested
      if (it.first->ApplyPenalty())
	ret += util::sqr(it.second);
    }
    return ret;
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
    for(unsigned int i = 0; i < fSysts.size(); ++i){
      if(i != 0) ret += ",";
      ret += fSysts[i].first->ShortName();
      ret += TString::Format("=%+g", fSysts[i].second).Data();
    }

    return ret;
  }

  //----------------------------------------------------------------------
  std::string SystShifts::LatexName() const
  {
    if(fSysts.empty()) return "Nominal";

    std::string ret;
    for(unsigned int i = 0; i < fSysts.size(); ++i){
      if(i != 0) ret += ", ";
      ret += fSysts[i].first->LatexName();
      ret += TString::Format(" = %+g", fSysts[i].second).Data();
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
  const ISyst* SystShifts::SystFromShortName(const std::string& name) const
  {
    for(auto it: fSysts){
      if(it.first->ShortName() == name) return it.first;
    }
    return 0;
  }
}
