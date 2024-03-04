#include "CAFAna/Core/EnsembleSpectrum.h"

#include "CAFAna/Core/EnsembleRatio.h"

#include "CAFAna/Core/FitMultiverse.h"
#include "CAFAna/Core/Utilities.h"

#include "TDirectory.h"
#include "TGraphAsymmErrors.h"
#include "TH1.h"
#include "TObjString.h"
#include "TPad.h"

namespace ana
{
  //----------------------------------------------------------------------
  EnsembleSpectrum::EnsembleSpectrum(IValueEnsembleSource& src,
                                     const LabelsAndBins& axis)
    : fMultiverse(src.GetMultiverse()),
      fHist(Hist::Zero((axis.GetBins1D().NBins()+2) * fMultiverse->NUniv())),
      fPOT(0), fLivetime(0),
      fAxis(axis)
  {
    src.Register(this);
  }

  //----------------------------------------------------------------------
  EnsembleSpectrum EnsembleSpectrum::ReplicatedData(const Spectrum& spec, const FitMultiverse* multiverse)
  {
    Eigen::ArrayXd data = spec.GetEigen().replicate(multiverse->NUniv(), 1);

    return EnsembleSpectrum(multiverse, Hist::Adopt(std::move(data)), spec.POT(), spec.Livetime(),
                            LabelsAndBins(spec.GetLabels(), spec.GetBinnings()));
  }

  //----------------------------------------------------------------------
  void EnsembleSpectrum::FillSingle(double x, double w, int universeId)
  {
    // Filling a single constituent universe
    const int bin = fAxis.GetBins1D().FindBin(x);
    fHist.Fill((fAxis.GetBins1D().NBins()+2) * universeId + bin, w);
  }

  //----------------------------------------------------------------------
  void EnsembleSpectrum::FillEnsemble(double x, const std::vector<double>& ws)
  {
    const unsigned int bin = fAxis.GetBins1D().FindBin(x);
    const unsigned int nbins = fAxis.GetBins1D().NBins()+2;

    const unsigned int nuniv = fMultiverse->NUniv();
    assert(ws.size() == nuniv);

    for(unsigned int univIdx = 0; univIdx < nuniv; ++univIdx)
      fHist.Fill(nbins * univIdx + bin, ws[univIdx]);
  }

  //----------------------------------------------------------------------
  void EnsembleSpectrum::FillPOT(double pot)
  {
    fPOT += pot;
  }

  //----------------------------------------------------------------------
  void EnsembleSpectrum::FillLivetime(double livetime)
  {
    fLivetime += livetime;
  }

  //----------------------------------------------------------------------
  unsigned int EnsembleSpectrum::NUniverses() const
  {
    return fMultiverse->NUniv();
  }

  //----------------------------------------------------------------------
  Spectrum EnsembleSpectrum::Universe(unsigned int univIdx) const
  {
    const int nbins = fAxis.GetBins1D().NBins()+2;
    if(fHist.HasStan()){
      return Spectrum(Eigen::ArrayXstan(fHist.GetEigenStan().segment(nbins*univIdx, nbins)),
                      fAxis, fPOT, fLivetime);
    }
    else{
      return Spectrum(Eigen::ArrayXd(fHist.GetEigen().segment(nbins*univIdx, nbins)),
                      fAxis, fPOT, fLivetime);
    }
  }

