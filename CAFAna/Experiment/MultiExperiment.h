#pragma once

#include "CAFAna/Experiment/IExperiment.h"

#include <memory>
#include <vector>

namespace ana
{
  /// Combine multiple component experiments
  class MultiExperiment: public IExperiment
  {
  public:
    MultiExperiment(std::vector<const IExperiment*> expts = {}) : fExpts(expts)
    {
      fSystCorrelations.resize(expts.size());
    }

    void Add(const IExperiment* expt)
    {
      fExpts.push_back(expt);
      fSystCorrelations.resize(fExpts.size());
    }

    virtual double ChiSq(osc::IOscCalculatorAdjustable* osc,
                         const SystShifts& syst = SystShifts::Nominal()) const override;

    /// Sum up log-likelihoods of sub-expts.  N.b.: covariance matrix business not currently supported for Stan.
    stan::math::var LogLikelihood(osc::IOscCalculatorAdjustableStan* osc,
                                  const SystShifts& syst) const override;

    /// \brief For the subexperiment \a idx, set up a mapping between
    /// systematics
    ///
    /// Each element in the vector is a pair from a "primary" systematic to a
    /// "secondary". When this MultiExperiment is called with a primary
    /// systematic shifted, the sub-experiment will be called with the
    /// secondary systematic set to the same value (and the primary unset).
    ///
    /// You can pass NULL for a secondary to indicate that the systematic
    /// simply has no effect on the experiment in question and should be
    /// filtered out.
    void SetSystCorrelations(int idx,
                             const std::vector<std::pair<const ISyst*,
                                                         const ISyst*>>& corrs);

    virtual void SaveTo(TDirectory* dir, const std::string& name) const override;
    static std::unique_ptr<MultiExperiment> LoadFrom(TDirectory* dir, const std::string& name);

  protected:
    SystShifts TranslateShifts(const SystShifts& syst, int idx) const;

    std::vector<std::vector<std::pair<const ISyst*, const ISyst*>>> fSystCorrelations;

    std::vector<const IExperiment*> fExpts;
  };
}
