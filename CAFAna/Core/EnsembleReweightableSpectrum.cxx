#include "CAFAna/Core/EnsembleReweightableSpectrum.h"

#include "CAFAna/Core/EnsembleRatio.h"
#include "CAFAna/Core/FitMultiverse.h"

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Ratio.h"
//#include "CAFAna/Core/Stan.h"

#include "TDirectory.h"
#include "TH2.h"
#include "TObjString.h"

#include <cassert>
#include <iostream>
#include <memory>

namespace ana
{
  //----------------------------------------------------------------------
  EnsembleReweightableSpectrum::
  EnsembleReweightableSpectrum(IValuePairEnsembleSource& src,
                               const LabelsAndBins& recoAxis,
                               const LabelsAndBins& trueAxis)
    : EnsembleReweightableSpectrum(src.GetMultiverse(), recoAxis, trueAxis)
  {
    fMat.resize(trueAxis.GetBins1D().NBins()+2,
                (recoAxis.GetBins1D().NBins()+2) * fMultiverse->NUniv());

    fMat.setZero();

    src.Register(this);
  }

  //----------------------------------------------------------------------
  EnsembleReweightableSpectrum::
  EnsembleReweightableSpectrum(const FitMultiverse* multiverse,
                               const Eigen::MatrixXd&& mat,
                               const LabelsAndBins& recoAxis,
                               const LabelsAndBins& trueAxis,
                               double pot, double livetime)
    : fMultiverse(multiverse), fMat(mat), fPOT(pot), fLivetime(livetime),
      fAxisX(recoAxis), fAxisY(trueAxis)
  {
  }

  //----------------------------------------------------------------------
  EnsembleReweightableSpectrum::~EnsembleReweightableSpectrum()
  {
  }

  //----------------------------------------------------------------------
  EnsembleReweightableSpectrum::EnsembleReweightableSpectrum(const EnsembleReweightableSpectrum& rhs)
    : fMultiverse(rhs.fMultiverse),
      fMat(rhs.fMat),
      fPOT(rhs.fPOT),
      fLivetime(rhs.fLivetime),
      fAxisX(rhs.fAxisX),
      fAxisY(rhs.fAxisY)
  {
  }

  //----------------------------------------------------------------------
  EnsembleReweightableSpectrum& EnsembleReweightableSpectrum::operator=(const EnsembleReweightableSpectrum& rhs)
  {
    if(this == &rhs) return *this;

    fMultiverse = rhs.fMultiverse;
    fMat = rhs.fMat;
    fPOT = rhs.fPOT;
    fLivetime = rhs.fLivetime;
    fAxisX = rhs.fAxisX;
    fAxisY = rhs.fAxisY;

    return *this;
  }

  //----------------------------------------------------------------------
  void EnsembleReweightableSpectrum::FillSingle(double x, double y, double w, int universeId)
  {
    // Filling a single constituent universe
    const int xbin = fAxisX.GetBins1D().FindBin(x);
    const int ybin = fAxisY.GetBins1D().FindBin(y);
    const unsigned int nbinsx = fAxisX.GetBins1D().NBins()+2;
    fMat(ybin, nbinsx * universeId + xbin) += w;
  }

  //----------------------------------------------------------------------
  void EnsembleReweightableSpectrum::FillEnsemble(double x, double y, const std::vector<double>& ws)
  {
    const unsigned int xbin = fAxisX.GetBins1D().FindBin(x);
    const unsigned int ybin = fAxisY.GetBins1D().FindBin(y);
    const unsigned int nbinsx = fAxisX.GetBins1D().NBins()+2;

    const unsigned int nuniv = fMultiverse->NUniv();
    assert(ws.size() == nuniv);

    for(unsigned int univIdx = 0; univIdx < nuniv; ++univIdx)
      fMat(ybin, nbinsx * univIdx + xbin) += ws[univIdx];
  }

  //----------------------------------------------------------------------
  void EnsembleReweightableSpectrum::FillPOT(double pot)
  {
    fPOT += pot;
  }

  //----------------------------------------------------------------------
  void EnsembleReweightableSpectrum::FillLivetime(double livetime)
  {
    fLivetime += livetime;
  }

  //----------------------------------------------------------------------
  unsigned int EnsembleReweightableSpectrum::NUniverses() const
  {
    return fMultiverse->NUniv();
  }

  //----------------------------------------------------------------------
  ReweightableSpectrum EnsembleReweightableSpectrum::Universe(unsigned int univIdx) const
  {
    const int nbinsx = fAxisX.GetBins1D().NBins()+2;
    const int nbinsy = fAxisY.GetBins1D().NBins()+2; // or fMat.cols()
    return ReweightableSpectrum(fMat.block(0, nbinsx*univIdx, nbinsy, nbinsx),
                                fAxisX, fAxisY, fPOT, fLivetime);
  }

  /// Helper for \ref Unweighted
  inline Eigen::ArrayXd ProjectionX(const Eigen::MatrixXd& mat)
  {
    return Eigen::RowVectorXd::Ones(mat.rows()) * mat;
  }

