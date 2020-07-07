#include "CAFAna/Core/ReweightableSpectrum.h"

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/HistCache.h"
#include "CAFAna/Core/Ratio.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/SpectrumLoaderBase.h"
#include "CAFAna/Core/Utilities.h"

#include "Utilities/func/Stan.h"

#include "TDirectory.h"
#include "TH2.h"
#include "TObjString.h"

#include <cassert>
#include <iostream>
#include <memory>

namespace ana
{
  //----------------------------------------------------------------------
  ReweightableSpectrum::ReweightableSpectrum(SpectrumLoaderBase& loader,
                                             const HistAxis& recoAxis,
                                             const HistAxis& trueAxis,
                                             const Cut& cut,
                                             const SystShifts& shift,
                                             const Var& wei)
    : ReweightableSpectrum(recoAxis.GetLabels(),
                           recoAxis.GetBinnings(),
                           trueAxis.GetBinnings()[0],
                           trueAxis.GetVars()[0])
  {
    assert(trueAxis.NDimensions() == 1);

    fTrueLabel = trueAxis.GetLabels()[0];

    DontAddDirectory guard;

    // Can't use HistCache here because y-axis is not necessarily
    // TrueEnergyBinning. TODO - that should maybe be generalized.

    const std::string name = UniqueName();

    const Binning xbins = Bins1DX();
    const Binning ybins = trueAxis.GetBinnings()[0];

    fMat = new Eigen::MatrixXd(ybins.NBins()+2, xbins.NBins()+2);
    fMat->setZero();

    loader.AddReweightableSpectrum(*this, recoAxis.GetMultiDVar(), cut, shift, wei);
  }

  //----------------------------------------------------------------------
  ReweightableSpectrum::~ReweightableSpectrum()
  {
    for(SpectrumLoaderBase* loader: fLoaderCount){
      loader->RemoveReweightableSpectrum(this);
    }

    delete fMat;
  }

  //----------------------------------------------------------------------
  ReweightableSpectrum::ReweightableSpectrum(const ReweightableSpectrum& rhs)
    : fRWVar(rhs.fRWVar), fLabels(rhs.fLabels), fBins(rhs.fBins), fBinsY(rhs.fBinsY)
  {
    DontAddDirectory guard;

    fMat = rhs.fMat ? new Eigen::MatrixXd(*rhs.fMat) : 0;

    fPOT = rhs.fPOT;
    fLivetime = rhs.fLivetime;

    assert( rhs.fLoaderCount.empty() ); // Copying with pending loads is unexpected
  }

  //----------------------------------------------------------------------
  ReweightableSpectrum& ReweightableSpectrum::operator=(const ReweightableSpectrum& rhs)
  {
    if(this == &rhs) return *this;

    DontAddDirectory guard;

    fRWVar = rhs.fRWVar;
    fLabels = rhs.fLabels;
    fBins = rhs.fBins;
    fBinsY = rhs.fBinsY;

    delete fMat;
    fMat = rhs.fMat ? new Eigen::MatrixXd(*rhs.fMat) : 0;
    fPOT = rhs.fPOT;
    fLivetime = rhs.fLivetime;

    assert( fLoaderCount.empty() ); // Copying with pending loads is unexpected

    return *this;
  }

  //----------------------------------------------------------------------
  TH2D* ReweightableSpectrum::ToTH2(double pot) const
  {
    // Could have a file temporarily open
    DontAddDirectory guard;

    TH2D* ret = HistCache::NewTH2D("", Bins1DX(), fBinsY);

    for(int i = 0; i < fMat->rows(); ++i){
      for(int j = 0; j < fMat->cols(); ++j){
        ret->SetBinContent(j, i, (*fMat)(i, j));
      }
    }

    if(fPOT){
      ret->Scale(pot/fPOT);
    }
    else{
      // How did it get events with no POT?
      assert(ret->Integral() == 0);
    }

    std::string label;
    for(const std::string& l: fLabels) label += l + " and ";
    label.resize(label.size()-5); // drop the last "and"
    ret->GetXaxis()->SetTitle(label.c_str());
    ret->GetYaxis()->SetTitle(fTrueLabel.c_str());

    return ret;
  }

  //----------------------------------------------------------------------
  void ReweightableSpectrum::Fill(double x, double y, double w)
  {
    // TODO repeated calling of Bins1DX() here is going to be very inefficient
    (*fMat)(fBinsY.FindBin(y), Bins1DX().FindBin(x)) += w;
  }

  /// Helper for \ref Unweighted
  inline Eigen::ArrayXd ProjectionX(const Eigen::MatrixXd& mat)
  {
    return Eigen::RowVectorXd::Ones(mat.cols()) * mat;
  }

