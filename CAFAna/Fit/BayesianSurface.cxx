
#include "TH2F.h"

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Core/Utilities.h"
#include "CAFAna/Fit/BayesianSurface.h"
#include "CAFAna/Fit/MCMCSamples.h"

namespace ana
{
  BayesianSurface::BayesianSurface(const MCMCSamples &samples,
                                   const IFitVar * xvar, int nbinsx, double xmin, double xmax,
                                   const IFitVar * yvar, int nbinsy, double ymin, double ymax,
                                   MarginalMode mode)
      : BayesianMarginal(samples, {xvar, yvar}, mode)
  {
    BuildHist(samples, xvar, nbinsx, xmin, xmax, yvar, nbinsy, ymin, ymax);
  }

  //----------------------------------------------------------------------
  BayesianSurface::BayesianSurface(const MCMCSamples &samples,
                                   const ISyst * xsyst, int nbinsx, double xmin, double xmax,
                                   const IFitVar * yvar, int nbinsy, double ymin, double ymax,
                                   MarginalMode mode)
      : BayesianMarginal(samples, {xsyst, yvar}, mode)
  {
    BuildHist(samples, xsyst, nbinsx, xmin, xmax, yvar, nbinsy, ymin, ymax);
  }

  //----------------------------------------------------------------------
  BayesianSurface::BayesianSurface(const MCMCSamples &samples,
                                   const IFitVar *xvar, int nbinsx, double xmin, double xmax,
                                   const ISyst *ysyst, int nbinsy, double ymin, double ymax,
                                   MarginalMode mode)
      : BayesianMarginal(samples, {xvar, ysyst}, mode)
  {
    BuildHist(samples, xvar, nbinsx, xmin, xmax, ysyst, nbinsy, ymin, ymax);
  }

  //----------------------------------------------------------------------
  BayesianSurface::BayesianSurface(const MCMCSamples &samples,
                                   const ISyst *xsyst, int nbinsx, double xmin, double xmax,
                                   const ISyst *ysyst, int nbinsy, double ymin, double ymax,
                                   MarginalMode mode)
      : BayesianMarginal(samples, {xsyst, ysyst}, mode)
  {
    BuildHist(samples, xsyst, nbinsx, xmin, xmax, ysyst, nbinsy, ymin, ymax);
  }

  //----------------------------------------------------------------------
  template <typename SystOrVar1, typename SystOrVar2>
  void BayesianSurface::BuildHist(const MCMCSamples &samples,
                                  const SystOrVar1 *x, int nbinsx, double xmin, double xmax,
                                  const SystOrVar2 *y, int nbinsy, double ymin, double ymax)
  {
    static_assert( (   (std::is_same<SystOrVar1, IFitVar>::value || std::is_same<SystOrVar1, ISyst>::value)
                    && (std::is_same<SystOrVar2, IFitVar>::value || std::is_same<SystOrVar2, ISyst>::value)),
                   "BayesianSurface::BuildHist() can only be used with IFitVar or ISyst objects");

    auto bestSampleIdx = samples.BestFitSampleIdx();
    fBestLikelihood = samples.SampleLL(bestSampleIdx);
    fBestFitX = samples.SampleValue(x, bestSampleIdx);
    fBestFitY = samples.SampleValue(y, bestSampleIdx);

    fHist = ExpandedHistogram(";"+x->LatexName()+";"+y->LatexName(),
                              nbinsx, xmin, xmax,
                              nbinsy, ymin, ymax);

    std::unique_ptr<TH1> tmpHist;
    if (fMode == MarginalMode::kHistogram || fMode == MarginalMode::kLLWgtdHistogram)
      tmpHist = ToHistogram({Binning::Simple(nbinsx, xmin, xmax),
                             Binning::Simple(nbinsy, ymin, ymax)});
    for (int xBin = 0; xBin < fHist->GetNbinsX() + 2; xBin++)
    {
      for (int yBin = 0; yBin < fHist->GetNbinsY() + 2; yBin++)
      {
        double val = std::numeric_limits<double>::signaling_NaN();
        if ((fMode == MarginalMode::kHistogram || fMode == MarginalMode::kLLWgtdHistogram) && tmpHist)
          val = tmpHist->GetBinContent(xBin, yBin);
        else if (fMode == MarginalMode::kKNN)
        {
          float xBinCtr = fHist->GetXaxis()->GetBinCenter(xBin);
          float yBinCtr = fHist->GetYaxis()->GetBinCenter(yBin);
          val = EstimateLLFromKNN({xBinCtr, yBinCtr});
        }
        fHist->SetBinContent(xBin, yBin, val);
      }
    }
  }
  // explicitly instantiate the ones we need
  template void BayesianSurface::BuildHist(const MCMCSamples & samples,
                                           const IFitVar * x, int nbinsx, double xmin, double xmax,
                                           const IFitVar * y, int nbinsy, double ymin, double ymax);
  template void BayesianSurface::BuildHist(const MCMCSamples & samples,
                                           const IFitVar * x, int nbinsx, double xmin, double xmax,
                                           const ISyst * y, int nbinsy, double ymin, double ymax);
  template void BayesianSurface::BuildHist(const MCMCSamples & samples,
                                           const ISyst * x, int nbinsx, double xmin, double xmax,
                                           const IFitVar * y, int nbinsy, double ymin, double ymax);
  template void BayesianSurface::BuildHist(const MCMCSamples & samples,
                                           const ISyst * x, int nbinsx, double xmin, double xmax,
                                           const ISyst * y, int nbinsy, double ymin, double ymax);

  //----------------------------------------------------------------------
  std::unique_ptr<BayesianSurface> BayesianSurface::LoadFrom(TDirectory *dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    DontAddDirectory guard;

    auto tag = (TObjString *) dir->Get("type");
    assert(tag);
    assert(tag->GetString() == "BayesianSurface");

    // can't use make_unique<> because the one-arg constructor is protected
    // and the std::forward() inside make_unique can't forward the visibility
    std::unique_ptr<BayesianSurface> surf(new BayesianSurface);
    ISurface::FillSurfObj(*surf, dir);

    BayesianMarginal::LoadInto(surf.get(), dir);

    delete dir;

    return std::move(surf);
  }

  //----------------------------------------------------------------------
  TH2 * BayesianSurface::QuantileSurface(Quantile quantile) const
  {
    return Flat(QuantileThreshold(quantile, fHist), *this);
  }


  //----------------------------------------------------------------------
  TH2 * BayesianSurface::QuantileSurface(double quantile, const MCMCSamples& mcmcsamples) const
  {
    return Flat(QuantileThreshold(quantile, mcmcsamples, fHist), *this);
  }

  //----------------------------------------------------------------------
  void BayesianSurface::SaveTo(TDirectory *dir) const
  {
    auto oldDir = gDirectory;
    dir->cd();

    dir->cd();
    TObjString("BayesianSurface").Write("type");

    ISurface::SaveToHelper(dir);

    BayesianMarginal::SaveTo(dir);

    if (oldDir)
      oldDir->cd();
  }


}
