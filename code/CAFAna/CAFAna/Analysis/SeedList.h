#pragma once

#include <iostream>
#include <map>
#include <vector>

namespace osc{class IOscCalculatorAdjustable;}

namespace ana
{
  class IFitVar;

  class Seed
  {
  public:
    friend std::ostream& operator<<(std::ostream&, const Seed&);

    Seed(const std::map<const IFitVar*, double>& vals) : fVals(vals.begin(), vals.end()) {}

    void Set(const IFitVar* v, double x) {fVals[v] = x;}

    void ResetCalc(osc::IOscCalculatorAdjustable* calc) const;

    const std::map<const IFitVar*, double> GetVals() const {return fVals;}
  protected:
    std::map<const IFitVar*, double> fVals;
  };

  std::ostream& operator<<(std::ostream& os, const Seed& seed);

  class SeedList
  {
  public:
    /// "empty" seed list actually consists of one no-op seed
    SeedList() : fSeeds(1, Seed({})) {}

    /// Explicit list of seeds
    SeedList(const std::vector<Seed>& seeds) : fSeeds(seeds) {}
    /// \param seedBasis Set each var to each of the values. Try all
    ///                  combinations. Beware of combinatorical explosion...
    SeedList(const std::map<const IFitVar*, std::vector<double>>& seedBasis);

    const std::vector<Seed>& GetSeeds() const {return fSeeds;}

    size_t size() { return fSeeds.size(); }
  protected:
    std::vector<Seed> fSeeds;
  };

  std::ostream& operator<<(std::ostream& os, const SeedList& seeds);
}
