#include <limits>
#include <string>
#include <vector>

#include "TH1D.h"
#include "TParameter.h"

#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Utilities.h"

#include "CAFAna/Fit/Bayesian1DMarginal.h"
#include "CAFAna/Fit/BayesianSurface.h"
#include "CAFAna/Fit/MCMCSamples.h"

#include "Utilities/func/MathUtil.h"

// an internal tool only
namespace
{
  class BranchStatusResetter
  {
    public:
      BranchStatusResetter(TTree * tree)
        : fTree(tree)
      {}

      ~BranchStatusResetter()
      {
        if (fTree)
          fTree->SetBranchStatus("*", false);
      }
    private:
      TTree * fTree;
  };
}

namespace ana
{
  const std::string MCMCSamples::LOGLIKELIHOOD_BRANCH_NAME = "logprob";

  //----------------------------------------------------------------------
  MCMCSamples::MCMCSamples(const std::vector<const IFitVar *> &vars,
                           const std::vector<const ana::ISyst *> &systs)
    : fOffset(0),
      fVars(vars),
      fSysts(systs),
      fBestFitFound(false),
      fSamples(std::make_unique<TTree>("samples", "MCMC samples"))
  {}

  //----------------------------------------------------------------------
  MCMCSamples::MCMCSamples(std::size_t offset,
                           const std::vector<std::string> &diagBranchNames,
                           const std::vector<const IFitVar *> &vars,
                           const std::vector<const ana::ISyst *> &systs,
                           std::unique_ptr<TTree> &tree,
                           const Hyperparameters &hyperParams)
    : fOffset(offset),
      fDiagBranches(diagBranchNames),
      fVars(vars),
      fSysts(systs),
      fBestFitFound(false),
      fSamples(std::move(tree)),
      fHyperparams(hyperParams)
  {
    // note: SetupTree() takes care of fDiagnosticVals and fEntryVals
    SetupTree();
  }

  //----------------------------------------------------------------------
  MCMCSamples::MCMCSamples(MCMCSamples &&other)
      : fOffset(std::move(other.fOffset)),
        fDiagBranches(std::move(other.fDiagBranches)),
        fVars(std::move(other.fVars)),
        fSysts(std::move(other.fSysts)),
        fBestFitSampleIdx(std::move(other.fBestFitSampleIdx)),
        fBestFitFound(std::move(other.fBestFitFound)),
        fSamples(std::move(other.fSamples)),
        fEntryLL(std::move(other.fEntryLL)),
        fEntryVals(std::move(other.fEntryVals)),
        fDiagnosticVals(std::move(other.fDiagnosticVals)),
        fHyperparams(std::move(other.fHyperparams))
  {
    // note: SetupTree() takes care of fDiagnosticVals and fEntryVals
    SetupTree();

    // almost certainly we don't want to be writing back to whatever file this may have come from ...
    fSamples->SetDirectory(nullptr);
  }

  //----------------------------------------------------------------------
  MCMCSamples &MCMCSamples::operator=(MCMCSamples &&other)
  {
    fOffset = std::move(other.fOffset);

    fVars = std::move(other.fVars);
    fSysts = std::move(other.fSysts);

    fBestFitSampleIdx = std::move(other.fBestFitSampleIdx);
    fBestFitFound = std::move(other.fBestFitFound);

    fSamples = std::move(other.fSamples);

    fEntryLL = std::move(other.fEntryLL);
    fDiagBranches = std::move(other.fDiagBranches);

    fDiagnosticVals = std::move(other.fDiagnosticVals);
    fEntryVals = std::move(other.fEntryVals);

    fHyperparams = std::move(other.fHyperparams);

    // note: SetupTree() takes care of fDiagnosticVals and fEntryVals
    SetupTree();

    return *this;
  }

