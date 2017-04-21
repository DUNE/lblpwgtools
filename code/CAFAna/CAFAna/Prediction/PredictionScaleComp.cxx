#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistCache.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Prediction/PredictionScaleComp.h"

#include "TDirectory.h"
#include "TObjString.h"
#include "TH1.h"
#include "TVectorD.h"

#include <cassert>

namespace ana
{
  namespace
  {
    // We need some way to identify what components the user wants to vary in
    // the fit. We just create N copies of this class, each one identifying one
    // component. They're distringuished by their pointer values.
    class DummyScaleCompSyst : public ana::ISyst
    {
    public:
      DummyScaleCompSyst(int i) : fN(i) {}

      std::set<std::string> Requires() const override { return {}; }

      std::string ShortName() const override
      {
        return "CompNormShift_" + std::to_string(fN);
      }

      std::string LatexName() const override
      {
        return "Component Normalization Shift " + std::to_string(fN);
      }

      void Shift(double sigma, ana::Restorer&, caf::StandardRecord* sr,
                 double& weight) const override
      {
      }
    private:
      int fN;
    };
  }

  //----------------------------------------------------------------------
  PredictionScaleComp::PredictionScaleComp(SpectrumLoaderBase& loader,
                                           const HistAxis& axis,
                                           Cut cut,
                                           std::vector<Cut> truthcuts,
                                           const SystShifts& shift,
                                           const Var& wei)
    : fComplementCut(kNoCut)
  {
    assert(truthcuts.size()>0 && "Please give at least one truth selection.");
    for (int i = 0; i < int(truthcuts.size()); i++){
      fSpectra.push_back(new Spectrum(loader, axis,
                                      cut && truthcuts[i], shift, wei));
      fSysts.push_back(new DummyScaleCompSyst(i));
      fComplementCut = fComplementCut && !truthcuts[i];
    }

    // The idea is that if truthcuts are exhaustive, this Spectrum should wind
    // up empty
    fComplement = new Spectrum(loader, axis,
                               cut && fComplementCut, shift, wei);
  }

  //----------------------------------------------------------------------
  PredictionScaleComp::PredictionScaleComp(SpectrumLoaderBase& loader,
                                           const HistAxis& axis1,
                                           const HistAxis& axis2,
                                           Cut cut,
                                           std::vector<Cut> truthcuts,
                                           const SystShifts& shift,
                                           const Var& wei)
    : fComplementCut(kNoCut)
  {
    assert(truthcuts.size()>0 && "Please give at least one truth selection.");
    for (int i = 0; i < int(truthcuts.size()); i++){
      fSpectra.push_back(new Spectrum(loader, axis1, axis2,
                                      cut && truthcuts[i], shift, wei));
      fSysts.push_back(new DummyScaleCompSyst(i));
      fComplementCut = fComplementCut && !truthcuts[i];
    }

    // The idea is that if truthcuts are exhaustive, this Spectrum should wind
    // up empty
    fComplement = new Spectrum(loader, axis1, axis2,
                               cut && fComplementCut, shift, wei);
  }

  //----------------------------------------------------------------------
  PredictionScaleComp::PredictionScaleComp(Spectrum*              complement,
                                           std::vector<Spectrum*> spectra)
    : fSpectra(spectra),
      fComplementCut(kNoCut),
      fComplement(complement)
  {
    for (int i = 0; i < int(spectra.size()); i++)
      fSysts.push_back(new DummyScaleCompSyst(i));
  }

  //----------------------------------------------------------------------
  Spectrum PredictionScaleComp::Predict(osc::IOscCalculator* calc) const
  {
    Spectrum ret = *fSpectra[0];
    for (int i = 1; i < int(fSpectra.size()); i++) ret += *fSpectra[i];
    return ret;
  }

  //----------------------------------------------------------------------
  Spectrum PredictionScaleComp::PredictSyst(osc::IOscCalculator* calc,
                                            const SystShifts&    shift) const
  {
    double pot = fSpectra[0]->POT();
    std::unique_ptr<TH1D> hist(fSpectra[0]->ToTH1(pot));
    hist->Scale(1 + shift.GetShift(fSysts[0]));
    for (int i = 1; i < int(fSpectra.size()); i++){
      TH1D* curhist = (TH1D*)fSpectra[i]->ToTH1(pot);
      hist->Add(curhist, 1 + shift.GetShift(fSysts[i]));
      HistCache::Delete(curhist);
    }
    return Spectrum(std::move(hist), fSpectra[0]->GetLabels(), fSpectra[0]->GetBinnings(), pot, fSpectra[0]->Livetime());
  }

  //----------------------------------------------------------------------
  void PredictionScaleComp::SaveTo(TDirectory* dir) const
  {
    TDirectory* tmp = gDirectory;
    dir->cd();

    TObjString("PredictionScaleComp").Write("type");

    // Save the number of MC spectra as an entry in a dummy histogram;
    TVectorD nspectra(1);
    nspectra[0] = fSpectra.size();
    nspectra.Write("NSpectra");
    // Save Vector of Spectrum*'s
    for (int i = 0; i < int(fSpectra.size()); i++)
      fSpectra[i]->SaveTo(dir->mkdir(("Spectrum_"+std::to_string(i)).c_str()));
    fComplement->SaveTo(dir->mkdir("Complement"));

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<PredictionScaleComp> PredictionScaleComp::LoadFrom(TDirectory* dir)
  {
    Spectrum* complement = ana::LoadFrom<Spectrum>
      (dir->GetDirectory("Complement")).release();

    // Get number of spectra we expect
    TVectorD* nspectra = (TVectorD*)dir->Get("NSpectra");
    assert(nspectra->GetNoElements()==1 && "Vector of N MC Bkgds is not of size 1.");
    // Load spectra
    std::vector<Spectrum*> spectra;
    for (int i = 0; i < (*nspectra)[0]; i++)
      spectra.push_back(ana::LoadFrom<Spectrum>(
        dir->GetDirectory(("Spectrum_"+std::to_string(i)).c_str())).release());
    delete nspectra;
    return std::unique_ptr<PredictionScaleComp>(
      new PredictionScaleComp(complement, spectra));
  }

}