  /// Helper for \ref WeightingVariable
  inline Eigen::ArrayXd ProjectionY(const Eigen::MatrixXd& mat)
  {
    return mat * Eigen::VectorXd::Ones(mat.cols());
  }

  //----------------------------------------------------------------------
  EnsembleSpectrum EnsembleReweightableSpectrum::UnWeighted() const
  {
    return EnsembleSpectrum(fMultiverse,
                            Hist::Adopt(ProjectionX(fMat)),
                            fPOT, fLivetime, fAxisX);
  }

  //----------------------------------------------------------------------
  EnsembleSpectrum EnsembleReweightableSpectrum::WeightingVariable() const
  {
    return EnsembleSpectrum(fMultiverse,
                            Hist::Adopt(ProjectionY(fMat)),
                            fPOT, fLivetime, fAxisY);
  }

  //----------------------------------------------------------------------
  EnsembleSpectrum EnsembleReweightableSpectrum::WeightedBy(const Ratio& ws) const
  {
    // This version is appropriate for oscillations. We should probably add
    // another that takes EnsembleRatio.

    const Eigen::VectorXd& vec = ws.GetEigen();

    return EnsembleSpectrum(fMultiverse,
                            Hist::Adopt(Eigen::ArrayXd(vec.transpose() * fMat)),
                            fPOT, fLivetime, fAxisX);
  }

  //----------------------------------------------------------------------
  EnsembleReweightableSpectrum& EnsembleReweightableSpectrum::PlusEqualsHelper(const EnsembleReweightableSpectrum& rhs, int sign, const std::string& func)
  {
    CheckMultiverses(rhs.GetMultiverse(), func);

    // In this case it would be OK to have no POT/livetime
    if(rhs.fMat.sum() == 0) return *this;


    if((!fPOT && !fLivetime) || (!rhs.fPOT && !rhs.fLivetime)){
      std::cout << "Error: can't sum EnsembleReweightableSpectrum with no POT or livetime."
                << fPOT << " " << rhs.fPOT
                << std::endl;
      //      abort();
      return *this;
    }

    if(!fLivetime && !rhs.fPOT){
      std::cout << "Error: can't sum EnsembleReweightableSpectrum with POT ("
                << fPOT << ") but no livetime and EnsembleReweightableSpectrum with livetime ("
                << rhs.fLivetime << " sec) but no POT." << std::endl;
      abort();
    }

    if(!fPOT && !rhs.fLivetime){
      std::cout << "Error: can't sum EnsembleReweightableSpectrum with livetime ("
                << fLivetime << " sec) but no POT and EnsembleReweightableSpectrum with POT ("
                << rhs.fPOT << ") but no livetime." << std::endl;
      abort();
    }

    // And now there are still a bunch of good cases to consider

    if(fPOT && rhs.fPOT){
      // Scale by POT when possible
      fMat += rhs.fMat * sign*fPOT/rhs.fPOT;

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
      fMat += rhs.fMat * sign*fLivetime/rhs.fLivetime;

      if(!fPOT && rhs.fPOT){
        // If the RHS has a POT and we don't, copy it in (suitably scaled)
        fPOT = rhs.fPOT * fLivetime/rhs.fLivetime;
      }
      // Otherwise, keep our own POT (if any)

      return *this;
    }

    // That should have been all the cases. I definitely want to know what
    // happened if it wasn't.
    std::cout << "EnsembleReweightableSpectrum::operator+=(). How did we get here? "
              << fPOT << " " << fLivetime << " "
              << rhs.fPOT << " " << rhs.fLivetime << std::endl;
    abort();
  }

  //----------------------------------------------------------------------
  EnsembleReweightableSpectrum& EnsembleReweightableSpectrum::operator+=(const EnsembleReweightableSpectrum& rhs)
  {
    return PlusEqualsHelper(rhs, +1, __func__);
  }

  //----------------------------------------------------------------------
  EnsembleReweightableSpectrum EnsembleReweightableSpectrum::operator+(const EnsembleReweightableSpectrum& rhs) const
  {
    EnsembleReweightableSpectrum ret = *this;
    ret += rhs;
    return ret;
  }

  //----------------------------------------------------------------------
  EnsembleReweightableSpectrum& EnsembleReweightableSpectrum::operator-=(const EnsembleReweightableSpectrum& rhs)
  {
    return PlusEqualsHelper(rhs, -1, __func__);
  }

  //----------------------------------------------------------------------
  EnsembleReweightableSpectrum EnsembleReweightableSpectrum::operator-(const EnsembleReweightableSpectrum& rhs) const
  {
    EnsembleReweightableSpectrum ret = *this;
    ret -= rhs;
    return ret;
  }

  //----------------------------------------------------------------------
  void EnsembleReweightableSpectrum::SaveTo(TDirectory* dir,
                                            const std::string& name) const
  {
    _SaveTo(dir, name, "EnsembleReweightableSpectrum");
  }

