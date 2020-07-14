#include "CAFAna/Fit/BayesianMarginal.h"

#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TFile.h"
#include "TMap.h"
#include "TParameter.h"
#include "TSystem.h"
#include "TMVA/Config.h"
#include "TMVA/DataLoader.h"
#include "TMVA/DataInputHandler.h"
#include "TMVA/Factory.h"
#include "TMVA/MethodKNN.h"
#include "TMVA/Types.h"

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Utilities.h"
#include "CAFAna/Fit/MCMCSamples.h"


namespace ana
{

  // initialize the static members
  const std::string BayesianMarginal::sKNNName = "bayes_marginal_knn";
  const std::vector<std::pair<Quantile, double>> BayesianMarginal::QuantileUpVals
  {
      {Quantile::kGaussian1Sigma,  0.6827},
      {Quantile::kGaussian2Sigma, 0.9545},
      {Quantile::kGaussian3Sigma, 0.9973},
      {Quantile::k0pc, 0},
      {Quantile::k90pc, 0.9},
      {Quantile::k95pc, 0.95},
      {Quantile::k100pc, 1},
  };


//----------------------------------------------------------------------
  BayesianMarginal::BayesianMarginal(const MCMCSamples &samples,
                                     const std::vector<IFitVarOrISyst> & toFit,
                                     MarginalMode mode)
    : fMode(mode), fMCMCSamples(&samples)
  {
    for (const auto & registerable : toFit)
    {
      if(registerable.ifitvar){
        fVars.push_back(registerable.ifitvar);
        fOrderedBrNames.push_back(registerable.ifitvar->ShortName());
      }
      else if (registerable.isyst){
        fSysts.push_back(registerable.isyst);
        fOrderedBrNames.push_back(registerable.isyst->ShortName());
      }
      else{
        assert(false && "BayesianMarginal can only marginalize over IFitVars or ISysts");
      }
    }

    if (mode == MarginalMode::kKNN && samples.NumSamples() < 100)
    {
      // the kNN requires at least 100 points
      std::cerr << "BayesianMarginal requires at least 100 MCMC samples to create a marginal distribution from them." << std::endl;
      abort();
    }

    for (const auto & var : fVars)
    {
      if (std::find(samples.Vars().begin(), samples.Vars().end(), var) == samples.Vars().end())
      {
        std::cerr << "Error: Requesting to marginalize over IFitVar not in MCMCSamples: " << var->ShortName() << std::endl;
        abort();
      }
    }
    for (const auto & syst : fSysts)
    {
      if (std::find(samples.Systs().begin(), samples.Systs().end(), syst) == samples.Systs().end())
      {
        std::cerr << "Error: Requesting to marginalize over ISyst not in MCMCSamples: " << syst->ShortName() << std::endl;
        abort();
      }
    }

    if (fMode == MarginalMode::kKNN)
    {
      // load the data & train...
      // Use TMVA to build an k-nearest-neightbor estimator for the LL surface
      std::cout << "  Building a kNN from the MCMC points to estimate marginals..." << std::flush;
      bool oldSilent = TMVA::gConfig().IsSilent();
      TMVA::gConfig().SetSilent(true);
      fdataLoader = std::make_unique<TMVA::DataLoader>(sKNNName.c_str());

      // we'd rather not mess around with the original tree,
      // but passing a tree to AddRegressionTree() implicitly
      // gives the kNN builder permission to modify the tree
      // because it needs to be able to SetBranchAddress() etc.
      // unfortunately, there is no 'const' method for cloning a tree
      // because the clone comes back 'connected' (with all the same
      // branch addresses etc.) as the original.
      // so, we use some const_cast<>s just long enough to make a copy
      // and then disconnect it in the next line.
      // (that's what the second line is doing, despite the confusing method name,
      //  and the fact that it looks like it's called on the wrong object;
      //  see https://root.cern/doc/master/classTTree.html#a67af1e2908bc275c03268f84f1f2b282)
      // stuff into a unique_ptr<> so taht this copy disappears as soon as training is over.
      auto trainTree = std::unique_ptr<TTree>(const_cast<TTree *>(samples.ToTTree())->CloneTree());
      const_cast<TTree *>(samples.ToTTree())->CopyAddresses(trainTree.get(), true);
      const_cast<TTree *>(samples.ToTTree())->GetListOfClones()->Remove(trainTree.get());

      fdataLoader->AddRegressionTree(trainTree.get());
      SetupKNN(fdataLoader.get());

      fkNN->Train();
      TMVA::gConfig().SetSilent(oldSilent);
      std::cout << "  ... done." << std::endl;
    }

    // this is bit painful, but we want to only sort the LLs once in case there are many
    std::vector<double> qVals;
    std::transform(QuantileUpVals.begin(), QuantileUpVals.end(),
                   std::back_inserter(qVals),
                   [](const std::pair<Quantile, double>& p){return p.second;});

    // this is a map from quantile -> {idx, LL}
    std::map<double, std::pair<std::size_t, double>> vals = samples.QuantileLL(qVals);

    for (std::size_t i = 0; i < QuantileUpVals.size(); ++i)
      fQuantileSampleMap.emplace(QuantileUpVals[i].first, samples.Sample(vals[QuantileUpVals[i].second].first));
  }