  //----------------------------------------------------------------------
  void MCMCSamples::AddSample(const std::vector<double> &sample)
  {
//    std::cout << "\nGot sample: ";
//    for (const auto & s: sample)
//      std::cout << s << " ";
//    std::cout << std::endl;
//    std::cout << "sample size: " << sample.size() << std::endl;
//    std::cout << " target size = LL (1) + diagVals (" << fDiagnosticVals.size() << ") + entryVals (" << fEntryVals.size() << ")" << std::endl;
    fEntryLL = sample[0];
    for (std::size_t idx = 1; idx < fOffset && idx < sample.size(); idx++)
    {
      assert(idx-1 < fDiagnosticVals.size());
      fDiagnosticVals[idx - 1] = sample[idx];
    }
    for (std::size_t targetIdx = 0, sourceIdx = fOffset; sourceIdx < sample.size(); targetIdx++, sourceIdx++)
    {
      assert(targetIdx < fEntryVals.size());
      fEntryVals[targetIdx] = sample[sourceIdx];
    }
    fSamples->Fill();
//    fSamples->Scan("*");
  }

  //----------------------------------------------------------------------
  void MCMCSamples::AdoptSamples(MCMCSamples && other)
  {
    // if we don't have any samples at all, then just swap
    if (!fSamples || (fDiagBranches.size() + fVars.size() + fSysts.size()) == 0)
    {
      this->fSamples.reset(other.fSamples->CloneTree());
      this->fDiagBranches = other.fDiagBranches;
      this->fVars = other.fVars;
      this->fSysts = other.fSysts;
      //SetupTree();

      // make sure we extract this guy out of his old directory, if any,
      // otherwise he wants to write back to that file
      this->fSamples->LoadBaskets();
      this->fSamples->SetDirectory(nullptr);

      return;
    }

    // first, check that the branches (etc.) all match.
    // if they don't, throw an exception
    bool branchesSame = true;
    branchesSame = branchesSame && fDiagBranches == other.fDiagBranches;
    branchesSame = branchesSame && fVars == other.fVars;
    branchesSame = branchesSame && fSysts == other.fSysts;
    if (!branchesSame)
      throw std::runtime_error("MCMCSamples::AdoptSamples(): branches are not the same!");


    // then, take the entries from its TTree and and them to ours using TTree::Merge().
    // clear the other tree once done.
    BranchStatusResetter bsr(fSamples.get());  // turn all branches off when done
    fSamples->SetBranchStatus("*", true);
    other.fSamples->SetBranchStatus("*", true);
    TList otherTreeList;
    otherTreeList.SetOwner(false);
    otherTreeList.Add(other.fSamples.get());
    fSamples->Merge(&otherTreeList);

    otherTreeList.Clear();
    other.fSamples.reset(nullptr);

    // clear the rest of the other MCMCSamples so it isn't left in an intermediate state
    other.fDiagBranches.clear();
    other.fVars.clear();
    other.fSysts.clear();
    other.fOffset = 0;
    other.fBestFitSampleIdx = 0;
    other.fBestFitFound = false;
    other.fEntryLL = 0;
    other.fDiagnosticVals.clear();
    other.fEntryVals.clear();

    // finally, the best fit point isn't necessarily the same any more, so force recalculation the next time it's needed
    fBestFitFound = false;
  }

  //----------------------------------------------------------------------
  std::size_t MCMCSamples::BestFitSampleIdx() const
  {
    if (fBestFitFound)
      return fBestFitSampleIdx;

    double maxLL = -std::numeric_limits<double>::infinity();
    auto branch = fSamples->GetBranch(MCMCSamples::LOGLIKELIHOOD_BRANCH_NAME.c_str());
    branch->SetStatus(true);
    for (int idx = 0; idx < fSamples->GetEntries(); idx++)
    {
      branch->GetEntry(idx);
      if (fEntryLL > maxLL)
      {
        fBestFitSampleIdx = idx;
        maxLL = fEntryLL;
        fBestFitFound = true;
      }
    }
    return fBestFitSampleIdx;
  }

  //----------------------------------------------------------------------
  std::size_t MCMCSamples::DiagOffset(const std::string &diagName) const
  {
    auto itr = std::find(fDiagBranches.begin(), fDiagBranches.end(), diagName);
    assert(itr != fDiagBranches.end() && ("Unknown diagnostic branch: " + diagName).c_str());
    return std::distance(fDiagBranches.begin(), itr);
  }


