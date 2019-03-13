#pragma once

#include <map>
#include <unordered_map>
#include <vector>

namespace osc{class IOscCalculatorAdjustable;}

namespace ana
{
  class IFitVar;

  class Seed
  {
  public:
    Seed(const std::map<const IFitVar*, double>& vals) : fVals(vals.begin(), vals.end()) {}

    void Set(const IFitVar* v, double x) {fVals[v] = x;}

    void ResetCalc(osc::IOscCalculatorAdjustable* calc) const;

  protected:
    std::unordered_map<const IFitVar*, double> fVals;
  };

  class SeedList
  {
  public:
    SeedList() {}

    /// Explicit list of seeds
    SeedList(const std::vector<Seed>& seeds) : fSeeds(seeds) {}
    /// \param seedBasis Set each var to each of the values. Try all
    ///                  combinations. Beware of combinatorical explosion...
    SeedList(const std::map<const IFitVar*, std::vector<double>>& seedBasis);
    
    const std::vector<Seed>& GetSeeds() const {return fSeeds;}
  protected:
    std::vector<Seed> fSeeds;
  };
}
