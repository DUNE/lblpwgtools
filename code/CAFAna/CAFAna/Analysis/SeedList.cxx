#include "CAFAna/Analysis/SeedList.h"

#include "CAFAna/Core/IFitVar.h"

namespace ana
{
  //----------------------------------------------------------------------
  void Seed::ResetCalc(osc::IOscCalculatorAdjustable* calc) const
  {
    for(auto it: fVals) it.first->SetValue(calc, it.second);
  }

  //----------------------------------------------------------------------
  std::ostream& operator<<(std::ostream& os, const Seed& seed)
  {
    os << "(";
    for(auto it: seed.fVals){
      os << " " << it.first->ShortName() << "=" << it.second;
    }
    os << " )";
    return os;
  }

  //----------------------------------------------------------------------
  SeedList::SeedList(const std::map<const IFitVar*, std::vector<double>>& seedBasis)
  {
    fSeeds.push_back(Seed({}));

    for(auto it: seedBasis){
      // For every variable, duplicate every entry in ret with the value set to
      // each possibility.
      const IFitVar* fv = it.first;
      std::vector<Seed> newseeds;
      for(double val: it.second){
        for(Seed pt: fSeeds){
          pt.Set(fv, val);
          newseeds.push_back(pt);
        }
      } // end for val
      fSeeds = newseeds;
    } // end for it
  }

  //----------------------------------------------------------------------
  std::ostream& operator<<(std::ostream& os, const SeedList& seeds)
  {
    for(Seed s: seeds.GetSeeds()) os << s << std::endl;
    return os;
  }
}