  //----------------------------------------------------------------------
  std::unique_ptr<MCMCSamples> MCMCSamples::LoadFrom(TDirectory *dir,
                                                     const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    DontAddDirectory guard;

    TObjString *tag = (TObjString *) dir->Get("type");
    assert(tag);
    assert(tag->GetString() == "MCMCSamples");

    auto offset = dynamic_cast<TParameter<int>*>(dir->Get("offset"));

    auto diagBranchNames = dynamic_cast<TList*>(dir->Get("diagBranchNames"));
    std::vector<std::string> diagBranches;
    for (const TObject * brNameObj : *diagBranchNames)
    {
      auto str = dynamic_cast<const TObjString*>(brNameObj);
      if (str)
        diagBranches.emplace_back(str->GetName());
    }

    auto fitVarNames = dynamic_cast<TList*>(dir->Get("fitVarNames"));
    std::vector<const IFitVar*> fitVars;
    for (const TObject * varNmObj : *fitVarNames)
    {
      auto str = dynamic_cast<const TObjString*>(varNmObj);
      if (str)
        fitVars.emplace_back(Registry<IFitVar>::ShortNameToPtr(str->GetName()));
    }

    auto systNames = dynamic_cast<TList*>(dir->Get("systNames"));
    std::vector<const ISyst*> systs;
    for (const TObject * systNmObj : *systNames)
    {
      auto str = dynamic_cast<const TObjString*>(systNmObj);
      if (str)
        systs.emplace_back(Registry<ISyst>::ShortNameToPtr(str->GetName()));
    }

    auto samplesPtr = dynamic_cast<TTree*>(dir->Get("samples"));
    std::unique_ptr<TTree> samples(samplesPtr);
    if (samples->GetCurrentFile())
    {
      BranchStatusResetter bsr(samples.get());  // turn branches off when done
      samples->SetBranchStatus("*", true);
      samples->LoadBaskets();   // read the entire TTree into memory
      samples->SetDirectory(nullptr);  // disassociate it from the file it came from so that when the file is closed it persists
    }

    auto hyperParamsDir = dynamic_cast<TDirectory*>(dir->Get("hyperparams"));
    auto stepSize = hyperParamsDir->Get<TParameter<double>>("stepsize");
    auto invMetric = std::unique_ptr<TMatrixD>(hyperParamsDir->Get<TMatrixD>("inv_metric"));
    Hyperparameters hyperparams{stepSize->GetVal(), std::move(invMetric)};

    delete dir;

    return std::unique_ptr<MCMCSamples>(new MCMCSamples(offset->GetVal(),
                                                        diagBranches,
                                                        fitVars,
                                                        systs,
                                                        samples,
                                                        hyperparams));
  }

  //----------------------------------------------------------------------
  Bayesian1DMarginal MCMCSamples::MarginalizeTo(const IFitVar *var, BayesianMarginal::MarginalMode marginalMode) const
  {
    assert(std::find(fVars.begin(), fVars.end(), var) != fVars.end());
    return Bayesian1DMarginal(*this, var, marginalMode);
  }

  //----------------------------------------------------------------------
  Bayesian1DMarginal MCMCSamples::MarginalizeTo(const ISyst * syst, BayesianMarginal::MarginalMode marginalMode) const
  {
    assert(std::find(fSysts.begin(), fSysts.end(), syst) != fSysts.end());
    return Bayesian1DMarginal(*this, syst, marginalMode);
  }

  //----------------------------------------------------------------------
  BayesianSurface MCMCSamples::MarginalizeTo(const IFitVar *xvar, int nbinsx, double xmin, double xmax,
                                             const IFitVar *yvar, int nbinsy, double ymin, double ymax,
                                             BayesianMarginal::MarginalMode marginalMode) const
  {
    assert(std::find(fVars.begin(), fVars.end(), xvar) != fVars.end());
    assert(std::find(fVars.begin(), fVars.end(), yvar) != fVars.end());
    return BayesianSurface(*this,
                           xvar, nbinsx, xmin, xmax,
                           yvar, nbinsy, ymin, ymax,
                           marginalMode);
  }