  //----------------------------------------------------------------------
  BayesianMarginal::BayesianMarginal()
      :  fMode(MarginalMode::kHistogram), fMCMCSamples(nullptr)
  {}

  //----------------------------------------------------------------------
  double BayesianMarginal::EstimateLLFromKNN(const std::vector<float> &vals) const
  {
    assert(fMode == MarginalMode::kKNN);

    assert(vals.size() == fkNN->DataInfo().GetNVariables());

    TMVA::Event ev(vals, fkNN->DataInfo().GetNVariables());
    return dynamic_cast<TMVA::MethodBase*>(fkNN.get())->GetRegressionValues(&ev)[0];
  }

  //----------------------------------------------------------------------
  void BayesianMarginal::LoadInto(BayesianMarginal * marg, TDirectory *dir)
  {
    auto varNames = dynamic_cast<TList*>(dir->Get("vars"));
    for (const auto & obj : *varNames)
      marg->fVars.push_back(Registry<IFitVar>::ShortNameToPtr(obj->GetName()));
    auto systNames = dynamic_cast<TList*>(dir->Get("systs"));
    for (const auto & obj : *systNames)
      marg->fSysts.push_back(Registry<ISyst>::ShortNameToPtr(obj->GetName()));

    auto mode = dynamic_cast<TParameter<int>*>(dir->Get("mode"));
    marg->fMode = MarginalMode(mode->GetVal());

    if (marg->fMode == MarginalMode::kKNN)
    {
      // again we're forced to make a temporary file (see SaveTo()).  c'est la vie...
      marg->fdataLoader = std::make_unique<TMVA::DataLoader>(sKNNName.c_str());
      marg->SetupKNN(marg->fdataLoader.get());
      auto tree = dynamic_cast<TTree *>(dir->Get("kNN_weights"));
      tree->SetDirectory(nullptr);
      TFile tmpFile("tmp_surface.root", "recreate");
      tmpFile.cd();
      tree->Write("knn");
      marg->fkNN->ReadWeightsFromStream(tmpFile);
      tmpFile.Close();
      gSystem->Unlink("tmp_surface.root");
    }

    auto quantMap = dynamic_cast<TMap*>(dir->Get("quantileSamples"));
    for (const auto & quantPair : *quantMap)
    {
      auto pair = dynamic_cast<TPair*>(quantPair);
      auto quant = Quantile(dynamic_cast<TParameter<int>*>(pair->Key())->GetVal());
      marg->fQuantileSampleMap.emplace(std::piecewise_construct,
                                       std::forward_as_tuple(quant),
                                       std::forward_as_tuple(*dynamic_cast<TMap*>(pair->Value()),
                                                             marg->Vars(), marg->Systs()));
    }

  }

  //----------------------------------------------------------------------
  double BayesianMarginal::QuantileThreshold(ana::Quantile quantile, const TH1 * pdf) const
  {
    // in "histogram" mode, the output isn't weighted by LL;
    // what's returned is simply the best-guess pdf
    // approximated by the histogram itself.
    // so instead of simply yielding the LL itself,
    // we need to find the pdf value corresponding to the right breakpoint.
    if (fMode == MarginalMode::kHistogram)
    {
      assert(pdf && "In 'histogram' marginal mode, must supply the histogram for which the quantile threshold is to be calculated");
      return ThresholdFromTH1(pdf, std::find_if(QuantileUpVals.begin(),
                                                QuantileUpVals.end(),
                                                [quantile](const auto & pair) { return pair.first == quantile;})->second);
    }

    return fQuantileSampleMap.at(quantile).LL();
  }

