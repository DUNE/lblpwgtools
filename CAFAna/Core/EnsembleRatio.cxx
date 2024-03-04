#include "CAFAna/Core/EnsembleRatio.h"

#include "CAFAna/Core/EnsembleSpectrum.h"

#include "CAFAna/Core/FitMultiverse.h"
#include "CAFAna/Core/Utilities.h"

#include "cafanacore/UtilsExt.h"

#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"

#include <iostream>

namespace ana
{
  //----------------------------------------------------------------------
  EnsembleRatio::EnsembleRatio(const EnsembleSpectrum& num,
                               const EnsembleSpectrum& denom,
                               bool purOrEffErrs)
    : fMultiverse(&num.GetMultiverse()),
      fHist(num.fHist),
      fAxis(num.GetLabels(), num.GetBinnings())
  {
    CheckMultiverses(denom.GetMultiverse(), __func__);

    fHist.Divide(denom.fHist);
    // TODO TODO this should handle livetime too
    fHist.Scale(denom.POT()/num.POT());

    // This is clumsy, but the old histogram operation considered 0/0 = 0,
    // which is actually pretty useful (at least PredictionInterp relies on
    // this).
    for(int i = 0; i < fHist.GetNbinsX()+2; ++i){
      if(denom.fHist.GetBinContent(i) == 0){
        if(num.fHist.GetBinContent(i) == 0){
          fHist.SetBinContent(i, 0);
        }
        else{
          // Actual infinities break ROOT plotting
          fHist.SetBinContent(i, 1e100);
          // As in fact do merely very large numbers
          // fHist.SetBinContent(i, std::numeric_limits<double>::max());
        }
      }
    }

    if(purOrEffErrs){
      if(!AlmostEqual(num.POT(), denom.POT())){
        std::cout << "EnsembleRatio: Warning: creating purity or efficiency ratio between two spectra with different POTs"
                  << "\n  " << num.POT() << " vs " << denom.POT()
                  << "\n  That doesn't make much sense" << std::endl;
      }

      Eigen::ArrayXd errsqarr(fHist.GetNbinsX()+2);

      const double kOneSigma = .6827;
      for(int i = 0; i < fHist.GetNbinsX()+2; ++i){
        const double n = num.fHist.GetBinContent(i);
        const double d = denom.fHist.GetBinContent(i);

        // Clopper-Peason is the TEfficiency default and "recommended by the
        // PDG". If the user wants something else (for rendering purposes) they
        // should use TEfficiency directly.
        const double up = TEfficiency::ClopperPearson(n+d, n, kOneSigma, true);
        const double dn = TEfficiency::ClopperPearson(n+d, n, kOneSigma, false);

        // Crudely symmetrizes asymmetric errors. The underlying Hist class
        // can't cope with that, only really makes sense for plotting. In which
        // case the user should do it themselves.
        errsqarr[i] = util::sqr((up-dn)/2);
      } // end for i

      Eigen::ArrayXd valarr = fHist.GetEigen();
      // Replace fHist with same values but new errors
      fHist = Hist::AdoptWithErrors(std::move(valarr), std::move(errsqarr));
    }
  }

  //----------------------------------------------------------------------
  unsigned int EnsembleRatio::NUniverses() const
  {
    return fMultiverse->NUniv();
  }

  //----------------------------------------------------------------------
  Ratio EnsembleRatio::Universe(unsigned int univIdx) const
  {
    const int nbins = fAxis.GetBins1D().NBins()+2;

    if(fHist.HasStan()){
      return Ratio(std::move(Eigen::ArrayXstan(fHist.GetEigenStan().segment(nbins*univIdx, nbins))),
                   fAxis.GetLabels(), fAxis.GetBinnings());
    }
    else{
      Eigen::ArrayXd arr(fHist.GetEigen().segment(nbins*univIdx, nbins));

      if(Hist::StatErrorsEnabled()){
        Eigen::ArrayXd errarr(fHist.GetEigenSqErrors().segment(nbins*univIdx, nbins));
        return Ratio(std::move(arr), std::move(errarr),
                     fAxis.GetLabels(), fAxis.GetBinnings());
      }
      else{
        return Ratio(std::move(arr), fAxis.GetLabels(), fAxis.GetBinnings());
      }
    }
  }

