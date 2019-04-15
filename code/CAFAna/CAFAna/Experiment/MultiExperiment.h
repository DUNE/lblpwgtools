#pragma once

#include "CAFAna/Experiment/IExperiment.h"
#include "TMatrixD.h"

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

    void Add(const IExperiment* expt){
      fExpts.push_back(expt); 
      fUseCovMx.push_back(false); // default to no covariance matrix
      fSystCorrelations.resize(fExpts.size());
    }

    void AddCovarianceMatrix(const std::string covMatFileName, 
                             const std::string covMatName, 
                             const bool preInvert, 
                             const std::vector<int> expt_idxes);

    virtual double ChiSq(osc::IOscCalculatorAdjustable* osc,
                         const SystShifts& syst = SystShifts::Nominal()) const override;

    virtual void Derivative(osc::IOscCalculator* calc,
                            const SystShifts& shift,
                            std::unordered_map<const ISyst*, double>& dchi) const override;

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

    virtual void SaveTo(TDirectory* dir) const override;
    static std::unique_ptr<MultiExperiment> LoadFrom(TDirectory* dir);

  protected:
    std::vector<const IExperiment*> fExpts;
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