  //----------------------------------------------------------------------
  BayesianSurface MCMCSamples::MarginalizeTo(const ISyst *xsyst, int nbinsx, double xmin, double xmax,
                                             const ISyst *ysyst, int nbinsy, double ymin, double ymax,
                                             BayesianMarginal::MarginalMode marginalMode) const
  {
    assert(std::find(fSysts.begin(), fSysts.end(), xsyst) != fSysts.end());
    assert(std::find(fSysts.begin(), fSysts.end(), ysyst) != fSysts.end());
    return BayesianSurface(*this,
                           xsyst, nbinsx, xmin, xmax,
                           ysyst, nbinsy, ymin, ymax,
                           marginalMode);
  }

  //----------------------------------------------------------------------
  template <typename T>
  double MCMCSamples::MaxValue(const T *var) const
  {
    static_assert(std::is_same<IFitVar, T>::value || std::is_same<ISyst, T>::value,
                  "MCMCSamples::MaxValue() can only be used with IFitVars and ISysts");
    double max = -std::numeric_limits<double>::infinity();
    std::size_t offset = VarOffset(var);
    for (int idx = 0; idx < fSamples->GetEntries(); idx++)
    {
      double val = SampleValue(idx, var->ShortName(), offset);
      if (val > max)
        max = val;
    }
    return max;
  }
  // explicit instantiation of the correct types
  template double MCMCSamples::MaxValue(const IFitVar *) const;
  template double MCMCSamples::MaxValue(const ISyst *) const;

  //----------------------------------------------------------------------
  template <typename T>
  double MCMCSamples::MinValue(const T *var) const
  {
    static_assert(std::is_same<IFitVar, T>::value || std::is_same<ISyst, T>::value,
                  "MCMCSamples::MinValue() can only be used with IFitVars and ISysts");
    double min = std::numeric_limits<double>::infinity();
    std::size_t offset = VarOffset(var);
    for (int idx = 0; idx < fSamples->GetEntries(); idx++)
    {
      double val = SampleValue(idx, var->ShortName(), offset);
      if (val < min)
        min = val;
    }
    return min;
  }
  // explicit instantiation of the correct types
  template double MCMCSamples::MinValue(const IFitVar *) const;
  template double MCMCSamples::MinValue(const ISyst *) const;

  //----------------------------------------------------------------------
  void MCMCSamples::ParseDiagnosticBranches(const std::vector<std::string>& names)
  {
    std::size_t idx = 1;
    fDiagnosticVals.resize(names.size() - (fVars.size() + fSysts.size() + 1));
    auto firstFitVar = fVars.empty() ? fSysts[0]->ShortName() : fVars[0]->ShortName();
    for (; idx < names.size(); idx++)
    {
      if (names[idx] == firstFitVar)
        break;
      fDiagBranches.push_back(names[idx]);
    }
    assert(idx < names.size() && "Did not find fit variables in Stan output!");
    assert (names.size() - idx >= fVars.size() + fSysts.size() && "Stan output didn't contain all FitVars & Systs!");
  }

  //----------------------------------------------------------------------
  std::pair<std::size_t, double> MCMCSamples::QuantileLL(double quantile) const
  {
    auto LLs = SortedLLs();
    return LLs[std::size_t((1 - quantile) * LLs.size())];
  }

  //----------------------------------------------------------------------
  std::pair<std::size_t, double> MCMCSamples::QuantileLL(double quantile,
      std::vector<std::pair<std::size_t, double>> &LLs) const
  {
    if (LLs.empty())
      LLs = SortedLLs();  // I think this should be efficient since std::vector has a move assignment operator
    return LLs[std::size_t((1 - quantile) * LLs.size())];
  }

  //----------------------------------------------------------------------
  std::map<double, std::pair<std::size_t, double>> MCMCSamples::QuantileLL(const std::vector<double> &quantiles) const
  {
    std::map<double, std::pair<std::size_t, double> > ret;
    std::vector<std::pair<std::size_t, double> > LLs;
    for (const auto & quantile : quantiles)
      ret[quantile] = QuantileLL(quantile, LLs);

    return ret;
  }