  //----------------------------------------------------------------------
  void EnsembleReweightableSpectrum::_SaveTo(TDirectory* dir,
                                             const std::string& name,
                                             const std::string& type) const
  {
    TDirectory* tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to sbudir
    dir->cd();

    TObjString(type.c_str()).Write("type");

    fMultiverse->SaveTo(dir, "multiverse");

    TH2D* hist = MakeTH2D("hist", "",
                          Binning::Simple(fMat.cols(), 0, fMat.cols()),
                          fAxisY.GetBins1D());
    hist->GetXaxis()->SetTitle(fAxisX.GetLabel1D().c_str());
    hist->GetYaxis()->SetTitle(fAxisY.GetLabel1D().c_str());
    for(int i = 0; i < fMat.cols(); ++i){
      for(int j = 0; j < fMat.rows(); ++j){
        hist->SetBinContent(i, j, fMat(j, i));
      }
    }
    hist->Write("hist");

    TH1D hPot("", "", 1, 0, 1);
    hPot.Fill(.5, fPOT);
    hPot.Write("pot");
    TH1D hLivetime("", "", 1, 0, 1);
    hLivetime.Fill(.5, fLivetime);
    hLivetime.Write("livetime");

    for(unsigned int i = 0; i < fAxisX.NDimensions(); ++i){
      TObjString(fAxisX.GetLabels()[i].c_str()).Write(TString::Format("label%d", i).Data());
      fAxisX.GetBinnings()[i].SaveTo(dir, TString::Format("bins%d", i).Data());
    }

    for(unsigned int i = 0; i < fAxisY.NDimensions(); ++i){
      TObjString(fAxisY.GetLabels()[i].c_str()).Write(TString::Format("labely%d", i).Data());
      fAxisY.GetBinnings()[i].SaveTo(dir, TString::Format("binsy%d", i).Data());
    }

    dir->Write();
    delete dir;

    delete hist;

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<EnsembleReweightableSpectrum> EnsembleReweightableSpectrum::LoadFrom(TDirectory* dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    TObjString* tag = (TObjString*)dir->Get("type");
    assert(tag);
    assert(tag->GetString() == "EnsembleReweightableSpectrum");
    delete tag;

    TH2D* spect = (TH2D*)dir->Get("hist");
    assert(spect);
    TH1* hPot = (TH1*)dir->Get("pot");
    assert(hPot);
    TH1* hLivetime = (TH1*)dir->Get("livetime");
    assert(hLivetime);

    std::vector<std::string> labels, labelsy;
    std::vector<Binning> bins, binsy;

    for(int i = 0; ; ++i){
      const std::string subname = TString::Format("bins%d", i).Data();
      TDirectory* subdir = dir->GetDirectory(subname.c_str());
      if(!subdir) break;
      delete subdir;
      bins.push_back(*Binning::LoadFrom(dir, subname.c_str()));
      TObjString* label = (TObjString*)dir->Get(TString::Format("label%d", i));
      labels.push_back(label ? label->GetString().Data() : "");
    }

    for(int i = 0; ; ++i){
      const std::string subname = TString::Format("binsy%d", i).Data();
      TDirectory* subdir = dir->GetDirectory(subname.c_str());
      if(!subdir) break;
      delete subdir;
      binsy.push_back(*Binning::LoadFrom(dir, subname.c_str()));
      TObjString* labely = (TObjString*)dir->Get(TString::Format("labely%d", i));
      labelsy.push_back(labely ? labely->GetString().Data() : "");
    }

    // Backwards compatibility
    if(labelsy.empty()) labelsy.push_back(spect->GetYaxis()->GetTitle());
    if(binsy.empty()) binsy.push_back(Binning::FromTAxis(spect->GetYaxis()));

    const LabelsAndBins xax(labels, bins);
    const LabelsAndBins yax(labelsy, binsy);

    const FitMultiverse* multiverse = FitMultiverse::LoadFrom(dir, "multiverse");

    // ROOT histogram storage is row-major, but Eigen is column-major by
    // default
    typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen:: Dynamic, Eigen::RowMajor> MatRowMajor;
    std::unique_ptr<EnsembleReweightableSpectrum> ret(new EnsembleReweightableSpectrum(
      multiverse,
      Eigen::Map<MatRowMajor>(spect->GetArray(),
                              yax.GetBins1D().NBins()+2,
                              (xax.GetBins1D().NBins()+2) * multiverse->NUniv()),
      xax, yax, hPot->Integral(0, -1), hLivetime->Integral(0, -1)));

    delete spect;

    delete hPot;
    delete hLivetime;

    delete dir;

    return ret;
  }

  //----------------------------------------------------------------------
  void EnsembleReweightableSpectrum::CheckMultiverses(const FitMultiverse& rhs,
                                                      const std::string& func) const
  {
    if(&rhs == fMultiverse) return;

    std::cout << "EnsembleReweightableSpectrum::" << func << ": attempting to combine two spectra made with different multiverses: " << std::endl;
    std::cout << "  " << fMultiverse->ShortName() << std::endl;
    std::cout << "vs" << std::endl;
    std::cout << rhs.ShortName() << std::endl;
    abort();
  }
}