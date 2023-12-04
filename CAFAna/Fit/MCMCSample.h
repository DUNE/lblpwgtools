#pragma once

#include <string>
#include <vector>

#include "TMap.h"

#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Core/ISyst.h"

namespace ana
{
/// A single MCMC sample.
  class MCMCSample
  {
    public:
      MCMCSample(double LL, const std::vector<double> &diagVals, const std::vector<double> &entryVals,
                 const std::vector<std::string> &diagBranchNames,
                 const std::vector<const IFitVar *> &vars, const std::vector<const ISyst *> &systs);

      MCMCSample(const TMap & map,
                 const std::vector<const ana::IFitVar *> & vars,
                 const std::vector<const ana::ISyst *>& systs);

      MCMCSample(const MCMCSample& other) = default;
      MCMCSample(MCMCSample&& other) = default;

      [[nodiscard]]
      double DiagnosticVal(const std::string &brName) const;

      [[nodiscard]]
      const std::vector <std::string> &DiagnosticValNames() const
      {
        return fDiagBranches;
      }

      [[nodiscard]]
      double LL() const
      {
        return fVals[0];
      }

      [[nodiscard]]
      const std::vector<const ana::ISyst *> &Systs() const
      {
        return fSysts;
      }

      /// Mostly for serialization.
      [[nodiscard]]
      std::unique_ptr<TMap> ToTMap() const;

      [[nodiscard]]
      double Val(const ana::IFitVar *var) const;

      [[nodiscard]]
      double Val(const ana::ISyst *syst) const;

      [[nodiscard]]
      const std::vector<const ana::IFitVar *> &Vars() const
      {
        return fVars;
      }

    private:
      std::vector <std::string> fDiagBranches;
      std::vector<const ana::IFitVar *> fVars;
      std::vector<const ana::ISyst *> fSysts;

      std::vector<double> fVals;
  };
}