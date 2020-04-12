#pragma once

#include "CAFAna/Experiment/IExperiment.h"
#include "TMatrixD.h"

#include <algorithm>
#include <memory>
#include <vector>

namespace ana
{
  namespace
  {
    std::vector<IExperiment> _ChiSqExpPtrsToExps(std::vector<const IChiSqExperiment*> expts)
    {
      std::vector<IExperiment> ret;
      std::transform(expts.begin(), expts.end(), std::back_inserter(ret), [](const IChiSqExperiment* e) {return IExperiment(e);});
      return ret;
    }
  }

  /// Combine multiple component experiments
  class MultiExperiment: public IChiSqExperiment, public ILkhdExperiment
  {
  public:
    MultiExperiment() = default;

    explicit MultiExperiment(std::vector<IExperiment> expts) : fExpts(expts)
    {
      fSystCorrelations.resize(expts.size());
      fUseCovMx.resize(expts.size(), false);
    }

    explicit MultiExperiment(std::vector<const IChiSqExperiment*> expts)
      : MultiExperiment(_ChiSqExpPtrsToExps(expts))
    {}

    /// this constructor helps disambiguate between the vector<IChiSqExperiment> and vector<IExperiment> versions
    /// if passed a brace-initializer
    MultiExperiment(std::initializer_list<const IChiSqExperiment*> expts)
      : MultiExperiment(std::vector<const IChiSqExperiment*>(expts))
    {}

    void Add(IExperiment expt){
      fExpts.push_back(expt);
      fUseCovMx.push_back(false); // default to no covariance matrix
      fSystCorrelations.resize(fExpts.size());
    }

    void AddCovarianceMatrix(const std::string covMatFileName,
                             const std::string covMatName,
                             const bool preInvert,
                             const std::vector<int> &expt_idxes);
    void AddCovarianceMatrix(TMatrixD *,
                             const bool preInvert,
                             const std::vector<int> &expt_idxes);

    virtual double ChiSq(osc::IOscCalculatorAdjustable* osc,
                         const SystShifts& syst = SystShifts::Nominal()) const override;

    stan::math::var LogLikelihood(osc::_IOscCalculatorAdjustable<stan::math::var> *osc,
                                  const SystShifts &syst = SystShifts::Nominal()) const override;

    /// For the subexperiment \a idx, set up a mapping between systematics
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
    template <typename T>
    T _SumSimpleStatistic(osc::_IOscCalculatorAdjustable<T>* osc, const SystShifts& syst) const;

    std::vector<IExperiment> fExpts;
    std::vector<bool> fUseCovMx; // is there a covariance matrix for this Experiment?

    std::vector<std::vector<std::pair<const ISyst*, const ISyst*>>> fSystCorrelations;

    // vectors with one entry per covariance matrix
    // there can be fewer matrices than there are experiments
    // one covariance matrix can cover several experiments
    std::vector<TMatrixD*> fCovMx;
    std::vector<TMatrixD*> fCovMxInv;
    std::vector<bool> fPreInvert;
    std::vector<std::vector<int>> fCovMxExpIdx;
  };
}