  //----------------------------------------------------------------------
  void MCMCSamples::RunDiagnostics(const StanConfig & cfg) const
  {
    BranchStatusResetter bsr(fSamples.get());  // turn branches off when done

    // these diagnostics adapted from CmdStan's diagnose.cpp
    auto treeDepthBr = fSamples->GetBranch("treedepth__");
    if (treeDepthBr)
    {
      treeDepthBr->SetStatus(true);
      unsigned int numMax = 0;
      auto brOffset = DiagOffset(treeDepthBr->GetName());
      for (int idx = 0; idx < treeDepthBr->GetEntries(); idx++)
      {
        treeDepthBr->GetEntry(idx);
        if (fDiagnosticVals[brOffset] >= cfg.max_depth)
          numMax++;
      }
      if (numMax > 0)
      {
        std::cout << numMax << " of " << treeDepthBr->GetEntries() << " ("
                  << std::setprecision(2)
                  << 100 * static_cast<double>(numMax) / treeDepthBr->GetEntries()
                  << "%) transitions hit the maximum treedepth limit of "
                  << cfg.max_depth << ", or 2^" << cfg.max_depth << " leapfrog steps."
                  << " Trajectories that are prematurely terminated due to this"
                  << " limit will result in slow exploration and you should"
                  << " increase the limit to ensure optimal performance."
                  << std::endl << std::endl;

      }
    }

    auto divergentBr = fSamples->GetBranch("divergent__");
    if (divergentBr)
    {
      divergentBr->SetStatus(true);
      unsigned int numDiv = 0;
      auto brOffset = DiagOffset(divergentBr->GetName());
      for (int idx = 0; idx < divergentBr->GetEntries(); idx++)
      {
        divergentBr->GetEntry(idx);
        if (fDiagnosticVals[brOffset] >0)
          numDiv++;
      }
      if (numDiv > 0)
        std::cout << numDiv << " of " << divergentBr->GetEntries() << " ("
                  << std::setprecision(2)
                  << 100 * static_cast<double>(numDiv) / divergentBr->GetEntries()
                  << "%) transitions ended with a divergence.  These divergent"
                  << " transitions indicate that HMC is not fully able to"
                  << " explore the posterior distribution.  Try rerunning with"
                  << " adapt_delta set to a larger value and see if the"
                  << " divergences vanish.  If increasing adapt delta towards"
                  << " 1 does not remove the divergences then you will likely"
                  << " need to reparameterize your model."
                  << std::endl << std::endl;
    }

    auto energyBr = fSamples->GetBranch("energy__");
    if (energyBr)
    {
      double delta_e_sq_mean = 0;
      double e_mean = 0;
      double e_var = 0;

      double e_sample = 0;
      double e_sample_prev = 0;

      auto brOffset = DiagOffset(energyBr->GetName());
      for (int idx = 0; idx < energyBr->GetEntries(); idx++)
      {
        energyBr->SetStatus(true);
        energyBr->GetEntry(idx);
        e_sample = fDiagnosticVals[brOffset];

        double delta_e_sq = util::sqr(e_sample - e_sample_prev);
        double d = delta_e_sq - delta_e_sq_mean;
        delta_e_sq_mean += d / (idx + 1);

        d = e_sample - e_mean;
        e_mean += d / (idx + 2);
        e_var += d * (e_sample - e_mean);

        e_sample_prev = e_sample;
      }

      e_var /= static_cast<double>(energyBr->GetEntries() - 1);

      double e_bfmi = delta_e_sq_mean / e_var;

      const double e_bfmi_threshold = 0.3;
      if (e_bfmi < e_bfmi_threshold)
        std::cout << "The E-BFMI, " << e_bfmi << ", is below the nominal"
                  << " threshold of " << e_bfmi_threshold << " which suggests"
                  << " that HMC may have trouble exploring the target"
                  << " distribution.  You should consider any"
                  << " reparameterizations if possible."
                  << std::endl << std::endl;
    }

    // unfortunately the remaining diagnostics really require a stan::mcmc::chains<> object.
    // this presents an annoying issue: the TTree storage is necessary for use with RooNDKeysPDF,
    // and is much more convenient for persistence.
    // even more unfortunately, these are the most important diagnostics.
    // Should we build a stan::mcmc::chains from it?
    // todo: decide how to implement remaining diagnostics
/*
  else if (chains.param_name(i).find("__") == std::string::npos) {

        double n_eff = chains.effective_sample_size(i);
        if (n_eff / num_samples < 0.001)
          bad_n_eff_names.push_back(chains.param_name(i));

        double split_rhat = chains.split_potential_scale_reduction(i);
        if (split_rhat > 1.1)
          bad_rhat_names.push_back(chains.param_name(i));
      }
    }

    if (bad_n_eff_names.size() > 0) {
      std::cout << "The following parameters had fewer than 0.001 effective"
                << " samples per transition:" << std::endl;
      std::cout << "  ";
      for (size_t n = 0; n < bad_n_eff_names.size() - 1; ++n)
        std::cout << bad_n_eff_names.at(n) << ", ";
      std::cout << bad_n_eff_names.back() << std::endl;

      std::cout << "Such low values indicate that the effective sample size"
                << " estimators may be biased high and actual performance"
                << " may be substantially lower than quoted."
                << std::endl << std::endl;
    }

    if (bad_rhat_names.size() > 0) {
      std::cout << "The following parameters had split R-hat less than 1.1:"
                << std::endl;
      std::cout << "  ";
      for (size_t n = 0; n < bad_rhat_names.size() - 1; ++n)
        std::cout << bad_rhat_names.at(n) << ", ";
      std::cout << bad_rhat_names.back() << std::endl;

      std::cout << "Such high values indicate incomplete mixing and biased"
                << "estimation.  You should consider regularization your model"
                << " with additional prior information or looking for a more"
                << " effective parameterization."
                << std::endl << std::endl;
    }
*/

  }