  //----------------------------------------------------------------------
  double BayesianMarginal::QuantileThreshold(double quantile, const MCMCSamples &samples, const TH1 * pdf) const
  {
    if (quantile == -1)
      quantile = 0;
    assert(quantile >= 0 && quantile <= 1 && "QuantileSurface(): quantile must be between 0 and 1");


    auto quantilePair = samples.QuantileLL(quantile);

    if (fMode == MarginalMode::kHistogram)
      return ThresholdFromTH1(pdf, quantile);

    return quantilePair.second;
  }

  //----------------------------------------------------------------------
  void BayesianMarginal::SaveTo(TDirectory *dir) const
  {
    auto oldDir = gDirectory;
    dir->cd();

    TParameter<int> mode("mode", int(fMode));
    mode.Write();

    if (fMode == MarginalMode::kKNN)
    {
      // the fact that the tree gets saved into the file with name 'knn' is an unadvertised implementation detail...
      // but do they need to save it as its own TFile so badly?
      // we want to save it into our own file to keep the bits from getting too spread out.
      TFile tmpFile("tmp_surface.root", "recreate");
      fkNN->WriteWeightsToStream(tmpFile);
      auto tree = dynamic_cast<TTree *>(tmpFile.Get("knn"));
      tree->SetDirectory(nullptr);
      dir->cd();
      tree->Write("kNN_weights");
      tmpFile.Close();
      gSystem->Unlink("tmp_surface.root");
    }

    TMap quantMap;
    for (const auto & quantPair : fQuantileSampleMap)
      quantMap.Add(new TParameter<int>("", int(quantPair.first)), quantPair.second.ToTMap().release());
    quantMap.Write("quantileSamples", TObject::kSingleKey);

    TList varNames;
    for (const auto & var: fVars)
      varNames.Add(new TObjString(var->ShortName().c_str()));
    varNames.Write("vars", TObject::kSingleKey);

    TList systNames;
    for (const auto & syst : fSysts)
      systNames.Add(new TObjString(syst->ShortName().c_str()));
    systNames.Write("systs", TObject::kSingleKey);

    if (oldDir)
      oldDir->cd();
  }

  //----------------------------------------------------------------------
  void BayesianMarginal::SetupKNN(TMVA::DataLoader * loader)
  {
    int entries = 0;
    if (loader)
    {
      entries = loader->DataInput().GetEntries();
      auto opt = Form("KNN,nkNN=%d", entries > 20 ? 20 : entries-1);  // 20 neighbors is kNN default
      fkNN = std::make_unique<TMVA::MethodKNN>(loader->GetDataSetInfo(), opt);
    }
    else
    {
      TMVA::DataSetInfo info(sKNNName.c_str());
      fkNN = std::make_unique<TMVA::MethodKNN>(info, "KNN");
    }

    auto & dsInfo = fkNN->DataInfo();
    for (const auto & var : fVars)
      dsInfo.AddVariable(var->ShortName());
    for (const auto & syst : fSysts)
      dsInfo.AddVariable(syst->ShortName());
    dsInfo.AddTarget(MCMCSamples::LOGLIKELIHOOD_BRANCH_NAME, "", "", 0, 0);
    if (loader)
      dsInfo.SetSplitOptions(Form("nTrain_Regression=%d,nTest_Regression=0", entries));

    fkNN->SetAnalysisType( TMVA::Types::kRegression );
    fkNN->SetupMethod();
    fkNN->ParseOptions();
    fkNN->ProcessSetup();
  }