  //----------------------------------------------------------------------
  TGraphAsymmErrors* EnsembleRatio::ErrorBand() const
  {
    // TODO lots of code duplication with EnsembleSpectrum

    const Eigen::ArrayXd arr = fHist.GetEigen();

    TGraphAsymmErrors* g = new TGraphAsymmErrors;

    const int nbins = fAxis.GetBins1D().NBins()+2;
    const std::vector<double>& edges = fAxis.GetBins1D().Edges();

    for(int binIdx = 1; binIdx+1 < nbins; ++binIdx){
      const double xnom = (edges[binIdx-1] + edges[binIdx]) / 2; // bin center
      const double ynom = arr[binIdx];
      g->SetPoint(binIdx-1, xnom, ynom);

      const double dx = edges[binIdx] - edges[binIdx-1];

      std::vector<double> ys;
      ys.reserve(NUniverses()-1);
      for(unsigned int univIdx = 1; univIdx < NUniverses(); ++univIdx)
        ys.push_back(arr[univIdx*nbins + binIdx]);

      // 1 sigma
      const double y0 = FindQuantile(.5-0.6827/2, ys);
      const double y1 = FindQuantile(.5+0.6827/2, ys);

      // It's theoretically possible for the central value to be outside the
      // error bands - clamp to zero in that case
      g->SetPointError(binIdx-1, dx/2, dx/2,
                       std::max(ynom-y0, 0.),
                       std::max(y1-ynom, 0.));
    } // end for binIdx

    return g;
  }

  //----------------------------------------------------------------------
  Eigen::MatrixXd EnsembleRatio::CovarianceMatrix()
  {
    assert (fMultiverse->MultiverseType() == kRandomGas);

    const Eigen::ArrayXd& arr = fHist.GetEigen();

    const int nbins = fAxis.GetBins1D().NBins()+2;
    std::vector<Eigen::ArrayXd> histVec;
    histVec.reserve(NUniverses());

    for(unsigned int univIdx = 1; univIdx < NUniverses(); ++univIdx)
      histVec.push_back(arr.segment(nbins*univIdx, nbins));

    return ana::CalcCovMx(histVec);// returns TMatrixD
  }

  //----------------------------------------------------------------------
  Eigen::MatrixXd EnsembleRatio::BiasMatrix()
  {
    assert (fMultiverse->MultiverseType() == kRandomGas);

    const Eigen::ArrayXd& arr = fHist.GetEigen();

    const int nbins = fAxis.GetBins1D().NBins()+2;
    std::vector<Eigen::ArrayXd> histVec;
    histVec.reserve(NUniverses());

    for(unsigned int univIdx = 1; univIdx < NUniverses(); ++univIdx)
      histVec.push_back(arr.segment(nbins*univIdx, nbins));

    return ana::CalcBiasMx(arr.segment(0, nbins), histVec);
  }
  //----------------------------------------------------------------------
  void EnsembleRatio::CheckMultiverses(const FitMultiverse& rhs,
                                       const std::string& func) const
  {
    if(&rhs == fMultiverse) return;

    std::cout << "EnsembleRatio::" << func << ": attempting to combine two spectra made with different multiverses: " << std::endl;
    std::cout << "  " << fMultiverse->ShortName() << std::endl;
    std::cout << "vs" << std::endl;
    std::cout << rhs.ShortName() << std::endl;
    abort();
  }

  //----------------------------------------------------------------------
  EnsembleRatio& EnsembleRatio::operator*=(const EnsembleRatio& rhs)
  {
    CheckMultiverses(rhs.GetMultiverse(), __func__);

    fHist.Multiply(rhs.fHist);

    return *this;
  }

  //----------------------------------------------------------------------
  EnsembleRatio EnsembleRatio::operator*(const EnsembleRatio& rhs) const
  {
    EnsembleRatio ret = *this;
    ret *= rhs;
    return ret;
  }

  //----------------------------------------------------------------------
  EnsembleRatio& EnsembleRatio::operator/=(const EnsembleRatio& rhs)
  {
    CheckMultiverses(rhs.GetMultiverse(), __func__);

    fHist.Divide(rhs.fHist);

    return *this;
  }

  //----------------------------------------------------------------------
  EnsembleRatio EnsembleRatio::operator/(const EnsembleRatio& rhs) const
  {
    EnsembleRatio ret = *this;
    ret /= rhs;
    return ret;
  }
}