  //----------------------------------------------------------------------
  double MCMCSamples::SampleValue(std::size_t rowIdx, const std::string & branchName, std::size_t varIdx) const
  {
    BranchStatusResetter bsr(fSamples.get());  // turn branches off when done

    auto branch = fSamples->GetBranch(branchName.c_str());
    branch->SetStatus(true);
    branch->GetEntry(rowIdx);
    return fEntryVals[varIdx];
  }

  //----------------------------------------------------------------------
  void MCMCSamples::SampleValues(std::size_t idx,
                                 const std::vector<const IFitVar *> &vars,
                                 std::map<const IFitVar *, double> &varVals) const
  {
    for (const auto &var : vars)
      varVals[var] = SampleValue(var, idx);
  }

  //----------------------------------------------------------------------
  void MCMCSamples::SampleValues(std::size_t idx,
                                 const std::vector<const IFitVar *> &vars,
                                 std::map<const IFitVar *, double> &varVals,
                                 const std::vector<const ana::ISyst *> &systs,
                                 std::map<const ana::ISyst *, double> &systVals) const
  {
    SampleValues(idx, vars, varVals);
    for (const auto &syst : systs)
      systVals[syst] = SampleValue(syst, idx);
  }


  //----------------------------------------------------------------------
  void MCMCSamples::SaveTo(TDirectory *dir, const std::string& name) const
  {
    TDirectory * oldDir = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("MCMCSamples").Write("type");

    TParameter<int> offset("offset", fOffset);
    offset.Write();

    TList diagBranchNames;
    diagBranchNames.SetOwner();
    for (const auto & brName : fDiagBranches)
      diagBranchNames.AddLast(new TObjString(brName.c_str()));
    diagBranchNames.Write("diagBranchNames", TObject::kSingleKey);

    TList fitVarNames;
    fitVarNames.SetOwner();
    for (const auto & var : fVars)
      fitVarNames.AddLast(new TObjString(var->ShortName().c_str()));
    fitVarNames.Write("fitVarNames", TObject::kSingleKey);

    TList systNames;
    systNames.SetOwner();
    for (const auto & syst : fSysts)
      systNames.AddLast(new TObjString(syst->ShortName().c_str()));
    systNames.Write("systNames", TObject::kSingleKey);

    // re-enable all the branches before writing...
    fSamples->SetBranchStatus("*", true);
    fSamples->Write("samples");
    // don't let it get attached to the file that's about to be closed
    fSamples->LoadBaskets();
    fSamples->SetDirectory(nullptr);

    auto hyperdir = dir->mkdir("hyperparams");
    hyperdir->cd();
    TParameter<double> stepSize("stepsize", fHyperparams.stepSize);
    stepSize.Write();
    if (fHyperparams.invMetric)
      fHyperparams.invMetric->Write("inv_metric");
    dir->cd();
    hyperdir->Write();

    dir->Write();
    delete dir;

    if (oldDir)
      oldDir->cd();
  }

