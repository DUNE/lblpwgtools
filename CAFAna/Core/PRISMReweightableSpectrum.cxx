#include "CAFAna/Core/PRISMReweightableSpectrum.h"
#include "CAFAna/Core/Weight.h"

#include "TDirectory.h"
#include "TH2.h"
#include "TObjString.h"

#include <iostream>

namespace ana {
 
  //-----------------------------------------------------------------------------
  PRISMReweightableSpectrum::PRISMReweightableSpectrum(SpectrumLoaderBase& loader,
                                                       const HistAxis& recoAxis,
                                                       const HistAxis& trueAxis,
                                                       const Cut& cut,
                                                       const SystShifts& shift,
                                                       const Weight& wei) :
                            ReweightableSpectrum(loader, recoAxis, trueAxis,
                                                 cut, shift, wei)
  {
    fMatSumSq = Eigen::MatrixXd();
  }      

  //-----------------------------------------------------------------------------
  PRISMReweightableSpectrum::PRISMReweightableSpectrum(const Eigen::MatrixXd&& mat,
                                                       const Eigen::MatrixXd&& err_mat,
                                                       const LabelsAndBins& recoAxis,
                                                       const LabelsAndBins& trueAxis,
                                                       double pot, double livetime) :
                            ReweightableSpectrum(std::move(mat), recoAxis, 
                                                 trueAxis, pot, livetime) 
  {
    fMatSumSq = Eigen::MatrixXd(std::move(err_mat));
  }

  //-----------------------------------------------------------------------------
  Spectrum PRISMReweightableSpectrum::WeightedByErrors(Eigen::ArrayXd &w_arr) const {

    Eigen::VectorXd w_vec = w_arr.matrix();
    Eigen::VectorXd w_vec_sq = (w_arr * w_arr).matrix();
    
    return Spectrum(Eigen::ArrayXd(w_vec.transpose() * fMat),
                    Eigen::ArrayXd(w_vec_sq.transpose() * fMatSumSq), 
                    fAxisX, fPOT, fLivetime);
  }

  //-----------------------------------------------------------------------------
  // Reimplementing this upsets me, but it will have to do for now.
  PRISMReweightableSpectrum& PRISMReweightableSpectrum::
    PlusEqualsHelper(const PRISMReweightableSpectrum& rhs, int sign)
  {
    if(rhs.fMat.sum() == 0) return *this;
    std::cout << "Adding or subtracting PRISMRWSpec!" << std::endl;

    if((!fPOT && !fLivetime) || (!rhs.fPOT && !rhs.fLivetime)){
      std::cout << "Error: can't sum PRISMReweightableSpectrum with no POT or livetime."
                << fPOT << " " << rhs.fPOT
                << std::endl;
      return *this;
    }

    if(!fLivetime && !rhs.fPOT){
      std::cout << "Error: can't sum PRISMReweightableSpectrum with POT ("
                << fPOT << ") but no livetime and ReweightableSpectrum with livetime ("
                << rhs.fLivetime << " sec) but no POT." << std::endl;
      abort();
    }

    if(!fPOT && !rhs.fLivetime){
      std::cout << "Error: can't sum PRISMReweightableSpectrum with livetime ("
                << fLivetime << " sec) but no POT and ReweightableSpectrum with POT ("
                << rhs.fPOT << ") but no livetime." << std::endl;
      abort();
    }

    if(fPOT && rhs.fPOT){
      // Scale by POT when possible
      fMat += rhs.fMat * sign*fPOT/rhs.fPOT;
      fMatSumSq += rhs.fMatSumSq * sign*fPOT/rhs.fPOT;

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
      fMatSumSq += rhs.fMatSumSq * sign*fLivetime/rhs.fLivetime;

      if(!fPOT && rhs.fPOT){
        // If the RHS has a POT and we don't, copy it in (suitably scaled)
                fPOT = rhs.fPOT * fLivetime/rhs.fLivetime;
      }
      // Otherwise, keep our own POT (if any)
            return *this;
    }

    // That should have been all the cases. I definitely want to know what
    // happened if it wasn't.    
    std::cout << "PRISMReweightableSpectrum::operator+=(). How did we get here? "
              << fPOT << " " << fLivetime << " "
              << rhs.fPOT << " " << rhs.fLivetime << std::endl;
    abort();
  }

  //-----------------------------------------------------------------------------
  PRISMReweightableSpectrum& PRISMReweightableSpectrum::
    operator+=(const PRISMReweightableSpectrum& rhs)
  {
    return PlusEqualsHelper(rhs, +1);
  }

  //-----------------------------------------------------------------------------
  PRISMReweightableSpectrum PRISMReweightableSpectrum::
    operator+(const PRISMReweightableSpectrum& rhs) const
  {
    PRISMReweightableSpectrum ret = *this;
    ret += rhs;
    return ret;
  }

  //-----------------------------------------------------------------------------
  PRISMReweightableSpectrum& PRISMReweightableSpectrum::
    operator-=(const PRISMReweightableSpectrum& rhs)
  {
    return PlusEqualsHelper(rhs, -1);
  }

  //-----------------------------------------------------------------------------
  PRISMReweightableSpectrum PRISMReweightableSpectrum::
    operator-(const PRISMReweightableSpectrum& rhs) const
  {
    PRISMReweightableSpectrum ret = *this;
    ret -= rhs;
    return ret;
  }

  //------------------------------------------------------------------------------
  std::unique_ptr<PRISMReweightableSpectrum> 
      PRISMReweightableSpectrum::LoadFrom(TDirectory* dir, const std::string& name) {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);
    TObjString* tag = (TObjString*)dir->Get("type");
    assert(tag);
    assert(tag->GetString() == "ReweightableSpectrum");
    delete tag;

    TH2D* spect = (TH2D*)dir->Get("hist");
    assert(spect);
    // Hist for SumSq matrix
    TH2D *spect_err = (TH2D*)spect->Clone("hist_err");
    spect_err->Clear();
    for (int x = 1; x <= spect_err->GetXaxis()->GetNbins(); x++) {
      for (int y = 1; y <= spect_err->GetYaxis()->GetNbins(); y++) {
        spect_err->SetBinContent(x, y, std::pow(spect->GetBinError(x, y), 2));
      }
    }

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
    
    // ROOT histogram storage is row-major, but Eigen is column-major by
    // default
    typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen:: Dynamic, Eigen::RowMajor> MatRowMajor;
    auto ret = std::make_unique<PRISMReweightableSpectrum>(
    Eigen::Map<MatRowMajor>(spect->GetArray(),
                            yax.GetBins1D().NBins()+2,
                            xax.GetBins1D().NBins()+2),
    Eigen::Map<MatRowMajor>(spect_err->GetArray(),
                            yax.GetBins1D().NBins()+2, 
                            xax.GetBins1D().NBins()+2),
                            xax, yax, hPot->Integral(0, -1), hLivetime->Integral(0, -1));
     
    delete spect;
    delete hPot;
    delete hLivetime;
     
    delete dir;
    return ret;
  }
}
