#include "CAFAna/Prediction/PredictionNoExtrap.h"

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/LoadFromRegistry.h"

#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Extrap/TrivialExtrap.h"

#include "TDirectory.h"
#include "TObjString.h"


namespace ana
{
  REGISTER_LOADFROM("PredictionNoExtrap", IPrediction, PredictionNoExtrap);

  //----------------------------------------------------------------------
  PredictionNoExtrap::PredictionNoExtrap(IInteractionSource& srcNonswap,
                                         IInteractionSource& srcNue,
                                         IInteractionSource& srcNuTau,
                                         const HistAxis& axis)
    : PredictionExtrap(new TrivialExtrap(srcNonswap, srcNue, srcNuTau, axis))
  {
  }

  //----------------------------------------------------------------------
  PredictionNoExtrap::PredictionNoExtrap(PredictionExtrap* pred) : PredictionExtrap(pred->GetExtrap())
  {
  }

  /*
  //----------------------------------------------------------------------
  PredictionNoExtrap::PredictionNoExtrap(Loaders& loaders,
                                         const HistAxis& axis,
                                         const Cut& cut,
                                         const SystShifts& shift,
                                         const Weight& wei)
    : PredictionExtrap(new TrivialExtrap(loaders, axis, cut, shift, wei))
  {
  }

  //----------------------------------------------------------------------
  void PredictionNoExtrap::SaveTo(TDirectory* dir, const std::string& name) const
  {
    TDirectory* tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("PredictionNoExtrap").Write("type");

    fExtrap->SaveTo(dir, "extrap");

    dir->Write();
    delete dir;

    tmp->cd();
  }


  //----------------------------------------------------------------------
  std::unique_ptr<PredictionNoExtrap> PredictionNoExtrap::LoadFrom(TDirectory* dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    IExtrap* extrap = ana::LoadFrom<IExtrap>(dir, "extrap").release();
    PredictionExtrap* pred = new PredictionExtrap(extrap);

    delete dir;

    return std::unique_ptr<PredictionNoExtrap>(new PredictionNoExtrap(pred));
  }


  //----------------------------------------------------------------------
  PredictionNoExtrap::~PredictionNoExtrap()
  {
    // We created this in the constructor so it's our responsibility
    delete fExtrap;
  }


  //--------------------------------------------------------------------------
  NoExtrapGenerator::NoExtrapGenerator(
    const HistAxis axis,
    const Cut cut,
    const Weight wei
  ) : fAxis(axis), fCut(cut), fWei(wei) {}

  //--------------------------------------------------------------------------
  std::unique_ptr<IPrediction> NoExtrapGenerator::Generate(
    Loaders& loaders,
    const ana::RecoType & ixnRecoType, const SystShifts& shiftMC
  ) const {
    return std::make_unique<PredictionNoExtrap>(loaders, fAxis, fCut, ixnRecoType, shiftMC, fWei );
  }

}


