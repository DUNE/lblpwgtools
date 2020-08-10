#pragma once

#include "CAFAna/Core/FwdDeclare.h"

#include <iostream>
#include <map>
#include <set>
#include <vector>

namespace ana
{
  class IFitVar;

  class Seed
  {
  public:
    friend std::ostream& operator<<(std::ostream&, const Seed&);

    Seed(const std::map<const IFitVar*, double>& vals) : fVals(vals) {}

    void Set(const IFitVar* v, double x){fVals[v] = x;}

    void ResetCalc(osc::IOscCalcAdjustable* calc) const;

    const std::map<const IFitVar*, double> GetVals() const {return fVals;}

    std::set<const IFitVar*> ActiveFitVars() const;
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

    size_t size() {return fSeeds.size();}

    std::set<const IFitVar*> ActiveFitVars() const;
  protected:
    std::vector<Seed> fSeeds;
  };

  std::ostream& operator<<(std::ostream& os, const SeedList& seeds);
}
