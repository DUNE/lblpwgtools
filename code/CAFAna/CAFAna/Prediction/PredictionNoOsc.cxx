#include "CAFAna/Prediction/PredictionNoOsc.h"

#include "CAFAna/Extrap/IExtrap.h"
#include "CAFAna/Core/LoadFromFile.h"

#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Extrap/TrivialExtrap.h"

#include "TDirectory.h"
#include "TObjString.h"


namespace ana
{
  //----------------------------------------------------------------------
  PredictionNoOsc::PredictionNoOsc(SpectrumLoaderBase& loader,
                                   const std::string& label,
                                   const Binning& bins,
                                   const Var& var,
                                   const Cut& cut,
                                   const SystShifts& shift,
                                   const Var& wei)
    : fSpectrum(label, bins, loader, var, cut, shift, wei)
  {
  }

  //----------------------------------------------------------------------
  PredictionNoOsc::PredictionNoOsc(SpectrumLoaderBase& loader,
                                   const HistAxis& axis,
                                   const Cut& cut,
                                   const SystShifts& shift,
                                   const Var& wei)
    : fSpectrum(loader, axis, cut, shift, wei)
  {
  }

  //----------------------------------------------------------------------
  void PredictionNoOsc::SaveTo(TDirectory* dir) const
  {
    TDirectory* tmp = gDirectory;

    dir->cd();

    TObjString("PredictionNoOsc").Write("type");

    fSpectrum.SaveTo(dir->mkdir("spect"));

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<PredictionNoOsc> PredictionNoOsc::LoadFrom(TDirectory* dir)
  {
    assert(dir->GetDirectory("spect"));
    std::unique_ptr<Spectrum> spect = ana::LoadFrom<Spectrum>(dir->GetDirectory("spect"));
    //    return std::make_unique<PredictionNoOsc>(*spect);
    return std::unique_ptr<PredictionNoOsc>(new PredictionNoOsc(*spect));
  }
}