  //----------------------------------------------------------------------
  TGraphAsymmErrors* EnsembleSpectrum::ErrorBand(double exposure,
                                                 EExposureType expotype,
                                                 EBinType bintype) const
  {
    // TODO lots of code duplication with EnsembleRatio

    const Eigen::ArrayXd arr = fHist.GetEigen() * exposure / (expotype == kPOT ? fPOT : fLivetime);

    TGraphAsymmErrors* g = new TGraphAsymmErrors;

    const int nbins = fAxis.GetBins1D().NBins()+2;
    const std::vector<double>& edges = fAxis.GetBins1D().Edges();

    for(int binIdx = 1; binIdx+1 < nbins; ++binIdx){
      const double dx = edges[binIdx] - edges[binIdx-1];
      assert(dx > 0);

      const double xnom = (edges[binIdx-1] + edges[binIdx]) / 2; // bin center
      const double ynom = (bintype == kBinDensity) ? arr[binIdx] / dx : arr[binIdx];
      g->SetPoint(binIdx-1, xnom, ynom);

      std::vector<double> ys;
      ys.reserve(NUniverses()-1);
      for(unsigned int univIdx = 1; univIdx < NUniverses(); ++univIdx)
        ys.push_back((bintype == kBinDensity) ? arr[univIdx*nbins + binIdx] / dx : arr[univIdx*nbins + binIdx]);

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
  Eigen::MatrixXd EnsembleSpectrum::CovarianceMatrix(const double exposure, EExposureType expotype)
  {
    assert (fMultiverse->MultiverseType() == kRandomGas);

    const Eigen::ArrayXd& arr = fHist.GetEigen() * exposure / (expotype == kPOT ? fPOT : fLivetime);

    const int nbins = fAxis.GetBins1D().NBins()+2;
    std::vector<Eigen::ArrayXd> histVec;
    histVec.reserve(NUniverses());

    for(unsigned int univIdx = 1; univIdx < NUniverses(); ++univIdx)
      histVec.push_back(arr.segment(nbins*univIdx, nbins));

    return ana::CalcCovMx(histVec);
  }

  //----------------------------------------------------------------------
  Eigen::MatrixXd EnsembleSpectrum::BiasMatrix(const double exposure, EExposureType expotype)
  {
    assert (fMultiverse->MultiverseType() == kRandomGas);

    const Eigen::ArrayXd& arr = fHist.GetEigen() * exposure / (expotype == kPOT ? fPOT : fLivetime);

    const int nbins = fAxis.GetBins1D().NBins()+2;
    std::vector<Eigen::ArrayXd> histVec;
    histVec.reserve(NUniverses());

    for(unsigned int univIdx = 1; univIdx < NUniverses(); ++univIdx)
      histVec.push_back(arr.segment(nbins*univIdx, nbins));

    return ana::CalcBiasMx(arr.segment(0, nbins), histVec);
  }

  //----------------------------------------------------------------------
  void EnsembleSpectrum::Scale(double c)
  {
    fHist.Scale(c);
  }

  //----------------------------------------------------------------------
  EnsembleSpectrum& EnsembleSpectrum::PlusEqualsHelper(const EnsembleSpectrum& rhs, int sign, const std::string& func)
  {
    CheckMultiverses(rhs.GetMultiverse(), func);

    // In this case it would be OK to have no POT/livetime
    if(rhs.fHist.Initialized() && rhs.fHist.Integral() == 0) return *this;

    if((!fPOT && !fLivetime) || (!rhs.fPOT && !rhs.fLivetime)){
      std::cout << "Error: can't sum Spectrum with no POT or livetime: "
                << fPOT << " " << rhs.fPOT << " " << fLivetime << " " << rhs.fL\
ivetime
                << std::endl;
      abort();
    }

    if(!fLivetime && !rhs.fPOT){
      std::cout << "Error: can't sum Spectrum with POT ("
                << fPOT << ") but no livetime and Spectrum with livetime ("
                << rhs.fLivetime << " sec) but no POT." << std::endl;
      abort();
    }

    if(!fPOT && !rhs.fLivetime){
      std::cout << "Error: can't sum Spectrum with livetime ("
                << fLivetime << " sec) but no POT and Spectrum with POT ("
                << rhs.fPOT << ") but no livetime." << std::endl;
      abort();
    }

    // And now there are still a bunch of good cases to consider

    if(fPOT && rhs.fPOT){
      // Scale by POT when possible
      fHist.Add(rhs.fHist, sign*fPOT/rhs.fPOT);

      if(fLivetime && rhs.fLivetime){
        // If POT/livetime ratios match, keep regular lifetime, otherwise zero
        // it out.
        if(!AlmostEqual(fLivetime*rhs.fPOT, rhs.fLivetime*fPOT))
          fLivetime = 0;
      }
      if(!fLivetime && rhs.fLivetime){
        // If the RHS has a livetime and we don't, copy it in (suitably scaled)
        fLivetime = rhs.fLivetime * fPOT/rhs.fPOT;
      }
      // Otherwise, keep our own livetime (if any)

      return *this;
    }

    if(fLivetime && rhs.fLivetime){
      // Scale by livetime, the only thing in common
      fHist.Add(rhs.fHist, sign*fLivetime/rhs.fLivetime);

      if(!fPOT && rhs.fPOT){
        // If the RHS has a POT and we don't, copy it in (suitably scaled)
        fPOT = rhs.fPOT * fLivetime/rhs.fLivetime;
      }
      // Otherwise, keep our own POT (if any)

      return *this;
    }

    // That should have been all the cases. I definitely want to know what
    // happened if it wasn't.
    std::cout << "EnsembleSpectrum::operator+=(). How did we get here? "
              << fPOT << " " << fLivetime << " "
              << rhs.fPOT << " " << rhs.fLivetime << std::endl;
    abort();
  }

  //----------------------------------------------------------------------
  EnsembleSpectrum& EnsembleSpectrum::operator+=(const EnsembleSpectrum& rhs)
  {
    return PlusEqualsHelper(rhs, +1, __func__);
  }

  //----------------------------------------------------------------------
  EnsembleSpectrum EnsembleSpectrum::operator+(const EnsembleSpectrum& rhs) const
  {
    EnsembleSpectrum ret = *this;
    ret += rhs;
    return ret;
  }

  //----------------------------------------------------------------------
  EnsembleSpectrum& EnsembleSpectrum::operator-=(const EnsembleSpectrum& rhs)
  {
    return PlusEqualsHelper(rhs, -1, __func__);
  }

  //----------------------------------------------------------------------
  EnsembleSpectrum EnsembleSpectrum::operator-(const EnsembleSpectrum& rhs) const
  {
    EnsembleSpectrum ret = *this;
    ret -= rhs;
    return ret;
  }

  //----------------------------------------------------------------------
  EnsembleSpectrum& EnsembleSpectrum::operator*=(const EnsembleRatio& rhs)
  {
    CheckMultiverses(rhs.GetMultiverse(), __func__);
    fHist.Multiply(rhs.fHist);

    return *this;
  }

  //----------------------------------------------------------------------
  EnsembleSpectrum EnsembleSpectrum::operator*(const EnsembleRatio& rhs) const
  {
    EnsembleSpectrum ret = *this;
    ret *= rhs;
    return ret;
  }

  //----------------------------------------------------------------------
  EnsembleSpectrum& EnsembleSpectrum::operator/=(const EnsembleRatio& rhs)
  {
    CheckMultiverses(rhs.GetMultiverse(), __func__);
    fHist.Divide(rhs.fHist);

    return *this;
  }

  //----------------------------------------------------------------------
  EnsembleSpectrum EnsembleSpectrum::operator/(const EnsembleRatio& rhs) const
  {
    EnsembleSpectrum ret = *this;
    ret /= rhs;
    return ret;
  }

  //----------------------------------------------------------------------
  void EnsembleSpectrum::SaveTo(TDirectory* dir, const std::string& name) const
  {
    TDirectory* tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("EnsembleSpectrum").Write("type");

    fMultiverse->SaveTo(dir, "multiverse");

    // TODO potentially version of Write() that doesn't take binning
    fHist.Write(Binning::Simple(fHist.GetNbinsX(), 0, fHist.GetNbinsX()));

    TH1D hPot("", "", 1, 0, 1);
    hPot.Fill(.5, fPOT);
    hPot.Write("pot");
    TH1D hLivetime("", "", 1, 0, 1);
    hLivetime.Fill(.5, fLivetime);
    hLivetime.Write("livetime");

    for(unsigned int i = 0; i < NDimensions(); ++i){
      TObjString(GetLabels()[i].c_str()).Write(TString::Format("label%d", i).Data());
      GetBinnings()[i].SaveTo(dir, TString::Format("bins%d", i).Data());
    }

    dir->Write();
    delete dir;

    tmp->cd();
  }

  //----------------------------------------------------------------------
  EnsembleSpectrum::EnsembleSpectrum(const FitMultiverse* multiverse,
                                     const Hist&& hist,
                                     double pot,
                                     double livetime,
                                     const LabelsAndBins& axis)
    : fMultiverse(multiverse), fHist(hist), fPOT(pot), fLivetime(livetime), fAxis(axis)
  {
  }

  //----------------------------------------------------------------------
  std::unique_ptr<EnsembleSpectrum> EnsembleSpectrum::LoadFrom(TDirectory* topdir, const std::string& name)
  {
    std::unique_ptr<TDirectory> dir(topdir->GetDirectory(name.c_str())); // switch to subdir
    assert(dir);

    DontAddDirectory guard;

    std::unique_ptr<TObjString> tag((TObjString*)dir->Get("type"));
    assert(tag);
    assert(tag->GetString() == "EnsembleSpectrum");

    std::unique_ptr<TH1> hPot((TH1*)dir->Get("pot"));
    assert(hPot);
    std::unique_ptr<TH1> hLivetime((TH1*)dir->Get("livetime"));
    assert(hLivetime);

    // TODO LabelsAndBins::LoadFrom() or FromDirectory()
    std::vector<std::string> labels;
    std::vector<Binning> bins;
    for(int i = 0; ; ++i){
      const std::string subname = TString::Format("bins%d", i).Data();
      TDirectory* subdir = dir->GetDirectory(subname.c_str());
      if(!subdir) break;
      delete subdir;
      bins.push_back(*Binning::LoadFrom(dir.get(), subname));
      std::unique_ptr<TObjString> label((TObjString*)dir->Get(TString::Format("label%d", i)));
      labels.push_back(label->GetString().Data());
    }

    return std::unique_ptr<EnsembleSpectrum>(new EnsembleSpectrum(FitMultiverse::LoadFrom(dir.get(), "multiverse"),
                                                                  Hist::FromDirectory(dir.get()),
                                                                  hPot->GetBinContent(1),
                                                                  hLivetime->GetBinContent(1),
                                                                  LabelsAndBins(labels, bins)));
  }

  //----------------------------------------------------------------------
  void EnsembleSpectrum::CheckMultiverses(const FitMultiverse& rhs,
                                          const std::string& func) const
  {
    if(&rhs == fMultiverse) return;

    std::cout << "EnsembleSpectrum::" << func << ": attempting to combine two spectra made with different multiverses: " << std::endl;
    std::cout << "  " << fMultiverse->ShortName() << std::endl;
    std::cout << "vs" << std::endl;
    std::cout << rhs.ShortName() << std::endl;
    abort();
  }

  //----------------------------------------------------------------------
  void DrawErrorBand(TH1* nom, TGraphAsymmErrors* band, int bandCol, double alpha)
  {
    if(bandCol == -1) bandCol = nom->GetLineColor()-10; // hopefully a lighter version

    // Check if this pad has already been drawn in
    const bool same = gPad && !gPad->GetListOfPrimitives()->IsEmpty();

    nom->Draw(same ? "hist same" : "hist");

    band->SetFillColorAlpha(bandCol, alpha);
    band->Draw("e2 same");

    nom->Draw("hist same");

    // If we are the first spectrum to draw, scale the y-axis appropriately to
    // fit the error band as well as the nominal
    if(!same){
      double maxY = 0;
      // Don't consider underflow or overflow bins when determining maximum
      for(int i = 1; i < band->GetN()-1; ++i){
        if(band->GetY()[i] > 1e99) continue; // effectively infinite
        maxY = std::max(maxY, band->GetY()[i] + band->GetErrorYhigh(i));
      }

      // Use non-zero lower value so that SetLogy() still works
      nom->GetYaxis()->SetRangeUser(1e-10, 1.1 * maxY);
    }

    gPad->RedrawAxis();
  }
}