  double BayesianMarginal::ThresholdFromTH1(const TH1* pdf, double quantile)
  {
    if (pdf->Integral() <= 0)
      return std::numeric_limits<double>::signaling_NaN();

    // easy cases
    if (quantile == 0)
      return 1.0;
    else if (quantile == 1)
      return 0.0;
    else if (quantile < 0 || quantile > 1)
    {
      std::cerr << "BayesianMarginal::ThresholdFromTH1(): requested quantile = " << quantile << " is not between 0 and 1.  Abort" << std::endl;
      abort();
    }

    // check if the histogram I was passed was already unit normalized.
    // if not, make a copy that is
     bool copied = false;
    if (std::abs(pdf->Integral() - 1) > 0.00001)
    {
      pdf = dynamic_cast<TH1*>(pdf->Clone());
      const_cast<TH1*>(pdf)->Scale(1./pdf->Integral());
      copied = true;
    }

    // first make a list of the fractional bin contents.
    std::vector<double> contents(pdf->GetNcells());
    double overflowC = 0;
    double underflowC = 0;
    for (int binIdx = 0; binIdx <= pdf->GetNcells(); binIdx++)
    {
      double binC = pdf->GetBinContent(binIdx);
      if (binC == 0)
        continue;   // don't go inserting useless 0s into the vector to make it much longer and slower to sort

      if (pdf->IsBinOverflow(binIdx))
        overflowC += binC;
      else if (pdf->IsBinUnderflow(binIdx))
        underflowC += binC;

      contents.push_back(binC);
    }
    if ((1 - quantile) < underflowC || (1 - quantile) < overflowC)
    {
      std::cerr << "Warning: Histogram underflow or overflow contains more than requested quantile (" << quantile << ") of events." << std::endl;
      std::cerr << "         Bayesian credible region will be incorrect." << std::endl;
    }

    // then do a search to find the value above which the integral corresponds to the desired quantile.
    // begin at the specified quantile index as a first guess

    std::sort(contents.begin(), contents.end());
    std::size_t idx = (1 - quantile) * (contents.size() - 1);
    auto it = contents.begin();
    decltype(it) oldIt = it;
    std::advance(it, idx);
    double oldSum = std::accumulate(it, contents.end(), 0.0);
    // i.e., are we including too much, so we need to shrink the interval by moving towards the end?
    bool forward = oldSum > quantile;
    while (it != contents.begin() && it != contents.end())
    {
      // if we're exactly right, then we're good
      if (oldSum == quantile)
        return *it;

      std::advance(it, forward ? +1 : -1);
      double newSum = std::accumulate(it, contents.end(), 0.0);

      // if we just crossed over the quantile boundary, we're also done.
      if ((newSum - quantile) / (oldSum - quantile) < 0)
        break;

      oldSum = newSum;
      oldIt = it;
    }

    std::cout << "Deduced threshold for quantile " << quantile << " is " << (*it + *oldIt) / 2 << std::endl;

    // the average is not unbiased (it depends on the steepness of the distribution near the breakpoint)
    // but it's probably closer than the values on either side?
    return (*it + *oldIt) / 2;
  }

