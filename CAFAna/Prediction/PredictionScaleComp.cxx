#include "CAFAna/Prediction/PredictionScaleComp.h"

#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Prediction/PredictionNoOsc.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"

#include "TDirectory.h"
#include "TObjString.h"
#include "TH1.h"
#include "TVectorD.h"

#include <cassert>

namespace ana
{
  //----------------------------------------------------------------------
  PredictionScaleComp::
  PredictionScaleComp(SpectrumLoaderBase& loader,
                      const HistAxis& axis,
                      Cut cut,
                      const std::vector<const SystComponentScale*>& systs,
                      const SystShifts& shift,
                      const Weight& wei)
    : fSysts(systs)
  {
    Cut complementCut = kNoCut;

    assert(!systs.empty() && "Please give at least one systematic.");
    for(const SystComponentScale* syst: systs){
      fPreds.push_back(new PredictionNoOsc(loader, axis,
                                           cut && syst->GetCut(), shift, wei));
    }

    fTotal = new PredictionNoOsc(loader, axis, cut, shift, wei);
  }

  //----------------------------------------------------------------------
  PredictionScaleComp::
  PredictionScaleComp(SpectrumLoaderBase& loaderNonswap,
                      SpectrumLoaderBase& loaderNue,
                      SpectrumLoaderBase& loaderNuTau,
                      const HistAxis&     axis,
                      Cut                 cut,
                      const std::vector<const SystComponentScale*>& systs,
                      const SystShifts&   shift,
                      const Weight&       wei)
    : fSysts(systs)
  {
    assert(!systs.empty() && "Please give at least one systematic.");
    for(const SystComponentScale* syst: systs){
      fPreds.push_back(new PredictionNoExtrap(loaderNonswap, loaderNue, loaderNuTau,
                                              axis, cut && syst->GetCut(), shift, wei));
    }

    fTotal = new PredictionNoExtrap(loaderNonswap, loaderNue, loaderNuTau,
                                    axis, cut, shift, wei);
  }

  //----------------------------------------------------------------------
  PredictionScaleComp::
  PredictionScaleComp(SpectrumLoaderBase& loader,
                      const HistAxis& axis1,
                      const HistAxis& axis2,
                      Cut cut,
                      const std::vector<const SystComponentScale*>& systs,
                      const SystShifts& shift,
                      const Weight& wei)
  {
    assert(0 && "unimplemented");

    // TODO TODO TODO
    /*
    assert(truthcuts.size()>0 && "Please give at least one truth selection.");
    for(unsigned int i = 0; i < truthcuts.size(); ++i){
      fPreds.push_back(new PredictionNoOsc(loader, axis1, axis2,
                                           cut && truthcuts[i], shift, wei));
      fSysts.push_back(new DummyScaleCompSyst(i));
      fComplementCut = fComplementCut && !truthcuts[i];
    }

    // The idea is that if truthcuts are exhaustive, this Spectrum should wind
    // up empty
    fComplement = new Spectrum(loader, axis1, axis2,
                               cut && fComplementCut, shift, wei);
    */
  }

  //----------------------------------------------------------------------
  PredictionScaleComp::
  PredictionScaleComp(const IPrediction* total,
                      const std::vector<const IPrediction*>& preds,
                      const std::vector<const SystComponentScale*>& systs)
    : fSysts(systs),
      fPreds(preds),
      fTotal(total)
  {
  }

  //----------------------------------------------------------------------
  PredictionScaleComp::~PredictionScaleComp()
  {
    for(const IPrediction* p: fPreds) delete p;
    delete fTotal;
  }

  //----------------------------------------------------------------------
  Spectrum PredictionScaleComp::PredictComponentSyst(osc::IOscCalc* calc,
                                                     const SystShifts& shift,
                                                     Flavors::Flavors_t flav,
                                                     Current::Current_t curr,
                                                     Sign::Sign_t sign) const
  {
    SystShifts shiftClean = shift;
    for(const ISyst* s: fSysts) shiftClean.SetShift(s, 0);

    // Starting with the total and adding in the differences for components
    // that need to change is faster if most of the knobs are set to zero.
    Spectrum ret = fTotal->PredictSyst(calc, shiftClean);

    for(unsigned int i = 0; i < fPreds.size(); ++i){
      const double x = shift.GetShift(fSysts[i]);
      if(x == 0) continue; // Nominal, can skip

      Spectrum si = fPreds[i]->PredictComponentSyst(calc, shiftClean,
                                                    flav, curr, sign);

      si.Scale(pow(1+fSysts[i]->OneSigmaScale(), x) - 1);

      ret += si;
    }

    return ret;
  }

  //----------------------------------------------------------------------
  Spectrum PredictionScaleComp::
  PredictCategory(osc::IOscCalc* osc,
                  const SystComponentScale* syst) const
  {
    for(unsigned int i = 0; i < fSysts.size(); ++i){
      if(fSysts[i] == syst) return fPreds[i]->Predict(osc);
    }

    std::cout << "PredictionScaleComp::PredictCategory(): Unknown systematic " << syst->ShortName() << std::endl;
    abort();
  }

  //----------------------------------------------------------------------
  void PredictionScaleComp::SaveTo(TDirectory* dir, const std::string& name) const
  {
    TDirectory* tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("PredictionScaleComp").Write("type");

    fTotal->SaveTo(dir, "total");

    for(unsigned int i = 0; i < fPreds.size(); ++i){
      fPreds[i]->SaveTo(dir, "pred"+std::to_string(i));
    }

    for(unsigned int i = 0; i < fSysts.size(); ++i){
      fSysts[i]->SaveTo(dir, "syst"+std::to_string(i));
    }

    dir->Write();
    delete dir;

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<PredictionScaleComp> PredictionScaleComp::LoadFrom(TDirectory* dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    IPrediction* total = ana::LoadFrom<IPrediction>(dir, "total").release();

    std::vector<const IPrediction*> preds;
    for(unsigned int i = 0; ; ++i){
      const std::string subname = "pred"+std::to_string(i);
      TDirectory* di = dir->GetDirectory(subname.c_str());
      if(!di) break; // We got all the predictions
      delete di;

      preds.push_back(ana::LoadFrom<IPrediction>(dir, subname).release());
    }

    std::vector<const SystComponentScale*> systs;
    for(unsigned int i = 0; ; ++i){
      const std::string subname = "syst"+std::to_string(i);
      TDirectory* si = dir->GetDirectory(subname.c_str());
      if(!si) break; // We got all the predictions
      delete si;

      systs.push_back(ana::LoadFrom<SystComponentScale>(dir, subname).release());
    }

    delete dir;

    return std::unique_ptr<PredictionScaleComp>(new PredictionScaleComp(total, preds, systs));
  }

}