  //----------------------------------------------------------------------
  void MCMCSamples::SetNames(const std::vector<std::string> &names)
  {
    assert (fOffset == 0 && "MCMCSamples::SetNames() was called after tree was already set up!");

    ParseDiagnosticBranches(names);
    SetupTree();
  }

  //----------------------------------------------------------------------
  void MCMCSamples::SetupTree()
  {
    auto CreateOrSetAddress = [](TTree * tree, const std::string& brName, double* address)
    {
      auto name = brName.c_str();
      TBranch * br = tree->GetBranch(name);
      if (br)
        br->SetAddress(address);
      else
        tree->Branch(name, address);
      return br;
    };

    CreateOrSetAddress(fSamples.get(), MCMCSamples::LOGLIKELIHOOD_BRANCH_NAME, &fEntryLL);

    fDiagnosticVals.clear();
    fDiagnosticVals.resize(fDiagBranches.size(), std::numeric_limits<double>::signaling_NaN());
    assert(!fVars.empty() || !fSysts.empty());
    auto firstFitVar = fVars.empty() ? fSysts[0]->ShortName() : fVars[0]->ShortName();
    for (std::size_t valIdx = 0; valIdx < fDiagBranches.size(); valIdx++)
      CreateOrSetAddress(fSamples.get(), fDiagBranches[valIdx], &fDiagnosticVals[valIdx]);

    fOffset = 1 + fDiagBranches.size(); //  + 1 for the LL at the beginning

    fEntryVals.clear();
    fEntryVals.resize(fVars.size() + fSysts.size(), std::numeric_limits<double>::signaling_NaN());
    std::size_t valIdx = 0;
    for (const auto &var : fVars)
      CreateOrSetAddress(fSamples.get(), var->ShortName(), &fEntryVals[valIdx++]);
    for (const auto &syst : fSysts)
      CreateOrSetAddress(fSamples.get(), syst->ShortName(), &fEntryVals[valIdx++]);

    // disable the 'autosave' and 'autoflush' mechanisms
    // since there's nowhere to write to anyway
    // (this tree is memory-only unless SaveTo()'d).
    fSamples->SetAutoFlush(0);
    fSamples->SetAutoSave(0);
  }

  //----------------------------------------------------------------------
  std::vector<std::pair<std::size_t, double>> MCMCSamples::SortedLLs() const
  {
    std::vector<std::pair<std::size_t, double>> LLs;
    LLs.reserve(fSamples->GetEntries());
    auto branch = fSamples->GetBranch(MCMCSamples::LOGLIKELIHOOD_BRANCH_NAME.c_str());
    fSamples->SetBranchStatus("*", false);
    fSamples->SetBranchStatus(MCMCSamples::LOGLIKELIHOOD_BRANCH_NAME.c_str(), true);
    for (int idx = 0; idx < fSamples->GetEntries(); idx++)
    {
      branch->GetEntry(idx);
      LLs.push_back(std::make_pair(idx, fEntryLL));
    }

    std::sort(LLs.begin(), LLs.end(),
        [](const std::pair<std::size_t, double> & a, const std::pair<std::size_t, double> & b)
        {
          return a.second < b.second;
        }
    );

    return std::move(LLs);
  }

  //----------------------------------------------------------------------
  const TTree *MCMCSamples::ToTTree() const
  {
    // presumably user doesn't want the tree with some branches missing...
    fSamples->SetBranchStatus("*", true);
    return fSamples.get();
  }

  //----------------------------------------------------------------------
  std::size_t MCMCSamples::VarOffset(const IFitVar *var) const
  {
    auto itr = std::find(fVars.begin(), fVars.end(), var);
    assert(itr != fVars.end() && ("Var was not fitted: " + var->ShortName()).c_str());
    return std::distance(fVars.begin(), itr);
  }

  //----------------------------------------------------------------------
  std::size_t MCMCSamples::VarOffset(const ana::ISyst *syst) const
  {
    auto itr = std::find(fSysts.begin(), fSysts.end(), syst);
    assert(itr != fSysts.end() && ("Syst was not fitted: " + syst->ShortName()).c_str());
    return fVars.size() + std::distance(fSysts.begin(), itr);
  }



}