  //----------------------------------------------------------------------
  std::unique_ptr<TH1> BayesianMarginal::ToHistogram(const std::vector<Binning> & bins) const
  {
    // idea:
    //  * for mode==kHistogram:
    //    simply histogram the MCMC samples.
    //  * for mode==kLLWgtdHistogram:
    //    find the bin-averaged probability distribution
    //    by making the histogram weighted by exp(LL)
    //    and dividing it by the simple histogram of the events
    assert(fMCMCSamples);
    assert(bins.size() == fOrderedBrNames.size());
    assert(bins.size() > 0 && bins.size() < 3);  // could port NOvA CAFAna 3D hist stuff if needed

    auto axisNameStr = std::accumulate(fOrderedBrNames.begin(), fOrderedBrNames.end(),
                                       std::string(""),
                                       [this](std::string s, const std::string & b)
                                       {

                                         std::string name;
                                         if (auto var = Registry<IFitVar>::ShortNameToPtr(fOrderedBrNames[0], true))
                                             name = var->LatexName();
                                         else if (auto syst = Registry<ISyst>::ShortNameToPtr(fOrderedBrNames[0], true))
                                             name = syst->LatexName();
                                         return s + ";" + name;
                                       });
    std::unique_ptr<TH1> ret;
    if (bins.size() == 1)
      ret.reset(MakeTH1D(UniqueName().c_str(), axisNameStr.c_str(), bins[0]));
    else if (bins.size() == 2)
      ret.reset(MakeTH2D(UniqueName().c_str(), axisNameStr.c_str(), bins[0], bins[1]));
    // we haven't implemented MakeTH3D here.  but won't worry about it for now
    //else if (bins.size() == 3)
    //  ret.reset(MakeTH3D(UniqueName().c_str(), axisNameStr.c_str(), bins[0], bins[1], bins[2]));

    std::unique_ptr<TH1> denom;
    if (bins.size() == 1)
      denom = std::make_unique<TH1D>(*dynamic_cast<TH1D*>(ret.get()));
    else if (bins.size() == 2)
      denom = std::make_unique<TH2D>(*dynamic_cast<TH2D*>(ret.get()));
    //else if (bins.size() == 3)
    //  denom = std::make_unique<TH3D>(*dynamic_cast<TH3D*>(ret.get()));
    denom->SetName(UniqueName().c_str());

    // this could certainly be done less sloppily
    // but I wanted to ensure we don't do lots of expensive lookups in std::maps
    // inside the loop over samples, which could run into the millions of iterations
    union VarOrSyst
    {
      const IFitVar * var;
      const ISyst * syst;
    };
    std::vector<bool> isVar(fVars.size() + fSysts.size(), true);
    std::vector<VarOrSyst> varsOrSysts(fVars.size() + fSysts.size());
    for (std::size_t brIdx = 0; brIdx < fOrderedBrNames.size(); brIdx++)
    {
      const auto & brName = fOrderedBrNames[brIdx];
      if (auto var = Registry<IFitVar>::ShortNameToPtr(brName, true))
        varsOrSysts[brIdx].var = var;
      else if (auto syst = Registry<ISyst>::ShortNameToPtr(brName, true))
      {
        isVar[brIdx] = false;
        varsOrSysts[brIdx].syst = syst;
      }
    }
    for (std::size_t sample = 0; sample < fMCMCSamples->NumSamples(); ++sample)
    {
      double logprob = fMCMCSamples->SampleLL(sample);
      if (std::isnan(logprob))
        std::cerr << "Warning: Encountered NaN log-probability in an MCMC sample.  Other things will probably go wrong..." << std::endl;

      std::map<const IFitVar*, double> varVals;
      std::map<const ISyst*, double> systVals;
      fMCMCSamples->SampleValues(sample, fVars, varVals, fSysts, systVals);
      std::vector<double> vals;
      for (std::size_t brIdx = 0; brIdx < varsOrSysts.size(); ++brIdx)
        vals.push_back(
            isVar[brIdx] ? fMCMCSamples->SampleValue(varsOrSysts[brIdx].var, sample)
                         : fMCMCSamples->SampleValue(varsOrSysts[brIdx].syst, sample));

      // could template this, but probably not worth the effort?
      double numWgt = (fMode == MarginalMode::kHistogram) ? 1.0 : logprob;
      if (bins.size() == 1)
        ret->Fill(vals[0], numWgt);
      else if (bins.size() == 2)
      {
        dynamic_cast<TH2*>(ret.get())->Fill(vals[0], vals[1], numWgt);
        dynamic_cast<TH2*>(denom.get())->Fill(vals[0], vals[1]);
      }
      else if (bins.size() == 3)
      {
        dynamic_cast<TH3*>(ret.get())->Fill(vals[0], vals[1], vals[2], numWgt);
        dynamic_cast<TH3*>(denom.get())->Fill(vals[0], vals[1], vals[2]);
      }
    }

    // now exponentiate the log-probs to get probabilities
    if (fMode == MarginalMode::kLLWgtdHistogram)
    {
      ret->Divide(denom.get());
      for (int binIdx = 0; binIdx < ret->GetNcells(); binIdx++)
      {
        // if it's *exactly* 0, this is a region where there were no samples altogether.  just leave empty
        if (ret->GetBinContent(binIdx) != 0.0)
          ret->SetBinContent(binIdx, exp(ret->GetBinContent(binIdx)));
      }
    }
    else if (fMode == MarginalMode::kHistogram)
    {
      if (ret->Integral() > 0)
        ret->Scale(1. / ret->Integral());
    }

    return ret;
  }
}
