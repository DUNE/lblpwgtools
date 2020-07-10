#include "CAFAna/Fit/Bayesian1DMarginal.h"

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Core/Utilities.h"
#include "CAFAna/Fit/MCMCSamples.h"

#include "Utilities/func/Stan.h"

#include "TH1D.h"

#include <cmath>

namespace ana
{
  //----------------------------------------------------------------------
  Bayesian1DMarginal::Bayesian1DMarginal(const MCMCSamples &samples, const IFitVar *var, MarginalMode mode)
      : BayesianMarginal(samples, {var}, mode)
  {}

  //----------------------------------------------------------------------
  Bayesian1DMarginal::Bayesian1DMarginal(const MCMCSamples &samples, const ISyst *syst, MarginalMode mode)
      : BayesianMarginal(samples, {syst}, mode)
  {}

  //----------------------------------------------------------------------
  TH1D Bayesian1DMarginal::ToTH1(const Binning & bins) const
  {
    if (bins == *fCachedBinning)
      return *fCachedHist;

    *fCachedBinning = bins;

    std::string name;
    if (Vars().size() > 0)
      name = Vars()[0]->LatexName();
    else if (Systs().size() > 0)
      name = Systs()[0]->LatexName();
    if (fMode == MarginalMode::kHistogram || fMode == MarginalMode::kLLWgtdHistogram)
      fCachedHist.reset(dynamic_cast<TH1D*>(ToHistogram({bins}).get()));
    else if (fMode == MarginalMode::kKNN)
    {
      fCachedHist = std::make_unique<TH1D>(UniqueName().c_str(), (";"+name).c_str(), bins.NBins(), &bins.Edges()[0]);
      for (int bin = 1; bin <= bins.NBins(); bin++)
        fCachedHist->SetBinContent(bin, EstimateLLFromKNN({float(fCachedHist->GetBinCenter(bin))}));
    }
    return *fCachedHist;
  }


  //----------------------------------------------------------------------
  std::vector<std::pair<double, double>> Bayesian1DMarginal::QuantileRanges(Quantile quantile, const Binning & bins) const
  {
    double threshold = QuantileLL(quantile);

    assert(bins.NBins() >= 2);

    std::unique_ptr<TH1> hist;
    if (fMode == MarginalMode::kHistogram || fMode == MarginalMode::kLLWgtdHistogram)
      hist = ToHistogram({bins});

    // walk through the bins.
    // if we cross from above to below the threshold (or vice versa),
    // take that as a boundary of a range
    std::vector<std::pair<double, double>> ranges;
    std::pair<double, double> pair;
    bool above = false;
    for (unsigned int bin = 0; bin < bins.Edges().size(); ++bin)
    {

      double bnd, y;
      if ((fMode == MarginalMode::kHistogram || fMode == MarginalMode::kLLWgtdHistogram) && hist)
      {
        // the uppermost bin edge isn't the right thing to use when we use bin centers, like for the hist
        if (bin >= bins.Edges().size())
          break;

        bnd = hist->GetBinCenter(bin);
        y = hist->GetBinContent(bin);
      }
      else
      {
        bnd = bins.Edges()[bin];
        y = EstimateLLFromKNN({float(bnd)});
      }

      // only do anything if we are changing from being above threshold to below
      // or vice versa
      bool transition = (y >= threshold && !above) || (y < threshold && above)
                         || bin==0 || bin>=bins.Edges().size()-1;
      if (!transition)
        continue;

      // we're opening a new interval if we went from below to above threshold
      bool opening = !above;
      double &val = opening ? pair.first : pair.second;
      val = bnd;

      // store the pair if we're closing it
      if (!opening)
        ranges.emplace_back(pair);

      above = y >= threshold;
    }

    return ranges;
  }

}