  /// Helper for \ref WeightingVariable
  inline Eigen::ArrayXd ProjectionY(const Eigen::MatrixXd& mat)
  {
    return mat * Eigen::VectorXd::Ones(mat.rows());
  }

  //----------------------------------------------------------------------
  Spectrum ReweightableSpectrum::UnWeighted() const
  {
    return Spectrum(Hist::Adopt(ProjectionX(*fMat)), fLabels, fBins, fPOT, fLivetime);
  }

  //----------------------------------------------------------------------
  Spectrum ReweightableSpectrum::WeightingVariable() const
  {
    return Spectrum(Hist::Adopt(ProjectionY(*fMat)), fLabels, fBins, fPOT, fLivetime);
  }

  //----------------------------------------------------------------------
  Spectrum ReweightableSpectrum::WeightedBy(const Ratio& ws) const
  {
    if(!ws.HasStan()){
      const Eigen::VectorXd& vec = ws.GetEigen();

      return Spectrum(Hist::Adopt(Eigen::ArrayXd(vec.transpose() * *fMat)),
                      fLabels, fBins, fPOT, fLivetime);
    }
    else{
      const Eigen::VectorXstan& vec = ws.GetEigenStan();

      return Spectrum(Hist::Adopt(vec.transpose() * *fMat),
                      fLabels, fBins, fPOT, fLivetime);
    }
  }

  /*
  //----------------------------------------------------------------------
  void ReweightableSpectrum::ReweightToTrueSpectrum(const Spectrum& target)
  {
    // This is a big component of what extrapolations do, so it should be fast

    // But I don't believe DUNE in particular uses this...
    abort();
  }

  //----------------------------------------------------------------------
  void ReweightableSpectrum::ReweightToRecoSpectrum(const Spectrum& target)
  {
    // This is a big component of what extrapolations do, so it should be fast

    // But I don't believe DUNE in particular uses this...
    abort();
  }
*/

  ReweightableSpectrum& ReweightableSpectrum::PlusEqualsHelper(const ReweightableSpectrum& rhs, int sign)
  {
    // In this case it would be OK to have no POT/livetime
    if(rhs.fMat && rhs.fMat->sum() == 0) return *this;


    if((!fPOT && !fLivetime) || (!rhs.fPOT && !rhs.fLivetime)){
      std::cout << "Error: can't sum ReweightableSpectrum with no POT or livetime."
                << fPOT << " " << rhs.fPOT
                << std::endl;
      //      abort();
      return *this;
    }

    if(!fLivetime && !rhs.fPOT){
      std::cout << "Error: can't sum ReweightableSpectrum with POT ("
                << fPOT << ") but no livetime and ReweightableSpectrum with livetime ("
                << rhs.fLivetime << " sec) but no POT." << std::endl;
      abort();
    }

    if(!fPOT && !rhs.fLivetime){
      std::cout << "Error: can't sum ReweightableSpectrum with livetime ("
                << fLivetime << " sec) but no POT and ReweightableSpectrum with POT ("
                << rhs.fPOT << ") but no livetime." << std::endl;
      abort();
    }

    // And now there are still a bunch of good cases to consider

    if(fPOT && rhs.fPOT){
      // Scale by POT when possible
      if(rhs.fMat) *fMat += *rhs.fMat * sign*fPOT/rhs.fPOT;

      if(fLivetime && rhs.fLivetime){
        // If POT/livetime ratios match, keep regular lifetime, otherwise zero
        // it out.
        if(AlmostEqual(fLivetime*rhs.fPOT, rhs.fLivetime*fPOT))
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
      if(rhs.fMat) *fMat += *rhs.fMat * sign*fLivetime/rhs.fLivetime;

      if(!fPOT && rhs.fPOT){
        // If the RHS has a POT and we don't, copy it in (suitably scaled)
        fPOT = rhs.fPOT * fLivetime/rhs.fLivetime;
      }
      // Otherwise, keep our own POT (if any)

      return *this;
    }

    // That should have been all the cases. I definitely want to know what
    // happened if it wasn't.
    std::cout << "ReweightableSpectrum::operator+=(). How did we get here? "
              << fPOT << " " << fLivetime << " "
              << rhs.fPOT << " " << rhs.fLivetime << std::endl;
    abort();
  }

  //----------------------------------------------------------------------
  ReweightableSpectrum& ReweightableSpectrum::operator+=(const ReweightableSpectrum& rhs)
  {
    return PlusEqualsHelper(rhs, +1);
  }

  //----------------------------------------------------------------------
  ReweightableSpectrum ReweightableSpectrum::operator+(const ReweightableSpectrum& rhs) const
  {
    ReweightableSpectrum ret = *this;
    ret += rhs;
    return ret;
  }

  //----------------------------------------------------------------------
  ReweightableSpectrum& ReweightableSpectrum::operator-=(const ReweightableSpectrum& rhs)
  {
    return PlusEqualsHelper(rhs, -1);
  }

  //----------------------------------------------------------------------
  ReweightableSpectrum ReweightableSpectrum::operator-(const ReweightableSpectrum& rhs) const
  {
    ReweightableSpectrum ret = *this;
    ret -= rhs;
    return ret;
  }

  //----------------------------------------------------------------------
  void ReweightableSpectrum::Clear()
  {
    fMat->setZero();
  }

  //----------------------------------------------------------------------
  void ReweightableSpectrum::RemoveLoader(SpectrumLoaderBase* p)
  { fLoaderCount.erase(p); }

  //----------------------------------------------------------------------
  void ReweightableSpectrum::AddLoader(SpectrumLoaderBase* p)
  { fLoaderCount.insert(p); }

  //----------------------------------------------------------------------
  void ReweightableSpectrum::SaveTo(TDirectory* dir, const std::string& name) const
  {
    _SaveTo(dir, name, "ReweightableSpectrum");
  }

  //----------------------------------------------------------------------
  void ReweightableSpectrum::_SaveTo(TDirectory* dir,
                                     const std::string& name,
                                     const std::string& type) const
  {
    TDirectory* tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to sbudir
    dir->cd();

    TObjString(type.c_str()).Write("type");

    TH2* h = ToTH2(fPOT);
    h->Write("hist");

    TH1D hPot("", "", 1, 0, 1);
    hPot.Fill(.5, fPOT);
    hPot.Write("pot");
    TH1D hLivetime("", "", 1, 0, 1);
    hLivetime.Fill(.5, fLivetime);
    hLivetime.Write("livetime");

    for(unsigned int i = 0; i < fBins.size(); ++i){
      TObjString(fLabels[i].c_str()).Write(TString::Format("label%d", i).Data());
      fBins[i].SaveTo(dir, TString::Format("bins%d", i).Data());
    }

    TObjString(fTrueLabel.c_str()).Write("true_label");

    dir->Write();
    delete dir;

    delete h;

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<ReweightableSpectrum> ReweightableSpectrum::LoadFrom(TDirectory* dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    TObjString* tag = (TObjString*)dir->Get("type");
    assert(tag);
    assert(tag->GetString() == "ReweightableSpectrum");

    TH2D* spect = (TH2D*)dir->Get("hist");
    assert(spect);
    TH1* hPot = (TH1*)dir->Get("pot");
    assert(hPot);
    TH1* hLivetime = (TH1*)dir->Get("livetime");
    assert(hLivetime);

    std::vector<std::string> labels;
    std::vector<Binning> bins;

    for(int i = 0; ; ++i){
      const std::string subname = TString::Format("bins%d", i).Data();
      TDirectory* subdir = dir->GetDirectory(subname.c_str());
      if(!subdir) break;
      delete subdir;
      bins.push_back(*Binning::LoadFrom(dir, subname.c_str()));
      TObjString* label = (TObjString*)dir->Get(TString::Format("label%d", i));
      labels.push_back(label ? label->GetString().Data() : "");
    }

    delete dir;

    // Can't restore the Vars, go with a dummy value
    const std::vector<Var> vars(labels.size(), kUnweighted);
    auto ret = std::make_unique<ReweightableSpectrum>(kNullLoader,
                                                      HistAxis(labels, bins, vars),
                                                      HistAxis(spect->GetYaxis()->GetTitle(), Binning::FromTAxis(spect->GetYaxis()), kUnweighted),
                                                      kNoCut);

    *ret->fMat = Eigen::Map<const Eigen::MatrixXd>(spect->GetArray(),
                                                   ret->fMat->rows(),
                                                   ret->fMat->cols());

    delete spect;

    ret->fPOT = hPot->Integral(0, -1);
    ret->fLivetime = hLivetime->Integral(0, -1);

    delete hPot;
    delete hLivetime;

    return ret;
  }

  //----------------------------------------------------------------------
  Binning ReweightableSpectrum::Bins1DX() const
  {
    assert(!fBins.empty());

    Binning xbins = fBins[0];
    if(fBins.size() > 1){
      int n = 1;
      for(const Binning& b: fBins) n *= b.NBins();
      xbins = Binning::Simple(n, 0, n);
    }
    return xbins;
  }
}
